#include <iostream>
#include <string>
#include <vector>
#include <bitset>

int main() {
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> groups;
    std::string temp;
    
    for (char c : input) {
        if (c != ' ') temp += c;
    }
    for (size_t i = 0; i + 32 <= temp.size(); i += 32) {
        groups.push_back(temp.substr(i, 32));
    }
    if (temp.size() % 32 != 0) {
        groups.push_back(temp.substr(groups.size() * 32));
    }

    std::vector<std::string> bitstrings;
    std::vector<int> chars;
    std::string result;

    // Each group of 8*4=32 chars is 4 bytes
    for (const auto& group : groups) {
        for (size_t i = 0; i + 32 <= group.size(); i += 32) {
            std::string boom = group.substr(i, 32);
            std::string bits;
            for (size_t k = 0; k < 8; ++k) {
                std::string b = boom.substr(k * 4, 4);
                bits += (isupper(b[0]) ? '1' : '0');
            }
            bitstrings.push_back(bits);
            int val = std::bitset<8>(bits).to_ulong();
            chars.push_back(val);
            result += static_cast<char>(val);
        }
    }

    for (size_t i = 0; i < bitstrings.size(); ++i) {
        std::cout << bitstrings[i];
        if (i + 1 < bitstrings.size()) std::cout << " ";
    }
    std::cout << std::endl;

    for (size_t i = 0; i < chars.size(); ++i) {
        std::cout << chars[i];
        if (i + 1 < chars.size()) std::cout << " ";
    }
    
    std::cout << std::endl;
    std::cout << result << std::endl;

    return 0;
}
