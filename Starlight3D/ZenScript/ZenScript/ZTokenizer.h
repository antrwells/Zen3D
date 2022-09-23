#pragma once
#include "ZTokenStream.h"
#include <vector>
#include <string>
#include "ZSource.h"
#include <iostream>

class ZTokenizer
{
public:

	ZTokenizer(ZSource * source);
	ZTokenStream* Tokenize();


private:

	std::vector<std::string> mCode;

};

