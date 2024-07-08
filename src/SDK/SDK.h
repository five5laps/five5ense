#pragma once

#include "Helpers/Input/Input.h"
#include "Helpers/EntityManager/EntityManager.h"
#include "Impl/TraceFilters/TraceFilters.h"
#include "Entities/C_BaseProjectile.h"
#include "Interfaces/CGlobalVarsBase.h"
#include "Interfaces/IBaseClientDLL.h"
#include "Interfaces/IClientEntityList.h"
#include "Interfaces/IEngineTrace.h"
#include "Interfaces/IGameEventManager.h"
#include "Interfaces/IGameMovement.h"
#include "Interfaces/IMatSystemSurface.h"
#include "Interfaces/IMoveHelper.h"
#include "Interfaces/IPrediction.h"
#include "Interfaces/IVEngineClient.h"
#include "Interfaces/IVModelInfo.h"
#include "Interfaces/IVRenderView.h"
#include "Interfaces/IEngineVGui.h"

#include "Entities/C_BaseAnimating.h"
#include "Entities/C_BaseCombatCharacter.h"
#include "Entities/C_BaseCombatWeapon.h"
#include "Entities/C_BaseEntity.h"
#include "Entities/C_BaseFlex.h"
#include "Includes/basehandle.h"
#include "Includes/basetypes.h"
#include "Entities/C_BasePlayer.h"
#include "Entities/C_BaseProjectile.h"
#include "Entities/C_EntitySphereQuery.h"
#include "Entities/C_TFPlayer.h"
#include "Entities/C_TFWeaponBase.h"
#include "Entities/IClientEntity.h"
#include "Entities/IClientNetworkable.h"
#include "Entities/IClientRenderable.h"
#include "Entities/IClientThinkable.h"
#include "Entities/IClientUnknown.h"
#include "Includes/ihandleentity.h"
#include "Includes/checksum_md5.h"
#include "Includes/usercmd.h"


#include "../Util/Memory/Memory.h"
#include "../Util/Signatures/Signatures.h"

namespace I { inline void* ClientMode = nullptr; }

namespace Util
{
	inline Color GetHealthColor(const int nHealth, const int nMaxHealth)
	{
		if (nHealth > nMaxHealth)
			return { 15, 150, 150, 255 };

		const int nHP = U::Math.Max(0, U::Math.Min(nHealth, nMaxHealth));

		const unsigned int nR = static_cast<unsigned int>(U::Math.Min((510 * (nMaxHealth - nHP)) / nMaxHealth, 200));
		const unsigned int nG = static_cast<unsigned int>(U::Math.Min((510 * nHP) / nMaxHealth, 200));

		return { static_cast<byte>(nR), static_cast<byte>(nG), 0, 255 };
	}

	inline Color GetTeamColor(const int nTeam)
	{
		if (nTeam == 2)
			return COLOR_TF_RED;
		else if (nTeam == 3)
			return COLOR_TF_BLUE;
		else
			return COLOR_TF_SPECTATOR;
	}

	inline float GetLatency()
	{
		if (auto pNet = I::EngineClient->GetNetChannelInfo())
			return pNet->GetLatency(FLOW_INCOMING) + pNet->GetLatency(FLOW_OUTGOING);

		return 0.0f;
	}

	__inline void Trace(const Vector& vecStart, const Vector& vecEnd, unsigned int nMask, CTraceFilter* pFilter, CGameTrace* pTrace)
	{
		Ray_t ray;
		ray.Init(vecStart, vecEnd);
		I::EngineTrace->TraceRay(ray, nMask, pFilter, pTrace);
	}

	__inline bool VisPos(C_BaseEntity* pSkip, const C_BaseEntity* pEntity, const Vector& from, const Vector& to, unsigned int nMask = MASK_SHOT | CONTENTS_GRATE)
	{
		CGameTrace trace = {};
		CTraceFilterHitscan filter = {};
		filter.m_pIgnore = pSkip;
		Trace(from, to, nMask, &filter, &trace);
		if (trace.DidHit())
			return trace.entity && trace.entity == pEntity;
		return true;
	}


	inline float __declspec (naked) __fastcall FastSqrt(float n)
	{
		_asm fld qword ptr[esp + 4]
			_asm fsqrt
		_asm ret 8
	}

	__inline void FixMovement(CUserCmd* pCmd, const Vector& vecTargetAngle)
	{
		const Vector vecMove(pCmd->forwardmove, pCmd->sidemove, pCmd->upmove);
		Vector vecMoveAng = Vector();

		U::Math.VectorAngles(vecMove, vecMoveAng);

		const float fSpeed = FastSqrt(vecMove.x * vecMove.x + vecMove.y * vecMove.y);
		const float fYaw = DEG2RAD(vecTargetAngle.y - pCmd->viewangles.y + vecMoveAng.y);

		pCmd->forwardmove = (cos(fYaw) * fSpeed);
		pCmd->sidemove = (sin(fYaw) * fSpeed);
	}

	inline ETextures GetClassIcon(const int nClass)
	{
		switch (nClass)
		{
		case TF_CLASS_SCOUT: return ETextures::ICON_SCOUT;
		case TF_CLASS_SNIPER: return ETextures::ICON_SNIPER;
		case TF_CLASS_SOLDIER: return ETextures::ICON_SOLDIER;
		case TF_CLASS_DEMOMAN: return ETextures::ICON_DEMO;
		case TF_CLASS_MEDIC: return ETextures::ICON_MEDIC;
		case TF_CLASS_HEAVYWEAPONS: return ETextures::ICON_HEAVY;
		case TF_CLASS_PYRO: return ETextures::ICON_PYRO;
		case TF_CLASS_SPY: return ETextures::ICON_SPY;
		case TF_CLASS_ENGINEER: return ETextures::ICON_ENGI;
		default: break;
		}

		return ETextures::TEXTURE_LAST;
	}
}