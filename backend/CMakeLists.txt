cmake_minimum_required(VERSION 3.20)
project(WikiLens)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wall -Wextra -O2)

include_directories(include)
include_directories(/usr/include)
include_directories(external/Crow/include)

add_executable(wikilens
    src/main.cpp
    src/indexer/tokenizer.cpp
    src/indexer/porter_stemmer.cpp
    src/indexer/inverted_index.cpp
    src/indexer/wiki_parser.cpp
    src/indexer/wikitext_stripper.cpp
    src/ranking/bm25.cpp
    src/query/query_engine.cpp
    src/storage/document_store.cpp
)

find_package(Threads REQUIRED)
find_package(EXPAT REQUIRED)

target_link_libraries(wikilens
    pthread
    EXPAT::EXPAT
)