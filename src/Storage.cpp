#pragma once

#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>
#include <fstream>

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

void Storage::OpenStorage(string filename)
{
	//std::filesystem::path cwd = std::filesystem::current_path() / filename;
	std::ifstream file;
	file.open(filename);
	auto is_open = file.is_open();
	vector<string> data;
	while (read_segment(file, data))
	{
		channels.push_back({data[0], data[1] == "true"});
	}

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

string Storage::ToFileString()
{
	return string();
}


