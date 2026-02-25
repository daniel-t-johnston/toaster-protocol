#include <iostream>
#include <string>
#include "Cipher.h"

void generate_key(std::string keyword, char key[52][2])
{
	//TODO: Generate key a-z, A-Z in COL 0, shifted text in COL 1
}

std::string get_input( std::string input_file)
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

