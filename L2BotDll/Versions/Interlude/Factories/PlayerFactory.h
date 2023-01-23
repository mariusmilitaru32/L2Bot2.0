#pragma once

#include <memory>
#include "../../../Common/Common.h"
#include "Domain/Entities/Player.h"

namespace Interlude
{
	class PlayerFactory
	{
	public:
		PlayerFactory() = default;
		virtual ~PlayerFactory() = default;

		std::unique_ptr<Entities::Player> Create(const User* item) const
		{
			return std::make_unique<Entities::Player>(
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
				ValueObjects::FullName(
					ConvertFromWideChar(item->nickname),
					ConvertFromWideChar(item->title)
				),
				ValueObjects::Phenotype(
					(Enums::RaceEnum)item->raceId,
					item->gender == L2::Gender::MALE,
					(Enums::ClassEnum)item->classId,
					(Enums::ClassEnum)item->activeClassId
				)
			);
		}
	};
}