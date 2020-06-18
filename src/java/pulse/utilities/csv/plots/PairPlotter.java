package pulse.utilities.csv.plots;

import java.util.List;

import pulse.utilities.FileUtils;
import pulse.utilities.Log;

public class PairPlotter
{

  // Recursively search a directory for files name 'pair_plots.config'
  // And plot each line 
  public static void main(String[] args)
  {
    String root = "./";
    if(args.length > 0)
      root = args[0];

    List<String>plots = FileUtils.findFiles(root, "plot_pairs.config", true);
    for(String plot_file : plots)
    {
      Log.info("Processing plot file "+plot_file);
      String[] lines = FileUtils.readFile(plot_file).split("\r\n");
      for(String line : lines)
      {
        String[] pair = line.split(",");
        pair[0] = pair[0].trim();
        pair[1] = pair[1].trim();
        PlotDriver.main(pair);
      }
    }
  }
}
