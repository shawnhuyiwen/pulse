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
  bool SerializeToFile(const std::string& filename, SerializationFormat m) const;
  bool SerializeFromString(const std::string& src, SerializationFormat m, const SESubstanceManager& subMgr);
  bool SerializeFromFile(const std::string& filename, SerializationFormat m, const SESubstanceManager& subMgr);

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

  SEDataRequest& CreateGasCompartmentDataRequest(const std::string& cmptName, const std::string& property, const SEDecimalFormat* dfault = nullptr);
  SEDataRequest& CreateGasCompartmentDataRequest(const std::string& cmptName, const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault = nullptr);
  SEDataRequest& CreateGasCompartmentDataRequest(const std::string& cmptName, const SESubstance& sub, const std::string& property, const SEDecimalFormat* dfault = nullptr);
  SEDataRequest& CreateGasCompartmentDataRequest(const std::string& cmptName, const SESubstance& sub, const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault = nullptr);

  SEDataRequest& CreateLiquidCompartmentDataRequest(const std::string& cmptName, const std::string& property, const SEDecimalFormat* dfault = nullptr);
  SEDataRequest& CreateLiquidCompartmentDataRequest(const std::string& cmptName, const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault = nullptr);
  SEDataRequest& CreateLiquidCompartmentDataRequest(const std::string& cmptName, const SESubstance& sub, const std::string& property, const SEDecimalFormat* dfault = nullptr);
  SEDataRequest& CreateLiquidCompartmentDataRequest(const std::string& cmptName, const SESubstance& sub, const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault = nullptr);

  SEDataRequest& CreateThermalCompartmentDataRequest(const std::string& property, const SEDecimalFormat* dfault = nullptr);
  SEDataRequest& CreateThermalCompartmentDataRequest(const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault = nullptr);

  SEDataRequest& CreateTissueCompartmentDataRequest(const std::string& property, const SEDecimalFormat* dfault = nullptr);
  SEDataRequest& CreateTissueCompartmentDataRequest(const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault = nullptr);

  SEDataRequest& CreateSubstanceDataRequest(const SESubstance& sub, const std::string& property, const SEDecimalFormat* dfault = nullptr);
  SEDataRequest& CreateSubstanceDataRequest(const SESubstance& sub, const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault = nullptr);

  SEDataRequest& CreateAnesthesiaMachineDataRequest(const std::string& property, const SEDecimalFormat* dfault = nullptr);
  SEDataRequest& CreateAnesthesiaMachineDataRequest(const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault = nullptr);

  SEDataRequest& CreateECGDataRequest(const std::string& property, const SEDecimalFormat* dfault = nullptr);
  SEDataRequest& CreateECGDataRequest(const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault = nullptr);

  SEDataRequest& CreateInhalerDataRequest(const std::string& property, const SEDecimalFormat* dfault = nullptr);
  SEDataRequest& CreateInhalerDataRequest(const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault = nullptr);
  
protected:
  std::string                  m_ResultsFilename;
  double                       m_SamplesPerSecond;
  std::vector<SEDataRequest*>  m_Requests;

  SEDecimalFormat*             m_DefaultDecimalFormatting;
  SEDecimalFormat*             m_OverrideDecimalFormatting;
};