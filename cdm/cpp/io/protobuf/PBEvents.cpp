/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "io/protobuf/PBEvents.h"
#include "io/protobuf/PBProperties.h"
#include "io/protobuf/PBUtils.h"
#include "bind/cpp/cdm/Events.pb.h"
#include "engine/SEEventManager.h"
#include "utils/FileUtils.h"

const std::string& eEvent_Name(eEvent m)
{
  return cdm::eEvent_Name((cdm::eEvent)m);
}

bool PBEvents::SerializeToString(std::vector<const SEEventChange*>& changes, std::string& output, SerializationFormat m, Logger* logger)
{
  if (changes.empty())
    return false;

  cdm::EventChangeListData data;
  for (const SEEventChange* c : changes)
  {
    cdm::EventChangeData* change = data.add_change();
    change->set_event((cdm::eEvent)c->GetEvent());
    change->set_active(c->GetActive());
    if (c->GetSimTime().IsValid())
      change->set_allocated_simtime(PBProperty::Unload(c->GetSimTime()));
  }
  if (!PBUtils::SerializeToString(data, output, m, logger))
    return false;
  return true;
}

bool PBEvents::SerializeFromString(const std::string& src, std::vector<const SEEventChange*>& changes, SerializationFormat m, Logger* logger)
{
  SEScalarTime time;
  cdm::EventChangeListData data;
  if (!PBUtils::SerializeFromString(src, data, m, logger))
    return false;
  for (int i = 0; i < data.change_size(); i++)
  {
    auto& e = data.change()[i];
    if (e.has_simtime())
      PBProperty::Load(e.simtime(), time);
    else
      time.Invalidate();
    SEEventChange* c = new SEEventChange((eEvent)e.event(), e.active(), &time);
    changes.push_back(c);
  }

  return true;
}

bool PBEvents::SerializeToString(std::vector<const SEActiveEvent*>& active_events, std::string& output, SerializationFormat m, Logger* logger)
{
  if (active_events.empty())
    return false;

  cdm::ActiveEventListData data;
  for (const SEActiveEvent* a : active_events)
  {
    cdm::ActiveEventData* aeData = data.add_activeevent();
    aeData->set_event((cdm::eEvent)a->GetEvent());
    if (a->GetDuration().IsValid())
      aeData->set_allocated_duration(PBProperty::Unload(a->GetDuration()));
  }
  if (!PBUtils::SerializeToString(data, output, m, logger))
    return false;
  return true;
}

bool PBEvents::SerializeFromString(const std::string& src, std::vector<const SEActiveEvent*>& active_events, SerializationFormat m, Logger* logger)
{
  SEScalarTime time;
  cdm::ActiveEventListData data;
  if (!PBUtils::SerializeFromString(src, data, m, logger))
    return false;
  for (int i = 0; i < data.activeevent_size(); i++)
  {
    auto& ae = data.activeevent()[i];
    if (ae.has_duration())
      PBProperty::Load(ae.duration(), time);
    else
      time.Invalidate();
    SEActiveEvent* c = new SEActiveEvent((eEvent)ae.event(), time);
    active_events.push_back(c);
  }

  return true;
}