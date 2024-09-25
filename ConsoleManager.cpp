#include "ConsoleManager.h"

ConsoleManager* ConsoleManager::sharedInstance = nullptr;
ConsoleManager* ConsoleManager::getInstance() 
{
	return sharedInstance;
}

void ConsoleManager::initialize()
{
	sharedInstance = new ConsoleManager();
}

void ConsoleManager::destroy()
{
	delete sharedInstance;
}

void ConsoleManager::drawConsole()
{
	return;
}