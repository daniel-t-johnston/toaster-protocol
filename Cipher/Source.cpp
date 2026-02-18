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
#include "Cipher.h"

int main(int argc, char* argv[])
{
	//TODO: Handle args (-d or -e, input file, output file, -k keyword)

	const std::string INPUT_FILE = ""; //add input file name from args
	const std::string OUTPUT_FILE = ""; //add output file name from args
	const std::string KEYWORD = ""; //add keyword from args
	const char ACTION = 'a'; //add d or e from args

	//Initialize empty key array
	const int KEY_ROWS = 52; //26 upper case, 26 lower case
	const int KEY_COLS = 2; //alphabet and shifted alphabet
	char key[KEY_ROWS][KEY_COLS] = {}; //52x2 empty array

	//The following code calls functions from Cipher.cpp in the Cipher.core static library. 

	generate_key(KEYWORD, key); //Calls function in Cipher.cpp to generate key array

	std::string input_string = get_input(INPUT_FILE); //Calls function in Cipher.cpp to write input file to string

	std::string output_string = shift_text(input_string, key, ACTION); //Calls function in Cipher.cpp to shift string

	int complete = save_output(OUTPUT_FILE, output_string); //Calls function in Cipher.cpp to write output string to file

	if (complete == 1) //Print confirmation statement
	{
		std::cout << "File " << INPUT_FILE << " was ";
		if ( ACTION == 'd')
		{
			std::cout << "decrypted ";
		}
		else if ( ACTION == 'e')
		{
			std::cout << "encrypted ";
		}
		std::cout << "using keyword" << KEYWORD << std::endl;
	}
	return 0;
}
