/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.dataset;

import java.io.File;
import java.io.FileInputStream;
import java.util.*;

import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.CellType;
import org.apache.poi.ss.usermodel.Row;
import org.apache.poi.xssf.usermodel.XSSFSheet;
import org.apache.poi.xssf.usermodel.XSSFWorkbook;

import com.kitware.pulse.cdm.bind.Engine.ValidationTargetData.eType;
import com.kitware.pulse.cdm.datarequests.SEDataRequestManager;
import com.kitware.pulse.cdm.datarequests.SEValidationTarget;
import com.kitware.pulse.cdm.properties.CommonUnits;
import com.kitware.pulse.utilities.DoubleUtils;
import com.kitware.pulse.utilities.FileUtils;
import com.kitware.pulse.utilities.Log;
import com.kitware.pulse.utilities.RunConfiguration;
import com.kitware.pulse.utilities.JNIBridge;

public class ValidationDataSetReader
{
  private static  RunConfiguration cfg;

  public static void main(String[] args)
  {
    JNIBridge.initialize();
    cfg = new RunConfiguration();
    loadData(cfg.getDataDirectory()+"/human/adult/validation/SystemValidationData.xlsx");
  }

  public static void loadData(String xlsFile)
  {
    try
    {
      // Delete current dir contents
      FileUtils.delete(new File("./optimizer/"));
      // Ok, let's make them again
      FileUtils.createDirectory("./optimizer/");
    }
    catch(Exception ex)
    {
      Log.error("Unable to clean directories");
      return;
    }
    
    try
    {   
      File xls = new File(xlsFile);
      if(!xls.exists())
      {
        Log.error("Could not find xls file "+xlsFile);
        return;
      }
      Log.info("Generating data from "+xlsFile);
      FileInputStream xlFile = new FileInputStream(xlsFile);
      try(XSSFWorkbook xlWBook =  new XSSFWorkbook (xlFile))
      {
        String system = "Cardiovascular";
        // TODO loop all the system/circuits we can optimize
        if(!readSheet(xlWBook.getSheet(system)))
        {
          Log.error("Unable to read "+system+" sheet");
        }
        
      }
    }
    catch(Exception ex)
    {
      Log.error("Error reading XSSF : "+xlsFile,ex);
      return;
    }
    Log.info("Data Generation Complete");
  }

  protected static boolean readSheet(XSSFSheet xlSheet)
  {
    List<Double> values = new ArrayList<Double>();
    Map<String, SEDataRequestManager> targets = new HashMap<String, SEDataRequestManager>();

    try
    {
      int rows = xlSheet.getPhysicalNumberOfRows();
      for (int r = 0; r < rows; r++) 
      {
        Row row = xlSheet.getRow(r);
        if (row == null || row.getPhysicalNumberOfCells() <= 1) 
          continue;
        
        String property  = row.getCell(0).getStringCellValue();
        if(property==null||property.isEmpty())
          continue;
        
        String unit      = row.getCell(1).getStringCellValue().trim();
        String type      = row.getCell(2).getStringCellValue().trim();
        Cell   vCell     = row.getCell(3);
        String optimizer = row.getCell(12).getStringCellValue().trim();
        if(optimizer==null||optimizer.isEmpty()||optimizer.equals("OptimizerTargets"))
          continue;
        
        if(!targets.containsKey(optimizer))
          targets.put(optimizer, new SEDataRequestManager());
        SEDataRequestManager drMgr = targets.get(optimizer);
        
        String[] propertySplit = property.split("-");
        SEValidationTarget tgt =
            drMgr.createLiquidCompartmentValidationTarget(propertySplit[0].trim(), propertySplit[1].trim(), CommonUnits.getUnit(unit));
        tgt.type = eType.valueOf(type);
        
        if(vCell.getCellType() == CellType.STRING)
        {
          values.clear();
          String sValues = vCell.getStringCellValue().trim();
          sValues = sValues.replace('[', ' ');
          sValues = sValues.replace(']', ' ');
          String[] split= sValues.split(",");
          for(String s : split)
            values.add(Double.parseDouble(s.trim()));
          tgt.rangeMax = DoubleUtils.getMax(values);
          tgt.rangeMin = DoubleUtils.getMin(values);
        }
        else if(vCell.getCellType() == CellType.NUMERIC)
        {
          tgt.rangeMax = vCell.getNumericCellValue();
          tgt.rangeMin = vCell.getNumericCellValue();
        }
        
        //System.out.println("Row "+r+": "+tgt);
      }
      
      for(String optimizer : targets.keySet())
      {
        String fileName = "./optimizer/"+optimizer+".json";
        Log.info("Writing : "+fileName);
        targets.get(optimizer).writeFile(fileName);
      }
    }
    catch(Exception ex)
    {
      Log.error("Error reading XLS",ex);
      return false;
    }   
    return true;
  }
}
