/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "engine/SEInhalerActionCollection.h"
#include "substance/SESubstanceManager.h"
#include "system/equipment/inhaler/SEInhaler.h"
#include "system/equipment/inhaler/actions/SEInhalerConfiguration.h"

SEInhalerActionCollection::SEInhalerActionCollection(SESubstanceManager& substances) : Loggable(substances.GetLogger()), m_Substances(substances)
{
   m_Configuration = nullptr;
}

SEInhalerActionCollection::~SEInhalerActionCollection()
{
  Clear();
}

void SEInhalerActionCollection::Clear()
{
  // State
  RemoveConfiguration();
}

bool SEInhalerActionCollection::ProcessAction(const SEInhalerAction& action)
{
  const SEInhalerConfiguration* config = dynamic_cast<const SEInhalerConfiguration*>(&action);
  if (config != nullptr)
  {
    if (m_Configuration == nullptr)
      m_Configuration = new SEInhalerConfiguration(m_Substances);
    m_Configuration->Copy(*config);
    if (!m_Configuration->IsActive())
      RemoveConfiguration();
    return true;
  }

  /// \error Unsupported Action
  Error("Unsupported Inhaler Action");
  return false;
}

bool SEInhalerActionCollection::HasConfiguration() const
{
  return m_Configuration == nullptr ? false : true;
}
SEInhalerConfiguration* SEInhalerActionCollection::GetConfiguration()
{
  return m_Configuration;
}
const SEInhalerConfiguration* SEInhalerActionCollection::GetConfiguration() const
{
  return m_Configuration;
}
void SEInhalerActionCollection::RemoveConfiguration()
{
  SAFE_DELETE(m_Configuration);
}

void SEInhalerActionCollection::GetAllActions(std::vector<const SEAction*>& actions) const
{
  if (HasConfiguration())
    actions.push_back(GetConfiguration());
}