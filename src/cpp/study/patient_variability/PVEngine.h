/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include <thread>
#include "PulseEngine.h"

#include "engine/common/Engine.h"
#include "engine/common/controller/Controller.h"
#include "cdm/substance/SESubstanceManager.h"
#include "cdm/patient/SEPatient.h"
#include "cdm/engine/SEOverrides.h"
#include "cdm/engine/SEEngineTracker.h"
#include "cdm/engine/SEDataRequestManager.h"
#include "cdm/engine/SEActionManager.h"
#include "cdm/engine/SEPatientActionCollection.h"
#include "cdm/engine/SEPatientConfiguration.h"
#include "cdm/substance/SESubstanceManager.h"
#include "cdm/utils/DataTrack.h"
#include "cdm/utils/FileUtils.h"
#include "cdm/utils/GeneralMath.h"
#include "cdm/utils/TimingProfile.h"

PUSH_PROTO_WARNINGS
#include "pulse/study/bind/PatientVariability.pb.h"
#include <google/protobuf/text_format.h>
#include <google/protobuf/util/json_util.h>
POP_PROTO_WARNINGS
#include "io/protobuf/PBUtils.h"

namespace pulse::study::patient_variability
{
  class PULSE_DECL PVEngine : public Loggable, public SEEventHandler
  {
    friend class PVRunner;
    friend class PVGenerator;
  public:
    PVEngine(std::string const& logfile = "", bool cout_enabled = true, std::string const& data_dir = "./");
    ~PVEngine();

    bool CreateEngine(const std::string& simulationDataStr /*eSerializationFormat fmt*/);
    bool CreateEngine(pulse::study::bind::patient_variability::SimulationData& sim);

    bool AdvanceTime(double time_s);

    bool ProcessAction(const SEAction& action);
    bool ProcessActions(std::string const& actions, eSerializationFormat format);

    std::string GetSimulationState(/*eSerializationFormat fmt*/);
    bool GetSimulationState(pulse::study::bind::patient_variability::SimulationData& sim);

    void DestroyEngines();

    static void TrackData(SEEngineTracker& trkr, const std::string& csv_filename);

    static bool SerializeToString(pulse::study::bind::patient_variability::SimulationData& src, std::string& dst /*eSerializationFormat fmt*/);
    static bool SerializeFromString(const std::string& src, pulse::study::bind::patient_variability::SimulationData& dst /*eSerializationFormat fmt*/);

  protected:
    void HandleEvent(eEvent e, bool active, const SEScalarTime* simTime = nullptr) override;


    std::string m_DataDir;
    std::vector<pulse::Engine*> m_Engines;
    std::vector<pulse::Controller*> m_Controllers;

    double                    m_TimeStep_s;
    double                    m_CurrentTime_s;
    double                    m_SpareAdvanceTime_s;
    // Substances
    SESubstanceManager* m_SubMgr;

    pulse::study::bind::patient_variability::SimulationData* m_SimulationData;
  };
}
