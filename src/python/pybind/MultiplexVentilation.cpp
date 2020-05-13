/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "MVEngine.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

void MultiplexVentilationEngine(py::module& m)
{
  py::class_<MVEngine>(m, "MVEngine", py::buffer_protocol())
    .def(py::init<std::string const&, bool, std::string const&>())
    .def("create_engine", (bool (MVEngine::*)(const std::string & simulationDataStr, SerializationFormat fmt)) &MVEngine::CreateEngine)
    .def("advance_time", &MVEngine::AdvanceTime)
    .def("process_actions", &MVEngine::ProcessActions)
    .def("get_patient_state", (std::string (MVEngine::*)(SerializationFormat fmt)) &MVEngine::GetSimulationState)
    .def("destroy_engine", &MVEngine::DestroyEngines)
    ;
}