#include "common.h"
#include "Communicator.h"
#include "RouteGenerator.h"

void Main()
{
	Communicator communicator;
	RouteGenerator routeGenerator;

	if (!communicator.connect(U"COM16")) return;

	
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
