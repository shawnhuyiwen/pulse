/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using System.Collections.Generic;
using Google.Protobuf;


public class PBLogMessages
{
  public static void Load(Cdm.LogMessagesData src, SELogMessages dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.LogMessagesData src, SELogMessages dst)
  {
    dst.Clear();
    foreach (var msg in src.DebugMessages)
      dst.debug_msgs.Add(msg);
    foreach (var msg in src.InfogMessages)
      dst.info_msgs.Add(msg);
    foreach (var msg in src.WarningMessages)
      dst.warning_msgs.Add(msg);
    foreach (var msg in src.ErrorMessages)
      dst.error_msgs.Add(msg);
    foreach (var msg in src.FatalMessages)
      dst.fatal_msgs.Add(msg);
  }
  public static bool SerializeFromString(string src, SELogMessages dst)
  {
    try
    {
      Cdm.LogMessagesData data = JsonParser.Default.Parse<Cdm.LogMessagesData>(src);
      PBLogMessages.Load(data, dst);
    }
    catch (Google.Protobuf.InvalidJsonException)
    {
      return false;
    }
    return true;
  }

  public static Cdm.LogMessagesData Unload(SELogMessages src)
  {
    Cdm.LogMessagesData dst = new Cdm.LogMessagesData();
    Serialize(src, dst);
    return dst;
  }
  public static void Serialize(SELogMessages src, Cdm.LogMessagesData dst)
  {
    foreach (string msg in src.debug_msgs)
      dst.DebugMessages.Add(msg);
    foreach (string msg in src.info_msgs)
      dst.InfogMessages.Add(msg);
    foreach (string msg in src.warning_msgs)
      dst.WarningMessages.Add(msg);
    foreach (string msg in src.error_msgs)
      dst.ErrorMessages.Add(msg);
    foreach (string msg in src.fatal_msgs)
      dst.FatalMessages.Add(msg);
  }
  public static string SerializeToString(SELogMessages src)
  {
    var pb = PBLogMessages.Unload(src);
    return pb.ToString();
  }

}
