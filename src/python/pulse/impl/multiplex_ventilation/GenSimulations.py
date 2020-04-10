# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import pandas as pd
import itertools
import json

def get_patient_pairs(solo_patients_file):
    # values of patients to compare
    patient_pair_sweeps = {
        'Compliance_L_Per_cmH2O': [0.01, 0.025, 0.045],
        'PEEP_cmH2O': [10, 16, 20],
        'ImpairmentFraction': [0.3, 0.6, 0.9],
    }

    # read in the patient json file
    patientsfile = open(solo_patients_file, 'r')

    # convert it to pandas dataframe (i.e. table)
    patientdata = json.load(patientsfile)['Patients']
    patients = pd.read_json(path_or_buf=json.dumps(patientdata), orient='records')

    # correct no impairment case (from NaN to 0.0)
    patients.fillna(value=0.0, axis='columns', inplace=True)

    # find the patient records of interest (use df.query)
    # handle non-exact impairments (0.2, 0.4, 0.6, 0.8)
    individuals = []
    for compliance in patient_pair_sweeps['Compliance_L_Per_cmH2O']:
        for PEEP in patient_pair_sweeps['PEEP_cmH2O']:
            for impairment in patient_pair_sweeps['ImpairmentFraction']:
                query_string = 'Compliance_L_Per_cmH2O >= {compliance_lower} & \
                                Compliance_L_Per_cmH2O < {compliance_upper} & \
                                PEEP_cmH2O == {PEEP} & \
                                ImpairmentFraction >= {impairment_lower} & \
                                ImpairmentFraction < {impairment_upper} \
                                '.format(compliance_lower=(compliance - 0.002),
                                         compliance_upper=(compliance + 0.002),
                                         PEEP=PEEP,
                                         impairment_lower=impairment,
                                         impairment_upper=(impairment + 0.1)
                                         )

                individuals.append(patients.query(query_string).to_dict(orient='records')[0])

    # return record of pairs
    pairs = list(itertools.combinations_with_replacement(individuals, 2))
    return pairs


# test function
if __name__ == '__main__':
    # get patient pairs
    pairs = get_patient_pairs('./states/multiplex_ventilation/solo_ventilated_states_list.json')
    # write to json file
    json.dump(pairs, open('./states/multiplex_ventilation/multiplex_simulation_list.json', 'w'), indent=4)
