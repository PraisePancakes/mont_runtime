cmake_minimum_required(VERSION 3.28.0)

set(PROJECT_NAME montpiler)
set(EXEC mont)
project(PROJECT_NAME, VERSION 0.0.1)

add_executable(
    EXEC 
    src/main.cpp 
    src/frontend/tokenizer.cpp 
    src/frontend/parser.cpp 
    src/frontend/file_info.cpp 
    src/frontend/expressions/expression.cpp 
    src/frontend/interfaces/token.cpp
    src/frontend/interfaces/expression_base.cpp
    src/frontend/interfaces/statement_base.cpp
    src/frontend/statements/statement.cpp
    src/backend/interpreter.cpp
    src/mont.cpp
    src/frontend/parser_error.hpp
    src/frontend/environment.cpp
    )