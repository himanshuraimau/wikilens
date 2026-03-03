#include "inverted_index.h"

void InvertedIndex::addDocument(
    uint32_t doc_id,
    const std::vector<std::string>& tokens
) {
    for (uint32_t pos = 0; pos < tokens.size(); ++pos) {
        const std::string& term = tokens[pos];
        auto& plist = index_[term];

        if (plist.postings.empty() ||
            plist.postings.back().doc_id != doc_id) {

            Posting p;
            p.doc_id = doc_id;
            p.term_freq = 1;
            p.positions.push_back(pos);

            plist.postings.push_back(p);
            plist.doc_freq++;
        }
        else {
            plist.postings.back().term_freq++;
            plist.postings.back().positions.push_back(pos);
        }
    }

    doc_lengths_[doc_id] = tokens.size();
    total_docs_++;
}

const PostingList*
InvertedIndex::lookup(const std::string& term) const {
    auto it = index_.find(term);
    if (it == index_.end()) return nullptr;
    return &it->second;
}

uint32_t InvertedIndex::totalDocs() const {
    return total_docs_;
}

uint32_t InvertedIndex::docLength(uint32_t doc_id) const {
    auto it = doc_lengths_.find(doc_id);
    if (it == doc_lengths_.end()) return 0;
    return it->second;
}

double InvertedIndex::avgDocLength() const {
    if (total_docs_ == 0) return 0.0;

    uint64_t sum = 0;
    for (const auto& [doc, len] : doc_lengths_)
        sum += len;

    return static_cast<double>(sum) / total_docs_;
}