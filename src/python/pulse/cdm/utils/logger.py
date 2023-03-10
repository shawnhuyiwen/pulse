# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.
from enum import Enum

from pulse.cdm.engine import SEAction

class ePrettyPrintType(Enum):
    Action = 0
    Condition = 1
def pretty_print(string: str, print_type: ePrettyPrintType):
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
        if "Comment:" in line and len(line) < 9:
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
                    line = ''.join([line, peek[(peek.find("Unit:")+5):].rstrip()])
                else:
                    idx += 2

        ret = ''.join([ret, line, "\n"])
        idx += 1

    ret = ret.replace('::', ':')

    return ret
