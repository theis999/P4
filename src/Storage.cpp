#pragma once

#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>
#include "FileEncrypt.h"
#include "Uuid.h"
#include "Storage.h"
#include "wx/wx.h"

static bool read_segment(std::istream& file, vector<string>& data)
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

void Storage::OpenStorage(string filename, std::vector<unsigned char> encryption_key, User currentUser)
{
	this->encryption_key = encryption_key;
	//std::filesystem::path cwd = std::filesystem::current_path() / filename;
	std::vector<std::string> decryptedLines;

	// This if statement is for testing and the transition to the encrypted data files. 
	// If the individual file doesn't exist, fallback to old data.txt - This functionionality can also be used if the channel access needs to be changed during testing. Ideally the system for managing channel access should manage this file.
	if (!std::filesystem::exists(filename))
	{
		std::string fallbackPath = "../data.txt";
		std::ifstream oldFile(fallbackPath);
		if (!oldFile.is_open())
		{
			wxMessageBox("No existing user data file or old data.txt found.", "Error", wxOK | wxICON_ERROR);
			return;
		}

		std::vector<std::string> oldDataLines;
		std::string line;
		while (std::getline(oldFile, line))
		{
			oldDataLines.push_back(line);
		}
		oldFile.close();

		std::ofstream newUserFile(filename, std::ios::binary);
		if (!newUserFile.is_open())
		{
			wxMessageBox("Failed to create user-specific encrypted file.", "File Error", wxOK | wxICON_ERROR);
			return;
		}
		for (const auto& l : oldDataLines)
		{
			EncryptMessageGCM(l, encryption_key, newUserFile);
		}
		newUserFile.close();

		decryptedLines = std::move(oldDataLines);
		//return;
	}
	//This is the actual method for decrypting the file
	else
	{
		if (!DecryptAllMessagesGCM(encryption_key, filename, decryptedLines))
		{
			HandleOpenSSLErrors();
			return;
		}
	}

	std::stringstream file;
	for (const auto& line : decryptedLines)
	{
		file << line << "\n";
	}

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
		string encFilePath = "chat_history/" + currentUser.name + "_" + ch.name + "messagedataEnc.bin";

		// If no file data file exists, create one, so the function doesn't fail
		if (!std::filesystem::exists(encFilePath))
		{
			std::ofstream createEmpty(encFilePath, std::ios::binary);
		}

		std::vector<std::string> decryptedLines;
		if (!DecryptAllMessagesGCM(this->encryption_key, encFilePath, decryptedLines))
		{
			HandleOpenSSLErrors();
			continue;
		}

		for (auto& line : decryptedLines)
		{
			std::stringstream ss(line);
			std::vector<std::string> parts;
			std::string seg;
			while (std::getline(ss, seg, ';'))
				parts.push_back(seg);

			if (parts.size() == 4)
			{
				ch.messages.emplace_back(std::stoi(parts[0]), std::stoi(parts[1]), parts[2], "0", string_to_hash(parts[3]));
			}
		}
	}
}

void Storage::AppendMessage(const Channel& c,const iMessage& msg,const User& currentUser)
{
	std::string encFilePath = "chat_history/" + currentUser.name + "_" + c.name + "messagedataEnc.bin";

	// Format the message line
	std::stringstream ss;
	ss << msg.timestamp << ';' << msg.member_id << ';' << msg.text << ';' << hash_to_string(msg.hash);

	std::ofstream out(encFilePath, std::ios::binary | std::ios::app);
	if (!out.is_open())
	{
		wxMessageBox("Unable to open encrypted history for appending.", "File Error", wxOK | wxICON_ERROR);
		return;
	}

	if (!EncryptMessageGCM(ss.str(), this->encryption_key, out))
	{
		wxMessageBox("Failed to encrypt and append message.", "Encryption Error", wxOK | wxICON_ERROR);
	}
}

Channel& Storage::GetCurrentChannel()
{
	return channels.at(currentChannelIndex);
}

Channel& Storage::GetChannel(GUID guid)
{
	for (Channel& ch : this->channels)
	{
		if (ch.global_id == guid)
		{
			return ch;
		}
	}
	throw "Channel doesn't exist";
}

void Storage::Save(string filename)
{
	if (channels.empty() || users.empty()) return; // prevent saving nothing
	std::ofstream file(filename, std::ios::binary);
	if (!file.is_open()) return;

	std::stringstream ss(ToFileString());
	std::string line;
	while (std::getline(ss, line))
	{
		EncryptMessageGCM(line, encryption_key, file);
	}
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
