/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "PulseEngineC.h"
#include "patient/SEPatient.h"
#include "engine/SEDataRequest.h"
#include "engine/SEDataRequestManager.h"
#include "engine/SEActionManager.h"
#include "engine/SEConditionManager.h"
#include "engine/SEEngineTracker.h"
#include "properties/SEScalarTime.h"
#include "utils/DataTrack.h"

#define C_EXPORT __declspec(dllexport)
#define C_CALL __stdcall

extern "C"
C_EXPORT PulseEngineC* C_CALL PulseAllocate(const char* logFile, int len)
{ 
  std::string str(logFile);
  PulseEngineC *engineC = new PulseEngineC(str);
  return engineC;
}

extern "C"
C_EXPORT void C_CALL PulseDelete(PulseEngineC* ptr)
{
  SAFE_DELETE(ptr);
}

/*
extern "C"
C_EXPORT bool C_CALL PulseSerializeFromFile(JNIEnv *env, jobject obj, jlong ptr, jstring stateFilename, jdouble simTime_s, jstring dataRequests)
{
  PulseEngineJNI *engineJNI = reinterpret_cast<PulseEngineJNI*>(ptr);
  engineJNI->jniEnv = env;
  engineJNI->jniObj = obj;
  

  // Load up the data requests
  if (dataRequests != nullptr)
  {
    const char* drmStr = env->GetStringUTFChars(dataRequests, JNI_FALSE);
    if (!engineJNI->eng->GetEngineTracker()->GetDataRequestManager().SerializeFromString(drmStr, ASCII, engineJNI->eng->GetSubstanceManager()))
    {
      env->ReleaseStringUTFChars(dataRequests, drmStr);
      std::cerr << "Unable to load datarequest string" << std::endl;
      return false;
    }
    env->ReleaseStringUTFChars(dataRequests, drmStr);
  }

  // Ok, crank 'er up!
  const char* pStateFilename = env->GetStringUTFChars(stateFilename, JNI_FALSE);
  jboolean bRet;
  SEScalarTime simTime;
  if (simTime_s >= 0)
  {
    simTime.SetValue(simTime_s, TimeUnit::s);
    bRet = engineJNI->eng->SerializeFromFile(pStateFilename, ASCII, &simTime, nullptr);
  }
  else
  {
    std::cout << "Loading... " << std::endl;
    bRet = engineJNI->eng->SerializeFromFile(pStateFilename, ASCII);
  }  
  engineJNI->eng->SetEventHandler(engineJNI);

  env->ReleaseStringUTFChars(stateFilename, pStateFilename);
  return bRet;
}

extern "C"
C_EXPORT void C_CALL Java_com_kitware_physiology_pulse_engine_PulseEngine_nativeSerializeToFile(JNIEnv *env, jobject obj, jlong ptr, jstring stateFilename)
{
  PulseEngineJNI *engineJNI = reinterpret_cast<PulseEngineJNI*>(ptr);
  engineJNI->jniEnv = env; 
  engineJNI->jniObj = obj;
  const char* pStateFilename = env->GetStringUTFChars(stateFilename, JNI_FALSE);
  engineJNI->eng->SerializeToFile(pStateFilename,ASCII);
  env->ReleaseStringUTFChars(stateFilename, pStateFilename);
}

extern "C"
C_EXPORT jboolean C_CALL Java_com_kitware_physiology_pulse_engine_PulseEngine_nativeInitializeEngine(JNIEnv *env, jobject obj, jlong ptr, jstring patient, jstring conditions, jstring dataRequests)
{
  bool ret = false;
  
  
  const char* drStr = env->GetStringUTFChars(dataRequests, JNI_FALSE);
  try
  {
    PulseEngineJNI *engineJNI = reinterpret_cast<PulseEngineJNI*>(ptr);
    engineJNI->jniEnv = env;
    engineJNI->jniObj = obj;

    // Load up the patient
    const char* pStr = env->GetStringUTFChars(patient, JNI_FALSE);
    SEPatient p(engineJNI->eng->GetLogger());
    if (!p.SerializeFromString(pStr,ASCII))
    {
      env->ReleaseStringUTFChars(patient, pStr);
      std::cerr << "Unable to load patient string" << std::endl;
      return false;
    }
    env->ReleaseStringUTFChars(patient, pStr);

    // Load up the conditions
    std::vector<const SECondition*> c;
    if (conditions != nullptr)
    {
      const char* cStr = env->GetStringUTFChars(conditions, JNI_FALSE);
      SEConditionManager cMgr(engineJNI->eng->GetSubstanceManager());
      if (!cMgr.SerializeFromString(cStr, ASCII))
      {
        env->ReleaseStringUTFChars(dataRequests, cStr);
        return false;
      }
      env->ReleaseStringUTFChars(dataRequests, cStr);
      cMgr.GetAllConditions(c);
    }
      
    // Load up the data requests
    if (dataRequests != nullptr)
    {
      const char* drmStr = env->GetStringUTFChars(dataRequests, JNI_FALSE);
      if (!engineJNI->eng->GetEngineTracker()->GetDataRequestManager().SerializeFromString(drmStr, ASCII, engineJNI->eng->GetSubstanceManager()))
      {
        env->ReleaseStringUTFChars(dataRequests, drmStr);
        std::cerr << "Unable to load datarequest string" << std::endl;
        return false;
      }
      env->ReleaseStringUTFChars(dataRequests, drmStr);
    }

    // Ok, crank 'er up!
    ret = engineJNI->eng->InitializeEngine(p, &c);
    engineJNI->eng->SetEventHandler(engineJNI);
  }

  catch (std::exception& ex)
  {
    ret = false;
    std::cerr << "TODO Handle this Failure : " << ex.what() << std::endl;
  }
  
  return ret;
}

extern "C"
C_EXPORT bool C_CALL Java_com_kitware_physiology_pulse_engine_PulseEngine_nativeAdvanceTimeStep(JNIEnv *env, jobject obj, jlong ptr)
{
  bool success = true;
  PulseEngineJNI *engineJNI = reinterpret_cast<PulseEngineJNI*>(ptr);
  engineJNI->jniEnv = env;
  engineJNI->jniObj = obj;
  try
  {
    engineJNI->eng->AdvanceModelTime();
  }
  catch (CommonDataModelException& ex)
  {
    std::cerr << "JNI Caught Exception " << ex.what() << std::endl;
    success = false;
  }
  catch (std::exception& ex)
  {
    std::cerr << "JNI Caught Exception " << ex.what() << std::endl;
    success = false;
  }
  catch (...)
  {
    std::cerr << "JNI Caught Unknown Exception " << std::endl;
    success = false;
  }
  if (success)
  {
    double currentTime_s = engineJNI->eng->GetSimulationTime(TimeUnit::s);
    engineJNI->eng->GetEngineTracker()->TrackData(currentTime_s);
    engineJNI->PushData(currentTime_s);
  }
  return success;
}

extern "C"
C_EXPORT bool C_CALL Java_com_kitware_physiology_pulse_engine_PulseEngine_nativeAdvanceTime(JNIEnv *env, jobject obj, jlong ptr, jdouble time_s)
{
  bool success = true;
  PulseEngineJNI *engineJNI = reinterpret_cast<PulseEngineJNI*>(ptr);
  engineJNI->jniEnv = env;
  engineJNI->jniObj = obj;
  try
  {
    engineJNI->eng->AdvanceModelTime(time_s, TimeUnit::s);
  }
  catch (CommonDataModelException& ex)
  {
    std::cerr << "JNI Caught Exception " << ex.what() << std::endl;
    success = false;
  }
  catch (std::exception& ex)
  {
    std::cerr << "JNI Caught Exception " << ex.what() << std::endl;
    success = false;
  }
  catch (...)
  {
    std::cerr << "JNI Caught Unknown Exception " << std::endl;
    success = false;
  }
  if (success)
  {
    double currentTime_s = engineJNI->eng->GetSimulationTime(TimeUnit::s);
    engineJNI->eng->GetEngineTracker()->TrackData(currentTime_s);
    engineJNI->PushData(currentTime_s);
  }
  return success;
}

extern "C"
C_EXPORT bool C_CALL Java_com_kitware_physiology_pulse_engine_PulseEngine_nativeProcessActions(JNIEnv *env, jobject obj, jlong ptr, jstring actions)
{
  bool success = true;
  if (actions == nullptr)
    return success;
  PulseEngineJNI *engineJNI = reinterpret_cast<PulseEngineJNI*>(ptr);
  engineJNI->jniEnv = env;
  engineJNI->jniObj = obj;
  const char* aStr = env->GetStringUTFChars(actions, JNI_FALSE);

  try
  {
    std::vector<SEAction*> vActions;
    if (!SEActionManager::SerializeFromString(aStr, vActions, ASCII, engineJNI->eng->GetSubstanceManager()))
    {
      env->ReleaseStringUTFChars(actions, aStr);
      return false;
    }
    env->ReleaseStringUTFChars(actions, aStr);

    for (const SEAction* a : vActions)
    {
      if (!engineJNI->eng->ProcessAction(*a))
        success = false;
      delete a;
    }
  }
  catch (CommonDataModelException& ex)
  {
    success = false;
    engineJNI->eng->GetLogger()->Error(ex.what());
  }
  catch (std::exception& ex)
  {
    success = false;
    engineJNI->eng->GetLogger()->Error(ex.what());
  }
  catch (...)
  {
    success = false;
  }

  return success;
}
*/

PulseEngineC::PulseEngineC(const std::string& logFile) : SEEventHandler()
{// No logger needed for the event handler, at this point
  eng = std::unique_ptr<PulseEngine>((PulseEngine*)CreatePulseEngine(logFile).release());
  eng->GetLogger()->SetForward(this);
  eng->GetLogger()->LogToConsole(false);
  trk=&eng->GetEngineTracker()->GetDataTrack();
}

PulseEngineC::~PulseEngineC()
{
  
}

/*
void PulseEngineC::OnAdvance(double time_s, const PhysiologyEngine& engine)
{
  if (update_cnt++ > updateFrequency_cnt)
  {    
    PushData(time_s);
    update_cnt = 0;
  }
}
void PulseEngineJNI::PushData(double time_s)
{
  if (jniEnv != nullptr && jniObj != nullptr)
  {
    jmethodID m;
    std::vector<std::string>& headings = trk->GetHeadings();

    if (firstUpdate)
    {
      firstUpdate = false;
      jobjectArray sary = jniEnv->NewObjectArray(headings.size(), jniEnv->FindClass("java/lang/String"), jniEnv->NewStringUTF(""));
      for (unsigned int i = 0; i < headings.size(); i++)
        jniEnv->SetObjectArrayElement(sary, i, jniEnv->NewStringUTF(headings[i].c_str()));
      m = jniEnv->GetMethodID(jniEnv->GetObjectClass(jniObj), "setCDMHeadings", "([Ljava/lang/String;)V");
      if (m == nullptr)
        std::cerr << "Can't find setCDMHeadings method in Java" << std::endl;
      jniEnv->CallVoidMethod(jniObj, m, sary);
    }

    // Gather up the requested data into an array and pass it over to java
    // The order is set in the header order
    jdoubleArray ary = jniEnv->NewDoubleArray(trk->GetProbes()->size());

    jboolean isCopy = JNI_FALSE;
    jdouble* reqData = jniEnv->GetDoubleArrayElements(ary, &isCopy);
    for (unsigned int i = 0; i < headings.size(); i++)
      reqData[i] = trk->GetProbe(headings[i]);
    if (isCopy == JNI_TRUE)
      jniEnv->ReleaseDoubleArrayElements(ary, reqData, JNI_COMMIT);

    m = jniEnv->GetMethodID(jniEnv->GetObjectClass(jniObj), "updateCDM", "(D[D)V");
    if (m == nullptr)
      std::cerr << "Can't find updateCDM method in Java" << std::endl;
    jniEnv->CallVoidMethod(jniObj, m, time_s, ary);
  }
}
*/

void PulseEngineC::ForwardDebug(const std::string&  msg, const std::string&  origin)
{
  
}

void PulseEngineC::ForwardInfo(const std::string&  msg, const std::string&  origin)
{
 
}

void PulseEngineC::ForwardWarning(const std::string&  msg, const std::string&  origin)
{
  
}

void PulseEngineC::ForwardError(const std::string&  msg, const std::string&  origin)
{
  
}

void PulseEngineC::ForwardFatal(const std::string&  msg, const std::string&  origin)
{
  
}

void PulseEngineC::HandlePatientEvent(ePatient_Event type, bool active, const SEScalarTime* time)
{
  
}
void PulseEngineC::HandleAnesthesiaMachineEvent(eAnesthesiaMachine_Event type, bool active, const SEScalarTime* time)
{
  
}
