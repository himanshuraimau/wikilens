#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "WikiLens Search Engine Starting...\n";

    if (argc > 1) {
        std::cout << "Query: " << argv[1] << "\n";
    }

    return 0;
}