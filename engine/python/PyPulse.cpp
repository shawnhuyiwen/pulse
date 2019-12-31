/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "PulsePhysiologyEngine.h"

#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(PyPulse, m)
{
  m.doc() = R"pbdoc(
        Pybind11 Pulse Physiology Engine
        -----------------------
        .. currentmodule:: pulse
        .. autosummary::
           :toctree: _generate
           serialize_from_file
           serialize_to_file
           serialize_from_string
           serialize_to_string
    )pbdoc";

  py::class_<PulseEngineThunk>(m, "Engine")
    .def(py::init<std::string const&, bool, std::string const&>())
    .def("serialize_from_file", &PulseEngineThunk::SerializeFromFile)
    .def("serialize_to_file", &PulseEngineThunk::SerializeToFile)
    .def("serialize_from_string", &PulseEngineThunk::SerializeFromString)
    .def("serialize_to_string", &PulseEngineThunk::SerializeToString)
    ;

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}