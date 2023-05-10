# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import logging
import numbers
import os
import shutil
import numpy as np
import pandas as pd

from pulse.cdm.engine import SEDataRequestManager, SEValidationTarget, eValidationTargetType
from pulse.cdm.io.engine import serialize_data_request_manager_to_file
from pulse.cdm.scalars import get_unit
from pulse.cdm.utils.csv_utils import read_csv_into_df
from pulse.cdm.utils.file_utils import get_data_dir


_pulse_logger = logging.getLogger('pulse')

# TODO: add openpyxl to requirements.txt
def load_data(xls_file: str):
    # Remove and recreate directory
    output_dir = "./optimizer/"
    try:
        shutil.rmtree(output_dir)
        os.makedirs(output_dir)
    except OSError as e:
        _pulse_logger.error(f"Unable to clean directories")
        return

    if not os.path.isfile(xls_file):
        _pulse_logger.error(f"Could not find xls file {xls_file}")
        return

    _pulse_logger.info(f"Generating data from {xls_file}")

    system = "Cardiovascular"
    if not read_sheet(xls_file, system, output_dir):
        _pulse_logger.error(f"Unable to read {system} sheet")

def read_sheet(xls_file: str, system: str, output_dir: str):
    df = pd.read_excel(xls_file, sheet_name=system)
    if df.empty:
        return False

    targets = {}

    headers = ['Output', 'Units', 'Algorithm', 'Reference Values', 'OptimizerTargets']
    for row in df[headers].itertuples(index=True, name="Row"):
        if not row.Output:
            continue

        if (isinstance(row.OptimizerTargets, str) and row.OptimizerTargets == "OptimizerTargets") or \
           (isinstance(row.OptimizerTargets, numbers.Number) and np.isnan(row.OptimizerTargets)):
            continue

        # Space in 'Reference Values' is an invalid Python identifier, so it becomes positional
        ref_cell = row._4

        if row.OptimizerTargets not in targets:
            targets[row.OptimizerTargets] = SEDataRequestManager()
        dr_manager = targets[row.OptimizerTargets]
        vts = dr_manager.get_validation_targets() if dr_manager.has_validation_targets() else []

        prop_split = [s.strip() for s in row.Output.split("-")]
        unit = None
        try:
            unit = get_unit(row.Units.strip)
        except:
            _pulse_logger.warning(f"Could not identify unit: {row.Units}")
        tgt = SEValidationTarget.create_liquid_compartment_validation_target(prop_split[0], prop_split[1], unit)
        tgt.set_type(eValidationTargetType[row.Algorithm])

        if isinstance(ref_cell, str):
            s_vals = ref_cell.strip()
            s_vals = s_vals.replace('[', ' ')
            s_vals = s_vals.replace(']', ' ')
            vals = [float(s) for s in s_vals.split(',')]

            tgt.set_range_min(min(vals))
            tgt.set_range_max(max(vals))
        elif isinstance(ref_cell, numbers.Number):
            val = float(ref_cell)
            tgt.set_range_min(val)
            tgt.set_range_max(val)
        else:
            _pulse_logger.warning(f"Unknown reference value type {ref_cell}")
            tgt.set_range_min(np.nan)
            tgt.set_range_max(np.nan)

        vts.append(tgt)
        dr_manager.set_validation_targets(vts)

    for optimizer in targets.keys():
        filename = f"{output_dir}{optimizer}.json"
        _pulse_logger.info(f"Writing {filename}")
        serialize_data_request_manager_to_file(targets[optimizer], filename)

    return True


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO, format='%(levelname)s: %(message)s')

    load_data(get_data_dir() + "/human/adult/validation/SystemValidationData.xlsx")
