#ifndef UNTITLED13_TEXTCOLLECTION_H
#define UNTITLED13_TEXTCOLLECTION_H
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include "Allocator.h"
#include "Stack.h"
using string = std::string;

class TextCollection {
private:
    string activeFileName;
    std::vector<string> lines;
    Stack<std::vector<string>> undoStack;
    Stack<std::vector<string>> redoStack;

public:
    TextCollection() : activeFileName(""){}

    TextCollection(const string fileName) : activeFileName(fileName){}

    TextCollection(TextCollection& other)
            : activeFileName(other.activeFileName),
              lines(other.lines),
              undoStack(other.undoStack),
              redoStack(other.redoStack){}

    ~TextCollection(){
        lines.clear();
        undoStack.clear();
        redoStack.clear();
    }

    void saveToStack(){
        undoStack.push(lines);
    }

    void undo(){
        redoStack.push(lines);
        lines = undoStack.top();
        undoStack.pop();
    }

    void redo(){
        lines = redoStack.top();
        redoStack.pop();
    }

    void writeLine(){
        std::string userInput;
        std::cout << "Enter your text: ";
        std::getline(std::cin, userInput);
        lines.push_back(userInput);
        saveToStack();
    }

    void loadFile(const string& fileName) {
        std::ifstream file(fileName);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                lines.push_back(line);
            }
            file.close();
            activeFileName.assign(fileName);
        }
    }

    void searchAndReplace(const string& search, const string& replace) {
        saveToStack();
        for (string& line : lines) {
            size_t pos = line.find(search);
            while (pos != string::npos) {
                line.replace(pos, search.length(), replace);
                pos = line.find(search, pos + replace.length());
            }
        }
    }

    void convertToLowercase() {
        saveToStack();
        for (string& line : lines) {
            transform(line.begin(), line.end(), line.begin(), ::tolower);
        }
    }

    void convertToUppercase() {
        saveToStack();
        for (string& line : lines) {
            transform(line.begin(), line.end(), line.begin(), ::toupper);
        }
    }

    void trimLeadingAndTrailingSpaces() {
        saveToStack();
        for (string& line : lines) {
            size_t start = line.find_first_not_of(" \t");
            size_t end = line.find_last_not_of(" \t");
            if (start != string::npos && end != string::npos) {
                line = line.substr(start, end - start + 1);
            } else {
                line.clear();
            }
        }
    }

    const string trim(const string& str) {
        size_t start = str.find_first_not_of(" \t");
        size_t end = str.find_last_not_of(" \t");
        if (start == string::npos || end == string::npos) {
            return "";
        }
        return str.substr(start, end - start + 1);
    }

    void sortLines() {
        saveToStack();
        sort(lines.begin(), lines.end(), [](const string& a, const string& b) {
            if (isdigit(a[0]) && isdigit(b[0])) {
                return stoi(a) < stoi(b);
            }
            return a < b;
        });
    }

    void shuffleLines() {
        saveToStack();
        random_shuffle(lines.begin(), lines.end());
    }

    void removeDuplicates() {
        saveToStack();
        sortLines();
        lines.erase(unique(lines.begin(), lines.end()), lines.end());
    }

    void display() const {
        for (const string& line : lines) {
            std::cout << line << std::endl;
        }
    }

    void save() {
        std::ofstream file(activeFileName);
        if (file.is_open()) {
            for (const string& line : lines) {
                file << line << std::endl;
            }
            file.close();
        }
    }

    void saveAs(const string& fileName) {
        std::ofstream file(fileName);
        if (file.is_open()) {
            for (const string& line : lines) {
                file << line << std::endl;
            }
            file.close();
        }
    }
};

#endif //UNTITLED13_TEXTCOLLECTION_H
