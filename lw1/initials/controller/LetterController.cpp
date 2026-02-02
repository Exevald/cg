#include "LetterController.h"

void LetterController::Update(std::vector<std::unique_ptr<Letter>>& letters, float deltaTime)
{
	for (auto& letter : letters)
	{
		letter->Update(deltaTime);
	}
}
