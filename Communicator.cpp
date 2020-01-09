#include "Communicator.h"

void Communicator::update()
{
	if (!m_serial.isOpened()) return;

	// 受信処理
	{
		auto bytes = m_serial.readBytes();
		while (!bytes.isEmpty())
		{
			const char read = bytes.front();

			Logger << Format(U"recv ", int(read));

			//if (read == 'G')
			{
				m_canSend = true;
			}

			bytes.pop_back();
		}
	}

	// 送信処理
	if (m_canSend && !m_commands.isEmpty())
	{
		Logger << U"send";

		const auto sendBuffer = m_commands.front().asSendBuffer();

		m_serial.write(sendBuffer.c_str(), sendBuffer.size());

		m_commands.pop_front();
		m_canSend = false;
	}
}
