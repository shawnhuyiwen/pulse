# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.
from enum import Enum
from typing import Set
import re

from pulse.cdm.engine import SEAction

def break_camel_case(string: str, preserve_units: Set[str]):
    # https://stackoverflow.com/a/9283563
    camel_case_regex = r"""
        (            # start the group
            # alternative 1
        (?<=[a-z])  # current position is preceded by a lower char
                    # (positive lookbehind: does not consume any char)
        [A-Z]       # an upper char
                    #
        |   # or
            # alternative 2
        (?<!\A)     # current position is not at the beginning of the string
                    # (negative lookbehind: does not consume any char)
        [A-Z]       # an upper char
        (?=[a-z])   # matches if next char is a lower char
                    # lookahead assertion: does not consume any char
        )           # end the group"""

    string = re.sub(camel_case_regex, r' \1', string, flags=re.VERBOSE)

    # Return specified units to their original form
    for unit in preserve_units:
        spaced_unit = re.sub(camel_case_regex, r' \1', unit, flags=re.VERBOSE)
        if spaced_unit != unit:
            string = string.replace(spaced_unit, unit)
    return string

class ePrettyPrintType(Enum):
    Action = 0
    Condition = 1
def pretty_print(string: str, print_type: ePrettyPrintType, preserve_camel_case: bool = False):
    typeTag = ""
    if print_type == ePrettyPrintType.Action:
        typeTag = "Action"
    elif print_type == ePrettyPrintType.Condition:
        typeTage = "Condtion"

    ret = ""
    string = string.replace('"', '')
    string = string.replace('{', '')
    string = string.replace('}', '')
    string = string.replace(',', '')
    string = string.replace('[', '')
    string = string.replace(']', '')

    lines = [s.rstrip() for s in string.splitlines() if s]

    units = set()
    idx = 0
    while idx < len(lines):
        line = lines[idx].rstrip()

        if len(line) == 0:
            idx += 1
            continue
        if typeTag in line:
            idx += 1
            continue
        if "ReadOnly" in line:
            idx += 1
            continue
        if line.endswith("Comment:"):
            idx += 1
            continue

        if idx+1 < len(lines):
            peek = lines[idx+1]
            if "Scalar" in peek:
                peek = lines[idx+2]
                line = ''.join([line, peek[(peek.find("Value")+5):].rstrip()])
                if (idx+3) < len(lines) and "Unit:" in lines[idx+3]:
                    idx += 3
                    peek = lines[idx]
                    unit = peek[(peek.find("Unit:")+5):].rstrip()
                    units.add(unit)
                    line = ''.join([line, unit])
                else:
                    idx += 2

        ret = ''.join([ret, line, "\n"])
        idx += 1

    ret = ret.replace('::', ':')
    if not preserve_camel_case:
        ret = break_camel_case(ret, units)

    return ret
