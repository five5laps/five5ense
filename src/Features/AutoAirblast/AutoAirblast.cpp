#include "AutoAirblast.h"
#include "../../Config.h"
#include <cmath>

struct TargetProjectile
{
	C_BaseProjectile* Projectile = nullptr;
	Vector Position = {};
};

/*
bool VisPos(C_BaseEntity* pIgnore, const C_BaseEntity* pEntity, const Vector& from, const Vector& to, unsigned int nMask)
{
	CGameTrace trace = {};
	CTraceFilterHitscan filter = {};
	filter.m_pIgnore = pIgnore;
	Trace(from, to, nMask, &filter, &trace);
	if (trace.DidHit())
		return trace.m_pEnt && trace.m_pEnt == pEntity;
	return true;
}

void FixMovement(CUserCmd* pCmd, const Vector& vecTargetAngle)
{
	const Vector vecMove(pCmd->forwardmove, pCmd->sidemove, pCmd->upmove);
	Vector vecMoveAng = Vector();

	U::Math.VectorAngles(vecMove, vecMoveAng);

	const float fSpeed = std::sqrt(vecMove.x * vecMove.x + vecMove.y * vecMove.y);
	const float fYaw = DEG2RAD(vecTargetAngle.y - pCmd->viewangles.y + vecMoveAng.y);

	pCmd->forwardmove = (cos(fYaw) * fSpeed);
	pCmd->sidemove = (sin(fYaw) * fSpeed);
}
*/


bool FindTargetProjectile(C_TFPlayer* local, TargetProjectile& outTarget)
{
	for (const auto pEntity : H::Entities.GetGroup(EEntGroup::PROJECTILES_ENEMIES))
	{
		if (!pEntity)
		{
			continue;
		}

		const auto pProjectile = pEntity->As<C_BaseProjectile>();

		if (!pProjectile)
		{
			continue;
		}
		/*

		if (CFG::Triggerbot_AutoAirblast_Ignore_Rocket && pProjectile->GetClassId() == ETFClassIds::CTFProjectile_Rocket)
		{
			continue;
		}

		if (CFG::Triggerbot_AutoAirblast_Ignore_SentryRocket && pProjectile->GetClassId() == ETFClassIds::CTFProjectile_SentryRocket)
		{
			continue;
		}

		if (CFG::Triggerbot_AutoAirblast_Ignore_Jar && pProjectile->GetClassId() == ETFClassIds::CTFProjectile_Jar)
		{
			continue;
		}

		if (CFG::Triggerbot_AutoAirblast_Ignore_JarGas && pProjectile->GetClassId() == ETFClassIds::CTFProjectile_JarGas)
		{
			continue;
		}

		if (CFG::Triggerbot_AutoAirblast_Ignore_JarMilk && pProjectile->GetClassId() == ETFClassIds::CTFProjectile_JarMilk)
		{
			continue;
		}

		if (CFG::Triggerbot_AutoAirblast_Ignore_Arrow && pProjectile->GetClassId() == ETFClassIds::CTFProjectile_Arrow)
		{
			continue;
		}

		if (CFG::Triggerbot_AutoAirblast_Ignore_Flare && pProjectile->GetClassId() == ETFClassIds::CTFProjectile_Flare)
		{
			continue;
		}

		if (CFG::Triggerbot_AutoAirblast_Ignore_Cleaver && pProjectile->GetClassId() == ETFClassIds::CTFProjectile_Cleaver)
		{
			continue;
		}

		if (CFG::Triggerbot_AutoAirblast_Ignore_HealingBolt && pProjectile->GetClassId() == ETFClassIds::CTFProjectile_HealingBolt)
		{
			continue;
		}

		if (CFG::Triggerbot_AutoAirblast_Ignore_PipebombProjectile && pProjectile->GetClassId() == ETFClassIds::CTFGrenadePipebombProjectile)
		{
			continue;
		}
		*/


		Vector vel{};
		pProjectile->EstimateAbsVelocity(vel);

		
		if (pProjectile->GetClassId() == ETFClassIds::CTFGrenadePipebombProjectile
			&& (pProjectile->As<C_TFGrenadePipebombProjectile>()->m_bTouched()
				|| pProjectile->As<C_TFGrenadePipebombProjectile>()->m_iType() == TF_PROJECTILE_PIPEBOMB_PRACTICE))
		{
			continue;
		}
		

		/*
		if (pProjectile->GetClassId() == ETFClassIds::CTFProjectile_Arrow && fabsf(vel.Length()) <= 10.0f)
		{
			continue;
		}
		*/

		auto pos = pProjectile->m_vecOrigin() + (vel * Util::GetLatency());

		if (pos.DistTo(local->GetShootPos()) > 160.0f)
		{
			continue;
		}

		//legit

		
		if (!Config::Misc::Trigger::AutoAirBlast::rage
			&& U::Math.GetFovBetween(I::EngineClient->GetViewAngles(), U::Math.GetAngleToPosition(local->GetShootPos(), pos)) > 60.0f)
		{
			continue;
		}
		
		
		CTraceFilterWorldCustom filter{};
		trace_t trace{};

		Util::Trace(local->GetShootPos(), pos, MASK_SOLID, &filter, &trace);

		if (trace.flFraction < 1.0f || trace.bAllSolid || trace.bStartSolid)
		{
			continue;
		}

		outTarget.Projectile = pProjectile;
		outTarget.Position = pos;
		

		return true;
	}

	return false;
}




/*
bool CAutoAirblast::CanAirblastEntity(C_TFPlayer* pLocal, C_BaseEntity* pEntity, Vector& vAngle, Vector& vPos)
{
	Vector vForward = {}; U::Math.AngleVectors(vAngle, &vForward);
	const Vector vOrigin = pLocal->GetShootPos() + (vForward * 128.f);

	C_BaseEntity* pTarget;
	for (CEntitySphereQuery sphere(vOrigin, 128.f);
		(pTarget = sphere.GetCurrentEntity()) != nullptr;
		sphere.NextEntity())
	{
		if (pTarget == pEntity)
			break;
	}

	return pTarget == pEntity && VisPos(pLocal, pEntity, pLocal->GetShootPos(), vPos, 1174421515U);
}
*/


bool CAutoAirblast::CanAirblastEntity(C_TFPlayer* pLocal, C_BaseEntity* pEntity, Vector& vAngle, Vector& vPos)
{
	Vector vForward = {}; U::Math.AngleVectors(vAngle, &vForward);
	const Vector vOrigin = pLocal->GetShootPos() + (vForward * 128.f);

	C_BaseEntity* pTarget;
	for (CEntitySphereQuery sphere(vOrigin, 128.f);
		(pTarget = sphere.GetCurrentEntity()) != nullptr;
		sphere.NextEntity())
	{
		if (pTarget == pEntity)
			break;
	}

	return pTarget == pEntity && Util::VisPos(pLocal, pEntity, pLocal->GetShootPos(), vPos);
}

void CAutoAirblast::Run(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* pCmd)
{
	if (Config::Misc::Trigger::AutoAirBlast::autoAirBlastTest)
	{
		pCmd->buttons |= IN_ATTACK2;
	}

	/*
	if (!Config::Misc::Trigger::AutoAirBlast::autoAirBlast)
		return;

	const int iWeaponID = pWeapon->GetWeaponID();
	if (iWeaponID != TF_WEAPON_FLAMETHROWER && iWeaponID != TF_WEAPON_FLAME_BALL)
		return;

	const Vector vEyePos = pLocal->GetShootPos();
	bool bShouldBlast = false;

	for (auto pProjectile : H::Entities.GetGroup(EEntGroup::WORLD_PROJECTILES))
	{
		if (pProjectile->m_iTeamNum() == pLocal->m_iTeamNum())
			continue;

		Vector vPos = pProjectile->m_vecOrigin();
		//if (Math::GetFov(I::EngineClient->GetViewAngles(), vEyePos, vPos) > Vars::Aimbot::General::AimFOV.Value)
		//	continue;

		if (CanAirblastEntity(pLocal, pProjectile, pCmd->viewangles, vPos))
		{
			bShouldBlast = true;
			break;
		}
		if (!bShouldBlast && Config::Misc::Trigger::AutoAirBlast::rage) // possibly implement proj aimbot somehow ?
		{
			Vector vAngle = U::Math.GetAngleToPosition(vEyePos, vPos);
			if (CanAirblastEntity(pLocal, pProjectile, vAngle, vPos))
			{
				FixMovement(pCmd, vAngle);
				pCmd->viewangles = vAngle;
				//G::PSilentAngles = true;
				bShouldBlast = true;
				break;
			}
		}
	}


	if (bShouldBlast)
	{
		//G::IsAttacking = true;
		pCmd->buttons |= IN_ATTACK2;
	}
	*/


	/*
	if (!Config::Misc::Trigger::AutoAirBlast::autoAirBlast)
	{
		return;
	}

	if ((pWeapon->GetWeaponID() != TF_WEAPON_FLAMETHROWER && pWeapon->GetWeaponID() != TF_WEAPON_FLAME_BALL)
		|| pWeapon->m_iItemDefinitionIndex() == Pyro_m_ThePhlogistinator)
	{
		return;
	}

	TargetProjectile targetProjectile{};

	if (!FindTargetProjectile(pLocal, targetProjectile))
	{
		return;

	}


	pCmd->buttons |= IN_ATTACK2;
	// Rage airblast
	if (Config::Misc::Trigger::AutoAirBlast::rage)
	{
		// Config::Misc::Trigger::AutoAirBlast::assist
		if (true)
		{
			auto getOwner = [](C_BaseProjectile* proj) -> C_BaseEntity* {
				const auto owner{ proj->m_hOwnerEntity().Get() };

				if (owner)
				{
					return owner;
				}
				if (proj->GetClassId() == ETFClassIds::CTFGrenadePipebombProjectile)
				{
					if (const auto launcher = proj->As<C_TFGrenadePipebombProjectile>()->m_hLauncher().Get())
					{
						return launcher->m_hOwnerEntity().Get();
					}
				}

				return nullptr;
				};

			const auto owner = getOwner(targetProjectile.Projectile);
			if (owner && owner->GetClassId() == ETFClassIds::CTFPlayer)
			{
				CTraceFilterWorldCustom filter{};
				trace_t trace{};

				Util::Trace(pLocal->GetShootPos(), owner->GetCenter(), MASK_SOLID, &filter, &trace);

				if (trace.flFraction > 0.99f && !trace.bAllSolid && !trace.bStartSolid)
				{
					pCmd->viewangles = U::Math.GetAngleToPosition(pLocal->GetShootPos(), owner->GetCenter());
				}

				else
				{
					pCmd->viewangles = U::Math.GetAngleToPosition(pLocal->GetShootPos(), targetProjectile.Position);
				}
			}
		}

		else
		{
			pCmd->viewangles = U::Math.GetAngleToPosition(pLocal->GetShootPos(), targetProjectile.Position);
			pCmd->buttons |= IN_ATTACK2;
		}

	}
	*/


	if (!Config::Misc::Trigger::AutoAirBlast::autoAirBlast)
		return;

	const int iWeaponID = pWeapon->GetWeaponID();

	if (iWeaponID != TF_WEAPON_FLAMETHROWER && iWeaponID != TF_WEAPON_FLAME_BALL)
		return;

	const auto& pNet = I::EngineClient->GetNetChannelInfo();
	const float flLatency = pNet ? pNet->GetLatency(FLOW_INCOMING) + pNet->GetLatency(FLOW_OUTGOING) : 0.f;

	const Vector& vEyePos = pLocal->GetShootPos();
	bool bShouldBlast = false;

	for (const auto& pProjectile : H::Entities.GetGroup(EEntGroup::WORLD_PROJECTILES))
	{
		if (pProjectile->m_iTeamNum() == pLocal->m_iTeamNum())
			continue;
		/*
		switch (pProjectile->GetClassId())
		{
			case ETFClassID::CTFGrenadePipebombProjectile:
			case ETFClassID::CTFStunBall:
			{
				if (pProjectile->m_bTouched())
					continue; // Ignore landed stickies and sandman balls
				break;
			}
			case ETFClassID::CTFProjectile_Arrow:
			{
				if (pProjectile->GetVelocity().IsZero())
					continue; // Ignore arrows with no velocity / not moving
			}
		}
		*/

		Vector vPos = pProjectile->m_vecOrigin();
		if (U::Math.GetFov(I::EngineClient->GetViewAngles(), vEyePos, vPos) > 60.f)
			continue;

		if (CanAirblastEntity(pLocal, pProjectile, pCmd->viewangles, vPos))
		{
			bShouldBlast = true;
			break;
		}
		if (!bShouldBlast && Config::Misc::Trigger::AutoAirBlast::rage) // possibly implement proj aimbot somehow ?
		{
			Vector vAngle = U::Math.GetAngleToPosition(vEyePos, vPos);
			if (CanAirblastEntity(pLocal, pProjectile, vAngle, vPos))
			{
				Util::FixMovement(pCmd, vAngle);
				pCmd->viewangles = vAngle;
				//G::PSilentAngles = true;
				bShouldBlast = true;
				break;
			}
		}
	}

	if (const auto& pNet = I::EngineClient->GetNetChannelInfo())
	{
		const Vector vEyePos = pLocal->GetShootPos();
		float flLatency = (pNet->GetLatency(FLOW_OUTGOING));

		// pretty sure the game shows the predicted position of projectiles so accounting for incoming ping seems useless.
		bool bShouldBlast = false;

		for (const auto& pProjectile : H::Entities.GetGroup(EEntGroup::WORLD_PROJECTILES))
		{
			if (pProjectile->GetTeamNumber() == pLocal->GetTeamNumber())
			{
				continue; //Ignore team's projectiles
			}


			Vector vPredicted = (pProjectile->GetAbsOrigin() + pProjectile->GetVelocity().Scale(flLatency));



			if (vEyePos.DistTo(vPredicted) <= 256.0f && Util::VisPos(pLocal, pProjectile, vEyePos, vPredicted))
			{
				//G::SilentTime = true;
				//G::IsAttacking = true;
				pCmd->viewangles = U::Math.GetAngleToPosition(vEyePos, vPredicted);
				pCmd->buttons |= IN_ATTACK2;
				break;
			}
		}

	}


	if (bShouldBlast)
	{
		pCmd->buttons |= IN_ATTACK2;
	}
	
}

