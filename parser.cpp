#include <string>
#include <iostream>
#include <queue>
#include <regex>
#include <fstream>
using namespace std;

enum class Commands {
    F, B, R, L, U, D, M,
    f, b, r, l, u, d, m,
    x, y, z, X, Y, Z
};

void printFile(const char* filename) {
    ifstream file(filename);
    string line;
    
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
        return;
    }
    
    while (getline(file, line)) {
        cout << line << endl;
    }
    file.close();
}


queue <char> parse_input(string& str) {
    regex pattern("([FBRLUDMxyz](2|')?)");
    queue <char> result;
    auto begin = sregex_iterator(str.begin(), str.end(), pattern);
    auto end = sregex_iterator();

    for (regex_iterator i = begin; i != end; ++i) {
        smatch match = *i;
        string temp = match.str();

        if (temp.length() == 1) {
            result.push(temp[0]);
        }
        else if (temp[1] == '2') {
            result.push(temp[0]);
            result.push(temp[0]);
        }    
        else if (temp[1] == '\''){
            result.push(temp[0] + 32);
        }
    }
    return result;
}

int main() {
    string input = "start";
    string end = "end";
    queue <char> output;
    cout << "Commands: help, end, clear" << endl;
    while (input != "end") {
        cin >> input;   

        if (input == "help") {
            printFile("command_list");
            continue;
        }

        else if (input == "clear") {
            cout << "\033[2J\033[1;1H";
        }
        else {
            output = parse_input(input);
            while (! output.empty()) {
                cout << output.front() << ' ';
                output.pop();
            }
            cout << endl;
        }
    }
}