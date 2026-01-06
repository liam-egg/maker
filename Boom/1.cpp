#include <iostream>
#include <string>

int main() {
    std::string input;
    std::getline(std::cin, input);

    std::string result;
    size_t pos = 0;
    while (pos < input.size()) {
        if (input.substr(pos, 4) == "boom") {
            result += '0';
            pos += 4;
        } else if (input.substr(pos, 4) == "BOOM") {
            result += '1';
            pos += 4;
        } else {
            ++pos;
        }
    }

    std::cout << result << std::endl;
    return 0;
}