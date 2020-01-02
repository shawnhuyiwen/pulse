/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public enum eDataRequest_Category : int
  {
    Patient = 0, Physiology, Environment, GasCompartment,
    LiquidCompartment, ThermalCompartment, TissueCompartment, Substance,
    AnesthesiaMachine, ECG, Inhaler
  };

  public class SEDataRequest
  {
    protected eDataRequest_Category Category;
    protected string CompartmentName = null;
    protected string SubstanceName = null;
    protected string PropertyName = null;
    protected string Unit = null;

    protected SEDataRequest(eDataRequest_Category category)
    {
      this.Category = category;
    }

    public eDataRequest_Category GetCategory() { return Category; }
    public bool HasCompartmentName() { return CompartmentName != null; }
    public string GetCompartmentName() { return CompartmentName; }
    public bool HasSubstanceName() { return SubstanceName != null; }
    public string GetSubstanceName() { return SubstanceName; }
    public bool HasPropertyName() { return PropertyName != null; }
    public string GetPropertyName() { return PropertyName; }
    public bool HasUnit() { return Unit != null; }
    public string GetUnit() { return Unit; }

    public new string ToString()
    {
      string str = "";
      if (HasCompartmentName())
        str = str + CompartmentName + "-";
      if (HasSubstanceName())
        str = str + SubstanceName + "-";
      str = str + PropertyName + "(" + Unit + ")";
      return str;
    }

    public static SEDataRequest New(eDataRequest_Category category, string compartment, string substance, string property, string unit)
    {
      SEDataRequest dr = new SEDataRequest(category);
      dr.CompartmentName = compartment;
      dr.SubstanceName = substance;
      dr.PropertyName = property;
      dr.Unit = unit;
      return dr;
    }

    public static SEDataRequest CreatePatientRequest(string property)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.Patient);
      dr.PropertyName = property;
      return dr;
    }
    public static SEDataRequest CreatePatientRequest(string property, string unit)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.Patient);
      dr.PropertyName = property;
      dr.Unit = unit;
      return dr;
    }

    public static SEDataRequest CreatePhysiologyRequest(string property)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.Physiology);
      dr.PropertyName = property;
      return dr;
    }
    public static SEDataRequest CreatePhysiologyRequest(string property, string unit)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.Physiology);
      dr.PropertyName = property;
      dr.Unit = unit;
      return dr;
    }

    public static SEDataRequest CreateEnvironmentRequest(string property)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.Environment);
      dr.PropertyName = property;
      return dr;
    }
    public static SEDataRequest CreateEnvironmentRequest(string property, string unit)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.Environment);
      dr.PropertyName = property;
      dr.Unit = unit;
      return dr;
    }

    public static SEDataRequest CreateGasCompartmentRequest(string compartment, string property)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.GasCompartment);
      dr.PropertyName = property;
      dr.CompartmentName = compartment;
      return dr;
    }
    public static SEDataRequest CreateGasCompartmentRequest(string compartment, string property, string unit)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.GasCompartment);
      dr.PropertyName = property;
      dr.CompartmentName = compartment;
      dr.Unit = unit;
      return dr;
    }
    public static SEDataRequest CreateGasCompartmentSubstanceRequest(string compartment, string substance, string property)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.GasCompartment);
      dr.PropertyName = property;
      dr.CompartmentName = compartment;
      dr.SubstanceName = substance;
      return dr;
    }
    public static SEDataRequest CreateGasCompartmentSubstanceRequest(string compartment, string substance, string property, string unit)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.GasCompartment);
      dr.PropertyName = property;
      dr.CompartmentName = compartment;
      dr.SubstanceName = substance;
      dr.Unit = unit;
      return dr;
    }

    public static SEDataRequest CreateLiquidCompartmentRequest(string compartment, string property)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.LiquidCompartment);
      dr.PropertyName = property;
      dr.CompartmentName = compartment;
      return dr;
    }
    public static SEDataRequest CreateLiquidCompartmentRequest(string compartment, string property, string unit)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.LiquidCompartment);
      dr.PropertyName = property;
      dr.CompartmentName = compartment;
      dr.Unit = unit;
      return dr;
    }
    public static SEDataRequest CreateLiquidCompartmentSubstanceRequest(string compartment, string substance, string property)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.LiquidCompartment);
      dr.PropertyName = property;
      dr.CompartmentName = compartment;
      dr.SubstanceName = substance;
      return dr;
    }
    public static SEDataRequest CreateLiquidCompartmentSubstanceRequest(string compartment, string substance, string property, string unit)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.LiquidCompartment);
      dr.PropertyName = property;
      dr.CompartmentName = compartment;
      dr.SubstanceName = substance;
      dr.Unit = unit;
      return dr;
    }

    public static SEDataRequest CreateThermalCompartmentRequest(string compartment, string property)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.ThermalCompartment);
      dr.PropertyName = property;
      dr.CompartmentName = compartment;
      return dr;
    }
    public static SEDataRequest CreateThermalCompartmentRequest(string compartment, string property, string unit)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.ThermalCompartment);
      dr.PropertyName = property;
      dr.CompartmentName = compartment;
      dr.Unit = unit;
      return dr;
    }

    public static SEDataRequest CreateTissueCompartmentRequest(string compartment, string property)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.TissueCompartment);
      dr.PropertyName = property;
      dr.CompartmentName = compartment;
      return dr;
    }
    public static SEDataRequest CreateTissueCompartmentRequest(string compartment, string property, string unit)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.TissueCompartment);
      dr.PropertyName = property;
      dr.CompartmentName = compartment;
      dr.Unit = unit;
      return dr;
    }

    public static SEDataRequest CreateSubstanceRequest(string substance, string property)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.Substance);
      dr.PropertyName = property;
      dr.SubstanceName = substance;
      return dr;
    }
    public static SEDataRequest CreateSubstanceRequest(string substance, string property, string unit)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.Substance);
      dr.PropertyName = property;
      dr.SubstanceName = substance;
      dr.Unit = unit;
      return dr;
    }

    public static SEDataRequest CreateECGRequest(string property)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.ECG);
      dr.PropertyName = property;
      return dr;
    }
    public static SEDataRequest CreateECGRequest(string property, string unit)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.ECG);
      dr.PropertyName = property;
      dr.Unit = unit;
      return dr;
    }
  }
}
