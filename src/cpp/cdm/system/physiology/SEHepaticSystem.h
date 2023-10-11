/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/system/SESystem.h"

class CDM_DECL SEHepaticSystem : public SESystem
{
  friend class PBPhysiology;//friend the serialization class
public:

  SEHepaticSystem(Logger* logger);
  virtual ~SEHepaticSystem();

  void Clear() override;// Deletes all members
  
  const SEScalar* GetScalar(const std::string& name) override;
  
protected:

  
};