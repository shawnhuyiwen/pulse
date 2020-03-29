/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SESubstanceManager;

class CDM_DECL SEAction : public Loggable
{
  friend class PBAction;//friend the serialization class
public:

  SEAction();
  virtual ~SEAction();

  virtual void Clear();// Deletes all members
  
  /** Test if the action has all data it needs */
  virtual bool IsValid() const { return true; }
  /** Actions can be turned off or on with various data combinations
  *  This method will encapsulate that logic in a single function */
  virtual bool IsActive() const { return IsValid(); }

  virtual std::string GetComment() const;
  virtual void SetComment(const std::string& comment);
  virtual bool HasComment() const;
  virtual void InvalidateComment();

  virtual void ToString(std::ostream &str)const=0;

protected:

  std::string           m_Comment;
};  

inline std::ostream& operator<< (std::ostream& out, const SEAction& a) 
{
    a.ToString(out);
    return out;
}