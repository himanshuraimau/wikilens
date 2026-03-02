#include <iostream>
#include "tokenizer.h"
#include "inverted_index.h"

int main() {
    Tokenizer tokenizer("../data/stopwords.txt");
    InvertedIndex index;

    std::vector<std::string> docs = {
        "Computer science studies algorithms and data structures",
        "Algorithms are fundamental to computer science",
        "Data science and Algorithms involves statistics and machine learning"
    };

    for (uint32_t i = 0; i < docs.size(); ++i) {
        auto tokens = tokenizer.tokenize(docs[i]);
        index.addDocument(i, tokens);
    }

    auto plist = index.lookup("algorithm");

    if (plist) {
        std::cout << "Doc Frequency: "
                  << plist->doc_freq << "\n";

        for (const auto& posting : plist->postings) {
            std::cout << "DocID: "
                      << posting.doc_id
                      << " TF: "
                      << posting.term_freq << "\n";
        }
    } else {
        std::cout << "Term not found\n";
    }

    return 0;
}