# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import json
import logging
import numbers
import numpy as np
import os
import shutil
from dataclasses import dataclass, field
from enum import Enum
from openpyxl import load_workbook
from openpyxl.worksheet.worksheet import Worksheet
from typing import Hashable, List, Tuple

from pulse.cdm.scenario import SEScenario
from pulse.cdm.utils.file_utils import get_data_dir


_pulse_logger = logging.getLogger('pulse')


def load_data(xls_file: str):
    # Remove and recreate directory
    output_dir = "./validation/scenarios/"
    xls_basename = os.path.splitext(os.path.splitext(os.path.basename(xls_file))[0])[0]
    xls_basename_out = xls_basename[:-10] if xls_basename.lower().endswith("validation") else xls_basename
    output_dir = os.path.join(output_dir, xls_basename_out + "/")
    try:
        if os.path.isdir(output_dir):
            shutil.rmtree(output_dir)
        os.makedirs(output_dir)
    except OSError as e:
        _pulse_logger.error(f"Unable to clean directories")
        return

    _pulse_logger.info(f"Generating data from {xls_file}")

    # Iterate through each sheet in the file, generating a scenario for each
    workbook = load_workbook(filename=xls_file, data_only=True)
    for s in workbook.sheetnames:
        if not read_sheet(workbook[s], output_dir):
            _pulse_logger.error(f"Unable to read {s} sheet")


# Dataclass encapsulating information for a singular scenario "segment"
@dataclass
class ScenarioSegment():
    id: Hashable
    note: str = ""
    actions: str = ""
    val_tgts: List[Tuple[str, str, str]] = field(default_factory=list)

    def __str__(self):
        tabbed_note = ("\n\t\t" + self.note.replace("\n", "\n\t\t")) if self.note else None
        tabbed_actions = ("\n\t\t" + self.actions.replace("\n", "\n\t\t")) if self.actions else None
        tabbed_tgts = "".join(["\n\t\t" + str(tgt) for tgt in self.val_tgts])
        return f'Segment {self.id}:\n\tNote: {tabbed_note}\n\t' \
            f'Actions: {tabbed_actions}\n\tValidation Targets: {tabbed_tgts}'


# Read xlsx sheet and generate corresponding scenario file
def read_sheet(sheet: Worksheet, output_dir: str):
    class Stage(Enum):
        IDScenario = 0
        FilesAndConditions = 1
        Segment = 2
        ValidationTargets = 3

    stage = Stage.IDScenario
    scenario = SEScenario()
    segments = []
    seg = None
    conditions = ""

    for row_num, r in enumerate(sheet.iter_rows(min_row=2, values_only=True)):
        # Scenario name and description
        if stage == Stage.IDScenario:
            scenario.set_name(r[0])
            scenario.set_description(r[1] if r[1] is not None else "")

            stage = Stage.FilesAndConditions
        # DR files, patient file, and conditions
        elif stage == Stage.FilesAndConditions:
            # Header row
            if isinstance(r[0], str) and r[0].strip().lower() == "data request files":
                continue

            if isinstance(r[0], str):
                dr_files = [f"{f.strip()}.json" for f in r[0].split(",")]
                scenario.get_data_request_files().extend(dr_files)
            if isinstance(r[1], str):
                scenario.get_patient_configuration().set_patient_file(r[1].strip())
            if isinstance(r[2], str):
                conditions = r[2]

            stage = Stage.Segment
        # Segment ID, note, and actions
        elif stage == Stage.Segment:
            # Header row
            if isinstance(r[0], str) and r[0].strip().lower() == "segment":
                continue

            seg = ScenarioSegment(
                id = r[0],
                note = r[1] if isinstance(r[1], str) else "",
                actions = r[2] if isinstance(r[2], str) else ""
            )

            stage = Stage.ValidationTargets
        # Validation targets, comparison type/to, reference/notes
        elif stage == Stage.ValidationTargets:
            # Header row
            if isinstance(r[0], str) and r[0].strip().lower() == "data header":
                continue

            # Header row, identified end of segment
            if isinstance(r[0], str) and r[0].strip().lower() == "segment":
                segments.append(seg)
                seg = None

                stage = Stage.Segment
                continue

            if r[0] is not None:
                seg.val_tgts.append((
                    r[0],
                    r[1],
                    r[2] if isinstance(r[2], str) else ""
                ))
        else:
            raise ValueError(f"Unknown automated scenario validation stage: {stage}")

    if seg is not None:
        segments.append(seg)

    # TODO: Actually use validation targets
    for s in segments:
        val_tgts = s.val_tgts
        _pulse_logger.info(f'Segment {s.id} Validation targets: {"None" if len(val_tgts) == 0 else ""}')
        for tgt in val_tgts:
            _pulse_logger.info(f"\t{tgt}")

    # Write out scenario
    filename = os.path.join(output_dir, scenario.get_name() + ".json")
    _pulse_logger.info(f"Writing {filename}")
    with open(filename, "w") as f:
        f.write(write_scenario(scenario, segments, conditions))

    return True


def write_scenario(scenario: SEScenario, segments: List[ScenarioSegment], conditions: str):
    # Load actions into dict from concatenated JSON across segments
    all_actions_str = '{"AnyAction": ['
    for s in segments:
        all_actions_str += s.actions
    all_actions_str += ']}'
    all_actions = {}
    if all_actions_str != '{"AnyAction": []}':
        all_actions = json.loads(all_actions_str)

    # Load conditions into dict
    all_conditions_str = '{"AnyCondition": ['
    all_conditions_str += conditions
    all_conditions_str += ']}'
    all_conditions = {}
    if conditions:
        all_conditions = json.loads(all_conditions_str)

    # Create patient configuration dict
    patient_config = scenario.get_patient_configuration()
    patient_config_dict = {}
    if patient_config.has_patient_file():
        patient_config_dict["PatientFile"] = patient_config.get_patient_file()
    elif patient_config.has_patient():
        raise ValueError("Patient to JSON not implemented")
    patient_config_dict["Conditions"] = all_conditions
    patient_config_dict["DataRoot"] = patient_config.get_data_root_dir()

    # Create data request manager dict
    dr_mgr = scenario.get_data_request_manager()
    dr_mgr_dict = {}
    # TODO: Properly encode/decode DRs and validation targets
    if dr_mgr.get_data_requests():
        dr_mgr_dict["DataRequest"] = dr_mgr.get_data_requests()
    if dr_mgr.get_validation_targets():
        dr_mgr_dict["ValidationTarget"] = dr_mgr.get_validation_targets()
    if dr_mgr.get_results_filename():
        dr_mgr_dict["ResultFilename"] = dr_mgr.get_results_filename()

    # Compose full scenario dict
    scenario_dict = {
        "Name": scenario.get_name(),
        "Description": scenario.get_description(),
        "PatientConfiguration": patient_config_dict,
        "DataRequestFile": scenario.get_data_request_files(),
        "DataRequestManager": dr_mgr_dict,
        "AnyAction": all_actions["AnyAction"],
    }

    return json.dumps(scenario_dict, indent=2)


if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG, format='%(levelname)s: %(message)s')

    load_data(get_data_dir() + "/human/adult/validation/Scenarios/MechanicalVentilatorValidation.automated.xlsx")
