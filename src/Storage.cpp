#pragma once

#include <fstream>
#include <sstream>
#include <filesystem>

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
}

string Storage::ToFileString()
{
	return string();
}


