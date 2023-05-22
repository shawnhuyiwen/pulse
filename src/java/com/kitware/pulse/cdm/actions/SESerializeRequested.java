/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.actions;

import com.kitware.pulse.cdm.bind.Actions.SerializeRequestedData;

public class SESerializeRequested extends SEAction 
{
  private static final long serialVersionUID = 3176123488867413596L;
  
  protected String              filename;
  
  public SESerializeRequested() 
  {
    filename = "";
  }
  
  public void copy(SESerializeRequested other)
  {
    if(this==other)
      return;
    super.copy(other);
    this.filename = other.filename;
  }
  
  @Override
  public void clear() 
  {
    super.clear();
    filename = "";
  }
  
  @Override
  public boolean isValid()
  {
    return hasFilename();
  }

  public static void load(SerializeRequestedData src, SESerializeRequested dst) 
  {
    SEAction.load(src.getAction(), dst);
    if(src.getFilename()!=null)
      dst.setFilename(src.getFilename());
  }
  public static SerializeRequestedData unload(SESerializeRequested src)
  {
    SerializeRequestedData.Builder dst = SerializeRequestedData.newBuilder();
    unload(src,dst);
    return dst.build();
  }  
  protected static void unload(SESerializeRequested src, SerializeRequestedData.Builder dst)
  {
    SEAction.unload(src, dst.getActionBuilder());
    if(src.hasFilename())
      dst.setFilename(src.filename);
  }
  
  public boolean hasFilename()
  {
    return !(filename==null || filename.isEmpty());
  }
  public String getFilename() 
  {
    return filename;
  }
  public void setFilename(String filename)
  {
    this.filename = filename;
  }
  public void invalidateFilename()
  {
    this.filename = "";
  }
  
  @Override
  public String toString() 
  {
    return "Serialize Requested" 
        + "\n\tFilename: " + getFilename();
  }
}
