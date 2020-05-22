/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "system/SESystem.h"
class SEPupillaryResponse;

class CDM_DECL SENervousSystem : public SESystem
{
  friend class PBPhysiology;//friend the serialization class
public:

  SENervousSystem(Logger* logger);
  virtual ~SENervousSystem();

  virtual void Clear();// Deletes all members
  
  virtual const SEScalar* GetScalar(const std::string& name);

  virtual eSwitch GetBaroreceptorFeedback() const;
  virtual void SetBaroreceptorFeedback(eSwitch s);

  virtual bool HasBaroreceptorHeartRateScale() const;
  virtual SEScalar& GetBaroreceptorHeartRateScale();
  virtual double GetBaroreceptorHeartRateScale() const;

  virtual bool HasBaroreceptorHeartElastanceScale() const;
  virtual SEScalar& GetBaroreceptorHeartElastanceScale();
  virtual double GetBaroreceptorHeartElastanceScale() const;

  virtual bool HasBaroreceptorResistanceScale() const;
  virtual SEScalar& GetBaroreceptorResistanceScale();
  virtual double GetBaroreceptorResistanceScale() const;

  virtual bool HasBaroreceptorComplianceScale() const;
  virtual SEScalar& GetBaroreceptorComplianceScale();
  virtual double GetBaroreceptorComplianceScale() const;

  virtual eSwitch GetChemoreceptorFeedback() const;
  virtual void SetChemoreceptorFeedback(eSwitch s);

  virtual bool HasChemoreceptorHeartRateScale() const;
  virtual SEScalar& GetChemoreceptorHeartRateScale();
  virtual double GetChemoreceptorHeartRateScale() const;

  virtual bool HasChemoreceptorHeartElastanceScale() const;
  virtual SEScalar& GetChemoreceptorHeartElastanceScale();
  virtual double GetChemoreceptorHeartElastanceScale() const;

  virtual bool HasLeftEyePupillaryResponse() const;
  virtual SEPupillaryResponse& GetLeftEyePupillaryResponse();
  virtual const SEPupillaryResponse* GetLeftEyePupillaryResponse() const;
  virtual void RemoveLeftEyePupillaryResponse();

  virtual bool HasRightEyePupillaryResponse() const;
  virtual SEPupillaryResponse& GetRightEyePupillaryResponse();
  virtual const SEPupillaryResponse* GetRightEyePupillaryResponse() const;
  virtual void RemoveRightEyePupillaryResponse();


protected:

  eSwitch                m_BaroreceptorFeedback;
  SEScalar*              m_BaroreceptorHeartRateScale;
  SEScalar*              m_BaroreceptorHeartElastanceScale;
  SEScalar*              m_BaroreceptorResistanceScale;
  SEScalar*              m_BaroreceptorComplianceScale;
  eSwitch                m_ChemoreceptorFeedback;
  SEScalar*              m_ChemoreceptorHeartRateScale;
  SEScalar*              m_ChemoreceptorHeartElastanceScale;
  SEPupillaryResponse*   m_LeftEyePupillaryResponse;
  SEPupillaryResponse*   m_RightEyePupillaryResponse;
};
