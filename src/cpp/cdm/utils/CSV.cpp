/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/utils/CSV.h"
#include "cdm/utils/FileUtils.h"
#include "cdm/properties/SEScalar.h"

#include <algorithm>
#include <fstream>
#include <iterator>
#include <sstream>

#include <cctype>

// ----------------------------------------------------------------------------
std::string trim(std::string const& in)
{
  if (in.empty())
  {
    return {};
  }

  int (*isspace)(int) = &std::isspace;
  auto const begin = std::find_if_not(in.begin(), in.end(), isspace);
  if (begin == in.end())
  {
    return {};
  }

  auto end = std::find_if_not(in.rbegin(), in.rend(), isspace);
  return {begin, end.base()};
}

// ----------------------------------------------------------------------------
class CSV::Private
{
public:
  Private(std::string const& path)
    : stream{path}
  {
  }

  std::ifstream stream;
  std::stringstream record;
};

// ----------------------------------------------------------------------------
CSV::CSV(std::string const& path)
  : m_p{new Private{path}}
{
}

// ----------------------------------------------------------------------------
CSV::CSV(CSV&& other)
  : m_p{std::move(other.m_p)}
{
}

// ----------------------------------------------------------------------------
CSV::~CSV()
{
}

// ----------------------------------------------------------------------------
bool CSV::NextRecord()
{
  auto buffer = std::string{};
  for (;;)
  {
    // Read next line from stream
    if (!std::getline(m_p->stream, buffer))
    {
      return false;
    }
    buffer = trim(buffer);

    // Skip blank lines and comments
    if (buffer.empty() || buffer[0] == '#')
    {
      continue;
    }

    // Reset record stream
    m_p->record = std::stringstream{buffer};
    return true;
  }
}

// ----------------------------------------------------------------------------
std::string CSV::NextValue(unsigned skip)
{
  for (;;)
  {
    auto out = std::string{};
    if (!std::getline(m_p->record, out, ','))
      out = "";
    if (skip)
    {
      --skip;
      continue;
    }
    return trim(out);
  }

  return "";
}

// ----------------------------------------------------------------------------
double CSV::NextValueAsDouble(unsigned skip)
{
  std::string v = NextValue(skip);
  if (!v.empty())
  {
    auto p = static_cast<char*>(nullptr);
    auto const out = std::strtod(v.c_str(), &p);
    if (*p == 0) // All characters were consumed
    {
      return out;
    }
  }
  return SEScalar::dNaN();
}

// ----------------------------------------------------------------------------
std::string CSV::Record() const
{
  return m_p->record.str();
}

// ----------------------------------------------------------------------------
void CSV::SplitCSV(const std::string& originalCSV, const std::map<std::string, std::vector<std::string>>& fileMapping)
{
  //Determine results directory
  auto slash = originalCSV.find_last_of("/");
  std::string dir = "./";
  if(slash != std::string::npos)
  {
    dir = originalCSV.substr(0,++slash);
  }

  //Create result files
  std::map<std::string, std::ofstream> outputFiles;
  std::map<std::string, std::vector<std::string>>::const_iterator itr;
  for(itr = fileMapping.begin(); itr != fileMapping.end(); ++itr)
  {
    outputFiles.insert(std::pair<std::string, std::ofstream>(itr->first, std::ofstream(dir + itr->first)));
  }

  //Extract each header
  std::ifstream origFile(originalCSV);
  std::string line, header;
  std::getline(origFile, line);
  std::stringstream ss(line);
  std::vector<std::vector<std::string>> headersToFiles;
  while(std::getline(ss, header, ','))
  {
    headersToFiles.push_back(std::vector<std::string>());

    //Determine which files this header belongs to
    for(itr = fileMapping.begin(); itr != fileMapping.end(); ++itr)
    {
      //The first column (time) goes to every file
      if(headersToFiles.size() == 1 || std::find(itr->second.begin(), itr->second.end(), header) != itr->second.end())
      {
        //Add to headers to files mapping
        headersToFiles.back().push_back(itr->first);

        //Print headers
        outputFiles[itr->first] << header << ",";
      }
    }
  }

  //Read data, line by line
  std::map<std::string, std::ofstream>::iterator outputFileItr;
  while(std::getline(origFile, line))
  {
    //New line for every file
    for(outputFileItr = outputFiles.begin(); outputFileItr != outputFiles.end(); ++outputFileItr)
    {
      outputFileItr->second << std::endl;
    }

    std::stringstream ss(line);
    int colIdx = 0;
    std::string value;
    while(std::getline(ss, value, ','))
    {
      //Put this value in the relevant files
      for(auto file: headersToFiles[colIdx])
      {
        outputFiles[file] << value << ",";
      }

      ++colIdx;
    }
  }

  //Close files
  origFile.close();
  for(outputFileItr = outputFiles.begin(); outputFileItr != outputFiles.end(); ++outputFileItr)
  {
    outputFileItr->second.close();
  }
}
