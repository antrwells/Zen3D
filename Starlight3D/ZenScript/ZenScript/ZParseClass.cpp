#include "ZParseClass.h"
#include "ZClassNode.h"
#include "ZParseMethod.h"
#include "ZMethodNode.h";

ZParseClass::ZParseClass(ZTokenStream* stream) : ZParseNode(stream)
{

}

ZScriptNode* ZParseClass::Parse()
{

	auto class_node = new ZClassNode;

	auto class_name = mStream->NextToken();

	class_node->SetName(class_name.mText);

	while (!mStream->EOS()) {

		auto token = mStream->NextToken();

		//
		ZParseMethod* parse_meth;
		ZMethodNode* meth_node;
		//

		switch (token.mType) {
		case TokenType::TokenMethod:

			parse_meth = new ZParseMethod(mStream);

			meth_node = (ZMethodNode*)parse_meth->Parse();

			class_node->AddMethod(meth_node);

			break;
		case TokenType::TokenFunction:

			break;
		}

	}

	

	return class_node;

}