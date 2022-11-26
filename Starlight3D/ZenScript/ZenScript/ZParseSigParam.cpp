#include "ZParseSigParam.h"
#include "ZSigParamNode.h"

ZParseSigParam::ZParseSigParam(ZTokenStream* stream) : ZParseNode(stream) {



}

ZScriptNode* ZParseSigParam::Parse() {

	auto sigparam_node = new ZSigParamNode;

	auto token = mStream->NextToken();

	VarType var_type;

	switch (token.mType) {
	case TokenType::TokenVar:
		var_type = VarType::VarVar;
		break;
	case TokenType::TokenInt:
		var_type = VarType::VarInt;
		break;
	case TokenType::TokenFloat:
		var_type = VarType::VarFloat;
		break;
	case TokenType::TokenString:
		var_type = VarType::VarString;
		break;
	case TokenType::TokenIdent:
		var_type = VarType::VarInstance;
		break;
	case TokenType::TokenCObj:
		var_type = VarType::VarCObj;
		break;
	}

	auto par_name = mStream->NextToken();

	sigparam_node->SetType(var_type);
	sigparam_node->SetName(par_name.mText);

	int aa = 5;
	 
	return sigparam_node;

}