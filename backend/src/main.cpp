#include <iostream>
#include <fstream>
#include <filesystem>
#include "tokenizer.h"
#include "inverted_index.h"
#include "query_engine.h"
#include "document_store.h"

namespace fs = std::filesystem;

std::string readFile(const std::string& path) {
    std::ifstream file(path);
    return std::string(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );
}

int main() {

    Tokenizer tokenizer("../data/stopwords.txt");
    InvertedIndex index;
    DocumentStore store;

    std::string corpus_path = "../data/corpus";

    uint32_t doc_id = 0;

    for (const auto& entry : fs::directory_iterator(corpus_path)) {

        if (!entry.is_regular_file()) continue;

        std::string path = entry.path().string();
        std::string filename = entry.path().filename().string();

        std::string content = readFile(path);

        auto tokens = tokenizer.tokenize(content);

        index.addDocument(doc_id, tokens);
        store.addDocument(doc_id, filename, path);

        std::cout << "Indexed: " << filename << "\n";

        doc_id++;
    }

    QueryEngine engine(index, tokenizer);

    std::string query;
    std::cout << "\nEnter query: ";
    std::getline(std::cin, query);

    auto results = engine.search(query, 5);

    std::cout << "\nResults:\n";

    for (const auto& r : results) {
        const Document* doc = store.getDocument(r.doc_id);
        if (doc) {
            std::cout << doc->title
                      << " | Score: "
                      << r.score << "\n";
        }
    }

    return 0;
}