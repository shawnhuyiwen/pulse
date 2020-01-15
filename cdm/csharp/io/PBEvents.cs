/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using System.Collections.Generic;
using Google.Protobuf;

namespace Pulse.CDM
{
  public class PBEvents
  {
    public static void Load(Cdm.ActiveEventListData src, List<SEActiveEvent> dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(Cdm.ActiveEventListData src, List<SEActiveEvent> dst)
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
        Cdm.ActiveEventListData data = JsonParser.Default.Parse<Cdm.ActiveEventListData>(src);
        PBEvents.Load(data, dst);
      }
      catch (Google.Protobuf.InvalidJsonException)
      {
        return false;
      }
      return true;
    }

    public static Cdm.ActiveEventListData Unload(List<SEActiveEvent> src)
    {
      Cdm.ActiveEventListData dst = new Cdm.ActiveEventListData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(List<SEActiveEvent> src, Cdm.ActiveEventListData dst)
    {
      foreach (var active in src)
      {
        Cdm.ActiveEventData ae = new Cdm.ActiveEventData();
        ae.Event = (Cdm.eEvent)active.Event;
        ae.Duration = PBProperty.Unload(active.Duration);
        dst.ActiveEvent.Add(ae);
      }
    }
    public static string SerializeToString(List<SEActiveEvent> src)
    {
      var pb = PBEvents.Unload(src);
      return pb.ToString();
    }

    public static void Load(Cdm.EventChangeListData src, List<SEEventChange> dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(Cdm.EventChangeListData src, List<SEEventChange> dst)
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
        Cdm.EventChangeListData data = JsonParser.Default.Parse<Cdm.EventChangeListData>(src);
        PBEvents.Load(data, dst);
      }
      catch (Google.Protobuf.InvalidJsonException)
      {
        return false;
      }
      return true;
    }

    public static Cdm.EventChangeListData Unload(List<SEEventChange> src)
    {
      Cdm.EventChangeListData dst = new Cdm.EventChangeListData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(List<SEEventChange> src, Cdm.EventChangeListData dst)
    {
      foreach (var change in src)
      {
        Cdm.EventChangeData c = new Cdm.EventChangeData();
        c.Event = (Cdm.eEvent)change.Event;
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
