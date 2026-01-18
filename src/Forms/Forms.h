#pragma once

#include "MCM/MCM.h"

namespace Forms
{
	static void Load()
	{
		if (auto TESDataHandler = RE::TESDataHandler::GetSingleton())
		{
			MCM::Settings::AutoHack::Runtime::BakaAutoHackVoice =
				TESDataHandler->LookupForm<RE::ActorValueInfo>(0x900, "BakaAutoLockpicking.esm"sv);
			MCM::Settings::AutoHack::Runtime::BakaAutoHackAttempts =
				TESDataHandler->LookupForm<RE::ActorValueInfo>(0xA00, "BakaAutoLockpicking.esm"sv);
			MCM::Settings::AutoHack::Runtime::Items_Lockpick =
				TESDataHandler->LookupForm<RE::BGSListForm>(0x901, "BakaAutoLockpicking.esm"sv);
			MCM::Settings::AutoHack::Runtime::Perks_Mod =
				TESDataHandler->LookupForm<RE::BGSListForm>(0x902, "BakaAutoLockpicking.esm"sv);
			MCM::Settings::AutoHack::Runtime::Perks_Unbreakable =
				TESDataHandler->LookupForm<RE::BGSListForm>(0x903, "BakaAutoLockpicking.esm"sv);
			MCM::Settings::AutoHack::Runtime::Perks_WaxKey =
				TESDataHandler->LookupForm<RE::BGSListForm>(0x904, "BakaAutoLockpicking.esm"sv);

			MCM::Settings::AutoPick::Runtime::BakaAutoPickVoice =
				TESDataHandler->LookupForm<RE::ActorValueInfo>(0x800, "BakaAutoLockpicking.esm"sv);
			MCM::Settings::AutoPick::Runtime::LGND_LockPickSweetSpot =
				TESDataHandler->LookupForm<RE::ActorValueInfo>(0x001F3CAB, "Fallout4.esm"sv);
			MCM::Settings::AutoPick::Runtime::Items_Lockpick =
				TESDataHandler->LookupForm<RE::BGSListForm>(0x801, "BakaAutoLockpicking.esm"sv);
			MCM::Settings::AutoPick::Runtime::Perks_Mod =
				TESDataHandler->LookupForm<RE::BGSListForm>(0x802, "BakaAutoLockpicking.esm"sv);
			MCM::Settings::AutoPick::Runtime::Perks_Unbreakable =
				TESDataHandler->LookupForm<RE::BGSListForm>(0x803, "BakaAutoLockpicking.esm"sv);
			MCM::Settings::AutoPick::Runtime::Perks_WaxKey =
				TESDataHandler->LookupForm<RE::BGSListForm>(0x804, "BakaAutoLockpicking.esm"sv);
		}

		MCM::Settings::Update();
	}
}
