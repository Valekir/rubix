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
    regex pattern1("([FBRLUDMXYZfbrludxyz]('2|2'|'|2)?)");
    queue <char> result;
    auto begin = sregex_iterator(str.begin(), str.end(), pattern1);
    auto end = sregex_iterator();

    if (begin == end) 
        cout << 123;

    for (regex_iterator i = begin; i != end; ++i) {
        smatch s = *i;
        string match = s.str();

        // Для того, чтобы парсер не зависел от регистра ввода
        char temp_char = tolower(match[0]);
        if (temp_char == 'x' || temp_char == 'y' || temp_char == 'z')
            match[0] = tolower(match[0]);
        else 
            match[0] = toupper(match[0]);
        
        if (match.length() != 1) {
            if (match[1] == '\'' || match.length() == 3) {
                if (match[0] < 'a')
                    match[0] = match[0] + ('a'-'A');
                else 
                    match[0] = match[0] - ('a'-'A');
            }
            if (match[1] == '2' || match.length() == 3)
                result.push(match[0]);
        }
        result.push(match[0]);
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