#pragma once
#include <unordered_map>
#include <memory>
#include "AConsole.h"
#include "BaseScreen.h"
#include "TypedefRepo.h"

const String MAIN_CONSOLE = "MAIN_CONSOLE";
const String MARQUEE_CONSOLE = "MARQUEE_CONSOLE";
const String SCHEDULING_CONSOLE = "SCHEDULING_CONSOLE";
const String MEMORY_CONSOLE = "MEMORY_CONSOLE";

class ConsoleManager
{
public:
    typedef std::unordered_map<String, std::shared_ptr<AConsole>> ConsoleTable;

    static ConsoleManager* getInstance();
    static void initialize();
    static void destroy();

    void drawConsole() const;
    void process() const;
    void switchConsole(String consoleName);

    void registerScreen(std::shared_ptr<BaseScreen> screenRef);
    void switchToScreen(String screenName);
    void unregisterScreen(String screenName);
	void createProcessScreen(String processName);

    void returnToPreviousConsole();
    void exitApplication();
    bool isRunning() const;

    bool isScreenRegistered(const String& screenName);

private:
    ConsoleManager();
    ~ConsoleManager() = default;
    ConsoleManager(ConsoleManager const&) {};
    ConsoleManager& operator=(ConsoleManager const&) {};
    static ConsoleManager* sharedInstance;

    ConsoleTable consoleTable;
    std::shared_ptr<AConsole> previousConsole;
    std::shared_ptr<AConsole> currentConsole;

    bool running;
};
