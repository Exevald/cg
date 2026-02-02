#pragma once

#include "Letter.h"

class LetterP : public Letter
{
public:
	LetterP(float x, float y, sf::Color color, float delay);
};

class LetterK : public Letter
{
public:
	LetterK(float x, float y, sf::Color color, float delay);
};

class LetterO : public Letter
{
public:
	LetterO(float x, float y, sf::Color color, float delay);
};
