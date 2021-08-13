/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "PulseEngineThunk.h"
#include "PulseEnginePool.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include <pybind11/numpy.h>

namespace py = pybind11;
namespace pe = pulse::engine;

void PulseEngine(py::module &m)
{
  py::class_<pe::PulseEngineThunk>(m, "Engine", py::buffer_protocol())
    .def(py::init<pe::eModelType,const std::string&>())
    .def("serialize_from_file", &pe::PulseEngineThunk::SerializeFromFile)
    .def("serialize_to_file", &pe::PulseEngineThunk::SerializeToFile)
    .def("serialize_from_string", &pe::PulseEngineThunk::SerializeFromString)
    .def("serialize_to_string", &pe::PulseEngineThunk::SerializeToString)
    .def("initialize_engine", &pe::PulseEngineThunk::InitializeEngine)
    .def("get_initial_patient", &pe::PulseEngineThunk::GetInitialPatient)
    .def("log_to_console", &pe::PulseEngineThunk::LogToConsole)
    .def("keep_log_messages", &pe::PulseEngineThunk::KeepLogMessages)
    .def("set_log_filename", &pe::PulseEngineThunk::SetLogFilename)
    .def("pull_log_messages", &pe::PulseEngineThunk::PullLogMessages)
    .def("keep_event_changes", &pe::PulseEngineThunk::KeepEventChanges)
    .def("pull_events", &pe::PulseEngineThunk::PullEvents)
    .def("pull_active_events", &pe::PulseEngineThunk::PullActiveEvents)
    .def("process_actions", &pe::PulseEngineThunk::ProcessActions)
    .def("advance_timestep", &pe::PulseEngineThunk::AdvanceTimeStep)
    .def("get_timestep", &pe::PulseEngineThunk::GetTimeStep)
    .def("pull_data", [](pe::PulseEngineThunk& t) -> py::array {
       double* r = t.PullDataPtr();
       // This is a copy, but it's pretty small
       // If we need to, we can not copy, and send a buffer info description
       return py::array(t.DataLength(), r);
     })
    ;
}

void PulseEnginePool(py::module& m)
{
  py::class_<pe::PulseEnginePool>(m, "EnginePool", py::buffer_protocol())
    .def(py::init<size_t, const std::string&>())
    .def("initialize_engines", &pe::PulseEnginePool::InitializeEngines)
    .def("remove_engine", &pe::PulseEnginePool::RemoveEngine)
    .def("process_actions", &pe::PulseEnginePool::ProcessActions)
    .def("pull_data_requested", &pe::PulseEnginePool::PullDataRequested)
    ;
}
