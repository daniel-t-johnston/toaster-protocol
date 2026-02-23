#pragma once
# include <string>
#include <vector>

/**
 * Creates a vector containing each word from the specified dictionary file, sorted in alphabetical order. The words are normalized to lowercase for consistent comparison.
 * @param dict Blank vector passed by reference to be filled with the words from the dictionary file
 * @param dict_file file name of the dictionary file to read from, which should contain one word per line. If the file cannot be opened, the function throws a runtime error.
 */
void create_dict(std::vector<std::string>& dict, std::string dict_file);

/**
 * Creates a vector containing each word from the specified input file, sorted in alphabetical order. The words are normalized to lowercase for consistent comparison.
 * @param source Blank vector passed by reference to be filled with the words from the input file.
 * @param input_file 
 */
void load_source(std::vector<std::string>& source, std::string input_file);

/**
 * Removes non-alphabetic characters from the input word and converts it to lowercase for consistent comparison against the dictionary. 
 * This function is used to clean up words from the input file before checking them against the dictionary.
 * @param word String to be cleaned
 * @return Cleaned string
 */
std::string clean_word(const std::string& word);