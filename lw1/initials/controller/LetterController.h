#pragma once

#include "../model/Letter.h"

#include <SFML/System/Clock.hpp>
#include <memory>
#include <vector>

class LetterController
{
public:
	static void Update(std::vector<std::unique_ptr<Letter>>& letters, float deltaTime);
};