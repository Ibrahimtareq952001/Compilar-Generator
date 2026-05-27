<img width="100%" src="https://capsule-render.vercel.app/api?type=waving&color=gradient&customColorList=24,18,12&height=180&section=header&text=Java-to-Bytecode%20Compiler&fontSize=36&fontColor=fff&animation=twinkling&fontAlignY=38&desc=NFA%E2%86%92DFA%20Lexer%20%2B%20LL(1)%20Parser%20%E2%80%94%20Implemented%20in%20C%2B%2B&descAlignY=58&descSize=16&descColor=cbd5e1"/>

<div align="center">

![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=flat-square&logo=c%2B%2B&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-064F8C?style=flat-square&logo=cmake&logoColor=white)

</div>

---

## Overview

A fully hand-written **multi-phase compiler** implemented in C++, built as a course project for the Programming Languages & Compilers course at Alexandria University. Covers the complete front-end pipeline from raw source text to a parsed token stream with leftmost derivations.

---

## Compiler Phases

### Phase 1 — Lexical Analysis (NFA → DFA)

```
Regex Specs  ──►  NFA (Thompson's)  ──►  DFA (Subset Construction)  ──►  Token Stream
```

- Converts regular expression specs to **NFA** using Thompson's construction
- Converts NFA to **minimized DFA** via subset construction + state minimization
- Runs DFA simulation to tokenize the input source file
- Detects and reports lexical errors

### Phase 2 — Syntax Analysis (LL(1) Parser)

```
Token Stream  ──►  FIRST/FOLLOW sets  ──►  LL(1) Table  ──►  Leftmost Derivation
```

- Computes **FIRST** and **FOLLOW** sets from context-free grammar rules
- Builds the **LL(1) predictive parsing table**
- Drives a top-down parser producing the leftmost derivation sequence
- Detects and reports syntax errors with recovery

---

## Project Structure

```
Compilar-Generator/
├── Lexical Analyzer/
│   ├── NFA.h / NFA.cpp              # Thompson's NFA construction
│   ├── DFA.h / DFA.cpp              # Subset construction + minimization
│   ├── input_parser.h/cpp           # Regex spec parser
│   └── input handler.h/cpp          # Source file tokenizer
├── Parser/
│   ├── GrammarParser.h/cpp          # CFG rule reader
│   ├── FirstFollow.h/cpp            # FIRST / FOLLOW set computation
│   ├── ParsingTable.h/cpp           # LL(1) table builder
│   └── ParserOutput.h/cpp           # Derivation output
├── input files/                     # Sample grammars + test inputs
├── main.cpp                         # Entry point
└── CMakeLists.txt
```

---

## Example

**Input grammar rule:**
```
STMT → if EXPR then STMT | id := EXPR
EXPR → TERM EXPR'
...
```

**Generated FIRST/FOLLOW sets:**
```
FIRST(STMT)  = { if, id }
FOLLOW(STMT) = { $, else, end }
```

**Sample token stream output:**
```
<keyword, if>  <id, x>  <relop, >>  <num, 0>  <keyword, then>  ...
```

---

## Build & Run

### Prerequisites
- C++17 compiler (g++ or clang++)
- CMake 3.15+

```bash
git clone https://github.com/Ibrahimtareq952001/Compilar-Generator.git
cd Compilar-Generator

mkdir build && cd build
cmake ..
make

# Run with sample input
./compiler ../input\ files/sample_grammar.txt ../input\ files/sample_program.txt
```

---

<div align="center">

*Programming Languages & Compilers — Alexandria University 2025*

[![Resume](https://img.shields.io/badge/View_Resume-PDF-008080?style=flat-square&logo=latex&logoColor=white)](https://github.com/Ibrahimtareq952001/Resume/blob/main/resume.pdf)
[![Portfolio](https://img.shields.io/badge/GitHub-Ibrahimtareq952001-181717?style=flat-square&logo=github&logoColor=white)](https://github.com/Ibrahimtareq952001)

</div>

<img width="100%" src="https://capsule-render.vercel.app/api?type=waving&color=gradient&customColorList=24,18,12&height=100&section=footer"/>
