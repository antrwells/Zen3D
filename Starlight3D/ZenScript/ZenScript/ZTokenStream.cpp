#include "ZTokenStream.h"

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
	if (mTokenIndex == mTokens.size() - 1)
	{
		return Token(TokenType::EndOfFile);
	}
	return mTokens[mTokenIndex++];

}

Token ZTokenStream::PeekToken(int increment) {

	if (mTokenIndex + increment >= mTokens.size()) {
		return Token(TokenType::EndOfFile);
	}

	return mTokens[mTokenIndex + increment];

}