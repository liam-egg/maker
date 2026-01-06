#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <fstream>
#include <sstream>

using namespace std;

void run (const string& code) {
    ofstream file("temp.cpp");
    file << code;
    file.close();

    if (system("g++ temp.cpp -o temp.out") != 0) {
        cerr << "Compilation failed\n";
        return;
    }

    system("temp.out");

    remove("temp.cpp");
    remove("temp.out");
}

int main() {
    string input;
    getline(cin, input);

    vector<string> groups;
    string temp;
    
    for (char c : input) {
        if (c != ' ') temp += c;
    }
    for (size_t i = 0; i + 32 <= temp.size(); i += 32) {
        groups.push_back(temp.substr(i, 32));
    }
    if (temp.size() % 32 != 0) {
        groups.push_back(temp.substr(groups.size() * 32));
    }

    vector<string> bitstrings;
    vector<int> chars;
    string result;

    // Each group of 8*4=32 chars is 4 bytes
    for (const auto& group : groups) {
        for (size_t i = 0; i + 32 <= group.size(); i += 32) {
            string boom = group.substr(i, 32);
            string bits;
            for (size_t k = 0; k < 8; ++k) {
                string b = boom.substr(k * 4, 4);
                bits += (isupper(b[0]) ? '1' : '0');
            }
            bitstrings.push_back(bits);
            int val = bitset<8>(bits).to_ulong();
            chars.push_back(val);
            result += static_cast<char>(val);
        }
    }

    cout << endl << result << endl << endl;
    run(result);
    

    return 0;
}