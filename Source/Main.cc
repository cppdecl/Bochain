#include "CommandHandler/CommandHandler.hh"
#include "Utils/Clock.hh"
#include "Utils/Log.hh"

int main()
{
	Log::Init();

	Log::Info("{}Bochain {}- {}A simple blockchain implementation in C++ developed by cappu (github.com/cppdecl).",
			  Log::Color::YELLOW, Log::Color::WHITE, Log::Color::BLUE);
	Log::Info("Type 'help' or '?' to see a list of commands.");

	std::thread t2(
		[]()
		{
			for (;;)
			{
				GetCommandHandler().WaitForCommands();
			}
		});

	for (;;)
	{
	}

	return 0;
}