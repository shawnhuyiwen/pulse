# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

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
    _pulse_logger.info(output_dir)
    try:
        if os.path.isdir(output_dir):
            shutil.rmtree(output_dir)
        os.makedirs(output_dir)
    except OSError as e:
        _pulse_logger.error(f"Unable to clean directories")
        return

    _pulse_logger.info(f"Generating data from {xls_file}")

    workbook = load_workbook(filename=xls_file, data_only=True)
    for s in workbook.sheetnames:
        if not read_sheet(workbook[s], output_dir):
            _pulse_logger.error(f"Unable to read {s} sheet")


def read_sheet(sheet: Worksheet, output_dir: str):
    class Stage(Enum):
        IDScenario = 0
        FilesAndConditions = 1
        Segment = 2
        ValidationTargets = 3

    @dataclass
    class Segment():
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


    stage = Stage.IDScenario
    scenario = SEScenario()
    segments = []
    seg = None
    for row_num, r in enumerate(sheet.iter_rows(min_row=2, values_only=True)):
        if stage == Stage.IDScenario:
            scenario.set_name(r[0])
            scenario.set_description(r[1])
            stage = Stage.FilesAndConditions
        elif stage == Stage.FilesAndConditions:
            if isinstance(r[0], str) and r[0].strip().lower() == "data request files":
                continue
            dr_files = [f"{f.strip()}.json" for f in r[0].split(",")]
            scenario.get_data_request_files().extend(dr_files)
            scenario.get_patient_configuration().set_patient_file(r[1].strip())
            # TODO: Add conditions to patient config
            conditions = r[2]
            _pulse_logger.debug(f"Conditions: {conditions}")
            stage = Stage.Segment
        elif stage == Stage.Segment:
            if isinstance(r[0], str) and r[0].strip().lower() == "segment":
                continue
            if seg is not None:
                segments.append(seg)
            # TODO: Add actions to scenario
            seg = Segment(
                id = r[0],
                note = r[1],
                actions = r[2]
            )
            stage = Stage.ValidationTargets
        elif stage == Stage.ValidationTargets:
            if isinstance(r[0], str) and r[0].strip().lower() == "data header":
                continue
            if isinstance(r[0], str) and r[0].strip().lower() == "segment":
                segments.append(seg)
                stage = Stage.Segment
                seg = None
                continue
            if r[0] is not None:
                seg.val_tgts.append((r[0], r[1], r[2]))
        else:
            raise ValueError(f"Unknown automated validation stage: {stage}")

    if seg is not None:
        segments.append(seg)

    _pulse_logger.debug(scenario)
    for s in segments:
        _pulse_logger.debug(str(s))

    return True


if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG, format='%(levelname)s: %(message)s')

    load_data(get_data_dir() + "/human/adult/validation/Scenarios/MechanicalVentilatorValidation.automated.xlsx")
