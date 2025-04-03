#pragma once
#include "STDimport.h"
#include "PierProto.h"
#include "iMessage.h"

enum PierErr_t {
	OK = 0,
	ERR,
	HASH_MISMATCH,
	FAILED_SEND,
	SYNC_PROBE_DENIED,
	SHASH_REQUEST_DENIED,
	MSG_REQUEST_DENIED,
};

	
class PierEndpoint {
	
	std::array<std::byte, 1024> buffer_arr;

	boost::asio::io_context io;
	uint16_t listening_port;
	boost::asio::ip::tcp::socket socket;
	boost::asio::ip::address ip_address;

	// Receive buffer with size of 1024 bytes
	boost::asio::mutable_buffer recv_buffer;

	PierErr_t SendMSG(sockaddr dest_ip, iMessage msg);
	PierErr_t SendMSGRequest(sockaddr dest_ip, iMessage::shash hash);
	PierErr_t SendMSGMulti(sockaddr dest_ip, std::vector<iMessage>);
	PierErr_t SendSyncProbe(sockaddr dest_ip, iMessage::shash hash);
	PierErr_t SendSyncStatus(sockaddr dest_ip, uint8_t);
	PierErr_t SendSHASHRequest(sockaddr dest_ip, uint32_t amount);
	PierErr_t SendSHASHMulti(sockaddr dest_ip, std::vector<iMessage::shash> hashes);

	PierErr_t Send(std::vector<std::byte> bytes);

	
};
