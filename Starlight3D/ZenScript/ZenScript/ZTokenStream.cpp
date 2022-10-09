#include "ZTokenStream.h"
#include <assert.h>

std::string TokenToString(enum TokenType type) {

	switch (type) {
	case TokenType::TokenIdent:
		return "TokenIdent";
	case TokenType::TokenNumber:
		return "TokenNumber";
	case TokenType::TokenOperator:
		return "TokenOperator";
	case TokenType::TokenString:
		return "TokenString";
	case TokenType::TokenInt:
		return "TokenInt";
	case TokenType::TokenFloat:
		return "TokenFloat";
	case TokenType::TokenPlus:
		return "TokenPlus";
	case TokenType::TokenMinus:
		return "TokenMinus";
	case TokenType::TokenMultiply:
		return "TokenMultiply";
	case TokenType::TokenDivide:
		return "TokenDivide";
	case TokenType::TokenEndOfLine:
		return "TokenEndOfLine";
	case TokenType::TokenEquals:
		return "TokenEquals";
	case TokenType::TokenUMinus:
		return "TokenUMinus";
		break;

	}
	return "Unknown";
}


//--

ZTokenStream::ZTokenStream() {

}

void ZTokenStream::AddToken(Token token) {

	mTokens.push_back(token);

}

Token ZTokenStream::NextToken() {
	if (mTokens.size() == 0) {
		return Token(TokenType::EndOfFile);
	}
	if (mTokenIndex >= mTokens.size())
	{
		return Token(TokenType::EndOfFile);
	}
	return mTokens[mTokenIndex++];

}

Token ZTokenStream::AssertNextToken(TokenType token_type) {

	auto token = NextToken();

	assert(token.mType == token_type);

	return token;


}

Token ZTokenStream::PeekToken(int increment) {

	if (mTokenIndex + increment >= mTokens.size()) {
		return Token(TokenType::EndOfFile);
	}

	return mTokens[mTokenIndex + increment];

}

void ZTokenStream::SetTokens(std::vector<Token> tokens)
{
	mTokens = tokens;
}

void ZTokenStream::Back() {

	mTokenIndex--;
	if (mTokenIndex < 0)
	{
		mTokenIndex = 0;
	}

}

bool ZTokenStream::EOS() {

	if (mTokenIndex >= mTokens.size()) {
		return true;
	}
	return false;

}

bool ZTokenStream::FindInLine(TokenType type,int peek)
{
	int pa = 0;
	pa = pa + peek;
	
	while (true) {

		if (mTokens[mTokenIndex + pa].mType == type)
		{
			return true;
		}
		if (mTokens[mTokenIndex + pa].mType == TokenType::TokenEndOfLine)
		{
			return false;
		}

		pa++;

	}
	return false;

}