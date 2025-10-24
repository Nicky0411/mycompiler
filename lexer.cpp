#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Token {
    int type;
    string value;
};

static const int keywords_size = 8;
static vector<string> keywords = {"int", "if", "else", "while", "break", "continue", "return", "void"};
static vector<string> types =  {"Ident", "IntConst", "'int'", "'if'", "'else'", 
                                "'while'", "'break'", "'continue'", "'return'", "'void'", 
                                "'+'", "'-'", "'*'", "'/'", "'%'", 
                                "'<'", "'<='", "'>'", "'>='", "'=='", 
                                "'!='", "'!'", "'='", "';'", "','", 
                                "'('", "')'", "'{'", "'}'", "'||'", 
                                "'&&'"};

char getch(string &input, int &pos) {
    if (pos >= input.size()) return '\0';
    return input[pos++];
}
char getnbc(string &input, int &pos) {
    char c;
    do {
        c = getch(input, pos);
    } while (c == ' ' || c == '\n' || c == '\t');
    return c;
}
bool isLetter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}
bool isDigit(char c) {
    return c >= '0' && c <= '9';
}
int reserve(string &word) {
    for (int i = 0; i < keywords_size; i++) {
        if (word == keywords[i]) 
            return i + 2; // keyword token type starts from 2
    }
    return 0; // not a keyword but an identifier
}
bool read(string &input, int &pos, Token &token) {
    while (true) {
        string word = "";
        char peek = getnbc(input, pos);
        if (peek == '\0') {
            return false;
        }
        if (isLetter(peek)) {
            while(isLetter(peek) || isDigit(peek)) {
                word += peek;
                peek = getch(input, pos);
            }
            pos--; // retract
            int type = reserve(word);
            token = {type, word};
        } else if (isDigit(peek)) {
            while(isDigit(peek)) {
                word += peek;
                peek = getch(input, pos);
            }
            pos--; // retract
            token = {1, word}; // IntConst token type is 1
        } else {
            switch(peek) {
                case '+': token = {10, "+"}; break;
                case '-': token = {11, "-"}; break;
                case '*': token = {12, "*"}; break;
                // case '/': token = {13, "/"}; break;
                case '/': {
                    peek = getch(input,pos);
                    if (peek == '/') { // single line comment
                        while (peek != '\n' && peek != '\0') {
                            peek = getch(input, pos);
                        }
                        pos--; // retract
                        continue; // read next token
                    } else if (peek == '*') { // multi-line comment
                        char prev = '\0'; // previous character
                        while (true) {
                            peek = getch(input, pos);
                            if (peek == '\0') {
                                cout << "Error: unclosed comment\n";
                                exit(1);
                            }
                            if (prev == '*' && peek == '/') break; // end of comment
                            prev = peek;
                        }
                        continue; // read next token
                    } else {
                        pos--; // retract
                        token = {13, "/"}; // '/' token type is 13
                    }
                    break;
                }
                case '%': token = {14, "%"}; break;
                case '<': {
                    peek = getch(input, pos);
                    if (peek == '=') token = {16, "<="};
                    else {
                        pos--; // retract
                        token = {15, "<"};
                    }
                    break;
                }
                case '>': {
                    peek = getch(input, pos);
                    if (peek == '=') token = {18, ">="};
                    else {
                        pos--; // retract
                        token = {17, ">"};
                    }
                    break;
                }
                case '=': {
                    peek = getch(input, pos);
                    if (peek == '=') token = {19, "=="};
                    else {
                        pos--; // retract
                        token = {22, "="};
                    }
                    break;
                }
                case '!': {
                    peek = getch(input, pos);
                    if (peek == '=') token = {20, "!="};
                    else {
                        pos--; // retract
                        token = {21, "!"};
                    }
                    break;
                }
                case ';': token = {23, ";"}; break;
                case ',': token = {24, ","}; break;
                case '(': token = {25, "("}; break;
                case ')': token = {26, ")"}; break;
                case '{': token = {27, "{"}; break;
                case '}': token = {28, "}"}; break;
                case '|': {
                    peek = getch(input, pos);
                    if (peek == '|') token = {29, "||"};
                    else {
                        cout << "Error: invalid character '|" << peek << "'\n";
                        exit(1);
                    }
                    break;
                }
                case '&': {
                    peek = getch(input, pos);
                    if (peek == '&') token = {30, "&&"};
                    else {
                        cout << "Error: invalid character '&" << peek << "'\n";
                        exit(1);
                    }
                    break;
                }
                default: {
                    cout << "Error: invalid character '" << peek << "'\n";
                    exit(1);
                }
            }
        }
        return true;
    }
}

int main() {
    string input, line;
    while(getline(cin, line)) {
        input += line + '\n';
    }
    int pos = 0; // pointer to traverse the input
    int tokenID = 0;
    Token token;
    while(read(input, pos, token)) {
        cout << tokenID++ << ':' << types[token.type] << ":\"" << token.value << '\"' << endl;
    }
    return 0;
}