/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

   // Testing creating and converting units
#include "CommonDataModelTest.h"

#include "cdm/system/equipment/electrocardiogram/SEElectroCardioGramWaveformInterpolator.h"
#include "cdm/utils/FileUtils.h"
#include "cdm/utils/TimingProfile.h"
#include "cdm/utils/testing/SETestReport.h"
#include "cdm/utils/testing/SETestSuite.h"
#include "cdm/utils/testing/SETestCase.h"

#include "cdm/properties/SEFunctionElectricPotentialVsTime.h"
#include "cdm/properties/SEScalarElectricPotential.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarTime.h"

#include "unsupported/Eigen/Splines"

std::vector<double> SplineInterpolation(std::vector<double>& input, int nOutput)
{
  // https://eigen.tuxfamily.org/dox/unsupported/structEigen_1_1SplineFitting.html
  const int nInput = (int)input.size();
  Eigen::MatrixXd points(2, nInput);
  for (int i = 0; i < nInput; ++i)
  {
    points(0, i) = (double)i / (nInput - 1);
    points(1, i) = input[i];
  }

  typedef Eigen::Spline<double, 1, 2> Spline1D;
  typedef Eigen::SplineFitting<Spline1D> SplineFitting1D;
  const auto fit = SplineFitting1D::Interpolate(points.row(1), 2, points.row(0));
  Spline1D spline(fit);

  std::vector<double> output;
  output.reserve(nOutput);
  for (int i = 0; i < nOutput; ++i)
  {
    auto point = spline((double)i / (nOutput - 1));
    output.push_back(point.coeffRef(0));
  }
  return output;
}

void CommonDataModelTest::WaveformInterpolatorTest(const std::string& sOutputDirectory)
{
  TimingProfile pTimer;
  std::string testName = "WaveformInterpolatorTest";
  Logger logger(sOutputDirectory + "/" + testName + "/"+ testName + ".log");

  SEElectroCardioGramWaveformInterpolator ecg(&logger);
  ecg.SerializeFromFile("./ecg/StandardECG.json");

  SEElectroCardioGramWaveform& sinus =
    ecg.GetWaveform(eElectroCardioGram_WaveformLead::Lead3, eElectroCardioGram_WaveformType::Sinus);
  sinus.GetData().WriteCSVFile(sOutputDirectory + "/" + testName + "/sinus.orig.csv");

  SEElectroCardioGramWaveform& vtach =
    ecg.GetWaveform(eElectroCardioGram_WaveformLead::Lead3, eElectroCardioGram_WaveformType::VentricularTachycardia);
  vtach.GetData().WriteCSVFile(sOutputDirectory + "/" + testName + "/vtach.orig.csv");

  SETestReport testReport(&logger);
  SETestSuite& testSuite = testReport.CreateTestSuite();
  testSuite.SetName(testName);

  SEScalarFrequency hr;

  // Downsample points from the original sinus input for hr = 185
  pTimer.Start("Case");
  hr.SetValue(185, FrequencyUnit::Per_min);
  SETestCase& testCase = testSuite.CreateTestCase();
  testCase.SetName("SINUS2HR185");
  double cycleTime_s = 1 / hr.GetValue(FrequencyUnit::Per_s);
  size_t numCyclePoints = (size_t)std::floor(cycleTime_s / 0.02 + 1);

  SEFunctionElectricPotentialVsTime sinus185downsampling;
  sinus185downsampling.SetTimeUnit(TimeUnit::s);
  sinus185downsampling.SetElectricPotentialUnit(ElectricPotentialUnit::mV);
  auto points = SplineInterpolation(sinus.GetData().GetElectricPotential(), (int)numCyclePoints);
  for (size_t i = 0; i < numCyclePoints; i++)
  {
    sinus185downsampling.GetTime().push_back(0.02 * i);
    sinus185downsampling.GetElectricPotential().push_back(points[i]);
  }
  sinus185downsampling.WriteCSVFile(sOutputDirectory + "/" + testName + "/sinus.185.downsampling.csv");
  testCase.GetDuration().SetValue(pTimer.GetElapsedTime_s("Case"), TimeUnit::s);

  // Interpolate waveform from downsampling points for hr = 60
  pTimer.Start("Case");
  hr.SetValue(60, FrequencyUnit::Per_min);
  SETestCase& testCase1 = testSuite.CreateTestCase();
  testCase1.SetName("SINUS2HR60");
  double cycleTime_s1 = 1 / hr.GetValue(FrequencyUnit::Per_s);
  size_t numCyclePoints1 = (size_t)std::floor(cycleTime_s1 / 0.02 + 1);

  SEFunctionElectricPotentialVsTime sinus60interpolate;
  sinus60interpolate.SetTimeUnit(TimeUnit::s);
  sinus60interpolate.SetElectricPotentialUnit(ElectricPotentialUnit::mV);
  auto points1 = SplineInterpolation(sinus185downsampling.GetElectricPotential(), (int)numCyclePoints1);
  for (size_t i = 0; i < numCyclePoints1; i++)
  {
    sinus60interpolate.GetTime().push_back(0.02 * i);
    sinus60interpolate.GetElectricPotential().push_back(points1[i]);
  }
  sinus60interpolate.WriteCSVFile(sOutputDirectory + "/" + testName + "/sinus.60.interpolate.csv");
  testCase1.GetDuration().SetValue(pTimer.GetElapsedTime_s("Case"), TimeUnit::s);

  // Downsample points from the original VTACH input for hr = 185
  pTimer.Start("Case");
  hr.SetValue(185, FrequencyUnit::Per_min);
  SETestCase& testCase2 = testSuite.CreateTestCase();
  testCase2.SetName("VTACH2HR185");
  double cycleTime_s2 = 1 / hr.GetValue(FrequencyUnit::Per_s);
  size_t numCyclePoints2 = (size_t)std::floor(cycleTime_s2 / 0.02 + 1);

  SEFunctionElectricPotentialVsTime vtach185downsampling;
  vtach185downsampling.SetTimeUnit(TimeUnit::s);
  vtach185downsampling.SetElectricPotentialUnit(ElectricPotentialUnit::mV);
  auto points2 = SplineInterpolation(vtach.GetData().GetElectricPotential(), (int)numCyclePoints2);
  for (size_t i = 0; i < numCyclePoints2; i++)
  {
    vtach185downsampling.GetTime().push_back(0.02 * i);
    vtach185downsampling.GetElectricPotential().push_back(points2[i]);
  }
  vtach185downsampling.WriteCSVFile(sOutputDirectory + "/" + testName + "/vtach.185.downsampling.csv");
  testCase2.GetDuration().SetValue(pTimer.GetElapsedTime_s("Case"), TimeUnit::s);

  // Interpolate waveform from downsampling points for hr = 60
  pTimer.Start("Case");
  hr.SetValue(60, FrequencyUnit::Per_min);
  SETestCase& testCase3 = testSuite.CreateTestCase();
  testCase3.SetName("VTACH2HR60");
  double cycleTime_s3 = 1 / hr.GetValue(FrequencyUnit::Per_s);
  size_t numCyclePoints3 = (size_t)std::floor(cycleTime_s3 / 0.02 + 1);

  SEFunctionElectricPotentialVsTime vtach60interpolate;
  vtach60interpolate.SetTimeUnit(TimeUnit::s);
  vtach60interpolate.SetElectricPotentialUnit(ElectricPotentialUnit::mV);
  auto points3 = SplineInterpolation(vtach185downsampling.GetElectricPotential(), (int)numCyclePoints3);
  for (size_t i = 0; i < numCyclePoints3; i++)
  {
    vtach60interpolate.GetTime().push_back(0.02 * i);
    vtach60interpolate.GetElectricPotential().push_back(points3[i]);
  }
  vtach60interpolate.WriteCSVFile(sOutputDirectory + "/" + testName + "/vtach.60.interpolate.csv");
  testCase3.GetDuration().SetValue(pTimer.GetElapsedTime_s("Case"), TimeUnit::s);
  // testCase.AddFailure("Unable to load patient " + *it);

  testReport.SerializeToFile(sOutputDirectory + "/" + testName + "Report.json");
}
