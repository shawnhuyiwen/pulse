/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include <pybind11/pybind11.h>
#include "PulsePhysiologyEngine.h"

namespace py = pybind11;

// Core Physiology Engine
void PhysiologyEngine(py::module&);
// Studies
void MultiplexVentilationEngine(py::module&);

PYBIND11_MODULE(utils, m)
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
   
   py::enum_<SerializationFormat>(m, "serialization_format")
    .value("binary", SerializationFormat::BINARY)
    .value("json", SerializationFormat::JSON)
    .export_values();
    
  PhysiologyEngine(m);
  MultiplexVentilationEngine(m);

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}