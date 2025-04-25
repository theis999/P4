#pragma once

#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>
#include <fstream>

#include "Uuid.h"
#include "Storage.h"

static bool read_segment(std::ifstream& file, vector<string>& data)
{
	string line;
	string segment;
	std::stringstream data_line;

	if (std::getline(file, line))
	{
		data_line.str(line);

		data.clear();
		data.reserve(16);

		while (std::getline(data_line, segment, ';'))
		{
			data.push_back(segment);
		}

		return true;
	}

	return false;
}

static string hash_to_string(iMessage::shash bytes)
{
	char s[65];
	for (int i = 0; i < 32; i++)
	{
		auto byte = (int)(bytes[i]);
		auto a = (byte & 0xF0) >> 4;
		auto b = byte & 0x0F;
		s[i * 2] = (char)(a < 10 ? '0' + a : 'a' + a - 10);
		s[i * 2 + 1] = (char)(b < 10 ? '0' + b : 'a' + b - 10);
	}
	s[64] = 0;
	return s;
}

static iMessage::shash string_to_hash(string hexidecimal)
{
	iMessage::shash out{};

	auto in = hexidecimal.c_str();
	for (int i = 0; i < 32; i++)
	{
		auto a = in[i * 2] < 'a' ? in[i * 2] - '0' : in[i * 2] - 'a' + 10;
		auto b = in[i * 2 + 1] < 'a' ? in[i * 2 + 1] - '0' : in[i * 2 + 1] - 'a' + 10;
		auto c = a << 4 | b;
		out[i] = (std::byte)(c);
	}
	return out;
}

bool Storage::UserNameExists(const std::string& username, const std::string& filepath)
{
	std::ifstream in(filepath);
	if (!in.is_open()) return false;

	std::string line;
	std::string lowerInput = makeLowercase(username);

	while (std::getline(in, line))
	{
		std::istringstream ss(line);
		std::string idStr, name, guidStr;

		if (std::getline(ss, idStr, ';') && std::getline(ss, name, ';') && std::getline(ss, guidStr))
		{
			if (makeLowercase(name) == lowerInput)
			{
				return true;
			}
		}
	}
	return false;
}

bool Storage::CreateUser(const User& user, const std::string& filepath)
{
	std::fstream file(filepath, std::fstream::out | std::fstream::app | std::fstream::ate);
	if (!file.is_open()) return false;
	file << user.ToFileString() << std::endl;
	file.close();
	return true;
}

void Storage::OpenStorage(string filename)
{
	//std::filesystem::path cwd = std::filesystem::current_path() / filename;
	std::ifstream file;
	file.open(filename);
	auto is_open = file.is_open();

	channels.clear(); users.clear();

	vector<Member> memberqueue;
	vector<string> data;
	while (read_segment(file, data))
	{
		auto datatype = map_name_to_datatype.find(data[0])->second;
		switch (datatype)
		{
			case d_user:
				users.push_back(User(GuidFromString(data[3]), stoi(data[1]), data[2]));
				break;

			case d_membership:
				memberqueue.push_back(Member(stoi(data[3]), stoi(data[2]), data[4], stoi(data[1])));
				break;

			case d_channel:
				channels.push_back(Channel(stoi(data[1]), data[2], GuidFromString(data[3])));
				break;

			default:
				break;
		}
	}

	std::sort(users.begin(), users.end(), [](User& a, User& b)
{
	return a.user_id < b.user_id;
 });
	std::sort(channels.begin(), channels.end(), [](Channel& a, Channel& b)
{
	return a.channel_id < b.channel_id;
});
	for (auto& member : memberqueue)
		channels.at(member.channel_id).members.insert({member.member_id, member});

	for (auto& ch : channels)
	{
		string filePath = "chat_history/" + ch.name + "messagedata.txt";
		std::ifstream file(filePath);

		if (!file) return;

		while (read_segment(file, data) && data.size() > 1)
		{
			auto a = std::stoi(data[0]);
			ch.messages.push_back(iMessage(std::stoi(data[0]), std::stoi(data[1]), data[2], string_to_hash(data[3])));
		}
	}
}

void Storage::AppendMessage(Channel c, iMessage msg)
{
	string filePath = "chat_history/" + c.name + "messagedata.txt";

	// 1) Read existing file content
	std::fstream inFile(filePath, std::fstream::out | std::fstream::app | std::fstream::ate);
	if (!inFile.is_open() && !inFile.good())
	{
		// If no file yet, we can just write everything
		std::ofstream createFile(filePath, std::ios::out | std::ios::trunc);
		if (!createFile)
		{
			//wxMessageBox("Failed to create chat history file", "Error");
			return;
		}
		createFile << msg.timestamp << ";" << std::to_string(msg.member_id) << ";" << msg.text << ";" << hash_to_string(msg.hash) << std::endl;

		createFile.close();
		return;
	}

	inFile << msg.timestamp << ";" << std::to_string(msg.member_id) << ";" << msg.text << ";" << hash_to_string(msg.hash) << std::endl;
	inFile.close();
	return;
}

Channel& Storage::GetCurrentChannel()
{
	return channels.at(currentChannelIndex);
}

void Storage::Save(string filename)
{
	if (channels.empty() || users.empty()) return; // prevent saving nothing
	std::ofstream file(filename);
	if (!file.is_open()) return;
	file << ToFileString();
	file.close();
}

string Storage::ToFileString()
{
	using std::endl;
	std::stringstream out;
	for (User u : users)
		out << datatype_names[d_user] << ";" << u.ToFileString() << endl;
	for (Channel c : channels)
	{
		out << datatype_names[d_channel] << ";" << c.ToFileString() << endl;
		for(auto m = c.members.begin();  m != c.members.end(); m++)
			out << datatype_names[d_membership] << ";" << m->second.ToFileString() << endl;
	}
	return out.str();
}


