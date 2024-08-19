#ifndef BOCHAIN_UTILS_SOCKET_HH
#define BOCHAIN_UTILS_SOCKET_HH

#include "Types.hh"

#define _WINSOCKAPI_
#include <Windows.h>
#include <Ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

enum class BindStatus
{
	SUCCESS,
	WSA_STARTUP_FAILED,
	SOCKET_CREATION_FAILED,
	BIND_FAILED
};

namespace Socket
{
	inline BindStatus BindToPort(int port, SOCKET &outSocket)
	{
		WSADATA		wsaData;
		SOCKET		sockfd = INVALID_SOCKET;
		sockaddr_in addr;

		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		{
			std::cerr << "WSAStartup failed." << std::endl;
			return BindStatus::WSA_STARTUP_FAILED;
		}

		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd == INVALID_SOCKET)
		{
			std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
			WSACleanup();
			return BindStatus::SOCKET_CREATION_FAILED;
		}

		memset(&addr, 0, sizeof(addr));
		addr.sin_family		 = AF_INET;
		addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_port		 = htons(port);

		if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR)
		{
			std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
			closesocket(sockfd);
			WSACleanup();
			return BindStatus::BIND_FAILED;
		}

		outSocket = sockfd;
		return BindStatus::SUCCESS;
	}

	inline bool CanPortBeUsed(u16 port)
	{
		WSADATA		wsaData;
		SOCKET		sockfd = INVALID_SOCKET;
		sockaddr_in addr;

		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		{
			std::cerr << "WSAStartup failed." << std::endl;
			return false;
		}

		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd == INVALID_SOCKET)
		{
			std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
			WSACleanup();
			return false;
		}

		memset(&addr, 0, sizeof(addr));
		addr.sin_family		 = AF_INET;
		addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_port		 = htons(port);

		int bindResult = bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));

		closesocket(sockfd);
		WSACleanup();

		return bindResult == 0;
	}
}

#endif	// BOCHAIN_UTILS_SOCKET_HH