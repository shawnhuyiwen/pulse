# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import sys
import logging
from pathlib import Path
from typing import Dict, List, Optional, Set


_pulse_logger = logging.getLogger('pulse')


def process_file(fpath: Path, t_dir: Path, d_dir: Path, replace_refs: bool=False, ancestors: Optional[Set[Path]]=None) -> None:
    if ancestors is None:
        ancestors = set()

    out_fname = d_dir.resolve() / fpath.stem
    out_fname = out_fname.with_suffix(out_fname.suffix+".md")

    def _process_references(lines: List[str]) -> List[str]:
        table_refs = {}
        fig_refs = {}

        # Build up references
        def _parse_refs(line: str, tag: str) -> List[str]:
            ref_defs = []
            words = line.split()

            for idx, word in enumerate(words):
                if tag in word:
                    ref_defs.append(words[idx+1])

            return ref_defs
        table_num = 1
        fig_num = 1
        for line in lines:
            if "*@tabledef" in line:
                for ref_def in _parse_refs(line, "*@tabledef"):
                    if ref_def in table_refs:
                        _pulse_logger.warning(f"Redefinition of reference: {ref_def}")
                    table_refs[ref_def] = table_num
                    table_num += 1
            if "*@figuredef" in line:
                for ref_def in _parse_refs(line, "*@figuredef"):
                    if ref_def in fig_refs:
                        _pulse_logger.warning(f"Redefinition of reference: {ref_def}")
                    fig_refs[ref_def] = fig_num
                    fig_num += 1

        # Replace references
        def _replace_refs(line: str, tag: str, replacement: str, ref_defs: Dict[str, int]) -> str:
            if not line:
                return ""

            words = line.split()

            for idx, word in enumerate(words):
                if tag in word:
                    words[idx] = word.replace(tag, f"{replacement} {ref_defs[words[idx+1]]}.")
                    words[idx + 1] = ""
            words[:] = [word for word in words if word]

            return " ".join(words)
        for idx, line in enumerate(lines):
            if "@tabledef" in line:
                lines[idx] = _replace_refs(line, "@tabledef", "Table", table_refs)
            if "@tableref" in line:
                lines[idx] = _replace_refs(line, "@tableref", "Table", table_refs)
            if "@figuredef" in line:
                lines[idx] = _replace_refs(line, "@figuredef", "Figure", fig_refs)
            if "@figureref" in line:
                lines[idx] = _replace_refs(line, "@figureref", "Figure", fig_refs)

        return lines

    def _process_file(fpath: Path, ancestors: Set[Path]) -> List[str]:
        _pulse_logger.info(f"Processing file: {fpath}")
        if fpath.resolve() in ancestors:
            raise RuntimeError(f"Circular insert involving {fpath} detected. Aborting.")
        ancestors.add(fpath.resolve())

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
            if "@insert" in line:
                stripped_line = line.strip()
                i_name = stripped_line[stripped_line.find(" "):].strip()
                f = Path(i_name)
                _pulse_logger.info(f"Inserting {i_name}")
                if not f.exists():
                    # Try to process this file so it is in the dst directory
                    inserting = f.name
                    f = t_dir.resolve() / inserting
                    process_file(f, t_dir, d_dir, replace_refs=False, ancestors=ancestors.copy())
                    i_out_fname = d_dir.resolve() / f.stem
                    i_out_fname = i_out_fname.with_suffix(i_out_fname.suffix+".md")
                    with open(i_out_fname, "r") as i_out:
                        i_lines = i_out.readlines()
                        out_lines.extend(i_lines)
                else:
                    out_lines.extend(_process_file(f, ancestors.copy()))
            else:
                out_lines.append(line)
        return out_lines

    out_lines = _process_file(fpath, ancestors.copy())
    if replace_refs:
        out_lines = _process_references(out_lines)

    with open(out_fname, 'w') as out_file:
        out_file.writelines(out_lines)


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
                    process_file(f, t_dir, d_dir, replace_refs=True)
            else:
                _pulse_logger.error(f"Cannot find source directory: {sys.argv[1]}")
        else:
            _pulse_logger.error(
                "Command arguments are: <Directory of file to process> " \
                "<Directory to place processed files> [Directory where to find tables for insert tags]"
        )
    except Exception as e:
        _pulse_logger.error(e)
