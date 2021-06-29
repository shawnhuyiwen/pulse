/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "CommonDataModel.h"

#define ENGINE_TEST_DECL
#ifdef SHARED_ENGINE_TEST
  #if defined (__clang__)
    #define ENGINE_TEST_DECL
  #elif defined(__gnu_linux__)
    #define ENGINE_TEST_DECL __attribute__ ((visibility ("default")))
  #else
    #ifdef SHARED_ENGINE_TEST
      #define ENGINE_TEST_DECL __declspec(dllexport)
    #else
      #define ENGINE_TEST_DECL __declspec(dllimport)
   #endif
  #endif
#endif

class ENGINE_TEST_DECL SETestManager : public Loggable
{
public:
  SETestManager(Logger* logger = nullptr) : Loggable(logger) {};

  virtual ~SETestManager() = default;

  virtual bool RunTest(const std::string& testName, const std::string& sOutputDirectory) = 0;

protected:
  virtual void FillFunctionMap()=0;
  std::stringstream m_ss;
};
