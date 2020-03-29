/* Distributed under the Apache License, Version 2.0.
See accompanying NOTICE file for details.*/

#pragma once

// These classes are used to expose any methodology specific data to end users
// This data would not belong in the cdm, and it's not a clinical assessment, nor on a compartment
// It is data specific to the system implementation
// Users only get this data if they are using the pulse engine
// They should only contain const pure vitrual methods

class PULSE_DECL PulseBloodChemistrySystem
{
public:
  // Pure virtual methods to expose methodology data with
  // virtual double GetXXX(const ScalarUnit& unit) const = 0;
};

class PULSE_DECL PulseCardiovascularSystem
{
public:
  // Pure virtual methods to expose methodology data with
  // virtual double GetXXX(const ScalarUnit& unit) const = 0;
};

class PULSE_DECL PulseDrugsSystem
{
public:
  // Pure virtual methods to expose methodology data with
  // virtual double GetXXX(const ScalarUnit& unit) const = 0;
};

class PULSE_DECL PulseEndocrineSystem
{
public:
  // Pure virtual methods to expose methodology data with
  // virtual double GetXXX(const ScalarUnit& unit) const = 0;
};

class PULSE_DECL PulseEnergySystem
{
public:
  // Pure virtual methods to expose methodology data with
  // virtual double GetXXX(const ScalarUnit& unit) const = 0;
};

class PULSE_DECL PulseEnvironmentSystem
{
public:
  // Pure virtual methods to expose methodology data with
  // virtual double GetXXX(const ScalarUnit& unit) const = 0;
};

class PULSE_DECL PulseGastrointestinalSystem
{
public:
  // Pure virtual methods to expose methodology data with
  // virtual double GetXXX(const ScalarUnit& unit) const = 0;
};

class PULSE_DECL PulseHepaticSystem
{
public:
  // Pure virtual methods to expose methodology data with
  // virtual double GetXXX(const ScalarUnit& unit) const = 0;
};

class PULSE_DECL PulseNervousSystem
{
public:
  // Pure virtual methods to expose methodology data with
  // virtual double GetXXX(const ScalarUnit& unit) const = 0;
};

class PULSE_DECL PulseRenalSystem
{
public:
  // Pure virtual methods to expose methodology data with
  // virtual double GetXXX(const ScalarUnit& unit) const = 0;
};

class PULSE_DECL PulseRespiratorySystem
{
public:
  // Pure virtual methods to expose methodology data with
  // virtual double GetXXX(const ScalarUnit& unit) const = 0;
};

class PULSE_DECL PulseTissueSystem
{
public:
  // Pure virtual methods to expose methodology data with
  // virtual double GetXXX(const ScalarUnit& unit) const = 0;
};

class PULSE_DECL PulseAnesthesiaMachine
{
public:
  // Pure virtual methods to expose methodology data with
  // virtual double GetXXX(const ScalarUnit& unit) const = 0;
};

class PULSE_DECL PulseElectroCardioGram
{
public:
  // Pure virtual methods to expose methodology data with
  // virtual double GetXXX(const ScalarUnit& unit) const = 0;
};

class PULSE_DECL PulseInhaler
{
public:
  // Pure virtual methods to expose methodology data with
  // virtual double GetXXX(const ScalarUnit& unit) const = 0;
};