#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "EntityInterface.h"
#include "../Enums/ItemTypeEnum.h"

namespace L2Bot::Domain::Entities
{
	class BaseItem : public EntityInterface
	{
	public:
		const uint32_t GetId() const override
		{
			return m_ObjectId;
		}
		const uint32_t GetItemId() const
		{
			return m_ItemId;
		}
		virtual void Update(const EntityInterface* other) override
		{
			const BaseItem* casted = static_cast<const BaseItem*>(other);
			SaveState();

			m_ObjectId = casted->m_ObjectId;
			m_ItemId = casted->m_ItemId;
			m_Mana = casted->m_Mana;
			m_Name = casted->m_Name;
			m_IconName = casted->m_IconName;
			m_Description = casted->m_Description;
			m_Weight = casted->m_Weight;
			m_Type = casted->m_Type;
		}
		virtual void SaveState() override
		{
			m_PrevState =
			{
				m_Mana,
				m_Weight,
				false
			};
		}
		virtual const bool IsEqual(const EntityInterface* other) const override
		{
			const BaseItem* casted = static_cast<const BaseItem*>(other);
			return m_ObjectId == casted->m_ObjectId &&
			    m_ItemId == casted->m_ItemId &&
				m_Mana == casted->m_Mana &&
				m_Name == casted->m_Name &&
				m_IconName == casted->m_IconName &&
				m_Description == casted->m_Description &&
				m_Weight == casted->m_Weight &&
				m_Type == casted->m_Type;
		}

		virtual const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			std::vector<Serializers::Node> result;

			result.push_back({ L"id", std::to_wstring(m_ObjectId) });
			result.push_back({ L"itemId", std::to_wstring(m_ItemId) });

			if (m_PrevState.isNewState)
			{
				result.push_back({ L"type", std::to_wstring(static_cast<int8_t>(m_Type))});
				result.push_back({ L"name", m_Name });
				result.push_back({ L"iconName", m_IconName });
				result.push_back({ L"description", m_Description });
			}

			if (m_PrevState.isNewState || m_Mana != m_PrevState.mana)
			{
				result.push_back({ L"mana", std::to_wstring(m_Mana) });
			}
			if (m_PrevState.isNewState || m_Weight != m_PrevState.weight)
			{
				result.push_back({ L"weight", std::to_wstring(m_Weight) });
			}

			return result;
		}

		BaseItem(
			const uint32_t objectId,
			const uint32_t itemId,
			const int32_t mana,
			const std::wstring& name,
			const std::wstring& iconName,
			const std::wstring& description,
			const uint16_t weight,
			const Enums::ItemTypeEnum type
		) :
			m_ObjectId(objectId),
			m_ItemId(itemId),
			m_Mana(mana),
			m_Name(name),
			m_IconName(iconName),
			m_Description(description),
			m_Weight(weight),
			m_Type(type)
		{
		}

		BaseItem(const BaseItem* other) :
			m_ObjectId(other->m_ObjectId),
			m_ItemId(other->m_ItemId),
			m_Mana(other->m_Mana),
			m_Name(other->m_Name),
			m_IconName(other->m_IconName),
			m_Description(other->m_Description),
			m_Weight(other->m_Weight),
			m_Type(other->m_Type)
		{
		}

		BaseItem() = default;
		virtual ~BaseItem() = default;

	private:
		struct GetState
		{
			int32_t mana = -1;
			uint16_t weight = 0;

			bool isNewState = true;
		};

	private:
		uint32_t m_ObjectId = 0;
		uint32_t m_ItemId = 0;
		int32_t m_Mana = -1;
		std::wstring m_Name = L"";
		std::wstring m_IconName = L"";
		std::wstring m_Description = L"";
		uint16_t m_Weight = 0;
		Enums::ItemTypeEnum m_Type = Enums::ItemTypeEnum::none;
		GetState m_PrevState = GetState();
	};
}
