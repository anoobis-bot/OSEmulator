#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

int printAscii(std::string fileName) {
    std::ifstream file;  // Open the file
    file.open(fileName);

    if (!file) {
        std::cerr << "Unable to open file art.txt" << std::endl;
        return 1;  // Exit if file cannot be opened
    }

    std::string line;
    while (std::getline(file, line)) {   // Read file line by line
        std::cout << line << std::endl;  // Print each line
    }

    file.close();  // Close the file
    return 0;
}


void printMsgNewLine(std::string msg) {
    std::cout << msg << std::endl;
}

void printMsg(std::string msg) {
    std::cout << msg;
}

void printHeader(std::string headerFile) {
    printAscii(headerFile);
    printMsgNewLine("Hello, Welcome to CSOPESY commandline!");
}


std::string toLowerCase(const std::string str) {
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}