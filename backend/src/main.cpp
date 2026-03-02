#include <iostream>
#include "tokenizer.h"
#include "inverted_index.h"
#include "query_engine.h"

int main() {
    Tokenizer tokenizer("../data/stopwords.txt");
    InvertedIndex index;

    std::vector<std::string> docs = {
        "Computer science studies algorithms and data structures",
        "Algorithms are fundamental to computer science",
        "Data science involves statistics and machine learning"
    };

    for (uint32_t i = 0; i < docs.size(); ++i) {
        auto tokens = tokenizer.tokenize(docs[i]);
        index.addDocument(i, tokens);
    }

    QueryEngine engine(index, tokenizer);

    auto results = engine.search("computer algorithms", 5);

    for (const auto& r : results) {
        std::cout << "Doc "
                  << r.doc_id
                  << " Score: "
                  << r.score << "\n";
    }

    return 0;
}