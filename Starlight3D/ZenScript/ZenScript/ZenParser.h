#pragma once

class ZTokens;
class ZenByteCode;

class ZenParser
{
public:

	ZenParser(ZTokens* toks);
	ZenByteCode* Parse();

private:

	ZTokens* mTokens;

};

