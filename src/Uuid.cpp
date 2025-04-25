#include "Uuid.h"
#include <rpc.h>

GUID GuidFromString(const string& s)
{
	GUID g;
	auto status = UuidFromStringA((RPC_CSTR)s.c_str(), &g);
	if (status != RPC_S_OK)
		throw "UUID didn't parse for string: " + s;
	return g;
}

void GuidToString(GUID g, string& out)
{
	RPC_CSTR uuidStr;
	auto status = UuidToStringA(&g, &uuidStr);
	if (status != RPC_S_OK) throw "Error creating string from GUID";
	
	out = (char*)uuidStr;

	RpcStringFreeA(&uuidStr);
}

string GuidToString(GUID g)
{
	string s;
	GuidToString(g, s);
	return s;
}

GUID GuidCreate()
{
	UUID g;
	auto result = UuidCreate(&g);
	if (result != RPC_S_OK) throw "Could not create GUID";
	return g;
}
