#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <set>
#include <vector>

using namespace std;

// Token types
enum TokenType {
    KEY_WORD,
    IDENTIFIER,
    INT_CONSTANT,
    FLOAT_CONSTANT,
    CHAR_CONSTANT,
    STRING_CONSTANT,
    OPERATOR,
    DELIMITATOR,
    COMMENT,
    END_OF_FILE,
    ERROR
};

struct Token {
    string lexeme;
    TokenType type;
    int length;
    int startLine;
    int endLine;
    string error;
};

// C/C++ keywords
set<string> keywords = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "int", "long", "register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while",
    "class", "namespace", "using", "public", "private", "protected", "virtual",
    "bool", "true", "false", "this", "new", "delete", "try", "catch", "throw",
    "main", "printf", "scanf", "include", "define", "ifdef", "ifndef", "endif"
};

// Global scanner state variables
string inputText;
int currentPos = 0;
int currentLine = 1;


string tokenTypeToString(TokenType type) {
    switch(type) {
        case KEY_WORD: return "key_word";
        case IDENTIFIER: return "identifier";
        case INT_CONSTANT: return "int_constant";
        case FLOAT_CONSTANT: return "float_constant";
        case CHAR_CONSTANT: return "char_constant";
        case STRING_CONSTANT: return "string_constant";
        case OPERATOR: return "operator";
        case DELIMITATOR: return "delimitator";
        case COMMENT: return "comment";
        case END_OF_FILE: return "end_of_file";
        case ERROR: return "error";
        default: return "unknown";
    }
}


bool isDelimiter(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' ||
           c == '[' || c == ']' || c == ';' || c == ',' || c == '.';
}

bool isOperatorChar(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' ||
           c == '=' || c == '!' || c == '<' || c == '>' || c == '&' ||
           c == '|' || c == '^' || c == '~' || c == '?' || c == ':';
}

// returns next token
Token getNextToken() {
    Token token;
    token.type = ERROR;
    token.length = 0;
    token.startLine = currentLine;
    token.endLine = currentLine;
    token.error = "";

    // Skip whitespace, tabs, newlines
    while (currentPos < inputText.length() &&
           (inputText[currentPos] == ' ' || inputText[currentPos] == '\t' ||
            inputText[currentPos] == '\n' || inputText[currentPos] == '\r')) {
        if (inputText[currentPos] == '\n') {
            currentLine++;
        }
        currentPos++;
    }

    // Check for end of file
    if (currentPos >= inputText.length()) {
        token.type = END_OF_FILE;
        token.length = 0;
        return token;
    }

    token.startLine = currentLine;
    token.endLine = currentLine;
    int startPos = currentPos;
    char currentChar = inputText[currentPos];

    // Comments
    if (currentChar == '/' && currentPos + 1 < inputText.length()) {
        // Single line comment //
        if (inputText[currentPos + 1] == '/') {
            int start = currentPos;
            currentPos += 2;
            while (currentPos < inputText.length() && inputText[currentPos] != '\n') {
                currentPos++;
            }
            token.lexeme = inputText.substr(start, currentPos - start);
            token.type = COMMENT;
            token.length = currentPos - start;
            return token;
        }
        // Block comment /* */
        else if (inputText[currentPos + 1] == '*') {
            int start = currentPos;
            int startLineComment = currentLine;
            currentPos += 2;
            bool closed = false;
            while (currentPos < inputText.length()) {
                if (inputText[currentPos] == '\n') {
                    currentLine++;
                    token.endLine = currentLine;
                }
                if (inputText[currentPos] == '*' && currentPos + 1 < inputText.length() &&
                    inputText[currentPos + 1] == '/') {
                    currentPos += 2;
                    closed = true;
                    break;
                }
                currentPos++;
            }
            if (!closed) {
                token.error = "Unclosed comment";
                token.type = ERROR;
                token.startLine = startLineComment;
                token.lexeme = inputText.substr(start, currentPos - start);
                token.length = currentPos - start;
                return token;
            }
            token.lexeme = inputText.substr(start, currentPos - start);
            token.type = COMMENT;
            token.length = currentPos - start;
            token.startLine = startLineComment;
            return token;
        }
    }

    // Identifiers and keywords
    if (isalpha(currentChar) || currentChar == '_') {
        int start = currentPos;
        while (currentPos < inputText.length() &&
               (isalnum(inputText[currentPos]) || inputText[currentPos] == '_')) {
            currentPos++;
        }
        token.lexeme = inputText.substr(start, currentPos - start);
        token.length = currentPos - start;

        if (keywords.find(token.lexeme) != keywords.end()) {
            token.type = KEY_WORD;
        } else {
            token.type = IDENTIFIER;
        }
        return token;
    }

    // Numeric constants
    if (isdigit(currentChar)) {
        int start = currentPos;
        bool isFloat = false;

        // Read integer part
        while (currentPos < inputText.length() && isdigit(inputText[currentPos])) {
            currentPos++;
        }

        // Check for float with decimal point
        if (currentPos < inputText.length() && inputText[currentPos] == '.') {
            // Look ahead to see if next char is a digit
            if (currentPos + 1 < inputText.length() && isdigit(inputText[currentPos + 1])) {
                isFloat = true;
                currentPos++; // skip '.'
                while (currentPos < inputText.length() && isdigit(inputText[currentPos])) {
                    currentPos++;
                }
            }
            // If '.' is not followed by a digit, it's a separate delimitator
        }

        // Check for exponent
        if (currentPos < inputText.length() &&
            (inputText[currentPos] == 'e' || inputText[currentPos] == 'E')) {
            isFloat = true;
            currentPos++;
            if (currentPos < inputText.length() &&
                (inputText[currentPos] == '+' || inputText[currentPos] == '-')) {
                currentPos++;
            }
            while (currentPos < inputText.length() && isdigit(inputText[currentPos])) {
                currentPos++;
            }
        }

        token.lexeme = inputText.substr(start, currentPos - start);
        token.length = currentPos - start;
        token.type = isFloat ? FLOAT_CONSTANT : INT_CONSTANT;
        return token;
    }

    // Float starting with decimal point
    if (currentChar == '.' && currentPos + 1 < inputText.length() &&
        isdigit(inputText[currentPos + 1])) {
        int start = currentPos;
        currentPos++; // skip '.'
        while (currentPos < inputText.length() && isdigit(inputText[currentPos])) {
            currentPos++;
        }

        // Check for exponent
        if (currentPos < inputText.length() &&
            (inputText[currentPos] == 'e' || inputText[currentPos] == 'E')) {
            currentPos++;
            if (currentPos < inputText.length() &&
                (inputText[currentPos] == '+' || inputText[currentPos] == '-')) {
                currentPos++;
            }
            while (currentPos < inputText.length() && isdigit(inputText[currentPos])) {
                currentPos++;
            }
        }

        token.lexeme = inputText.substr(start, currentPos - start);
        token.length = currentPos - start;
        token.type = FLOAT_CONSTANT;
        return token;
    }

    // Character constants
    if (currentChar == '\'') {
        int start = currentPos;
        currentPos++;
        bool validChar = false;

        if (currentPos < inputText.length()) {
            // Handle escape sequences
            if (inputText[currentPos] == '\\' && currentPos + 1 < inputText.length()) {
                currentPos += 2;
            } else if (inputText[currentPos] != '\'') {
                currentPos++;
            }

            // Check for closing quote
            if (currentPos < inputText.length() && inputText[currentPos] == '\'') {
                currentPos++;
                validChar = true;
            }
        }

        token.lexeme = inputText.substr(start, currentPos - start);
        token.length = currentPos - start;

        if (!validChar) {
            token.error = "Invalid character constant";
            token.type = ERROR;
        } else {
            token.type = CHAR_CONSTANT;
        }
        return token;
    }

    // String constants
    if (currentChar == '"') {
        int start = currentPos;
        int startLineString = currentLine;
        currentPos++;
        bool closed = false;

        while (currentPos < inputText.length()) {
            if (inputText[currentPos] == '\n') {
                currentLine++;
                token.endLine = currentLine;
            }
            if (inputText[currentPos] == '\\' && currentPos + 1 < inputText.length()) {
                currentPos += 2; // skip escape sequence
                continue;
            }
            if (inputText[currentPos] == '"') {
                currentPos++;
                closed = true;
                break;
            }
            currentPos++;
        }

        token.lexeme = inputText.substr(start, currentPos - start);
        token.length = currentPos - start;
        token.startLine = startLineString;

        if (!closed) {
            token.error = "Unclosed string";
            token.type = ERROR;
        } else {
            token.type = STRING_CONSTANT;
        }
        return token;
    }

    // Delimitators (but NOT . if followed by digit)
    if (isDelimiter(currentChar)) {
        // Special case: '.' followed by digit is a float, not delimitator
        if (!(currentChar == '.' && currentPos + 1 < inputText.length() &&
              isdigit(inputText[currentPos + 1]))) {
            token.lexeme = string(1, currentChar);
            token.type = DELIMITATOR;
            token.length = 1;
            currentPos++;
            return token;
        }
    }

    // Operators
    if (isOperatorChar(currentChar)) {
        int start = currentPos;
        string op(1, currentChar);
        currentPos++;

        // Check for 2-3 character operators
        if (currentPos < inputText.length()) {
            string twoChar = op + inputText[currentPos];
            // Two character operators
            if (twoChar == "++" || twoChar == "--" || twoChar == "==" ||
                twoChar == "!=" || twoChar == "<=" || twoChar == ">=" ||
                twoChar == "&&" || twoChar == "||" || twoChar == "<<" ||
                twoChar == ">>" || twoChar == "+=" || twoChar == "-=" ||
                twoChar == "*=" || twoChar == "/=" || twoChar == "%=" ||
                twoChar == "&=" || twoChar == "|=" || twoChar == "^=" ||
                twoChar == "->" || twoChar == "::") {
                currentPos++;
                op = twoChar;

                // Check for three character operators
                if (currentPos < inputText.length()) {
                    string threeChar = op + inputText[currentPos];
                    if (threeChar == "<<=" || threeChar == ">>=") {
                        currentPos++;
                        op = threeChar;
                    }
                }
            }
        }

        token.lexeme = op;
        token.type = OPERATOR;
        token.length = currentPos - start;
        return token;
    }

    // Illegal character
    token.error = "Illegal character: '" + string(1, currentChar) + "'";
    token.type = ERROR;
    token.lexeme = string(1, currentChar);
    token.length = 1;
    currentPos++;
    return token;
}

int main() {
    string filename;
    cout << "Enter input file name: ";
    cin >> filename;

    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    inputText = string((istreambuf_iterator<char>(inputFile)),
                       istreambuf_iterator<char>());
    inputFile.close();


    while (true) {
        Token token = getNextToken();

        if (token.type == END_OF_FILE) {
            cout << "End of file reached." << endl;
            break;
        }

        if (token.type == ERROR) {
            cout << "ERROR: " << token.error;
            if (token.startLine == token.endLine) {
                cout << "; line " << token.startLine << endl;
            } else {
                cout << "; lines " << token.startLine << "-" << token.endLine << endl;
            }
            continue;
        }

        // Display token
        cout << "'" << token.lexeme << "', " << tokenTypeToString(token.type)
             << "; " << token.length << "; ";

        if (token.startLine == token.endLine) {
            cout << "line " << token.startLine << endl;
        } else {
            cout << "lines " << token.startLine << "-" << token.endLine << endl;
        }
    }
    
    return 0;
}