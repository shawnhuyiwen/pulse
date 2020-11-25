/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SEPatientConfiguration
  {
    protected SEPatient patient = null;
    protected string patient_file = "";
    protected SEConditionManager condition_manager=null;
    protected string data_root_dir = "./";

    public SEPatientConfiguration()
    {
      Clear();
    }

    public void Clear()
    {
      patient_file = null;
      if(patient!=null)
        patient.Clear();
      condition_manager = null;
      data_root_dir = "./";
    }

    public bool IsValid()
    {
      if (!HasPatient() && !HasPatientFile())
        return false;
      return true;
    }

    public string GetDataRootDir()
    {
      return data_root_dir;
    }
    public void SetDataRootDir(string dir)
    {
      data_root_dir = dir;
    }

    public bool HasPatientFile()
    {
      return patient_file != null;
    }
    public string GetPatientFile()
    {
      return patient_file;
    }
    public void SetPatientFile(string f)
    {
      patient_file = f;
    }

    public bool HasPatient()
    {
      return patient != null;
    }
    public SEPatient GetPatient()
    {
      if (patient == null)
        patient = new SEPatient();
      return patient;
    }
    public void SetPatient(SEPatient p)
    {
      patient = p;
    }

    public bool HasConditions()
    {
      return condition_manager == null ? false : !condition_manager.IsEmpty();
    }
    public SEConditionManager GetConditions()
    {
      if (condition_manager == null)
        condition_manager = new SEConditionManager();
      return condition_manager;
    }
    public void InvalidateConditions()
    {
      condition_manager = null;
    }
  }
}
