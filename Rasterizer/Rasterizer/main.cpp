#include "includes/Rendering/Window.h"

using namespace Rendering;

int main(int argc, char* argv[])
{
	{
		Window window(1024, 768);

		window.Setup();
		window.Run();
	}
//	system("pause");
	return 0;
}
