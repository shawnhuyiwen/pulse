/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "PulseEngineJNI.h"
#include "PulseScenario.h"

#include "EngineTest.h"

  /////////////////////
  // TESTING SUPPORT //
  /////////////////////

extern "C"
JNIEXPORT jlong JNICALL Java_com_kitware_pulse_engine_testing_EngineUnitTestDriver_nativeAllocate(JNIEnv *env, jobject obj)
{
  PulseEngineTest *executor = new PulseEngineTest();
  return reinterpret_cast<jlong>(executor);
}

extern "C"
JNIEXPORT void JNICALL Java_com_kitware_pulse_engine_testing_EngineUnitTestDriver_nativeDelete(JNIEnv *env, jobject obj, jlong ptr)
{
  PulseEngineTest *executor = reinterpret_cast<PulseEngineTest*>(ptr);
  SAFE_DELETE(executor);
}

extern "C"
JNIEXPORT void JNICALL Java_com_kitware_pulse_engine_testing_EngineUnitTestDriver_nativeExecute(JNIEnv * env, jobject obj, jlong ptr, jstring test, jstring toDir)
{
  const char* testName = env->GetStringUTFChars(test, JNI_FALSE);
  const char* outputDir = env->GetStringUTFChars(toDir, JNI_FALSE);
  PulseEngineTest* executor = reinterpret_cast<PulseEngineTest*>(ptr);
  executor->RunTest(testName, outputDir);
  env->ReleaseStringUTFChars(test, testName);
  env->ReleaseStringUTFChars(toDir, outputDir);
}

  ////////////////////
  // ENGINE SUPPORT //
  ////////////////////

extern "C"
JNIEXPORT jlong JNICALL Java_com_kitware_pulse_engine_PulseEngine_nativeAllocate(JNIEnv *env, jobject obj, jstring dataDir)
{ 
  const char* dataDirStr = env->GetStringUTFChars(dataDir, JNI_FALSE);
  PulseEngineJNI *engineJNI = new PulseEngineJNI(dataDirStr);
  engineJNI->jniEnv = env;
  engineJNI->jniObj = obj;
  env->ReleaseStringUTFChars(dataDir, dataDirStr);
  
  return reinterpret_cast<jlong>(engineJNI);
}

extern "C"
JNIEXPORT void JNICALL Java_com_kitware_pulse_engine_PulseEngine_nativeDelete(JNIEnv *env, jobject obj, jlong ptr)
{
  PulseEngineJNI *engineJNI = reinterpret_cast<PulseEngineJNI*>(ptr);
  engineJNI->jniEnv = env;
  engineJNI->jniObj = obj;
  SAFE_DELETE(engineJNI);
}

//////////////////////
// SCENARIO SUPPORT //
//////////////////////

extern "C"
JNIEXPORT jboolean JNICALL Java_com_kitware_pulse_engine_PulseEngine_nativeExecuteScenario(JNIEnv * env, jobject obj, jlong ptr, jstring sceOpts, jint scenario_format)
{
  PulseEngineJNI* engineJNI = reinterpret_cast<PulseEngineJNI*>(ptr);
  engineJNI->jniEnv = env;
  engineJNI->jniObj = obj;

  jboolean bRet;
  const char* sceOptsStr = env->GetStringUTFChars(sceOpts, JNI_FALSE);
  bRet = engineJNI->ExecuteScenario(sceOptsStr, (SerializationFormat)scenario_format);
  env->ReleaseStringUTFChars(sceOpts, sceOptsStr);
  return bRet;
}

//////////////////
// START ENGINE //
//////////////////

extern "C"
JNIEXPORT jboolean JNICALL Java_com_kitware_pulse_engine_PulseEngine_nativeSerializeFromFile(JNIEnv *env, jobject obj, jlong ptr, jstring stateFilename, jstring dataRequests, jint dataRequestsFormat)
{
  PulseEngineJNI *engineJNI = reinterpret_cast<PulseEngineJNI*>(ptr);
  engineJNI->jniEnv = env;
  engineJNI->jniObj = obj;

  const char* fnStr = env->GetStringUTFChars(stateFilename, JNI_FALSE);
  const char* dmStr = nullptr;
  if(dataRequests != nullptr)
    dmStr = env->GetStringUTFChars(dataRequests, JNI_FALSE);
  jboolean bRet = engineJNI->SerializeFromFile(fnStr, dmStr, (SerializationFormat)dataRequestsFormat);

  env->ReleaseStringUTFChars(stateFilename, fnStr);
  if(dmStr != nullptr)
    env->ReleaseStringUTFChars(dataRequests, dmStr);
  return bRet;
}
extern "C"
JNIEXPORT jboolean JNICALL Java_com_kitware_pulse_engine_PulseEngine_nativeSerializeToFile(JNIEnv *env, jobject obj, jlong ptr, jstring stateFilename)
{
  jboolean bRet;
  PulseEngineJNI *engineJNI = reinterpret_cast<PulseEngineJNI*>(ptr);
  engineJNI->jniEnv = env; 
  engineJNI->jniObj = obj;

  const char* fnStr = env->GetStringUTFChars(stateFilename, JNI_FALSE);
  bRet = engineJNI->SerializeToFile(fnStr);

  env->ReleaseStringUTFChars(stateFilename, fnStr);
  return bRet;
}

extern "C"
JNIEXPORT jboolean JNICALL Java_com_kitware_pulse_engine_PulseEngine_nativeSerializeFromString(JNIEnv * env, jobject obj, jlong ptr, jstring state, jstring dataRequests, jint format)
{
  jboolean bRet;
  PulseEngineJNI* engineJNI = reinterpret_cast<PulseEngineJNI*>(ptr);
  engineJNI->jniEnv = env;
  engineJNI->jniObj = obj;

  const char* dmStr = nullptr;
  if (dataRequests != nullptr)
    dmStr = env->GetStringUTFChars(dataRequests, JNI_FALSE);\
  const char* sStr = env->GetStringUTFChars(state, JNI_FALSE);
  bRet = engineJNI->SerializeFromString(sStr, dmStr, (SerializationFormat)format);

  env->ReleaseStringUTFChars(state, sStr);
  if (dmStr != nullptr)
    env->ReleaseStringUTFChars(dataRequests, dmStr);
  return bRet;
}
extern "C"
JNIEXPORT jstring JNICALL Java_com_kitware_pulse_engine_PulseEngine_nativeSerializeToString(JNIEnv * env, jobject obj, jlong ptr, jint format)
{
  PulseEngineJNI* engineJNI = reinterpret_cast<PulseEngineJNI*>(ptr);
  engineJNI->jniEnv = env;
  engineJNI->jniObj = obj;

  std::string out = engineJNI->SerializeToString((SerializationFormat)format);
  jstring state = env->NewStringUTF(out.c_str());
  return state;
}

extern "C"
JNIEXPORT jboolean JNICALL Java_com_kitware_pulse_engine_PulseEngine_nativeInitializeEngine(JNIEnv *env, jobject obj, jlong ptr, jstring patient_configuration, jstring dataRequests, jint format)
{
  bool bRet;
  PulseEngineJNI *engineJNI = reinterpret_cast<PulseEngineJNI*>(ptr);
  engineJNI->jniEnv = env;
  engineJNI->jniObj = obj;

  if (patient_configuration == nullptr)
    return false;
  const char* pcStr = env->GetStringUTFChars(patient_configuration, JNI_FALSE);
    
  const char* drStr = nullptr;
  if (dataRequests != nullptr)
    drStr = env->GetStringUTFChars(dataRequests, JNI_FALSE);

  bRet = engineJNI->InitializeEngine(pcStr, drStr, (SerializationFormat)format);

  env->ReleaseStringUTFChars(patient_configuration, pcStr);
  if(drStr != nullptr)
    env->ReleaseStringUTFChars(dataRequests, drStr);

  return bRet;
}

extern "C"
JNIEXPORT jstring JNICALL Java_com_kitware_pulse_engine_PulseEngine_nativeGetInitialPatient(JNIEnv * env, jobject obj, jlong ptr, jint format)
{
  PulseEngineJNI* engineJNI = reinterpret_cast<PulseEngineJNI*>(ptr);
  engineJNI->jniEnv = env;
  engineJNI->jniObj = obj;

  std::string stream = engineJNI->GetInitialPatient((SerializationFormat)format);
  jstring patient = env->NewStringUTF(stream.c_str());
  return patient;
}

extern "C"
JNIEXPORT jstring JNICALL Java_com_kitware_pulse_engine_PulseEngine_nativeGetConditions(JNIEnv * env, jobject obj, jlong ptr, jint format)
{
  PulseEngineJNI* engineJNI = reinterpret_cast<PulseEngineJNI*>(ptr);
  engineJNI->jniEnv = env;
  engineJNI->jniObj = obj;

  std::string stream = engineJNI->GetConditions((SerializationFormat)format);
  jstring conditionManager = env->NewStringUTF(stream.c_str());
  return conditionManager;
}

extern "C"
JNIEXPORT jstring JNICALL Java_com_kitware_pulse_engine_PulseEngine_nativeGetAssessment(JNIEnv * env, jobject obj, jlong ptr, jint type, jint format)
{
  PulseEngineJNI* engineJNI = reinterpret_cast<PulseEngineJNI*>(ptr);
  engineJNI->jniEnv = env;
  engineJNI->jniObj = obj;

  std::string stream = engineJNI->GetPatientAssessment(type,(SerializationFormat)format);
  jstring assessment = env->NewStringUTF(stream.c_str());
  return assessment;
}

extern "C"
JNIEXPORT void JNICALL Java_com_kitware_pulse_engine_PulseEngine_nativeForwardLogMessages(JNIEnv * env, jobject obj, jlong ptr, jboolean b)
{
  PulseEngineJNI* engineJNI = reinterpret_cast<PulseEngineJNI*>(ptr);
  engineJNI->jniEnv = env;
  engineJNI->jniObj = obj;
  engineJNI->KeepLogMessages(b);
}
extern "C"
JNIEXPORT void JNICALL Java_com_kitware_pulse_engine_PulseEngine_nativeSetLogFilename(JNIEnv * env, jobject obj, jlong ptr, jstring logFilename)
{
  PulseEngineJNI* engineJNI = reinterpret_cast<PulseEngineJNI*>(ptr);
  engineJNI->jniEnv = env;
  engineJNI->jniObj = obj;
  const char* logF = env->GetStringUTFChars(logFilename, JNI_FALSE);
  engineJNI->SetLogFilename(logF);
  env->ReleaseStringUTFChars(logFilename, logF);
}

extern "C"
JNIEXPORT void JNICALL Java_com_kitware_pulse_engine_PulseEngine_nativeKeepEventChanges(JNIEnv * env, jobject obj, jlong ptr, jboolean b)
{
  PulseEngineJNI* engineJNI = reinterpret_cast<PulseEngineJNI*>(ptr);
  engineJNI->jniEnv = env;
  engineJNI->jniObj = obj;
  engineJNI->KeepEventChanges(b);
}
extern "C"
JNIEXPORT jstring JNICALL Java_com_kitware_pulse_engine_PulseEngine_nativePullEvents(JNIEnv * env, jobject obj, jlong ptr, jint format)
{
  PulseEngineJNI* engineJNI = reinterpret_cast<PulseEngineJNI*>(ptr);
  engineJNI->jniEnv = env;
  engineJNI->jniObj = obj;

  std::string out = engineJNI->PullEvents((SerializationFormat)format);
  jstring events = env->NewStringUTF(out.c_str());
  return events;
}
extern "C"
JNIEXPORT jstring JNICALL Java_com_kitware_pulse_engine_PulseEngine_nativePullActiveEvents(JNIEnv * env, jobject obj, jlong ptr, jint format)
{
  PulseEngineJNI* engineJNI = reinterpret_cast<PulseEngineJNI*>(ptr);
  engineJNI->jniEnv = env;
  engineJNI->jniObj = obj;

  std::string out = engineJNI->PullActiveEvents((SerializationFormat)format);
  jstring events = env->NewStringUTF(out.c_str());
  return events;
}

extern "C"
JNIEXPORT jboolean JNICALL Java_com_kitware_pulse_engine_PulseEngine_nativeProcessActions(JNIEnv * env, jobject obj, jlong ptr, jstring actions, jint format)
{
  if (actions == nullptr)
    return true;
  PulseEngineJNI* engineJNI = reinterpret_cast<PulseEngineJNI*>(ptr);
  engineJNI->jniEnv = env;
  engineJNI->jniObj = obj;

  const char* aStr = env->GetStringUTFChars(actions, JNI_FALSE);
  jboolean bRet = engineJNI->ProcessActions(aStr, (SerializationFormat)format);
  env->ReleaseStringUTFChars(actions, aStr);
  return bRet;
}

extern "C"
JNIEXPORT jstring JNICALL Java_com_kitware_pulse_engine_PulseEngine_nativePullActiveActions(JNIEnv * env, jobject obj, jlong ptr, jint format)
{
  PulseEngineJNI* engineJNI = reinterpret_cast<PulseEngineJNI*>(ptr);
  engineJNI->jniEnv = env;
  engineJNI->jniObj = obj;

  std::string stream = engineJNI->PullActiveActions((SerializationFormat)format);
  jstring actions = env->NewStringUTF(stream.c_str());
  return actions;
}

extern "C"
JNIEXPORT jdouble JNICALL Java_com_kitware_pulse_engine_PulseEngine_nativeGetTimeStep(JNIEnv * env, jobject obj, jlong ptr, jstring unit, jint format)
{
  PulseEngineJNI* engineJNI = reinterpret_cast<PulseEngineJNI*>(ptr);
  engineJNI->jniEnv = env;
  engineJNI->jniObj = obj;

  const char* uStr = "s";
  if(unit != nullptr)
    uStr = env->GetStringUTFChars(unit, JNI_FALSE);
  jdouble ts = engineJNI->GetTimeStep(uStr);
  env->ReleaseStringUTFChars(unit, uStr);
  return ts;
}

extern "C"
JNIEXPORT bool JNICALL Java_com_kitware_pulse_engine_PulseEngine_nativeAdvanceTimeStep(JNIEnv *env, jobject obj, jlong ptr)
{
  PulseEngineJNI *engineJNI = reinterpret_cast<PulseEngineJNI*>(ptr);
  engineJNI->jniEnv = env;
  engineJNI->jniObj = obj;

  return engineJNI->AdvanceTimeStep();
}

extern "C"
JNIEXPORT jdoubleArray JNICALL Java_com_kitware_pulse_engine_PulseEngine_nativePullData(JNIEnv * env, jobject obj, jlong ptr)
{
  PulseEngineJNI* engineJNI = reinterpret_cast<PulseEngineJNI*>(ptr);
  engineJNI->jniEnv = env;
  engineJNI->jniObj = obj;

  jdoubleArray jData;
  double* data = engineJNI->PullDataPtr();
  jsize len = (jsize)engineJNI->DataLength();
  jData = env->NewDoubleArray(len);
  env->SetDoubleArrayRegion(jData, 0, len, data);
  return jData;
}

PulseEngineJNI::PulseEngineJNI(const std::string& dataDir) : PulseEngineThunk(dataDir)
{
  Reset();
}

PulseEngineJNI::~PulseEngineJNI()
{
  Reset();
}

void PulseEngineJNI::Reset()
{
  jniEnv=nullptr;
  jniObj=nullptr;
  jniDebugMethodID=nullptr;
  jniInfoMethodID=nullptr;
  jniWarnMethodID=nullptr;
  jniErrorMethodID=nullptr;
  jniFatalMethodID=nullptr;
}

void PulseEngineJNI::ForwardDebug(const std::string& msg, const std::string& origin)
{
  if (jniEnv != nullptr && jniObj != nullptr)
  {
    jstring m = jniEnv->NewStringUTF(msg.c_str());
    jstring o = jniEnv->NewStringUTF(origin.c_str());
    if (jniDebugMethodID == nullptr)
      jniDebugMethodID = jniEnv->GetMethodID(jniEnv->GetObjectClass(jniObj), "LogDebug", "(Ljava/lang/String;Ljava/lang/String;)V");
    jniEnv->CallVoidMethod(jniObj, jniDebugMethodID, m, o);
  }
}

void PulseEngineJNI::ForwardInfo(const std::string& msg, const std::string& origin)
{
  if (jniEnv != nullptr && jniObj != nullptr)
  {
    jstring m = jniEnv->NewStringUTF(msg.c_str());
    jstring o = jniEnv->NewStringUTF(origin.c_str());
    if (jniInfoMethodID == nullptr)
      jniInfoMethodID = jniEnv->GetMethodID(jniEnv->GetObjectClass(jniObj), "LogInfo", "(Ljava/lang/String;Ljava/lang/String;)V");
    jniEnv->CallVoidMethod(jniObj, jniInfoMethodID, m, o);
  }
}

void PulseEngineJNI::ForwardWarning(const std::string& msg, const std::string& origin)
{
  if (jniEnv != nullptr && jniObj != nullptr)
  {
    jstring m = jniEnv->NewStringUTF(msg.c_str());
    jstring o = jniEnv->NewStringUTF(origin.c_str());
    if (jniWarnMethodID == nullptr)
      jniWarnMethodID = jniEnv->GetMethodID(jniEnv->GetObjectClass(jniObj), "LogWarning", "(Ljava/lang/String;Ljava/lang/String;)V");
    jniEnv->CallVoidMethod(jniObj, jniWarnMethodID, m, o);
  }
}

void PulseEngineJNI::ForwardError(const std::string& msg, const std::string& origin)
{
  if (jniEnv != nullptr && jniObj != nullptr)
  {
    jstring m = jniEnv->NewStringUTF(msg.c_str());
    jstring o = jniEnv->NewStringUTF(origin.c_str());
    if (jniErrorMethodID == nullptr)
      jniErrorMethodID = jniEnv->GetMethodID(jniEnv->GetObjectClass(jniObj), "LogError", "(Ljava/lang/String;Ljava/lang/String;)V");
    jniEnv->CallVoidMethod(jniObj, jniErrorMethodID, m, o);
  }
}

void PulseEngineJNI::ForwardFatal(const std::string& msg, const std::string& origin)
{
  if (jniEnv != nullptr && jniObj != nullptr)
  {
    jstring m = jniEnv->NewStringUTF(msg.c_str());
    jstring o = jniEnv->NewStringUTF(origin.c_str());
    if (jniFatalMethodID == nullptr)
      jniFatalMethodID = jniEnv->GetMethodID(jniEnv->GetObjectClass(jniObj), "LogFatal", "(Ljava/lang/String;Ljava/lang/String;)V");
    jniEnv->CallVoidMethod(jniObj, jniFatalMethodID, m, o);
  }
}
