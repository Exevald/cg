#pragma once

#include "../model/AlchemyModel.h"

class IAlchemyView
{
public:
	virtual ~IAlchemyView() = default;
	virtual void Render(const AlchemyModel& model) = 0;
	[[nodiscard]] virtual bool IsOpen() const = 0;
};