/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package pulse.cdm.engine;

import java.util.HashMap;
import java.util.Map;

import com.google.protobuf.InvalidProtocolBufferException;
import com.google.protobuf.util.JsonFormat;

import pulse.cdm.bind.Engine.TimedStabilizationData;
import pulse.cdm.bind.Enums.eSwitch;
import pulse.cdm.properties.SEScalarTime;
import pulse.utilities.FileUtils;

public class SETimedStabilization
{
  protected eSwitch                  trackingStabilization;
  protected SEScalarTime             restingStabilizationTime;
  protected SEScalarTime             feedbackStabilizationTime;
  protected Map<String,SEScalarTime> conditionStabilizationTimes;
  
  public SETimedStabilization()
  {
    super();
    this.conditionStabilizationTimes=new HashMap<String,SEScalarTime>();
  }
  
  public void reset()
  {
    reset();
    this.trackingStabilization=eSwitch.Off;
    this.restingStabilizationTime=null;
    this.feedbackStabilizationTime=null;
    this.conditionStabilizationTimes.clear();
  }
  
  public void readFile(String fileName) throws InvalidProtocolBufferException
  {
    TimedStabilizationData.Builder builder = TimedStabilizationData.newBuilder();
    JsonFormat.parser().merge(FileUtils.readFile(fileName), builder);
    SETimedStabilization.load(builder.build(), this);
  }
  public void writeFile(String fileName) throws InvalidProtocolBufferException
  {
    FileUtils.writeFile(fileName, JsonFormat.printer().print(SETimedStabilization.unload(this)));
  }
  
  public static void load(TimedStabilizationData src, SETimedStabilization dst) 
  {
    if(src.getTrackingStabilization()!=eSwitch.UNRECOGNIZED && src.getTrackingStabilization()!=eSwitch.NullSwitch)
      dst.trackingStabilization=src.getTrackingStabilization();
    if(src.hasRestingStabilizationTime())
      SEScalarTime.load(src.getRestingStabilizationTime(),dst.getRestingStabilizationTime());
    if(src.hasFeedbackStabilizationTime())
      SEScalarTime.load(src.getFeedbackStabilizationTime(),dst.getFeedbackStabilizationTime());
    for(String name : src.getConditionStabilizationMap().keySet())
    {
      SEScalarTime.load(src.getConditionStabilizationMap().get(name),dst.createConditionStabilizationTime(name));     
    }
  }
  
  public static TimedStabilizationData unload(SETimedStabilization src)
  {
    TimedStabilizationData.Builder dst = TimedStabilizationData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SETimedStabilization src, TimedStabilizationData.Builder dst)
  {
  	if(src.trackingStabilization!=null)
  		dst.setTrackingStabilization(src.trackingStabilization);
    if(src.hasRestingStabilizationTime())
      dst.setRestingStabilizationTime(SEScalarTime.unload(src.restingStabilizationTime));
    if(src.hasFeedbackStabilizationTime())
      dst.setFeedbackStabilizationTime(SEScalarTime.unload(src.feedbackStabilizationTime));
    for(String name : src.conditionStabilizationTimes.keySet())
    {
    	dst.putConditionStabilization(name, SEScalarTime.unload(src.conditionStabilizationTimes.get(name)));
    }
  }
  
  public eSwitch isTrackingStabilization()
  {
    return this.trackingStabilization;
  }
  public void TrackStabilization(eSwitch b)
  {
  	this.trackingStabilization = (b==eSwitch.NullSwitch) ? eSwitch.Off : b;
  }
  
  public boolean hasRestingStabilizationTime()
  {
    return restingStabilizationTime == null ? false : restingStabilizationTime.isValid();
  }
  public SEScalarTime getRestingStabilizationTime()
  {
    if (restingStabilizationTime == null)
      restingStabilizationTime = new SEScalarTime();
    return restingStabilizationTime;
  }
  
  public boolean hasFeedbackStabilizationTime()
  {
    return feedbackStabilizationTime == null ? false : feedbackStabilizationTime.isValid();
  }
  public SEScalarTime getFeedbackStabilizationTime()
  {
    if (feedbackStabilizationTime == null)
      feedbackStabilizationTime = new SEScalarTime();
    return feedbackStabilizationTime;
  }
  
  public SEScalarTime createConditionStabilizationTime(String type)
  {
    SEScalarTime stabilizationTime = new SEScalarTime();
    this.conditionStabilizationTimes.put(type, stabilizationTime);
    return stabilizationTime;
  }
  public boolean hasConditionStabilizationTime(String type)
  {
    return this.conditionStabilizationTimes.containsKey(type);
  }
  public SEScalarTime getConditionStabilizationTime(String type)
  {
    return this.conditionStabilizationTimes.get(type);
  }
}
