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
	dict.erase(std::unique(dict.begin(), dict.end()), dict.end()); //Removes duplicate words from the source vector after sorting
}

void load_source(std::vector<std::string>& source, std::string input_file)
{
	std::ifstream input(input_file); //Opens the file
	if (!input) //Checks if the file was opened successfully
	{
		throw std::runtime_error("Failed to open input file: ");
	}
	std::string word; //Variable to hold each word read from the file
	while (input >> word) //Loops through each word in the file
	{
		word = clean_word(word); //Cleans the word by removing non-alphabetic characters and converting it to lowercase for comparison against the dictionary
		source.push_back(word); //Adds the word to the source vector
	}
	std::sort(source.begin(), source.end()); //Sorts the source vector in alphabetical order to make searching easier
	source.erase(std::unique(source.begin(), source.end()), source.end()); //Removes duplicate words from the source vector after sorting
}

std::string clean_word(const std::string& word) //removes non-alphabetic characters and converts the word to lowercase for comparison against the dictionary
{
	std::string cleaned;

	for (unsigned char c : word)
	{
		if (std::isalpha(c) || c == '\'')
		{
			cleaned += std::tolower(c);
		}
	}
	return cleaned;
}

