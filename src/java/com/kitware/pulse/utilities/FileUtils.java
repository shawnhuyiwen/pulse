/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.utilities;


import static java.nio.file.StandardCopyOption.REPLACE_EXISTING;

import java.io.*;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.*;

import org.apache.commons.compress.archivers.ArchiveEntry;
import org.apache.commons.compress.archivers.ArchiveInputStream;
import org.apache.commons.compress.archivers.ArchiveOutputStream;
import org.apache.commons.compress.archivers.zip.ZipArchiveInputStream;
import org.apache.commons.compress.archivers.zip.ZipArchiveOutputStream;
import org.apache.commons.compress.utils.IOUtils;

/**
 * The <code>FileUtils</code> contains support for writing and exporting files
 * 
 * @author byoung
 */
public class FileUtils
{
  public static void main(String[] args)
  {
    for(String file : FileUtils.findFiles("./new_baselines", ".csv", false))
      FileUtils.zipFiles(new String[]{file}, file.replaceAll(".csv", ".zip"));
  }

  public static boolean loadLibraries(List<String> libs, String location)
  {
    boolean b = true;
    for (String lib : libs)
      b &= loadLibrary(lib, location);
    return b;
  }
  public static boolean loadLibraries(List<String> libs)
  {
    return loadLibraries(libs,System.getProperty("user.dir"));
  }
  public static boolean loadLibrary(String libName, String location)
  {
    Log.info("Loading native library : "+libName);
    // See if we can get the library from the JVM/java.library.path
    try
    {
      System.loadLibrary(libName);
      return true;
    }
    catch(UnsatisfiedLinkError ex)
    {
      // Ok, its not on the java.library.path,
      // Let's try to look for it in the location provided
    }

    if(location==null || location.isEmpty())
      location = System.getProperty("user.dir");
    Log.info("Could not find library on path, looking for library in "+location);
    // With out '.' we cant tell diff between libName.dll and libNameXYZ.dll
    List<String> files = FileUtils.findFiles(location, libName+".", true);
    for(String file : files)
    {
      Log.info("Found "+file);
      if(file.equals(libName+".dll") ||
         file.equals(libName+".so")  ||
         file.equals(libName+".dylib"))
      {
        try
        {
          Log.info("Loading "+file);
          System.load(file);
          return true;
        }
        catch(UnsatisfiedLinkError ex)
        {
          // Keep looking...
          Log.error("Could not load "+file);
        }
      }
    }
    return false;
  }

  /** 
   * Tests if a file can be created with this name on the OS
   * @param fileName
   * @return
   */
  public static boolean validFileName(String fileName)
  {
    try
    {
      File f = new File(fileName);
      f.createNewFile();
      if(f.exists())
      {
        f.delete();
        return true;
      }
      else
        return false;
    }
    catch(IOException e)
    {
      Log.error("Filename invalid: "+fileName,e);
      return false;
    }
  }

  /**
   * Writes from a string into a file
   *
   * @param fileName File name to write to 
   * @param content String to write to file
   *
   * @return True if writing was successful, false if error occurred
   */
  public static boolean writeFile(String fileName, String content)
  {
    FileWriter writer = null;

    try
    {
      writer = new FileWriter(fileName);
      writer.write(content);

      return true;
    }
    catch (Exception e)
    {
      Log.error(e);
      return false;
    }
    finally
    {
      if (writer != null)
      {
        try
        {
          writer.close();
        }
        catch (Exception e1)
        {
          Log.error(e1);
          return false;
        }
      }
    }
  }

  /**
   * Writes the specified byte[] to the specified File path.
   * 
   * @param theFile File Object representing the path to write to.
   * @param bytes The byte[] of data to write to the File.
   * @throws IOException Thrown if there is problem creating or writing the 
   * File.
   */
  public static void writeBytesToFile(File theFile, byte[] bytes) throws IOException 
  {
    BufferedOutputStream bos = null;
    try 
    {
      FileOutputStream fos = new FileOutputStream(theFile);
      bos = new BufferedOutputStream(fos); 
      bos.write(bytes);
    }
    finally 
    {
      if(bos != null) 
      {
        try  
        {
          //flush and close the BufferedOutputStream
          bos.flush();
          bos.close();
        } catch(Exception e){}
      }
    }
  }

  public static String readFile(String fileName)
  {
    File f = new File(fileName);
    return readFile(f);
  }

  /**
   * Reads from a file into a String.
   *
   * @param fileName File name to read from
   *
   * @return String containing file contents, or null if there was an error
   */
  public static String readFile(File f)
  {
    FileReader reader = null;
    try
    {
      reader = new FileReader(f);
      char[] buffer = new char[(int) f.length()];
      reader.read(buffer);
      return new String(buffer);
    }
    catch (Exception e)
    {
      Log.error("Could not read file " + f.getAbsolutePath(),e);
      return null;
    }
    finally
    {
      if (reader != null)
      {
        try
        {
          reader.close();
        }
        catch (Exception e1)
        {
        }
      }
    }
  }

  /** 
   * Read bytes from a File into a byte[].
   * 
   * @param file The File to read.
   * @return A byte[] containing the contents of the File.
   * @throws IOException Thrown if the File is too long to read or couldn't be
   * read fully.
   */
  public static byte[] readBytesFromFile(File file) throws IOException 
  {
    try(InputStream is = new FileInputStream(file)) {

      // Get the size of the file
      long length = file.length();
  
      // You cannot create an array using a long type.
      // It needs to be an int type.
      // Before converting to an int type, check
      // to ensure that file is not larger than Integer.MAX_VALUE.
      if (length > Integer.MAX_VALUE) 
      {
        throw new IOException("Could not completely read file " + file.getName() + " as it is too long (" + length + " bytes, max supported " + Integer.MAX_VALUE + ")");
      }
  
      // Create the byte array to hold the data
      byte[] bytes = new byte[(int)length];
  
      // Read in the bytes
      int offset = 0;
      int numRead = 0;
      while (offset < bytes.length && (numRead=is.read(bytes, offset, bytes.length-offset)) >= 0) 
      {
        offset += numRead;
      }
  
      // Ensure all the bytes have been read in
      if (offset < bytes.length) 
      {
        throw new IOException("Could not completely read file " + file.getName());
      }
  
      return bytes;
    }
  }

  /**
   * Creates the directory to this file and returns the File object with the proper name
   * that can then be created.
   * Does not create a temporary file, just makes sure a file can be made where asked and
   * creates the name.
   * @param directory
   * @param filename
   * @param extension
   * @param classification
   * @return
   * @throws IOException
   */
  public synchronized static File createFile(String directory, String filename, String extension, String classification) throws IOException
  {


    //get file path
    StringBuilder sb = new StringBuilder();    
    if(directory != null && directory.length() > 0)
    {
      sb.append("\\");
      sb.append(directory);
    }         
    String path = sb.toString();

    //if path doesn't exist, create it
    createDirectory(path);

    //create file and return it
    return new File(path+"\\"+filename+extension);  
  }


  /**
   * write the specified content to the IWMDT export folder. The filename is 
   * prepended with the specified short marking
   * 

   * @param directory
   * @param filename
   * @param ext
   * @param marking
   * @param content
   * @return File as written on disk
   */
  public synchronized static File createFile(String directory, String filename, String ext, String classification, String content)
  { 
    File file = null;

    if(content == null || filename == null || ext == null || classification == null)
    {
      Log.error("Unable to export null content");
      return file;
    }             

    try
    {     
      //create file
      file = createFile(directory, filename, ext, classification);

      //write file contents
      PrintWriter out = new PrintWriter(new FileWriter(file));
      out.println(content);
      out.close();
    }
    catch (Exception e)
    {
      Log.error("Unable to export file: " + filename, e);
    }  

    return file;
  }

  /**
   * Create directory if it doesn't already exist
   * 
   * @param path
   * @return
   * @throws IOException
   */
  public synchronized static File createDirectory(String path) throws IOException
  {
    //TimingProfile.profile(LogFile, "createDirectory:Enter");
    File dirFile = new File(path);
    int tries=0;
    while(tries<10)
    {
      if(!dirFile.exists() && !dirFile.mkdirs())
      {
        tries++;
        try
        {Thread.sleep(1000);} 
        catch (InterruptedException e){}
      }
      else 
        break;
    }
    if(!dirFile.exists() && !dirFile.mkdirs())
      throw new IOException("Unable to create directory: " + path);
    //TimingProfile.profile(LogFile, "createDirectory:Exit");
    return dirFile;
  }

  public synchronized static void copy(File source, File destination) throws IOException
  {
    if (source.isDirectory())
    {
      destination.mkdirs();

      for (File f : source.listFiles())
      {
        copy(f, new File(destination, f.getName()));
      }
    }
    else
    {
      Files.copy(source.toPath(), destination.toPath(), REPLACE_EXISTING);
    }
  }

  /**
   * @param directory
   * @return
   */
  public synchronized static void changeDirectoryName(String directory, String newName)
  {
    File dir = new File(directory);
    File newDir = new File(newName);    
    dir.renameTo(newDir);
  }


  public static boolean delete(String fileName)
  {
    return delete(new File(fileName));      
  }

  public static boolean delete(File f)
  {
    if (f.isDirectory())
    {
      for (File c : f.listFiles())
      {
        delete(c);
      }
    }
    return f.delete();      
  }


  /**
   * Clean up - delete unzipped files that have already been included into ZIP
   */
  public static void deleteFiles(String[] filenames)
  {
    File file;
    if(filenames == null)
      return;
    for(int i =0; i<filenames.length;i++)
    {
      file = new File(filenames[i]);
      if(file.exists())
        file.delete();
    }
  }

  /**
   * Delete files in a specified directory
   * @param fileNames
   */
  public static void deleteFiles(String[] fileNames, String directory)
  {
    File dir = new File(directory);

    //delete files
    for(int i =0; i<fileNames.length;i++)
    {
      dir = new File(directory + "/" + fileNames[i]);
      if(dir.exists())
        dir.delete();
    }    
  }

  /**
   * Utility to see if path for directory exists and is a directory
   * @param filepath
   * @return
   */
  public static boolean directoryExists(String filepath)
  {
    File file = new File(filepath);
    if(file.exists())
      return file.isDirectory();
    return false;
  }

  /**
   * Utility to see if path for file exists and is a file 
   * @param string path
   * @return
   */
  public static boolean fileExists(String filepath)
  {
    File file = new File(filepath);
    return fileExists(file);
  }

  /**
   * Utility to see if path for file exists and is a file 
   * @param file
   * @return
   */
  public static boolean fileExists(File f)
  {
    if(f.exists())
      return f.isFile();
    return false;
  }

  /**
   * Search for files that contain a search string
   * in the provided directory
   * @param directory
   * @param search
   * @param recursive
   * @return
   */
  public static List<String> findFiles(String directory, String search, boolean recursive)
  {
    List<String> files = new ArrayList<>(); 
    File dir = new File(directory);
    if(!dir.isDirectory())
    {
      Log.warn("Cannot find files, "+directory+" is not a directory");
      return files;
    }
    findFiles(dir,search,recursive,files);
    return files;
  }

  private static void findFiles(File directory, String search, boolean recursive, List<String> files)
  {
    for(File file : directory.listFiles())
    {
      if(file.isDirectory())
      {
        if(recursive)
          findFiles(file,search,recursive,files);
        else
          continue;
      }
      if(file.getName().indexOf(search)>-1)
        files.add(file.getAbsolutePath());
    }
  }

  public static String getAllButFirstFile(String url)
  {
    String s = url;
    int i = s.indexOf('/');
    if (i >= 0) s = s.substring(i+1, url.length());
    return s;
  }

  public static long getFileSize(String fileName)
  {
    int size=0;
    File f = new File(fileName);
    if(!f.exists())
      return size;
    return f.length();
  }

  public static String removeFilePrefix(String url)
  {
    String s = url.replace(File.separatorChar, '/');
    if(s.startsWith("file:/"))
    {
      return getAllButFirstFile(s);
    }
    else
    {
      return s;
    }
  }

  /**
   * @param string
   * @return
   */
  public static String stripTrailingSlash(String string)
  {
    string = string.replace(File.separatorChar, '/');
    string = stripTrailingChar(string, '/');
    return string;
  }

  /**
   * Strip trailing character from string, if it ends with it
   * 
   * @param string
   * @param toRemove
   * @return
   */
  public static String stripTrailingChar(String string, char toRemove)
  {
    if(string.charAt(string.length()-1) == toRemove)
      return string.substring(0, string.length()-1);
    else
      return string;  
  }

  public static String stripTrailingChars(String string, char toRemove)
  {
    while(string.charAt(string.length()-1) == toRemove)
    {
      string = stripTrailingChar(string, toRemove);
    }

    return string;
  }

  /**
   * Strip extension from string
   * 
   * @param string
   * @return
   */
  public static String stripExtension(String string)
  {
    String s = string;
    int i = s.lastIndexOf('.');
    if (i >= 0) s = s.substring(0,i);
    return s; 
  }


  /**
   * Get all but the last file component of the URL
   * 
   * @param url URL to read
   * @return all but the last file component
   */
  public static String getAllButLastFile (String url)
  {
    String s = url;
    int i = s.lastIndexOf('/');
    if (i >= 0) s = s.substring(0, i);
    return s;
  }


  /**
   * Get the last file component of the URL
   * 
   * @param url URL to read
   * @return last file component
   */
  public static String getLastFile (String url)
  {
    String s = url;
    int i = s.lastIndexOf('/');
    if (i >= 0) s = s.substring(i+1);
    return s; 
  }

  /**
   * If the specified filename doesnt include a path, prepend the specified directory
   * 
   * @param file
   * @param path
   * @return
   */
  public static String fixupFilepath(String file, String path)
  {
    if(file == null)
      return null;

    //check if it is a filename or filepath
    if(file.indexOf('/') >= 0 || file.indexOf("/") >= 0)
      return file;

    if(path == null)
      return file;

    return path + "\\" + file;
  }


  /**
   * @param path
   * @return
   */
  public static String addTrailingSlash(String path)
  {
    if(path == null)
      return "\\";

    if(!path.endsWith("\\") && !path.endsWith("/"))
    {
      path += "\\";
    }

    return path;
  }

  /**
   * Removed Windows reserved filename characters, and optionally remove spaces
   *  http://msdn.microsoft.com/en-us/library/aa365247%28VS.85%29.aspx
   *  
   *  < (less than)
   * > (greater than)
   * : (colon)
   * " (double quote)
   * / (forward slash)
   * \ (backslash)
   * | (vertical bar or pipe)
   * ? (question mark)
   * * (asterisk)
   * 
   * @param filename
   * @param removeSpace
   * @return
   */
  public static String fixupFileName(String filename, boolean removeSpace)
  {
    if(removeSpace)
      filename = filename.replace(' ', '_');
    filename = filename.replace('<', '_');
    filename = filename.replace('>', '_');
    filename = filename.replace(':', '_');
    filename = filename.replace('"', '_');
    filename = filename.replace('/', '_');
    filename = filename.replace('\\', '_');
    filename = filename.replace('|', '_');
    filename = filename.replace('*', '_');
    return filename.replace('?', '_');    
  }


  public static String[] getFilesOfSameName(String filename)
  {
    File file = new File("./");

    FilenameFilter filenameFilter = new FileNameFilter(filename);
    return file.list(filenameFilter);
  }

  public static String[] getFilesOfSameName(String filename, String...ignoreExtensions)
  {
    File file = new File("./");

    FilenameFilter filenameFilter = new FileNameFilter(filename, ignoreExtensions);
    return file.list(filenameFilter);
  }
  
  /**
  * Zips all files in fileNames array
  * @param fileNames
  * @param zipFileName
  * @return
  */
  public static void zipFiles(String[] fileNames, String zipFileName)
  {
   	zipFiles(fileNames,zipFileName,null);
  }

  /**
   * Zips all files in fileNames array
   * @param fileNames
   * @param zipFileName
   * @param zipPath directory structure to put in the zip
   * @return
   */
  public static void zipFiles(String[] fileNames, String zipFileName, String zipPath)
  {
    try
    {
      new File(zipFileName).delete();
      OutputStream fo = Files.newOutputStream(Paths.get(zipFileName));
      ArchiveOutputStream o = new ZipArchiveOutputStream(fo);

      for (String fileName : fileNames)
      {
        String path;
        File file = new File(fileName);
        if (!file.isFile())
          continue;
        if(fileName.startsWith("./"))
          fileName = fileName.substring(2);
        
        fileName = fileName.substring(fileName.lastIndexOf("/")+1);
        if(zipPath==null||zipPath.isEmpty())
        {
          path = fileName.substring(0,fileName.lastIndexOf("/")+1);
        }
        else
        {
          path = zipPath.substring(0,zipPath.lastIndexOf("/")+1);
        }
        ArchiveEntry entry = o.createArchiveEntry(file, path+fileName);
        // potentially add more flags to entry
        o.putArchiveEntry(entry);
        try (InputStream i = Files.newInputStream(file.toPath()))
        {
            IOUtils.copy(i, o);
        }
        o.closeArchiveEntry();
      }
      o.finish();
    }
    catch (IOException e) 
    {
      Log.error("Error compressing files to " + zipFileName + "\n", e);
    }
  }

  /**
   * Zip files of filename.*
   * @param filename
   * @return - the files that are included in the zip
   */
  public static String[] zipFilesOfSameName(String filename, String[] excludeExtensions)
  {
    String[] files;
    File file = new File("./");

    FilenameFilter filenameFilter = new FileNameFilter(filename, excludeExtensions);
    files = file.list(filenameFilter);
    if(files.length == 0)
      return null;      
    zipFiles(files, filename);

    return files;
  }

  public static void zipDirectoryContents(File srcDir, File tgtDir, String extension) throws IOException
  {
    for(String l : srcDir.list())
    {
      File s = new File(srcDir.getAbsolutePath()+"\\"+l);
      File t = new File(tgtDir.getAbsolutePath()+"\\"+l);

      if(s.isDirectory())
      {
        if(!t.exists())
          t.mkdir();
        zipDirectoryContents(s,t,extension);
      }
      else if(s.isFile())
      {
        if(s.getAbsolutePath().endsWith(extension))
        {
          String zipTo = t.getAbsolutePath().substring(0, t.getAbsolutePath().length()-4);
          FileUtils.zipFiles(new String[]{s.getAbsolutePath()}, zipTo);
        }
      }
    }
  }

  /**
   * Unzips specified directory
   * @param zipfileName (name includes .zip)
   * @return
   */
  public static void unzip(String zipFileName)
  {
    File f = new File(zipFileName);
    unzip(f);
  }

  public static void unzipToDirectory(String zipFileName, String toDirectory)
  {
    File f = new File(zipFileName);
    unzipToDirectory(f,toDirectory);
  }

  public static void unzip(File f)
  {
    unzipToDirectory(f,"./");
  }

  public static void unzipToDirectory(File f, String toDirectory)
  {
    try
    {
      InputStream fi = Files.newInputStream(Paths.get(f.getPath()));
      ArchiveInputStream i = new ZipArchiveInputStream(fi);

      ArchiveEntry entry = null;
      while ((entry = i.getNextEntry()) != null)
      {
        if (!i.canReadEntryData(entry))
        {
          // log something?
          continue;
        }
        String name = toDirectory+"/"+entry;
        File file = new File(name);
        if (entry.isDirectory())
        {
          if (!file.isDirectory() && !file.mkdirs())
          {
            throw new IOException("failed to create directory " + f);
          }
        }
        else
        {
          File parent = file.getParentFile();
          if (!parent.isDirectory() && !parent.mkdirs()) {
            throw new IOException("failed to create directory " + parent);
          }
          try (OutputStream o = Files.newOutputStream(file.toPath()))
          {
            IOUtils.copy(i, o);
          }
        }
      }
    }
    catch (Exception e)
    {
      Log.error(e.getMessage());
    }
  }

  // BLOCK_SIZE should probably a factor or multiple of the size of a disk sector/cluster.
  // your max heap size may need to be adjusted if you have a very big block size or many file comparisons.
  private final static int BLOCK_SIZE = 4096; //4k //65536;
  public static boolean compareBinaryFiles(String filename1, String filename2)
  {
    File file1 = new File(filename1);
    File file2 = new File(filename2);

    if (!file1.exists() && !file2.exists())
      return false;

    if (!file1.isFile() && !file2.isFile())
      return false;

    boolean compare = true;
    InputStream inputStream1 = null;
    InputStream inputStream2 = null;

    try
    {
      if (file1.getCanonicalPath().equals(file2.getCanonicalPath()))
        return true;

      if (file1.length() != file2.length())
        return false;

      inputStream1 = new FileInputStream(file1);
      inputStream2 = new FileInputStream(file2);

      // assume inputStream1 and inputStream2 are streams from your two files.
      byte[] streamBlock1 = new byte[BLOCK_SIZE];
      byte[] streamBlock2 = new byte[BLOCK_SIZE];
      Integer bytesRead1 = -1;
      Integer bytesRead2 = -2;
      do
      {
        bytesRead1 = inputStream1.read(streamBlock1);
        bytesRead2 = inputStream2.read(streamBlock2);
        compare = ((bytesRead1 == bytesRead2) && streamBlock1.equals(streamBlock2));
      }
      while (compare && (bytesRead1 > -1));
    }
    catch (Exception e)
    {
      Log.error(e.getMessage());
      compare = false;
    }
    finally
    {
      try
      {
        if (inputStream1 != null)
          inputStream1.close();
        if (inputStream2 != null)
          inputStream2.close();
      }
      catch(Exception e)
      {
        // error closing the inputStreams shouldn't change compare
        Log.error(e.getMessage());
      }
    }

    return compare;
  }
}
