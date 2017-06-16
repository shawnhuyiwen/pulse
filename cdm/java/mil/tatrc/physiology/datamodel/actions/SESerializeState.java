/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/

package mil.tatrc.physiology.datamodel.actions;

import com.kitware.physiology.cdm.Actions.SerializeStateData;
import com.kitware.physiology.cdm.Actions.SerializeStateData.eSerializationType;

public class SESerializeState extends SEAction 
{
  protected String             filename;
  protected eSerializationType type;
  
  public SESerializeState() 
  {
    filename = "";
    type = null;
  }
  
  public void copy(SESerializeState other)
  {
    if(this==other)
      return;
    super.copy(other);
    this.filename = other.filename;
    this.type = other.type;
  }
  
  public void reset() 
  {
    super.reset();
    filename = "";
    type = null;
  }
  
  public boolean isValid()
  {
    return hasType();
  }

  public static void load(SerializeStateData src, SESerializeState dst) 
  {
    SEAction.load(src.getAction(), dst);
    if(src.getType()!=eSerializationType.UNRECOGNIZED)
      dst.type = src.getType();
    if(src.getFilename()!=null)
      dst.setFilename(src.getFilename());
  }
  public static SerializeStateData unload(SESerializeState src)
  {
    SerializeStateData.Builder dst = SerializeStateData.newBuilder();
    unload(src,dst);
    return dst.build();
  }  
  protected static void unload(SESerializeState src, SerializeStateData.Builder dst)
  {
    SEAction.unload(src, dst.getActionBuilder());
    if(src.hasFilename())
      dst.setFilename(src.filename);
    if(src.hasType())
      dst.setType(src.type);
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
  
  public boolean hasType()
  {
    return type!=null;
  }
  public eSerializationType getType() 
  {
    return type;
  }
  public void setType(eSerializationType t)
  {
    this.type = t;
  }
  public void invalidateType()
  {
    this.type = null;
  }
  
  public String toString() 
  {
    return "Save State" 
        + "\n\tType: " + getType()
        + "\n\tFilename: " + getFilename();
  }
}
