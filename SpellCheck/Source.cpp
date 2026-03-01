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
#include <stdexcept>

// Forward declaration
struct misspelled_word;

// Function prototypes
static void create_dict(std::vector<std::string>& dict, const std::string& dict_file);						//Author: Daniel Johnston
static std::string clean_word(const std::string& word);														//Author: Daniel Johnston
static void load_source(std::vector<std::string>& source, const std::string& input_file);					//Author: Andre Gonzalez
static void output_misspelled(const std::vector<misspelled_word>& misspelled);								//Author: Afshin Bahrampour
static void find_misspelled(const std::vector<std::string>& source, const std::vector<std::string>& dict,	//Author: Andre Gonzalez
                            std::vector<misspelled_word>& misspelled);

/**
 * Main function that orchestrates the spell checking process. 
 * It takes command-line arguments for the input file and an optional dictionary file. 
 * It loads the dictionary and source text, finds misspelled words, and outputs the results to a file and the terminal.
 * @param argc Number of command-line arguments passed to the program, which should be either 2 (input file only) or 3 (input file and dictionary file).
 * @param argv Array of C-style strings representing the command-line arguments. 
 *             argv[0] is the program name, 
 *             argv[1] is the input file,
 *             argv[2] (if provided) is the dictionary file.
 * @return Returns 0 on successful execution, or 1 if there is an error (e.g., incorrect usage, file opening failure).
 */
int main(int argc, char* argv[])																			//Author: Afshin Bahrampour
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
		std::cerr << "Error: " << e.what() << '\n';
		return 1;
	}
	return 0;
}


/**
 * Struct to hold a misspelled word and its position in the source text.
 * The word is stored as a string, and the word number indicates its position in the source text.
 */
struct misspelled_word
{
	std::string word;
	int word_number = 0;

	misspelled_word(const std::string& w, int n) : word(w), word_number(n)
	{
	}
};

/**
 * Creates a vector containing each word from the specified dictionary file, sorted in alphabetical order. The words are normalized to lowercase for consistent comparison.
 *
 * @param dict Blank vector passed by reference to be filled with the words from the dictionary file
 * @param dict_file file name of the dictionary file to read from, which should contain one word per line. If the file cannot be opened, the function throws a runtime error.
 */
static void create_dict(std::vector<std::string>& dict, const std::string& dict_file)
{
	std::ifstream input(dict_file);
	if (!input)
	{
		throw std::runtime_error("Failed to open dictionary: " + dict_file);
	}

	std::string word;
	while (input >> word)
	{
		word = clean_word(word);
		if (!word.empty())
			dict.push_back(word);
	}
	std::sort(dict.begin(), dict.end());
	dict.erase(std::unique(dict.begin(), dict.end()), dict.end());
}

/**
 * Removes non-alphabetic characters from the input word and converts it to lowercase for consistent comparison against the dictionary.
 * This function is used to clean up words from the input file before checking them against the dictionary.
 *
 * @param word String to be cleaned
 * @return Cleaned string
 */
static std::string clean_word(const std::string& word)
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

/**
 * Creates a vector containing each word from the specified input file, sorted in alphabetical order. The words are normalized to lowercase for consistent comparison.
 *
 * @param source Blank vector passed by reference to be filled with the words from the input file.
 * @param input_file
 */
static void load_source(std::vector<std::string>& source, const std::string& input_file)
{
	std::ifstream input(input_file);
	if (!input)
	{
		throw std::runtime_error("Failed to open input file: " + input_file);
	}
	std::string word;
	while (input >> word)
	{
		word = clean_word(word);
		if (!word.empty())
			source.push_back(word);
	}
}

/**
 * Finds strings in the source vector that are not present in the dict vector and adds them to the misspelled vector.
 *
 * @param source Input text file written to a vector of strings passed by reference.
 * @param dict  Dictionary file written to a vector of strings passed by reference.
 * @param misspelled Empty vector passed by reference to be filled with the misspelled words.
 */
static void find_misspelled(const std::vector<std::string>& source, const std::vector<std::string>& dict,
                     std::vector<misspelled_word>& misspelled)
{
	for (size_t i = 0; i < source.size(); ++i)
	{
		const std::string& source_word = source[i];
		if (!std::binary_search(dict.begin(), dict.end(), source_word))
		{
				misspelled.push_back(misspelled_word(source_word, static_cast<int>(i + 1)));
		}
	}
}

/**
 * Outputs the misspelled words.
 *
 * @param misspelled Vector of misspelled words passed by reference, which contains the misspelled words and position in the source text.
 */
static void output_misspelled(const std::vector<misspelled_word>& misspelled)
{

	std::cout << "Misspelled words:" << '\n';
	for (const auto& mw : misspelled)
	{
		std::cout << "Word: " << mw.word << ", Position: " << mw.word_number << '\n';
	}
}
