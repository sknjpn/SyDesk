#include "common.h"
#include "Communicator.h"

void Main()
{
	Communicator communicator;

	if (!communicator.connect(U"COM16")) return;

	communicator.addCommand('D', 1000, 2000, 500, 4000);
	communicator.addCommand('D', -1000, 2000, 500, 4000);
	communicator.addCommand('D', -1000, 2000, -500, 4000);
	communicator.addCommand('D', 1000, 2000, -500, 4000);

	while (System::Update())
	{
		communicator.update();
	}
}
