# Compiler Generator

A C++ implementation of a complete compiler that includes lexical analysis (NFA to DFA conversion), syntax analysis (LL(1) parsing), and code generation with leftmost derivation tracking.

## Project Overview

This project is a complete implementation of a compiler for a context-free grammar, featuring:

1. **Lexical Analysis Phase**
   - Regular Expression to NFA conversion
   - NFA to DFA conversion with state minimization
   - Token generation and lexical error detection

2. **Syntax Analysis Phase**
   - LL(1) grammar parsing
   - First and Follow set computation
   - LL(1) parsing table generation
   - Syntax error detection and reporting

3. **Code Generation Phase**
   - Leftmost derivation tracking
   - Parse tree construction
   - Token value assignments

## Project Structure

```
.
├── Lexical Analyzer/          # Lexical analysis components
│   ├── NFA.h / NFA.cpp        # NFA representation and conversion
│   ├── DFA.h / DFA.cpp        # DFA representation
│   ├── input_parser.h / input_parser.cpp  # Input parsing
│   └── input handler.h / input handler.cpp
├── Parser/                    # Syntax analysis components
│   ├── GrammarParser.h / GrammarParser.cpp        # Grammar parsing
│   ├── FirstFollow.h / FirstFollow.cpp            # First/Follow computation
│   ├── ParsingTable.h / ParsingTable.cpp          # LL(1) table generation
│   └── ParserOutput.h / ParserOutput.cpp          # Output generation
├── input files/               # Input data
│   ├── grammer.txt           # Context-free grammar definition
│   ├── rules.txt             # Lexical rules (regular expressions)
│   └── test.txt              # Test input program
├── output files/              # Generated output
│   ├── DFA.txt               # DFA state transitions
│   ├── Tokens.txt            # Generated tokens
│   ├── Values.txt            # Token values/lexemes
│   ├── FirstFollow.txt       # First and Follow sets
│   ├── LL1 grammar.txt       # LL(1) grammar output
│   ├── Stack.csv             # Stack operations during parsing
│   ├── table.csv             # LL(1) parsing table
│   └── leftmostDerivation.txt # Leftmost derivation sequence
├── CMakeLists.txt            # CMake build configuration
└── main.cpp                  # Main entry point
```

## Input File Formats

### Rules File (`rules.txt`)
Regular expression-based lexical rules with priority:
```
rule_name = regex_pattern
```

### Grammar File (`grammer.txt`)
Context-free grammar in standard notation:
```
S -> E
E -> E + T | T
T -> T * F | F
F -> ( E ) | id | num
```

### Test File (`test.txt`)
Program code to be compiled (tokens separated appropriately)

## Building the Project

### Prerequisites
- C++17 compatible compiler (g++, clang, MSVC)
- CMake 3.24+

### On Linux/macOS:
```bash
mkdir -p build
cd build
cmake ..
make
```

### On Windows:
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Running the Compiler

```bash
./Compilers_Generator
```

The compiler will:
1. Read lexical rules from `input files/rules.txt`
2. Read the grammar from `input files/grammer.txt`
3. Read the test program from `input files/test.txt`
4. Generate output files in the `output files/` directory

## Output Files Description

| File | Description |
|------|-------------|
| `DFA.txt` | State transitions of the DFA for lexical analysis |
| `Tokens.txt` | List of recognized tokens from the input |
| `Values.txt` | Associated values/lexemes for each token |
| `FirstFollow.txt` | Computed First and Follow sets for the grammar |
| `LL1 grammar.txt` | Transformed LL(1) compatible grammar |
| `table.csv` | LL(1) parsing table (CSV format) |
| `Stack.csv` | Stack operations during parsing |
| `leftmostDerivation.txt` | Derivation sequence if parsing succeeds |

## Key Features

- **Cross-platform compatibility**: Works on Windows, Linux, and macOS
- **Comprehensive error handling**: Reports lexical and syntax errors
- **Detailed output**: Multiple output files for debugging and analysis
- **LL(1) parsing**: Efficient single-pass parsing with no lookahead limitations
- **State minimization**: Optimized DFA for lexical analysis

## Implementation Highlights

### Lexical Analysis
- Thompson's construction for NFA generation
- Subset construction for NFA to DFA conversion
- Priority-based token selection for ambiguous matches

### Syntax Analysis
- Recursive computation of First and Follow sets
- LL(1) parsing table construction with conflict detection
- Stack-based parsing with derivation tracking

### Code Quality
- Modular design with clear separation of concerns
- Comprehensive error checking for file I/O
- Formatted output for easy debugging and analysis

## Requirements Met

✅ Full lexical analysis pipeline (NFA → DFA)  
✅ LL(1) grammar parsing and table generation  
✅ Token generation and identification  
✅ Leftmost derivation tracking  
✅ Multiple output formats for analysis  
✅ Cross-platform file handling  
✅ Comprehensive error detection  

## Future Enhancements

- Support for more sophisticated grammar classes (LR parsing)
- LLVM backend for code generation
- Interactive debugging mode
- Performance optimizations for large inputs
- GUI for visualization of NFA/DFA
- Semantic analysis phase

## License

This is an educational project for the Compilers course.

## Author

Ibrahim Tarek

## Course

Compilers Course - CSED (Computer Science and Engineering Department)

---

**Note**: Ensure input files are properly formatted in the `input files/` directory before running the compiler. Refer to example files for correct format.
