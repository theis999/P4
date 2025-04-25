#pragma once
#include <guiddef.h>
#include <string>

using std::string;
typedef GUID UUID;

void GuidFromString(const string& s);
void GuidToString(GUID g, string& out);
string GuidToString(GUID g);
GUID GuidCreate();
