#pragma once

# define SIV3D_CONCURRENT
# include <Siv3D.hpp> // OpenSiv3D v0.4.2

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
		DroppedFilePath m_droppedFilePath;
		Image	m_image;
		Texture	m_texture;
		MultiPolygon	m_multiPolygon;
		double	m_ppi = 72;

		Array<Array<Vec2>> getOutlines(const Grid<bool>& map);
		Array<Vec2> getOutline(const Grid<bool>& map);
		Array<Array<Vec2>> getOutlines(const Image& image, std::function<bool(Color)> judge);
		Array<Vec2> getOutline(const Image& image, std::function<bool(Color)> judge);

	public:
		ImageDialog(const DroppedFilePath& droppedFilePath, const Image& image);

		void	onLoad();

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
		Texture	m_textureD = Texture(Icon(0xf13a, 80));
		Texture	m_textureU = Texture(Icon(0xf139, 80));
		Texture	m_textureR = Texture(Icon(0xf138, 80));
		Texture	m_textureL = Texture(Icon(0xf137, 80));
		Texture	m_explain = Texture(U"assets/img.png");

		enum class StateRL
		{
			None,
			MoveToR,
			MoveToL,
		};
		StateRL m_stateNowRL = StateRL::None;
		StateRL m_statePreRL = StateRL::None;

		enum class StateUD
		{
			None,
			MoveToU,
			MoveToD,
		};
		StateUD m_stateNowUD = StateUD::None;
		StateUD m_statePreUD = StateUD::None;

	public:
		void	init() override;
		void	update() override;
	};

	class SerialSelector : public EasyViewer
	{
		class SerialList : public EasyViewer
		{
			class Item : public EasyViewer
			{
			public:
				enum class State
				{
					None,
					ReConnected,
					DisConnected,
					LastConnected,
				} m_state;

				SerialPortInfo	m_serialPort;

			public:
				Item(const SerialPortInfo& serialPort, State state = State::None)
					: m_serialPort(serialPort)
					, m_state(state)
				{}

				void	init() override;
				void	update() override;
			};

		public:
			void	init() override;
			void	update() override;
		};

	public:
		void	init() override;
		void	update() override;

		void	connect(const String& port);
	};

	class SerialViewer : public EasyViewer
	{
	public:
		void	init() override;
		void	update() override;
	};

	class Workspace : public EasyViewer
	{
		class Shape
		{
		public:
			bool m_isGrabbed = false;
			Polygon	m_polygon;
			Polygon	m_cuttingPolygon;
			Polygon m_circlingPolygon;

		public:
			Shape(const Polygon& polygon);

			void update(const RouteGenerator& routeGenerator);
		};

		ConcurrentTask<void> m_updater;
		Array<Shape> m_shapes;
		bool	m_needToUpdate = true;
		bool	m_inUpdate = false;

		void	updateShapes();

	public:
		void	addPolygon(const Polygon& polygon);

		void	onMarginChanged();
		void	init() override;
		void	update() override;
		
	};

	Array<DroppedFilePath>	m_reservedItems;

public:
	Communicator	m_communicator;
	RouteGenerator	m_routeGenerator;

	void	init() override;
	void	update() override;

	void	setPolygon(const Polygon& polygon);
};
