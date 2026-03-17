#include "Window.h"

#include <iostream>

int main()
{
	try
	{
		Window window(1024, 768, "Dodecahedron");
		window.Run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}