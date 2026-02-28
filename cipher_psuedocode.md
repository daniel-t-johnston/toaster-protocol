# Cipher Pseudocode

```text
START
  READ command-line arguments 
  PARSE action flag (-e or -d), keyword, input file, output file (Function)
  IF invalid arguments
    PRINT usage
    EXIT with error

  BUILD substitution key table: (Function)
    - add unique keyword letters (case-insensitive)
    - append remaining letters from z to a
    - create lowercase + uppercase mapping rows

  OPEN input file and read all text (Function)
  IF open fails
    PRINT error
    EXIT with error

  FOR each character in input text (Function)
    IF action is encrypt
      replace plain character with mapped cipher character
    ELSE IF action is decrypt
      replace cipher character with mapped plain character
    ELSE
      keep original character
  END FOR

  WRITE transformed text to output file (Function)
  IF write fails
    PRINT error
    EXIT with error

  PRINT success message
END
```
