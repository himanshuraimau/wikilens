#include "wikitext_stripper.h"

std::string WikitextStripper::clean(const std::string& text) {

    std::string output = text;

    output = stripComments(output);
    output = stripRefTags(output);
    output = stripTemplates(output);
    output = stripTables(output);
    output = processLinks(output);
    output = stripFormatting(output);

    return output;
}

std::string WikitextStripper::stripComments(const std::string& text) {
    std::string result;
    bool in_comment = false;

    for (size_t i = 0; i < text.size(); ++i) {

        if (!in_comment &&
            i + 3 < text.size() &&
            text.substr(i, 4) == "<!--") {
            in_comment = true;
            i += 3;
            continue;
        }

        if (in_comment &&
            i + 2 < text.size() &&
            text.substr(i, 3) == "-->") {
            in_comment = false;
            i += 2;
            continue;
        }

        if (!in_comment)
            result += text[i];
    }

    return result;
}

std::string WikitextStripper::stripTemplates(const std::string& text) {

    std::string result;
    int depth = 0;

    for (size_t i = 0; i < text.size(); ++i) {

        if (i + 1 < text.size() &&
            text[i] == '{' && text[i+1] == '{') {
            depth++;
            i++;
            continue;
        }

        if (i + 1 < text.size() &&
            text[i] == '}' && text[i+1] == '}') {
            if (depth > 0) depth--;
            i++;
            continue;
        }

        if (depth == 0)
            result += text[i];
    }

    return result;
}

std::string WikitextStripper::stripRefTags(const std::string& text) {

    std::string result;
    bool in_ref = false;

    for (size_t i = 0; i < text.size(); ++i) {

        if (!in_ref &&
            i + 4 < text.size() &&
            text.substr(i, 4) == "<ref") {
            in_ref = true;
            continue;
        }

        if (in_ref &&
            i + 5 < text.size() &&
            text.substr(i, 6) == "</ref>") {
            in_ref = false;
            i += 5;
            continue;
        }

        if (!in_ref)
            result += text[i];
    }

    return result;
}

std::string WikitextStripper::stripTables(const std::string& text) {

    std::string result;
    bool in_table = false;

    for (size_t i = 0; i < text.size(); ++i) {

        if (!in_table &&
            i + 1 < text.size() &&
            text[i] == '{' && text[i+1] == '|') {
            in_table = true;
            i++;
            continue;
        }

        if (in_table &&
            i + 1 < text.size() &&
            text[i] == '|' && text[i+1] == '}') {
            in_table = false;
            i++;
            continue;
        }

        if (!in_table)
            result += text[i];
    }

    return result;
}

std::string WikitextStripper::processLinks(const std::string& text) {

    std::string result;
    for (size_t i = 0; i < text.size(); ++i) {

        // Internal link
        if (i + 1 < text.size() &&
            text[i] == '[' && text[i+1] == '[') {

            i += 2;
            std::string link_text;

            while (i < text.size() &&
                   !(text[i] == ']' && text[i+1] == ']')) {

                if (text[i] == '|') {
                    link_text.clear();
                    i++;
                    continue;
                }

                link_text += text[i];
                i++;
            }

            result += link_text;
            i++; // skip closing
            continue;
        }

        // External link
        if (text[i] == '[') {

            size_t space_pos = text.find(' ', i);
            size_t close_pos = text.find(']', i);

            if (space_pos != std::string::npos &&
                close_pos != std::string::npos &&
                space_pos < close_pos) {

                result += text.substr(space_pos + 1,
                                      close_pos - space_pos - 1);

                i = close_pos;
                continue;
            }
        }

        result += text[i];
    }

    return result;
}

std::string WikitextStripper::stripFormatting(const std::string& text) {

    std::string result;
    size_t i = 0;

    while (i < text.size()) {

        // Strip bold+italic: '''''
        if (i + 4 < text.size() && text.substr(i, 5) == "'''''") {
            i += 5;
            continue;
        }

        // Strip bold: '''
        if (i + 2 < text.size() && text.substr(i, 3) == "'''") {
            i += 3;
            continue;
        }

        // Strip italic: ''
        if (i + 1 < text.size() && text.substr(i, 2) == "''") {
            i += 2;
            continue;
        }

        // Strip heading markers (== ... ==) — replace with a space
        if (text[i] == '=') {
            while (i < text.size() && text[i] == '=')
                i++;
            result += ' ';
            continue;
        }

        result += text[i];
        i++;
    }

    return result;
}
