
#include "ZSource.h"

ZSource::ZSource() {

	mPath = "";
	

}

void ZSource::AddLine(std::string line)
{
	code.push_back(line);
	Clean();
}

ZSource::ZSource(std::string path) {

	mPath = path;

	std::ifstream source_file(path);

	std::string line_text;

	while (std::getline(source_file, line_text))
	{
		std::cout << "Line:" << line_text << std::endl;
		code.push_back(line_text);
	}

	source_file.close();
	Clean();

}

std::vector<std::string> ZSource::GetCode() {

	return code;

}

void ZSource::Clean() {


	std::vector<char> bad_chars;

	//bad_chars.push_back(" "[0]);
	bad_chars.push_back("\n"[0]);
	bad_chars.push_back("	"[0]);
	bad_chars.push_back("\0"[0]);

	std::vector<std::string> new_code;

	for (int i = 0; i < code.size(); i++) {

		auto line = code[i];

		if (line.size() == 0)
		{
			continue;
		}

		std::string new_line = "";

		for (int c = 0; c < line.size(); c++)
		{
			
			auto ch = line[c];
			bool skip = false;
			if (c == 0) {
				if (ch == " "[0])
				{
					skip = true;
				}
			}
			for (int b = 0; b < bad_chars.size(); b++)
			{

				if (ch == bad_chars[b])
				{
					skip = true;
					break;
				}

			}

			if (!skip) {
				new_line = new_line + ch;
			}
			

		}


		std::string new_line2 = "";
		int s_c = 0;
		bool skip = false;
		for (int c = 0; c < new_line.size(); c++) {

			auto ch = new_line[c];

			if (ch == " "[0])
			{
				s_c++;
				if (s_c > 1)
				{
					skip = true;
				}
			}
			else {
				skip = false;
				s_c = 0;
			}

			if (!skip) {
				new_line2 = new_line2 + ch;
			}
		}

		//std::cout << "Cleaned:" << new_line << std::endl;
		
		if (new_line2[0] == " "[0])
		{
			new_line2 = new_line2.substr(1, new_line2.size() - 1);
		}
		std::cout << "Further Cleaned:" << new_line2 << std::endl;
		new_code.push_back(new_line2);
	}

	code = new_code;

}