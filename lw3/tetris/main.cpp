#include "controller/Controller.h"
#include "model/Model.h"
#include "view/View.h"
#include <glut.h>

int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	Model model;
	View view(model);
	Controller controller(model, view);

	controller.Run();

	return 0;
}