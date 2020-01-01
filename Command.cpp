#include "Command.h"

std::string Command::asSendBuffer() const
{
	std::string sendBuffer;

	sendBuffer.push_back(m_code);

	for (int i = 0; i < 4; ++i)
	{
		sendBuffer.push_back(char(m_parameters[i] >> 8));
		sendBuffer.push_back(char(m_parameters[i]));
	}

	return sendBuffer;
}
