cmake_minimum_required(VERSION 3.20)
project(WikiLens)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wall -Wextra -O2)

include_directories(include)
include_directories(/usr/include)
include_directories(external/Crow/include)

add_executable(wikilens
    src/main.cpp
    src/tokenizer.cpp
    src/porter_stemmer.cpp
    src/inverted_index.cpp
    src/bm25.cpp
    src/query_engine.cpp
    src/document_store.cpp
)

find_package(Threads REQUIRED)

target_link_libraries(wikilens pthread)