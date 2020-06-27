/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SESubstance;
class SESubstanceCompound;

class CDM_DECL SESubstanceManager : public Loggable
{
public:
  SESubstanceManager(Logger* logger);
  virtual ~SESubstanceManager();

  // Will clear (see below) and refresh all substance to their original content
  // *NOTE* If you have added custom substances (not on disk)
  // They will still be in the sub mgr, but all properties will be invalid
  // It is your responsibility to reset them to their original values
  virtual bool                                           LoadSubstanceDirectory(const std::string& data_dir="./");

  virtual const std::vector<SESubstance*>&               GetSubstances();
  virtual const std::vector<const SESubstance*>&         GetSubstances() const;
  virtual bool                                           HasSubstance(const std::string& name) const;
  virtual SESubstance*                                   GetSubstance(const std::string& name);
  virtual const SESubstance*                             GetSubstance(const std::string& name) const;

  virtual bool                                           IsActive(const SESubstance& substance) const;
  virtual const std::vector<SESubstance*>&               GetActiveSubstances();
  virtual const std::vector<const SESubstance*>&         GetActiveSubstances() const;
  virtual void                                           AddActiveSubstance(const SESubstance& substance);
  virtual void                                           RemoveActiveSubstance(const SESubstance& substance);
  virtual void                                           RemoveActiveSubstances(const std::vector<SESubstance*>& substances);
  virtual void                                           RemoveActiveSubstances();
  
  virtual const std::vector<SESubstance*>&               GetActiveGases();
  virtual const std::vector<const SESubstance*>&         GetActiveGases() const;
  virtual const std::vector<SESubstance*>&               GetActiveLiquids();
  virtual const std::vector<const SESubstance*>&         GetActiveLiquids() const;

  virtual const std::vector<SESubstanceCompound*>&       GetCompounds();
  virtual const std::vector<const SESubstanceCompound*>& GetCompounds() const;
  virtual bool                                           HasCompound(const std::string& name) const;
  virtual SESubstanceCompound*                           GetCompound(const std::string& name);
  virtual const SESubstanceCompound*                     GetCompound(const std::string& name) const;

  virtual bool                                           IsActive(const SESubstanceCompound& compound) const;
  virtual const std::vector<SESubstanceCompound*>&       GetActiveCompounds();
  virtual const std::vector<const SESubstanceCompound*>& GetActiveCompounds() const;
  virtual void                                           AddActiveCompound(const SESubstanceCompound& compound);
  virtual void                                           RemoveActiveCompound(const SESubstanceCompound& compound);
  virtual void                                           RemoveActiveCompounds(const std::vector<SESubstanceCompound*>& compounds);

protected:
  // The sub mgr is designed to keep the pointers of all subs (and their components)
  // at the same address as when they were first allocated.
  // This clear preserves all addresses.
  // This clear method will just invalidate all properties of all substances.
  // Substances will keep their name and have "no properties"
  // All substance component (ex. clearance) properties will also be invalidated.
  // 
  // It is your responsibility to set the substance properties back to something
  // (i.e. Just use LoadSubstanceDirectory)
  virtual void                                     Clear();

  // I am making all these vectors of SESubstances,
  // Usually these are passed to methods that take a 
  // vector of substances, and a vector of drugs won't be accepted
  // I could do some crazy template methods that will auto convert
  // these to substance vectors, but I think that would be too 
  // slow for the amount of times we call stuff
  // So for now, these will just be substance vectors
  // If we want drug vectors, I can rename methods to 
  // std::vector<SESubstance*>* GetActiveDrugSubstances 
  // std::vector<SEDrug*>* GetActiveDrugs()
  // But I don't see a need for that yet...
  // So will just leave it as it now

  std::vector<SESubstance*>                m_Substances;
  std::vector<const SESubstance*>          m_cSubstances;
  std::vector<SESubstance*>                m_ActiveSubstances;
  std::vector<const SESubstance*>          m_cActiveSubstances;
  std::vector<SESubstance*>                m_ActiveGases;
  std::vector<const SESubstance*>          m_cActiveGases;
  std::vector<SESubstance*>                m_ActiveLiquids;
  std::vector<const SESubstance*>          m_cActiveLiquids;

  std::vector<SESubstanceCompound*>        m_Compounds;
  std::vector<const SESubstanceCompound*>  m_cCompounds;
  std::vector<SESubstanceCompound*>        m_ActiveCompounds;
  std::vector<const SESubstanceCompound*>  m_cActiveCompounds;
};