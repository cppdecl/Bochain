#ifndef BOCHAIN_COMMANDHANDLER_COMMANDS_HELP_HH
#define BOCHAIN_COMMANDHANDLER_COMMANDS_HELP_HH

#include "Utils/Log.hh"
#include "Utils/Types.hh"

namespace Commands
{
	namespace Help
	{
		inline void Execute(std::vector<std::string> &args)
		{
			Log::Info("Available commands:");
			Log::Info("help or ? - Display this message.");
			Log::Info("init <port> - Initialize this node with a port number.");
			Log::Info(
				"node [add <ip:port> | remove <ip:port> | list | refresh] - Add, remove, list, or refresh nodes.");
			Log::Info("exit - Exit the application.");
		}
	}
}

#endif	// BOCHAIN_COMMANDHANDLER_COMMANDS_HELP_HH