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
#include "patient/actions/SEPatientAction.h"

class DLL_DECL SEPericardialEffusion : public SEPatientAction
{
public:

  SEPericardialEffusion();
  virtual ~SEPericardialEffusion();

  virtual void Clear(); //clear memory

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  static void Load(const cdm::PericardialEffusionData& src, SEPericardialEffusion& dst);
  static cdm::PericardialEffusionData* Unload(const SEPericardialEffusion& src);
protected:
  static void Serialize(const cdm::PericardialEffusionData& src, SEPericardialEffusion& dst);
  static void Serialize(const SEPericardialEffusion& src, cdm::PericardialEffusionData& dst);

public:

  virtual bool HasEffusionRate() const;
  virtual SEScalarVolumePerTime& GetEffusionRate();
  
  virtual void ToString(std::ostream &str) const;

protected:
  SEScalarVolumePerTime* m_EffusionRate;
};      