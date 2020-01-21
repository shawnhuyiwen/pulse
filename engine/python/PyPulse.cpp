/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "PulsePhysiologyEngine.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

//using ResultVector = std::vector<double>;
//PYBIND11_MAKE_OPAQUE(ResultVector);
PYBIND11_MAKE_OPAQUE(std::vector<double>);

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

  py::enum_<SerializationFormat>(m, "serialization_format")
    .value("binary", SerializationFormat::BINARY)
    .value("json", SerializationFormat::JSON)
    .export_values();

  //py::class_<ResultVector>(m, "ResultVector")
  //  .def(py::init<>())
  //  .def("back", (double &(ResultVector::*)()) &ResultVector::back)
  //  .def("__len__", [](const ResultVector& v) { return v.size(); })
  //  .def("__iter__", [](ResultVector& v) {
  //    return py::make_iterator(v.begin(), v.end());
  //  }, py::keep_alive<0,1>())
  //  ;

  py::bind_vector<std::vector<double>>(m, "ResultVector");

  py::class_<PulseEngineThunk>(m, "Engine", py::buffer_protocol())
    .def(py::init<std::string const&, bool, std::string const&>())
    .def("serialize_from_file", &PulseEngineThunk::SerializeFromFile)
    .def("serialize_to_file", &PulseEngineThunk::SerializeToFile)
    .def("serialize_from_string", &PulseEngineThunk::SerializeFromString)
    .def("serialize_to_string", &PulseEngineThunk::SerializeToString)
    .def("initialize_engine", &PulseEngineThunk::InitializeEngine)
    .def("keep_log_messages", &PulseEngineThunk::KeepLogMessages)
    .def("pull_log_messages", &PulseEngineThunk::PullLogMessages)
    .def("keep_event_changes", &PulseEngineThunk::KeepEventChanges)
    .def("pull_events", &PulseEngineThunk::PullEvents)
    .def("pull_actie_events", &PulseEngineThunk::PullActiveEvents)
    .def("process_actions", &PulseEngineThunk::ProcessActions)
    .def("advance_timestep", &PulseEngineThunk::AdvanceTimeStep)
    //.def("pull_data", (void(PulseEngineThunk::*)(std::vector<double>&)) & PulseEngineThunk::PullData)
    .def("pull_data", [](PulseEngineThunk& t) -> py::array {
       double* r = t.PullDataPtr();
       // This is a copy, but it's pretty small
       // If we need to, we can not copy, and send a buffer info description
       return py::array(t.DataLength(), r);
     })
    ;

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}