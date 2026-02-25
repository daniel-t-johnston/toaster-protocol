# toaster-protocol
CSC 242 Group Project Repo

## Cipher README (Draft)

- #### Cipher: Compiled to create Cipher.exe
  - ##### Source.cpp: Contains main(), which is used to call functions from the Cipher.Core static library using Cipher.h
- #### Cipher.Core: Cipher static library
  - ##### Cipher.cpp: Contains functions for the Cipher program
  - ##### Cipher.h: Header file for Cipher.Core. All exposed functions need to be included.
- #### Cipher.Test: Testing for Cipher
  - ##### Cipher.Test.cpp: Tests for Cipher.Core functions

## SpellCheck README (Draft)

### Overview
The SpellCheck program reads an input text file, compares each cleaned word against a dictionary, and reports words that do not appear in the dictionary.

Core behavior:
- Accepts an input file path and an optional dictionary file path from the command line.
- Defaults to `words.txt` when no dictionary is supplied.
- Normalizes words to lowercase before comparing.
- Preserves apostrophes in words (for example: `can't`).
- Removes punctuation and non-letter characters from tokens before checking.
- Writes detailed misspelling results to `output.txt`.
- Prints total misspellings and per-word misspelling counts to the console.

### Command Line Usage
From the directory containing `SpellCheck.exe`:

```bash
SpellCheck.exe <input_file> [dictionary_file]
```

Arguments:
- `<input_file>`: Required. Text file to spell-check.
- `[dictionary_file]`: Optional. Dictionary file path. Defaults to `words.txt`.

Example:

```bash
SpellCheck.exe input.txt words.txt
```

### Input and Dictionary Format
- Input file:
  - Any plain-text file.
  - Split by whitespace into tokens.
  - Tokens are cleaned so only alphabetic characters and apostrophes remain.
- Dictionary file:
  - Plain-text list of words (whitespace-delimited).
  - Case-insensitive matching is applied by lowercasing dictionary entries.
  - Duplicate dictionary entries are removed internally.

### Output
- `output.txt` is generated/overwritten and contains one line per misspelled word occurrence:

```text
Misspelled word: <word> at position <word_number>
```

- Console output includes:
  - Total misspelled words.
  - A per-word misspelling frequency summary.

### Error Handling
- If the command line argument count is invalid, the program prints:

```text
Usage: SpellCheck.exe <input_file> [dictionary_file]
```

- If the dictionary or input file cannot be opened, the core functions throw `std::runtime_error`.

### Related Files
- `SpellCheck/Source.cpp`: Program entry point and argument handling.
- `SpellCheck.Core/SpellCheck.cpp`: Dictionary loading, input cleaning, misspelling detection, and output formatting.
- `SpellCheck.Core/SpellCheck.h`: Public API and `misspelled_word` struct.
- `SpellCheck.Test/SpellCheck.Test.cpp`: Unit tests for SpellCheck core behavior.