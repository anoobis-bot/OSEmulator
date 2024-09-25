// OSEmulator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "utils.h"

int main()
{
    printHeader("3D_CSOPESY.txt");
    
    std::string userInput = "";
    std::string formattedInput = "";
    printMsgNewLine("Type 'exit' to quit, 'clear' to clear the screen.");
    while (formattedInput != "exit") {
        printMsg("Command: ");
        std::cin >> userInput;
        formattedInput = toLowerCase(userInput);

        if (formattedInput == "clear") {
            printMsgNewLine("clear command recognized. Doing something.");
            std::cout << "\033[2J\033[1;1H"; // ANSII escape code for clearning the screen
            printHeader("3D_CSOPESY.txt");
        }

        else if (formattedInput == "initialize") {
            printMsgNewLine("initialize command recognized. Doing something");
        }

        else if (formattedInput == "screen") {
            printMsgNewLine("screen command recognized. Doing something");
        }

        else if (formattedInput == "scheduler-test") {
            printMsgNewLine("scheduler-test command recognized. Doing something");
        }

        else if (formattedInput == "scheduler-stop") {
            printMsgNewLine("scheduler-stop command recognized. Doing something");
        }

        else if (formattedInput == "report-util") {
            printMsgNewLine("report-util command recognized. Doing something");
        }


        else if (formattedInput != "exit") {
            printMsgNewLine("Command not found. These are the available commands: ");
            printMsgNewLine("initialize, screen, scheduler-test, scheduler-stop, report-util, clear, and exit.");
        }
    }

    printMsg("Program Terminated");
}

//  initialize, screen, scheduler-test, scheduler-stop, report-util, clear, and exit.