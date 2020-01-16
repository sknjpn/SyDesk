#include "Communicator.h"

std::unique_ptr<Communicator> Communicator::g_instance;

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
