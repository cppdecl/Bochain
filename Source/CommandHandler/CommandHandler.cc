#include "CommandHandler.hh"

#include "Commands/Help.hh"
#include "Commands/Node.hh"
#include "Commands/Quit.hh"
#include "Core/Protocol.hh"
#include "Utils/Clock.hh"
#include "Utils/Log.hh"
#include "Utils/Socket.hh"

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
	else if (command == "node")
	{
		Commands::Node::Execute(args);
	}
	else if (command == "exit")
	{
		Commands::Quit::Execute(args);
		return;
	}
	else if (command == "send")
	{
		std::string ip		= args[0];
		std::string port	= args[1];
		std::string message = args[2];

		BochainPacketType packetType = BochainPacketType::BROADCAST;

		// Create the socket
		SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (clientSocket == INVALID_SOCKET)
		{
			std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
			return;
		}

		// Specify the server address and port
		sockaddr_in serverAddr;
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port	  = htons(atoi(port.c_str()));
		inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);

		// Connect to the server
		if (connect(clientSocket, reinterpret_cast<sockaddr *>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR)
		{
			std::cerr << "Connect failed: " << WSAGetLastError() << std::endl;
			closesocket(clientSocket);
			return;
		}

		if (message.empty())
			message = "Hello from Bochain!";

		usize packetSize = sizeof(BochainPacket) + message.size();

		// Create the packet with empty message
		BochainPacket *packet	  = static_cast<BochainPacket *>(malloc(packetSize));
		packet->Header.PacketType = packetType;

		packet->Body.DataLength = message.size();
		memcpy(&packet->Body.Data, message.c_str(), message.size());

		u8 *finalData = static_cast<u8 *>(malloc(sizeof(u32) + packetSize));
		memcpy(finalData, &packetSize, sizeof(u32));
		memcpy(finalData + sizeof(u32), packet, packetSize);
		u32 finalDataSize = sizeof(u32) + packetSize;

		Log::Info("Final Data Size: {}", finalDataSize);
		Log::Info("Packet Size: {}", packetSize);

		// Send the packet
		int bytesSent = send(clientSocket, (char *)finalData, finalDataSize, 0);
		if (bytesSent == SOCKET_ERROR)
		{
			std::cerr << "Failed to send packet with type: " << static_cast<int>(packetType)
					  << ". Error: " << WSAGetLastError() << std::endl;
		}
		else
		{
			std::cout << "Successfully sent packet with type: " << static_cast<int>(packetType) << "." << std::endl;
		}

		// Clean up
		closesocket(clientSocket);
		WSACleanup();
	}
	else
	{
		Log::Info("{}Unknown command: '{}'. Type 'help' or '?' to see a list of commands.", Log::Color::RED, command);
	}
}
