#pragma once
#include <unordered_map>
#include <memory>
#include "TypedefRepo.h"

class ConsoleManager
{
public:
	typedef std::unordered_map<String, std::shared_ptr<String>> ConsoleTable;

	static ConsoleManager* getInstance();
	static void initialize();
	static void destroy();

	void drawConsole();
	void process();
	void switchConsole(String consoleName);

	void registerScreen(std::shared_ptr<String> screenRef);
	void switchToScreen(String screenName);
	void unregisterScreen(String screenName);

	void returnToPreviousConsole();
	void exitApplication();
	bool isRunning();

private:
	ConsoleManager();
	~ConsoleManager();
	ConsoleManager(ConsoleManager const&) {};
	static ConsoleManager* sharedInstance;

	ConsoleTable consoleTable;
	std::shared_ptr<String> previousConsole;
	std::shared_ptr<String> currentConsole;

	bool running = true;
};

