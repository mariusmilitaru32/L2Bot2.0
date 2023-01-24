#pragma once

#include <cstdint>
#include <vector>
#include "Event.h"
#include "../DTO/ItemData.h"

class ItemCreatedEvent : public Event
{
public:
	static constexpr const char* name = "itemCreated";

	const std::string GetName() const
	{
		return std::string(name);
	}

	const ItemData& GetItemData() const
	{
		return m_ItemData;
	}

	ItemCreatedEvent(const ItemData itemData) :
		m_ItemData(itemData)
	{

	}

	ItemCreatedEvent() = delete;
	virtual ~ItemCreatedEvent() = default;

private:
	const ItemData m_ItemData;
};