/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package pulse.utilities;

import java.io.Serializable;

/**
 * The <code>Pair</code> contains two objects, paired together
 */
public class Pair<L,R> implements Serializable
{
  private static final long serialVersionUID = -2791959188291906115L;
  
  private L l;
  private R r;
  
  public Pair(L l, R r)
  { 
    this.l = l;
    this.r = r;   
  }
  
  @Override
  public boolean equals(Object o)
  {
    if (o == null || o.getClass() != getClass())
    {
      return false;
    }
    else
    {
      @SuppressWarnings("unchecked")
      Pair<L,R> in = (Pair<L,R>)o;
      if (!this.getL().equals(in.getL()) || !this.getR().equals(in.getR()))
      {
        return false;
      }
    }
      
    return true;
  }
  
  @Override
  public int hashCode()
  {
    int hash = 5;
    hash = 11 * hash + this.l.hashCode();
    hash = 11 * hash + this.r.hashCode();
    return hash; 
  }
 
  public L getL()
  {
    return this.l;
  }
 
  public void setL(L l) 
  {
    this.l = l;
  }
  
  public R getR() 
  {
    return this.r;
  }
  
  public void setR(R r) 
  {
    this.r = r;
  } 
  
  @Override
  public String toString()
  { 
    return "(" + 
      (l==null  ? "null" : l.toString()) + ", " + 
      (r==null ? "null" : r.toString()) + ")"; 
  }
  
  /**
   * Check if both parts of the pair are set (non-null)
   * 
   * @return
   */
  public boolean isPaired()
  {
    return l != null && r != null;
  }
}

