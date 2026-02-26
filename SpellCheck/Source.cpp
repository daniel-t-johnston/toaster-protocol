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

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

/**
 * Struct to hold a misspelled word and its position in the source text.
 */
struct misspelled_word
{
	std::string word;
	int word_number = 0;

	misspelled_word(std::string w, int n) : word(w), word_number(n) {}
};

void create_dict(std::vector<std::string>& dict, std::string dict_file)
{
	std::ifstream input(dict_file);
	if (!input)
	{
		throw std::runtime_error("Failed to open dictionary: ");
	}

	std::string word;
	while (input >> word)
	{
		std::transform(word.begin(), word.end(), word.begin(),
			[](unsigned char c) { return std::tolower(c); });

		dict.push_back(word);
	}
	std::sort(dict.begin(), dict.end());
	dict.erase(std::unique(dict.begin(), dict.end()), dict.end());
}

std::string clean_word(const std::string& word)
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

void load_source(std::vector<std::string>& source, std::string input_file)
{
	std::ifstream input(input_file);
	if (!input)
	{
		throw std::runtime_error("Failed to open input file: ");
	}
	std::string word;
	while (input >> word)
	{
		word = clean_word(word);
		source.push_back(word);
	}
}

void find_misspelled(const std::vector<std::string>& source, const std::vector<std::string>& dict,
	std::vector<misspelled_word>& misspelled)
{
	for (size_t i = 0; i < source.size(); ++i)
	{
		const std::string& source_word = source[i];
		if (!std::binary_search(dict.begin(), dict.end(), source_word))
		{
			if (source_word != "" && source_word != "\n" && source_word != " ")
			{
				misspelled.push_back(misspelled_word(source_word, static_cast<int>(i + 1)));
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

int main(int argc, char* argv[])
{
	if (argc < 2 || argc > 3)
	{
		std::cerr << "Usage: " << argv[0]
			<< " <input_file> [dictionary_file]\n";
		return 1;
	}
	std::string input_file = argv[1];
	std::string dict_file;
	if (argc == 3)
	{
		dict_file = argv[2];
	}
	else
	{
		dict_file = "words.txt";
	}

	std::vector<std::string> dict;
	std::vector<std::string> source;
	std::vector<misspelled_word> misspelled;
	try
	{
		create_dict(dict, dict_file);
		load_source(source, input_file);
		find_misspelled(source, dict, misspelled);
		output_misspelled(misspelled);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}
