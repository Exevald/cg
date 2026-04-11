#include "controller/GameController.h"
#include "model/GameModel.h"
#include "view/GameView.h"

#include <GLUT/glut.h>
#include <exception>
#include <iostream>
#include <memory>

int main(int argc, char** argv)
{
	try
	{
		glutInit(&argc, argv);

		GameModel model;
		auto view = std::make_unique<GameView>();
		GameController controller(model, *view);
		controller.Run();
		view.reset();
	}
	catch (const std::exception& exception)
	{
		std::cerr << exception.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
