#pragma once
#include "TypedefRepo.h"

class AConsole {
public:
    AConsole(String name) : name(name) {}
    virtual ~AConsole() = default;

    String getName() { return name; }
    virtual void onEnabled() = 0;
    virtual void onExecute() = 0;
    virtual void display() = 0;
    virtual void process() = 0;

protected:
    String name;
    friend class ConsoleManager;
};
