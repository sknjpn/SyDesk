#include "common.h"

void Main()
{
	Serial serial;

	if (!serial.open(U"COM16", 115200)) return;

	{
		std::string sendBuffer;

		short px = 1000;
		short sx = 3000;
		short py = 1000;
		short sy = 3000;

		sendBuffer.push_back('D');
		sendBuffer.push_back(char(px >> 8));
		sendBuffer.push_back(char(px));
		sendBuffer.push_back(char(sx >> 8));
		sendBuffer.push_back(char(sx));
		sendBuffer.push_back(char(py >> 8));
		sendBuffer.push_back(char(py));
		sendBuffer.push_back(char(sy >> 8));
		sendBuffer.push_back(char(sy));

		serial.write(sendBuffer.c_str(),sendBuffer.size());
	}
}
