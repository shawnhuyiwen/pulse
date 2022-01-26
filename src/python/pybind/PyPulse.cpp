/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include <pybind11/pybind11.h>
#include "PulseEngineThunk.h"

namespace py = pybind11;

// Core Physiology Engine
void PulseEngineBind(py::module&);
void PulseEnginePoolBind(py::module&);
// Studies
void MultiplexVentilationEngineBind(py::module&);

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
   
   py::enum_<eSerializationFormat>(m, "serialization_format")
    .value("binary", eSerializationFormat::BINARY)
    .value("json", eSerializationFormat::JSON)
    .export_values();
   py::enum_<eModelType>(m, "model_type")
     .value("human_adult_whole_body", eModelType::HumanAdultWholeBody)
     .value("human_adult_ventilation_mechanics", eModelType::HumanAdultVentilationMechanics)
     .export_values();

  PulseEngineBind(m);
  PulseEnginePoolBind(m);
  MultiplexVentilationEngineBind(m);

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}