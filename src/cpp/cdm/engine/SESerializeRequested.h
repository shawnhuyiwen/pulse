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
  std::string GetName() const override { return Name; }

  void Clear() override; //clear memory

  bool IsValid() const override;

  virtual int GetID() const { return m_ID; }
  virtual void SetID(int id) { m_ID = id; }

  virtual bool GetClearCache() const { return m_ClearCache; }
  virtual void SetClearCache(bool b) { m_ClearCache = b; }

  virtual bool HasFilename() const;
  virtual std::string GetFilename() const;
  virtual void SetFilename(const std::string& filename);
  virtual void InvalidateFilename();

  const SEScalar* GetScalar(const std::string& name) override;

protected:
  bool                m_ClearCache;
  std::string         m_Filename;
  int                 m_ID;
};