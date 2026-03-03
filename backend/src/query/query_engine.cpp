#include "query_engine.h"
#include <queue>
#include <algorithm>

QueryEngine::QueryEngine(InvertedIndex& index,
                         Tokenizer& tokenizer)
    : index_(index),
      tokenizer_(tokenizer),
      scorer_(index) {}

std::vector<SearchResult>
QueryEngine::search(const std::string& query,
                    size_t top_k) {

    auto terms = tokenizer_.tokenize(query);
    auto scores = scorer_.score(terms);

    // Min-heap for Top-K
    auto cmp = [](const SearchResult& a,
                  const SearchResult& b) {
        return a.score > b.score;
    };

    std::priority_queue<
        SearchResult,
        std::vector<SearchResult>,
        decltype(cmp)
    > heap(cmp);

    for (auto& [doc_id, score] : scores) {
        heap.push({doc_id, score});
        if (heap.size() > top_k)
            heap.pop();
    }

    std::vector<SearchResult> results;

    while (!heap.empty()) {
        results.push_back(heap.top());
        heap.pop();
    }

    // reverse to get highest first
    std::reverse(results.begin(), results.end());

    return results;
}