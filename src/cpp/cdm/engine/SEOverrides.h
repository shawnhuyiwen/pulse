/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/SEAction.h"

class CDM_DECL SEOverrides : public SEAction
{
  friend class PBAction;//friend the serialization class
public:

  SEOverrides();
  virtual ~SEOverrides();

  virtual void Clear(); //clear memory
  virtual void Copy(const SEOverrides& src);

  virtual bool IsValid() const;

  virtual void ToString(std::ostream &str) const;

  virtual bool HasPairs() const;
  virtual void AddPair(const std::string& name, double value);
  virtual std::map<std::string, double>& GetPairs();
  virtual const std::map<std::string, double>& GetPairs() const;
  virtual void RemovePairs();

protected:
  std::map<std::string, double> m_Pairs;
};