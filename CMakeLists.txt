cmake_minimum_required(VERSION 3.24)
project(Compilers_Generator)

set(CMAKE_CXX_STANDARD 17)

add_executable(Compilers_Generator main.cpp Parser/FirstFollow.h Parser/FirstFollow.cpp Parser/GrammarParser.h Parser/GrammarParser.cpp Parser/ParserOutput.h Parser/ParserOutput.cpp Parser/ParsingTable.h Parser/ParsingTable.cpp "Lexical Analyzer/NFA.h" "Lexical Analyzer/NFA.cpp" "Lexical Analyzer/DFA.h" "Lexical Analyzer/DFA.cpp" "Lexical Analyzer/input handler.cpp" "Lexical Analyzer/input handler.h" "Lexical Analyzer/input_parser.h" "Lexical Analyzer/input_parser.cpp")
