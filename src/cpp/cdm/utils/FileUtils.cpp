/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/utils/FileUtils.h"
#include <iterator>
#include <filesystem>

std::string Replace(const std::string& original, const std::string& replace, const std::string& withThis)
{
  size_t idx = 0;
  std::string s = original;
  idx = s.find(replace);
  if (idx != std::string::npos)
  {
    s.erase(idx, replace.length());
    s.insert(idx, withThis);
  }
  return s;
}

bool CreatePath(const std::string& path)
{
  if (path.empty())
  {
    return false;
  }

  // Recursively create parents
  auto const sep = path.find_last_of("\\/");
  if (sep != std::string::npos && sep > 0)
  {
    if (!CreatePath(path.substr(0, sep)))
    {
      return false;
    }
  }

  MakeDirectory(path.c_str());
  return true;
}

bool CreateFilePath(const std::string& filenamePath)
{
  // Separate path from file, and create the path
    auto const sep = filenamePath.find_last_of("\\/");
  if (sep != std::string::npos && sep > 0)
    return std::filesystem::create_directory(filenamePath.substr(0, sep));
  return true; // Nothing to do... 
}

bool WriteFile(const std::string& content, const std::string& filename)
{
  if (!CreateFilePath(filename))
    return false;
  std::ofstream ascii_ostream(filename, std::ios::out | std::ios::trunc);
  ascii_ostream << content;
  ascii_ostream.flush();
  ascii_ostream.close();
  return true;
}

bool ReadFile(const std::string& filename, std::string& content)
{
  if (!FileExists(filename))
    return false;

  std::ifstream input(filename);
  if (!input.is_open())
    return false;
  std::string buff((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
  input.close();
  content = buff;
  return true;
}

bool IsDirectory(const std::string& dir)
{
  return std::filesystem::is_directory(dir);
}

void ListFiles(const std::string& dir, std::vector<std::string>& files, bool recursive, const std::string& mask)
{
  std::string filename;
  if (recursive)
  {
    for (const auto& entry : std::filesystem::recursive_directory_iterator(dir))
    {
      if (entry.exists() && entry.is_regular_file())
      {
        filename = entry.path().filename().string();
        if (filename.find(mask) != std::string::npos)
          files.push_back(filename);
      }

    }
  }
  else
  {
    for (const auto& entry : std::filesystem::directory_iterator(dir))
    {
      if (entry.exists() && entry.is_regular_file())
      {
        filename = entry.path().filename().string();
        if (filename.find(mask) != std::string::npos)
          files.push_back(filename);
      }
      
    }
  }
}

void MakeDirectory(std::string const& dir)
{
  std::filesystem::create_directory(dir);
}

bool DeleteDirectory(const std::string &dir)
{
  return std::filesystem::remove_all(dir);
}

std::string GetCurrentWorkingDirectory()
{
  return std::filesystem::current_path().string();
}

bool FileExists(const std::string& filename)
{
  return std::filesystem::exists(filename);
}

bool IsJSONFile(const std::string& filename)
{
  size_t ext = filename.find_last_of(".");
  return filename.substr(ext) == ".json";
}

bool SplitFilenamePath(const std::string& filepath, std::string& filename)
{
  auto slash = filepath.find_last_of("/");
  (slash == std::string::npos) ?
    slash = 0 : slash++;
  filename = filepath.substr(slash);
  return true;
}
bool SplitFilenameExt(const std::string& filepath, std::string& filename, std::string& ext)
{
  SplitFilenamePath(filepath, filename);
  auto ePos = filename.find_last_of(".");
  if (ePos != std::string::npos)
  {
    ext = filename.substr(ePos);
    filename = filename.substr(0, ePos);
  }
  else
  {
    // No extension...
    ext = "";
  }
  return true;
}
