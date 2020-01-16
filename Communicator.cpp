#include "Communicator.h"

void Communicator::updateRecv()
{
	auto bytes = m_serial.readBytes();
	while (!bytes.isEmpty())
	{
		const char read = bytes.front();

		//Logger << Format(U"recv ", int(read));

		if (read == 'G')
		{
			m_canSend = true;
		}
		if (read == 'T')
		{
			m_canSend_sub = true;
		}

		bytes.pop_back();
	}
}

void Communicator::updateSend()
{
	if (!m_commands.isEmpty() && ((m_commands.front().m_code == 'H' && m_canSend_sub) || m_canSend))
	{
		//Logger << U"send";

		const auto sendBuffer = m_commands.front().asSendBuffer();

		m_serial.write(sendBuffer.c_str(), sendBuffer.size());

		m_commands.pop_front();
		m_canSend_sub = false;
		m_canSend = false;
	}
}

bool Communicator::connect(String portname)
{
	if (m_serial.open(portname, 115200))
	{
		std::string text = "!!!!!!!!!!";
		m_serial.write(text.c_str(), text.size());
		
		m_isConnected = true;

		return true;
	}

	return false;
}

void Communicator::update()
{
	try
	{
		if (m_isConnected)
		{
			updateRecv();
			updateSend();
		}
	}
	catch (const std::exception& e)
	{
		Logger << Unicode::Widen(e.what());

		m_isConnected = false;
	}
}
