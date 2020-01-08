#pragma once

#include "EasyViewer.h"
#include "Communicator.h"
#include "RouteGenerator.h"

class MainViewer : public EasyViewer
{
	class Adviser : public EasyViewer
	{
	public:
		void	init() override;
		void	update() override;
	};

	class CutSetting : public EasyViewer
	{
	public:
		void	init() override;
		void	update() override;
	};

	class ImageDialog : public EasyViewer
	{
	public:
		void	init() override;
		void	update() override;
	};

	class MachineViewer : public EasyViewer
	{
	public:
		void	init() override;
		void	update() override;
	};

	class OriginAdjustment : public EasyViewer
	{
		enum class StateRL
		{
			None,
			MoveToRight,
			MoveToLeft,
		};
		StateRL m_stateNowRL = StateRL::None;
		StateRL m_statePreRL = StateRL::None;

		enum class StateTD
		{
			None,
			MoveToTop,
			MoveToDown,
		};
		StateTD m_stateNowTD = StateTD::None;
		StateTD m_statePreTD = StateTD::None;

	public:
		void	init() override;
		void	update() override;
	};
	
	class SerialSelector : public EasyViewer
	{
	public:
		void	init() override;
		void	update() override;
	};

	class SerialViewer : public EasyViewer
	{
	public:
		void	init() override;
		void	update() override;
	};

	class Workspace : public EasyViewer
	{
	public:
		void	init() override;
		void	update() override;
	};

public:
	Communicator	m_communicator;
	RouteGenerator	m_routeGenerator;

	void	init() override;
	void	update() override;
};
