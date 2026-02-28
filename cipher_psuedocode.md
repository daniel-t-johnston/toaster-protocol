# Cipher Pseudocode

```text
START
  READ command-line arguments
  PARSE action flag (-e or -d), keyword, input file, output file
  IF invalid arguments
    PRINT usage
    EXIT with error

  BUILD substitution key table:
    - add unique keyword letters (case-insensitive)
    - append remaining letters from z to a
    - create lowercase + uppercase mapping rows

  OPEN input file and read all text
  IF open fails
    PRINT error
    EXIT with error

  FOR each character in input text
    IF action is encrypt
      replace plain character with mapped cipher character
    ELSE IF action is decrypt
      replace cipher character with mapped plain character
    ELSE
      keep original character
  END FOR

  WRITE transformed text to output file
  IF write fails
    PRINT error
    EXIT with error

  PRINT success message
END
```
