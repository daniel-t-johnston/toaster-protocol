/* ---------------------------------------------------------------------------
** I attest that this following code represents my own work and is subject to
** the plagiarism policy found in the course syllabus.
**
**       Class: CSC 242
**  Assignment: Lesson 5 / Part 1 / Cipher
**        File: source.cpp
** Description: Program that encrypts/decrypts a file using a keyword.
**      Author: Afshin Bahrampour, Andre Gonzalez, Daniel Johnston
**        Date: 14 Feb 26
** -------------------------------------------------------------------------*/


#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>  

// Constants for the substitution key table.
static const int KEY_ROWS = 52;
static const int KEY_COLS = 2;

// Function prototypes
static void parse_args(int argc, char* argv[],														// Authors: Daniel Johnston
	char& action,
	std::string& keyword,
	std::string& input_file,
	std::string& output_file);
static void print_usage(const char* program_name);													// Authors: Daniel Johnston
static void generate_key(const std::string& keyword, char key[KEY_ROWS][KEY_COLS]);					// Authors:
static std::string get_input(const std::string& input_file);										// Authors:
static std::string shift_text(const std::string& input_string,										// Authors:
	const char key[KEY_ROWS][KEY_COLS],
	char action);
static void save_output(const std::string& output_file, const std::string& output_string);			// Authors:

int main(int argc, char* argv[])
{
	try
	{
		/* Argument handling*/
		char action = '\0';               // 'e' for encrypt, 'd' for decrypt
		std::string keyword;              // cipher keyword
		std::string input_file;           // input filename
		std::string output_file;          // output filename

		parse_args(argc, argv, action, keyword, input_file, output_file);

		/* Key generation and file processing */
		char key[KEY_ROWS][KEY_COLS] = {}; 
		generate_key(keyword, key);											// Generate the substitution key table based on the keyword
		std::string input_string = get_input(input_file);					// Read the entire contents of the input file into a string
		std::string output_string = shift_text(input_string, key, action);	// Encrypt or decrypt the input string using the key table and the specified action
		save_output(output_file, output_string);							// Write the resulting output string to the output file

		/*Output a message indicating the action is complete*/
		std::cout << "File " << input_file << " was "						
			<< (action == 'd' ? "decrypted " : "encrypted ")
			<< "using keyword " << keyword << '\n';

		return 0;
	}
	/*Error handling: catch any exceptions thrown during argument parsing, key generation, 
	 *file reading/writing, or text shifting, and print an error message to the standard error stream. 
	 *Return 1 to indicate an error occurred. 
	 */
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << '\n';
		return 1;
	}
}

/**
 * Prints usage instructions for the program.
 *
 * @param program_name The name of the program (typically argv[0]) to include in the usage message.
 */
static void print_usage(const char* program_name)
{
	std::cerr << "Usage:\n"
		<< "  " << program_name << " -e -kKEYWORD <input_file> <output_file>\n"
		<< "  " << program_name << " -d -kKEYWORD <input_file> <output_file>\n"
		<< "Also accepted:\n"
		<< "  " << program_name << " -e -k KEYWORD <input_file> <output_file>\n";
}


/*
 *Parses command-line arguments to determine the action (encrypt/decrypt), keyword, input file, and output file.
 *
 *@param argc Number of command-line arguments.
 *@param argv Array of C-style strings representing the command-line arguments.
 *@param action Reference to a char that will be set to 'e' for encryption or 'd' for decryption based on the parsed arguments.
 *@param keyword Reference to a string that will be set to the cipher keyword extracted from the arguments.
 *@param input_file Reference to a string that will be set to the input filename extracted from the arguments.
 *@param output_file Reference to a string that will be set to the output filename extracted from the arguments.
 */
static void parse_args(int argc, char* argv[],
	char& action,
	std::string& keyword,
	std::string& input_file,
	std::string& output_file)
{

	if (argc < 5)
	{
		print_usage(argv[0]);
		throw std::runtime_error("Not enough arguments.");
	}

	action = '\0';
	keyword.clear();
	input_file.clear();
	output_file.clear();

	// Parse flags in the first few args
	for (int i = 1; i < argc; ++i)
	{
		std::string arg = argv[i];

		if (arg == "-e")
		{
			action = 'e';
		}
		else if (arg == "-d")
		{
			action = 'd';
		}
		else if (arg.rfind("-k", 0) == 0) // starts with "-k"
		{
			// Supports "-kFEATHER" or "-k"
			if (arg.size() > 2)
			{
				keyword = arg.substr(2); // everything after -k
			}
			else
			{
				// "-k" followed by keyword as next arg
				if (i + 1 >= argc)
				{
					print_usage(argv[0]);
					throw std::runtime_error("Missing keyword after -k.");
				}
				keyword = argv[++i];
			}
		}
		else
		{
			// First non-flag is input_file, second is output_file
			if (input_file.empty())
			{
				input_file = arg;
			}
			else if (output_file.empty())
			{
				output_file = arg;
			}
			else
			{
				// Too many positional args
				print_usage(argv[0]);
				throw std::runtime_error("Too many arguments.");
			}
		}
	}

	if (action != 'e' && action != 'd')
	{
		print_usage(argv[0]);
		throw std::runtime_error("Must specify -e (encrypt) or -d (decrypt).");
	}

	if (keyword.empty())
	{
		print_usage(argv[0]);
		throw std::runtime_error("Keyword is required (-kKEYWORD).");
	}

	if (input_file.empty() || output_file.empty())
	{
		print_usage(argv[0]);
		throw std::runtime_error("Input and output files are required.");
	}
}

/*
 *Generates a substitution key table based on the provided keyword. 
 *The key table is a 2D array where each row corresponds to a letter of the alphabet (both lowercase and uppercase), 
 *and the columns represent the original letter and its corresponding cipher letter.
 *
 *@param keyword The cipher keyword used to generate the substitution key table. The function will process this keyword to create a cipher alphabet.
 *@param key A 2D array of characters (size KEY_ROWS x KEY_COLS)
 */
static void generate_key(const std::string& keyword, char key[KEY_ROWS][KEY_COLS])
{
	// TODO:
	// 1) Build a cipher alphabet from keyword:
	//    - Keep letters only
	//    - Convert to lowercase
	//    - Remove duplicates (keep first occurrence)
	//    - Append remaining letters in reverse alphabet order (z..a) excluding used letters
	// 2) Fill key table:
	//    Rows 0-25:  key[row][0] = 'a'+row, key[row][1] = cipherLower[row]
	//    Rows 26-51: key[row][0] = 'A'+(row-26), key[row][1] = toupper(cipherLower[row-26])

}

/*
 *Reads the entire contents of the specified input file into a string.
 *
 *@param input_file The name of the input file to read.
 *@return A string containing the entire contents of the input file, including spaces and newlines. If the file cannot be opened, the function throws a runtime error.
 */
static std::string get_input(const std::string& input_file)
{
	// TODO:
	// - Open input_file
	// - Read entire contents (including spaces/newlines) into a string
	// - Throw runtime_error if open fails

	return "";
}

/*
 *Encrypts or decrypts the input string using the provided substitution key table and the specified action.
 *
 *@param input_string The string to be encrypted or decrypted. This string may contain letters, punctuation, whitespace, etc.
 *@param key A 2D array of characters representing the substitution key table generated from the keyword. 
 *The first column contains original letters (a-z, A-Z) and the second column contains their corresponding cipher letters.
 *@param action A character indicating whether to encrypt ('e') or decrypt ('d') the input string. 
 *The function will use this action to determine how to apply the substitutions based on the key table.
 *@return A new string that is the result of encrypting or decrypting the input string according to the key table and action. 
 *Letters found in the key table will be replaced accordingly, while characters not found (such as punctuation and whitespace) will be left unchanged.
 */
static std::string shift_text(const std::string& input_string,
	const char key[KEY_ROWS][KEY_COLS],
	char action)
{

	// TODO:
	// For each character in input_string:
	//  - If action == 'e': replace chars found in COL 0 with COL 1
	//  - If action == 'd': replace chars found in COL 1 with COL 0
	//  - If not found (punctuation/whitespace), keep as-is
	//
	// Use simple loops:
	//  - Outer loop over characters
	//  - Inner loop over 52 rows to find mapping

	return "";
}

/*
 *Writes the specified output string to the specified output file.
 *
 *@param output_file The name of the output file to write to.
 *@param output_string The string to be written to the output file. This string may contain letters, punctuation, whitespace, etc.
 *The function will write this string exactly as it is to the output file. If the file cannot be opened for writing, the function throws a runtime error.
 */
static void save_output(const std::string& output_file, const std::string& output_string)
{
	// TODO:
	// - Open output_file for writing
	// - Write output_string
	// - Throw runtime_error if open fails

}