# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import sys
import logging
import argparse
import matplotlib.pyplot as plt
from pathlib import Path
from typing import Optional

from pulse.cdm.utils.doxygen import process_file
from pulse.cdm.utils.file_utils import get_root_dir, get_validation_dir
from pulse.cdm.utils.plotter import create_plots
from pulse.dataset.segment_dataset_reader import load_data
from pulse.dataset.segment_validation import validate
from pulse.engine.PulseScenarioExec import PulseScenarioExec


_pulse_logger = logging.getLogger('pulse')


def segment_validation_pipeline(xls_file: Path, doc_dir: Path, gen_monitors: bool=False,
                                run_scenarios: bool=False, use_baseline: bool=False
) -> None:
    xls_basename = "".join(xls_file.name.rsplit("".join(xls_file.suffixes), 1))

    # Create scenario and validation target files from xls file
    # TODO: Do we need to regenerate this if we're not runnning scenarios?
    validation_dir, results_dir = load_data(xls_file)

    # Get list of all scenarios
    scenarios = [item.name for item in validation_dir.glob("*") if item.is_dir()]

    if use_baseline:
        results_dir = Path(*["verification" if part == "test_results" else part for part in results_dir.parts])
    # Run scenarios if needed
    elif run_scenarios:
        for scenario in scenarios:
            scenario_file = validation_dir / scenario / f"{scenario}.json"
            sce_exec = PulseScenarioExec()
            sce_exec.set_scenario_filename(scenario_file.as_posix())
            if not sce_exec.execute_scenario():
                _pulse_logger.warning("Scenario {scenario} was not sucessfully run.")

    if not results_dir.is_dir():
        _pulse_logger.error(f"Results directory ({results_dir}) does not exist. Aborting")
        return

    # Carry out validation on each scenario
    for scenario in scenarios:
        sce_val_dir = validation_dir / scenario
        sce_res_dir = results_dir / scenario
        if not sce_res_dir.is_dir():
            _pulse_logger.error(f"Unable to locate results for {scenario}. Continuing without validating.")
            continue

        monitors_dir = None
        if gen_monitors:
            monitors_dir = Path("./docs/html/plots") / xls_basename
        validate(sce_val_dir, sce_res_dir, md_dir=results_dir, monitors_dir=monitors_dir)

    # TODO: Always generate plots?
    plot_file = doc_dir / f"{xls_basename}.json"
    if not plot_file.is_file():
        _pulse_logger.info(f"Plot file ({plot_file}) does not seem to exist. Skipping plot generation.")
    else:
        # TODO: Change results file if needed?
        create_plots(plot_file)

    # Run doxygen preprocessor
    md_template = doc_dir / f"{xls_basename}.md"
    if not md_template.is_file():
        _pulse_logger.error(f"Markdown template does not seem to exist: {md_template}")
        return
    process_file(
        fpath=md_template,
        t_dir=results_dir,
        d_dir=Path("./docs/markdown"),
        replace_refs=True
    )

    # TODO: Run doxygen? On/with what?


if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG, format='%(levelname)s: %(message)s')
    logging.getLogger('PIL').setLevel(logging.INFO)
    plt.set_loglevel("info")

    parser = argparse.ArgumentParser(description="Process the full pipeline for segment validation")
    parser.add_argument(
        "xls_file",
        type=Path,
        help="xls file to process during this pipeline run"
    )
    parser.add_argument(
        "-m", "--gen-monitors",
        action="store_true",
        help="whether to generate monitor images"
    )
    parser.add_argument(
        "-d", "--doc-dir",
        type=Path,
        default=Path(get_root_dir()) / "docs" / "Validation",
        help="directory where the markdown file and optional plot file can be found (default: %(default)s)"
    )
    group = parser.add_mutually_exclusive_group()
    group.add_argument(
        "-b", "--use-baseline",
        action="store_true",
        help="use results in verification directory instead of test_results"
    )
    group.add_argument(
        "-r", "--run-scenarios",
        action="store_true",
        help="whether to run scenarios or use existing results (cannot be used if using baseline results)"
    )
    opts = parser.parse_args()

    xls_file = opts.xls_file
    if not xls_file.is_file():
        xls_file = Path(get_validation_dir()) / xls_file
        if not xls_file.is_file():
            _pulse_logger.error("Please provide a valid xls file")
            sys.exit(1)

    doc_dir = opts.doc_dir
    if not doc_dir.is_dir():
        _pulse_logger.error(f"Please provide a valid documents directory: {doc_dir}")
        sys.exit(1)

    segment_validation_pipeline(
        xls_file=xls_file,
        doc_dir=doc_dir,
        gen_monitors=opts.gen_monitors,
        run_scenarios=opts.run_scenarios,
        use_baseline=opts.use_baseline
    )
