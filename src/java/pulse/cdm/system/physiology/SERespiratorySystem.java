/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.cdm.system.physiology;

import pulse.cdm.bind.Physiology.RespiratorySystemData;

import pulse.cdm.properties.*;
import pulse.cdm.system.SESystem;

public class SERespiratorySystem extends SEPhysiologySystem implements SESystem
{
  protected SEScalarPressure              alveolarArterialGradient;
  protected SEScalarVolume                alveolarDeadSpace;
  protected SEScalarVolume                anatomicDeadSpace;
  protected SEScalarPressure              carricoIndex;
  protected SEScalarVolumePerPressure     chestWallCompliance;
  protected SEScalarEnergy                elasticWorkOfBreathing;
  protected SEScalar0To1                  endTidalCarbonDioxideFraction;
  protected SEScalarPressure              endTidalCarbonDioxidePressure;
  protected SEScalar0To1                  endTidalOxygenFraction;
  protected SEScalarPressure              endTidalOxygenPressure;
  protected SEScalarVolumePerTime         expiratoryFlow;
  protected SEScalarPressureTimePerVolume expiratoryPulmonaryResistance;
  protected SEScalarPower                 imposedPowerOfBreathing;
  protected SEScalarEnergy                imposedWorkOfBreathing;
  protected SEScalar                      inspiratoryExpiratoryRatio;
  protected SEScalarVolumePerTime         inspiratoryFlow;
  protected SEScalarPressureTimePerVolume inspiratoryPulmonaryResistance;
  protected SEScalarPressure              intrapleuralPressure;
  protected SEScalarPressure              intrapulmonaryPressure;
  protected SEScalarVolumePerPressure     lungCompliance;
  protected SEScalarPressure              maximalInspiratoryPressure;
  protected SEScalarPower                 patientPowerOfBreathing;
  protected SEScalarEnergy                patientWorkOfBreathing;
  protected SEScalarPressure              peakInspiratoryPressure;
  protected SEScalarVolume                physiologicDeadSpace;
  protected SEScalarPressure              positiveEndExpiratoryPressure;
  protected SEScalarVolumePerPressure     pulmonaryCompliance;
  protected SEScalarPressurePerVolume     pulmonaryElastance;
  protected SEScalarEnergy                resistiveExpiratoryWorkOfBreathing;
  protected SEScalarEnergy                resistiveInspiratoryWorkOfBreathing;
  protected SEScalarFrequency             respirationRate;
  protected SEScalar0To1                  respiratoryMuscleFatigue;
  protected SEScalarPressure              respiratoryMusclePressure;
  protected SEScalar                      specificVentilation;
  protected SEScalarVolume                tidalVolume;
  protected SEScalarVolumePerTime         totalAlveolarVentilation;
  protected SEScalarVolumePerTime         totalDeadSpaceVentilation;
  protected SEScalarVolume                totalLungVolume;
  protected SEScalarPower                 totalPowerOfBreathing;
  protected SEScalarVolumePerTime         totalPulmonaryVentilation;
  protected SEScalarEnergy                totalWorkOfBreathing;
  protected SEScalarPressure              transairwayPressure;
  protected SEScalarPressure              transalveolarPressure;
  protected SEScalarPressure              transChestWallPressure;
  protected SEScalarPressure              transMusclePressure;
  protected SEScalarPressure              transpulmonaryPressure;
  protected SEScalarPressure              transrespiratoryPressure;
  protected SEScalarPressure              transthoracicPressure;


  public SERespiratorySystem()
  {    
    alveolarArterialGradient = null;
    alveolarDeadSpace = null;
    anatomicDeadSpace = null;
    carricoIndex = null;
    chestWallCompliance = null;
    elasticWorkOfBreathing = null;
    endTidalCarbonDioxideFraction = null;
    endTidalCarbonDioxidePressure = null;
    endTidalOxygenFraction = null;
    endTidalOxygenPressure = null;
    expiratoryFlow = null;
    expiratoryPulmonaryResistance = null;
    imposedPowerOfBreathing = null;
    imposedWorkOfBreathing = null;
    inspiratoryExpiratoryRatio = null;
    inspiratoryFlow = null;
    inspiratoryPulmonaryResistance = null;
    intrapleuralPressure = null;
    intrapulmonaryPressure = null;
    lungCompliance = null;
    maximalInspiratoryPressure = null;
    patientPowerOfBreathing = null;
    patientWorkOfBreathing = null;
    peakInspiratoryPressure = null;
    physiologicDeadSpace = null;
    positiveEndExpiratoryPressure = null;
    pulmonaryCompliance = null;
    pulmonaryElastance = null;
    resistiveExpiratoryWorkOfBreathing = null;
    resistiveInspiratoryWorkOfBreathing = null;
    respirationRate = null;
    respiratoryMuscleFatigue = null;
    respiratoryMusclePressure = null;
    specificVentilation = null;
    tidalVolume = null;
    totalAlveolarVentilation = null;
    totalDeadSpaceVentilation = null;
    totalLungVolume = null;
    totalPowerOfBreathing = null;
    totalPulmonaryVentilation = null;
    totalWorkOfBreathing = null;
    transairwayPressure = null;
    transalveolarPressure = null;
    transChestWallPressure = null;
    transMusclePressure = null;
    transpulmonaryPressure = null;
    transrespiratoryPressure = null;
    transthoracicPressure = null;
  }

  public void reset()
  {
    if (alveolarArterialGradient != null)
      alveolarArterialGradient.invalidate();
    if (alveolarDeadSpace != null)
      alveolarDeadSpace.invalidate();
    if (anatomicDeadSpace != null)
      anatomicDeadSpace.invalidate();
    if (carricoIndex != null)
      carricoIndex.invalidate();
    if (chestWallCompliance != null)
      chestWallCompliance.invalidate();
    if (elasticWorkOfBreathing != null)
      elasticWorkOfBreathing.invalidate();
    if (endTidalCarbonDioxideFraction != null)
      endTidalCarbonDioxideFraction.invalidate();
    if (endTidalCarbonDioxidePressure != null)
      endTidalCarbonDioxidePressure.invalidate();
    if (endTidalOxygenFraction != null)
      endTidalOxygenFraction.invalidate();
    if (endTidalOxygenPressure != null)
      endTidalOxygenPressure.invalidate();
    if (expiratoryFlow != null)
      expiratoryFlow.invalidate();
    if (expiratoryPulmonaryResistance != null)
      expiratoryPulmonaryResistance.invalidate();
    if (imposedPowerOfBreathing != null)
      imposedPowerOfBreathing.invalidate();
    if (imposedWorkOfBreathing != null)
      imposedWorkOfBreathing.invalidate();
    if (inspiratoryExpiratoryRatio != null)
      inspiratoryExpiratoryRatio.invalidate();
    if (inspiratoryFlow != null)
      inspiratoryFlow.invalidate();
    if (inspiratoryPulmonaryResistance != null)
      inspiratoryPulmonaryResistance.invalidate();
    if (intrapleuralPressure != null)
      intrapleuralPressure.invalidate();
    if (intrapulmonaryPressure != null)
      intrapulmonaryPressure.invalidate();
    if (lungCompliance != null)
      lungCompliance.invalidate();
    if (maximalInspiratoryPressure != null)
      maximalInspiratoryPressure.invalidate();
    if (patientPowerOfBreathing != null)
      patientPowerOfBreathing.invalidate();
    if (patientWorkOfBreathing != null)
      patientWorkOfBreathing.invalidate();
    if (peakInspiratoryPressure != null)
      peakInspiratoryPressure.invalidate();
    if (physiologicDeadSpace != null)
      physiologicDeadSpace.invalidate();
    if (positiveEndExpiratoryPressure != null)
      positiveEndExpiratoryPressure.invalidate();
    if (pulmonaryCompliance != null)
      pulmonaryCompliance.invalidate();
    if (pulmonaryElastance != null)
      pulmonaryElastance.invalidate();
    if (resistiveExpiratoryWorkOfBreathing != null)
      resistiveExpiratoryWorkOfBreathing.invalidate();
    if (resistiveInspiratoryWorkOfBreathing != null)
      resistiveInspiratoryWorkOfBreathing.invalidate();
    if (respirationRate != null)
      respirationRate.invalidate();
    if (respiratoryMuscleFatigue != null)
      respiratoryMuscleFatigue.invalidate();
    if (respiratoryMusclePressure != null)
      respiratoryMusclePressure.invalidate();
    if (specificVentilation != null)
      specificVentilation.invalidate();
    if (tidalVolume != null)
      tidalVolume.invalidate();
    if (totalAlveolarVentilation != null)
      totalAlveolarVentilation.invalidate();
    if (totalDeadSpaceVentilation != null)
      totalDeadSpaceVentilation.invalidate();
    if (totalLungVolume != null)
      totalLungVolume.invalidate();
    if (totalPowerOfBreathing != null)
      totalPowerOfBreathing.invalidate();
    if (totalPulmonaryVentilation != null)
      totalPulmonaryVentilation.invalidate();
    if (totalWorkOfBreathing != null)
      totalWorkOfBreathing.invalidate();
    if (transairwayPressure != null)
      transairwayPressure.invalidate();
    if (transalveolarPressure != null)
      transalveolarPressure.invalidate();
    if (transChestWallPressure != null)
      transChestWallPressure.invalidate();
    if (transMusclePressure != null)
      transMusclePressure.invalidate();
    if (transpulmonaryPressure != null)
      transpulmonaryPressure.invalidate();
    if (transrespiratoryPressure != null)
      transrespiratoryPressure.invalidate();
    if (transthoracicPressure != null)
      transthoracicPressure.invalidate();
  }

  public static void load(RespiratorySystemData src, SERespiratorySystem dst)
  {
    if (src.hasAlveolarArterialGradient())
      SEScalarPressure.load(src.getAlveolarArterialGradient(),dst.getAlveolarArterialGradient());
    if (src.hasAlveolarDeadSpace())
      SEScalarVolume.load(src.getAlveolarDeadSpace(),dst.getAlveolarDeadSpace());
    if (src.hasAnatomicDeadSpace())
      SEScalarVolume.load(src.getAnatomicDeadSpace(),dst.getAnatomicDeadSpace());
    if (src.hasCarricoIndex())
      SEScalarPressure.load(src.getCarricoIndex(),dst.getCarricoIndex());
    if (src.hasChestWallCompliance())
      SEScalarVolumePerPressure.load(src.getChestWallCompliance(),dst.getChestWallCompliance());
    if (src.hasElasticWorkOfBreathing())
      SEScalarEnergy.load(src.getElasticWorkOfBreathing(),dst.getElasticWorkOfBreathing());
    if (src.hasEndTidalCarbonDioxideFraction())
      SEScalar0To1.load(src.getEndTidalCarbonDioxideFraction(),dst.getEndTidalCarbonDioxideFraction());
    if (src.hasEndTidalCarbonDioxidePressure())
      SEScalarPressure.load(src.getEndTidalCarbonDioxidePressure(),dst.getEndTidalCarbonDioxidePressure());
    if (src.hasEndTidalOxygenFraction())
      SEScalar0To1.load(src.getEndTidalOxygenFraction(),dst.getEndTidalOxygenFraction());
    if (src.hasEndTidalOxygenPressure())
      SEScalarPressure.load(src.getEndTidalOxygenPressure(),dst.getEndTidalOxygenPressure());
    if (src.hasExpiratoryFlow())
      SEScalarVolumePerTime.load(src.getExpiratoryFlow(),dst.getExpiratoryFlow());
    if (src.hasExpiratoryPulmonaryResistance())
      SEScalarPressureTimePerVolume.load(src.getExpiratoryPulmonaryResistance(),dst.getExpiratoryPulmonaryResistance()); 
    if (src.hasImposedPowerOfBreathing())
      SEScalarPower.load(src.getImposedPowerOfBreathing(),dst.getImposedPowerOfBreathing()); 
    if (src.hasImposedWorkOfBreathing())
      SEScalarEnergy.load(src.getImposedWorkOfBreathing(),dst.getImposedWorkOfBreathing()); 
    if (src.hasInspiratoryExpiratoryRatio())
      SEScalar.load(src.getInspiratoryExpiratoryRatio(),dst.getInspiratoryExpiratoryRatio()); 
    if (src.hasInspiratoryFlow())
      SEScalarVolumePerTime.load(src.getInspiratoryFlow(),dst.getInspiratoryFlow()); 
    if (src.hasInspiratoryPulmonaryResistance())
      SEScalarPressureTimePerVolume.load(src.getInspiratoryPulmonaryResistance(),dst.getInspiratoryPulmonaryResistance()); 
    if (src.hasIntrapleuralPressure())
      SEScalarPressure.load(src.getIntrapleuralPressure(),dst.getIntrapleuralPressure());
    if (src.hasIntrapulmonaryPressure())
      SEScalarPressure.load(src.getIntrapulmonaryPressure(),dst.getIntrapulmonaryPressure()); 
    if (src.hasLungCompliance())
      SEScalarVolumePerPressure.load(src.getLungCompliance(),dst.getLungCompliance()); 
    if (src.hasMaximalInspiratoryPressure())
      SEScalarPressure.load(src.getMaximalInspiratoryPressure(),dst.getMaximalInspiratoryPressure()); 
    if (src.hasPatientPowerOfBreathing())
      SEScalarPower.load(src.getPatientPowerOfBreathing(),dst.getPatientPowerOfBreathing()); 
    if (src.hasPatientWorkOfBreathing())
      SEScalarEnergy.load(src.getPatientWorkOfBreathing(),dst.getPatientWorkOfBreathing()); 
    if (src.hasPeakInspiratoryPressure())
      SEScalarPressure.load(src.getPeakInspiratoryPressure(),dst.getPeakInspiratoryPressure()); 
    if (src.hasPhysiologicDeadSpace())
      SEScalarVolume.load(src.getPhysiologicDeadSpace(),dst.getPhysiologicDeadSpace()); 
    if (src.hasPositiveEndExpiratoryPressure())
      SEScalarPressure.load(src.getPositiveEndExpiratoryPressure(),dst.getPositiveEndExpiratoryPressure()); 
    if (src.hasPulmonaryCompliance())
      SEScalarVolumePerPressure.load(src.getPulmonaryCompliance(),dst.getPulmonaryCompliance()); 
    if (src.hasPulmonaryElastance())
      SEScalarPressurePerVolume.load(src.getPulmonaryElastance(),dst.getPulmonaryElastance()); 
    if (src.hasResistiveExpiratoryWorkOfBreathing())
      SEScalarEnergy.load(src.getResistiveExpiratoryWorkOfBreathing(),dst.getResistiveExpiratoryWorkOfBreathing()); 
    if (src.hasResistiveInspiratoryWorkOfBreathing())
      SEScalarEnergy.load(src.getResistiveInspiratoryWorkOfBreathing(),dst.getResistiveInspiratoryWorkOfBreathing()); 
    if (src.hasRespirationRate())
      SEScalarFrequency.load(src.getRespirationRate(),dst.getRespirationRate());
    if (src.hasRespiratoryMuscleFatigue())
      SEScalar0To1.load(src.getRespiratoryMuscleFatigue(),dst.getRespiratoryMuscleFatigue());
    if (src.hasRespiratoryMusclePressure())
      SEScalarPressure.load(src.getRespiratoryMusclePressure(),dst.getRespiratoryMusclePressure());
    if (src.hasSpecificVentilation())
      SEScalar.load(src.getSpecificVentilation(),dst.getSpecificVentilation());
    if (src.hasTidalVolume())
      SEScalarVolume.load(src.getTidalVolume(),dst.getTidalVolume()); 
    if (src.hasTotalAlveolarVentilation())
      SEScalarVolumePerTime.load(src.getTotalAlveolarVentilation(),dst.getTotalAlveolarVentilation());
    if (src.hasTotalDeadSpaceVentilation())
      SEScalarVolumePerTime.load(src.getTotalDeadSpaceVentilation(),dst.getTotalDeadSpaceVentilation());
    if (src.hasTotalLungVolume())
      SEScalarVolume.load(src.getTotalLungVolume(),dst.getTotalLungVolume());
    if (src.hasTotalPowerOfBreathing())
      SEScalarPower.load(src.getTotalPowerOfBreathing(),dst.getTotalPowerOfBreathing());
    if (src.hasTotalPulmonaryVentilation())
      SEScalarVolumePerTime.load(src.getTotalPulmonaryVentilation(),dst.getTotalPulmonaryVentilation());
    if (src.hasTotalWorkOfBreathing())
      SEScalarEnergy.load(src.getTotalWorkOfBreathing(),dst.getTotalWorkOfBreathing());
    if (src.hasTransairwayPressure())
      SEScalarPressure.load(src.getTransairwayPressure(),dst.getTransairwayPressure());
    if (src.hasTransalveolarPressure())
      SEScalarPressure.load(src.getTransalveolarPressure(),dst.getTransalveolarPressure());
    if (src.hasTransChestWallPressure())
      SEScalarPressure.load(src.getTransChestWallPressure(),dst.getTransChestWallPressure());
    if (src.hasTransMusclePressure())
      SEScalarPressure.load(src.getTransMusclePressure(),dst.getTransMusclePressure());
    if (src.hasTranspulmonaryPressure())
      SEScalarPressure.load(src.getTranspulmonaryPressure(),dst.getTranspulmonaryPressure());
    if (src.hasTransrespiratoryPressure())
      SEScalarPressure.load(src.getTransrespiratoryPressure(),dst.getTransrespiratoryPressure());
    if (src.hasTransthoracicPressure())
      SEScalarPressure.load(src.getTransthoracicPressure(),dst.getTransthoracicPressure());
  }

  public static RespiratorySystemData unload(SERespiratorySystem src)
  {
    RespiratorySystemData.Builder dst = RespiratorySystemData.newBuilder();
    unload(src,dst);
    return dst.build();
  }

  protected static void unload(SERespiratorySystem src, RespiratorySystemData.Builder dst)
  {
    if (src.hasAlveolarArterialGradient())
      dst.setAlveolarArterialGradient(SEScalarPressure.unload(src.getAlveolarArterialGradient()));
    if (src.hasAlveolarDeadSpace())
      dst.setAlveolarDeadSpace(SEScalarVolume.unload(src.getAlveolarDeadSpace()));
    if (src.hasAnatomicDeadSpace())
      dst.setAnatomicDeadSpace(SEScalarVolume.unload(src.getAnatomicDeadSpace()));
    if (src.hasCarricoIndex())
      dst.setCarricoIndex(SEScalarPressure.unload(src.getCarricoIndex()));
    if (src.hasChestWallCompliance())
      dst.setChestWallCompliance(SEScalarVolumePerPressure.unload(src.getChestWallCompliance()));
    if (src.hasElasticWorkOfBreathing())
      dst.setElasticWorkOfBreathing(SEScalarEnergy.unload(src.getElasticWorkOfBreathing()));
    if (src.hasEndTidalCarbonDioxideFraction())
      dst.setEndTidalCarbonDioxideFraction(SEScalar0To1.unload(src.getEndTidalCarbonDioxideFraction()));  
    if (src.hasEndTidalCarbonDioxidePressure())
      dst.setEndTidalCarbonDioxidePressure(SEScalarPressure.unload(src.getEndTidalCarbonDioxidePressure()));
    if (src.hasEndTidalOxygenFraction())
      dst.setEndTidalOxygenFraction(SEScalar0To1.unload(src.getEndTidalOxygenFraction()));  
    if (src.hasEndTidalOxygenPressure())
      dst.setEndTidalOxygenPressure(SEScalarPressure.unload(src.getEndTidalOxygenPressure())); 
    if (src.hasExpiratoryFlow())
      dst.setExpiratoryFlow(SEScalarVolumePerTime.unload(src.getExpiratoryFlow()));
    if (src.hasExpiratoryPulmonaryResistance())
      dst.setExpiratoryPulmonaryResistance(SEScalarPressureTimePerVolume.unload(src.getExpiratoryPulmonaryResistance()));
    if (src.hasImposedPowerOfBreathing())
      dst.setImposedPowerOfBreathing(SEScalarPower.unload(src.getImposedPowerOfBreathing()));
    if (src.hasImposedWorkOfBreathing())
      dst.setImposedWorkOfBreathing(SEScalarEnergy.unload(src.getImposedWorkOfBreathing()));
    if (src.hasInspiratoryExpiratoryRatio())
      dst.setInspiratoryExpiratoryRatio(SEScalar.unload(src.getInspiratoryExpiratoryRatio()));
    if (src.hasInspiratoryFlow())
      dst.setInspiratoryFlow(SEScalarVolumePerTime.unload(src.getInspiratoryFlow()));
    if (src.hasInspiratoryPulmonaryResistance())
      dst.setInspiratoryPulmonaryResistance(SEScalarPressureTimePerVolume.unload(src.getInspiratoryPulmonaryResistance()));
    if (src.hasIntrapleuralPressure())
      dst.setIntrapleuralPressure(SEScalarPressure.unload(src.getIntrapleuralPressure()));
    if (src.hasIntrapulmonaryPressure())
      dst.setIntrapulmonaryPressure(SEScalarPressure.unload(src.getIntrapulmonaryPressure()));
    if (src.hasLungCompliance())
      dst.setLungCompliance(SEScalarVolumePerPressure.unload(src.getLungCompliance()));
    if (src.hasMaximalInspiratoryPressure())
      dst.setMaximalInspiratoryPressure(SEScalarPressure.unload(src.getMaximalInspiratoryPressure()));
    if (src.hasPatientPowerOfBreathing())
      dst.setPatientPowerOfBreathing(SEScalarPower.unload(src.getPatientPowerOfBreathing()));
    if (src.hasPatientWorkOfBreathing())
      dst.setPatientWorkOfBreathing(SEScalarEnergy.unload(src.getPatientWorkOfBreathing()));
    if (src.hasPeakInspiratoryPressure())
      dst.setPeakInspiratoryPressure(SEScalarPressure.unload(src.getPeakInspiratoryPressure()));
    if (src.hasPhysiologicDeadSpace())
      dst.setPhysiologicDeadSpace(SEScalarVolume.unload(src.getPhysiologicDeadSpace()));
    if (src.hasPositiveEndExpiratoryPressure())
      dst.setPositiveEndExpiratoryPressure(SEScalarPressure.unload(src.getPositiveEndExpiratoryPressure()));
    if (src.hasPulmonaryCompliance())
      dst.setPulmonaryCompliance(SEScalarVolumePerPressure.unload(src.getPulmonaryCompliance()));
    if (src.hasPulmonaryElastance())
        dst.setPulmonaryElastance(SEScalarPressurePerVolume.unload(src.getPulmonaryElastance()));
    if (src.hasResistiveExpiratoryWorkOfBreathing())
      dst.setResistiveExpiratoryWorkOfBreathing(SEScalarEnergy.unload(src.getResistiveExpiratoryWorkOfBreathing()));
    if (src.hasResistiveInspiratoryWorkOfBreathing())
      dst.setResistiveInspiratoryWorkOfBreathing(SEScalarEnergy.unload(src.getResistiveInspiratoryWorkOfBreathing()));
    if (src.hasRespirationRate())
      dst.setRespirationRate(SEScalarFrequency.unload(src.getRespirationRate()));
    if (src.hasRespiratoryMuscleFatigue())
      dst.setRespiratoryMuscleFatigue(SEScalar0To1.unload(src.getRespiratoryMuscleFatigue()));
    if (src.hasRespiratoryMusclePressure())
      dst.setRespiratoryMusclePressure(SEScalarPressure.unload(src.getRespiratoryMusclePressure()));
    if (src.hasSpecificVentilation())
      dst.setSpecificVentilation(SEScalar.unload(src.getSpecificVentilation()));
    if (src.hasTidalVolume())
      dst.setTidalVolume(SEScalarVolume.unload(src.getTidalVolume()));
    if (src.hasTotalAlveolarVentilation())
      dst.setTotalAlveolarVentilation(SEScalarVolumePerTime.unload(src.getTotalAlveolarVentilation()));
    if (src.hasTotalDeadSpaceVentilation())
      dst.setTotalDeadSpaceVentilation(SEScalarVolumePerTime.unload(src.getTotalDeadSpaceVentilation()));
    if (src.hasTotalLungVolume())
      dst.setTotalLungVolume(SEScalarVolume.unload(src.getTotalLungVolume()));
    if (src.hasTotalPowerOfBreathing())
      dst.setTotalPowerOfBreathing(SEScalarPower.unload(src.getTotalPowerOfBreathing()));
    if (src.hasTotalPulmonaryVentilation())
      dst.setTotalPulmonaryVentilation(SEScalarVolumePerTime.unload(src.getTotalPulmonaryVentilation()));
    if (src.hasTotalWorkOfBreathing())
      dst.setTotalWorkOfBreathing(SEScalarEnergy.unload(src.getTotalWorkOfBreathing()));
    if (src.hasTransairwayPressure())
      dst.setTransairwayPressure(SEScalarPressure.unload(src.getTransairwayPressure()));
    if (src.hasTransalveolarPressure())
      dst.setTransalveolarPressure(SEScalarPressure.unload(src.getTransalveolarPressure()));
    if (src.hasTransChestWallPressure())
      dst.setTransChestWallPressure(SEScalarPressure.unload(src.getTransChestWallPressure()));
    if (src.hasTransMusclePressure())
      dst.setTransMusclePressure(SEScalarPressure.unload(src.getTransMusclePressure()));
    if (src.hasTranspulmonaryPressure())
      dst.setTranspulmonaryPressure(SEScalarPressure.unload(src.getTranspulmonaryPressure()));
    if (src.hasTransrespiratoryPressure())
      dst.setTransrespiratoryPressure(SEScalarPressure.unload(src.getTransrespiratoryPressure()));
    if (src.hasTransthoracicPressure())
      dst.setTransthoracicPressure(SEScalarPressure.unload(src.getTransthoracicPressure()));
  }

  public boolean hasAlveolarArterialGradient()
  {
    return alveolarArterialGradient == null ? false : alveolarArterialGradient.isValid();
  }
  public SEScalarPressure getAlveolarArterialGradient()
  {
    if (alveolarArterialGradient == null)
      alveolarArterialGradient = new SEScalarPressure();
    return alveolarArterialGradient;
  }
  
  public boolean hasAlveolarDeadSpace()
  {
    return alveolarDeadSpace == null ? false : alveolarDeadSpace.isValid();
  }
  public SEScalarVolume getAlveolarDeadSpace()
  {
    if (alveolarDeadSpace == null)
      alveolarDeadSpace = new SEScalarVolume();
    return alveolarDeadSpace;
  }
  
  public boolean hasAnatomicDeadSpace()
  {
    return anatomicDeadSpace == null ? false : anatomicDeadSpace.isValid();
  }
  public SEScalarVolume getAnatomicDeadSpace()
  {
    if (anatomicDeadSpace == null)
      anatomicDeadSpace = new SEScalarVolume();
    return anatomicDeadSpace;
  }

  public boolean hasCarricoIndex()
  {
    return carricoIndex == null ? false : carricoIndex.isValid();
  }
  public SEScalarPressure getCarricoIndex()
  {
    if (carricoIndex == null)
      carricoIndex = new SEScalarPressure();
    return carricoIndex;
  }

  public boolean hasChestWallCompliance()
  {
    return chestWallCompliance == null ? false : chestWallCompliance.isValid();
  }
  public SEScalarVolumePerPressure getChestWallCompliance()
  {
    if (chestWallCompliance == null)
      chestWallCompliance = new SEScalarVolumePerPressure();
    return chestWallCompliance;
  }

  public boolean hasElasticWorkOfBreathing()
  {
    return elasticWorkOfBreathing == null ? false : elasticWorkOfBreathing.isValid();
  }
  public SEScalarEnergy getElasticWorkOfBreathing()
  {
    if (elasticWorkOfBreathing == null)
      elasticWorkOfBreathing = new SEScalarEnergy();
    return elasticWorkOfBreathing;
  }

  public boolean hasEndTidalCarbonDioxideFraction()
  {
    return endTidalCarbonDioxideFraction == null ? false : endTidalCarbonDioxideFraction.isValid();
  }
  public SEScalar0To1 getEndTidalCarbonDioxideFraction()
  {
    if (endTidalCarbonDioxideFraction == null)
      endTidalCarbonDioxideFraction = new SEScalar0To1();
    return endTidalCarbonDioxideFraction;
  }

  public boolean hasEndTidalCarbonDioxidePressure()
  {
    return endTidalCarbonDioxidePressure == null ? false : endTidalCarbonDioxidePressure.isValid();
  }
  public SEScalarPressure getEndTidalCarbonDioxidePressure()
  {
    if (endTidalCarbonDioxidePressure == null)
      endTidalCarbonDioxidePressure = new SEScalarPressure();
    return endTidalCarbonDioxidePressure;
  }
  
  public boolean hasEndTidalOxygenFraction()
  {
    return endTidalOxygenFraction == null ? false : endTidalOxygenFraction.isValid();
  }
  public SEScalar0To1 getEndTidalOxygenFraction()
  {
    if (endTidalOxygenFraction == null)
      endTidalOxygenFraction = new SEScalar0To1();
    return endTidalOxygenFraction;
  }

  public boolean hasEndTidalOxygenPressure()
  {
    return endTidalOxygenPressure == null ? false : endTidalOxygenPressure.isValid();
  }
  public SEScalarPressure getEndTidalOxygenPressure()
  {
    if (endTidalOxygenPressure == null)
      endTidalOxygenPressure = new SEScalarPressure();
    return endTidalOxygenPressure;
  }

  public boolean hasExpiratoryFlow()
  {
    return expiratoryFlow == null ? false : expiratoryFlow.isValid();
  }
  public SEScalarVolumePerTime getExpiratoryFlow()
  {
    if (expiratoryFlow == null)
      expiratoryFlow = new SEScalarVolumePerTime();
    return expiratoryFlow;
  }

  public boolean hasExpiratoryPulmonaryResistance()
  {
    return expiratoryPulmonaryResistance == null ? false : expiratoryPulmonaryResistance.isValid();
  }
  public SEScalarPressureTimePerVolume getExpiratoryPulmonaryResistance()
  {
    if (expiratoryPulmonaryResistance == null)
      expiratoryPulmonaryResistance = new SEScalarPressureTimePerVolume();
    return expiratoryPulmonaryResistance;
  }
  

  public boolean hasImposedPowerOfBreathing()
  {
    return imposedPowerOfBreathing == null ? false : imposedPowerOfBreathing.isValid();
  }
  public SEScalarPower getImposedPowerOfBreathing()
  {
    if (imposedPowerOfBreathing == null)
      imposedPowerOfBreathing = new SEScalarPower();
    return imposedPowerOfBreathing;
  }

  public boolean hasImposedWorkOfBreathing()
  {
    return imposedWorkOfBreathing == null ? false : imposedWorkOfBreathing.isValid();
  }
  public SEScalarEnergy getImposedWorkOfBreathing()
  {
    if (imposedWorkOfBreathing == null)
      imposedWorkOfBreathing = new SEScalarEnergy();
    return imposedWorkOfBreathing;
  }

  public boolean hasInspiratoryExpiratoryRatio()
  {
    return inspiratoryExpiratoryRatio == null ? false : inspiratoryExpiratoryRatio.isValid();
  }
  public SEScalar getInspiratoryExpiratoryRatio()
  {
    if (inspiratoryExpiratoryRatio == null)
      inspiratoryExpiratoryRatio = new SEScalar();
    return inspiratoryExpiratoryRatio;
  }

  public boolean hasInspiratoryFlow()
  {
    return inspiratoryFlow == null ? false : inspiratoryFlow.isValid();
  }
  public SEScalarVolumePerTime getInspiratoryFlow()
  {
    if (inspiratoryFlow == null)
      inspiratoryFlow = new SEScalarVolumePerTime();
    return inspiratoryFlow;
  }
  
  public boolean hasInspiratoryPulmonaryResistance()
  {
    return inspiratoryPulmonaryResistance == null ? false : inspiratoryPulmonaryResistance.isValid();
  }
  public SEScalarPressureTimePerVolume getInspiratoryPulmonaryResistance()
  {
    if (inspiratoryPulmonaryResistance == null)
      inspiratoryPulmonaryResistance = new SEScalarPressureTimePerVolume();
    return inspiratoryPulmonaryResistance;
  }
  
  public boolean hasIntrapleuralPressure()
  {
    return intrapleuralPressure == null ? false : intrapleuralPressure.isValid();
  }
  public SEScalarPressure getIntrapleuralPressure()
  {
    if (intrapleuralPressure == null)
      intrapleuralPressure = new SEScalarPressure();
    return intrapleuralPressure;
  }
  
  public boolean hasIntrapulmonaryPressure()
  {
    return intrapulmonaryPressure == null ? false : intrapulmonaryPressure.isValid();
  }
  public SEScalarPressure getIntrapulmonaryPressure()
  {
    if (intrapulmonaryPressure == null)
      intrapulmonaryPressure = new SEScalarPressure();
    return intrapulmonaryPressure;
  }
  
  public boolean hasLungCompliance()
  {
    return lungCompliance == null ? false : lungCompliance.isValid();
  }
  public SEScalarVolumePerPressure getLungCompliance()
  {
    if (lungCompliance == null)
      lungCompliance = new SEScalarVolumePerPressure();
    return lungCompliance;
  }
  
  public boolean hasMaximalInspiratoryPressure()
  {
    return maximalInspiratoryPressure == null ? false : maximalInspiratoryPressure.isValid();
  }
  public SEScalarPressure getMaximalInspiratoryPressure()
  {
    if (maximalInspiratoryPressure == null)
      maximalInspiratoryPressure = new SEScalarPressure();
    return maximalInspiratoryPressure;
  }
  
  public boolean hasPatientPowerOfBreathing()
  {
    return patientPowerOfBreathing == null ? false : patientPowerOfBreathing.isValid();
  }
  public SEScalarPower getPatientPowerOfBreathing()
  {
    if (patientPowerOfBreathing == null)
      patientPowerOfBreathing = new SEScalarPower();
    return patientPowerOfBreathing;
  }
  
  public boolean hasPatientWorkOfBreathing()
  {
    return patientWorkOfBreathing == null ? false : patientWorkOfBreathing.isValid();
  }
  public SEScalarEnergy getPatientWorkOfBreathing()
  {
    if (patientWorkOfBreathing == null)
      patientWorkOfBreathing = new SEScalarEnergy();
    return patientWorkOfBreathing;
  }
  
  public boolean hasPeakInspiratoryPressure()
  {
    return peakInspiratoryPressure == null ? false : peakInspiratoryPressure.isValid();
  }
  public SEScalarPressure getPeakInspiratoryPressure()
  {
    if (peakInspiratoryPressure == null)
      peakInspiratoryPressure = new SEScalarPressure();
    return peakInspiratoryPressure;
  }
  
  public boolean hasPhysiologicDeadSpace()
  {
    return physiologicDeadSpace == null ? false : physiologicDeadSpace.isValid();
  }
  public SEScalarVolume getPhysiologicDeadSpace()
  {
    if (physiologicDeadSpace == null)
      physiologicDeadSpace = new SEScalarVolume();
    return physiologicDeadSpace;
  }
  
  public boolean hasPositiveEndExpiratoryPressure()
  {
    return positiveEndExpiratoryPressure == null ? false : positiveEndExpiratoryPressure.isValid();
  }
  public SEScalarPressure getPositiveEndExpiratoryPressure()
  {
    if (positiveEndExpiratoryPressure == null)
      positiveEndExpiratoryPressure = new SEScalarPressure();
    return positiveEndExpiratoryPressure;
  }

  public boolean hasPulmonaryCompliance()
  {
    return pulmonaryCompliance == null ? false : pulmonaryCompliance.isValid();
  }
  public SEScalarVolumePerPressure getPulmonaryCompliance()
  {
    if (pulmonaryCompliance == null)
      pulmonaryCompliance = new SEScalarVolumePerPressure();
    return pulmonaryCompliance;
  }

  public boolean hasPulmonaryElastance()
  {
    return pulmonaryElastance == null ? false : pulmonaryElastance.isValid();
  }
  public SEScalarPressurePerVolume getPulmonaryElastance()
  {
    if (pulmonaryElastance == null)
      pulmonaryElastance = new SEScalarPressurePerVolume();
    return pulmonaryElastance;
  }
  
  public boolean hasResistiveExpiratoryWorkOfBreathing()
  {
    return resistiveExpiratoryWorkOfBreathing == null ? false : resistiveExpiratoryWorkOfBreathing.isValid();
  }
  public SEScalarEnergy getResistiveExpiratoryWorkOfBreathing()
  {
    if (resistiveExpiratoryWorkOfBreathing == null)
      resistiveExpiratoryWorkOfBreathing = new SEScalarEnergy();
    return resistiveExpiratoryWorkOfBreathing;
  }
  
  public boolean hasResistiveInspiratoryWorkOfBreathing()
  {
    return resistiveInspiratoryWorkOfBreathing == null ? false : resistiveInspiratoryWorkOfBreathing.isValid();
  }
  public SEScalarEnergy getResistiveInspiratoryWorkOfBreathing()
  {
    if (resistiveInspiratoryWorkOfBreathing == null)
      resistiveInspiratoryWorkOfBreathing = new SEScalarEnergy();
    return resistiveInspiratoryWorkOfBreathing;
  }
  
  public boolean hasRespirationRate()
  {
    return respirationRate == null ? false : respirationRate.isValid();
  }
  public SEScalarFrequency getRespirationRate()
  {
    if (respirationRate == null)
      respirationRate = new SEScalarFrequency();
    return respirationRate;
  }

  boolean hasRespiratoryMuscleFatigue()
  {
    return respiratoryMuscleFatigue == null ? false : respiratoryMuscleFatigue.isValid();
  }
  public SEScalar0To1 getRespiratoryMuscleFatigue()
  {
    if (respiratoryMuscleFatigue == null)
      respiratoryMuscleFatigue = new SEScalar0To1();
    return respiratoryMuscleFatigue;
  }

  public boolean hasRespiratoryMusclePressure()
  {
    return respiratoryMusclePressure == null ? false : respiratoryMusclePressure.isValid();
  }
  public SEScalarPressure getRespiratoryMusclePressure()
  {
    if (respiratoryMusclePressure == null)
      respiratoryMusclePressure = new SEScalarPressure();
    return respiratoryMusclePressure;
  }

  public boolean hasSpecificVentilation()
  {
    return specificVentilation == null ? false : specificVentilation.isValid();
  }
  public SEScalar getSpecificVentilation()
  {
    if (specificVentilation == null)
      specificVentilation = new SEScalar();
    return specificVentilation;
  }

  public boolean hasTidalVolume()
  {
    return tidalVolume == null ? false : tidalVolume.isValid();
  }
  public SEScalarVolume getTidalVolume()
  {
    if (tidalVolume == null)
      tidalVolume = new SEScalarVolume();
    return tidalVolume;
  }

  public boolean hasTotalAlveolarVentilation()
  {
    return totalAlveolarVentilation == null ? false : totalAlveolarVentilation.isValid();
  }
  public SEScalarVolumePerTime getTotalAlveolarVentilation()
  {
    if (totalAlveolarVentilation == null)
      totalAlveolarVentilation = new SEScalarVolumePerTime();
    return totalAlveolarVentilation;
  }

  public boolean hasTotalDeadSpaceVentilation()
  {
    return totalDeadSpaceVentilation == null ? false : totalDeadSpaceVentilation.isValid();
  }
  public SEScalarVolumePerTime getTotalDeadSpaceVentilation()
  {
    if (totalDeadSpaceVentilation == null)
      totalDeadSpaceVentilation = new SEScalarVolumePerTime();
    return totalDeadSpaceVentilation;
  }

  public boolean hasTotalLungVolume()
  {
    return totalLungVolume == null ? false : totalLungVolume.isValid();
  }
  public SEScalarVolume getTotalLungVolume()
  {
    if (totalLungVolume == null)
      totalLungVolume = new SEScalarVolume();
    return totalLungVolume;
  }
  
  public boolean hasTotalPowerOfBreathing()
  {
    return totalPowerOfBreathing == null ? false : totalPowerOfBreathing.isValid();
  }
  public SEScalarPower getTotalPowerOfBreathing()
  {
    if (totalPowerOfBreathing == null)
      totalPowerOfBreathing = new SEScalarPower();
    return totalPowerOfBreathing;
  }

  public boolean hasTotalPulmonaryVentilation()
  {
    return totalPulmonaryVentilation == null ? false : totalPulmonaryVentilation.isValid();
  }
  public SEScalarVolumePerTime getTotalPulmonaryVentilation()
  {
    if (totalPulmonaryVentilation == null)
      totalPulmonaryVentilation = new SEScalarVolumePerTime();
    return totalPulmonaryVentilation;
  }
  
  public boolean hasTotalWorkOfBreathing()
  {
    return totalWorkOfBreathing == null ? false : totalWorkOfBreathing.isValid();
  }
  public SEScalarEnergy getTotalWorkOfBreathing()
  {
    if (totalWorkOfBreathing == null)
      totalWorkOfBreathing = new SEScalarEnergy();
    return totalWorkOfBreathing;
  }
  
  public boolean hasTransairwayPressure()
  {
    return transairwayPressure == null ? false : transairwayPressure.isValid();
  }
  public SEScalarPressure getTransairwayPressure()
  {
    if (transairwayPressure == null)
      transairwayPressure = new SEScalarPressure();
    return transairwayPressure;
  }
  
  public boolean hasTransalveolarPressure()
  {
    return transalveolarPressure == null ? false : transalveolarPressure.isValid();
  }
  public SEScalarPressure getTransalveolarPressure()
  {
    if (transalveolarPressure == null)
      transalveolarPressure = new SEScalarPressure();
    return transalveolarPressure;
  }
  
  public boolean hasTransChestWallPressure()
  {
    return transChestWallPressure == null ? false : transChestWallPressure.isValid();
  }
  public SEScalarPressure getTransChestWallPressure()
  {
    if (transChestWallPressure == null)
      transChestWallPressure = new SEScalarPressure();
    return transChestWallPressure;
  }

  public boolean hasTransMusclePressure()
  {
    return transMusclePressure == null ? false : transMusclePressure.isValid();
  }
  public SEScalarPressure getTransMusclePressure()
  {
    if (transMusclePressure == null)
      transMusclePressure = new SEScalarPressure();
    return transMusclePressure;
  }

  public boolean hasTranspulmonaryPressure()
  {
    return transpulmonaryPressure == null ? false : transpulmonaryPressure.isValid();
  }
  public SEScalarPressure getTranspulmonaryPressure()
  {
    if (transpulmonaryPressure == null)
      transpulmonaryPressure = new SEScalarPressure();
    return transpulmonaryPressure;
  }
  
  public boolean hasTransrespiratoryPressure()
  {
    return transrespiratoryPressure == null ? false : transrespiratoryPressure.isValid();
  }
  public SEScalarPressure getTransrespiratoryPressure()
  {
    if (transrespiratoryPressure == null)
      transrespiratoryPressure = new SEScalarPressure();
    return transrespiratoryPressure;
  }
  
  public boolean hasTransthoracicPressure()
  {
    return transthoracicPressure == null ? false : transthoracicPressure.isValid();
  }
  public SEScalarPressure getTransthoracicPressure()
  {
    if (transthoracicPressure == null)
      transthoracicPressure = new SEScalarPressure();
    return transthoracicPressure;
  }
}
