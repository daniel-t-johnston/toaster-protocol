# toaster-protocol
CSC 242 Group Project Repo

## Project Overview
This repository currently contains two C++ console applications:

- **Cipher**: encrypts or decrypts text using a keyword-based substitution alphabet.
- **SpellCheck**: checks a text file against a dictionary and reports misspelled words.

## Cipher

### What it does
`Cipher` builds a substitution key from a keyword and then transforms text by replacing each alphabetic character according to that key.

- Supports **encryption** (`-e`) and **decryption** (`-d`).
- Supports keyword format `-kKEYWORD` or `-k KEYWORD`.
- Preserves character case by maintaining lowercase and uppercase mappings.
- Leaves non-mapped characters (punctuation, spaces, digits) unchanged.

### Key construction behavior
The cipher alphabet is built as follows:
1. Start with unique letters from the keyword (in order encountered, case-insensitive).
2. Append remaining letters of the alphabet in reverse order (`z` to `a`) that were not already used.

### Command line usage
From the directory containing `Cipher.exe`:

```bash
Cipher.exe -e -kKEYWORD <input_file> <output_file>
Cipher.exe -d -kKEYWORD <input_file> <output_file>
```

Alternative keyword form:

```bash
Cipher.exe -e -k KEYWORD <input_file> <output_file>
```

### Error handling
- Prints usage text when arguments are invalid.
- Throws/prints runtime errors for missing action flag, missing keyword, missing input/output path, or file open failures.

### Cipher files
- `Cipher/Source.cpp`: main cipher implementation (argument parsing, key generation, transform, I/O).
- `Cipher/Source1.cpp`: additional source file in the Cipher project.
- `Cipher/input.txt`, `Cipher/input_2.txt`: sample input files.
- `Cipher/output.txt`: sample output file.

## SpellCheck

### What it does
`SpellCheck` reads an input file, cleans each token, and compares it against a cleaned dictionary.

- Accepts required input file and optional dictionary file path.
- Defaults dictionary to `words.txt` when omitted.
- Normalizes words to lowercase.
- Keeps apostrophes (`'`) and alphabetic characters; removes other characters.
- Uses a sorted unique dictionary and `binary_search` for lookup.
- Prints misspelled words and word positions to the console.

### Command line usage
From the directory containing `SpellCheck.exe`:

```bash
SpellCheck.exe <input_file> [dictionary_file]
```

Example:

```bash
SpellCheck.exe input.txt words.txt
```

### Error handling
- Prints usage text if argument count is invalid.
- Throws/prints runtime errors when input or dictionary files cannot be opened.

### SpellCheck files
- `SpellCheck/Source.cpp`: main spell checking implementation.
- `SpellCheck/input.txt`: sample input file.
- `SpellCheck/words.txt`: default dictionary file.
- `SpellCheck/test_dict.txt`: sample test dictionary.

## Tests
Visual Studio test projects are present:

- `Cipher.Test/`
- `SpellCheck.Test/`

These are scaffolded in the repository and can be extended with additional unit tests.
