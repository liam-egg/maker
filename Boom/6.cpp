#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <fstream>
#include <sstream>
#include <regex>

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

string brainfuck (string code) {
    unsigned char tape[30000] = { 0 };
    unsigned char* ptr = tape;

    char current;
    unsigned int i, loop;
    string out;
    
    string bf_code;
    for (i = 0; i < code.length(); i += 12) {
        string sub = code.substr(i, 12);
        if (sub == "boomboomboom") { // >
            bf_code += '>';
        } else if (sub == "boomboomBOOM") { // <
            bf_code += '<';
        } else if (sub == "boomBOOMboom") { // +
            bf_code += '+';
        } else if (sub == "boomBOOMBOOM") { // -
            bf_code += '-';
        } else if (sub == "BOOMboomboom") { // .
            bf_code += '.';
        } else if (sub == "BOOMboomBOOM") { // ,
            bf_code += ',';
        } else if (sub == "BOOMBOOMboom") { // [
            bf_code += '[';
        } else if (sub == "BOOMBOOMBOOM") { // ]
            bf_code += ']';
        }
    }
    

    for (i = 0; i < bf_code.length(); ++i) {
        current = bf_code[i];
        if (current == '>') {
            ++ptr;
        } else if (current == '<') {
            --ptr;
        } else if (current == '+') {
            ++*ptr;
        } else if (current == '-') {
            --*ptr;
        } else if (current == '.') {
            out += *ptr;
        } else if (current == ',') {
            *ptr = getchar();
        } else if (current == '[') {
            if (*ptr == 0) {
                loop = 1;
                while (loop > 0) {
                    ++i;
                    if (bf_code[i] == '[') loop++;
                    else if (bf_code[i] == ']') loop--;
                }
            }
        } else if (current == ']') {
            if (*ptr) {
                loop = 1;
                while (loop > 0) {
                    --i;
                    if (bf_code[i] == '[') loop--;
                    else if (bf_code[i] == ']') loop++;
                }
            }
        }
    }
    
    cout << endl << bf_code << endl << endl;
    return out;
}

string boom(const string& code) {
    istringstream stream(code);
    string line;
    vector<int> numbers;
    regex pattern(R"((\d+) BIG BOOMS)");
    
    while (getline(stream, line)) {
        smatch match;
        if (regex_match(line, match, pattern)) {
            if (match.size() < 2) return "Invalid format";
            int num = stoi(match[1].str());
            string boomLine;
            if (!getline(stream, boomLine)) return "Invalid format";
            
            istringstream boomStream(boomLine);
            string word;
            int count = 0;
            while (boomStream >> word) {
                if (word != "BOOM") return "Invalid format";
                count++;
            }
            
            if (count != num) return "Invalid format";
            numbers.push_back(num);
        }
    }
    
    string message;
    for (int num : numbers) {
        message += static_cast<char>(num);
    }
    
    return message;
}

int main () {
    string input;
    getline(cin, input);

    input = brainfuck(input);
    cout << input << endl << endl;
    
    input = boom(input);
    cout << input << endl << endl;

    run(input);

    return 0;
}