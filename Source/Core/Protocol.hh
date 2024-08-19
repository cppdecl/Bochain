#ifndef BOCHAIN_CORE_PROTOCOL_HH
#define BOCHAIN_CORE_PROTOCOL_HH

#include "Utils/Types.hh"

enum class BochainPacketType
{
	NONE	  = 0,
	DISCOVERY = 1,
	BROADCAST = 2,
};

#pragma pack(push, 1)

struct BochainProtocolHeader
{
	BochainPacketType PacketType = BochainPacketType::NONE;
};

struct BochainProtocolBody
{
	u32 DataLength = 0;
	u8	Data	   = 0;
};

struct BochainPacket
{
	BochainProtocolHeader Header;
	BochainProtocolBody	  Body;
};

#pragma pack(pop)

#endif	// BOCHAIN_CORE_PROTOCOL_HH