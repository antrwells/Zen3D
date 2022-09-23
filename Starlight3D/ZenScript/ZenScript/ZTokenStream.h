#pragma once
#include <vector>
#include <string>
#include <iostream>

enum TokenType {
	TokenIdent,TokenNumber,TokenString,TokenClass,TokenEnd,TokenLeftPara,TokenRightPara,TokenNot,TokenAnd,TokenElse,TokenIf,
	TokenElseIf,TokenSwitch,TokenFor,EndOfFile,TokenOperator
};

std::string TokenToString(enum TokenType type);
class Token {
public:
	
	Token(enum TokenType type) {
		mType = type;
	}
	Token(enum TokenType type, std::string text) {
		mType = type;
		mText = text;
	}
	enum TokenType mType;
	std::string mText;
	int TokenIndex = 0;
	int TokenLineIndex = 0;
	
};

class ZTokenStream
{
public:

	ZTokenStream();
	void AddToken(Token token);
	Token NextToken();
	Token PeekToken(int increment);

private:
	std::vector<Token> mTokens;
	int mTokenIndex = 0;
};

