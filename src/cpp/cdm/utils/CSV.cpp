/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "CSV.h"
#include "properties/SEScalar.h"

#include <algorithm>
#include <fstream>
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
