#include "ESP.h"
#include "../../Config.h"

void CFeatures_ESP::Render()
{
	if (!Config::Visuals::ESP::Players::enemies)
		return;

	if (g_Globals.m_bIsGameUIVisible || !g_Globals.m_bIsInGame)
		return;

	int x, y, w, h;
	for (int n = 1; n < (g_Globals.m_nMaxEntities + 1); n++)
	{
		if (n == g_Globals.m_nLocalIndex)
			continue;

		IClientEntity* pEntity = Util::EntityByIndex(n);

		if (!pEntity || pEntity->IsDormant())
			continue;

		ClientClass* pCC = pEntity->GetClientClass();

		if (!pCC)
			continue;

		switch (pCC->GetTFClientClass())
		{
		case ETFClientClass::CTFPlayer:
		{

			C_TFPlayer* pPlayer = dynamic_cast<C_TFPlayer*>(pEntity);

			if (pPlayer->deadflag() || pPlayer->InLocalTeam())
				break;

			if (!GetDynamicBounds(pPlayer, x, y, w, h))
				break;

			const int nHealth = pPlayer->GetHealth();
			const int nMaxHealth = pPlayer->GetMaxHealth();

			const Color clrHealth = Util::GetHealthColor(nHealth, nMaxHealth);

			player_info_t pi;
			if (Config::Visuals::ESP::Players::name && I::EngineClient->GetPlayerInfo(n, &pi))
			{
				H::Draw.String(EFonts::ESP_NAME,
					x + (w / 2),
					y - H::Draw.GetFontHeight(EFonts::ESP_NAME),
					{ 255, 255, 255, 255 },
					TXT_CENTERX,
					Util::ConvertUtf8ToWide(pi.name).c_str());
			}

			if (Config::Visuals::ESP::Players::gameClass)
			{
				const int nOffset = Config::Visuals::ESP::Players::name ? H::Draw.GetFontHeight(EFonts::ESP_NAME) : 0;
				H::Draw.Texture(Util::GetClassIcon(pPlayer->m_iClass()), x + (w / 2), (y - nOffset) - 18, 18, 18, TEX_CENTERX);
			}

			if (Config::Visuals::ESP::Players::healthBar)
			{
				x -= 1;

				const float flMaxHealth = static_cast<float>(nMaxHealth);
				const float flHealth = U::Math.Clamp<float>(static_cast<float>(nHealth), 1.0f, flMaxHealth);

				static const int nWidth = 2;
				const int nHeight = (h + (flHealth < flMaxHealth ? 2 : 1));
				const int nHeight2 = (h + 1);

				const float ratio = (flHealth / flMaxHealth);
				H::Draw.Rect(static_cast<int>(((x - nWidth) - 2)), static_cast<int>((y + nHeight - (nHeight * ratio))), nWidth, static_cast<int>((nHeight * ratio)), clrHealth);
				H::Draw.OutlinedRect(static_cast<int>(((x - nWidth) - 2) - 1), static_cast<int>((y + nHeight - (nHeight * ratio)) - 1), nWidth + 2, static_cast<int>((nHeight * ratio) + 1), COLOR_BLACK);

				x += 1;
			}

			break;
		}
		case ETFClientClass::CObjectSentrygun:
		case ETFClientClass::CObjectDispenser:
		case ETFClientClass::CObjectTeleporter:
		{
			if (!Config::Visuals::ESP::Buildings::buildings)
				break;

			C_BaseEntity* pBuilding = pEntity->As<C_BaseEntity>();

			if (!pBuilding->IsAlive())
				break;

			if (!GetDynamicBounds(pBuilding, x, y, w, h))
				break;

			const int nHealth = pBuilding->GetHealth();
			const int nMaxHealth = pBuilding->GetMaxHealth();
			const int nTeamNumber = pBuilding->GetTeamNumber();

			const Color clrTeam = Util::GetTeamColor(nTeamNumber);
			const Color clrHealth = Util::GetHealthColor(nHealth, nMaxHealth);

			if (Config::Visuals::ESP::Buildings::buildingType)
			{
				auto GetBuildingType = [&]()
					{
						switch (pCC->GetTFClientClass())
						{
						case ETFClientClass::CObjectSentrygun: return "Sentrygun";
						case ETFClientClass::CObjectDispenser: return "Dispenser";
						case ETFClientClass::CObjectTeleporter: return "Teleporter";
						default: return "";
						}
					};

				H::Draw.String(EFonts::ESP_NAME,
					x + (w / 2),
					y - H::Draw.GetFontHeight(EFonts::ESP_NAME),
					clrTeam,
					TXT_CENTERX,
					GetBuildingType());
			}

			if (Config::Visuals::ESP::Buildings::healthBar)
			{
				x -= 1;

				const float flMaxHealth = static_cast<float>(nMaxHealth);
				const float flHealth = U::Math.Clamp<float>(static_cast<float>(nHealth), 1.0f, flMaxHealth);

				static const int nWidth = 2;
				const int nHeight = (h + (flHealth < flMaxHealth ? 2 : 1));
				const int nHeight2 = (h + 1);

				const float ratio = (flHealth / flMaxHealth);
				H::Draw.Rect(static_cast<int>(((x - nWidth) - 2)), static_cast<int>((y + nHeight - (nHeight * ratio))), nWidth, static_cast<int>((nHeight * ratio)), clrHealth);
				H::Draw.OutlinedRect(static_cast<int>(((x - nWidth) - 2) - 1), static_cast<int>((y + nHeight - (nHeight * ratio)) - 1), nWidth + 2, static_cast<int>((nHeight * ratio) + 1), COLOR_BLACK);

				x += 1;
			}

			break;
		}
		default: break;
		}
	}
}

bool CFeatures_ESP::GetDynamicBounds(C_BaseEntity* pEntity, int& x, int& y, int& w, int& h)
{
	Vector vPoints[8];
	U::Math.BuildTransformedBox(vPoints, pEntity->m_vecMins(), pEntity->m_vecMaxs(), pEntity->RenderableToWorldTransform());

	Vector2D flb, brt, blb, frt, frb, brb, blt, flt;

	if (H::Draw.WorldPosToScreenPos(vPoints[3], flb) && H::Draw.WorldPosToScreenPos(vPoints[5], brt)
		&& H::Draw.WorldPosToScreenPos(vPoints[0], blb) && H::Draw.WorldPosToScreenPos(vPoints[4], frt)
		&& H::Draw.WorldPosToScreenPos(vPoints[2], frb) && H::Draw.WorldPosToScreenPos(vPoints[1], brb)
		&& H::Draw.WorldPosToScreenPos(vPoints[6], blt) && H::Draw.WorldPosToScreenPos(vPoints[7], flt)
		&& H::Draw.WorldPosToScreenPos(vPoints[6], blt) && H::Draw.WorldPosToScreenPos(vPoints[7], flt))
	{
		const Vector2D arr[8] = { flb, brt, blb, frt, frb, brb, blt, flt };

		float left = flb.x;
		float top = flb.y;
		float righ = flb.x;
		float bottom = flb.y;

		for (int n = 1; n < 8; n++)
		{
			if (left > arr[n].x)
				left = arr[n].x;

			if (top < arr[n].y)
				top = arr[n].y;

			if (righ < arr[n].x)
				righ = arr[n].x;

			if (bottom > arr[n].y)
				bottom = arr[n].y;
		}

		x = static_cast<int>(left);
		y = static_cast<int>(bottom);
		w = static_cast<int>((righ - left));
		h = static_cast<int>((top - bottom));

		return !(x > H::Draw.m_nScreenW || (x + w) < 0 || y > H::Draw.m_nScreenH || (y + h) < 0);
	}

	return false;
}
