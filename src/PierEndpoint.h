#pragma once
#include <winsock.h>
#include "iMessage.h"

/* TEMPORARY CLASS ONLY - REMOVE IF SHASH EXISTS */

/* TEMPORARY CLASS END */

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
	
	PierErr_t SendMSG(sockaddr dest_ip, iMessage msg);
	PierErr_t SendMSGRequest(sockaddr dest_ip, std::array<std::byte, 32> hash);
	PierErr_t SendMSGMulti(sockaddr dest_ip, std::vector<iMessage>);
	PierErr_t SendSyncProbe(sockaddr dest_ip, std::array<std::byte, 32> hash);
	PierErr_t SendSyncStatus(sockaddr dest_ip, uint8_t);
	PierErr_t SendSHASHRequest(sockaddr dest_ip, uint32_t amount);
	PierErr_t SendSHASHMulti(sockaddr dest_ip, std::vector<std::array<std::byte, 32>> hashes);

};