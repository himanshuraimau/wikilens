#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>

struct Document {
    uint32_t doc_id;
    std::string title;
    std::string path;
};

class DocumentStore {
public:
    void addDocument(uint32_t doc_id,
                     const std::string& title,
                     const std::string& path);

    const Document* getDocument(uint32_t doc_id) const;

private:
    std::unordered_map<uint32_t, Document> docs_;
};