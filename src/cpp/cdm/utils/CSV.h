/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include <memory>
#include <string>

// ----------------------------------------------------------------------------
/// Utility class to read a CSV file
class CDM_DECL CSV
{
public:
  /// Constructor.
  ///
  /// \param config Path to CSV file to read.
  CSV(std::string const& path);

  CSV(CSV&&);
  ~CSV();

  bool NextRecord();

  std::string NextValue(unsigned skip = 0);

  double NextValueAsDouble(unsigned skip = 0);

  std::string Record() const;

private:
  CSV(CSV const&) = delete;

  class Private;
  std::unique_ptr<Private> m_p;
};
