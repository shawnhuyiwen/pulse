/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.patient.actions;

import com.kitware.pulse.cdm.bind.PatientActions.ImpairedAlveolarExchangeExacerbationData;
import com.kitware.pulse.cdm.properties.SEScalar0To1;
import com.kitware.pulse.cdm.properties.SEScalarArea;

public class SEImpairedAlveolarExchangeExacerbation extends SEPatientAction
{

  private static final long serialVersionUID = 5044648250444265871L;
  protected SEScalarArea impairedSurfaceArea;
  protected SEScalar0To1 impairedFraction;
  protected SEScalar0To1 severity;
  
  public SEImpairedAlveolarExchangeExacerbation()
  {
    impairedSurfaceArea = null;
    impairedFraction = null;
    severity = null;
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

    if (other.severity != null)
      getSeverity().set(other.getSeverity());
    else if (severity != null)
      severity.invalidate();
  }
  
  @Override
  public void clear()
  {
    super.clear();
    if (impairedSurfaceArea != null)
      impairedSurfaceArea.invalidate();
    if (impairedFraction != null)
      impairedFraction.invalidate();
    if (severity != null)
      severity.invalidate();
  }
  
  @Override
  public boolean isValid()
  {
    return hasImpairedSurfaceArea() || hasImpairedFraction() || hasSeverity();
  }
  
  public static void load(ImpairedAlveolarExchangeExacerbationData src, SEImpairedAlveolarExchangeExacerbation dst)
  {
    SEPatientAction.load(src.getPatientAction(), dst);
    if(src.hasImpairedSurfaceArea())
      SEScalarArea.load(src.getImpairedSurfaceArea(),dst.getImpairedSurfaceArea());
    else if(src.hasImpairedFraction())
      SEScalar0To1.load(src.getImpairedFraction(),dst.getImpairedFraction());
    else if(src.hasSeverity())
      SEScalar0To1.load(src.getSeverity(),dst.getSeverity());
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
    else if (src.hasImpairedFraction())
      dst.setImpairedFraction(SEScalar0To1.unload(src.impairedFraction));
    else if (src.hasSeverity())
      dst.setSeverity(SEScalar0To1.unload(src.severity));
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
  
  public boolean hasSeverity()
  {
    return severity == null ? false : severity.isValid();
  }
  public SEScalar0To1 getSeverity()
  {
    if (severity == null)
      severity = new SEScalar0To1();
    return severity;
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
    else if (severity != null)
      return "Severity" 
          + "\n\tSeverity: " + getSeverity();
    else
      return "Action not specified properly";
  }
}
