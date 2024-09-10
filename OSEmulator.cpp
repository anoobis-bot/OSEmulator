// OSEmulator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "utils.h"

int main()
{
    printHeader("3D_CSOPESY.txt");
    
    std::string userInput = "";
    std::string formattedInput = "";
    while (formattedInput != "exit") {
        printMsg("Command: ");
        std::cin >> userInput;
        formattedInput = toLowerCase(userInput);

        if (formattedInput == "clear") {
            std::cout << "\033[2J\033[1;1H"; // ANSII escape code for clearning the screen
            printHeader("3D_CSOPESY.txt");
        }

        else if (formattedInput == "initialize") {
            printMsgNewLine("Initialize command recognized. Doing something");
        }

        else if (formattedInput == "screen") {
            printMsgNewLine("Screen command recognized. Doing something");
        }

        else if (formattedInput == "scheduler-test") {
            printMsgNewLine("Scheduler-test command recognized. Doing something");
        }

        else if (formattedInput == "scheduler-stop") {
            printMsgNewLine("Scheduler-stop command recognized. Doing something");
        }

        else if (formattedInput == "report-util") {
            printMsgNewLine("Report-util command recognized. Doing something");
        }


        else if (formattedInput != "exit") {
            printMsgNewLine("Command not found. These are the available commands: ");
            printMsgNewLine("initialize, screen, scheduler-test, scheduler-stop, report-util, clear, and exit.");
        }
    }

    printMsg("Program Terminated");
}

//  initialize, screen, scheduler-test, scheduler-stop, report-util, clear, and exit.