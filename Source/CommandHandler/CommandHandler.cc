#include "CommandHandler.hh"

#include "Commands/Help.hh"
#include "Commands/Init.hh"
#include "Commands/Node.hh"
#include "Commands/Quit.hh"
#include "Utils/Clock.hh"
#include "Utils/Log.hh"

CommandHandler::CommandHandler()
{
}

CommandHandler &CommandHandler::GetInstance()
{
	static CommandHandler instance;
	return instance;
}

CommandHandler &GetCommandHandler()
{
	return CommandHandler::GetInstance();
}

void CommandHandler::WaitForCommands()
{
	std::string line;

	Log::Print("{}> ", Log::ArrowColor);

	if (!std::getline(std::cin, line))
	{
		return;
	}

	if (line.empty())
	{
		return;
	}

	std::istringstream		 linestream(line);
	std::string				 command;
	std::vector<std::string> args;

	if (std::getline(linestream, command, ' '))
	{
		std::string param;
		while (std::getline(linestream, param, ' '))
		{
			args.push_back(param);
		}
	}

	Log::OverwritePreviousLine();

	if (command == "help" || command == "?")
	{
		Commands::Help::Execute(args);
	}
	else if (command == "init")
	{
		Commands::Init::Execute(args);
	}
	else if (command == "node")
	{
		Commands::Node::Execute(args);
	}
	else if (command == "exit")
	{
		Commands::Quit::Execute(args);
		return;
	}
	else
	{
		Log::Info("{}Unknown command: '{}'. Type 'help' or '?' to see a list of commands.", Log::Color::RED, command);
	}
}
