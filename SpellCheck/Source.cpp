/* ---------------------------------------------------------------------------
** I attest that this following code represents my own work and is subject to
** the plagiarism policy found in the course syllabus.
**
**       Class: CSC 242
**  Assignment: Lesson 5 / Part 1 / SpellCheck
**        File: source.cpp
** Description: Program that checks the spelling of all words in a file.
**      Author: Afshin Bahrampour, Andre Gonzalez, Daniel Johnston
**        Date: 14 Feb 26
** -------------------------------------------------------------------------*/

#include "../SpellCheck.Core/pch.h"
#include <iostream>
#include <string>
#include "../SpellCheck.Core/SpellCheck.h"
#include <vector>

int main(int argc, char* argv[])
{
	/*
	 *This chunk of code handles the command line arguments before proceeding.
	 *First arg is the input file name,
	 *Second arg is the dictionary file name (optional, defaults to words.txt if not provided).
	 *If more than 3 or less than 2 arguments are provided, it prints a usage message and exits with an error code.
	 *
	 */
	
	if (argc < 2 || argc > 3) //Checks if the number of command line arguments is valid (at least 2 and at most 3)
    {
        std::cerr << "Usage: " << argv[0]
            << " <input_file> [dictionary_file]\n";
        return 1;
    }
	std::string input_file = argv[1]; //Gets the input file name from the command line arguments
	std::string dict_file; //Variable to hold the dictionary file name
	if (argc == 3) //Checks if a dictionary file name was provided as a command line argument
    {
		dict_file = argv[2]; //Gets the dictionary file name from the command line arguments if provided
    }
	else // If no dictionary file name was provided, it defaults to "words.txt"
    {
        dict_file = "words.txt"; //Default dictionary file name if not provided
    }

	std::vector<std::string> dict; //Initialize empty dictionary vector
	std::vector<std::string> source; //Initialize empty source vector
	std::vector<misspelled_word> misspelled; //Initialize empty misspelled vector

	create_dict(dict, dict_file); //Passes a reference of the empty vector to the create_dict function, which fills it with words from the file words.txt

	load_source(source, input_file); //Passes a reference of the empty vector to the load_source function, which fills it with words from the input file

	find_misspelled(source, dict, misspelled); //Finds words in source missing from dict and adds them to misspelled

	output_misspelled(misspelled);

	return 0; //Returns 0 to indicate successful execution of the program
}
