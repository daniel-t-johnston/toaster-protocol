#pragma once
#include <string>
#include <vector>


/**
 * Struct to hold a misspelled word and its position in the source text. 
 * The word is stored as a string, and the word number indicates its position in the source text. 
 * This struct is used to store information about words that are not found in the dictionary.
 */
struct misspelled_word
{
	std::string word;
	int word_number = 0;

	/**
	 * Constructor to initialize the misspelled_word struct with a word and its position in the source text.
	 * 
	 * @param w String representing the misspelled word
	 * @param n Position of the word in the source text (word number)
	 */
	misspelled_word(std::string w, int n) : word(w), word_number(n) {}
};

/**
 * Creates a vector containing each word from the specified dictionary file, sorted in alphabetical order. The words are normalized to lowercase for consistent comparison.
 * 
 * @param dict Blank vector passed by reference to be filled with the words from the dictionary file
 * @param dict_file file name of the dictionary file to read from, which should contain one word per line. If the file cannot be opened, the function throws a runtime error.
 */
void create_dict(std::vector<std::string>& dict, std::string dict_file);

/**
 * Creates a vector containing each word from the specified input file, sorted in alphabetical order. The words are normalized to lowercase for consistent comparison.
 * 
 * @param source Blank vector passed by reference to be filled with the words from the input file.
 * @param input_file 
 */
void load_source(std::vector<std::string>& source, std::string input_file);

/**
 * Removes non-alphabetic characters from the input word and converts it to lowercase for consistent comparison against the dictionary. 
 * This function is used to clean up words from the input file before checking them against the dictionary.
 * 
 * @param word String to be cleaned
 * @return Cleaned string
 */
std::string clean_word(const std::string& word);

/**
 * Finds strings in the source vector that are not present in the dict vector and adds them to the misspelled vector. 
 * 
 * @param source Input text file written to a vector of strings passed by reference.
 * @param dict  Dictionary file written to a vector of strings passed by reference.
 * @param misspelled Empty vector passed by reference to be filled with the misspelled words.
 */
void find_misspelled(std::vector<std::string>& source, std::vector<std::string>& dict, std::vector<misspelled_word>& misspelled);

/**
 * Outputs the misspelled words and their position to output.txt. Prints count to terminal window.
 * 
 * @param misspelled Vector of misspelled words passed by reference, which contains the misspelled words and position in the source text.
 */
void output_misspelled(std::vector<misspelled_word>& misspelled);