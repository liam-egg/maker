#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <fstream>
#include <sstream>
#include <regex>
#include <chrono>

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
    
    ofstream bf_out("brainfuck.txt");
    bf_out << bf_code;
    bf_out.close();

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

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }

    string filename = argv[1];
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Could not open \"" << filename << '"' << endl;
        return 1;
    }

    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    auto start = chrono::high_resolution_clock::now();

    content = brainfuck(content);
    cout << content << endl;

    ofstream boom_out("boom.txt");
    boom_out << content;
    boom_out.close();
    
    content = boom(content);
    cout << content << endl << endl;

    run(content);

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "Execution time: " << duration << " ms" << endl;

    return 0;
}