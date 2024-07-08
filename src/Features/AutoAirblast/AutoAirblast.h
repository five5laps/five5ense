#pragma once
#include "../../SDK/SDK.h"

class CAutoAirblast
{
	bool CanAirblastEntity(C_TFPlayer* pLocal, C_BaseEntity* pEntity, Vector& vAngle, Vector& vPos);
public:
	void Run(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* pCmd);
};

namespace F { inline CAutoAirblast AutoAirblast; }
