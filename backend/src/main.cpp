#define ASIO_STANDALONE
#define CROW_MAIN
#include "crow.h"

#include <cstdint>
#include <iostream>
#include <filesystem>
#include <fstream>
#include "tokenizer.h"
#include "inverted_index.h"
#include "query_engine.h"
#include "document_store.h"
#include "wiki_parser.h"
#include "wikitext_stripper.h"

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
    WikitextStripper stripper;

    // Parse Wikipedia XML dump
    WikiParser parser;
    uint32_t doc_id = 0;

    parser.parse("../simplewiki-latest-pages-articles.xml.1",
        [&](const std::string& title,
            const std::string& text) {

            std::string cleaned = stripper.clean(text);
            auto tokens = tokenizer.tokenize(cleaned);

            index.addDocument(doc_id, tokens);
            store.addDocument(doc_id, title, cleaned);

            doc_id++;

            if (doc_id % 1000 == 0) {
                std::cout << "Indexed "
                          << doc_id
                          << " pages\n";
            }
        }
    );

    std::cout << "Finished indexing " << doc_id << " pages\n";

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
    .methods("GET"_method)
    ([&](const crow::request& req){
        crow::response res;
        res.add_header("Access-Control-Allow-Origin", "*");

        auto query = req.url_params.get("q");
        if (!query) {
            res.code = 400;
            res.body = "Missing query parameter";
            return res;
        }

        auto results = engine.search(query, 20);

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

        res.body = response.dump();
        res.add_header("Content-Type", "application/json");
        return res;
    });

    app.port(8080).multithreaded().run();
}