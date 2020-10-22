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
  
  virtual void Clear();

  virtual bool IsValid() const = 0;
  virtual bool IsActive() const = 0;

  virtual std::string GetName() const = 0;

  virtual std::string GetComment() const;
  virtual void SetComment(const std::string& comment);
  virtual bool HasComment()const;
  virtual void InvalidateComment();

  virtual void ToString(std::ostream &str) const=0;

protected:

  std::string  m_Comment;
};  

inline std::ostream& operator<< (std::ostream& out, const SECondition& a) 
{
    a.ToString(out);
    return out;
}
