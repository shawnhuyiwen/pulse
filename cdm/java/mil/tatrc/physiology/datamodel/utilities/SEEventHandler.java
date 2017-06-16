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
package mil.tatrc.physiology.datamodel.utilities;

import com.kitware.physiology.cdm.AnesthesiaMachine.AnesthesiaMachineData;
import com.kitware.physiology.cdm.Patient.PatientData;

import mil.tatrc.physiology.datamodel.properties.SEScalarTime;

public interface SEEventHandler
{
  public void handlePatientEvent(PatientData.eEvent type, boolean active, SEScalarTime time);
  public void handleAnesthesiaMachineEvent(AnesthesiaMachineData.eEvent type, boolean active, SEScalarTime time);
}
