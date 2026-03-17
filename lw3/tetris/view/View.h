#pragma once

#include "../model/Model.h"

class View
{
public:
	explicit View(const Model& m);

	void Render(int windowWidth, int windowHeight) const;

private:
	const Model& m_model;
};