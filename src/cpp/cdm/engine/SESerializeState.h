/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/SEAction.h"

// Keep enums in sync with appropriate schema/cdm/ActionEnums.proto file !!
enum class eSerialization_Type { Save = 0, Load };
extern const std::string& eSerialization_Type_Name(eSerialization_Type m);

class CDM_DECL SESerializeState : public SEAction
{
  friend class PBAction;//friend the serialization class
public:

  SESerializeState(Logger* logger=nullptr);
  virtual ~SESerializeState();

  virtual void Clear(); //clear memory

  virtual bool IsValid() const;

  virtual eSerialization_Type GetType() const;
  virtual void SetType(eSerialization_Type t);

  virtual bool HasFilename() const;
  virtual std::string GetFilename() const;
  virtual void SetFilename(const std::string& filename);
  virtual void InvalidateFilename();

  virtual void ToString(std::ostream &str) const;

  virtual const SEScalar* GetScalar(const std::string& name);

protected:

  std::string         m_Filename;
  eSerialization_Type m_Type;
};