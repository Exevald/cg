#include "src/App.h"

#include <iostream>

int main()
{
	App app;

	if (!app.Initialize())
	{
		std::cerr << "Failed to initialize application\n";
		return -1;
	}

	app.Run();
	app.Shutdown();

	return 0;
}