#pragma once

#include "../model/Model.h"
#include "../view/View.h"
#include "BaseWindow.h"

#include <chrono>

class Controller : public BaseWindow
{
public:
	Controller(Model& m, View& v);

protected:
	void OnDraw(int w, int h) override;
	void HandleInput() const;

private:
	Model& m_model;
	View& m_view;
	std::chrono::steady_clock::time_point m_lastTime;
};