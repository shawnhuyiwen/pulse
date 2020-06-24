/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "system/SESystem.h"
class SEActiveConditioning;
class SEAppliedTemperature;
class SEEnvironmentalConditions;
class SEChangeEnvironmentalConditions;
class SEInitialEnvironmentalConditions;

class CDM_DECL SEEnvironment : public SESystem
{
  friend class PBEnvironment;//friend the serialization class
public:

  SEEnvironment(Logger* logger);
  virtual ~SEEnvironment();

  virtual void Clear();

protected:
  /** @name ProcessChange
  * @brief - Will change this class as directed by the Action
  */
  virtual bool ProcessChange(SEChangeEnvironmentalConditions& action, SESubstanceManager& subMgr);

  /** @name ProcessChange
  * @brief - Will change this class as directed by the Condition
  */
  virtual bool ProcessChange(SEInitialEnvironmentalConditions& change, SESubstanceManager& subMgr);

  /** @name StateChange
  *   @brief - This method is called when ever there is a state change
  *            Specically a new file has been loaded, configuration action, or the system reset
  *            Engine specific methodology can then update their logic.
  */
  virtual void StateChange() {};

public:

  virtual const SEScalar* GetScalar(const std::string& name);

  virtual bool HasActiveHeating() const;
  virtual SEActiveConditioning& GetActiveHeating();
  virtual const SEActiveConditioning* GetActiveHeating() const;
  virtual void RemoveActiveHeating();

  virtual bool HasActiveCooling() const;
  virtual SEActiveConditioning& GetActiveCooling();
  virtual const SEActiveConditioning* GetActiveCooling() const;
  virtual void RemoveActiveCooling();

  virtual bool HasAppliedTemperature() const;
  virtual SEAppliedTemperature& GetAppliedTemperature();
  virtual const SEAppliedTemperature* GetAppliedTemperature() const;
  virtual void RemoveAppliedTemperature();

  virtual bool HasEnvironmentalConditions() const;
  virtual SEEnvironmentalConditions& GetEnvironmentalConditions();
  virtual const SEEnvironmentalConditions* GetEnvironmentalConditions() const;
  virtual void RemoveEnvironmentalConditions();

  virtual bool HasConvectiveHeatLoss() const;
  virtual SEScalarPower& GetConvectiveHeatLoss();
  virtual double GetConvectiveHeatLoss(const PowerUnit& unit) const;

  virtual bool HasConvectiveHeatTranferCoefficient() const;
  virtual SEScalarHeatConductancePerArea& GetConvectiveHeatTranferCoefficient();
  virtual double GetConvectiveHeatTranferCoefficient(const HeatConductancePerAreaUnit& unit) const;

  virtual bool HasEvaporativeHeatLoss() const;
  virtual SEScalarPower& GetEvaporativeHeatLoss();
  virtual double GetEvaporativeHeatLoss(const PowerUnit& unit) const;

  virtual bool HasEvaporativeHeatTranferCoefficient() const;
  virtual SEScalarHeatConductancePerArea& GetEvaporativeHeatTranferCoefficient();
  virtual double GetEvaporativeHeatTranferCoefficient(const HeatConductancePerAreaUnit& unit) const;

  virtual bool HasRadiativeHeatLoss() const;
  virtual SEScalarPower& GetRadiativeHeatLoss();
  virtual double GetRadiativeHeatLoss(const PowerUnit& unit) const;

  virtual bool HasRadiativeHeatTranferCoefficient() const;
  virtual SEScalarHeatConductancePerArea& GetRadiativeHeatTranferCoefficient();
  virtual double GetRadiativeHeatTranferCoefficient(const HeatConductancePerAreaUnit& unit) const;

  virtual bool HasRespirationHeatLoss() const;
  virtual SEScalarPower& GetRespirationHeatLoss();
  virtual double GetRespirationHeatLoss(const PowerUnit& unit) const;

  virtual bool HasSkinHeatLoss() const;
  virtual SEScalarPower& GetSkinHeatLoss();
  virtual double GetSkinHeatLoss(const PowerUnit& unit) const;

protected:

  SEScalarPower*                    m_ConvectiveHeatLoss;
  SEScalarHeatConductancePerArea*   m_ConvectiveHeatTranferCoefficient;
  SEScalarPower*                    m_EvaporativeHeatLoss;
  SEScalarHeatConductancePerArea*   m_EvaporativeHeatTranferCoefficient;
  SEScalarPower*                    m_RadiativeHeatLoss;
  SEScalarHeatConductancePerArea*   m_RadiativeHeatTranferCoefficient;
  SEScalarPower*                    m_RespirationHeatLoss;
  SEScalarPower*                    m_SkinHeatLoss;

  SEActiveConditioning*             m_ActiveHeating;
  SEActiveConditioning*             m_ActiveCooling;
  SEAppliedTemperature*             m_AppliedTemperature;
  SEEnvironmentalConditions*        m_EnvironmentalConditions;
};
