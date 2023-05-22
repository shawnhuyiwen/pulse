/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.dataset;

import java.io.File;
import java.io.FileInputStream;
import java.util.*;

import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.CellType;
import org.apache.poi.ss.usermodel.FormulaEvaluator;
import org.apache.poi.ss.usermodel.Row;
import org.apache.poi.xssf.usermodel.XSSFSheet;
import org.apache.poi.xssf.usermodel.XSSFWorkbook;

import com.kitware.pulse.cdm.bind.Engine.TimeSeriesValidationTargetData.eType;
import com.kitware.pulse.cdm.engine.SEDataRequest;
import com.kitware.pulse.cdm.engine.SEDataRequestManager;
import com.kitware.pulse.cdm.engine.SETimeSeriesValidationTarget;
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
      FileUtils.delete(new File("./validation/targets/"));
      FileUtils.delete(new File("./validation/requests/"));
      // Ok, let's make them again
      FileUtils.createDirectory("./validation/targets/");
      FileUtils.createDirectory("./validation/requests/");
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
        FormulaEvaluator evaluator  = xlWBook.getCreationHelper().createFormulaEvaluator();
        
        String system = "Cardiovascular";
        // TODO loop all the system/circuits we can optimize
        if(!readSheet(xlWBook.getSheet(system), evaluator))
        {
          Log.error("Unable to read "+system+" sheet");
        }
        
      }
    }
    catch(Exception ex)
    {
      Log.error("Error reading XSSF : "+xlsFile);
      Log.error(ex.getMessage());
      return;
    }
    Log.info("Data Generation Complete");
  }

  protected static boolean readSheet(XSSFSheet xlSheet, FormulaEvaluator evaluator)
  {
    List<Double> values = new ArrayList<Double>();
    Map<String, SEDataRequestManager> drMgrMap = new HashMap<String, SEDataRequestManager>();
    Map<String, List<SETimeSeriesValidationTarget>> targetMap = new HashMap<String, List<SETimeSeriesValidationTarget>>();

    try
    {
      String targetCategory;
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
        Cell   vCell     = row.getCell(5);
        targetCategory   = row.getCell(15).getStringCellValue().trim();
        if(targetCategory==null||targetCategory.isEmpty()||targetCategory.equals("ValidationTargetFile"))
          continue;
        
        targetCategory = xlSheet.getSheetName()+targetCategory;
        if(!drMgrMap.containsKey(targetCategory))
          drMgrMap.put(targetCategory, new SEDataRequestManager());
        if(!targetMap.containsKey(targetCategory))
          targetMap.put(targetCategory, new ArrayList<SETimeSeriesValidationTarget>());

        eType targetType;
        if(type.equals("Mean"))
          targetType = eType.Mean;
        else if(type.equals("Min"))
          targetType = eType.Minimum;
        else if(type.equals("Max"))
          targetType = eType.Maximum;
        else
        {
          Log.error("Unknown target type: "+type);
          return false;
        }
        

        SEDataRequestManager drMgr = drMgrMap.get(targetCategory);
        String[] propertySplit = property.split("-");
        SEDataRequest dr = drMgr.createLiquidCompartmentDataRequest(propertySplit[0].trim(), propertySplit[1].trim(), CommonUnits.getUnit(unit));
        
        List<SETimeSeriesValidationTarget> targets = targetMap.get(targetCategory);
        SETimeSeriesValidationTarget tgt = new SETimeSeriesValidationTarget();
        targets.add(tgt);
        tgt.setHeader(dr.toString());
        
        CellType ct = vCell.getCellType();
        if(ct == CellType.STRING)
        {
          values.clear();
          String sValues = vCell.getStringCellValue().trim();
          sValues = sValues.replace('[', ' ');
          sValues = sValues.replace(']', ' ');
          String[] split= sValues.split(",");
          for(String s : split)
            values.add(Double.parseDouble(s.trim()));
          tgt.setRange(
              DoubleUtils.getMin(values),
              DoubleUtils.getMax(values),
              targetType);
        }
        else if(ct == CellType.NUMERIC)
        {
          tgt.setRange(
              vCell.getNumericCellValue(),
              vCell.getNumericCellValue(),
              targetType);
        }
        else if(ct == CellType.FORMULA)
        {
          double cellValue = Double.NaN;
          switch(evaluator.evaluateFormulaCell(vCell))
          {
            case NUMERIC:
              cellValue = vCell.getNumericCellValue();
              break;
            default: // do nothing
          }
          tgt.setRange(
              cellValue,
              cellValue,
              targetType);
        }
        
        //System.out.println("Row "+r+": "+tgt);
      }
      
      for(String key : targetMap.keySet())
      {
        String drFileName =  "./validation/requests/"+key+".json";
        String tgtFileName = "./validation/targets/"+key+".json";
        Log.info("Writing : "+drFileName);
        drMgrMap.get(key).writeFile(drFileName);
        Log.info("Writing : "+tgtFileName);
        SETimeSeriesValidationTarget.writeFile(targetMap.get(key),tgtFileName);
      }
    }
    catch(Exception ex)
    {
      Log.error("Error reading XLS");
      Log.error(ex.getMessage());
      return false;
    }   
    return true;
  }
}
