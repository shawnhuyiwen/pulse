/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public enum eDataRequest_Category : int
  {
    Patient = 0, Physiology, Environment, Action, GasCompartment,
    LiquidCompartment, ThermalCompartment, TissueCompartment, Substance,
    AnesthesiaMachine, BagValveMask, ECG, Inhaler, MechanicalVentilator
  };

  public class SEDataRequest
  {
    protected eDataRequest_Category Category;
    protected string ActionName = null;
    protected string CompartmentName = null;
    protected string SubstanceName = null;
    protected string PropertyName = null;
    protected Unit Unit = null;

    protected SEDataRequest(eDataRequest_Category category)
    {
      this.Category = category;
    }

    public eDataRequest_Category GetCategory() { return Category; }
    public bool HasActionName() { return ActionName != null; }
    public string GetActionName() { return ActionName; }
    public bool HasCompartmentName() { return CompartmentName != null; }
    public string GetCompartmentName() { return CompartmentName; }
    public bool HasSubstanceName() { return SubstanceName != null; }
    public string GetSubstanceName() { return SubstanceName; }
    public bool HasPropertyName() { return PropertyName != null; }
    public string GetPropertyName() { return PropertyName; }
    public bool HasUnit() { return Unit != null; }
    public Unit GetUnit() { return Unit; }

    public new string ToString()
    {
      string str = "";
      switch(Category)
      {
        case eDataRequest_Category.Patient:
          str += "Patient-";
          break;
        case eDataRequest_Category.Physiology:
          str += "Physiology-";
          break;
        case eDataRequest_Category.Environment:
          str += "Environment-";
          break;
        case eDataRequest_Category.Action:
          str += ActionName + "-";
          break;
        case eDataRequest_Category.GasCompartment:
          str += "GasCompartment-";
          str += CompartmentName + "-";
          break;
        case eDataRequest_Category.LiquidCompartment:
          str += "LiquidCompartment-";
          str += CompartmentName + "-";
          break;
        case eDataRequest_Category.ThermalCompartment:
          str += "ThermalCompartment-";
          str += CompartmentName + "-";
          break;
        case eDataRequest_Category.TissueCompartment:
          str += "TissueCompartment-";
          str+= CompartmentName + "-";
          break;
        case eDataRequest_Category.Substance:
          str += "Substance-";
          str += SubstanceName + "-";
          break;
        case eDataRequest_Category.AnesthesiaMachine:
          str += "AnesthesiaMachine-";
          break;
        case eDataRequest_Category.BagValveMask:
          str += "BagValveMask-";
          break;
        case eDataRequest_Category.ECG:
          str += "ECG-";
          break;
        case eDataRequest_Category.Inhaler:
          str += "Inhaler-";
          break;
        case eDataRequest_Category.MechanicalVentilator:
          str += "MechanicalVentilator-";
          break;
      }
      str += PropertyName + "(" + Unit + ")";
      return str;
    }

    public static SEDataRequest New(eDataRequest_Category category, string action, string compartment, string substance, string property, Unit unit)
    {
      SEDataRequest dr = new SEDataRequest(category);
      dr.ActionName = action;
      dr.CompartmentName = compartment;
      dr.SubstanceName = substance;
      dr.PropertyName = property;
      dr.Unit = unit;
      return dr;
    }

    public static SEDataRequest CreatePatientDataRequest(string property)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.Patient);
      dr.PropertyName = property;
      return dr;
    }
    public static SEDataRequest CreatePatientDataRequest(string property, Unit unit)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.Patient);
      dr.PropertyName = property;
      dr.Unit = unit;
      return dr;
    }

    public static SEDataRequest CreatePhysiologyDataRequest(string property)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.Physiology);
      dr.PropertyName = property;
      return dr;
    }
    public static SEDataRequest CreatePhysiologyDataRequest(string property, Unit unit)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.Physiology);
      dr.PropertyName = property;
      dr.Unit = unit;
      return dr;
    }

    public static SEDataRequest CreateEnvironmentDataRequest(string property)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.Environment);
      dr.PropertyName = property;
      return dr;
    }
    public static SEDataRequest CreateEnvironmentDataRequest(string property, Unit unit)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.Environment);
      dr.PropertyName = property;
      dr.Unit = unit;
      return dr;
    }

    public static SEDataRequest CreateActionDataRequest(string action, string property)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.Action);
      dr.ActionName = action;
      dr.PropertyName = property;
      return dr;
    }
    public static SEDataRequest CreateActionDataRequest(string action, string property, Unit unit)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.Action);
      dr.ActionName = action;
      dr.PropertyName = property;
      dr.Unit = unit;
      return dr;
    }
    public static SEDataRequest CreateActionCompartmentDataRequest(string action, string compartment, string property)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.Action);
      dr.ActionName = action;
      dr.CompartmentName = compartment;
      dr.PropertyName = property;
      return dr;
    }
    public static SEDataRequest CreateActionCompartmentDataRequest(string action, string compartment, string property, Unit unit)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.Action);
      dr.ActionName = action;
      dr.CompartmentName = compartment;
      dr.PropertyName = property;
      dr.Unit = unit;
      return dr;
    }
    public static SEDataRequest CreateActionSubstanceDataRequest(string action, string substance, string property)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.Action);
      dr.ActionName = action;
      dr.SubstanceName = substance;
      dr.PropertyName = property;
      return dr;
    }
    public static SEDataRequest CreateActionSubstanceDataRequest(string action, string substance, string property, Unit unit)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.Action);
      dr.ActionName = action;
      dr.SubstanceName = substance;
      dr.PropertyName = property;
      dr.Unit = unit;
      return dr;
    }

    public static SEDataRequest CreateGasCompartmentDataRequest(string compartment, string property)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.GasCompartment);
      dr.PropertyName = property;
      dr.CompartmentName = compartment;
      return dr;
    }
    public static SEDataRequest CreateGasCompartmentDataRequest(string compartment, string property, Unit unit)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.GasCompartment);
      dr.PropertyName = property;
      dr.CompartmentName = compartment;
      dr.Unit = unit;
      return dr;
    }
    public static SEDataRequest CreateGasCompartmentDataRequest(string compartment, string substance, string property)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.GasCompartment);
      dr.PropertyName = property;
      dr.CompartmentName = compartment;
      dr.SubstanceName = substance;
      return dr;
    }
    public static SEDataRequest CreateGasCompartmentDataRequest(string compartment, string substance, string property, Unit unit)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.GasCompartment);
      dr.PropertyName = property;
      dr.CompartmentName = compartment;
      dr.SubstanceName = substance;
      dr.Unit = unit;
      return dr;
    }

    public static SEDataRequest CreateLiquidCompartmentDataRequest(string compartment, string property)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.LiquidCompartment);
      dr.PropertyName = property;
      dr.CompartmentName = compartment;
      return dr;
    }
    public static SEDataRequest CreateLiquidCompartmentDataRequest(string compartment, string property, Unit unit)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.LiquidCompartment);
      dr.PropertyName = property;
      dr.CompartmentName = compartment;
      dr.Unit = unit;
      return dr;
    }
    public static SEDataRequest CreateLiquidCompartmentDataRequest(string compartment, string substance, string property)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.LiquidCompartment);
      dr.PropertyName = property;
      dr.CompartmentName = compartment;
      dr.SubstanceName = substance;
      return dr;
    }
    public static SEDataRequest CreateLiquidCompartmentDataRequest(string compartment, string substance, string property, Unit unit)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.LiquidCompartment);
      dr.PropertyName = property;
      dr.CompartmentName = compartment;
      dr.SubstanceName = substance;
      dr.Unit = unit;
      return dr;
    }

    public static SEDataRequest CreateThermalCompartmentDataRequest(string compartment, string property)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.ThermalCompartment);
      dr.PropertyName = property;
      dr.CompartmentName = compartment;
      return dr;
    }
    public static SEDataRequest CreateThermalCompartmentDataRequest(string compartment, string property, Unit unit)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.ThermalCompartment);
      dr.PropertyName = property;
      dr.CompartmentName = compartment;
      dr.Unit = unit;
      return dr;
    }

    public static SEDataRequest CreateTissueCompartmentDataRequest(string compartment, string property)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.TissueCompartment);
      dr.PropertyName = property;
      dr.CompartmentName = compartment;
      return dr;
    }
    public static SEDataRequest CreateTissueCompartmentDataRequest(string compartment, string property, Unit unit)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.TissueCompartment);
      dr.PropertyName = property;
      dr.CompartmentName = compartment;
      dr.Unit = unit;
      return dr;
    }

    public static SEDataRequest CreateSubstanceDataRequest(string substance, string property)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.Substance);
      dr.PropertyName = property;
      dr.SubstanceName = substance;
      return dr;
    }
    public static SEDataRequest CreateSubstanceDataRequest(string substance, string property, Unit unit)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.Substance);
      dr.PropertyName = property;
      dr.SubstanceName = substance;
      dr.Unit = unit;
      return dr;
    }

    public static SEDataRequest CreateECGDataRequest(string property)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.ECG);
      dr.PropertyName = property;
      return dr;
    }
    public static SEDataRequest CreateECGDataRequest(string property, Unit unit)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.ECG);
      dr.PropertyName = property;
      dr.Unit = unit;
      return dr;
    }

    public static SEDataRequest CreateMechanicalVentilatorDataRequest(string property)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.MechanicalVentilator);
      dr.PropertyName = property;
      return dr;
    }
    public static SEDataRequest CreateMechanicalVentilatorDataRequest(string property, Unit unit)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.MechanicalVentilator);
      dr.PropertyName = property;
      dr.Unit = unit;
      return dr;
    }
  }
}
