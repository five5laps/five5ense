#pragma once
#include "cmodel.h"
#include "utlvector.h"
#include "ihandleentity.h"
#include "ispatialpartition.h"

class CGameTrace : public CBaseTrace
{
public:
	bool DidHitWorld() const;
	bool DidHitNonWorldEntity() const;
	int  GetEntityIndex() const;

	bool DidHit() const
	{
		return (flFraction < 1 || bAllSolid || bStartSolid);
	}

public:
	float			fraction_left_solid;
	csurface_t		surface;
	int				hit_group;
	short			physics_bone;
	C_BaseEntity* entity;
	int				hitbox;

	CGameTrace() {}
	CGameTrace(const CGameTrace& othr);
};
/*
inline bool CGameTrace::DidHit() const
{
	return fraction < 1.0f || bAllSolid || bStartSolid;
}
*/

typedef CGameTrace trace_t;