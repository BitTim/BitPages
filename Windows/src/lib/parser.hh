#pragma once

#include <string>
#include <vector>

enum ErrorTypes
{
	ERROR_WARNING,
	ERROR_ERROR
};

class Token
{
public:
	Token(std::string value, bool skipNewline = false)
	{
		this->skipNewline = skipNewline;
		this->value = value;
	}

	bool skipNewline = false;
	std::string value;
};

class ErrorHighlight
{
public:
	ErrorHighlight(int type, int line, std::string desc);

	int type;
	int line;
	std::string desc;
};

std::vector<Token> tokenize(std::string input);
void parse(std::vector<Token> tokens, int cursorLine = -1);