#include <iostream>
#include <string>
#include "SpellCheck.h"
#include <vector>
#include <fstream>
#include <algorithm>

void create_dict(std::vector<std::string>& dict, std::string dict_file)
{
	std::ifstream input(dict_file); //Opens the file
	if (!input) //Checks if the file was opened successfully
    {
		throw std::runtime_error("Failed to open dictionary: ");
	}

	std::string word; //Variable to hold each word read from the file
	while (input >> word) //Loops through each word in the file
	{
		std::transform(word.begin(), word.end(), word.begin(),
			[](unsigned char c) { return std::tolower(c); }); //Converts the word to lowercase for sorting

		dict.push_back(word); //Adds the word to the dictionary vector
	}
	std::sort(dict.begin(), dict.end()); //Sorts the dictionary vector in alphabetical order to make searching easier
}