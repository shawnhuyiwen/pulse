/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "PVEngine.h"

#include "engine/common/system/Model.h"
#include "cdm/system/environment/SEEnvironment.h"
#include "cdm/system/equipment/anesthesia_machine/SEAnesthesiaMachine.h"
#include "cdm/system/equipment/bag_valve_mask/SEBagValveMask.h"
#include "cdm/system/equipment/electrocardiogram/SEElectroCardioGram.h"
#include "cdm/system/equipment/inhaler/SEInhaler.h"
#include "cdm/system/equipment/mechanical_ventilator/SEMechanicalVentilator.h"
#include "cdm/system/physiology/SEBloodChemistrySystem.h"
#include "cdm/system/physiology/SECardiovascularSystem.h"
#include "cdm/system/physiology/SEDrugSystem.h"
#include "cdm/system/physiology/SEEndocrineSystem.h"
#include "cdm/system/physiology/SEEnergySystem.h"
#include "cdm/system/physiology/SEGastrointestinalSystem.h"
#include "cdm/system/physiology/SEHepaticSystem.h"
#include "cdm/system/physiology/SENervousSystem.h"
#include "cdm/system/physiology/SERenalSystem.h"
#include "cdm/system/physiology/SERespiratorySystem.h"
#include "cdm/system/physiology/SETissueSystem.h"

using namespace pulse;

namespace pulse::study::patient_variability
{
  PVEngine::PVEngine(std::string const& logfile, bool cout_enabled, std::string const& data_dir) : Loggable(new Logger(logfile))
  {
    m_TimeStep_s = 0.02;
    m_CurrentTime_s = 0;
    m_SpareAdvanceTime_s = 0;

    m_SubMgr = nullptr;

    m_DataDir = data_dir;
    m_SimulationData = nullptr;

    myLogger = true;
    GetLogger()->LogToConsole(cout_enabled);
    Info("Logging to console : " + std::string(cout_enabled ? "True" : "False"));
  }
  PVEngine::~PVEngine()
  {
    DestroyEngines();
    SAFE_DELETE(m_SimulationData);
  }

  void PVEngine::DestroyEngines()
  {
    m_CurrentTime_s = 0;
    SAFE_DELETE(m_SubMgr);

    DELETE_VECTOR(m_Engines);
    m_Controllers.clear();
  }

  void PVEngine::HandleEvent(eEvent /*e*/, bool /*active*/, const SEScalarTime* /*simTime*/)
  {

  }

  bool PVEngine::CreateEngine(const std::string& simulationDataStr)
  {
    SAFE_DELETE(m_SimulationData);
    m_SimulationData = new pulse::study::bind::patient_variability::SimulationData();
    SerializeFromString(simulationDataStr, *m_SimulationData);
    return CreateEngine(*m_SimulationData);
  }

  bool PVEngine::CreateEngine(pulse::study::bind::patient_variability::SimulationData& sim)
  {
    try
    {
      if (!m_Controllers.empty())
      {
        Error("The controller currently has engine allocated, please reset the controller to start new set of engines");
        return false;
      }
      // Figure out where to put results
      std::string outDir = sim.outputbasefilename();
      if (outDir.empty())
        outDir = "";

      m_SubMgr = new SESubstanceManager(GetLogger());
      m_SubMgr->LoadSubstanceDirectory(m_DataDir);

      Info("Creating " + std::to_string(sim.patientcomparisons_size()) + " patients");
      for (int p = 0; p < sim.patientcomparisons_size(); p++)
      {
        Engine* pe = (Engine*)CreatePulseEngine().release();
        Controller* pc = &pe->GetController();
        m_Controllers.push_back(pc);
        m_Engines.push_back(pe);
      }

      //Simulation actions here

      //Info("Processing Action");
      //ProcessAction(*m_MVC);
      m_CurrentTime_s = 0;
      m_SpareAdvanceTime_s = 0;
    }
    catch (CommonDataModelException& cdm_ex)
    {
      GetLogger()->Fatal("Exception caught runnning simulation " + sim.outputbasefilename());
      GetLogger()->Fatal(cdm_ex.what());
      std::cerr << cdm_ex.what() << std::endl;
      return false;
    }
    catch (std::exception& ex)
    {
      GetLogger()->Fatal("Exception caught runnning simulation " + sim.outputbasefilename());
      GetLogger()->Fatal(ex.what());
      std::cerr << ex.what() << std::endl;
      return false;
    }
    catch (...)
    {
      std::cerr << "Unable to run simulation " << sim.outputbasefilename() << std::endl;
      return false;
    }
    Info("Engine is ready");
    return true;
  }

  bool PVEngine::AdvanceTime(double time_s)
  {
    if (m_Controllers.empty())
    {
      Error("No engines have been allocated yet...");
      return false;
    }

    time_s += m_SpareAdvanceTime_s;
    int count = (int)(time_s / m_TimeStep_s);
    m_SpareAdvanceTime_s = time_s - (count * m_TimeStep_s);

    for (int i = 0; i < count; i++)
    {
      for (Controller* pc : m_Controllers)
      {
        if (pc->GetEvents().IsEventActive(eEvent::IrreversibleState))
          return false;
      }
      // PreProcess
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetEnvironment()).PreProcess();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetCardiovascular()).PreProcess();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetInhaler()).PreProcess();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetRespiratory()).PreProcess();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetAnesthesiaMachine()).PreProcess();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetMechanicalVentilator()).PreProcess();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetGastrointestinal()).PreProcess();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetHepatic()).PreProcess();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetRenal()).PreProcess();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetNervous()).PreProcess();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetEnergy()).PreProcess();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetEndocrine()).PreProcess();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetDrugs()).PreProcess();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetTissue()).PreProcess();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetBloodChemistry()).PreProcess();

      // Process
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetEnvironment()).Process();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetCardiovascular()).Process();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetInhaler()).Process();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetRespiratory()).Process();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetAnesthesiaMachine()).Process();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetMechanicalVentilator()).Process();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetGastrointestinal()).Process();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetHepatic()).Process();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetRenal()).Process();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetNervous()).Process();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetEnergy()).Process();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetEndocrine()).Process();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetDrugs()).Process();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetTissue()).Process();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetBloodChemistry()).Process();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetECG()).Process();

      // PostProcess
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetEnvironment()).PostProcess();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetCardiovascular()).PostProcess();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetInhaler()).PostProcess();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetRespiratory()).PostProcess();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetAnesthesiaMachine()).PostProcess();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetMechanicalVentilator()).PostProcess();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetGastrointestinal()).PostProcess();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetHepatic()).PostProcess();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetRenal()).PostProcess();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetNervous()).PostProcess();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetEnergy()).PostProcess();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetEndocrine()).PostProcess();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetDrugs()).PostProcess();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetTissue()).PostProcess();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetBloodChemistry()).PostProcess();
      for (Controller* pc : m_Controllers)
        dynamic_cast<Model&>(pc->GetECG()).PostProcess();

      // Increment Times and track data
      for (Controller* pc : m_Controllers)
      {
        pc->GetEvents().UpdateEvents(pc->GetTimeStep());
        const_cast<SEScalarTime&>(pc->GetEngineTime()).Increment(pc->GetTimeStep());
        const_cast<SEScalarTime&>(pc->GetSimulationTime()).Increment(pc->GetTimeStep());
        pc->GetEngineTracker().TrackData(m_CurrentTime_s);
      }
      m_CurrentTime_s += m_TimeStep_s;
    }
    return true;
  }

  bool PVEngine::ProcessAction(const SEAction& a)
  {
    if (m_Controllers.empty())
    {
      Error("No engines have been allocated yet...");
      return false;
    }
    bool b = true;
    for (Controller* pc : m_Controllers)
      if (!pc->ProcessAction(a))
        b = false;
    return b;
  }
  bool PVEngine::ProcessActions(std::string const& actions, eSerializationFormat format)
  {
    if (m_Controllers.empty())
    {
      Error("No engines have been allocated yet...");
      return false;
    }

    bool success = true;
    if (actions.empty())
      return success;

    try
    {
      std::vector<SEAction*> vActions;
      if (!SEActionManager::SerializeFromString(actions, vActions, format, *m_SubMgr))
        return false;

      for (const SEAction* a : vActions)
      {
        if (!ProcessAction(*a))
          success = false;
        delete a;
      }
    }
    catch (CommonDataModelException& ex)
    {
      success = false;
      GetLogger()->Error(ex.what());
    }
    catch (std::exception& ex)
    {
      success = false;
      GetLogger()->Error(ex.what());
    }
    catch (...)
    {
      success = false;
    }
    return success;
  }

  std::string PVEngine::GetSimulationState()
  {
    if (!GetSimulationState(*m_SimulationData))
      return "";

    std::string content;
    SerializeToString(*m_SimulationData, content);
    return content;
  }
  bool PVEngine::GetSimulationState(pulse::study::bind::patient_variability::SimulationData& sim)
  {
    if (m_Controllers.empty())
    {
      Error("No engines have been allocated yet...");
      return false;
    }

    for (int p = 0; p < sim.patientcomparisons_size(); p++)
    {
      Controller* pc = m_Controllers[p];

      // Write out data
  
      // Update the sim
    }
    return true;
  }

  void PVEngine::TrackData(SEEngineTracker& trkr, const std::string& csv_filename)
  {
    trkr.GetDataRequestManager().SetResultsFilename(csv_filename);
    //Create data requests

    trkr.SetupRequests();
  }

  bool PVEngine::SerializeToString(pulse::study::bind::patient_variability::SimulationData& src, std::string& dst)
  {
    google::protobuf::util::JsonPrintOptions printOpts;
    printOpts.add_whitespace = true;
    printOpts.preserve_proto_field_names = true;
    printOpts.always_print_primitive_fields = true;
    return google::protobuf::util::MessageToJsonString(src, &dst, printOpts).ok();
  }
  bool PVEngine::SerializeFromString(const std::string& src, pulse::study::bind::patient_variability::SimulationData& dst)
  {
    google::protobuf::util::JsonParseOptions parseOpts;
    google::protobuf::SetLogHandler([](google::protobuf::LogLevel level, const char* filename, int line, const std::string& message)
    {
      std::cout << "[" << level << "] " << filename << "::" << line << " " << message;
    });
    google::protobuf::util::Status stat = google::protobuf::util::JsonStringToMessage(src, &dst, parseOpts);
    if (!stat.ok())
    {
      std::cerr << "Unable to parse json in string : " << stat.ToString() << std::endl;
      return false;
    }
    return true;
  }
}