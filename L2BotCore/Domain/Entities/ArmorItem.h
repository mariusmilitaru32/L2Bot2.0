#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "BaseItem.h"
#include "../Enums/ArmorType.h"
#include "../Enums/CrystalType.h"

namespace L2Bot::Domain::Entities
{
	class ArmorItem : public BaseItem
	{
	public:
		void Update(const EntityInterface* other) override
		{
			const ArmorItem* casted = static_cast<const ArmorItem*>(other);

			BaseItem::Update(other);

			m_IsEquipped = casted->m_IsEquipped;
			m_EnchantLevel = casted->m_EnchantLevel;
			m_ArmorType = casted->m_ArmorType;
			m_CrystalType = casted->m_CrystalType;
			m_PDef = casted->m_PDef;
			m_MDef = casted->m_MDef;
			m_SetEffect = casted->m_SetEffect;
			m_AddSetEffect = casted->m_AddSetEffect;
			m_EnchantEffect = casted->m_EnchantEffect;
		}
		void SaveState() override
		{
			BaseItem::SaveState();
			m_PrevState =
			{
				m_IsEquipped,
				m_EnchantLevel,
				m_PDef,
				m_MDef,
				false
			};
		}
		const bool IsEqual(const EntityInterface* other) const override
		{
			const ArmorItem* casted = static_cast<const ArmorItem*>(other);
			return BaseItem::IsEqual(other) &&
				m_IsEquipped == casted->m_IsEquipped &&
				m_EnchantLevel == casted->m_EnchantLevel &&
				m_ArmorType == casted->m_ArmorType &&
				m_CrystalType == casted->m_CrystalType &&
				m_PDef == casted->m_PDef &&
				m_MDef == casted->m_MDef &&
				m_SetEffect == casted->m_SetEffect &&
				m_AddSetEffect == casted->m_AddSetEffect &&
				m_EnchantEffect == casted->m_EnchantEffect;
		}

		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			std::vector<Serializers::Node> result = BaseItem::BuildSerializationNodes();

			if (m_PrevState.isNewState)
			{
				result.push_back({ "armorType", std::to_string(static_cast<uint8_t>(m_ArmorType)) });
				result.push_back({ "crystalType", std::to_string(static_cast<int8_t>(m_CrystalType)) });
				result.push_back({ "setEffect", m_SetEffect });
				result.push_back({ "addSetEffect", m_AddSetEffect });
				result.push_back({ "enchantEffect", m_EnchantEffect });
			}

			if (m_PrevState.isNewState || m_IsEquipped != m_PrevState.isEquipped)
			{
				result.push_back({ "isEquipped", std::to_string(m_IsEquipped) });
			}
			if (m_PrevState.isNewState || m_EnchantLevel != m_PrevState.enchantLevel)
			{
				result.push_back({ "enchantLevel", std::to_string(m_EnchantLevel) });
			}
			if (m_PrevState.isNewState || m_PDef != m_PrevState.pDef)
			{
				result.push_back({ "pDef", std::to_string(m_PDef) });
			}
			if (m_PrevState.isNewState || m_MDef != m_PrevState.mDef)
			{
				result.push_back({ "mDef", std::to_string(m_MDef) });
			}

			return result;
		}

		ArmorItem(
			const uint32_t objectId,
			const uint32_t itemId,
			const int32_t mana,
			const std::string& name,
			const std::string& iconName,
			const std::string& description,
			const uint16_t weight,
			const bool isEquipped,
			const uint16_t enchantLevel,
			const Enums::ArmorType armorType,
			const Enums::CrystalType crystalType,
			const uint32_t pDef,
			const uint32_t mDef,
			const std::string& setEffect,
			const std::string& addSetEffect,
			const std::string& enchantEffect
		) :
			BaseItem
			(
				objectId,
				itemId,
				mana,
				name,
				iconName,
				description,
				weight,
				Enums::ItemType::armor
			),
			m_IsEquipped(isEquipped),
			m_EnchantLevel(enchantLevel),
			m_ArmorType(armorType),
			m_CrystalType(crystalType),
			m_PDef(pDef),
			m_MDef(mDef),
			m_SetEffect(setEffect),
			m_AddSetEffect(addSetEffect),
			m_EnchantEffect(enchantEffect)
		{
		}

		ArmorItem(const ArmorItem* other) :
			BaseItem(other),
			m_IsEquipped(other->m_IsEquipped),
			m_EnchantLevel(other->m_EnchantLevel),
			m_ArmorType(other->m_ArmorType),
			m_CrystalType(other->m_CrystalType),
			m_PDef(other->m_PDef),
			m_MDef(other->m_MDef),
			m_SetEffect(other->m_SetEffect),
			m_AddSetEffect(other->m_AddSetEffect),
			m_EnchantEffect(other->m_EnchantEffect)
		{
		}

		ArmorItem() = default;
		virtual ~ArmorItem() = default;

	private:
		struct GetState
		{
			bool isEquipped = 0;
			uint16_t enchantLevel = 0;
			uint32_t pDef = 0;
			uint32_t mDef = 0;

			bool isNewState = true;
		};

	private:
		bool m_IsEquipped = 0;
		uint16_t m_EnchantLevel = 0;
		Enums::ArmorType m_ArmorType = Enums::ArmorType::none;
		Enums::CrystalType m_CrystalType = Enums::CrystalType::none;
		uint32_t m_PDef = 0;
		uint32_t m_MDef = 0;
		std::string m_SetEffect = "";
		std::string m_AddSetEffect = "";
		std::string m_EnchantEffect = "";
		GetState m_PrevState = GetState();
	};
}