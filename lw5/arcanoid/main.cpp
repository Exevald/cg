#include "controller/GameController.h"
#include "model/GameModel.h"
#include "view/GameView.h"

#include <GLUT/glut.h>
#include <exception>
#include <iostream>

int main(int argc, char** argv)
{
	try
	{
		glutInit(&argc, argv);

		GameModel model;
		GameView view;
		GameController controller(model, view);
		controller.Run();
	}
	catch (const std::exception& exception)
	{
		std::cerr << exception.what() << std::endl;
		return 1;
	}

	return 0;
}
