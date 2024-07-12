#include <iostream>
#include <regex>
#include <string>

int main() {
    std::string input;
    std::cout << "Enter a transaction entry: ";
    std::getline(std::cin, input);

    std::regex noteRegex(R"(-n\s+([^"]*))");  // Regex to find the note
    std::smatch match;
    std::string note;
    bool hasNote = false;

    // Try to find and set the note
    if (std::regex_search(input, match, noteRegex) && match.size() > 1) {
        note = match[1];
        hasNote = true;
    }

    // Output results
    if (hasNote) {
        std::cout << "Note found: '" << note << "'" << std::endl;
    } else {
        std::cout << "No note found." << std::endl;
    }

    return 0;
}
