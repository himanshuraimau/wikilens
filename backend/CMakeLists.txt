# cmake_minimum_required(VERSION 3.20)
# project(WikiLens VERSION 1.0 LANGUAGES CXX)

# set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS OFF)

# add_compile_options(-Wall -Wextra -O2)

# add_executable(wikilens
#     src/main.cpp
#     src/tokenizer.cpp
#     src/inverted_index.cpp
#     src/bm25.cpp
# )

# target_include_directories(wikilens PRIVATE include)




# Minimal Version
cmake_minimum_required(VERSION 3.20)
project(WikiLens VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -O2)

add_executable(wikilens
    src/main.cpp
    src/tokenizer.cpp
    src/porter_stemmer.cpp
    src/inverted_index.cpp
)

target_include_directories(wikilens PRIVATE include)