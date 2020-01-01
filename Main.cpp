#include "common.h"
#include "Communicator.h"
#include "RouteGenerator.h"

void Main()
{
	Communicator communicator;
	RouteGenerator routeGenerator;

	if (!communicator.connect(U"COM16")) return;

	{
		short preState = 0b10000;
		while (System::Update())
		{
			short nowState = 0b10000;
			if (KeyLeft.pressed()) nowState += 0b0001;
			if (KeyUp.pressed()) nowState += 0b0010;
			if (KeyRight.pressed()) nowState += 0b0100;
			if (KeyDown.pressed()) nowState += 0b1000;

			if (preState != nowState)
			{
				communicator.addCommand('K', nowState, 0, 0, 0);

				preState = nowState;
			}

			communicator.update();

			if (KeyEnter.down()) break;
		}
	}

	/*communicator.addCommand('D', 1000, 2000, 500, 4000);
	communicator.addCommand('D', -1000, 2000, 500, 4000);
	communicator.addCommand('D', -1000, 2000, -500, 4000);
	communicator.addCommand('D', 1000, 2000, -500, 4000);
	*/

	routeGenerator.addPolygon(Shape2D::Star(50.0, Vec2(50, 50)));
	communicator.addCommands(routeGenerator.getCommands());

	while (System::Update())
	{
		communicator.update();
	}
}
