/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/

#pragma once
#include "patient/actions/SEConsciousRespirationCommand.h"
class SESubstance;

class DLL_DECL SEUseInhaler : public SEConsciousRespirationCommand
{
  friend class SEConsciousRespiration;
  SEUseInhaler();
public:
  virtual ~SEUseInhaler();

  virtual void Clear(); //clear memory

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  static void Load(const cdm::ConsciousRespirationData_UseInhalerData& src, SEUseInhaler& dst);
  static cdm::ConsciousRespirationData_UseInhalerData* Unload(const SEUseInhaler& src);
protected:
  static void Serialize(const cdm::ConsciousRespirationData_UseInhalerData& src, SEUseInhaler& dst);
  static void Serialize(const SEUseInhaler& src, cdm::ConsciousRespirationData_UseInhalerData& dst);

public:

  virtual void ToString(std::ostream &str) const;
protected:

};  
