/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/SEDataRequest.h"
class SESubstance;
class SESubstanceManager;

class CDM_DECL SEDataRequestManager : public Loggable
{
  friend class PBEngine;//friend the serialization class
public:
  SEDataRequestManager(Logger* logger);
  ~SEDataRequestManager();

  void Clear();
  void Copy(const SEDataRequestManager& src, const SESubstanceManager& subMgr);

  bool SerializeToString(std::string& output, SerializationFormat m) const;
  bool SerializeToFile(const std::string& filename) const;
  bool SerializeFromString(const std::string& src, SerializationFormat m, const SESubstanceManager& subMgr);
  bool SerializeFromFile(const std::string& filename, const SESubstanceManager& subMgr);

  bool HasResultsFilename() const { return !m_ResultsFilename.empty(); }
  std::string GetResultFilename() const { return m_ResultsFilename; }
  void SetResultsFilename(const std::string& name) { m_ResultsFilename = name; }

  double GetSamplesPerSecond() const { return m_SamplesPerSecond; }
  void SetSamplesPerSecond(double num) { m_SamplesPerSecond = num; }

  bool HasDataRequests() const { return !m_Requests.empty(); }
  const std::vector<SEDataRequest*>& GetDataRequests() const { return m_Requests; }

  virtual bool HasDefaultDecimalFormatting() const;
  virtual SEDecimalFormat& GetDefaultDecimalFormatting();
  virtual void RemoveDefaultDecimalFormatting();

  virtual bool HasOverrideDecimalFormatting() const;
  virtual SEDecimalFormat& GetOverrideDecimalFormatting();
  virtual void RemoveOverrideDecimalFormatting();

  SEDataRequest& CopyDataRequest(const SEDataRequest& dr);

  SEDataRequest& CreateDataRequest(eDataRequest_Category category, const SEDecimalFormat* dfault = nullptr);
  SEDataRequest& CreatePatientDataRequest(const std::string& property, const SEDecimalFormat* dfault = nullptr);
  SEDataRequest& CreatePatientDataRequest(const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault = nullptr);

  SEDataRequest& CreatePhysiologyDataRequest(const std::string& property, const SEDecimalFormat* dfault = nullptr);
  SEDataRequest& CreatePhysiologyDataRequest(const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault = nullptr);

  SEDataRequest& CreateEnvironmentDataRequest(const std::string& property, const SEDecimalFormat* dfault = nullptr);
  SEDataRequest& CreateEnvironmentDataRequest(const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault = nullptr);

  SEDataRequest& CreateActionDataRequest(const std::string& actionName, const std::string& property, const SEDecimalFormat* dfault = nullptr);
  SEDataRequest& CreateActionDataRequest(const std::string& actionName, const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault = nullptr);
  SEDataRequest& CreateActionCompartmentDataRequest(const std::string& actionName, const std::string& cmptName, const std::string& property, const SEDecimalFormat* dfault = nullptr);
  SEDataRequest& CreateActionCompartmentDataRequest(const std::string& actionName, const std::string& cmptName, const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault = nullptr);
  SEDataRequest& CreateActionSubstanceDataRequest(const std::string& actionName, const std::string& substance, const std::string& property, const SEDecimalFormat* dfault = nullptr);
  SEDataRequest& CreateActionSubstanceDataRequest(const std::string& actionName, const std::string& substance, const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault = nullptr);

  SEDataRequest& CreateGasCompartmentDataRequest(const std::string& cmptName, const std::string& property, const SEDecimalFormat* dfault = nullptr);
  SEDataRequest& CreateGasCompartmentDataRequest(const std::string& cmptName, const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault = nullptr);
  SEDataRequest& CreateGasCompartmentDataRequest(const std::string& cmptName, const std::string& substance, const std::string& property, const SEDecimalFormat* dfault = nullptr);
  SEDataRequest& CreateGasCompartmentDataRequest(const std::string& cmptName, const std::string& substance, const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault = nullptr);

  SEDataRequest& CreateLiquidCompartmentDataRequest(const std::string& cmptName, const std::string& property, const SEDecimalFormat* dfault = nullptr);
  SEDataRequest& CreateLiquidCompartmentDataRequest(const std::string& cmptName, const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault = nullptr);
  SEDataRequest& CreateLiquidCompartmentDataRequest(const std::string& cmptName, const std::string& substance, const std::string& property, const SEDecimalFormat* dfault = nullptr);
  SEDataRequest& CreateLiquidCompartmentDataRequest(const std::string& cmptName, const std::string& substance, const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault = nullptr);

  SEDataRequest& CreateThermalCompartmentDataRequest(const std::string& cmptName, const std::string& property, const SEDecimalFormat* dfault = nullptr);
  SEDataRequest& CreateThermalCompartmentDataRequest(const std::string& cmptName, const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault = nullptr);

  SEDataRequest& CreateTissueCompartmentDataRequest(const std::string& cmptName, const std::string& property, const SEDecimalFormat* dfault = nullptr);
  SEDataRequest& CreateTissueCompartmentDataRequest(const std::string& cmptName, const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault = nullptr);

  SEDataRequest& CreateSubstanceDataRequest(const std::string& substance, const std::string& property, const SEDecimalFormat* dfault = nullptr);
  SEDataRequest& CreateSubstanceDataRequest(const std::string& substance, const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault = nullptr);

  SEDataRequest& CreateAnesthesiaMachineDataRequest(const std::string& property, const SEDecimalFormat* dfault = nullptr);
  SEDataRequest& CreateAnesthesiaMachineDataRequest(const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault = nullptr);

  SEDataRequest& CreateECGDataRequest(const std::string& property, const SEDecimalFormat* dfault = nullptr);
  SEDataRequest& CreateECGDataRequest(const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault = nullptr);

  SEDataRequest& CreateInhalerDataRequest(const std::string& property, const SEDecimalFormat* dfault = nullptr);
  SEDataRequest& CreateInhalerDataRequest(const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault = nullptr);

  SEDataRequest& CreateMechanicalVentilatorDataRequest(const std::string& property, const SEDecimalFormat* dfault = nullptr);
  SEDataRequest& CreateMechanicalVentilatorDataRequest(const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault = nullptr);

protected:
  // Methods to find data requests so we don't keep making the same one
  SEDataRequest* FindDataRequest(const SEDataRequest& dr);
  SEDataRequest* FindPatientDataRequest(const std::string& property);
  SEDataRequest* FindPhysiologyDataRequest(const std::string& property);
  SEDataRequest* FindEnvironmentDataRequest(const std::string& property);
  SEDataRequest* FindActionDataRequest(const std::string& actionName, const std::string& cmptName, const std::string& substance, const std::string& property);
  SEDataRequest* FindGasCompartmentDataRequest(const std::string& cmptName, const std::string& property);
  SEDataRequest* FindGasCompartmentDataRequest(const std::string& cmptName, const std::string& substance, const std::string& property);
  SEDataRequest* FindLiquidCompartmentDataRequest(const std::string& cmptName, const std::string& property);
  SEDataRequest* FindLiquidCompartmentDataRequest(const std::string& cmptName, const std::string& substance, const std::string& property);
  SEDataRequest* FindThermalCompartmentDataRequest(const std::string& cmptName, const std::string& property);
  SEDataRequest* FindTissueCompartmentDataRequest(const std::string& cmptName, const std::string& property);
  SEDataRequest* FindSubstanceDataRequest(const std::string& substance, const std::string& property);
  SEDataRequest* FindAnesthesiaMachineDataRequest(const std::string& property);
  SEDataRequest* FindECGDataRequest(const std::string& property);
  SEDataRequest* FindInhalerDataRequest(const std::string& property);
  SEDataRequest* FindMechanicalVentilatorDataRequest(const std::string& property);

  std::string                  m_ResultsFilename;
  double                       m_SamplesPerSecond;
  std::vector<SEDataRequest*>  m_Requests;

  SEDecimalFormat*             m_DefaultDecimalFormatting;
  SEDecimalFormat*             m_OverrideDecimalFormatting;
};