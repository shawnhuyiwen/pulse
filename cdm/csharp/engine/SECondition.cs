/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public abstract class SECondition
{
  protected string comment;

  public SECondition()
  {
    comment = null;
  }

  public void Copy(SECondition other)
  {
    Clear();
    comment = other.comment;
  }

  public virtual void Clear()
  {
    comment = null;
  }

  public bool HasComment()
  {
    return !string.IsNullOrEmpty(comment);
  }
  public string GetComment()
  {
    return this.comment;
  }
  public void SetComment(string comment)
  {
    this.comment = comment;
  }
  public void InvalidateComment()
  {
    this.comment = null;
  }

  public abstract bool IsValid();
}
