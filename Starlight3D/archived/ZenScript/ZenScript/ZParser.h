#pragma once
#include "ZTokenStream.h"

class ZMainNode;

class ZParser
{
public:

	ZParser(ZTokenStream* stream);
	ZMainNode* Parse();

private:

	ZTokenStream* mStream;

};

