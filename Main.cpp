﻿#include "common.h"
#include "MainViewer.h"
#include "Communicator.h"
#include "RouteGenerator.h"

void Main()
{
	RouteGenerator::MakeInstance();
	Communicator::MakeInstance();

	Scene::SetScaleMode(ScaleMode::ResizeFill);
	Window::SetStyle(WindowStyle::Sizable);
	Window::Resize(1280, 720);

	EasyViewer::GetRootViewer()->addChildViewer<MainViewer>();
	EasyViewer::Run();
}
