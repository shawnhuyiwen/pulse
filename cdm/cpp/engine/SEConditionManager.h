/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SECondition;
//Patient Conditions
class SEAcuteRespiratoryDistressSyndrome;
class SEChronicAnemia;
class SEChronicObstructivePulmonaryDisease;
class SEChronicHeartFailure;
class SEChronicPericardialEffusion;
class SEChronicRenalStenosis;
class SEChronicVentricularSystolicDysfunction;
class SEConsumeMeal;
class SEImpairedAlveolarExchange;
class SELobarPneumonia;
class SEPulmonaryFibrosis;
class SESepsis;
// Environment Conditions
class SEInitialEnvironmentalConditions;
class SESubstanceManager;

class CDM_DECL SEConditionManager : public Loggable
{
  friend class PBEngine;//friend the serialization class
public:

  SEConditionManager(SESubstanceManager&);
  ~SEConditionManager();

  void Clear();
  void Copy(const SEConditionManager& src);
  
  bool SerializeToString(std::string& output, SerializationFormat m) const;
  bool SerializeToFile(const std::string& filename, SerializationFormat m) const;
  bool SerializeFromString(const std::string& src, SerializationFormat m);
  bool SerializeFromFile(const std::string& filename, SerializationFormat m);

  bool ProcessCondition(const SECondition& condition);// Will make a copy

  // Not too many conditions, so just have one manager
  // If we start getting alot, I will make patient/environment/equipment condition managers, like the action managers

  bool IsEmpty() const;

  // Patient Conditions

  bool HasAcuteRespiratoryDistressSyndrome() const;
  SEAcuteRespiratoryDistressSyndrome* GetAcuteRespiratoryDistressSyndrome();
  const SEAcuteRespiratoryDistressSyndrome* GetAcuteRespiratoryDistressSyndrome() const;

  bool HasChronicAnemia() const;
  SEChronicAnemia* GetChronicAnemia();
  const SEChronicAnemia* GetChronicAnemia() const;

  bool HasChronicObstructivePulmonaryDisease() const;
  SEChronicObstructivePulmonaryDisease* GetChronicObstructivePulmonaryDisease();
  const SEChronicObstructivePulmonaryDisease* GetChronicObstructivePulmonaryDisease() const;

  bool HasChronicHeartFailure() const;
  bool HasChronicVentricularSystolicDysfunction() const;
  SEChronicVentricularSystolicDysfunction* GetChronicVentricularSystolicDysfunction();
  const SEChronicVentricularSystolicDysfunction* GetChronicVentricularSystolicDysfunction() const;

  bool HasChronicPericardialEffusion() const;
  SEChronicPericardialEffusion* GetChronicPericardialEffusion();
  const SEChronicPericardialEffusion* GetChronicPericardialEffusion() const;

  bool HasChronicRenalStenosis() const;
  SEChronicRenalStenosis* GetChronicRenalStenosis();
  const SEChronicRenalStenosis* GetChronicRenalStenosis() const;

  bool HasConsumeMeal() const;
  SEConsumeMeal* GetConsumeMeal();
  const SEConsumeMeal* GetConsumeMeal() const;

  bool HasImpairedAlveolarExchange() const;
  SEImpairedAlveolarExchange* GetImpairedAlveolarExchange();
  const SEImpairedAlveolarExchange* GetImpairedAlveolarExchange() const;

  bool HasLobarPneumonia() const;
  SELobarPneumonia* GetLobarPneumonia();
  const SELobarPneumonia* GetLobarPneumonia() const;

  bool HasPulmonaryFibrosis() const;
  SEPulmonaryFibrosis* GetPulmonaryFibrosis();
  const SEPulmonaryFibrosis* GetPulmonaryFibrosis() const;

  bool HasSepsis() const;
  SESepsis* GetSepsis();
  const SESepsis* GetSepsis() const;

  // Environment Conditions

  bool HasInitialEnvironmentalConditions() const;
  SEInitialEnvironmentalConditions* GetInitialEnvironmentalConditions();
  const SEInitialEnvironmentalConditions* GetInitialEnvironmentalConditions() const;

  // Helper to get a list of the active conditions
  void GetAllConditions(std::vector<const SECondition*>& v) const;

protected:

  SESubstanceManager&                      m_Substances;
  SEAcuteRespiratoryDistressSyndrome*      m_ARDS;
  SEChronicAnemia*                         m_Anemia;
  SEConsumeMeal*                           m_ConsumeMeal;
  SEChronicObstructivePulmonaryDisease*    m_COPD;
  SEChronicVentricularSystolicDysfunction* m_ChronicVentricularSystolicDysfunction;
  SEImpairedAlveolarExchange*              m_ImpairedAlveolarExchange;
  SEChronicPericardialEffusion*            m_PericardialEffusion;
  SELobarPneumonia*                        m_LobarPneumonia;
  SEChronicRenalStenosis*                  m_RenalStenosis;
  SEPulmonaryFibrosis*                     m_PulmonaryFibrosis;
  SESepsis*                                m_Sepsis;

  SEInitialEnvironmentalConditions*        m_InitialEnvironmentalConditions;

  std::stringstream m_ss;
};