/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "CommonDataModel.h"
// CDM Features in use
#include "PhysiologyEngine.h"
#include "engine/SEEventManager.h"
#include "utils/ScopedMutex.h"
#define PULSE_DECL CDM_DECL

//--------------------------------------------------------------------------------------------------
/// \brief
/// Creates a Pulse Engine.
///
/// \details
/// The state can be saved as JSON or bytes in the given string.
/// Note that the bytes are binary, not text; we only use the string class as a convenient container.
/// Engine will be in a cleared state if this method fails.
//--------------------------------------------------------------------------------------------------
PULSE_DECL std::unique_ptr<PhysiologyEngine> CreatePulseEngine(Logger* logger=nullptr);

/**
 * An instance of Pulse where the interface is define in stl and base data types.
 * This interface is a thunk layer using serialized cdm objects to drive a Pulse engine.
 */
class PULSE_DECL PulseEngineThunk : public LoggerForward, public SEEventHandler
{
public:
  PulseEngineThunk(const std::string& dataDir = "./");
  virtual ~PulseEngineThunk();

  //void SetConfigurationOverride(std::string const& config); // Not Implemented

  bool ExecuteScenario(std::string const& sceExecOpts, SerializationFormat format);

  bool SerializeFromFile(std::string const& filename, std::string const& data_requests, SerializationFormat data_requests_format);
  bool SerializeToFile(std::string const& filename);

  bool SerializeFromString(std::string const& state, std::string const& data_requests, SerializationFormat format);
  std::string SerializeToString(SerializationFormat format);

  bool InitializeEngine(std::string const& patient_configuration, std::string const& data_requests, SerializationFormat format);

  std::string GetInitialPatient(SerializationFormat format);
  std::string GetConditions(SerializationFormat format);

  void LogToConsole(bool b);
  void KeepLogMessages(bool keep);// Set this to true if you are going to pull messages from the engine
  void SetLogFilename(std::string const& logfile);// Set to empty if no log file is wanted
  std::string PullLogMessages(SerializationFormat format);

  void KeepEventChanges(bool keep);
  std::string PullEvents(SerializationFormat format);
  std::string PullActiveEvents(SerializationFormat format);
  std::string GetPatientAssessment(int type, SerializationFormat format);

  bool ProcessActions(std::string const& actions, SerializationFormat format);
  std::string PullActiveActions(SerializationFormat format);

  bool AdvanceTimeStep();
  double GetTimeStep(std::string const& unit);

  size_t DataLength() const;
  double* PullDataPtr();
  void PullData(std::vector<double>& data);

  virtual void ForwardDebug(const std::string& msg, const std::string& origin);
  virtual void ForwardInfo(const std::string& msg, const std::string& origin);
  virtual void ForwardWarning(const std::string& msg, const std::string& origin);
  virtual void ForwardError(const std::string& msg, const std::string& origin);
  virtual void ForwardFatal(const std::string& msg, const std::string& origin);

  void HandleEvent(eEvent type, bool active, const SEScalarTime* time = nullptr);


protected:
  void SetupDefaultDataRequests();

  PhysiologyEngine& GetPhysiologyEngine();
private:
  class pimpl;
  pimpl* data;
};

#define PULSE_BIND pulse::cpm::bind
#define PULSE_BIND_DECL(type) \
  namespace pulse { namespace cpm { namespace bind { class type##Data; }}}

// I bet there is a way cool macro you could do for these enums...
namespace pulse {

  DEFINE_STATIC_STRING_EX(Version, Pulse_3.0.0);

  class Graph
  {
  public:
    DEFINE_STATIC_STRING(ActiveCardiovascular);
    DEFINE_STATIC_STRING(Cardiovascular);
    DEFINE_STATIC_STRING(Renal);
    DEFINE_STATIC_STRING(Respiratory);
    DEFINE_STATIC_STRING(RespiratoryAndAnesthesiaMachine);
    DEFINE_STATIC_STRING(RespiratoryAndInhaler);
    DEFINE_STATIC_STRING(RespiratoryAndMechanicalVentilation);
    DEFINE_STATIC_STRING(RespiratoryAndMechanicalVentilator);
    DEFINE_STATIC_STRING(RespiratoryAndNasalCannula);
    DEFINE_STATIC_STRING(RespiratoryAndNonRebreatherMask);
    DEFINE_STATIC_STRING(RespiratoryAndSimpleMask);
    DEFINE_STATIC_STRING(Aerosol);
    DEFINE_STATIC_STRING(AerosolAndInhaler);
    DEFINE_STATIC_STRING(AerosolAndMechanicalVentilation);
    DEFINE_STATIC_STRING(AerosolAndMechanicalVentilator);
    DEFINE_STATIC_STRING(AnesthesiaMachine);
    DEFINE_STATIC_STRING(MechanicalVentilator);

    static const std::vector<std::string>& GetValues()
    {
      ScopedMutex lock;
      if (_values.empty())
      {
        _values.push_back(ActiveCardiovascular);
        _values.push_back(Cardiovascular);
        _values.push_back(Renal);
        _values.push_back(Respiratory);
        _values.push_back(RespiratoryAndAnesthesiaMachine);
        _values.push_back(RespiratoryAndInhaler);
        _values.push_back(RespiratoryAndMechanicalVentilation);
        _values.push_back(RespiratoryAndMechanicalVentilator);
        _values.push_back(RespiratoryAndNasalCannula);
        _values.push_back(RespiratoryAndNonRebreatherMask);
        _values.push_back(RespiratoryAndSimpleMask);
        _values.push_back(Aerosol);
        _values.push_back(AerosolAndInhaler);
        _values.push_back(AerosolAndMechanicalVentilation);
        _values.push_back(AerosolAndMechanicalVentilator);
        _values.push_back(AnesthesiaMachine);
        _values.push_back(MechanicalVentilator);
      }
      return _values;
    }
  protected:
    static std::vector<std::string> _values;
  };

  // TODO Rachel
  class CerebrospinalFluidCompartment
  {
  public:
    DEFINE_STATIC_STRING(IntracranialSpace);

    static const std::vector<std::string>& GetValues()
    {
      ScopedMutex lock;
      if (_values.empty())
      {
        _values.push_back(IntracranialSpace);
      }
      return _values;
    }
  protected:
    static std::vector<std::string> _values;
  };

  class ChymeCompartment
  {
  public:
    DEFINE_STATIC_STRING_EX(SmallIntestine, SmallIntestineChyme);

    static const std::vector<std::string>& GetValues()
    {
      ScopedMutex lock;
      if (_values.empty())
      {
        _values.push_back(SmallIntestine);
      }
      return _values;
    }
  protected:
    static std::vector<std::string> _values;
  };

  class ChymeLink
  {
  public:
    DEFINE_STATIC_STRING(SmallIntestineChymeToVasculature);

    static const std::vector<std::string>& GetValues()
    {
      ScopedMutex lock;
      if (_values.empty())
      {
        _values.push_back(SmallIntestineChymeToVasculature);
      }
      return _values;
    }
  protected:
    static std::vector<std::string> _values;
  };

  class PulmonaryCompartment
  {
  public:
    DEFINE_STATIC_STRING(Airway);
    DEFINE_STATIC_STRING(Stomach);
    DEFINE_STATIC_STRING(Carina);
    DEFINE_STATIC_STRING_EX(Lungs, PulmonaryLungs);
    /**/DEFINE_STATIC_STRING_EX(LeftLung, LeftLungPulmonary);
    /***/DEFINE_STATIC_STRING(LeftAnatomicDeadSpace);
    /***/DEFINE_STATIC_STRING(LeftAlveolarDeadSpace);
    /***/DEFINE_STATIC_STRING(LeftAlveoli);
    /**/DEFINE_STATIC_STRING_EX(RightLung, RightLungPulmonary);
    /***/DEFINE_STATIC_STRING(RightAnatomicDeadSpace);
    /***/DEFINE_STATIC_STRING(RightAlveolarDeadSpace);
    /***/DEFINE_STATIC_STRING(RightAlveoli);
    DEFINE_STATIC_STRING(PleuralCavity);
    /**/DEFINE_STATIC_STRING(LeftPleuralCavity);
    /**/DEFINE_STATIC_STRING(RightPleuralCavity);

    DEFINE_STATIC_STRING(LeftAlveoliLeak);
    DEFINE_STATIC_STRING(LeftChestLeak);
    DEFINE_STATIC_STRING(RightAlveoliLeak);
    DEFINE_STATIC_STRING(RightChestLeak);

    static const std::vector<std::string>& GetValues()
    {
      ScopedMutex lock;
      if (_values.empty())
      {
        _values.push_back(Airway);
        _values.push_back(Stomach);
        _values.push_back(Carina);
        _values.push_back(Lungs);
        _values.push_back(LeftLung);
        _values.push_back(LeftAnatomicDeadSpace);
        _values.push_back(LeftAlveolarDeadSpace);
        _values.push_back(LeftAlveoli);
        _values.push_back(RightLung);
        _values.push_back(RightAnatomicDeadSpace);
        _values.push_back(RightAlveolarDeadSpace);
        _values.push_back(RightAlveoli);
        _values.push_back(PleuralCavity);
        _values.push_back(LeftPleuralCavity);
        _values.push_back(RightPleuralCavity);
        _values.push_back(LeftAlveoliLeak);
        _values.push_back(LeftChestLeak);
        _values.push_back(RightAlveoliLeak);
        _values.push_back(RightChestLeak);
      }
      return _values;
    }
  protected:
    static std::vector<std::string> _values;
  };

  class PulmonaryLink
  {
  public:
    DEFINE_STATIC_STRING(EnvironmentToAirway);
    DEFINE_STATIC_STRING(AirwayToCarina);
    DEFINE_STATIC_STRING(AirwayToStomach);
    DEFINE_STATIC_STRING(CarinaToLeftAnatomicDeadSpace);
    DEFINE_STATIC_STRING(LeftAnatomicDeadSpaceToLeftAlveolarDeadSpace);
    DEFINE_STATIC_STRING(LeftAlveolarDeadSpaceToLeftAlveoli);
    DEFINE_STATIC_STRING(CarinaToRightAnatomicDeadSpace);
    DEFINE_STATIC_STRING(RightAnatomicDeadSpaceToRightAlveolarDeadSpace);
    DEFINE_STATIC_STRING(RightAlveolarDeadSpaceToRightAlveoli);
    DEFINE_STATIC_STRING(EnvironmentToLeftChestLeak);
    DEFINE_STATIC_STRING(EnvironmentToRightChestLeak);
    DEFINE_STATIC_STRING(LeftAlveoliLeakToLeftPleural);
    DEFINE_STATIC_STRING(LeftAlveoliToLeftAlveoliLeak);
    DEFINE_STATIC_STRING(LeftChestLeakToLeftPleural);
    DEFINE_STATIC_STRING(LeftPleuralToEnvironment);
    DEFINE_STATIC_STRING(RightAlveoliLeakToRightPleural);
    DEFINE_STATIC_STRING(RightAlveoliToRightAlveoliLeak);
    DEFINE_STATIC_STRING(RightChestLeakToRightPleural);
    DEFINE_STATIC_STRING(RightPleuralToEnvironment);

    static const std::vector<std::string>& GetValues()
    {
      ScopedMutex lock;
      if (_values.empty())
      {
        _values.push_back(EnvironmentToAirway);
        _values.push_back(AirwayToCarina);
        _values.push_back(AirwayToStomach);
        _values.push_back(CarinaToLeftAnatomicDeadSpace);
        _values.push_back(LeftAnatomicDeadSpaceToLeftAlveolarDeadSpace);
        _values.push_back(LeftAlveolarDeadSpaceToLeftAlveoli);
        _values.push_back(CarinaToRightAnatomicDeadSpace);
        _values.push_back(RightAnatomicDeadSpaceToRightAlveolarDeadSpace);
        _values.push_back(RightAlveolarDeadSpaceToRightAlveoli);
        _values.push_back(EnvironmentToLeftChestLeak);
        _values.push_back(EnvironmentToRightChestLeak);
        _values.push_back(LeftAlveoliLeakToLeftPleural);
        _values.push_back(LeftAlveoliToLeftAlveoliLeak);
        _values.push_back(LeftChestLeakToLeftPleural);
        _values.push_back(LeftPleuralToEnvironment);
        _values.push_back(RightAlveoliLeakToRightPleural);
        _values.push_back(RightAlveoliToRightAlveoliLeak);
        _values.push_back(RightChestLeakToRightPleural);
        _values.push_back(RightPleuralToEnvironment);
      }
      return _values;
    }
  protected:
    static std::vector<std::string> _values;
  };

  class TissueCompartment
  {
  public:
    DEFINE_STATIC_STRING_EX(Bone, BoneTissue);
    DEFINE_STATIC_STRING_EX(Brain, BrainTissue);
    DEFINE_STATIC_STRING_EX(Fat, FatTissue);
    DEFINE_STATIC_STRING_EX(Gut, GutTissue);
    DEFINE_STATIC_STRING_EX(LeftKidney, LeftKidneyTissue);
    DEFINE_STATIC_STRING_EX(LeftLung, LeftLungTissue);
    DEFINE_STATIC_STRING_EX(Liver, LiverTissue);
    DEFINE_STATIC_STRING_EX(Muscle, MuscleTissue);
    DEFINE_STATIC_STRING_EX(Myocardium, MyocardiumTissue);
    DEFINE_STATIC_STRING_EX(RightKidney, RightKidneyTissue);
    DEFINE_STATIC_STRING_EX(RightLung, RightLungTissue);
    DEFINE_STATIC_STRING_EX(Skin, SkinTissue);
    DEFINE_STATIC_STRING_EX(Spleen, SpleenTissue);

    static const std::vector<std::string>& GetValues()
    {
      ScopedMutex lock;
      if (_values.empty())
      {
        _values.push_back(Bone);
        _values.push_back(Brain);
        _values.push_back(Fat);
        _values.push_back(Gut);
        _values.push_back(LeftKidney);
        _values.push_back(LeftLung);
        _values.push_back(Liver);
        _values.push_back(Muscle);
        _values.push_back(Myocardium);
        _values.push_back(RightKidney);
        _values.push_back(RightLung);
        _values.push_back(Skin);
        _values.push_back(Spleen);
      }
      return _values;
    }
  protected:
    static std::vector<std::string> _values;
  };

  class ExtravascularCompartment
  {
  public:
    DEFINE_STATIC_STRING_EX(BoneExtracellular, BoneTissueExtracellular);
    DEFINE_STATIC_STRING_EX(BrainExtracellular, BrainTissueExtracellular);
    DEFINE_STATIC_STRING_EX(FatExtracellular, FatTissueExtracellular);
    DEFINE_STATIC_STRING_EX(GutExtracellular, GutTissueExtracellular);
    DEFINE_STATIC_STRING_EX(LeftKidneyExtracellular, LeftKidneyTissueExtracellular);
    DEFINE_STATIC_STRING_EX(LeftLungExtracellular, LeftLungTissueExtracellular);
    DEFINE_STATIC_STRING_EX(LiverExtracellular, LiverTissueExtracellular);
    DEFINE_STATIC_STRING_EX(MuscleExtracellular, MuscleTissueExtracellular);
    DEFINE_STATIC_STRING_EX(MyocardiumExtracellular, MyocardiumTissueExtracellular);
    DEFINE_STATIC_STRING_EX(RightKidneyExtracellular, RightKidneyTissueExtracellular);
    DEFINE_STATIC_STRING_EX(RightLungExtracellular, RightLungTissueExtracellular);
    DEFINE_STATIC_STRING_EX(SkinExtracellular, SkinTissueExtracellular);
    DEFINE_STATIC_STRING_EX(SpleenExtracellular, SpleenTissueExtracellular);

    DEFINE_STATIC_STRING_EX(BoneIntracellular, BoneTissueIntracellular);
    DEFINE_STATIC_STRING_EX(BrainIntracellular, BrainTissueIntracellular);
    DEFINE_STATIC_STRING_EX(FatIntracellular, FatTissueIntracellular);
    DEFINE_STATIC_STRING_EX(GutIntracellular, GutTissueIntracellular);
    DEFINE_STATIC_STRING_EX(LeftKidneyIntracellular, LeftKidneyTissueIntracellular);
    DEFINE_STATIC_STRING_EX(LeftLungIntracellular, LeftLungTissueIntracellular);
    DEFINE_STATIC_STRING_EX(LiverIntracellular, LiverTissueIntracellular);
    DEFINE_STATIC_STRING_EX(MuscleIntracellular, MuscleTissueIntracellular);
    DEFINE_STATIC_STRING_EX(MyocardiumIntracellular, MyocardiumTissueIntracellular);
    DEFINE_STATIC_STRING_EX(RightKidneyIntracellular, RightKidneyTissueIntracellular);
    DEFINE_STATIC_STRING_EX(RightLungIntracellular, RightLungTissueIntracellular);
    DEFINE_STATIC_STRING_EX(SkinIntracellular, SkinTissueIntracellular);
    DEFINE_STATIC_STRING_EX(SpleenIntracellular, SpleenTissueIntracellular);

    static const std::vector<std::string>& GetValues()
    {
      ScopedMutex lock;
      if (_values.empty())
      {
        _values.push_back(BoneExtracellular);
        _values.push_back(BrainExtracellular);
        _values.push_back(FatExtracellular);
        _values.push_back(GutExtracellular);
        _values.push_back(LeftKidneyExtracellular);
        _values.push_back(LeftLungExtracellular);
        _values.push_back(LiverExtracellular);
        _values.push_back(MuscleExtracellular);
        _values.push_back(MyocardiumExtracellular);
        _values.push_back(RightKidneyExtracellular);
        _values.push_back(RightLungExtracellular);
        _values.push_back(SkinExtracellular);
        _values.push_back(SpleenExtracellular);

        _values.push_back(BoneIntracellular);
        _values.push_back(BrainIntracellular);
        _values.push_back(FatIntracellular);
        _values.push_back(GutIntracellular);
        _values.push_back(LeftKidneyIntracellular);
        _values.push_back(LeftLungIntracellular);
        _values.push_back(LiverIntracellular);
        _values.push_back(MuscleIntracellular);
        _values.push_back(MyocardiumIntracellular);
        _values.push_back(RightKidneyIntracellular);
        _values.push_back(RightLungIntracellular);
        _values.push_back(SkinIntracellular);
        _values.push_back(SpleenIntracellular);
      }
      return _values;
    }
  protected:
    static std::vector<std::string> _values;
  };

  class VascularCompartment
  {
  public:
    // Cardio
    DEFINE_STATIC_STRING(Aorta);
    DEFINE_STATIC_STRING(Heart);
    /**/DEFINE_STATIC_STRING_EX(Myocardium, MyocardiumVasculature);
    /**/DEFINE_STATIC_STRING(LeftHeart);
    /**/DEFINE_STATIC_STRING(RightHeart);
    /**/DEFINE_STATIC_STRING(Pericardium);
    DEFINE_STATIC_STRING(VenaCava);
    // Pulmonary
    DEFINE_STATIC_STRING(PulmonaryArteries);
    DEFINE_STATIC_STRING(PulmonaryCapillaries);
    DEFINE_STATIC_STRING(PulmonaryVeins);
    DEFINE_STATIC_STRING_EX(Lungs, LungsVasculature);
    /**/DEFINE_STATIC_STRING_EX(LeftLung, LeftLungVasculature);
    /***/DEFINE_STATIC_STRING(LeftPulmonaryArteries);
    /***/DEFINE_STATIC_STRING(LeftPulmonaryCapillaries);
    /***/DEFINE_STATIC_STRING(LeftPulmonaryVeins);
    /**/DEFINE_STATIC_STRING_EX(RightLung, RightLungVasculature);
    /***/DEFINE_STATIC_STRING(RightPulmonaryArteries);
    /***/DEFINE_STATIC_STRING(RightPulmonaryCapillaries);
    /***/DEFINE_STATIC_STRING(RightPulmonaryVeins);
    // Renal
    DEFINE_STATIC_STRING_EX(Kidneys, KidneyVasculature);
    /**/DEFINE_STATIC_STRING_EX(LeftKidney, LeftKidneyVasculature);
    /***/DEFINE_STATIC_STRING(LeftRenalArtery);
    /***/DEFINE_STATIC_STRING(LeftNephron);
    /*********/DEFINE_STATIC_STRING(LeftAfferentArteriole);
    /*********/DEFINE_STATIC_STRING(LeftGlomerularCapillaries);
    /*********/DEFINE_STATIC_STRING(LeftEfferentArteriole);
    /*********/DEFINE_STATIC_STRING(LeftPeritubularCapillaries);
    /*********/DEFINE_STATIC_STRING(LeftBowmansCapsules);
    /*********/DEFINE_STATIC_STRING(LeftTubules);
    /***/DEFINE_STATIC_STRING(LeftRenalVein);
    /**/DEFINE_STATIC_STRING_EX(RightKidney, RightKidneyVasculature);
    /***/DEFINE_STATIC_STRING(RightRenalArtery);
    /***/DEFINE_STATIC_STRING(RightNephron);
    /*********/DEFINE_STATIC_STRING(RightAfferentArteriole);
    /*********/DEFINE_STATIC_STRING(RightGlomerularCapillaries);
    /*********/DEFINE_STATIC_STRING(RightEfferentArteriole);
    /*********/DEFINE_STATIC_STRING(RightPeritubularCapillaries);
    /*********/DEFINE_STATIC_STRING(RightBowmansCapsules);
    /*********/DEFINE_STATIC_STRING(RightTubules);
    /***/DEFINE_STATIC_STRING(RightRenalVein);
    // General Organs and Periphery
    DEFINE_STATIC_STRING_EX(Bone, BoneVasculature);
    DEFINE_STATIC_STRING_EX(Brain, BrainVasculature);
    DEFINE_STATIC_STRING_EX(Fat, FatVasculature);
    DEFINE_STATIC_STRING_EX(Gut, GutVasculature);
    /**/DEFINE_STATIC_STRING_EX(Splanchnic, SplanchnicVasculature);
    /**/DEFINE_STATIC_STRING_EX(SmallIntestine, SmallIntestineVasculature);
    /**/DEFINE_STATIC_STRING_EX(LargeIntestine, LargeIntestineVasculature);
    DEFINE_STATIC_STRING_EX(Liver, LiverVasculature);
    DEFINE_STATIC_STRING_EX(Spleen, SpleenVasculature);
    DEFINE_STATIC_STRING_EX(Skin, SkinVasculature);
    DEFINE_STATIC_STRING_EX(Muscle, MuscleVasculature);
    DEFINE_STATIC_STRING_EX(LeftArm, LeftArmVasculature);
    DEFINE_STATIC_STRING_EX(LeftLeg, LeftLegVasculature);
    DEFINE_STATIC_STRING_EX(RightArm, RightArmVasculature);
    DEFINE_STATIC_STRING_EX(RightLeg, RightLegVasculature);
    DEFINE_STATIC_STRING(Abdomen);
    /**/DEFINE_STATIC_STRING(AbdominalCavity);

    DEFINE_STATIC_STRING(Ground);


    static const std::vector<std::string>& GetValues()
    {
      ScopedMutex lock;
      if (_values.empty())
      {
        _values.push_back(Aorta);
        _values.push_back(Abdomen);
        _values.push_back(Heart);
        _values.push_back(Myocardium);
        _values.push_back(LeftHeart);
        _values.push_back(RightHeart);
        _values.push_back(Pericardium);
        _values.push_back(VenaCava);
        _values.push_back(PulmonaryArteries);
        _values.push_back(PulmonaryCapillaries);
        _values.push_back(PulmonaryVeins);
        _values.push_back(Lungs);
        _values.push_back(LeftLung);
        _values.push_back(LeftPulmonaryArteries);
        _values.push_back(LeftPulmonaryCapillaries);
        _values.push_back(LeftPulmonaryVeins);
        _values.push_back(RightLung);
        _values.push_back(RightPulmonaryArteries);
        _values.push_back(RightPulmonaryCapillaries);
        _values.push_back(RightPulmonaryVeins);
        _values.push_back(Kidneys);
        _values.push_back(LeftKidney);
        _values.push_back(LeftRenalArtery);
        _values.push_back(LeftNephron);
        _values.push_back(LeftAfferentArteriole);
        _values.push_back(LeftGlomerularCapillaries);
        _values.push_back(LeftEfferentArteriole);
        _values.push_back(LeftPeritubularCapillaries);
        _values.push_back(LeftBowmansCapsules);
        _values.push_back(LeftTubules);
        _values.push_back(LeftRenalVein);
        _values.push_back(RightKidney);
        _values.push_back(RightRenalArtery);
        _values.push_back(RightNephron);
        _values.push_back(RightAfferentArteriole);
        _values.push_back(RightGlomerularCapillaries);
        _values.push_back(RightEfferentArteriole);
        _values.push_back(RightPeritubularCapillaries);
        _values.push_back(RightBowmansCapsules);
        _values.push_back(RightTubules);
        _values.push_back(RightRenalVein);
        _values.push_back(Bone);
        _values.push_back(Brain);
        _values.push_back(Fat);
        _values.push_back(Gut);
        _values.push_back(Splanchnic);
        _values.push_back(SmallIntestine);
        _values.push_back(LargeIntestine);
        _values.push_back(Liver);
        _values.push_back(Spleen);
        _values.push_back(Skin);
        _values.push_back(Muscle);
        _values.push_back(LeftArm);
        _values.push_back(LeftLeg);
        _values.push_back(RightArm);
        _values.push_back(RightLeg);
        //_values.push_back(Ground);
      }
      return _values;
    }
  protected:
    static std::vector<std::string> _values;
  };

  class VascularLink
  {
  public:
    // Heart and Lungs
    DEFINE_STATIC_STRING(VenaCavaToRightHeart);
    DEFINE_STATIC_STRING(RightHeartToLeftPulmonaryArteries);
    DEFINE_STATIC_STRING(LeftPulmonaryArteriesToCapillaries);
    DEFINE_STATIC_STRING(LeftPulmonaryArteriesToVeins);
    DEFINE_STATIC_STRING(LeftPulmonaryCapillariesToVeins);
    DEFINE_STATIC_STRING(LeftPulmonaryVeinsToLeftHeart);
    DEFINE_STATIC_STRING(RightHeartToRightPulmonaryArteries);
    DEFINE_STATIC_STRING(RightPulmonaryArteriesToCapillaries);
    DEFINE_STATIC_STRING(RightPulmonaryArteriesToVeins);
    DEFINE_STATIC_STRING(RightPulmonaryCapillariesToVeins);
    DEFINE_STATIC_STRING(RightPulmonaryVeinsToLeftHeart);
    DEFINE_STATIC_STRING(LeftHeartToAorta);
    // Bone 
    DEFINE_STATIC_STRING(AortaToBone);
    DEFINE_STATIC_STRING(BoneToVenaCava);
    // Brain 
    DEFINE_STATIC_STRING(AortaToBrain);
    DEFINE_STATIC_STRING(BrainToVenaCava);
    // Fat 
    DEFINE_STATIC_STRING(AortaToFat);
    DEFINE_STATIC_STRING(FatToVenaCava);
    // Large Intestine 
    DEFINE_STATIC_STRING(AortaToLargeIntestine);
    DEFINE_STATIC_STRING(LargeIntestineToLiver);
    // Left Arm 
    DEFINE_STATIC_STRING(AortaToLeftArm);
    DEFINE_STATIC_STRING(LeftArmToVenaCava);
    // Left Kidney 
    DEFINE_STATIC_STRING(AortaToLeftKidney);
    DEFINE_STATIC_STRING(LeftKidneyToVenaCava);
    // Left Leg 
    DEFINE_STATIC_STRING(AortaToLeftLeg);
    DEFINE_STATIC_STRING(LeftLegToVenaCava);
    // Liver 
    DEFINE_STATIC_STRING(AortaToLiver);
    DEFINE_STATIC_STRING(LiverToVenaCava);
    // Muscle 
    DEFINE_STATIC_STRING(AortaToMuscle);
    DEFINE_STATIC_STRING(MuscleToVenaCava);
    // Myocardium 
    DEFINE_STATIC_STRING(AortaToMyocardium);
    DEFINE_STATIC_STRING(MyocardiumToVenaCava);
    // Right Arm 
    DEFINE_STATIC_STRING(AortaToRightArm);
    DEFINE_STATIC_STRING(RightArmToVenaCava);
    // Right Kidney 
    DEFINE_STATIC_STRING(AortaToRightKidney);
    DEFINE_STATIC_STRING(RightKidneyToVenaCava);
    // Right Leg 
    DEFINE_STATIC_STRING(AortaToRightLeg);
    DEFINE_STATIC_STRING(RightLegToVenaCava);
    // Skin 
    DEFINE_STATIC_STRING(AortaToSkin);
    DEFINE_STATIC_STRING(SkinToVenaCava);
    // Small Intestine 
    DEFINE_STATIC_STRING(AortaToSmallIntestine);
    DEFINE_STATIC_STRING(SmallIntestineToLiver);
    // Splanchnic 
    DEFINE_STATIC_STRING(AortaToSplanchnic);
    DEFINE_STATIC_STRING(SplanchnicToLiver);
    // Spleen 
    DEFINE_STATIC_STRING(AortaToSpleen);
    DEFINE_STATIC_STRING(SpleenToLiver);

    // Hemorrhage and IV
    DEFINE_STATIC_STRING(VenaCavaIV);

    // Vascular To Tissue Links
    DEFINE_STATIC_STRING(BoneVascularToTissue);
    DEFINE_STATIC_STRING(BrainVascularToTissue);
    DEFINE_STATIC_STRING(FatVascularToTissue);
    DEFINE_STATIC_STRING(SmallIntestineVascularToTissue);
    DEFINE_STATIC_STRING(LargeIntestineVascularToTissue);
    DEFINE_STATIC_STRING(SplanchnicVascularToTissue);
    DEFINE_STATIC_STRING(LeftKidneyVascularToTissue);
    DEFINE_STATIC_STRING(LeftLungVascularToTissue);
    DEFINE_STATIC_STRING(LiverVascularToTissue);
    DEFINE_STATIC_STRING(MuscleVascularToTissue);
    DEFINE_STATIC_STRING(MyocardiumVascularToTissue);
    DEFINE_STATIC_STRING(RightKidneyVascularToTissue);
    DEFINE_STATIC_STRING(RightLungVascularToTissue);
    DEFINE_STATIC_STRING(SkinVascularToTissue);
    DEFINE_STATIC_STRING(SpleenVascularToTissue);

    // Renal Links
    // Reusing 'AortaToLeftKidney' to connect the Aorta To Left Renal Artery
    DEFINE_STATIC_STRING(LeftRenalArteryToAfferentArteriole);
    DEFINE_STATIC_STRING(LeftAfferentArterioleToGlomerularCapillaries);
    DEFINE_STATIC_STRING(LeftGlomerularCapillariesToEfferentArteriole);
    DEFINE_STATIC_STRING(LeftGlomerularCapillariesToBowmansCapsules);
    DEFINE_STATIC_STRING(LeftBowmansCapsulesToTubules);
    DEFINE_STATIC_STRING(LeftTubulesToPeritubularCapillaries);
    DEFINE_STATIC_STRING(LeftEfferentArterioleToPeritubularCapillaries);
    DEFINE_STATIC_STRING(LeftPeritubularCapillariesToRenalVein);
    // Reusing 'LeftKidneyToVenaCava' to connect the Left Renal Vein To Vena Cava

    // Reusing 'AortaToRightKidney' to connect the Aorta To Right Renal Artery
    DEFINE_STATIC_STRING(RightRenalArteryToAfferentArteriole);
    DEFINE_STATIC_STRING(RightAfferentArterioleToGlomerularCapillaries);
    DEFINE_STATIC_STRING(RightGlomerularCapillariesToEfferentArteriole);
    DEFINE_STATIC_STRING(RightGlomerularCapillariesToBowmansCapsules);
    DEFINE_STATIC_STRING(RightBowmansCapsulesToTubules);
    DEFINE_STATIC_STRING(RightTubulesToPeritubularCapillaries);
    DEFINE_STATIC_STRING(RightEfferentArterioleToPeritubularCapillaries);
    DEFINE_STATIC_STRING(RightPeritubularCapillariesToRenalVein);
    // Reusing 'RightKidneyToVenaCava' to connect the Right Renal Vein To Vena Cava);   


    static const std::vector<std::string>& GetValues()
    {
      ScopedMutex lock;
      if (_values.empty())
      {
        _values.push_back(VenaCavaToRightHeart);
        _values.push_back(RightHeartToLeftPulmonaryArteries);
        _values.push_back(LeftPulmonaryArteriesToCapillaries);
        _values.push_back(LeftPulmonaryArteriesToVeins);
        _values.push_back(LeftPulmonaryCapillariesToVeins);
        _values.push_back(LeftPulmonaryVeinsToLeftHeart);
        _values.push_back(RightHeartToRightPulmonaryArteries);
        _values.push_back(RightPulmonaryArteriesToCapillaries);
        _values.push_back(RightPulmonaryArteriesToVeins);
        _values.push_back(RightPulmonaryCapillariesToVeins);
        _values.push_back(RightPulmonaryVeinsToLeftHeart);
        _values.push_back(LeftHeartToAorta);
        _values.push_back(AortaToBone);
        _values.push_back(BoneToVenaCava);
        _values.push_back(AortaToBrain);
        _values.push_back(BrainToVenaCava);
        _values.push_back(AortaToFat);
        _values.push_back(FatToVenaCava);
        _values.push_back(AortaToLargeIntestine);
        _values.push_back(LargeIntestineToLiver);
        _values.push_back(AortaToLeftArm);
        _values.push_back(LeftArmToVenaCava);
        _values.push_back(AortaToLeftKidney);
        _values.push_back(LeftKidneyToVenaCava);
        _values.push_back(AortaToLeftLeg);
        _values.push_back(LeftLegToVenaCava);
        _values.push_back(AortaToLiver);
        _values.push_back(LiverToVenaCava);
        _values.push_back(AortaToMuscle);
        _values.push_back(MuscleToVenaCava);
        _values.push_back(AortaToMyocardium);
        _values.push_back(MyocardiumToVenaCava);
        _values.push_back(AortaToRightArm);
        _values.push_back(RightArmToVenaCava);
        _values.push_back(AortaToRightKidney);
        _values.push_back(RightKidneyToVenaCava);
        _values.push_back(AortaToRightLeg);
        _values.push_back(RightLegToVenaCava);
        _values.push_back(AortaToSkin);
        _values.push_back(SkinToVenaCava);
        _values.push_back(AortaToSmallIntestine);
        _values.push_back(SmallIntestineToLiver);
        _values.push_back(AortaToSplanchnic);
        _values.push_back(SplanchnicToLiver);
        _values.push_back(AortaToSpleen);
        _values.push_back(SpleenToLiver);

        _values.push_back(BoneVascularToTissue);
        _values.push_back(BrainVascularToTissue);
        _values.push_back(FatVascularToTissue);
        _values.push_back(SmallIntestineVascularToTissue);
        _values.push_back(LargeIntestineVascularToTissue);
        _values.push_back(SplanchnicVascularToTissue);
        _values.push_back(LeftKidneyVascularToTissue);
        _values.push_back(LeftLungVascularToTissue);
        _values.push_back(LiverVascularToTissue);
        _values.push_back(MuscleVascularToTissue);
        _values.push_back(MyocardiumVascularToTissue);
        _values.push_back(RightKidneyVascularToTissue);
        _values.push_back(RightLungVascularToTissue);
        _values.push_back(SkinVascularToTissue);
        _values.push_back(SpleenVascularToTissue);

        _values.push_back(LeftRenalArteryToAfferentArteriole);
        _values.push_back(LeftAfferentArterioleToGlomerularCapillaries);
        _values.push_back(LeftGlomerularCapillariesToEfferentArteriole);
        _values.push_back(LeftGlomerularCapillariesToBowmansCapsules);
        _values.push_back(LeftBowmansCapsulesToTubules);
        _values.push_back(LeftTubulesToPeritubularCapillaries);
        _values.push_back(LeftEfferentArterioleToPeritubularCapillaries);
        _values.push_back(LeftPeritubularCapillariesToRenalVein);
        _values.push_back(RightRenalArteryToAfferentArteriole);
        _values.push_back(RightAfferentArterioleToGlomerularCapillaries);
        _values.push_back(RightGlomerularCapillariesToEfferentArteriole);
        _values.push_back(RightGlomerularCapillariesToBowmansCapsules);
        _values.push_back(RightBowmansCapsulesToTubules);
        _values.push_back(RightTubulesToPeritubularCapillaries);
        _values.push_back(RightEfferentArterioleToPeritubularCapillaries);
        _values.push_back(RightPeritubularCapillariesToRenalVein);
      }
      return _values;
    }
  protected:
    static std::vector<std::string> _values;
  };

  class UrineCompartment
  {
  public:
    DEFINE_STATIC_STRING(Ureters);
    /*****/DEFINE_STATIC_STRING(LeftUreter);
    /*****/DEFINE_STATIC_STRING(RightUreter);
    DEFINE_STATIC_STRING(Bladder);

    static const std::vector<std::string>& GetValues()
    {
      ScopedMutex lock;
      if (_values.empty())
      {
        _values.push_back(Ureters);
        _values.push_back(LeftUreter);
        _values.push_back(RightUreter);
        _values.push_back(Bladder);
      }
      return _values;
    }
  protected:
    static std::vector<std::string> _values;
  };

  class UrineLink
  {
  public:
    DEFINE_STATIC_STRING(LeftTubulesToUreter);
    DEFINE_STATIC_STRING(LeftUreterToBladder);

    DEFINE_STATIC_STRING(RightTubulesToUreter);
    DEFINE_STATIC_STRING(RightUreterToBladder);

    DEFINE_STATIC_STRING(BladderToGround);
    DEFINE_STATIC_STRING(BladderToGroundSource);

    static const std::vector<std::string>& GetValues()
    {
      ScopedMutex lock;
      if (_values.empty())
      {
        _values.push_back(LeftTubulesToUreter);
        _values.push_back(LeftUreterToBladder);
        _values.push_back(RightTubulesToUreter);
        _values.push_back(RightUreterToBladder);
        _values.push_back(BladderToGround);
        _values.push_back(BladderToGroundSource);
      }
      return _values;
    }
  protected:
    static std::vector<std::string> _values;
  };

  class LymphCompartment
  {
  public:
    DEFINE_STATIC_STRING(Lymph);

    static const std::vector<std::string>& GetValues()
    {
      ScopedMutex lock;
      if (_values.empty())
      {
        _values.push_back(Lymph);
      }
      return _values;
    }
  protected:
    static std::vector<std::string> _values;
  };

  class LymphLink
  {
  public:
    DEFINE_STATIC_STRING(BoneTissueToLymph);
    DEFINE_STATIC_STRING(BrainTissueToLymph);
    DEFINE_STATIC_STRING(FatTissueToLymph);
    DEFINE_STATIC_STRING(GutTissueToLymph);
    DEFINE_STATIC_STRING(LeftKidneyTissueToLymph);
    DEFINE_STATIC_STRING(LeftLungTissueToLymph);
    DEFINE_STATIC_STRING(LiverTissueToLymph);
    DEFINE_STATIC_STRING(MuscleTissueToLymph);
    DEFINE_STATIC_STRING(MyocardiumTissueToLymph);
    DEFINE_STATIC_STRING(RightKidneyTissueToLymph);
    DEFINE_STATIC_STRING(RightLungTissueToLymph);
    DEFINE_STATIC_STRING(SkinTissueToLymph);
    DEFINE_STATIC_STRING(SpleenTissueToLymph);

    DEFINE_STATIC_STRING(LymphToVenaCava);

    static const std::vector<std::string>& GetValues()
    {
      ScopedMutex lock;
      if (_values.empty())
      {
        _values.push_back(BoneTissueToLymph);
        _values.push_back(BrainTissueToLymph);
        _values.push_back(FatTissueToLymph);
        _values.push_back(GutTissueToLymph);
        _values.push_back(LeftKidneyTissueToLymph);
        _values.push_back(LeftLungTissueToLymph);
        _values.push_back(LiverTissueToLymph);
        _values.push_back(MuscleTissueToLymph);
        _values.push_back(MyocardiumTissueToLymph);
        _values.push_back(RightKidneyTissueToLymph);
        _values.push_back(RightLungTissueToLymph);
        _values.push_back(SkinTissueToLymph);
        _values.push_back(SpleenTissueToLymph);
        _values.push_back(LymphToVenaCava);
      }
      return _values;
    }
  protected:
    static std::vector<std::string> _values;
  };

  class TemperatureCompartment
  {
  public:
    DEFINE_STATIC_STRING(Active);
    DEFINE_STATIC_STRING(Ambient);
    DEFINE_STATIC_STRING(Clothing);
    DEFINE_STATIC_STRING(Enclosure);
    DEFINE_STATIC_STRING(ExternalCore);
    DEFINE_STATIC_STRING(ExternalSkin);
    DEFINE_STATIC_STRING(ExternalGround);
    DEFINE_STATIC_STRING(InternalCore);
    DEFINE_STATIC_STRING(InternalSkin);
    DEFINE_STATIC_STRING(InternalGround);

    static const std::vector<std::string>& GetValues()
    {
      ScopedMutex lock;
      if (_values.empty())
      {
        _values.push_back(Active);
        _values.push_back(Ambient);
        _values.push_back(Clothing);
        _values.push_back(Enclosure);
        _values.push_back(ExternalCore);
        _values.push_back(ExternalSkin);
        _values.push_back(ExternalGround);
        _values.push_back(InternalCore);
        _values.push_back(InternalSkin);
        _values.push_back(InternalGround);
      }
      return _values;
    }
  protected:
    static std::vector<std::string> _values;
  };

  class TemperatureLink
  {
  public:

    DEFINE_STATIC_STRING(ActiveToClothing);
    DEFINE_STATIC_STRING(ClothingToEnclosure);
    DEFINE_STATIC_STRING(ClothingToEnvironment);
    DEFINE_STATIC_STRING(ExternalCoreToGround);
    DEFINE_STATIC_STRING(GroundToActive);
    DEFINE_STATIC_STRING(GroundToClothing);
    DEFINE_STATIC_STRING(GroundToEnclosure);
    DEFINE_STATIC_STRING(GroundToEnvironment);
    DEFINE_STATIC_STRING(ExternalSkinToGround);
    DEFINE_STATIC_STRING(ExternalSkinToClothing);
    DEFINE_STATIC_STRING(GroundToInternalCore);
    DEFINE_STATIC_STRING(InternalCoreToInternalSkin);
    DEFINE_STATIC_STRING(InternalCoreToGround);
    DEFINE_STATIC_STRING(InternalSkinToGround);
    DEFINE_STATIC_STRING(InternalCoreToExternalCore);
    DEFINE_STATIC_STRING(InternalSkinToExternalSkin);

    static const std::vector<std::string>& GetValues()
    {
      ScopedMutex lock;
      if (_values.empty())
      {
        _values.push_back(ActiveToClothing);
        _values.push_back(ClothingToEnclosure);
        _values.push_back(ClothingToEnvironment);
        _values.push_back(ExternalCoreToGround);
        _values.push_back(GroundToActive);
        _values.push_back(GroundToClothing);
        _values.push_back(GroundToEnclosure);
        _values.push_back(GroundToEnvironment);
        _values.push_back(ExternalSkinToGround);
        _values.push_back(ExternalSkinToClothing);
        _values.push_back(GroundToInternalCore);
        _values.push_back(InternalCoreToInternalSkin);
        _values.push_back(InternalCoreToGround);
        _values.push_back(InternalSkinToGround);
        _values.push_back(InternalCoreToExternalCore);
        _values.push_back(InternalSkinToExternalSkin);
      }
      return _values;
    }
  protected:
    static std::vector<std::string> _values;
  };

  class EnvironmentCompartment
  {
  public:
    DEFINE_STATIC_STRING(Ambient);

    static const std::vector<std::string>& GetValues()
    {
      ScopedMutex lock;
      if (_values.empty())
      {
        _values.push_back(Ambient);
      }
      return _values;
    }
  protected:
    static std::vector<std::string> _values;
  };

  class AnesthesiaMachineCompartment
  {
  public:
    DEFINE_STATIC_STRING_EX(Connection, AnesthesiaMachineConnection);
    DEFINE_STATIC_STRING_EX(ExpiratoryLimb, AnesthesiaMachineExpiratoryLimb);
    DEFINE_STATIC_STRING_EX(GasInlet, AnesthesiaMachineGasInlet);
    DEFINE_STATIC_STRING_EX(GasSource, AnesthesiaMachineGasSource);
    DEFINE_STATIC_STRING_EX(InspiratoryLimb, AnesthesiaMachineInspiratoryLimb);
    DEFINE_STATIC_STRING_EX(ReliefValve, AnesthesiaMachineReliefValve);
    DEFINE_STATIC_STRING_EX(Scrubber, AnesthesiaMachineScrubber);
    DEFINE_STATIC_STRING_EX(Selector, AnesthesiaMachineSelector);
    DEFINE_STATIC_STRING_EX(Ventilator, AnesthesiaMachineVentilator);
    DEFINE_STATIC_STRING_EX(YPiece, AnesthesiaMachineYPiece);

    static const std::vector<std::string>& GetValues()
    {
      ScopedMutex lock;
      if (_values.empty())
      {
        _values.push_back(Connection);
        _values.push_back(ExpiratoryLimb);
        _values.push_back(GasInlet);
        _values.push_back(GasSource);
        _values.push_back(InspiratoryLimb);
        _values.push_back(ReliefValve);
        _values.push_back(Scrubber);
        _values.push_back(Selector);
        _values.push_back(Ventilator);
        _values.push_back(YPiece);
      }
      return _values;
    }
  protected:
    static std::vector<std::string> _values;
  };

  class AnesthesiaMachineLink
  {
  public:
    DEFINE_STATIC_STRING_EX(EnvironmentToReliefValve, EnvironmentToAnesthesiaMachineReliefValve);
    DEFINE_STATIC_STRING_EX(VentilatorToSelector, AnesthesiaMachineVentilatorToSelector);
    DEFINE_STATIC_STRING_EX(SelectorToReliefValve, AnesthesiaMachineSelectorToReliefValve);
    DEFINE_STATIC_STRING_EX(SelectorToScrubber, AnesthesiaMachineSelectorToScrubber);
    DEFINE_STATIC_STRING_EX(ScrubberToGasInlet, AnesthesiaMachineScrubberToGasInlet);
    DEFINE_STATIC_STRING_EX(GasSourceToGasInlet, AnesthesiaMachineGasSourceToGasInlet);
    DEFINE_STATIC_STRING_EX(GasInletToInspiratoryLimb, AnesthesiaMachineGasInletToInspiratoryLimb);
    DEFINE_STATIC_STRING_EX(InspiratoryLimbToYPiece, AnesthesiaMachineInspiratoryLimbToYPiece);
    DEFINE_STATIC_STRING_EX(YPieceToExpiratoryLimb, AnesthesiaMachineYPieceToExpiratoryLimb);
    DEFINE_STATIC_STRING_EX(ExpiratoryLimbToSelector, AnesthesiaMachineExpiratoryLimbToSelector);
    DEFINE_STATIC_STRING_EX(YPieceToConnection, AnesthesiaMachineYPieceToConnection);
    DEFINE_STATIC_STRING_EX(ConnectionLeak, AnesthesiaMachineConnectionLeak);
    DEFINE_STATIC_STRING_EX(ConnectionToAirway, AnesthesiaMachineConnectionToAirway);

    static const std::vector<std::string>& GetValues()
    {
      ScopedMutex lock;
      if (_values.empty())
      {
        _values.push_back(EnvironmentToReliefValve);
        _values.push_back(VentilatorToSelector);
        _values.push_back(SelectorToReliefValve);
        _values.push_back(SelectorToScrubber);
        _values.push_back(ScrubberToGasInlet);
        _values.push_back(GasSourceToGasInlet);
        _values.push_back(GasInletToInspiratoryLimb);
        _values.push_back(InspiratoryLimbToYPiece);
        _values.push_back(YPieceToExpiratoryLimb);
        _values.push_back(ExpiratoryLimbToSelector);
        _values.push_back(YPieceToConnection);
        _values.push_back(ConnectionLeak);
        _values.push_back(ConnectionToAirway);
      }
      return _values;
    }
  protected:
    static std::vector<std::string> _values;
  };

  class InhalerCompartment
  {
  public:
    DEFINE_STATIC_STRING_EX(Mouthpiece, InhalerMouthpiece);

    static const std::vector<std::string>& GetValues()
    {
      ScopedMutex lock;
      if (_values.empty())
      {
        _values.push_back(Mouthpiece);
      }
      return _values;
    }
  protected:
    static std::vector<std::string> _values;
  };

  class InhalerLink
  {
  public:
    DEFINE_STATIC_STRING_EX(EnvironmentToMouthpiece, EnvironmentToInhalerMouthpiece);
    DEFINE_STATIC_STRING_EX(MouthpieceToAirway, InhalerMouthpieceToAirway);

    static const std::vector<std::string>& GetValues()
    {
      ScopedMutex lock;
      if (_values.empty())
      {
        _values.push_back(EnvironmentToMouthpiece);
        _values.push_back(MouthpieceToAirway);
      }
      return _values;
    }
  protected:
    static std::vector<std::string> _values;
  };

  class MechanicalVentilationCompartment
  {
  public:
    DEFINE_STATIC_STRING_EX(Connection, MechanicalVentilationConnection);

    static const std::vector<std::string>& GetValues()
    {
      ScopedMutex lock;
      if (_values.empty())
      {
        _values.push_back(Connection);
      }
      return _values;
    }
  protected:
    static std::vector<std::string> _values;
  };

  class MechanicalVentilationLink
  {
  public:
    DEFINE_STATIC_STRING_EX(ConnectionToAirway, MechanicalVentilationConnectionToAirway);

    static const std::vector<std::string>& GetValues()
    {
      ScopedMutex lock;
      if (_values.empty())
      {
        _values.push_back(ConnectionToAirway);
      }
      return _values;
    }
  protected:
    static std::vector<std::string> _values;
  };
  
  class MechanicalVentilatorCompartment
  {
  public:
    DEFINE_STATIC_STRING(MechanicalVentilator);
    DEFINE_STATIC_STRING_EX(ExpiratoryValve, MechanicalVentilatorExpiratoryValve);
    DEFINE_STATIC_STRING_EX(InspiratoryValve, MechanicalVentilatorInspiratoryValve);
    DEFINE_STATIC_STRING_EX(ExpiratoryLimb, MechanicalVentilatorExpiratoryLimb);
    DEFINE_STATIC_STRING_EX(InspiratoryLimb, MechanicalVentilatorInspiratoryLimb);
    DEFINE_STATIC_STRING_EX(YPiece, MechanicalVentilatorYPiece);
    DEFINE_STATIC_STRING_EX(Connection, MechanicalVentilatorConnection);

    static const std::vector<std::string>& GetValues()
    {
      ScopedMutex lock;
      if (_values.empty())
      {
        _values.push_back(MechanicalVentilator);
        _values.push_back(ExpiratoryValve);
        _values.push_back(InspiratoryValve);
        _values.push_back(ExpiratoryLimb);
        _values.push_back(InspiratoryLimb);
        _values.push_back(YPiece);
        _values.push_back(Connection);
      }
      return _values;
    }
  protected:
    static std::vector<std::string> _values;
  };

  class MechanicalVentilatorLink
  {
  public:
    DEFINE_STATIC_STRING(MechanicalVentilatorToExpiratoryValve);
    DEFINE_STATIC_STRING(MechanicalVentilatorToInspiratoryValve);
    DEFINE_STATIC_STRING_EX(ExpiratoryLimbToExpiratoryValve, MechanicalVentilatorExpiratoryLimbToExpiratoryValve);
    DEFINE_STATIC_STRING_EX(InspiratoryValveToInspiratoryLimb, MechanicalVentilatorInspiratoryValveToInspiratoryLimb);
    DEFINE_STATIC_STRING_EX(ExpiratoryLimbToYPiece, MechanicalVentilatorExpiratoryLimbToYPiece);
    DEFINE_STATIC_STRING_EX(InspiratoryLimbToYPiece, MechanicalVentilatorInspiratoryLimbToYPiece);
    DEFINE_STATIC_STRING_EX(YPieceToConnection, MechanicalVentilatorYPieceToConnection);
    DEFINE_STATIC_STRING_EX(ConnectionToEnvironment, MechanicalVentilatorConnectionToEnvironment);
    DEFINE_STATIC_STRING_EX(ConnectionToAirway, MechanicalVentilatorConnectionToAirway);

    static const std::vector<std::string>& GetValues()
    {
      ScopedMutex lock;
      if (_values.empty())
      {
        _values.push_back(MechanicalVentilatorToExpiratoryValve);
        _values.push_back(MechanicalVentilatorToInspiratoryValve);
        _values.push_back(ExpiratoryLimbToExpiratoryValve);
        _values.push_back(InspiratoryValveToInspiratoryLimb);
        _values.push_back(ExpiratoryLimbToYPiece);
        _values.push_back(InspiratoryLimbToYPiece);
        _values.push_back(YPieceToConnection);
        _values.push_back(ConnectionToEnvironment);
        _values.push_back(ConnectionToAirway);
      }
      return _values;
    }
  protected:
    static std::vector<std::string> _values;
  };

  class NasalCannulaCompartment
  {
  public:
    DEFINE_STATIC_STRING(NasalCannulaOxygenSource);
    DEFINE_STATIC_STRING(NasalCannula);

    static const std::vector<std::string>& GetValues()
    {
      ScopedMutex lock;
      if (_values.empty())
      {
        _values.push_back(NasalCannulaOxygenSource);
        _values.push_back(NasalCannula);
      }
      return _values;
    }
  protected:
    static std::vector<std::string> _values;
  };

  class NasalCannulaLink
  {
  public:
    DEFINE_STATIC_STRING(NasalCannulaOxygenInlet);
    DEFINE_STATIC_STRING(NasalCannulaSeal);
    DEFINE_STATIC_STRING(NasalCannulaToAirway);

    static const std::vector<std::string>& GetValues()
    {
      ScopedMutex lock;
      if (_values.empty())
      {
        _values.push_back(NasalCannulaOxygenInlet);
        _values.push_back(NasalCannulaSeal);
        _values.push_back(NasalCannulaToAirway);
      }
      return _values;
    }
  protected:
    static std::vector<std::string> _values;
  };

  class NonRebreatherMaskCompartment
  {
  public:
    DEFINE_STATIC_STRING(NonRebreatherMaskOxygenSource);
    DEFINE_STATIC_STRING(NonRebreatherMaskPorts);
    DEFINE_STATIC_STRING(NonRebreatherMaskBag);
    DEFINE_STATIC_STRING(NonRebreatherMask);

    static const std::vector<std::string>& GetValues()
    {
      ScopedMutex lock;
      if (_values.empty())
      {
        _values.push_back(NonRebreatherMaskOxygenSource);
        _values.push_back(NonRebreatherMaskPorts);
        _values.push_back(NonRebreatherMaskBag);
        _values.push_back(NonRebreatherMask);
      }
      return _values;
    }
  protected:
    static std::vector<std::string> _values;
  };

  class NonRebreatherMaskLink
  {
  public:
    DEFINE_STATIC_STRING(NonRebreatherMaskOxygenInlet);
    DEFINE_STATIC_STRING(NonRebreatherMaskReservoirValve);
    DEFINE_STATIC_STRING(NonRebreatherMaskSeal);
    DEFINE_STATIC_STRING(NonRebreatherMaskExhalationValves);
    DEFINE_STATIC_STRING(NonRebreatherMaskExhalation);
    DEFINE_STATIC_STRING(NonRebreatherMaskToAirway);

    static const std::vector<std::string>& GetValues()
    {
      ScopedMutex lock;
      if (_values.empty())
      {
        _values.push_back(NonRebreatherMaskOxygenInlet);
        _values.push_back(NonRebreatherMaskReservoirValve);
        _values.push_back(NonRebreatherMaskSeal);
        _values.push_back(NonRebreatherMaskExhalationValves);
        _values.push_back(NonRebreatherMaskExhalation);
        _values.push_back(NonRebreatherMaskToAirway);
      }
      return _values;
    }
  protected:
    static std::vector<std::string> _values;
  };

  class SimpleMaskCompartment
  {
  public:
    DEFINE_STATIC_STRING(SimpleMaskOxygenSource);
    DEFINE_STATIC_STRING(SimpleMask);

    static const std::vector<std::string>& GetValues()
    {
      ScopedMutex lock;
      if (_values.empty())
      {
        _values.push_back(SimpleMaskOxygenSource);
        _values.push_back(SimpleMask);
      }
      return _values;
    }
  protected:
    static std::vector<std::string> _values;
  };

  class SimpleMaskLink
  {
  public:
    DEFINE_STATIC_STRING(SimpleMaskOxygenInlet);
    DEFINE_STATIC_STRING(SimpleMaskSeal);
    DEFINE_STATIC_STRING(SimpleMaskPorts);
    DEFINE_STATIC_STRING(SimpleMaskToAirway);

    static const std::vector<std::string>& GetValues()
    {
      ScopedMutex lock;
      if (_values.empty())
      {
        _values.push_back(SimpleMaskOxygenInlet);
        _values.push_back(SimpleMaskSeal);
        _values.push_back(SimpleMaskPorts);
        _values.push_back(SimpleMaskToAirway);
      }
      return _values;
    }
  protected:
    static std::vector<std::string> _values;
  };
}
