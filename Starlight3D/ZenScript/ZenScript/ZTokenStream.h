#pragma once
#include <vector>
#include <string>
#include <iostream>

enum TokenType {
	TokenIdent,TokenNumber,TokenString,TokenClass,TokenEnd,TokenLeftPara,TokenRightPara,TokenNot,TokenAnd,TokenElse,TokenIf,
	TokenElseIf,TokenSwitch,TokenFor,EndOfFile,TokenOperator,TokenInt,TokenFloat,TokenMethod,TokenFunction,TokenPeriod,TokenComma,
	TokenPlus,TokenMinus,TokenDivide,TokenMultiply,TokenEndOfLine,TokenEquals
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
	void Back();
	Token NextToken();
	Token AssertNextToken(enum TokenType token_type);
	Token PeekToken(int increment);
	bool EOS();
	void SetTokens(std::vector<Token> tokens);

private:
	std::vector<Token> mTokens;
	int mTokenIndex = 0;
};

