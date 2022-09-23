#include "ZTokenizer.h"

ZTokenizer::ZTokenizer(ZSource* source)
{

	auto code = source->GetCode();

	for (int i = 0; i < code.size(); i++) {

		//std::cout << "Source:" << code[i] << std::endl;

	}

	mCode = code;

}

bool vec_contains(std::vector<char> vec, char look)
{

	for (int i = 0; i < vec.size(); i++) {

		if (vec[i] == look) {
			return true;
		}

	}
	return false;

}

ZTokenStream* ZTokenizer::Tokenize() {

	std::vector<char> is_text;
	std::vector<char> is_op;

	is_text.push_back("a"[0]);
	is_text.push_back("b"[0]);
	is_text.push_back("c"[0]);
	is_text.push_back("d"[0]);
	is_text.push_back("e"[0]);
	is_text.push_back("f"[0]);
	is_text.push_back("g"[0]);
	is_text.push_back("h"[0]);
	is_text.push_back("i"[0]);
	is_text.push_back("j"[0]);
	is_text.push_back("k"[0]);
	is_text.push_back("l"[0]);
	is_text.push_back("m"[0]);
	is_text.push_back("n"[0]);
	is_text.push_back("o"[0]);
	is_text.push_back("p"[0]);
	is_text.push_back("q"[0]);
	is_text.push_back("r"[0]);
	is_text.push_back("s"[0]);
	is_text.push_back("t"[0]);
	is_text.push_back("u"[0]);
	is_text.push_back("v"[0]);
	is_text.push_back("w"[0]);
	is_text.push_back("x"[0]);
	is_text.push_back("y"[0]);
	is_text.push_back("z"[0]);

	is_text.push_back("A"[0]);
	is_text.push_back("B"[0]);
	is_text.push_back("C"[0]);
	is_text.push_back("D"[0]);
	is_text.push_back("E"[0]);
	is_text.push_back("F"[0]);
	is_text.push_back("G"[0]);
	is_text.push_back("H"[0]);
	is_text.push_back("I"[0]);
	is_text.push_back("J"[0]);
	is_text.push_back("K"[0]);
	is_text.push_back("L"[0]);
	is_text.push_back("M"[0]);
	is_text.push_back("N"[0]);
	is_text.push_back("O"[0]);
	is_text.push_back("P"[0]);
	is_text.push_back("Q"[0]);
	is_text.push_back("R"[0]);
	is_text.push_back("S"[0]);
	is_text.push_back("T"[0]);
	is_text.push_back("U"[0]);
	is_text.push_back("V"[0]);
	is_text.push_back("W"[0]);
	is_text.push_back("X"[0]);
	is_text.push_back("Y"[0]);
	is_text.push_back("Z"[0]);
	
	//Operators
	is_op.push_back("("[0]);
	is_op.push_back(")"[0]);
	is_op.push_back(","[0]);

	std::vector<Token> tokens;


	for (int i = 0; i < mCode.size(); i++) {

		auto line = mCode[i];

		std::string cur_token = "";
		bool is_string = false;
		bool is_number = false;

		for (int c = 0; c < line.size(); c++) {

			auto ch = line[c];

			if (vec_contains(is_text, ch))
			{
				cur_token = cur_token + ch;
			}
			else {

				if (cur_token.size() > 0)
				{
					tokens.push_back(Token(TokenType::TokenIdent, cur_token));
					cur_token = "";
				}

				if (vec_contains(is_op, ch))
				{

					std::string op = "";
					op = op + ch;
					tokens.push_back(Token(TokenType::TokenOperator,op));

				}

			}


		}

	}

	std::cout << "Token Stream output." << std::endl;

	for (int i = 0; i < tokens.size(); i++)
	{

		std::cout << "Token:" << TokenToString(tokens[i].mType) << "  Text:" << tokens[i].mText << std::endl;

	}

	return nullptr;

}