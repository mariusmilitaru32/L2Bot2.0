#pragma once


#include "../../../Common/Common.h"

namespace Interlude
{
	class NPCFactory
	{
	public:
		NPCFactory() = default;
		virtual ~NPCFactory() = default;

		const DTO::NPC Create(const User* item, const Enums::SpoilStateEnum spoiledState) const
		{
			return DTO::NPC{
				item->objectId,
				ValueObjects::Transform(
					ValueObjects::Vector3(item->pawn->Location.x, item->pawn->Location.y, item->pawn->Location.z),
					ValueObjects::Vector3(
						static_cast<float_t>(item->pawn->Rotation.Pitch),
						static_cast<float_t>(item->pawn->Rotation.Yaw),
						static_cast<float_t>(item->pawn->Rotation.Roll)
					),
					ValueObjects::Vector3(item->pawn->Velocity.x, item->pawn->Velocity.y, item->pawn->Velocity.z),
					ValueObjects::Vector3(item->pawn->Acceleration.x, item->pawn->Acceleration.y, item->pawn->Acceleration.z)
				),
				item->isMob != 0,
				item->npcId,
				spoiledState,
				ValueObjects::FullName(
					ConvertFromWideChar(item->nickname),
					ConvertFromWideChar(item->title)
				),
				ValueObjects::VitalStats(
					item->maxHp, item->hp,
					item->maxMp, item->mp,
					item->maxCp, item->cp
				)
			};
		}
	};
}