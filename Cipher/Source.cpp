/* ---------------------------------------------------------------------------
** I attest that this following code represents my own work and is subject to
** the plagiarism policy found in the course syllabus.
**
**       Class: CSC 242
**  Assignment: Lesson 5 / Part 1 / Cipher
**        File: source.cpp
** Description: Program that encrypts/decrypts a file using a keyword.
**      Author: Afshin Bahrampour, Andre Gonzalez, Daniel Johnston, Nevada Marquis
**        Date: 14 Feb 26
** -------------------------------------------------------------------------*/

#include <iostream>
#include <string>

void generate_key(std::string keyword, char key[52][2])
{
	//TODO: Generate key a-z, A-Z in COL 0, shifted text in COL 1
}

std::string get_input(std::string input_file)
{
	std::string input_string;

	//TODO: Write input_file to input_string

	return input_string;
}

std::string shift_text(std::string input_string, char key[52][2], char action)
{
	std::string output_string;

	//TODO: Replace chars found in COL 0 with COL 1 to encrypt, reverse for decrypt

	return output_string;
}

int save_output(std::string OUTPUT_FILE, std::string output_string)
{
	//TODO: Save output string to file

	return 1;
}

int main(int argc, char* argv[])
{
	//TODO: Handle args (-d or -e, input file, output file, -k keyword)

	const std::string INPUT_FILE = "";
	const std::string OUTPUT_FILE = "";
	const std::string KEYWORD = "";
	const char ACTION = 'a';

	const int KEY_ROWS = 52;
	const int KEY_COLS = 2;
	char key[KEY_ROWS][KEY_COLS] = {};

	generate_key(KEYWORD, key);
	std::string input_string = get_input(INPUT_FILE);
	std::string output_string = shift_text(input_string, key, ACTION);
	int complete = save_output(OUTPUT_FILE, output_string);

	if (complete == 1)
	{
		std::cout << "File " << INPUT_FILE << " was ";
		if (ACTION == 'd')
		{
			std::cout << "decrypted ";
		}
		else if (ACTION == 'e')
		{
			std::cout << "encrypted ";
		}
		std::cout << "using keyword" << KEYWORD << std::endl;
	}
	return 0;
}
