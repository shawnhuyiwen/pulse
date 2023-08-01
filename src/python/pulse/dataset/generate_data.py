# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.dataset.segment_validation_pipeline import *

if __name__ == "__main__":

    # In the future, we will do all xlsx in the data/validation dir
    # But for now, we are just hard coding the automated xlsx files
    xls_files = ["AirwayObstruction.xlsx"]

    exec_opt = eExecOpt.GenerateOnly
    for xls_file in xls_files:
        segment_validation_pipeline(
            xls_file=Path(xls_file),
            exec_opt=exec_opt
        )
