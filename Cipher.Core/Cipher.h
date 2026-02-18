#pragma once
# include <string>

/**
 * Passes pointer for key array and keyword, fills array with a-z, A-Z in COL 0 and the shifted key in COL 1.
 * 
 * @param keyword Keyword from args
 * @param key Empty 52x2 char array
 */
void generate_key(std::string keyword, char key[52][2]);

/**
 * Reads input file text and writes it to a string variable.
 * 
 * @param input_file Input file name
 * @return String containing text from input file
 */
std::string get_input(std::string input_file);

/**
 *Shifts text using key and encryption/decryption action
 * 
 * @param input_string String generated from get_input
 * @param key Key to shift text in input_string
 * @param action d for decrypt, e for encrypt
 * @return String of shifted text
 */
std::string shift_text(std::string input_string, char key[52][2], char action);

/**
 * Writes string to file
 * 
 * @param OUTPUT_FILE File name to write output string to
 * @param output_string Shifted string being written to file.
 * @return 1 if file written
 */

int save_output(std::string OUTPUT_FILE, std::string output_string);
