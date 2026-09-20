#include "Window.h"

#include <cstdlib>
#include <exception>
#include <iostream>

int main()
{
	try
	{
		Window window(1024, 768, "Paraboloid");
		window.Run();
	}
	catch (const std::exception& exception)
	{
		std::cerr << exception.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
