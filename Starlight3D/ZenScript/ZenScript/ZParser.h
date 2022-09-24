#pragma once
#include "ZTokenStream.h"
class ZParser
{
public:

	ZParser(ZTokenStream* stream);
	void Parse();

private:

	ZTokenStream* mStream;

};

