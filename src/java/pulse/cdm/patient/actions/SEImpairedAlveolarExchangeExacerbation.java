/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.cdm.patient.actions;

import pulse.cdm.bind.PatientActions.ImpairedAlveolarExchangeExacerbationData;
import pulse.cdm.properties.SEScalar0To1;
import pulse.cdm.properties.SEScalarArea;

public class SEImpairedAlveolarExchangeExacerbation extends SEPatientAction
{

  private static final long serialVersionUID = 5044648250444265871L;
  protected SEScalarArea impairedSurfaceArea;
  protected SEScalar0To1 impairedFraction;
  
  public SEImpairedAlveolarExchangeExacerbation()
  {
    impairedSurfaceArea = null;
    impairedFraction = null;
  }
  
  public void copy(SEImpairedAlveolarExchangeExacerbation other)
  {
    if(this==other)
      return;
    super.copy(other);
    if (other.impairedSurfaceArea != null)
      getImpairedSurfaceArea().set(other.getImpairedSurfaceArea());
    else if (impairedSurfaceArea != null)
      impairedSurfaceArea.invalidate();
    if (other.impairedFraction != null)
      getImpairedFraction().set(other.getImpairedFraction());
    else if (impairedFraction != null)
      impairedFraction.invalidate();
  }
  
  @Override
  public void reset()
  {
    super.reset();
    if (impairedSurfaceArea != null)
      impairedSurfaceArea.invalidate();
    if (impairedFraction != null)
      impairedFraction.invalidate();
  }
  
  @Override
  public boolean isValid()
  {
    return hasImpairedSurfaceArea() || hasImpairedFraction();
  }
  
  public static void load(ImpairedAlveolarExchangeExacerbationData src, SEImpairedAlveolarExchangeExacerbation dst)
  {
    SEPatientAction.load(src.getPatientAction(), dst);
    if(src.hasImpairedSurfaceArea())
      SEScalarArea.load(src.getImpairedSurfaceArea(),dst.getImpairedSurfaceArea());
    if(src.hasImpairedFraction())
      SEScalar0To1.load(src.getImpairedFraction(),dst.getImpairedFraction());
  }
  
  public static ImpairedAlveolarExchangeExacerbationData unload(SEImpairedAlveolarExchangeExacerbation src)
  {
    ImpairedAlveolarExchangeExacerbationData.Builder dst = ImpairedAlveolarExchangeExacerbationData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SEImpairedAlveolarExchangeExacerbation src, ImpairedAlveolarExchangeExacerbationData.Builder dst)
  {
    SEPatientAction.unload(src, dst.getPatientActionBuilder());
    if (src.hasImpairedSurfaceArea())
      dst.setImpairedSurfaceArea(SEScalarArea.unload(src.impairedSurfaceArea));
    if (src.hasImpairedFraction())
      dst.setImpairedFraction(SEScalar0To1.unload(src.impairedFraction));
  }
  
  public boolean hasImpairedSurfaceArea()
  {
    return impairedSurfaceArea == null ? false : impairedSurfaceArea.isValid();
  }
  public SEScalarArea getImpairedSurfaceArea()
  {
    if (impairedSurfaceArea == null)
      impairedSurfaceArea = new SEScalarArea();
    return impairedSurfaceArea;
  }
  
  public boolean hasImpairedFraction()
  {
    return impairedFraction == null ? false : impairedFraction.isValid();
  }
  public SEScalar0To1 getImpairedFraction()
  {
    if (impairedFraction == null)
      impairedFraction = new SEScalar0To1();
    return impairedFraction;
  }
  
  @Override
  public String toString()
  {
    if (impairedFraction != null)
      return "ImpairedAlveolarExchange" 
          + "\n\tImpairedFraction: " + getImpairedFraction();
    else if (impairedSurfaceArea != null)
      return "ImpairedAlveolarExchange" 
          + "\n\tImpairedSurfaceArea: " + getImpairedSurfaceArea();
    else
      return "Action not specified properly";
  }
}
