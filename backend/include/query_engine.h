#pragma once
#include <vector>
#include <string>
#include "inverted_index.h"
#include "bm25.h"
#include "tokenizer.h"

struct SearchResult {
    uint32_t doc_id;
    double score;
};

class QueryEngine {
public:
    QueryEngine(InvertedIndex& index,
                Tokenizer& tokenizer);

    std::vector<SearchResult>
    search(const std::string& query,
           size_t top_k = 10);

private:
    InvertedIndex& index_;
    Tokenizer& tokenizer_;
    BM25 scorer_;
};