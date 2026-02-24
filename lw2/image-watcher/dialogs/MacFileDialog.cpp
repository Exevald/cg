#include "MacFileDialog.h"

std::string MacFileDialog::ShowOpenDialog()
{
#ifdef __APPLE__
	const char* script = R"(osascript -e 'POSIX path of (choose file with prompt "Select image" of type {"PNG","JPEG","BMP"})' 2>/dev/null)";

	std::array<char, 2048> buffer{};
	std::string result;

	FILE* pipe = popen(script, "r");
	if (pipe)
	{
		while (fgets(buffer.data(), buffer.size(), pipe) != nullptr)
		{
			result += buffer.data();
		}
		int status = pclose(pipe);
		if (status == 0 && !result.empty())
		{
			while (!result.empty() && (result.back() == '\n' || result.back() == '\r' || result.back() == ' '))
			{
				result.pop_back();
			}
			return result;
		}
	}
#endif
	std::cout << "Enter image file path: ";
	std::string path;
	std::getline(std::cin, path);
	return path;
}