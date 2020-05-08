/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package pulse.cdm.testing.validation;

import java.util.Arrays;

import pulse.utilities.jniBridge;
import pulse.utilities.Log;

public class SystemValidation extends ValidationTool
{
  public SystemValidation()
  {
    DEST_DIRECTORY = "./test_results/tables/";
    DEFAULT_DIRECTORY = cfg.getValidationDirectory();
    DEFAULT_FILE = "SystemValidationData.xlsx";
    TABLE_TYPE = "System";
    VALIDATION_FOLDER = "systems";
  }
  public static void main(String[] args)
  {
    jniBridge.initialize();
    Log.info("Running with agrs : "+Arrays.toString(args));
    SystemValidation me = new SystemValidation();
    if(args.length==0)
    {
      me.loadData("TEST");
    }
    else
    {
      me.loadData(args[0]);
    }
    jniBridge.deinitialize();
  }

}
