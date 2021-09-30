/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

class CDM_DECL SESegment
{
protected:
  friend class SECurve;
  SESegment();
public:
  virtual ~SESegment();

  virtual void Clear();
  virtual bool IsValid() const;

  virtual bool HasBeginVolume() const;
  virtual SEScalarVolume& GetBeginVolume();
  virtual double GetBeginVolume(const VolumeUnit& unit) const;

  virtual bool HasEndVolume() const;
  virtual SEScalarVolume& GetEndVolume();
  virtual double GetEndVolume(const VolumeUnit& unit) const;

  virtual std::string ToString() const;
  virtual void ToString(std::ostream& str) const;

protected:
  virtual void Abstract() = 0;// Force this class to be abstract

  SEScalarVolume* m_BeginVolume;
  SEScalarVolume* m_EndVolume;
};

inline std::ostream& operator<< (std::ostream& out, const SESegment* s)
{
  if (s == nullptr)
    out << "Null Segment" << std::flush;
  else
    (*s).ToString(out);
  return out;
}
inline std::ostream& operator<< (std::ostream& out, const SESegment& s)
{
  s.ToString(out);
  return out;
}
