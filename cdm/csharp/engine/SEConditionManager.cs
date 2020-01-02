/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class SEConditionManager
{
  protected SEAcuteRespiratoryDistressSyndrome ards;
  protected SEChronicAnemia anemia;
  //protected SEConsumeMeal consume_meal;
  protected SEChronicObstructivePulmonaryDisease copd;
  protected SEChronicVentricularSystolicDysfunction cvsd;
  protected SEImpairedAlveolarExchange impaired_alveolar_exchange;
  protected SEChronicPericardialEffusion pericardial_effusion;
  protected SELobarPneumonia lobar_pneumonia;
  protected SEPulmonaryFibrosis pulmonary_fibrosis;
  protected SEChronicRenalStenosis renal_stenosis;
  protected SESepsis sepsis;

  //protected SEInitialEnvironmentConditions* initial_environment_conditions;

  public SEConditionManager()
  {
    Clear();
  }

  public void Clear()
  {
    ards = null;
    anemia = null;
    //consume_meal = null;
    copd = null;
    cvsd = null;
    impaired_alveolar_exchange = null;
    pericardial_effusion = null;
    lobar_pneumonia = null;
    pulmonary_fibrosis = null;
    renal_stenosis = null;
    sepsis = null;
    //initial_environment_conditions = null;
  }

  public bool IsEmpty()
  {
    if (HasAcuteRespiratoryDistressSyndrome())
      return false;
    if (HasChronicAnemia())
      return false;
    if (HasChronicObstructivePulmonaryDisease())
      return false;
    if (HasChronicVentricularSystolicDysfunction())
      return false;
    if (HasChronicPericardialEffusion())
      return false;
    if (HasChronicRenalStenosis())
      return false;
    //if (HasConsumeMeal())
    //  return false;
    if (HasImpairedAlveolarExchange())
      return false;
    if (HasLobarPneumonia())
      return false;
    if (HasPulmonaryFibrosis())
      return false;
    if (HasSepsis())
      return false;

    //if (HasInitialEnvironmentConditions())
    //  return false;
    return true;
  }

  public bool Add(SECondition c)
  {
    if (c.GetType().IsAssignableFrom(typeof(SEAcuteRespiratoryDistressSyndrome)))
    {
      ards = (SEAcuteRespiratoryDistressSyndrome)c;
      return true;
    }
    if (c.GetType().IsAssignableFrom(typeof(SEChronicAnemia)))
    {
      anemia = (SEChronicAnemia)c;
      return true;
    }
    if (c.GetType().IsAssignableFrom(typeof(SEChronicObstructivePulmonaryDisease)))
    {
      copd = (SEChronicObstructivePulmonaryDisease)c;
      return true;
    }
    if (c.GetType().IsAssignableFrom(typeof(SEChronicVentricularSystolicDysfunction)))
    {
      cvsd = (SEChronicVentricularSystolicDysfunction)c;
      return true;
    }
    if (c.GetType().IsAssignableFrom(typeof(SEChronicPericardialEffusion)))
    {
      pericardial_effusion = (SEChronicPericardialEffusion)c;
      return true;
    }
    //if (c.GetType().IsAssignableFrom(typeof(SEConsumeMeal)))
    //{
    //  consume_meal = (SEConsumeMeal)c;
    //  return true;
    //}
    if (c.GetType().IsAssignableFrom(typeof(SEChronicRenalStenosis)))
    {
      renal_stenosis = (SEChronicRenalStenosis)c;
      return true;
    }
    if (c.GetType().IsAssignableFrom(typeof(SEImpairedAlveolarExchange)))
    {
      impaired_alveolar_exchange = (SEImpairedAlveolarExchange)c;
      return true;
    }
    if (c.GetType().IsAssignableFrom(typeof(SELobarPneumonia)))
    {
      lobar_pneumonia = (SELobarPneumonia)c;
      return true;
    }
    if (c.GetType().IsAssignableFrom(typeof(SEPulmonaryFibrosis)))
    {
      pulmonary_fibrosis = (SEPulmonaryFibrosis)c;
      return true;
    }
    if (c.GetType().IsAssignableFrom(typeof(SESepsis)))
    {
      sepsis = (SESepsis)c;
      return true;
    }

    //if (c.GetType().IsAssignableFrom(typeof(SEInitialEnvironmentalConditions)))
    //{
    //  initial_environmental_conditions = (SEInitialEnvironmentalConditions)c;
    //  return true;
    //}
    return false;
  }

  public bool HasAcuteRespiratoryDistressSyndrome()
  {
    return ards == null ? false : ards.IsValid();
  }
  public SEAcuteRespiratoryDistressSyndrome GetAcuteRespiratoryDistressSyndrome()
  {
    if (ards == null)
      ards = new SEAcuteRespiratoryDistressSyndrome();
    return ards;
  }
  public void RemoveAcuteRespiratoryDistressSyndrome()
  {
    ards = null;
  }

  public bool HasChronicAnemia()
  {
    return anemia == null ? false : anemia.IsValid();
  }
  public SEChronicAnemia GetChronicAnemia()
  {
    if (anemia == null)
      anemia = new SEChronicAnemia();
    return anemia;
  }
  public void RemoveChronicAnemia()
  {
    anemia = null;
  }

  public bool HasChronicObstructivePulmonaryDisease()
  {
    return copd == null ? false : copd.IsValid();
  }
  public SEChronicObstructivePulmonaryDisease GetChronicObstructivePulmonaryDisease()
  {
    if (copd == null)
      copd = new SEChronicObstructivePulmonaryDisease();
    return copd;
  }
  public void RemoveChronicObstructivePulmonaryDisease()
  {
    copd = null;
  }

  public bool HasChronicHeartFailure()
  {
    return HasChronicVentricularSystolicDysfunction();
  }
  public bool HasChronicVentricularSystolicDysfunction()
  {
    return cvsd == null ? false : cvsd.IsValid();
  }
  public SEChronicVentricularSystolicDysfunction GetChronicVentricularSystolicDysfunction()
  {
    if (cvsd == null)
      cvsd = new SEChronicVentricularSystolicDysfunction();
    return cvsd;
  }
  public void RemoveChronicVentricularSystolicDysfunction()
  {
    cvsd = null;
  }

  public bool HasChronicPericardialEffusion()
  {
    return pericardial_effusion == null ? false : pericardial_effusion.IsValid();
  }
  public SEChronicPericardialEffusion GetChronicPericardialEffusion()
  {
    if (pericardial_effusion == null)
      pericardial_effusion = new SEChronicPericardialEffusion();
    return pericardial_effusion;
  }
  public void RemoveChronicPericardialEffusion()
  {
    pericardial_effusion = null;
  }

  public bool HasChronicRenalStenosis()
  {
    return renal_stenosis == null ? false : renal_stenosis.IsValid();
  }
  public SEChronicRenalStenosis GetChronicRenalStenosis()
  {
    if (renal_stenosis == null)
      renal_stenosis = new SEChronicRenalStenosis();
    return renal_stenosis;
  }
  public void RemoveChronicRenalStenosis()
  {
    renal_stenosis = null;
  }

  //public bool HasConsumeMeal()
  //public SEConsumeMeal GetConsumeMeal()
  //public void RemoveConsumeMeal()

  public bool HasImpairedAlveolarExchange()
  {
    return impaired_alveolar_exchange == null ? false : impaired_alveolar_exchange.IsValid();
  }
  public SEImpairedAlveolarExchange GetImpairedAlveolarExchange()
  {
    if (impaired_alveolar_exchange == null)
      impaired_alveolar_exchange = new SEImpairedAlveolarExchange();
    return impaired_alveolar_exchange;
  }
  public void RemoveImpairedAlveolarExchange()
  {
    impaired_alveolar_exchange = null;
  }

  public bool HasLobarPneumonia()
  {
    return lobar_pneumonia == null ? false : lobar_pneumonia.IsValid();
  }
  public SELobarPneumonia GetLobarPneumonia()
  {
    if (lobar_pneumonia == null)
      lobar_pneumonia = new SELobarPneumonia();
    return lobar_pneumonia;
  }
  public void RemoveLobarPneumonia()
  {
    lobar_pneumonia = null;
  }

  public bool HasPulmonaryFibrosis()
  {
    return pulmonary_fibrosis == null ? false : pulmonary_fibrosis.IsValid();
  }
  public SEPulmonaryFibrosis GetPulmonaryFibrosis()
  {
    if (pulmonary_fibrosis == null)
      pulmonary_fibrosis = new SEPulmonaryFibrosis();
    return pulmonary_fibrosis;
  }
  public void RemovePulmonaryFibrosis()
  {
    pulmonary_fibrosis = null;
  }

  public bool HasSepsis()
  {
    return sepsis == null ? false : sepsis.IsValid();
  }
  public SESepsis GetSepsis()
  {
    if (sepsis == null)
      sepsis = new SESepsis();
    return sepsis;
  }
  public void RemoveSepsis()
  {
    sepsis = null;
  }

  // Environment Conditions
  //public bool HasInitialEnvironmentalConditions()
  //public SEInitialEnvironmentConditions GetInitialEnvironmentalConditions()
  //public void RemoveInitialEnvironmentalConditions()
}
