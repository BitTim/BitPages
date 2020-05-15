#pragma once
#include <string>

extern int nDotWarn;

extern std::vector<std::string> tokenize(std::string path);
extern void parse(std::vector<std::string> tokens);