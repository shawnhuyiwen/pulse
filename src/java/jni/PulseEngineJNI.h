/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include <jni.h>
#include <memory>
#include <iostream>
#include "PulsePhysiologyEngine.h"

class PulseEngineJNI : public PulseEngineThunk
{
public:
  PulseEngineJNI(const std::string& dataDir);
  ~PulseEngineJNI();

  void Reset();

  void ForwardDebug(const std::string& msg, const std::string& origin) override;
  void ForwardInfo(const std::string& msg, const std::string& origin) override;
  void ForwardWarning(const std::string& msg, const std::string& origin) override;
  void ForwardError(const std::string& msg, const std::string& origin) override;
  void ForwardFatal(const std::string& msg, const std::string& origin) override;

  JNIEnv*   jniEnv;
  jobject   jniObj;
  jmethodID jniDebugMethodID;
  jmethodID jniInfoMethodID;
  jmethodID jniWarnMethodID;
  jmethodID jniErrorMethodID;
  jmethodID jniFatalMethodID;
};
