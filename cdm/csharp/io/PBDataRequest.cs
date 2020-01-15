/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using System.Collections.Generic;
using Google.Protobuf;
using static Cdm.DataRequestData.Types;

namespace Pulse.CDM
{
  public class PBDataRequest
  {
    public static void Load(Cdm.DataRequestManagerData src, SEDataRequestManager dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(Cdm.DataRequestManagerData src, SEDataRequestManager dst)
    {
      dst.SetResultsFilename(src.ResultsFilename);
      dst.SetSamplesPerSecond(src.SamplesPerSecond);
      for (int i = 0; i < src.DataRequest.Count; i++)
      {
        Cdm.DataRequestData dr = src.DataRequest[i];
        dst.GetDataRequests().Add(SEDataRequest.New((eDataRequest_Category)dr.Category, dr.CompartmentName, dr.SubstanceName, dr.PropertyName, dr.Unit));
      }
    }
    public static bool SerializeFromString(string src, SEDataRequestManager dst)
    {
      try
      {
        Cdm.DataRequestManagerData data = JsonParser.Default.Parse<Cdm.DataRequestManagerData>(src);
        PBDataRequest.Load(data, dst);
      }
      catch (Google.Protobuf.InvalidJsonException)
      {
        return false;
      }
      return true;
    }

    public static Cdm.DataRequestManagerData Unload(SEDataRequestManager src)
    {
      Cdm.DataRequestManagerData dst = new Cdm.DataRequestManagerData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEDataRequestManager src, Cdm.DataRequestManagerData dst)
    {
      if (src.HasResultsFilename())
        dst.ResultsFilename = src.GetResultFilename();
      dst.SamplesPerSecond = src.GetSamplesPerSecond();
      foreach (var dr in src.GetDataRequests())
      {
        Cdm.DataRequestData dst_dr = new Cdm.DataRequestData();
        dst_dr.Category = (eCategory)dr.GetCategory();
        if (dr.HasCompartmentName())
          dst_dr.CompartmentName = dr.GetCompartmentName();
        if (dr.HasSubstanceName())
          dst_dr.SubstanceName = dr.GetSubstanceName();
        if (dr.HasPropertyName())
          dst_dr.PropertyName = dr.GetPropertyName();
        if (dr.HasUnit())
          dst_dr.Unit = dr.GetUnit();
        dst.DataRequest.Add(dst_dr);
      }
    }
    public static string SerializeToString(SEDataRequestManager src)
    {
      var pb = PBDataRequest.Unload(src);
      return pb.ToString();
    }
  }
}
