#pragma once
#include "BaseScreen.h"
#include "TypedefRepo.h"

class MainConsole : public AConsole
{
public:
	MainConsole();
	~MainConsole() = default;

	void onEnabled() override;
	void display() override;
	void process() override;

private:
	void displayHeader();
	bool handleCommand(String command);

	String ACTION;
};