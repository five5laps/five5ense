#pragma once
#include "../../Util/Signatures/Signatures.h"
#include "../Includes/const.h"
#include "C_BaseEntity.h"

#define MAX_SPHERE_QUERY	512

#pragma warning (disable : 26495)

//credits to KGB
class CEntitySphereQuery
{
public:
	CEntitySphereQuery(const Vector& center, const float radius, const int flagMask = 0, const int partitionMask = PARTITION_CLIENT_NON_STATIC_EDICTS)
	{
		static auto dwAddress = S::CEntitySphereQuery();
		reinterpret_cast<void(__thiscall*)(void*, const Vector&, float, int, int)>(dwAddress)(this, center, radius, flagMask, partitionMask);
	}

	C_BaseEntity* GetCurrentEntity()
	{
		return (m_nListIndex < m_nListCount) ? m_pList[m_nListIndex] : nullptr;
	}

	void NextEntity()
	{
		m_nListIndex++;
	}

private:
	int m_nListIndex, m_nListCount;
	C_BaseEntity* m_pList[MAX_SPHERE_QUERY];
};