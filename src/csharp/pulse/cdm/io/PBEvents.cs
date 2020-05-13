/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using System.Collections.Generic;
using Google.Protobuf;

namespace Pulse.CDM
{
  public class PBEvents
  {
    public static void Load(pulse.cdm.bind.ActiveEventListData src, List<SEActiveEvent> dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ActiveEventListData src, List<SEActiveEvent> dst)
    {
      SEScalarTime duration = new SEScalarTime();
      foreach (var active in src.ActiveEvent)
      {
        PBProperty.Load(active.Duration, duration);
        dst.Add(new SEActiveEvent((eEvent)active.Event, duration));
      }
    }
    public static bool SerializeFromString(string src, List<SEActiveEvent> dst)
    {
      try
      {
        pulse.cdm.bind.ActiveEventListData data = JsonParser.Default.Parse<pulse.cdm.bind.ActiveEventListData>(src);
        PBEvents.Load(data, dst);
      }
      catch (Google.Protobuf.InvalidJsonException)
      {
        return false;
      }
      return true;
    }

    public static pulse.cdm.bind.ActiveEventListData Unload(List<SEActiveEvent> src)
    {
      pulse.cdm.bind.ActiveEventListData dst = new pulse.cdm.bind.ActiveEventListData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(List<SEActiveEvent> src, pulse.cdm.bind.ActiveEventListData dst)
    {
      foreach (var active in src)
      {
        pulse.cdm.bind.ActiveEventData ae = new pulse.cdm.bind.ActiveEventData();
        ae.Event = (pulse.cdm.bind.eEvent)active.Event;
        ae.Duration = PBProperty.Unload(active.Duration);
        dst.ActiveEvent.Add(ae);
      }
    }
    public static string SerializeToString(List<SEActiveEvent> src)
    {
      var pb = PBEvents.Unload(src);
      return pb.ToString();
    }

    public static void Load(pulse.cdm.bind.EventChangeListData src, List<SEEventChange> dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.EventChangeListData src, List<SEEventChange> dst)
    {
      SEScalarTime simTime = new SEScalarTime();
      foreach (var change in src.Change)
      {
        PBProperty.Load(change.SimTime, simTime);
        dst.Add(new SEEventChange((eEvent)change.Event, change.Active, simTime));
      }
    }
    public static bool SerializeFromString(string src, List<SEEventChange> dst)
    {
      try
      {
        pulse.cdm.bind.EventChangeListData data = JsonParser.Default.Parse<pulse.cdm.bind.EventChangeListData>(src);
        PBEvents.Load(data, dst);
      }
      catch (Google.Protobuf.InvalidJsonException)
      {
        return false;
      }
      return true;
    }

    public static pulse.cdm.bind.EventChangeListData Unload(List<SEEventChange> src)
    {
      pulse.cdm.bind.EventChangeListData dst = new pulse.cdm.bind.EventChangeListData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(List<SEEventChange> src, pulse.cdm.bind.EventChangeListData dst)
    {
      foreach (var change in src)
      {
        pulse.cdm.bind.EventChangeData c = new pulse.cdm.bind.EventChangeData();
        c.Event = (pulse.cdm.bind.eEvent)change.Event;
        c.Active = change.Active;
        if (change.SimTime.IsValid())
          c.SimTime = PBProperty.Unload(change.SimTime);
        dst.Change.Add(c);
      }
    }
    public static string SerializeToString(List<SEEventChange> src)
    {
      var pb = PBEvents.Unload(src);
      return pb.ToString();
    }
  }
}
