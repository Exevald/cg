#pragma once

#include "Types.h"

#include <map>
#include <vector>

class AlchemyModel
{

public:
	AlchemyModel();

	void Initialize();
	void AddItemToField(int elementId, Vec2 pos);
	void RemoveItemFromField(int index);
	void TryCombine(int itemIndex1, int itemIndex2);

	void StartDragging(int index);
	void UpdateDrag(Vec2 newPos);
	void StopDragging();

	[[nodiscard]] const std::vector<Element>& GetElements() const { return m_elements; }
	[[nodiscard]] const std::vector<ActiveItem>& GetFieldItems() const { return m_itemsOnField; }
	[[nodiscard]] const std::string& GetStatusMessage() const { return m_fieldMessage; }
	[[nodiscard]] int GetDraggedIndex() const { return m_draggedItemIndex; }

	[[nodiscard]] bool IsGameComplete() const;

private:
	std::vector<Element> m_elements;
	std::map<std::pair<int, int>, std::vector<int>> m_recipes;
	std::vector<ActiveItem> m_itemsOnField;
	std::string m_fieldMessage;
	int m_draggedItemIndex = -1;
};