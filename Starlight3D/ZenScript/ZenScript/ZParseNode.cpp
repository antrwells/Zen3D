#include "ZParseNode.h"
#include <assert.h>

ZParseNode::ZParseNode(ZTokenStream* stream) {

	mStream = stream;
	TokeStart = stream->TokenIndex();


}

ZScriptNode* ZParseNode::Parse() {

	return nullptr;

}


void ZParseNode::Error(const char* err) {

	std::vector<Token> error_str;

	int cur_id = mStream->TokenIndex();

	for (int i = 0; i < 255; i++) {

		error_str.push_back(mStream->GetToken(TokeStart + i));
		if ((TokeStart + i) == cur_id)
		{
			error_str[i].mText = "[" + error_str[i].mText + "]";
		}
		if (error_str[i].mType == TokenType::TokenEndOfLine)
		{
			break;
		}

	}

	printf("Error:");
	printf(err);
	printf("\n");
	for (int i = 0; i < error_str.size(); i++)
	{
		printf(error_str[i].mText.c_str());
		printf(" ");
	}
	printf("\n");
	exit(1);


}
