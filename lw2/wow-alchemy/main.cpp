#include "controller/AlchemyController.h"
#include "model/AlchemyModel.h"
#include "view/SFMLView.h"

#include <iostream>

int main()
{
	try
	{
		AlchemyModel model;
		SFMLView view(model);
		AlchemyController controller(model, view);

		while (view.IsOpen())
		{
			controller.ProcessInput();
			view.Render(model);
		}
		return 0;
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}
}