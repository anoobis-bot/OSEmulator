#pragma once
#include "AConsole.h"
#include <memory>
#include "TypedefRepo.h"

class MainScreen : public BaseScreen {
public:
    MainScreen(); 
    ~MainScreen(); 

    void onEnabled() override;
    void onExecute() override;
    void process() override;
    void display() override;

private:
    void printProcessInfo();
    std::shared_ptr<String> attachedProcess;
    bool refreshed = false;
    void displayHeader();
    void handleCommand(String command);
    bool isDisplayed = false;
};
