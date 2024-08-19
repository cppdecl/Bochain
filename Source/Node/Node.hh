#ifndef BOCHAIN_NODE_NODE_HH
#define BOCHAIN_NODE_NODE_HH

#include "Core/Protocol.hh"
#include "Utils/Socket.hh"
#include "Utils/Types.hh"

class Node
{
   public:
	Node();
	~Node();

   public:
	bool Initialize();
	void Listen();

   private:
	void HandleClient(SOCKET clientSocket);

   public:
	u16 GetPort()
	{
		return m_Port;
	}
	void SetPort(u16 port)
	{
		m_Port = port;
	}

   private:
	u16				  m_Port	= 5000;
	SOCKET			  m_Socket	= {};
	std::atomic<bool> m_Running = false;
};

#endif	// BOCHAIN_NODE_NODE_HH