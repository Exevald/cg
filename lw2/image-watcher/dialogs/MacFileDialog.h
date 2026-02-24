#pragma once

#include "IFileDialog.h"

#include <array>
#include <cstdio>
#include <iostream>

#ifdef __APPLE__
#include <unistd.h>
#endif

class MacFileDialog : public IFileDialog
{
public:
	std::string ShowOpenDialog() override;
};