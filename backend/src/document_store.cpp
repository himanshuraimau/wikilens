#include "document_store.h"

void DocumentStore::addDocument(uint32_t doc_id,
                                const std::string& title,
                                const std::string& path) {

    docs_[doc_id] = {doc_id, title, path};
}

const Document*
DocumentStore::getDocument(uint32_t doc_id) const {

    auto it = docs_.find(doc_id);
    if (it == docs_.end()) return nullptr;
    return &it->second;
}