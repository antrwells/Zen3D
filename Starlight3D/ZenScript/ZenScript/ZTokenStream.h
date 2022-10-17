#pragma once
#include <vector>
#include <string>
#include <iostream>

enum TokenType {
	TokenIdent,TokenNumber,TokenString,TokenClass,TokenEnd,TokenLeftPara,TokenRightPara,TokenNot,TokenAnd,TokenElse,TokenIf,
	TokenElseIf,TokenSwitch,TokenFor,EndOfFile,TokenOperator,TokenInt,TokenFloat,TokenMethod,TokenFunction,TokenPeriod,TokenComma,
	TokenPlus,TokenMinus,TokenDivide,TokenMultiply,TokenEndOfLine,TokenEquals,TokenNew,TokenVoid,TokenGreater,TokenLess,TokenTo,TokenReturn,
	TokenWhile,TokenDebugStop,TokenParseStop,TokenInc,TokenDec,TokenColon,TokenSame,TokenNotSame,TokenOr,TokenUMinus,TokenCObj,TokenStatic
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

class ZTokenStream;

class ZTokens {
public:
	std::vector<ZTokenStream*> Lines;

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
	bool FindInLine(TokenType type, int peek = 0);
	int TokenIndex() {
		return mTokenIndex;
	}
	Token GetToken(int id)
	{
		return mTokens[id];
	}
	bool Before(TokenType t1, TokenType t2) {

		int peek = 0;
		while(true)
		{ 
			auto tok = PeekToken(peek);
			if (tok.mType == t1)
			{
				return true;
			}
			if (tok.mType == t2) {
				return false;
			}
			peek++;
		}
		return true;

	}
private:
	std::vector<Token> mTokens;
	int mTokenIndex = 0;
};

