#include "AlchemyController.h"

void AlchemyController::ProcessInput()
{
	while (const std::optional event = m_view.GetWindow().pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			m_view.GetWindow().close();
		}
		else if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
		{
			HandleMouseButtonPressed(mousePressed);
		}
		else if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>())
		{
			HandleMouseButtonReleased(mouseButtonReleased);
		}
		else if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
		{
			HandleMouseMoved(mouseMoved);
		}
	}
}

void AlchemyController::HandleMouseButtonPressed(const sf::Event::MouseButtonPressed* mousePressed)
{
	const Vec2 mousePos = {
		static_cast<float>(mousePressed->position.x),
		static_cast<float>(mousePressed->position.y)
	};

	if (const auto sidebarId = SFMLView::GetSidebarElementAt(mousePos, m_model))
	{
		m_model.AddItemToField(*sidebarId, { 400, 300 });
	}
	else
	{
		const auto& fieldItems = m_model.GetFieldItems();
		for (int i = 0; i < fieldItems.size(); ++i)
		{
			if (mousePos.x >= fieldItems[i].position.x
				&& mousePos.x <= fieldItems[i].position.x + 50
				&& mousePos.y >= fieldItems[i].position.y
				&& mousePos.y <= fieldItems[i].position.y + 50)
			{
				m_model.StartDragging(i);
				m_dragOffset = {
					fieldItems[i].position.x - mousePos.x,
					fieldItems[i].position.y - mousePos.y
				};
				break;
			}
		}
	}
}

void AlchemyController::HandleMouseButtonReleased(const sf::Event::MouseButtonReleased* mouseReleased) const
{
	if (const int draggedIndex = m_model.GetDraggedIndex();
		draggedIndex != -1)
	{
		if (const Vec2 mousePos = {
				static_cast<float>(mouseReleased->position.x),
				static_cast<float>(mouseReleased->position.y) };
			SFMLView::IsInTrashZone(mousePos))
		{
			m_model.RemoveItemFromField(draggedIndex);
		}
		else
		{
			const auto& items = m_model.GetFieldItems();
			auto [draggedX, draggedY] = items[draggedIndex].position;

			for (int itemIndex = 0; itemIndex < static_cast<int>(items.size()); ++itemIndex)
			{
				if (itemIndex == draggedIndex)
				{
					continue;
				}
				const float deltaX = std::abs(items[itemIndex].position.x - draggedX);
				const float deltaY = std::abs(items[itemIndex].position.y - draggedY);

				if (deltaX < 40 && deltaY < 40)
				{
					m_model.TryCombine(itemIndex, draggedIndex);
					break;
				}
			}
		}
		m_model.StopDragging();
	}
}

void AlchemyController::HandleMouseMoved(const sf::Event::MouseMoved* mouseMoved) const
{
	if (m_model.GetDraggedIndex() != -1)
	{
		m_model.UpdateDrag({ static_cast<float>(mouseMoved->position.x) + m_dragOffset.x,
			static_cast<float>(mouseMoved->position.y) + m_dragOffset.y });
	}
}