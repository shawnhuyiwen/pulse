/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once


bool CDM_DECL WriteFile(const std::string& content, const std::string& filename, SerializationFormat m);
std::string CDM_DECL ReadFile(const std::string& filename, SerializationFormat m);
bool CDM_DECL CreateFilePath(const std::string&);
std::string CDM_DECL Replace(const std::string& original, const std::string& replace, const std::string& withThis);
void CDM_DECL ListFiles(const std::string& dir, std::vector<std::string>& files, const std::string& mask = "");
std::string CDM_DECL GetCurrentWorkingDirectory();
void CDM_DECL MakeDirectory(std::string const& dir);
void CDM_DECL DeleteDirectory(const std::string &dir, bool bDeleteSubdirectories=true);
bool CDM_DECL IsDirectory(struct dirent* ent);
