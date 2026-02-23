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

void find_misspelled(std::vector<std::string>& source, std::vector<std::string>& dict,
	std::vector<misspelled_word>& misspelled)
{
	for (const std::string& source_word : source) //Loops through each word in source 
	{
		if (!std::binary_search(dict.begin(), dict.end(), source_word))
		{
			if (source_word != "" && source_word != "\n" && source_word != " ")
			{
				misspelled.push_back((misspelled_word(source_word, &source_word - &source[0] + 1)));
			}
		}
	}
}

void output_misspelled(std::vector<misspelled_word>& misspelled)
{
	std::ofstream outfile("output.txt");
	for (const misspelled_word& mw : misspelled)
	{
		outfile << "Misspelled word: " << mw.word << " at position " << mw.word_number << std::endl;
	}
	outfile.close(); 
	std::cout << "Total misspelled words: " << misspelled.size() << std::endl;

	std::vector<std::string> words_only;
	for (const misspelled_word& mw : misspelled)
	{
		words_only.push_back(mw.word);
	}
	std::sort(words_only.begin(), words_only.end());
	std::vector<std::pair<std::string, int>> word_count;
	if (!words_only.empty()) {
		std::string current = words_only[0];
		int count = 1;

		for (size_t i = 1; i < words_only.size(); i++) {
			if (words_only[i] == current) {
				count++;
			}
			else {
				word_count.push_back({ current, count });
				current = words_only[i];
				count = 1;
			}
		}
		word_count.push_back({ current, count });
	}

	std::cout << "Misspelled words and their counts:" << std::endl;
	for (const auto& wc : word_count)
	{
		std::cout << wc.first << ": " << wc.second << std::endl;
	}
}

