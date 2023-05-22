/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/engine/SEAction.h"

class CDM_DECL SESerializeRequested : public SEAction
{
  friend class PBAction;//friend the serialization class
public:

  SESerializeRequested(Logger* logger=nullptr);
  virtual ~SESerializeRequested();

  static constexpr char const* Name = "Serialize Requested";
  virtual std::string GetName() const { return Name; }

  virtual void Clear(); //clear memory

  virtual bool IsValid() const;

  virtual bool HasFilename() const;
  virtual std::string GetFilename() const;
  virtual void SetFilename(const std::string& filename);
  virtual void InvalidateFilename();

  virtual const SEScalar* GetScalar(const std::string& name);

protected:

  std::string         m_Filename;
};