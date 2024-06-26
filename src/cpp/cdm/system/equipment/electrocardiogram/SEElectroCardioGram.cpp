/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/system/equipment/electrocardiogram/SEElectroCardioGram.h"
#include "cdm/properties/SEArrayElectricPotential.h"
#include "cdm/properties/SEScalarElectricPotential.h"
#include "cdm/io/protobuf/PBElectroCardioGram.h"

SEElectroCardioGram::SEElectroCardioGram(Logger* logger) : SEEquipment(logger)
{
  m_Lead1ElectricPotential = nullptr;
  m_Lead2ElectricPotential = nullptr;
  m_Lead3ElectricPotential = nullptr;
  m_Lead4ElectricPotential = nullptr;
  m_Lead5ElectricPotential = nullptr;
  m_Lead6ElectricPotential = nullptr;
  m_Lead7ElectricPotential = nullptr;
  m_Lead8ElectricPotential = nullptr;
  m_Lead9ElectricPotential = nullptr;
  m_Lead10ElectricPotential = nullptr;
  m_Lead11ElectricPotential = nullptr;
  m_Lead12ElectricPotential = nullptr;

  m_ActiveType = eElectroCardioGram_WaveformType::Sinus;
}

SEElectroCardioGram::~SEElectroCardioGram()
{
  SAFE_DELETE(m_Lead1ElectricPotential);
  SAFE_DELETE(m_Lead2ElectricPotential);
  SAFE_DELETE(m_Lead3ElectricPotential);
  SAFE_DELETE(m_Lead4ElectricPotential);
  SAFE_DELETE(m_Lead5ElectricPotential);
  SAFE_DELETE(m_Lead6ElectricPotential);
  SAFE_DELETE(m_Lead7ElectricPotential);
  SAFE_DELETE(m_Lead8ElectricPotential);
  SAFE_DELETE(m_Lead9ElectricPotential);
  SAFE_DELETE(m_Lead10ElectricPotential);
  SAFE_DELETE(m_Lead11ElectricPotential);
  SAFE_DELETE(m_Lead12ElectricPotential);
}

void SEElectroCardioGram::Clear()
{
  SEEquipment::Clear();
  INVALIDATE_PROPERTY(m_Lead1ElectricPotential);
  INVALIDATE_PROPERTY(m_Lead2ElectricPotential);
  INVALIDATE_PROPERTY(m_Lead3ElectricPotential);
  INVALIDATE_PROPERTY(m_Lead4ElectricPotential);
  INVALIDATE_PROPERTY(m_Lead5ElectricPotential);
  INVALIDATE_PROPERTY(m_Lead6ElectricPotential);
  INVALIDATE_PROPERTY(m_Lead7ElectricPotential);
  INVALIDATE_PROPERTY(m_Lead8ElectricPotential);
  INVALIDATE_PROPERTY(m_Lead9ElectricPotential);
  INVALIDATE_PROPERTY(m_Lead10ElectricPotential);
  INVALIDATE_PROPERTY(m_Lead11ElectricPotential);
  INVALIDATE_PROPERTY(m_Lead12ElectricPotential);

  m_ActiveType = eElectroCardioGram_WaveformType::Sinus;

  DELETE_VECTOR(m_Waveforms);
}

void SEElectroCardioGram::Copy(const SEElectroCardioGram& src)
{
  PBElectroCardioGram::Copy(src, *this);
}

bool SEElectroCardioGram::SerializeToString(std::string& output, eSerializationFormat m) const
{
  return PBElectroCardioGram::SerializeToString(*this, output, m);
}
bool SEElectroCardioGram::SerializeToFile(const std::string& filename) const
{
  return PBElectroCardioGram::SerializeToFile(*this, filename);
}
bool SEElectroCardioGram::SerializeFromString(const std::string& src, eSerializationFormat m)
{
  return PBElectroCardioGram::SerializeFromString(src, *this, m);
}
bool SEElectroCardioGram::SerializeFromFile(const std::string& filename)
{
  return PBElectroCardioGram::SerializeFromFile(filename, *this);
}

const SEScalar* SEElectroCardioGram::GetScalar(const std::string& name)
{
  if (name.compare("Lead1ElectricPotential") == 0)
    return &GetLead1ElectricPotential();
  if (name.compare("Lead2ElectricPotential") == 0)
    return &GetLead2ElectricPotential();
  if (name.compare("Lead3ElectricPotential") == 0)
    return &GetLead3ElectricPotential();
  if (name.compare("Lead4ElectricPotential") == 0)
    return &GetLead4ElectricPotential();
  if (name.compare("Lead5ElectricPotential") == 0)
    return &GetLead5ElectricPotential();
  if (name.compare("Lead6ElectricPotential") == 0)
    return &GetLead6ElectricPotential();
  if (name.compare("Lead7ElectricPotential") == 0)
    return &GetLead7ElectricPotential();
  if (name.compare("Lead8ElectricPotential") == 0)
    return &GetLead8ElectricPotential();
  if (name.compare("Lead9ElectricPotential") == 0)
    return &GetLead9ElectricPotential();
  if (name.compare("Lead10ElectricPotential") == 0)
    return &GetLead10ElectricPotential();
  if (name.compare("Lead11ElectricPotential") == 0)
    return &GetLead11ElectricPotential();
  if (name.compare("Lead12ElectricPotential") == 0)
    return &GetLead12ElectricPotential();
  return nullptr;
}

bool SEElectroCardioGram::HasLead1ElectricPotential() const
{
  return m_Lead1ElectricPotential == nullptr ? false : m_Lead1ElectricPotential->IsValid();
}
SEScalarElectricPotential& SEElectroCardioGram::GetLead1ElectricPotential()
{
  if (m_Lead1ElectricPotential == nullptr)
    m_Lead1ElectricPotential = new SEScalarElectricPotential();
  return *m_Lead1ElectricPotential;
}
double SEElectroCardioGram::GetLead1ElectricPotential(const ElectricPotentialUnit& unit) const
{
  if (m_Lead1ElectricPotential == nullptr)
    return SEScalar::dNaN();
  return m_Lead1ElectricPotential->GetValue(unit);
}

bool SEElectroCardioGram::HasLead2ElectricPotential() const
{
  return m_Lead2ElectricPotential == nullptr ? false : m_Lead2ElectricPotential->IsValid();
}
SEScalarElectricPotential& SEElectroCardioGram::GetLead2ElectricPotential()
{
  if (m_Lead2ElectricPotential == nullptr)
    m_Lead2ElectricPotential = new SEScalarElectricPotential();
  return *m_Lead2ElectricPotential;
}
double SEElectroCardioGram::GetLead2ElectricPotential(const ElectricPotentialUnit& unit) const
{
  if (m_Lead2ElectricPotential == nullptr)
    return SEScalar::dNaN();
  return m_Lead2ElectricPotential->GetValue(unit);
}

bool SEElectroCardioGram::HasLead3ElectricPotential() const
{
  return m_Lead3ElectricPotential == nullptr ? false : m_Lead3ElectricPotential->IsValid();
}
SEScalarElectricPotential& SEElectroCardioGram::GetLead3ElectricPotential()
{
  if (m_Lead3ElectricPotential == nullptr)
    m_Lead3ElectricPotential = new SEScalarElectricPotential();
  return *m_Lead3ElectricPotential;
}
double SEElectroCardioGram::GetLead3ElectricPotential(const ElectricPotentialUnit& unit) const
{
  if (m_Lead3ElectricPotential == nullptr)
    return SEScalar::dNaN();
  return m_Lead3ElectricPotential->GetValue(unit);
}

bool SEElectroCardioGram::HasLead4ElectricPotential() const
{
  return m_Lead4ElectricPotential == nullptr ? false : m_Lead4ElectricPotential->IsValid();
}
SEScalarElectricPotential& SEElectroCardioGram::GetLead4ElectricPotential()
{
  if (m_Lead4ElectricPotential == nullptr)
    m_Lead4ElectricPotential = new SEScalarElectricPotential();
  return *m_Lead4ElectricPotential;
}
double SEElectroCardioGram::GetLead4ElectricPotential(const ElectricPotentialUnit& unit) const
{
  if (m_Lead4ElectricPotential == nullptr)
    return SEScalar::dNaN();
  return m_Lead4ElectricPotential->GetValue(unit);
}

bool SEElectroCardioGram::HasLead5ElectricPotential() const
{
  return m_Lead5ElectricPotential == nullptr ? false : m_Lead5ElectricPotential->IsValid();
}
SEScalarElectricPotential& SEElectroCardioGram::GetLead5ElectricPotential()
{
  if (m_Lead5ElectricPotential == nullptr)
    m_Lead5ElectricPotential = new SEScalarElectricPotential();
  return *m_Lead5ElectricPotential;
}
double SEElectroCardioGram::GetLead5ElectricPotential(const ElectricPotentialUnit& unit) const
{
  if (m_Lead5ElectricPotential == nullptr)
    return SEScalar::dNaN();
  return m_Lead5ElectricPotential->GetValue(unit);
}

bool SEElectroCardioGram::HasLead6ElectricPotential() const
{
  return m_Lead6ElectricPotential == nullptr ? false : m_Lead6ElectricPotential->IsValid();
}
SEScalarElectricPotential& SEElectroCardioGram::GetLead6ElectricPotential()
{
  if (m_Lead6ElectricPotential == nullptr)
    m_Lead6ElectricPotential = new SEScalarElectricPotential();
  return *m_Lead6ElectricPotential;
}
double SEElectroCardioGram::GetLead6ElectricPotential(const ElectricPotentialUnit& unit) const
{
  if (m_Lead6ElectricPotential == nullptr)
    return SEScalar::dNaN();
  return m_Lead6ElectricPotential->GetValue(unit);
}

bool SEElectroCardioGram::HasLead7ElectricPotential() const
{
  return m_Lead7ElectricPotential == nullptr ? false : m_Lead7ElectricPotential->IsValid();
}
SEScalarElectricPotential& SEElectroCardioGram::GetLead7ElectricPotential()
{
  if (m_Lead7ElectricPotential == nullptr)
    m_Lead7ElectricPotential = new SEScalarElectricPotential();
  return *m_Lead7ElectricPotential;
}
double SEElectroCardioGram::GetLead7ElectricPotential(const ElectricPotentialUnit& unit) const
{
  if (m_Lead7ElectricPotential == nullptr)
    return SEScalar::dNaN();
  return m_Lead7ElectricPotential->GetValue(unit);
}

bool SEElectroCardioGram::HasLead8ElectricPotential() const
{
  return m_Lead8ElectricPotential == nullptr ? false : m_Lead8ElectricPotential->IsValid();
}
SEScalarElectricPotential& SEElectroCardioGram::GetLead8ElectricPotential()
{
  if (m_Lead8ElectricPotential == nullptr)
    m_Lead8ElectricPotential = new SEScalarElectricPotential();
  return *m_Lead8ElectricPotential;
}
double SEElectroCardioGram::GetLead8ElectricPotential(const ElectricPotentialUnit& unit) const
{
  if (m_Lead8ElectricPotential == nullptr)
    return SEScalar::dNaN();
  return m_Lead8ElectricPotential->GetValue(unit);
}

bool SEElectroCardioGram::HasLead9ElectricPotential() const
{
  return m_Lead9ElectricPotential == nullptr ? false : m_Lead9ElectricPotential->IsValid();
}
SEScalarElectricPotential& SEElectroCardioGram::GetLead9ElectricPotential()
{
  if (m_Lead9ElectricPotential == nullptr)
    m_Lead9ElectricPotential = new SEScalarElectricPotential();
  return *m_Lead9ElectricPotential;
}
double SEElectroCardioGram::GetLead9ElectricPotential(const ElectricPotentialUnit& unit) const
{
  if (m_Lead9ElectricPotential == nullptr)
    return SEScalar::dNaN();
  return m_Lead9ElectricPotential->GetValue(unit);
}

bool SEElectroCardioGram::HasLead10ElectricPotential() const
{
  return m_Lead10ElectricPotential == nullptr ? false : m_Lead10ElectricPotential->IsValid();
}
SEScalarElectricPotential& SEElectroCardioGram::GetLead10ElectricPotential()
{
  if (m_Lead10ElectricPotential == nullptr)
    m_Lead10ElectricPotential = new SEScalarElectricPotential();
  return *m_Lead10ElectricPotential;
}
double SEElectroCardioGram::GetLead10ElectricPotential(const ElectricPotentialUnit& unit) const
{
  if (m_Lead10ElectricPotential == nullptr)
    return SEScalar::dNaN();
  return m_Lead10ElectricPotential->GetValue(unit);
}

bool SEElectroCardioGram::HasLead11ElectricPotential() const
{
  return m_Lead11ElectricPotential == nullptr ? false : m_Lead11ElectricPotential->IsValid();
}
SEScalarElectricPotential& SEElectroCardioGram::GetLead11ElectricPotential()
{
  if (m_Lead11ElectricPotential == nullptr)
    m_Lead11ElectricPotential = new SEScalarElectricPotential();
  return *m_Lead11ElectricPotential;
}
double SEElectroCardioGram::GetLead11ElectricPotential(const ElectricPotentialUnit& unit) const
{
  if (m_Lead11ElectricPotential == nullptr)
    return SEScalar::dNaN();
  return m_Lead11ElectricPotential->GetValue(unit);
}

bool SEElectroCardioGram::HasLead12ElectricPotential() const
{
  return m_Lead12ElectricPotential == nullptr ? false : m_Lead12ElectricPotential->IsValid();
}
SEScalarElectricPotential& SEElectroCardioGram::GetLead12ElectricPotential()
{
  if (m_Lead12ElectricPotential == nullptr)
    m_Lead12ElectricPotential = new SEScalarElectricPotential();
  return *m_Lead12ElectricPotential;
}
double SEElectroCardioGram::GetLead12ElectricPotential(const ElectricPotentialUnit& unit) const
{
  if (m_Lead12ElectricPotential == nullptr)
    return SEScalar::dNaN();
  return m_Lead12ElectricPotential->GetValue(unit);
}

bool SEElectroCardioGram::HasWaveforms()
{
  return !m_Waveforms.empty();
}
std::vector<SEElectroCardioGramWaveform*>& SEElectroCardioGram::GetWaveforms()
{
  return m_Waveforms;
}

bool SEElectroCardioGram::HasWaveform(eElectroCardioGram_WaveformLead lead, eElectroCardioGram_WaveformType type)
{
  for (SEElectroCardioGramWaveform* w : m_Waveforms)
  {
    if (w->GetLeadNumber() == lead && w->GetType() == type)
      return true;
  }
  return false;
}
SEElectroCardioGramWaveform& SEElectroCardioGram::GetWaveform(eElectroCardioGram_WaveformLead lead, eElectroCardioGram_WaveformType type)
{
  for (SEElectroCardioGramWaveform* w : m_Waveforms)
  {
    if (w->GetLeadNumber() == lead && w->GetType() == type)
      return *w;
  }
  SEElectroCardioGramWaveform* w = new SEElectroCardioGramWaveform(GetLogger());
  w->SetLeadNumber(lead);
  w->SetType(type);
  m_Waveforms.push_back(w);
  return *w;
}
const SEElectroCardioGramWaveform* SEElectroCardioGram::GetWaveform(eElectroCardioGram_WaveformLead lead, eElectroCardioGram_WaveformType type) const
{
  for (SEElectroCardioGramWaveform* w : m_Waveforms)
  {
    if (w->GetLeadNumber() == lead && w->GetType() == type)
      return w;
  }
  return nullptr;
}

void SEElectroCardioGram::ClearCycles()
{
  for (SEElectroCardioGramWaveform* w : m_Waveforms)
    w->GetActiveCycle().Clear();
}

void SEElectroCardioGram::PullCycleValues()
{
  for (SEElectroCardioGramWaveform* w : m_Waveforms)
  {
    if (w->GetType() == m_ActiveType)
    {
      switch (w->GetLeadNumber())
      {
      case eElectroCardioGram_WaveformLead::Lead1:
      {
        w->GetCycleValue(GetLead1ElectricPotential(), true);
        break;
      }
      case eElectroCardioGram_WaveformLead::Lead2:
      {
        w->GetCycleValue(GetLead2ElectricPotential(), true);
        break;
      }
      case eElectroCardioGram_WaveformLead::Lead3:
      {
        w->GetCycleValue(GetLead3ElectricPotential(), true);
        break;
      }
      case eElectroCardioGram_WaveformLead::Lead4:
      {
        w->GetCycleValue(GetLead4ElectricPotential(), true);
        break;
      }
      case eElectroCardioGram_WaveformLead::Lead5:
      {
        w->GetCycleValue(GetLead5ElectricPotential(), true);
        break;
      }
      case eElectroCardioGram_WaveformLead::Lead6:
      {
        w->GetCycleValue(GetLead6ElectricPotential(), true);
        break;
      }
      case eElectroCardioGram_WaveformLead::Lead7:
      {
        w->GetCycleValue(GetLead7ElectricPotential(), true);
        break;
      }
      case eElectroCardioGram_WaveformLead::Lead8:
      {
        w->GetCycleValue(GetLead8ElectricPotential(), true);
        break;
      }
      case eElectroCardioGram_WaveformLead::Lead9:
      {
        w->GetCycleValue(GetLead9ElectricPotential(), true);
        break;
      }
      case eElectroCardioGram_WaveformLead::Lead10:
      {
        w->GetCycleValue(GetLead10ElectricPotential(), true);
        break;
      }
      case eElectroCardioGram_WaveformLead::Lead11:
      {
        w->GetCycleValue(GetLead11ElectricPotential(), true);
        break;
      }
      case eElectroCardioGram_WaveformLead::Lead12:
      {
        w->GetCycleValue(GetLead12ElectricPotential(), true);
        break;
      }
      case eElectroCardioGram_WaveformLead::NullLead:
      {
        Warning("Cannot get cycle value of null lead");
        break;
      }
      }
    }
  }
}

void SEElectroCardioGram::StartNewCycle(eElectroCardioGram_WaveformType t, const SEScalarFrequency& hr, const SEScalarTime& dt, double amplitudeModifier)
{
  m_ActiveType = t;
  for (SEElectroCardioGramWaveform* w : m_Waveforms)
  {
    if (w->GetType() == t)
    {
      w->GenerateActiveCycle(hr, dt, amplitudeModifier);
    }
    else
      w->GetActiveCycle().Clear();
  }
}
