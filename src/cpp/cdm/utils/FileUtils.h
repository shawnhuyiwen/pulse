/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

bool CDM_DECL WriteFile(const std::string& content, const std::string& filename);// ASCII Support only
bool CDM_DECL ReadFile(const std::string& filename, std::string& content);// ASCII Support only
bool CDM_DECL IsDirectory(std::string const& dir);
bool CDM_DECL CreatePath(const std::string&);
bool CDM_DECL CreateFilePath(const std::string&);
std::string CDM_DECL Replace(const std::string& original, const std::string& replace, const std::string& withThis);
void CDM_DECL ListFiles(const std::string& dir, std::vector<std::string>& files, bool recursive, const std::string& mask = "");
std::string CDM_DECL GetCurrentWorkingDirectory();
void CDM_DECL MakeDirectory(std::string const& dir);
void CDM_DECL DeleteDirectory(const std::string &dir, bool bDeleteSubdirectories=true);
bool CDM_DECL FileExists(const std::string& filename);
bool CDM_DECL IsJSONFile(const std::string& filename);
// Get the filename (with extension) from a filepath
bool CDM_DECL SplitFilenamePath(const std::string& filepath, std::string& filename);
// Get the base filename and extension of a filepath
bool CDM_DECL SplitFilenameExt(const std::string& filepath, std::string& filename, std::string& ext);
