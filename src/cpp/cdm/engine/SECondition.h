/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SESubstanceManager;

class CDM_DECL SECondition : public Loggable
{
  friend class PBCondition;//friend the serialization class
public:

  SECondition(Logger* logger);
  virtual ~SECondition();

  virtual std::string GetName() const = 0;
  static constexpr char const* ConditionType = "Condition";
  virtual std::string GetConditionType() const { return ConditionType; }
  
  virtual void Clear();

  virtual bool IsValid() const = 0;
  virtual bool IsActive() const = 0;

  virtual std::string GetComment() const;
  virtual void SetComment(const std::string& comment);
  virtual bool HasComment()const;
  virtual void InvalidateComment();

  virtual void ToString(std::ostream &str) const
  {
    str << GetConditionType() + " : " + GetName();
  }

protected:

  std::string  m_Comment;
};  

inline std::ostream& operator<< (std::ostream& out, const SECondition& a) 
{
    a.ToString(out);
    return out;
}

struct CDM_DECL SEConditionDictionary
{
  std::string                        Name = "";
  SEScalarProperties                 Properties;
  std::map<std::string, std::string> Enumerations;
};
