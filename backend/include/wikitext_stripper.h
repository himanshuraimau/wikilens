#pragma once
#include <string>

class WikitextStripper {
public:
    std::string clean(const std::string& text);

private:
    std::string stripTemplates(const std::string& text);
    std::string stripRefTags(const std::string& text);
    std::string stripComments(const std::string& text);
    std::string stripTables(const std::string& text);
    std::string processLinks(const std::string& text);
    std::string stripFormatting(const std::string& text);
};