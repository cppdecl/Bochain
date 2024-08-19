#include "Node.hh"

#include "Core/Protocol.hh"
#include "Utils/Log.hh"
#include "Utils/Socket.hh"
#include "Utils/Types.hh"

Node::Node()
{
}

Node::~Node()
{
}

bool Node::Initialize()
{
	Log::Info("Initializing Bochain Node");

	u16 portFound = 0;
	Log::Info("Checking for available port (starting at 50000).. ");
	for (uint16_t port = 50000; port < 60000; port++)
	{
		if (!Socket::CanPortBeUsed(port))
			continue;

		Log::Info("Port {} is available.", port);
		portFound = port;
		break;
	}

	if (!portFound)
	{
		Log::Info("Traversed 50000 to 60000, no usable port found.");
		return false;
	}

	Log::Info("Attempting to bind to port {}.", portFound);

	BindStatus bStats = Socket::BindToPort(portFound, m_Socket);
	if (bStats != BindStatus::SUCCESS)
	{
		Log::Info("Failed to bind to port {}.", portFound);
		return false;
	}

	Log::Info("Successfully bound to port {}.", portFound);

	m_Running.store(true);

	return true;
}

void Node::Listen()
{
	Log::Info("Bochain is listening... Waiting for incoming connections.");

	while (true)
	{
		if (listen(m_Socket, 1) == SOCKET_ERROR)
		{
			Log::Info("Failed to listen on port {}.", m_Port);
			continue;
		}

		SOCKET clientSocket = accept(m_Socket, NULL, NULL);
		if (clientSocket == INVALID_SOCKET)
		{
			Log::Info("Failed to accept client connection.");
			continue;
		}

		HandleClient(clientSocket);

		closesocket(clientSocket);
	}
}

void Node::HandleClient(SOCKET clientSocket)
{
	u32 expectedLen = 0;

	usize pos  = 0;
	u8*	  data = nullptr;

	while (true)
	{
		usize bufferLen = 1024;
		u8*	  buffer	= static_cast<u8*>(malloc(bufferLen));
		if (!buffer)
		{
			return;
		}

		usize bytesReceived = recv(clientSocket, (char*)buffer, bufferLen, 0);
		if (bytesReceived > 0)
		{
			if (!expectedLen)
			{
				memcpy(&expectedLen, buffer, sizeof(u32));

				if (bytesReceived > sizeof(u32))
				{
					memmove(buffer, buffer + sizeof(u32), bufferLen - sizeof(u32));
					bytesReceived -= sizeof(u32);
				}
			}

			if (!data)
			{
				data = static_cast<u8*>(malloc(expectedLen));
				if (!data)
				{
					return;
				}
			}

			memcpy(data + pos, &buffer, bytesReceived);
			pos += bytesReceived;

			if (pos >= expectedLen)
			{
				BochainPacket* packet = static_cast<BochainPacket*>(malloc(sizeof(expectedLen)));
				memcpy(&packet, data, expectedLen);

				std::string str = std::string(reinterpret_cast<char*>(&packet->Body.Data), packet->Body.DataLength);

				Log::Info("Packet Type: {}", static_cast<int>(packet->Header.PacketType));
				Log::Info("Data Length: {}", packet->Body.DataLength);
				Log::Info("Data: {}", str);

				free(data);
				data		= nullptr;
				pos			= 0;
				expectedLen = 0;
			}
		}

		free(buffer);

		if (bytesReceived == 0)
		{
			break;
		}
		else if (bytesReceived < 0)
		{
			break;
		}
	}
}
