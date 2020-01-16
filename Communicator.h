#pragma once

#include "Command.h"

class Communicator
{
	Array<Command>	m_commands;
	Serial	m_serial;
	bool	m_isConnected = false;

	bool	m_canSend = true;
	bool	m_canSend_sub = true;

	void	updateRecv();
	void	updateSend();

	static std::unique_ptr<Communicator> g_instance;

public:
	static void MakeInstance() { g_instance = MakeUnique<Communicator>(); }

	static bool	Connect(String portname)
	{
		if (g_instance->m_serial.open(portname, 115200))
		{
			// シリアルリセット
			std::string text = "!!!!!!!!!!";
			g_instance->m_serial.write(text.c_str(), text.size());

			// PICリセット
			g_instance->m_commands.emplace_back(Command('C', 0, 0, 0, 0));
			g_instance->m_isConnected = true;

			return true;
		}

		return false;
	}

	static void	Update()
	{
		try
		{
			if (g_instance->m_isConnected)
			{
				g_instance->updateRecv();
				g_instance->updateSend();
			}
		}
		catch (const std::exception & e)
		{
			Logger << Unicode::Widen(e.what());

			g_instance->m_isConnected = false;
		}
	}

	// Add Command
	static void	AddCommand(const Command& command) { g_instance->m_commands.emplace_back(command); }
	static void	AddCommands(const Array<Command>& commands) { g_instance->m_commands.append(commands); }
	static void	AddCommandAtFront(const Command& command) { g_instance->m_commands.push_front(command); }

	// Get
	static const Serial& GetSerial() { return g_instance->m_serial; }
	static const Array<Command>& GetCommands() { return g_instance->m_commands; }

	static bool	IsConnected() { return g_instance->m_isConnected; }

	static void	ClearCommands() { g_instance->m_commands.clear(); }
};
