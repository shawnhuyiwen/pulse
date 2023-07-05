# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import sys
import logging
import numpy as np
from pathlib import Path
from typing import Dict, List, Optional

import PyPulse
from pulse.cdm.engine import SESegmentValidationTarget, SESegmentValidationTargetSegment
from pulse.cdm.utils.markdown import table
from pulse.cdm.utils.math_utils import generate_percentage_span, percent_change, percent_difference
from pulse.cdm.io.engine import serialize_segment_validation_target_segment_from_file, \
                                serialize_data_requested_result_from_file


_pulse_logger = logging.getLogger('pulse')


def validate(targets_dir: Path, results_dir: Path, md_dir: Optional[Path]=None) -> None:
    # Get all validation targets from files
    val_segments = {}
    prefix = "Segment"
    postfix = "ValidationTargets.json"
    for filename in targets_dir.glob(f"{prefix}*{postfix}"):
        seg_id = int(filename.name[len(prefix):-len(postfix)])

        val_segments[seg_id] = serialize_segment_validation_target_segment_from_file(filename)

    # Get all results from files
    results = {}
    prefix = "Segment"
    postfix = ".json"
    for filename in results_dir.glob(f"{prefix}*{postfix}"):
        seg_id = int(filename.name[len(prefix):-len(postfix)])

        drr = serialize_data_requested_result_from_file(filename)
        # In this case the DRR will only have one timestep, so get values from there
        drr_values = drr.segments_per_sim_time_s[list(drr.segments_per_sim_time_s.keys())[0]]
        results[seg_id] = {
            header: value for header, value in zip(drr.headers, drr_values)
        }

    scenario_name = targets_dir.name
    if md_dir is None:
        md_dir = results_dir / "docs" / "markdown"
    md_dir.mkdir(parents=True, exist_ok=True)

    # Evaluate targets and create markdown tables for each segment
    headers = ["Property Name", "Validation", "Engine Value", "Percent Error", "Percent Change", "Notes"]
    fields = list(range(len(headers)))
    align = [('<', '<')] * len(headers)
    for seg_id in sorted(val_segments.keys()):
        md_filename = md_dir / f"{scenario_name}-Segment{seg_id}Validation.md"
        table_data = []

        val_segment = val_segments[seg_id]
        for tgt in val_segment.get_validation_targets():
            table_data.append(evaluate(seg_id, tgt, results))

        # Only write file if we have data
        if val_segment.has_validation_targets():
            with open(md_filename, "w") as md_file:
                _pulse_logger.info(f"Writing {md_filename}")

                if val_segment.has_notes():
                    md_file.writelines(
                        f"<center>\n*@tabledef {scenario_name}Segment{seg_id} {val_segment.get_notes().rstrip()}*\n</center>\n\n"
                    )

                table(
                    md_file,
                    table_data,
                    fields,
                    headers,
                    align
                )


def evaluate(seg_id: int, tgt: SESegmentValidationTarget, results: Dict[int, Dict[str, float]]) -> List[str]:
    header = tgt.get_header()
    compare_type = tgt.get_comparison_type()

    epsilon = 1E-9
    percent_precision = 1
    value_precision = 4
    expected_str = ""
    err_str = ""
    change_str = ""

    if seg_id not in results:
        raise Exception(f"Segment {seg_id} not found in results")
    if header not in results[seg_id]:
        # Check for a different unit
        engine_val = None
        paren_idx = header.find("(")
        if paren_idx != -1:
            unitless_header = header[:paren_idx]
            val_unit = header[paren_idx+1:-1]
            for key in results[seg_id].keys():
                if key.startswith(unitless_header):
                    engine_unit = key[key.find("(")+1:-1]
                    engine_val = PyPulse.convert(results[seg_id][key], engine_unit, val_unit)
                    break

        if engine_val is None:
            raise Exception(f'{header} not found in Segment {seg_id} results')
    else:
        engine_val = results[seg_id][header]

    if compare_type == SESegmentValidationTarget.eComparisonType.EqualToSegment or \
        compare_type == SESegmentValidationTarget.eComparisonType.EqualToValue:

        if compare_type == SESegmentValidationTarget.eComparisonType.EqualToSegment:
            if tgt.get_target_segment() not in results:
                raise Exception(f"Segment {tgt.get_target_segment()} not found in results")
            if header not in results[tgt.get_target_segment()]:
                raise Exception(f'{header} not found in Segment {tgt.get_target_segment()} results')
            expected_val = results[tgt.get_target_segment()][header]
            expected_str = f"({expected_val:.{value_precision}G})"
        else:
            expected_val = tgt.get_target()
            expected_str = f"{expected_val:.{value_precision}G}"

        err = percent_difference(expected_val, engine_val, epsilon)

        # Close enough
        if abs(err) < epsilon:
            err = 0.

        err_str = generate_percentage_span(err, percent_precision)
    elif compare_type == SESegmentValidationTarget.eComparisonType.GreaterThanSegment or \
        compare_type == SESegmentValidationTarget.eComparisonType.GreaterThanValue:

        if compare_type == SESegmentValidationTarget.eComparisonType.GreaterThanSegment:
            if tgt.get_target_segment() not in results:
                raise Exception(f"Segment {tgt.get_target_segment()} not found in results")
            if header not in results[tgt.get_target_segment()]:
                raise Exception(f'{header} not found in Segment {tgt.get_target_segment()} results')
            expected_val = results[tgt.get_target_segment()][header]
            expected_str = f"({expected_val:.{value_precision}G})"
        else:
            expected_val = tgt.get_target()
            expected_str = f"{expected_val:.{value_precision}G}"

        # TODO: Implement gradient for greater than?
        change = percent_change(expected_val, engine_val, epsilon)
        c = '"danger"'
        if not np.isnan(change) and change > 0.0:
            c='"success"'
        change_str = f'<span class={c}>{change:.{percent_precision}f}%</span>'
    elif compare_type == SESegmentValidationTarget.eComparisonType.LessThanSegment or \
        compare_type == SESegmentValidationTarget.eComparisonType.LessThanValue:

        if compare_type == SESegmentValidationTarget.eComparisonType.LessThanSegment:
            if tgt.get_target_segment() not in results:
                raise Exception(f"Segment {tgt.get_target_segment()} not found in results")
            if header not in results[tgt.get_target_segment()]:
                raise Exception(f'{header} not found in Segment {tgt.get_target_segment()} results')
            expected_val = results[tgt.get_target_segment()][header]
            expected_str = f"({expected_val:.{value_precision}G})"
        else:
            expected_val = tgt.get_target()
            expected_str = f"{expected_val:.{value_precision}G}"

        # TODO: Implement gradient for less than?
        change = percent_change(expected_val, engine_val, epsilon)
        c = '"danger"'
        if not np.isnan(change) and change < 0.0:
            c='"success"'
        change_str = f'<span class={c}>{change:.{percent_precision}f}%</span>'
    elif compare_type == SESegmentValidationTarget.eComparisonType.TrendsToSegment or \
        compare_type == SESegmentValidationTarget.eComparisonType.TrendsToValue:

        if compare_type == SESegmentValidationTarget.eComparisonType.TrendsToSegment:
            if tgt.get_target_segment() not in results:
                raise Exception(f"Segment {tgt.get_target_segment()} not found in results")
            if header not in results[tgt.get_target_segment()]:
                raise Exception(f'{header} not found in Segment {tgt.get_target_segment()} results')
            expected_val = results[tgt.get_target_segment()][header]
            expected_str = f"({expected_val:.{value_precision}G})"
        else:
            expected_val = tgt.get_target()
            expected_str = f"{expected_val:.{value_precision}G}"

        # Get engine values at beginning (end of last) and end of current segment
        prev_seg_id = seg_id - 1
        if prev_seg_id not in results or header not in results[prev_seg_id]:
            raise Exception(f"Unable to find segment {seg_id} initial value for trends towards comparison for {header}.")
        init_val = results[prev_seg_id][header]

        init_change = percent_change(expected_val, init_val, epsilon)
        end_change = percent_change(expected_val, engine_val, epsilon)

        # TODO: gradient for trends to?
        c = '"danger"'
        # Check that difference between expected and engine decreased and both
        # percentages are the same sign
        if not np.isnan(init_change) and not np.isnan(end_change) and \
             abs(end_change) < abs(init_change) and init_change * end_change > 0.0:
            c='"success"'
        # TODO: what percent change value is valuable to display?
        change = percent_change(init_val, engine_val, epsilon)
        change_str = f'<span class={c}>{change:.{percent_precision}f}%</span>'
    elif compare_type == SESegmentValidationTarget.eComparisonType.Range:
        tgt_min = tgt.get_target_minimum()
        tgt_max = tgt.get_target_maximum()
        expected_str = f"[{tgt_min:.{value_precision}G},{tgt_max:.{value_precision}G}]"
        min_err = percent_difference(tgt_min, engine_val, epsilon)
        max_err = percent_difference(tgt_max, engine_val, epsilon)

        # No error if we are in range
        if engine_val >= tgt_min and engine_val <= tgt_max:
            err = 0.
        elif engine_val > tgt_max:
            err = max_err
        elif engine_val < tgt_min:
            err = min_err

        # Close enough
        if abs(err) < epsilon:
            err = 0.

        err_str = generate_percentage_span(err, percent_precision)
    elif compare_type == SESegmentValidationTarget.eComparisonType.NotValidating:
        pass
    else:
        raise ValueError(f"Unknown compare type: {compare_type}")

    if tgt.get_reference():
        references = [ref.strip() for ref in tgt.get_reference().replace("\n", "").split(",")]
        for ref in references:
            expected_str += f" @cite {ref}"

    # Add comparison type to beginning of expected string
    if "Segment" in compare_type.name:
        compare_str = compare_type.name.replace("Segment", "")
        tgt_seg = tgt.get_target_segment()
        if tgt_seg == 0:
            expected_str = f'{compare_str} Baseline {expected_str}'
        else:
            expected_str = f'{compare_str} Segment {tgt_seg} {expected_str}'
    elif "Value" in compare_type.name:
        expected_str = f'{compare_type.name.replace("Value", "")} {expected_str}'

    return [
        header,
        expected_str if expected_str else "&nbsp;",
        f"{engine_val:.{value_precision}G}",
        err_str if err_str else "&nbsp;",
        change_str if change_str else "&nbsp;",
        tgt.get_notes() if tgt.get_notes() else "&nbsp;"
        ]


if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG, format='%(levelname)s: %(message)s')

    targets_dir = None
    results_dir = None

    if len(sys.argv) < 3:
        _pulse_logger.error("Expected inputs : <validation targets directory> <results directory> [markdown directory]")
        sys.exit(1)

    targets_dir = Path(sys.argv[1])
    results_dir = Path(sys.argv[2])
    markdown_dir = None

    if not targets_dir.is_dir():
        _pulse_logger.error("Please provide a valid validation targets directory")
        sys.exit(1)

    if not results_dir.is_dir():
        _pulse_logger.error("Please provide a valid results directory")
        sys.exit(1)

    if len(sys.argv) > 3:
        markdown_dir = Path(sys.argv[3])
        if not markdown_dir.is_dir():
            _pulse_logger.error("Please provide a valid markdown directory")
            sys.exit(1)


    validate(targets_dir, results_dir, markdown_dir)
