/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "circuit/SECircuitManager.h"

#include "circuit/fluid/SEFluidCircuit.h"
#include "circuit/fluid/SEFluidCircuitNode.h"
#include "circuit/fluid/SEFluidCircuitPath.h"

#include "circuit/thermal/SEThermalCircuit.h"
#include "circuit/thermal/SEThermalCircuitNode.h"
#include "circuit/thermal/SEThermalCircuitPath.h"

#include "circuit/electrical/SEElectricalCircuit.h"
#include "circuit/electrical/SEElectricalCircuitNode.h"
#include "circuit/electrical/SEElectricalCircuitPath.h"


template<CIRCUIT_LEDGER_TEMPLATE> NodeType&          SECircuitManager::CreateNode(const std::string& name, SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger)
{
  NodeType* node = ledger.nodes[name];
  if (node != nullptr)
  {
    Error("Node already exists with name " + name + ", not creating a new node, returning the existing node");
    return *node;
  }
  node = new NodeType(name, GetLogger());
  ledger.nodes[name] = node;
  return *node;
}
template<CIRCUIT_LEDGER_TEMPLATE> void               SECircuitManager::DeleteNode(const std::string& name, SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger)
{
  PathType* path;
  NodeType* node = GetNode(name, ledger);
  if (node == nullptr)
    return;
  // Remove this node from any circuits
  for (auto citr : ledger.circuits)
    citr.second->RemoveNode(*node);
  // Search the paths and delete any paths that has this node
  // And remove those paths from any circuits
  for (auto pitr = ledger.paths.begin(); pitr != ledger.paths.end();)
  {
    if (pitr->second->GetSourceNode().GetName() == name)
    {
      path = pitr->second;
      ledger.paths.erase(pitr++);
      for (auto citr : ledger.circuits)
        citr.second->RemovePath(*path);
      SAFE_DELETE(path);
    }
    else
      pitr++;
  }
  ledger.nodes.erase(node->GetName());
  SAFE_DELETE(node);
}
template<CIRCUIT_LEDGER_TEMPLATE> bool               SECircuitManager::HasNode(const std::string& name, const SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger) const
{
  return ledger.nodes.find(name) != ledger.nodes.end();
}
template<CIRCUIT_LEDGER_TEMPLATE> NodeType*          SECircuitManager::GetNode(const std::string& name, SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger)
{
  auto itr = ledger.nodes.find(name);
  if (itr != ledger.nodes.end())
    return itr->second;
  return nullptr;
}
template<CIRCUIT_LEDGER_TEMPLATE> const NodeType*    SECircuitManager::GetNode(const std::string& name, const SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger) const
{
  auto itr = ledger.nodes.find(name);
  if (itr != ledger.nodes.end())
    return itr->second;
  return nullptr;
}
template<CIRCUIT_LEDGER_TEMPLATE> PathType&          SECircuitManager::CreatePath(NodeType& src, NodeType& tgt, const std::string& name, SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger)
{
  PathType* path = ledger.paths[name];
  if (path != nullptr)
  {
    Error(" path already exists with name " + name + ", not creating a new path, returning the existing path");
    return *path;
  }
  path = new PathType(src, tgt, name);
  ledger.paths[name] = path;
  return *path;
}
template<CIRCUIT_LEDGER_TEMPLATE> void               SECircuitManager::DeletePath(const std::string& name, SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger)
{
  PathType* path = GetPath(name, ledger);
  if (path == nullptr)
    return;
  ledger.paths.erase(name);
  for (auto itr : ledger.circuits)
    itr.second->RemovePath(*path);
  SAFE_DELETE(path);
}
template<CIRCUIT_LEDGER_TEMPLATE> bool               SECircuitManager::HasPath(const std::string& name, const SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger) const
{
  return ledger.paths.find(name) != ledger.paths.end();
}
template<CIRCUIT_LEDGER_TEMPLATE> PathType*          SECircuitManager::GetPath(const std::string& name, SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger)
{
  auto itr = ledger.paths.find(name);
  if (itr != ledger.paths.end())
    return itr->second;
  return nullptr;
}
template<CIRCUIT_LEDGER_TEMPLATE> const PathType*    SECircuitManager::GetPath(const std::string& name, const SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger) const
{
  auto itr = ledger.paths.find(name);
  if (itr != ledger.paths.end())
    return itr->second;
  return nullptr;
}
template<CIRCUIT_LEDGER_TEMPLATE> CircuitType&       SECircuitManager::CreateCircuit(const std::string& name, SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger)
{
  CircuitType* circuit = ledger.circuits[name];
  if (circuit != nullptr)
  {
    Error(" Circuit already exists with name " + name + ", not creating a new circuit, returning the existing circuit");
    return *circuit;
  }
  circuit = new CircuitType(name, *this);
  ledger.circuits[name] = circuit;
  return *circuit;
}
template<CIRCUIT_LEDGER_TEMPLATE> void               SECircuitManager::DeleteCircuit(const std::string& name, SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger)
{
  CircuitType* circuit = GetCircuit(name, ledger);
  if (circuit == nullptr)
    return;
  ledger.circuits.erase(name);
  SAFE_DELETE(circuit);
}
template<CIRCUIT_LEDGER_TEMPLATE> bool               SECircuitManager::HasCircuit(const std::string& name, const SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger) const
{
  return ledger.circuits.find(name) != ledger.circuits.end();
}
template<CIRCUIT_LEDGER_TEMPLATE> CircuitType*       SECircuitManager::GetCircuit(const std::string& name, SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger)
{
  auto itr = ledger.circuits.find(name);
  if (itr != ledger.circuits.end())
    return itr->second;
  return nullptr;
}
template<CIRCUIT_LEDGER_TEMPLATE> const CircuitType* SECircuitManager::GetCircuit(const std::string& name, const SECircuitLedger<CIRCUIT_LEDGER_TYPES>& ledger) const
{
  auto itr = ledger.circuits.find(name);
  if (itr != ledger.circuits.end())
    return itr->second;
  return nullptr;
}
