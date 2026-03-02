#define ASIO_STANDALONE
#define CROW_MAIN
#include "crow.h"

#include <filesystem>
#include <fstream>
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

    // =========================
    // Build Index At Startup
    // =========================

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

        doc_id++;
    }

    QueryEngine engine(index, tokenizer);

    // =========================
    // HTTP Server
    // =========================

    crow::SimpleApp app;

    // Health check
    CROW_ROUTE(app, "/")
    ([](){
        return "WikiLens Search Engine Running";
    });

    // Search endpoint
    CROW_ROUTE(app, "/search")
    ([&](const crow::request& req){

        auto query = req.url_params.get("q");
        if (!query) {
            return crow::response(400, "Missing query parameter");
        }

        auto results = engine.search(query, 5);

        crow::json::wvalue response;
        response["results"] = crow::json::wvalue::list();

        for (size_t i = 0; i < results.size(); ++i) {

            const Document* doc =
                store.getDocument(results[i].doc_id);

            if (!doc) continue;

            crow::json::wvalue item;
            item["title"] = doc->title;
            item["score"] = results[i].score;

            response["results"][i] = std::move(item);
        }

        return crow::response(response);
    });

    app.port(8080).multithreaded().run();
}