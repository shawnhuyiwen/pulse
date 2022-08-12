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
bool CDM_DECL DeleteFile(const std::string &dir, short retry=1);
bool CDM_DECL DeleteDirectory(const std::string& dir);
bool CDM_DECL FileExists(const std::string& filename);
bool CDM_DECL IsJSONFile(const std::string& filename);
std::string CDM_DECL RelativePathFrom(const std::string& dir, const std::string& filepath);

void CDM_DECL SplitPath(const std::string& filepath, std::string& path);
void CDM_DECL SplitPathFilename(const std::string& filepath, std::string& path, std::string& filename);
// Get the filename (with extension) from a filepath
void CDM_DECL SplitFilename(const std::string& filepath, std::string& filename);
// Get the base filename and extension of a filepath
void CDM_DECL SplitFilenameExt(const std::string& filepath, std::string& filename, std::string& ext);
// Get the path from the filename
void CDM_DECL SplitPathFilenameExt(const std::string& filepath, std::string& path, std::string& filename, std::string& ext);
