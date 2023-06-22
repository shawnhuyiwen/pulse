# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import sys
import logging
from typing import List
from pathlib import Path


_pulse_logger = logging.getLogger('pulse')


def process_file(fpath: Path, t_dir: Path, d_dir: Path) -> None:
    out_fname = d_dir.resolve() / fpath.stem
    out_fname = out_fname.with_suffix(out_fname.suffix+".md")

    processed_files = set()

    def _process_file(fpath: Path) -> List[str]:
        _pulse_logger.info(f"Processing file: {fpath}")
        if fpath.resolve() in processed_files:
            raise RuntimeError(f"Circular insert involving {fpath} detected. Aborting.")
        processed_files.add(fpath.resolve())

        try:
            in_file = open(fpath, 'r')
            lines = in_file.readlines()
            in_file.close()
        except Exception as ex:
            _pulse_logger.error(f"Could not find {fpath.resolve()}")
            # TODO: What happens if you can't find a file
            return [f'<img src="./images/MissingTable.jpg"><center><i>Could not find "{fpath}"</i></center><br>\n']

        out_lines = []
        for line in lines:
            if line.find("@insert") > -1:
                i_name = line[line.find(" "):].strip()
                f = Path(i_name)
                _pulse_logger.info(f"Inserting {i_name}")
                if not f.exists():
                    # Try to process this file so it is in the dst directory
                    inserting = f.name
                    f = t_dir.resolve() / inserting
                    process_file(f, t_dir, d_dir)
                    i_out_fname = d_dir.resolve() / f.stem
                    i_out_fname = i_out_fname.with_suffix(i_out_fname.suffix+".md")
                    with open(i_out_fname, "r") as i_out:
                        i_lines = i_out.readline()
                        out_lines.extend(i_lines)
                else:
                    out_lines.extend(_process_file(f))
            else:
                out_lines.append(line)
        return out_lines

    with open(out_fname, 'w') as out_file:
        out_file.writelines(_process_file(fpath))


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO, format='%(levelname)s: %(message)s')

    try:
        if len(sys.argv) == 3 or len(sys.argv) == 4:
            s_dir = Path(sys.argv[1])
            d_dir = Path(sys.argv[2])
            if s_dir.is_dir():
                d_dir.mkdir(parents=True, exist_ok=True)
                if len(sys.argv) == 4:
                    t_dir = Path(sys.argv[3])
                else:
                    t_dir = s_dir

                found = list(s_dir.rglob("*.[mM][dD]"))
                for f in found:
                    if f.is_dir():
                        continue # Not currently recursively processing directories
                    process_file(f, t_dir, d_dir)
            else:
                _pulse_logger.error(f"Cannot find source directory: {sys.argv[1]}")
        else:
            _pulse_logger.error(
                "Command arguments are: <Directory of file to process> " \
                "<Directory to place processed files> [Directory where to find tables for insert tags]"
        )
    except Exception as e:
        _pulse_logger.error("Unable to create single validation table file.")
        _pulse_logger.error(e)
