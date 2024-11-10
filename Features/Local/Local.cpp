#include "Local.hpp"
#include "../../Common.hpp"
#include "../../GTA/Invoker/Natives.hpp"
#include "../../GTA/Script/ScriptGlobal.hpp"
#include "../../GTA/Script/ScriptManager.hpp"
#include "../../Utility/GTA.hpp"
#include "../../Utility/Util.hpp"
#include "../../Utility/Math.hpp"

// Features using g_LastFeatureName for example, restore only once to improve performance

namespace Chim
{
	void LocalFeatures::SuperMan(bool enable)
	{
		if (enable)
		{
			// Disable Controls
			PAD::DISABLE_CONTROL_ACTION(0, 21, TRUE); // Sprint
			PAD::DISABLE_CONTROL_ACTION(0, 37, TRUE); // Duck

			if (!PED::IS_PED_IN_PARACHUTE_FREE_FALL(PLAYER::PLAYER_PED_ID()))
			{
				if (ENTITY::GET_ENTITY_HEIGHT_ABOVE_GROUND(PLAYER::PLAYER_PED_ID()) > 4.0f)
					TASK::TASK_SKY_DIVE(PLAYER::PLAYER_PED_ID(), TRUE);
			}

			if (ENTITY::IS_ENTITY_IN_AIR(PLAYER::PLAYER_PED_ID()) && !PED::IS_PED_RAGDOLL(PLAYER::PLAYER_PED_ID()))
			{
				if (PAD::IS_DISABLED_CONTROL_PRESSED(0, 21))
				{
					ENTITY::APPLY_FORCE_TO_ENTITY(PLAYER::PLAYER_PED_ID(), 1, 0.f, 100.0f, 100.0f, 0.f, 0.f, 0.f, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE);
				}
				if (PAD::IS_DISABLED_CONTROL_PRESSED(0, 37))
				{
					ENTITY::APPLY_FORCE_TO_ENTITY(PLAYER::PLAYER_PED_ID(), 1, 0.f, -100.f, -100.0f, 0.f, 0.f, 0.f, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE);
				}
			}
		}
	}

	static int g_Flip;
	int g_Speed = 6;
	void LocalFeatures::NinjaJump(bool enable)
	{
		if (enable)
		{
			if (PED::IS_PED_JUMPING(PLAYER::PLAYER_PED_ID()))
			{
				if (ENTITY::IS_ENTITY_IN_AIR(PLAYER::PLAYER_PED_ID()))
				{
					g_Flip -= g_Speed;
					if (g_Flip >= -(360 * 2))
						ENTITY::SET_ENTITY_ROTATION(PLAYER::PLAYER_PED_ID(), g_Flip, 0, ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()), 0, 0);
				}
				else
					g_Flip = 0;
			}
		}
	}

	bool g_LastFastRun = false;
	void LocalFeatures::FastRun(bool enable)
	{
		if (enable)
			GTAUtility::GetLocalPlayerInfo()->m_run_speed = 2.5f;
		else if (enable != g_LastFastRun)
			GTAUtility::GetLocalPlayerInfo()->m_run_speed = 1.f;

		g_LastFastRun = enable;
	}

	bool g_LastFastSwim = false;
	void LocalFeatures::FastSwim(bool enable)
	{
		if (enable)
			GTAUtility::GetLocalPlayerInfo()->m_swim_speed = 2.5f;
		else if (enable != g_LastFastSwim)
			GTAUtility::GetLocalPlayerInfo()->m_swim_speed = 1.f;

		g_LastFastSwim = enable;
	}

	bool g_DrugMode = false;
	void LocalFeatures::DrugMode(bool enable) // on drugs <3
	{
		if (enable)
		{
			GRAPHICS::ENABLE_ALIEN_BLOOD_VFX(true);
			GRAPHICS::ANIMPOSTFX_PLAY("DrugsMichaelAliensFight", 9999999, false);
		}
		else if (enable != g_DrugMode)
		{
			GRAPHICS::ENABLE_ALIEN_BLOOD_VFX(false);
			GRAPHICS::ANIMPOSTFX_STOP("DrugsMichaelAliensFight");
		}

		g_DrugMode = enable;
	}

	bool g_ignore = false;
	void LocalFeatures::Ignore(bool enable) // on drugs <3
	{
		if (enable)
		{
			const int ElementAmount = 10;
			const int ArrSize = ElementAmount * 2 + 2;
			Ped* peds = new Ped[ArrSize];
			peds[0] = ElementAmount;
			int PedFound = PED::GET_PED_NEARBY_PEDS(PLAYER::PLAYER_ID(), peds, -1);
			for (int i = 0; i < PedFound; i++)
			{
				int OffsetID = i * 2 + 2;
				Ped ped = peds[OffsetID];
				NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(ped);
				PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ped, true);
				PED::SET_PED_FLEE_ATTRIBUTES(ped, 0, false);
				PED::SET_PED_COMBAT_ATTRIBUTES(ped, 17, true);
			}
			PLAYER::SET_POLICE_IGNORE_PLAYER(PLAYER::PLAYER_ID(), true);
			PLAYER::SET_EVERYONE_IGNORE_PLAYER(PLAYER::PLAYER_ID(), true);
			PLAYER::SET_PLAYER_CAN_BE_HASSLED_BY_GANGS(PLAYER::PLAYER_ID(), false);
			PLAYER::SET_IGNORE_LOW_PRIORITY_SHOCKING_EVENTS(PLAYER::PLAYER_ID(), true);
		}
		else if (enable != g_ignore)
		{
			PLAYER::SET_POLICE_IGNORE_PLAYER(PLAYER::PLAYER_ID(), false);
			PLAYER::SET_EVERYONE_IGNORE_PLAYER(PLAYER::PLAYER_ID(), false);
			PLAYER::SET_PLAYER_CAN_BE_HASSLED_BY_GANGS(PLAYER::PLAYER_ID(), true);
			PLAYER::SET_IGNORE_LOW_PRIORITY_SHOCKING_EVENTS(PLAYER::PLAYER_ID(), false);
		}

		g_ignore = enable;
	}

	bool g_tiny = false;
	void LocalFeatures::Tiny(bool enable) // on drugs <3
	{
		if (enable)
		{
			PED::SET_PED_CONFIG_FLAG(PLAYER::PLAYER_PED_ID(), 223, true);
		}
		else if (enable != g_tiny)
		{
			PED::SET_PED_CONFIG_FLAG(PLAYER::PLAYER_PED_ID(), 223, false);
		}

		g_tiny = enable;
	}

	bool MONEYDROP = false;
	void LocalFeatures::MONEYDROP(bool enable) // money, idk if it's detected but oh welp
	{
		if (enable)
		{
			{
				Ped iPed = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer);
				STREAMING::REQUEST_MODEL(MISC::GET_HASH_KEY("bkr_prop_moneypack_03a"));
				if (!STREAMING::HAS_MODEL_LOADED(MISC::GET_HASH_KEY("bkr_prop_moneypack_03a")))
				{
					/* WAIT(0);*/
				}
				else
				{
					NativeVector3 playerPostion = ENTITY::GET_ENTITY_COORDS(iPed, FALSE);
					OBJECT::CREATE_AMBIENT_PICKUP(0xDE78F17E, playerPostion.x, playerPostion.y, playerPostion.z + 10.f / 10, 10, 2500, MISC::GET_HASH_KEY("bkr_prop_moneypack_03a"), FALSE, TRUE);
					STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(MISC::GET_HASH_KEY("bkr_prop_moneypack_03a"));
				}
			}
		}
	}

	Hash $(std::string str) {
		return MISC::GET_HASH_KEY(&str[0u]);
	}
	bool RPDROP = false;
	void LocalFeatures::RPDROP(bool enable) // money, idk if it's detected but oh welp
	{
		if (enable)
		{
			{
				Ped iPed = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer);
				STREAMING::REQUEST_MODEL(MISC::GET_HASH_KEY("vw_prop_vw_colle_prbubble"));
				if (!STREAMING::HAS_MODEL_LOADED(MISC::GET_HASH_KEY("vw_prop_vw_colle_prbubble")))
				{
					/* WAIT(0);*/
				}
				else
				{
					NativeVector3 playerPostion = ENTITY::GET_ENTITY_COORDS(iPed, FALSE);
					OBJECT::CREATE_AMBIENT_PICKUP(0x2C014CA6, playerPostion.x, playerPostion.y, playerPostion.z + 5, 0, 10, $("vw_prop_vw_colle_prbubble"), FALSE, TRUE);
					STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(MISC::GET_HASH_KEY("vw_prop_vw_colle_prbubble"));
				}
			}
		}
	}

	bool g_LastNeverWanted = false;
	void LocalFeatures::NeverWanted(bool enable)
	{
		if (enable)
		{
			GTAUtility::GetLocalPlayerInfo()->m_wanted_level = 0;
			GTAUtility::GetLocalPlayerInfo()->m_wanted_level_display = 0;
			GTAUtility::GetLocalPlayerInfo()->m_is_wanted = false;
			g_GameVariables->m_MaxWantedLevel->Apply();
			g_GameVariables->m_MaxWantedLevel2->Apply();
		}
		else if (enable != g_LastNeverWanted)
		{
			g_GameVariables->m_MaxWantedLevel->Restore();
			g_GameVariables->m_MaxWantedLevel2->Restore();
		}

		g_LastNeverWanted = enable;
	}

	bool g_LastGodMode = false;
	void LocalFeatures::GodMode(bool enable)
	{
		if (enable)
			GTAUtility::Offsets::SetValue<bool>({ 0x8, 0x189 }, true);
		else if (enable != g_LastGodMode)
			GTAUtility::Offsets::SetValue<bool>({ 0x8, 0x189 }, false);

		g_LastGodMode = enable;
	}
}