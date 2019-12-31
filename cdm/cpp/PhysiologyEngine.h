/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

class SEAdvanceHandler;
class SEPatient;
class SEPatientConfiguration;
class SEPatientAssessment;

class SECondition;
class SEAction;

class SESubstanceManager;
class SEBloodChemistrySystem;
class SECardiovascularSystem;
class SEDrugSystem;
class SEEndocrineSystem;
class SEEnergySystem;
class SEGastrointestinalSystem;
class SEHepaticSystem;
class SENervousSystem;
class SERenalSystem;
class SERespiratorySystem;
class SETissueSystem;
class SEEnvironment;
class SEAnesthesiaMachine;
class SEInhaler;
class SEElectroCardioGram;
class SECompartmentManager;
class SEActionManager;
class SEConditionManager;
class SEEventManager;

class SEEngineTracker;
class SEEngineConfiguration;

/** 
 * @brief
 * Base exception class that all CDM classes throw when an error occurs
 */
struct PhysiologyEngineException : public CommonDataModelException
{
  PhysiologyEngineException()
    : CommonDataModelException( "Physiology Engine Error" ) {}

  PhysiologyEngineException(const std::string& _Message)
    : CommonDataModelException(_Message) {}
};

class CDM_DECL PhysiologyEngine
{
public:
  virtual ~PhysiologyEngine() {}

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Reset engine and set it to the state in the provided file.
  /// You may provided a Simulation Time to be used if desired.
  /// It will be reflected in the GetSimulationTime method.
  /// Return value indicates engine was able to load provided state file.
  /// Engine will be in a cleared state if this method fails.
  /// Note the provided configuration will overwrite any configuration options in the state with its contents (Use with caution!)
  //--------------------------------------------------------------------------------------------------
  virtual bool SerializeFromFile(const std::string& filename, SerializationFormat m, const SEScalarTime* simTime=nullptr, const SEEngineConfiguration* config = nullptr) = 0;

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Save the current state of the engine to provided filename.
  /// Engine will be in a cleared state if this method fails.
  //--------------------------------------------------------------------------------------------------
  virtual bool SerializeToFile(const std::string& filename, SerializationFormat m) const = 0;

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Reset engine and set it to the state in the provided string.
  /// The state is saved as bytes in the given string.
  /// Note that the bytes are binary, not text; we only use the string class as a convenient container.
  /// You may provided a Simulation Time to be used if desired.
  /// It will be reflected in the GetSimulationTime method.
  /// Return value indicates engine was able to load provided state file.
  /// Engine will be in a cleared state if this method fails.
  /// Note the provided configuration will overwrite any configuration options in the state with its contents (Use with caution!)
  //--------------------------------------------------------------------------------------------------
  virtual bool SerializeFromString(const std::string& state, SerializationFormat m, const SEScalarTime* simTime = nullptr, const SEEngineConfiguration* config = nullptr) = 0;

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Save the current state of the engine.
  /// The state is saved as bytes in the given string.
  /// Note that the bytes are binary, not text; we only use the string class as a convenient container.
  /// Engine will be in a cleared state if this method fails.
  //--------------------------------------------------------------------------------------------------
  virtual bool SerializeToString(std::string& state, SerializationFormat m) const = 0;

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// locates the json patient file and reads in the values. 
  ///
  /// This will create an engine that you can send instructions (patient,actions,conditions) to dynamically.
  /// The return value will indicate success failure of the creation of the engine.  
  /// Some combinations of patients and conditions may prevent the engine from stabilizing
  ///
  //--------------------------------------------------------------------------------------------------
  virtual bool InitializeEngine(const std::string& patient_configuration, SerializationFormat m, const SEEngineConfiguration* config = nullptr) = 0;

  //--------------------------------------------------------------------------------------------------
  /// \brief
  ///
  /// This will create an engine that you can send instructions (patient,actions,conditions) to dynamically.
  /// The return value will indicate success failure of the creation of the engine.  
  /// Some combinations of patients and conditions may prevent the engine from stabilizing
  ///
  //--------------------------------------------------------------------------------------------------
  virtual bool InitializeEngine(const SEPatientConfiguration& patient_configuration, const SEEngineConfiguration* config = nullptr) = 0;

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Get the Condition Manager.
  /// Allows a user to check the state of active conditions
  ///
  //--------------------------------------------------------------------------------------------------
  virtual const SEConditionManager& GetConditionManager() const = 0;

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Retrieve the Logger associated with this engine
  //--------------------------------------------------------------------------------------------------
  virtual Logger* GetLogger() const = 0;

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Retrieve the SEEngineTracker associated with tracking data from this engine to a file
  /// The SEEngineTracker is a class set up to pull data requested from the engine (via SEDataRequest's)
  /// and provide access to the data easily (ex. to easily write data to csv files)
  //--------------------------------------------------------------------------------------------------
  virtual SEEngineTracker* GetEngineTracker() const = 0;
  
  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// returns the engine configuration.   
  //--------------------------------------------------------------------------------------------------
  virtual const SEEngineConfiguration* GetConfiguration() const = 0;
  
  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// returns the engine time step that is used when advancing time. 
  ///
  //--------------------------------------------------------------------------------------------------
  virtual double GetTimeStep(const TimeUnit& unit) const = 0;

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// returns the current time of the simulation.   
  //--------------------------------------------------------------------------------------------------
  virtual double GetSimulationTime(const TimeUnit& unit) const = 0;

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// executes one pass through the time loop of the engine at the fixed timestep
  ///
  /// Events, errors, and warning as are logged to file not errors are returned
  /// through the API at this time. 
  ///
  //--------------------------------------------------------------------------------------------------
  virtual void AdvanceModelTime() = 0;

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// executes time loop of the engine beginning at the current time
  ///  and running for the duration specified in the call at the fixed timestep
  ///
  /// Events, errors, and warning as are logged to file not errors are returned
  /// through the API at this time.
  ///
  //--------------------------------------------------------------------------------------------------
  virtual void AdvanceModelTime(double time, const TimeUnit& unit) = 0;

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Add a callback object that will be called after each timestep
  //--------------------------------------------------------------------------------------------------
  virtual void SetAdvanceHandler(SEAdvanceHandler* handler) = 0;

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Execute the provided action.
  /// true will be returned if the engine supports the action
  /// false will be returned if the engine does not support the action
  ///
  //--------------------------------------------------------------------------------------------------
  virtual bool ProcessAction(const SEAction& action) = 0;

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Get the Action Manager.
  /// Allows a user to check the state of active actions
  ///
  //--------------------------------------------------------------------------------------------------
  virtual const SEActionManager& GetActionManager() const = 0;

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Retrieves the associated substance manager.
  ///
  //--------------------------------------------------------------------------------------------------
  virtual SESubstanceManager& GetSubstanceManager() = 0;
  virtual const SESubstanceManager& GetSubstanceManager() const = 0;

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Retrieves the associated event manager.
  ///
  //--------------------------------------------------------------------------------------------------
  virtual const SEEventManager& GetEventManager() const = 0;

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Returns the patient object used by the engine 
  ///
  //--------------------------------------------------------------------------------------------------
  virtual const SEPatient& GetPatient() const = 0;

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Determines the assessment type and fills the data object with current data. 
  ///
  /// Assessments can be queried at any point in the calculation and as many times are desired. 
  ///
  //--------------------------------------------------------------------------------------------------
  virtual bool GetPatientAssessment(SEPatientAssessment& assessment) const = 0;

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Returns the environment object used by the engine 
  ///
  //--------------------------------------------------------------------------------------------------
  virtual const SEEnvironment* GetEnvironment() const = 0;

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Returns the current state of the Blood Chemistry System
  ///
  //--------------------------------------------------------------------------------------------------
  virtual const SEBloodChemistrySystem* GetBloodChemistrySystem() const = 0;

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Returns the current state of the Cardiovascular System
  ///
  //--------------------------------------------------------------------------------------------------
  virtual const SECardiovascularSystem* GetCardiovascularSystem() const = 0;

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Returns the current state of the drug system  
  ///
  //--------------------------------------------------------------------------------------------------
  virtual const SEDrugSystem* GetDrugSystem() const = 0;

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Returns the current state of the Endocrine System
  ///
  //--------------------------------------------------------------------------------------------------
  virtual const SEEndocrineSystem* GetEndocrineSystem() const = 0;

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Returns the current state of the Energy System
  ///
  //--------------------------------------------------------------------------------------------------
  virtual const SEEnergySystem* GetEnergySystem() const = 0;

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Returns the current state of the Gastrointestinal System
  ///
  //--------------------------------------------------------------------------------------------------
  virtual const SEGastrointestinalSystem* GetGastrointestinalSystem() const = 0;

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Returns the current state of the Hepatic System
  ///
  //--------------------------------------------------------------------------------------------------
  virtual const SEHepaticSystem* GetHepaticSystem() const = 0;

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Returns the current state of the Nervous System
  ///
  //--------------------------------------------------------------------------------------------------
  virtual const SENervousSystem* GetNervousSystem() const = 0;

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Returns the current state of the Renal System
  ///
  //--------------------------------------------------------------------------------------------------
  virtual const SERenalSystem* GetRenalSystem() const = 0;

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Returns the current state of the Respiratory System
  ///
  //--------------------------------------------------------------------------------------------------
  virtual const SERespiratorySystem* GetRespiratorySystem() const = 0;

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Returns the current state of the tissue system  
  ///
  //--------------------------------------------------------------------------------------------------
  virtual const SETissueSystem* GetTissueSystem() const = 0;

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Returns the current state of the Anesthesia machine
  ///
  //--------------------------------------------------------------------------------------------------
  virtual const SEAnesthesiaMachine* GetAnesthesiaMachine() const = 0;

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Returns the current state of the Electrocardiogram machine
  ///
  //--------------------------------------------------------------------------------------------------
  virtual const SEElectroCardioGram* GetElectroCardioGram() const = 0;

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Returns the current state of the Inhaler
  ///
  //--------------------------------------------------------------------------------------------------
  virtual const SEInhaler* GetInhaler() const = 0;

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Retrieves the engine compartments, providing such data as:
  /// flows, pressure, volume as well as substance volumes and volume fractions.
  ///
  //--------------------------------------------------------------------------------------------------
  virtual const SECompartmentManager& GetCompartments() const = 0;
};
