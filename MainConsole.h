#pragma once
#include "BaseScreen.h"
#include "TypedefRepo.h"

class MainConsole : public AConsole
{
public:
	MainConsole();
	~MainConsole() = default;

	void onEnabled();
	void display();
	void process();

	String formatNA(int num);

private:
	void displayHeader();
	void handleCommand(String command);
};