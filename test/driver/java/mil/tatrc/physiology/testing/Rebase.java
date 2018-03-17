/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package mil.tatrc.physiology.testing;

import java.io.*;
import java.util.*;
import java.text.SimpleDateFormat;

import org.eclipse.jgit.internal.storage.file.FileRepository;
import org.eclipse.jgit.lib.ObjectId;

import mil.tatrc.physiology.utilities.FileUtils;
import mil.tatrc.physiology.utilities.Log;
import mil.tatrc.physiology.utilities.RunConfiguration;
import mil.tatrc.physiology.utilities.UnitConverter;

public class Rebase
{
	public static void main(String[] args)
	{
	  UnitConverter.initialize("./");
		RunConfiguration cfg = new RunConfiguration();
		Log.setFileName("Rebase.log");    
		String toDir = "./test_results/rebase/";
		List<String> configFiles = new ArrayList<String>();
		try
		{
			File file = new File(cfg.getTestConfigDirectory()+"/Rebase.config");
			FileReader fileReader = new FileReader(file);
			BufferedReader bufferedReader = new BufferedReader(fileReader);
			String line;
			while ((line = bufferedReader.readLine()) != null) 
			{
				line = line.trim();
				if(line.indexOf("#")>-1)
					continue;
				if(line.indexOf(".config")==-1)
						continue;
				configFiles.add(line);
			}
			fileReader.close();
		}
		catch(IOException ex)
		{
			Log.error("Unable to open Rebase.config, check your run.config file test_config_dir");
			return;
		}
		
		// Replace back slashes with forward slashes
		toDir = toDir.replaceAll("\\\\", "/");
		toDir += new SimpleDateFormat("yyyy-MM-dd").format(new Date());
		// Ask git what the current revision hash is
		try
		{
			FileRepository repo = new FileRepository(cfg.getRootDirectory()+"/.git");
			ObjectId head = repo.resolve("HEAD");
			if(head!=null)
				toDir += " - "+head.getName().substring(0,8);
			repo.close();
		}
		catch(IOException ex)
		{
			Log.error("Unable to read git hash, check your run.config file root_dir");
		}
		toDir += "/";

		Log.info("Rebasing to "+toDir);

		// Make sure from/to dir ends with a slash
		if(!toDir.endsWith("/"))
			toDir = toDir + "/";
		try
		{
			FileUtils.delete(toDir);
			FileUtils.createDirectory(toDir);
		}
		catch(Exception ex)
		{
			Log.error("Could not create to Directory : "+toDir,ex);
			return;
		}

		String log_file;
		String scenario_file;
		String result_path;
		List<String> result_files = new ArrayList<String>();

		for(String configFile : configFiles)
		{
			Log.info("Rebasing config "+configFile);
			SETestConfiguration config = new SETestConfiguration();
			config.processConfigFile(new File(cfg.getTestConfigDirectory()+"/"+configFile), cfg);

			for(SETestJob job : config.jobs)
			{
				if(job.computedFiles.size()<=0)
				{
					Log.error("A job has no expected result files!?!?");
					continue;
				}
				if(!job.PlottableResults)
					continue;// Nothing to plot/compare, nothing to zip
				
				result_files.clear();
				for(String j : job.computedFiles)
				{
				  result_files.add(j.replaceAll(".csv", ".txt"));
				}
				job.computedFiles.clear();
				job.computedFiles.addAll(result_files);
				
				
				log_file = "";
				scenario_file = "";
				result_files.clear();
				result_path = job.computedFiles.get(0);
				if(!new File(result_path).exists())
				{
				  Log.error("Unable to find file to rebase for "+job.name+" at path "+result_path);
				  continue;
				}

				if(job.name.endsWith(".pba"))
				{
					scenario_file = job.baselineDirectory+job.name;
					if(job.computedFiles.size()>1)
					{
						Log.error("A scenario file has more than one result file!?!?");
						continue;
					}
					result_files.add(result_path);
					log_file = result_files.get(0).replaceAll("Results"+SETestConfiguration.ext, ".log");   
					result_files.add(log_file);
					result_files.add(scenario_file);
				}    	
				else
				{
					// Its a unit test, only zip up the results
					result_files.addAll(job.computedFiles);
				}

				String prepend = result_path.substring(result_path.indexOf("test_results/")+13,result_path.lastIndexOf("."));
				int ridx = prepend.indexOf("Results");
				if(ridx>-1)
					prepend = prepend.substring(0,ridx);
				String newFile = toDir + prepend + ".zip";

				Log.info("Creating new file "+newFile);
				String rPath = "";
				for(String rDir : newFile.split("/"))
				{
					if(rDir.endsWith(".zip"))
					{
						break;
					}
					rPath += rDir + "/";
				}
				try
				{
					FileUtils.createDirectory(rPath);
				} 
				catch (IOException ex)
				{
					Log.error("Could not create directory : "+rPath, ex);
				}
				File oldZip = new File(newFile);
				if(oldZip.exists())
					oldZip.delete();
				String[] files = new String[result_files.size()];
				files = result_files.toArray(files);
				FileUtils.zipFiles(files, newFile,prepend+".zip");
			}
		}
	}

}
