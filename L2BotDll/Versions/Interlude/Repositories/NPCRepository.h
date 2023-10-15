#pragma once

#include <map>
#include <shared_mutex>
#include "../GameStructs/NetworkHandlerWrapper.h"
#include "Domain/Repositories/EntityRepositoryInterface.h"
#include "../Factories/NPCFactory.h"
#include "../../../Events/EventDispatcher.h"
#include "../../../Events/SpoiledEvent.h"
#include "../../../Events/CreatureDiedEvent.h"
#include "../../GameStructs/FindObjectsTrait.h"

using namespace L2Bot::Domain;

namespace Interlude
{
	class NPCRepository : public Repositories::EntityRepositoryInterface, public FindObjectsTrait
	{
	public:
		const std::unordered_map<std::uint32_t, std::shared_ptr<Entities::EntityInterface>> GetEntities() override
		{
			std::unique_lock<std::shared_timed_mutex>(m_Mutex);

			const auto allCreatures = FindAllObjects<User*>(m_Radius, [this](float_t radius, int32_t prevId) {
				return m_NetworkHandler.GetNextCreature(radius, prevId);
			});

			std::unordered_map<std::uint32_t, std::shared_ptr<Entities::EntityInterface>> result;
			for (const auto kvp : allCreatures) {
				const auto creature = kvp.second;
				if (creature->userType != L2::UserType::NPC) {
					continue;
				}

				if (m_Npcs.find(creature->objectId) == m_Npcs.end()) {
					m_Npcs[creature->objectId] = m_Factory.Create(creature);
				}
				else
				{
					m_Factory.Update(m_Npcs[creature->objectId], creature);
				}

				const auto spoilState = m_Spoiled.find(creature->objectId) == m_Spoiled.end() ? Enums::SpoilStateEnum::none : m_Spoiled[creature->objectId];
				m_Npcs[creature->objectId]->UpdateSpoilState(spoilState);

				result[creature->objectId] = m_Npcs[creature->objectId];
			}

			return result;
		}

		void Reset() override
		{
			std::shared_lock<std::shared_timed_mutex>(m_Mutex);
			m_Npcs.clear();
		}

		NPCRepository(const NetworkHandlerWrapper& networkHandler, const NPCFactory& factory, const uint16_t radius) :
			m_NetworkHandler(networkHandler),
			m_Factory(factory),
			m_Radius(radius)
		{
			EventDispatcher::GetInstance().Subscribe(SpoiledEvent::name, [this](const Event& evt) {
				OnSpoiled(evt);
			});
			EventDispatcher::GetInstance().Subscribe(CreatureDiedEvent::name, [this](const Event& evt) {
				OnCreatureDied(evt);
			});
		}

		NPCRepository() = delete;
		virtual ~NPCRepository() = default;

		void OnSpoiled(const Event& evt)
		{
			std::shared_lock<std::shared_timed_mutex>(m_Mutex);
			if (evt.GetName() == SpoiledEvent::name)
			{
				const auto casted = static_cast<const SpoiledEvent&>(evt);
				const auto hero = m_NetworkHandler.GetHero();
				if (hero && hero->pawn && hero->pawn->lineagePlayerController)
				{
					const auto targetId = hero->pawn->lineagePlayerController->targetObjectId;
					if (targetId)
					{
						m_Spoiled[targetId] = Enums::SpoilStateEnum::spoiled;
					}
				}
			}
		}

		void OnCreatureDied(const Event& evt)
		{
			std::shared_lock<std::shared_timed_mutex>(m_Mutex);
			if (evt.GetName() == CreatureDiedEvent::name)
			{
				const auto casted = static_cast<const CreatureDiedEvent&>(evt);
				if (m_Spoiled.find(casted.GetCreatureId()) != m_Spoiled.end())
				{
					const auto isSweepable = casted.GetCreatureInfo()[4] != 0;
					if (m_Spoiled[casted.GetCreatureId()] == Enums::SpoilStateEnum::spoiled && isSweepable)
					{
						m_Spoiled[casted.GetCreatureId()] = Enums::SpoilStateEnum::sweepable;
					}
					else
					{
						m_Spoiled[casted.GetCreatureId()] = Enums::SpoilStateEnum::none;
					}
				}
			}
		}

	private:
		const NPCFactory& m_Factory;
		std::map<uint32_t, Enums::SpoilStateEnum> m_Spoiled;
		const NetworkHandlerWrapper& m_NetworkHandler;
		const uint16_t m_Radius = 0;
		std::shared_timed_mutex m_Mutex;
		std::unordered_map<uint32_t, std::shared_ptr<Entities::NPC>> m_Npcs;
	};
}