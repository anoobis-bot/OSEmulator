#pragma once
#include <unordered_map>
#include <memory>
#include "AConsole.h"
#include "BaseScreen.h"
#include "TypedefRepo.h"

const String MAIN_CONSOLE = "MAIN_CONSOLE";

class ConsoleManager
{
public:
    typedef std::unordered_map<String, std::shared_ptr<AConsole>> ConsoleTable;

    static ConsoleManager* getInstance();
    static void initialize();
    static void destroy();

    void drawConsole();
    void process();
    void switchConsole(String consoleName);

    void registerScreen(std::shared_ptr<BaseScreen> screenRef);
    void switchToScreen(String screenName);
    void unregisterScreen(String screenName);
	void createProcessScreen(String processName);

    void returnToPreviousConsole();
    void exitApplication();
    bool isRunning();

    bool isScreenRegistered(const String& screenName);

private:
    ConsoleManager();
    ~ConsoleManager();
    ConsoleManager(ConsoleManager const&) = delete;
    static ConsoleManager* sharedInstance;

    ConsoleTable consoleTable;
    std::shared_ptr<AConsole> previousConsole;
    std::shared_ptr<AConsole> currentConsole;

    bool running = true;
};
