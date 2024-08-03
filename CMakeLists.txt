cmake_minimum_required(VERSION 3.28.0)

set(PROJECT_NAME montpiler)
set(EXEC mont)
project(PROJECT_NAME, VERSION 0.0.1)

add_executable(
    EXEC 
    src/main.cpp 
    src/interpreter.cpp 
    src/error_manager.cpp 
    src/error.cpp 
    src/frontend/tokenizer.cpp 
    src/frontend/parser.cpp 
    src/frontend/lexer.cpp 
    src/frontend/file_info.cpp 
    src/frontend/interfaces/expression.cpp 
    src/frontend/interfaces/lexeme.hpp
    src/frontend/interfaces/token.hpp
    )