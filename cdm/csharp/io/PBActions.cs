/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

class PBAction
{
    #region SEAction
    public static void Serialize(Cdm.ActionData src, SEAction dst)
    {
        dst.Clear();
        if(src.Comment != null)
            dst.SetComment(src.Comment);
    }
    public static void Serialize(SEAction src, Cdm.ActionData dst)
    {
        if(src.HasComment())
            dst.Comment = src.GetComment();
    }
    #endregion
}