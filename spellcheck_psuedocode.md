# SpellCheck Pseudocode

```text
START
  READ command-line arguments
  IF argument count is not 2 or 3
    PRINT usage
    EXIT with error

  SET input file from argument
  SET dictionary file to optional argument OR default "words.txt"

  LOAD dictionary words:
    OPEN dictionary file
    IF open fails
      PRINT error
      EXIT with error
    FOR each token
      CLEAN token (lowercase, keep letters and apostrophes)
      IF cleaned token is not empty
        ADD to dictionary list
    END FOR
    SORT dictionary list
    REMOVE duplicates

  LOAD source words:
    OPEN input file
    IF open fails
      PRINT error
      EXIT with error
    FOR each token
      CLEAN token (lowercase, keep letters and apostrophes)
      IF cleaned token is not empty
        ADD to source list
    END FOR

  FIND misspellings:
    FOR each source word with position index
      IF word is not found in dictionary (binary search)
        STORE misspelled word + position
    END FOR

  PRINT all misspelled words and positions
END
```
