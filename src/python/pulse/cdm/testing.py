# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import os
from dataclasses import dataclass, field
from typing import Dict, List, Optional

from pulse.cdm.scalars import TimeUnit

class SETestCase():
    __slots__ = ["name", "duration", "failures", "eq_opts"]

    def __init__(self):
        # TODO: SEEqualOptions
        pass

    def clear(self):
        self.name = None
        if self.duration is not None:
            self.duration.invalidate()
        self.failures = []

    def get_name(self):
        return self.name
    def set_name(self, name: str):
        self.name = name
    def has_name(self):
        return self.name is not None

    def get_duration(self):
        if self.duration is None:
            self.duration = SEScalarTime()
        return self.duration
    def has_duration(self):
        return self.duration is not None and self.duration.is_valid()

    def has_failures(self):
        return len(self.failures) > 0
    def add_failure(self, f: str):
        self.failures.append(f)
    def get_failures(self):
        return list(self.failures)


class SETestSuite():
    __slots__ = ["name", "performed", "requirements", "test_cases", "active_case", "active_case_listener"]

    def __init__(self):
        self.clear()

    def clear(self):
        self.name = None
        self.performed = True
        self.requirements = []
        self.test_cases = []
        self.active_case = None
        #TODO: Test case listener

    def get_name(self):
        return self.name
    def set_name(self, name: str):
        self.name = name
    def has_name(self):
        return self.name is not None

    def get_performed(self):
        return self.performed
    def set_performed(self, p: bool):
        self.performed = p

    def add_requirement(self, f: str):
        self.requirements.append(f)
    def get_requirements(self):
        return self.requirements

    def create_test_case(self):
        tc = SETestCase()
        self.test_cases.append(tc)
        return tc
    def get_test_cases(self):
        return list(self.test_cases)

    def get_case_equal_options(self):
        if self.active_case is None:
            return None
        return self.active_case.get_equal_options()

    def start_case(self, name: str):
        self.active_case = self.create_test_case()
        self.active_case.set_name(name)
        return self.active_case

    def get_active_case(self):
        return self.active_case

    def get_num_errors(self):
        errs = 0
        for tc in self.test_cases:
            if not tc.get_failures():
                errs += 1
        return errs

    def get_duration(self, unit: TimeUnit):
        time = 0
        for tc in self.test_cases:
            time += tc.get_duration().get_value(unit)
        return time


class SETestReport():
    __slots__ = ["name", "file_name", "report_dir", "test_suites", "known_failing_suites"]

    @dataclass
    class Data():
        name: str
        runs: int = field(compare=False)
        errors: int = field(compare=False)
        duration_s: float = field(compare=False)
        failures: List[str] = field(default_factory=list, compare=False)
        requirements: List[str] = field(default_factory=list, compare=False)
        html: str = field(default="", compare=False)

    def __init__(self):
        self.report_dir = "./"
        self.test_suites = []
        self.known_failing_suites = []
        self.set_file_name("TestReport.json")

    def clear(self):
        self.name = ""
        self.file_name = ""
        self.report_dir = ""
        self.test_suites = []
        self.known_failing_suites = []

    def set_full_report_path(self, path: str):
        path = path.replace("\\\\", "/")
        dir = path[:path.rfind("/")+1]
        self.set_report_directory(dir)
        file = path[path.rfind("/")+1:]
        self.set_file_name(file)

    def set_report_directory(self, dir: str):
        if not os.path.exists(dir):
            os.makedirs(dir)

        self.report_dir = dir + "/"

    def get_name(self):
        return self.name
    def set_name(self, name: str):
        self.name = name

    def get_file_name(self):
        return this._file_name
    def set_file_name(self, file_name: str, ext: Optional[str]=None):
        if ext is not None:
            self.name = name
            self.file_name = name + ext
        else:
            if "." in file_name:
                self.file_name = file_name
                self.name = file_name[:file_name.rfind(".")]
            else:
                self.name = file_name
                self.file_name = file_name + ".json"

    def get_errors(self):
        errors = 0
        for suite in self.test_suites:
            errors += suite.get_num_errors()
        return errors

    def create_test_suite(self):
        ts = SETestSuite()
        self.test_suites.append(ts)
        return ts
    def get_test_suites(self):
        return list(self.test_suites)

    def create_error_suite(self, name: str, error: str):
        ts = SETestSuite()
        ts.set_name(name)
        ts.set_performed(False)
        tc = ts.create_test_case()
        tc.set_name(name)
        tc.add_failure(error)

        self.test_suites.append(ts)

    def add_known_suite_failure(self, suite_name: str):
        self.known_failing_suites.append(suite_name)

    def add_test_suite(self, ts: SETestSuite):
        self.test_suites.append(ts)

    def write(self):
        write(self.report_dir)
    def write(self, toDirectory: Optional[str]):
        if not toDirectory:
            toDirectory = "./"
        self.write_file(toDirectory + self.file_name)

    def to_html(self, title: str):
        return to_html(title, None)
    def to_html(self, title: str, groups: Optional[Dict[str, List[str]]]):
        sort_results = True
        html = '<html>'
        html += f'<head><title>{title}</titile></head>'
        html += '<body>'
        html += f'<h1>{title}</h1><br>'

        # Any test suites not run?
        html += '<table border="1">'
        html += '<tr><th>Missing Reports</th></tr>'
        for ts in self.test_suites:
            if not ts.get_performed():
                html += f'<tr bgcolor="#FF0000"<td>{ts.get_name()}</td></tr>'
        html += '</table>'

        #Make a little room
        html += '<br>'

        # Any test suires known to fail?
        html += '<table border="1">'
        html += '<tr><th>Known Failing Reports</th></tr>'
        for suite in self.known_failing_suites:
            html += f'<tr bgcolor="#FFFF00"><td>{suite}</td></tr>'
        html += '</table>'

        if groups is None:
            groups = dict()
            all = []
            for ts in self.test_suites:
                all.append(ts.get_name())
            groups[self.name] = all

        for group, group_tests in groups.items():
            # Make a little room
            html += '<br>'

            html += '<table border="1">'
            html += f'<caption><b>{group}</b></caption>'
            html += '<tr>'
            html += '<th> Test Case </th>'
            html += '<th> Failures </th>'
            html += '</tr>'

            runs = 0
            total_runs = 0
            total_errors = 0
            total_duration_s = 0
            error_data = []
            passed_data = []

            data = self.Data()
            for ts in self.test_suites:
                if ts.get_name() not in group_tests:
                    continue

                total_runs += len(ts.get_test_cases())
                total_errors += ts.get_num_errors()
                total_duration_s += ts.get_duration(TimeUnit.s)

                if ts.get_performed():
                    runs += 1
                    data = self.Data()
                    data.name = ts.get_name()
                    data.runs = len(ts.get_test_cases())
                    data.errors = ts.get_num_erros()
                    data.duration_s = ts.get_duration(TimeUnit.s)
                    for tc in ts.testCases:
                        data.failures.extend(tc.get_failures())
                    data.requirements.extend(ts.get_requirements())
                    if ts.get_num_errors() > 0:
                        error_data.append(data)
                        data.html += '<tr bgcolor="#FF0000">'
                    else:
                        passed_data.append(data)
                        data.html += '<tr bgcolor="#00FF00">'
                    data.html += f'<td align="left">{data.name}</td>'
                    data.html += '<td>'
                    if len(data.failures) > 0:
                        i = 0
                        while i < len(data.failures) - 1:
                            f = data.failures[i]
                            f = f.replace("\n", "<br>")
                            data.html += f'{f}<br>'
                            i += 1
                            # Only write out the first few errors, could be a LOT of errors
                            if i > 5:
                                break
                        data.html += data.failures[i]

                    data.html += '</td>'
                    data.html += '</tr>'

            html += '<tr>'
            html += f'<td align="left">Totals for {runs} test suites</td>'
            html += f'<td>{total_errors}</td>'
            html += '</tr>'

            if sort_results:
                error_data.sort()
                passed_data.sort()
            for d in error_data:
                html += d.html
            for d in passed_data:
                html += d.html

            html += '</table>'

            # Make a little room
            html += '<br>'

        html += '</body>'
        html += '</html>'
        return html
