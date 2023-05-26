# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import os
import logging
import numpy as np
from typing import Dict, List
from mdutils.mdutils import MdUtils

from pulse.cdm.engine import SEValidationTarget, SESegmentValidationTarget
from pulse.cdm.utils.math_utils import generate_percentage_span, percent_difference
from pulse.cdm.io.engine import serialize_segment_validation_target_list_from_file


_pulse_logger = logging.getLogger('pulse')


def validate(targets_dir: str, results_dir: str) -> None:
    # TODO: Get results from files
    results = {}
    results[0] = {
        "RespirationRate(1/min)": 12,
        "HeartRate(1/min)": 72,
        "SystolicArterialPressure(mmHg)": 114,
        "DiastolicArterialPressure(mmHg)": 73.6,
        "OxygenSaturation": 0.975
    }
    results[1] = {
        "RespirationRate(1/min)": 17.44,
        "HeartRate(1/min)": 73.64,
        "SystolicArterialPressure(mmHg)": 114,
        "DiastolicArterialPressure(mmHg)": 74.2,
        "OxygenSaturation": 0.956
    }
    results[2] = {
        "RespirationRate(1/min)": 12.45,
        "HeartRate(1/min)": 71.39,
        "SystolicArterialPressure(mmHg)": 114,
        "DiastolicArterialPressure(mmHg)": 73.6,
        "OxygenSaturation": 0.976
    }
    results[3] = {
        "RespirationRate(1/min)": 0,
        "HeartRate(1/min)": 146.04,
        "SystolicArterialPressure(mmHg)": 108,
        "DiastolicArterialPressure(mmHg)": 83.5,
        "OxygenSaturation": 0.676
    }
    results[4] = {
        "RespirationRate(1/min)": 12.5,
        "HeartRate(1/min)": 55.83,
        "SystolicArterialPressure(mmHg)": 110,
        "DiastolicArterialPressure(mmHg)": 61.3,
        "OxygenSaturation": 0.977
    }
    results[5] = {
        "RespirationRate(1/min)": 0,
        "HeartRate(1/min)": 146.19,
        "SystolicArterialPressure(mmHg)": 109,
        "DiastolicArterialPressure(mmHg)": 85.1,
        "OxygenSaturation": 0.597
    }
    results[6] = {
        "RespirationRate(1/min)": 12.3,
        "HeartRate(1/min)": 49.66,
        "SystolicArterialPressure(mmHg)": 111,
        "DiastolicArterialPressure(mmHg)": 58.6,
        "OxygenSaturation": 0.975
    }

    # Get all validation targets from files
    val_tgts = {}
    for filename in os.listdir(targets_dir):
        if not filename.endswith("ValidationTargets.json"):
            continue

        seg_id = int(filename[len("Segment"):-len("ValidationTargets.json")])

        val_tgts[seg_id] = serialize_segment_validation_target_list_from_file(os.path.join(targets_dir, filename))

    scenario_name = os.path.basename(os.path.dirname(targets_dir))
    md_file = MdUtils(file_name=os.path.join(targets_dir, scenario_name + "Validation"), title=scenario_name + " Validation")

    # Evaluate targets and create markdown tables for each segment
    for seg_id in sorted(val_tgts.keys()):
        table_data = [["Property Name", "Expected Value", "Engine Value", "Percent Error", "Notes"]]

        tgts = val_tgts[seg_id]
        for tgt in tgts:
            table_data.append(evaluate(seg_id, tgt, results))

        if tgts:
            md_file.new_header(1, f"Segment {seg_id}")
            md_file.new_table(
                columns=len(table_data[0]),
                rows=len(table_data),
                text=[item for sublist in table_data for item in sublist],
                text_align="left"
            )

    md_file.create_md_file()


def evaluate(seg_id: int, tgt: SESegmentValidationTarget, results: Dict[int, Dict[str, float]]) -> List[str]:
    header = tgt.get_header()
    compare_seg_id = tgt.get_segment()

    expected_str = ""
    compare_type = tgt.get_comparison_type()
    if compare_type == SEValidationTarget.eComparisonType.EqualTo:
        expected_val = tgt.get_target()
        if np.isnan(expected_val):
            expected_val = results[compare_seg_id][header]
        expected_str = str(expected_val)
        engine_val = results[seg_id][header]

        err = percent_difference(expected_val, engine_val, 1E-9)
    elif compare_type == SEValidationTarget.eComparisonType.GreaterThan or \
            compare_type == SEValidationTarget.eComparisonType.Increase:
        expected_val = tgt.get_target()
        if np.isnan(expected_val):
            expected_val = results[compare_seg_id][header]
        expected_str = str(expected_val)
        engine_val = results[seg_id][header]

        # TODO: Make this actually evaluate greater than
        err = percent_difference(expected_val, engine_val, 1E-9)
    elif compare_type == SEValidationTarget.eComparisonType.LessThan or \
            compare_type == SEValidationTarget.eComparisonType.Decrease:
        expected_val = tgt.get_target()
        if np.isnan(expected_val):
            expected_val = results[compare_seg_id][header]
        expected_str = str(expected_val)
        engine_val = results[seg_id][header]

        # TODO: Make this actually evaluate less than
        err = percent_difference(expected_val, engine_val, 1E-9)
    elif compare_type == SEValidationTarget.eComparisonType.Range:
        tgt_min = tgt.get_target_minimum()
        tgt_max = tgt.get_target_maximum()
        expected_str = f"[{tgt_min},{tgt_max}]"
        engine_val = results[seg_id][header]
        min_err = percent_difference(tgt_min, engine_val, 1E-9)
        max_err = percent_difference(tgt_max, engine_val, 1E-9)

        # No error if we are in range
        if engine_val >= tgt_min and engine_val <= tgt_max:
            err = 0.
        elif engine_val > tgt_max:
            err = max_err
        elif engine_val < tgt_min:
            err = min_err
    else:
        raise ValueError(f"Unknown compare type: {compare_type}")

    # Close enough
    if abs(err) < 1E-15:
        err = 0.

    err_str = generate_percentage_span(err, 1)

    if tgt.get_reference():
        references = [ref.strip() for ref in tgt.get_reference().replace("\n", "").split(",")]
        for ref in references:
            expected_str += f" @cite {ref}"

    return [header, expected_str, str(engine_val), err_str, tgt.get_notes()]


if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG, format='%(levelname)s: %(message)s')

    targets_dir = "./validation/scenarios/Respiratory/AirwayObstructionVaried/"
    validate(targets_dir, "")
