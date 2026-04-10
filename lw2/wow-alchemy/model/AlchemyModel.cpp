#include "AlchemyModel.h"
#include <algorithm>

AlchemyModel::AlchemyModel() { Initialize(); }

void AlchemyModel::Initialize()
{
	m_elements = {
		{ 0, "Fire", { 255, 0, 0 }, true },
		{ 1, "Water", { 0, 0, 255 }, true },
		{ 2, "Earth", { 139, 69, 19 }, true },
		{ 3, "Spirit", { 0, 255, 255 }, true },
		{ 4, "Lava", { 255, 69, 0 }, false },
		{ 5, "Mana", { 200, 0, 255 }, false },
		{ 6, "Iron_Ore", { 160, 160, 160 }, false },
		{ 7, "Steel", { 100, 100, 100 }, false },
		{ 8, "Arcane", { 150, 100, 255 }, false },
		{ 9, "Fel", { 50, 255, 50 }, false },
		{ 10, "Blood", { 150, 0, 0 }, false },
		{ 11, "Life", { 0, 200, 0 }, false },
		{ 12, "Night_Elf", { 100, 100, 250 }, false },
		{ 13, "Orc", { 50, 100, 50 }, false },
		{ 14, "Horde", { 150, 20, 20 }, false },
		{ 15, "Alliance", { 20, 20, 150 }, false },
		{ 16, "Frostmourne", { 200, 255, 255 }, false },
		{ 17, "Ashbringer", { 255, 200, 0 }, false },
		{ 18, "Lich King", { 50, 50, 100 }, false },
		{ 19, "Gold", { 255, 215, 0 }, false },
		{ 20, "Murloc", { 0, 255, 150 }, false },
		{ 21, "Dragon", { 200, 0, 50 }, false },
		{ 22, "Dark Portal", { 100, 0, 150 }, false },
		{ 23, "Hearthstone", { 50, 150, 250 }, false }
	};

	m_recipes[{ 0, 2 }] = { 4 }; // Fire + Earth = Lava
	m_recipes[{ 1, 3 }] = { 5, 11 }; // Water + Spirit = Mana and Life
	m_recipes[{ 2, 3 }] = { 6 }; // Earth + Spirit = Iron Ore
	m_recipes[{ 0, 6 }] = { 7 }; // Fire + Iron = Steel
	m_recipes[{ 0, 5 }] = { 8 }; // Fire + Mana = Arcane
	m_recipes[{ 0, 8 }] = { 9 }; // Fire + Arcane = Fel
	m_recipes[{ 1, 9 }] = { 10 }; // Water + Fel = Blood
	m_recipes[{ 3, 11 }] = { 12 }; // Spirit + Life = Night Elf
	m_recipes[{ 2, 9 }] = { 13 }; // Earth + Fel = Orc
	m_recipes[{ 0, 13 }] = { 14 }; // Fire + Orc = Horde
	m_recipes[{ 7, 12 }] = { 15 }; // Steel + Night Elf = Alliance
	m_recipes[{ 1, 7 }] = { 16 }; // Water(Ice) + Steel = Frostmourne
	m_recipes[{ 0, 7 }] = { 17 }; // Fire + Steel = Ashbringer
	m_recipes[{ 3, 16 }] = { 18 }; // Spirit + Frostmourne = Lich King
	m_recipes[{ 1, 11 }] = { 20 }; // Water + Life = Murloc
	m_recipes[{ 0, 11 }] = { 21 }; // Fire + Life = Dragon
	m_recipes[{ 4, 8 }] = { 22 }; // Lava + Arcane = Dark Portal
	m_recipes[{ 1, 19 }] = { 23 }; // Water + Gold = Hearthstone
	m_recipes[{ 2, 11 }] = { 19 }; // Earth + Life = Gold
}

void AlchemyModel::AddItemToField(const int elementId, const Vec2 pos)
{
	if (elementId >= 0 && elementId < static_cast<int>(m_elements.size()))
	{
		m_itemsOnField.push_back({ elementId, pos });
	}
}

void AlchemyModel::StartDragging(const int index)
{
	m_draggedItemIndex = index;
}

void AlchemyModel::StopDragging()
{
	m_draggedItemIndex = -1;
}

void AlchemyModel::UpdateDrag(const Vec2 newPos)
{
	if (m_draggedItemIndex != -1 && m_draggedItemIndex < static_cast<int>(m_itemsOnField.size()))
	{
		m_itemsOnField[m_draggedItemIndex].position = newPos;
	}
}

void AlchemyModel::TryCombine(const int itemIndex1, const int itemIndex2)
{
	if (itemIndex1 < 0
		|| itemIndex2 < 0
		|| itemIndex1 >= static_cast<int>(m_itemsOnField.size())
		|| itemIndex2 >= static_cast<int>(m_itemsOnField.size()))
	{
		return;
	}

	if (itemIndex1 == itemIndex2)
	{
		return;
	}

	const int itemId1 = m_itemsOnField[itemIndex1].elementId;
	const int itemId2 = m_itemsOnField[itemIndex2].elementId;

	const int firstId = std::min(itemId1, itemId2);
	const int secondId = std::max(itemId1, itemId2);
	const int highIdx = std::max(itemIndex1, itemIndex2);
	const int lowIdx = std::min(itemIndex1, itemIndex2);

	if (const auto recipeIt = m_recipes.find({ firstId, secondId });
		recipeIt != m_recipes.end())
	{
		const Vec2 spawnPos = m_itemsOnField[itemIndex1].position;

		m_itemsOnField.erase(m_itemsOnField.begin() + highIdx);
		m_itemsOnField.erase(m_itemsOnField.begin() + lowIdx);

		for (const int resourceId : recipeIt->second)
		{
			AddItemToField(resourceId, spawnPos);

			if (!m_elements[resourceId].isDiscovered)
			{
				m_elements[resourceId].isDiscovered = true;
				m_fieldMessage = "New item: " + m_elements[resourceId].name;
			}
			else
			{
				m_fieldMessage = "Created: " + m_elements[resourceId].name;
			}
		}
	}
	else
	{
		m_fieldMessage = "Nothing happens...";
	}
}

void AlchemyModel::RemoveItemFromField(const int index)
{
	if (index >= 0 && index < static_cast<int>(m_itemsOnField.size()))
	{
		m_itemsOnField.erase(m_itemsOnField.begin() + index);
	}
}

bool AlchemyModel::IsGameComplete() const
{
	return std::ranges::all_of(m_elements, [](const Element& e) { return e.isDiscovered; });
}