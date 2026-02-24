#pragma once
#include <string>

class IFileDialog
{
public:
	virtual std::string ShowOpenDialog() = 0;

	virtual ~IFileDialog() = default;
};