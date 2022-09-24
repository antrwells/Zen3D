#include "ZParseScript.h"
#include "ZProgramNode.h"
#include "ZParseClass.h"

ZParseScript::ZParseScript(ZTokenStream* stream) : ZParseNode(stream) {

	//int aa = 5;



}

ZScriptNode* ZParseScript::Parse()
{

	ZProgramNode* main_node = new ZProgramNode;

	while (!mStream->EOS())
	{

		auto token = mStream->NextToken();

		switch (token.mType) {
		case TokenType::TokenClass:


			auto parse_class = new ZParseClass(mStream);
			
			auto class_node = (ZClassNode*) parse_class->Parse();

			main_node->AddClass(class_node);
			
			int aa = 5;


			break;

		}

		//int aa = 5;


	}

	return nullptr;

}