#pragma once
#include "AConsole.h"
#include <memory>

class BaseScreen : public AConsole {
public:
    BaseScreen(String processName, std::shared_ptr<String> attachedProcess);
    void onEnabled() override;
    void process() override;
    void display() override;
    void onExecute() override {};

private:
    void printProcessInfo();
    std::shared_ptr<String> attachedProcess;
    bool refreshed = false;
    void handleCommand(String command);
};
