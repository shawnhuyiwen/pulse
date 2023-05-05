/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using System;

namespace Pulse.CDM
{
  public enum eDataRequest_Category : int
  {
    Patient = 0, Physiology, Environment, Action,
    GasCompartment, LiquidCompartment, ThermalCompartment, TissueCompartment,
    Substance, AnesthesiaMachine, BagValveMask, ECG, ECMO, Inhaler, MechanicalVentilator
  };

  public class SEDataRequest
  {
    public class DecimalFormat
    {
      public eDecimalFormatType type = eDecimalFormatType.SystemFormatting;
      public uint                precision = 1;
    }
    protected eDataRequest_Category Category;
    protected string ActionName = null;
    protected string CompartmentName = null;
    protected string SubstanceName = null;
    protected string PropertyName = null;
    protected Unit Unit = null;
    protected DecimalFormat decimal_format = null;

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
    public bool HasDecimalFormat() { return decimal_format != null; }
    public DecimalFormat GetDecimalFormat()
    {
      if (decimal_format == null)
        decimal_format = new DecimalFormat();
      return decimal_format;
    }
    public void InvalidateDecimalFormat() { decimal_format = null; }

    public new string ToString()
    {
      String str = "";
      switch(Category)
      {
        case eDataRequest_Category.Action:
          str = GetActionName()+"-";
          break;
        case eDataRequest_Category.Patient:
          str = "Patient-";
          break;
        case eDataRequest_Category.AnesthesiaMachine:
          str = "AnesthesiaMachine-";
          break;
        case eDataRequest_Category.BagValveMask:
          str = "BagValveMask-";
          break;
        case eDataRequest_Category.ECG:
          str = "ECG-";
          break;
        case eDataRequest_Category.ECMO:
          str = "ECMO-";
          break;
        case eDataRequest_Category.Inhaler:
          str = "Inhaler-";
          break;
        case eDataRequest_Category.MechanicalVentilator:
          str = "MechanicalVentilator-";
          break;
        case eDataRequest_Category.GasCompartment:
        case eDataRequest_Category.LiquidCompartment:
        case eDataRequest_Category.ThermalCompartment:
        case eDataRequest_Category.TissueCompartment:
          str += GetCompartmentName() + "-";
          if (HasSubstanceName())
            str += " - " + GetSubstanceName();
          break;
        case eDataRequest_Category.Substance:
          str += GetSubstanceName()+"-";
          break;
      }
      str += PropertyName;
      if (Unit != null)
        str += " (" + Unit.ToString() + ")"; ;
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

    public static SEDataRequest CreateAnesthesiaMachineDataRequest(string property)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.AnesthesiaMachine);
      dr.PropertyName = property;
      return dr;
    }
    public static SEDataRequest CreateAnesthesiaMachineDataRequest(string property, Unit unit)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.AnesthesiaMachine);
      dr.PropertyName = property;
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

    public static SEDataRequest CreateECMODataRequest(string property)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.ECMO);
      dr.PropertyName = property;
      return dr;
    }
    public static SEDataRequest CreateECMODataRequest(string property, Unit unit)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.ECMO);
      dr.PropertyName = property;
      dr.Unit = unit;
      return dr;
    }

    public static SEDataRequest CreateInhalerDataRequest(string property)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.Inhaler);
      dr.PropertyName = property;
      return dr;
    }
    public static SEDataRequest CreateInhalerDataRequest(string property, Unit unit)
    {
      SEDataRequest dr = new SEDataRequest(eDataRequest_Category.Inhaler);
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
