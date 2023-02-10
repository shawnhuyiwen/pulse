# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.
import math

def percent_tolerance(expected: float, calculated: float, epsilon: float):
    # Check for 'invalid' numbers
    if math.isnan(expected) or math.isnan(calculated) or \
       math.isinf(expected) or math.isinf(calculated):
        print(f"While finding percent tolerance from values 'expected' = {expected} and " \
               f"'calculated' = {calculated}, invalid values (NaN or Infinity) were found. Unexpected results may occur.")

    # Special cases
    if expected == 0.0 and calculated == 0.0:
        return 0.0
    elif expected == 0.0 or calculated == 0.0:
        if abs(expected + calculated) < epsilon:
            return 0.0
        else:
            if expected == 0.0:
                return float('inf')
            elif expected < 0.0:
                return -100.0
            else:
                return 100
    else:
        return abs(calculated - expected) / expected * 100.0

def generate_percent_tolerance_span(expected: float, calculated: float, epsilon: float, precision: int = 1):
    percent = percent_tolerance(expected, calculated, epsilon)

    return generate_percent_span(percent, precision)

def percent_difference(expected: float, calculated: float, epsilon: float):
    # Check for 'invalid' numbers
    if math.isnan(expected) or math.isnan(calculated) or \
       math.isinf(expected) or math.isinf(calculated):
        print(f"While finding percent difference from values 'expected' = {expected} and " \
               f"'calculated' = {calculated}, invalid values (NaN or Infinity) were found. Unexpected results may occur.")

    # Special cases
    if expected == 0.0 and calculated == 0.0:
        return 0.0
    elif expected == 0.0 or calculated == 0.0:
        if abs(expected + calculated) < epsilon:
            return 0.0
        else:
            return 200.0
    else:
        difference = calculated - expected
        average = (calculated + expected) / 2.0

        if average == 0.0:
            return float('inf')

        return abs(difference / average) * 100.0

def generate_percent_difference_span(expected: float, calculated: float, epsilon: float, precision: int = 1):
    percent = percent_difference(expected, calculated, epsilon)

    return generate_percentage_span(percent, precision)

def generate_percentage_span(percentage, precision):
    if percentage <= 10:
        c = '"success"'
    elif percentage <= 30:
        c = '"warning"'
    else:
        c = '"danger"'

    return f'<span class={c}>{percentage:.{precision}f}%</span>'
