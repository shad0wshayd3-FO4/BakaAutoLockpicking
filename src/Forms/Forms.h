#pragma once

namespace Forms
{
	inline RE::BGSListForm*    BakaAutoLock_Perks_Base{ nullptr };
	inline RE::BGSListForm*    BakaAutoLock_Perks_Unbreakable{ nullptr };
	inline RE::BGSListForm*    BakaAutoLock_Perks_WaxKey{ nullptr };
	inline RE::BGSListForm*    BakaAutoLock_Items_Lockpick{ nullptr };
	inline RE::BGSListForm*    BakaAutoHack_Perks_Base{ nullptr };
	inline RE::BGSListForm*    BakaAutoHack_Perks_WaxKey{ nullptr };
	inline RE::ActorValueInfo* BakaAutoHack_Attempts{ nullptr };
	inline RE::ActorValueInfo* LGND_LockPickSweetSpot{ nullptr };

	static void Register()
	{
		if (auto TESDataHandler = RE::TESDataHandler::GetSingleton())
		{
			BakaAutoLock_Perks_Base =
				TESDataHandler->LookupForm<RE::BGSListForm>(0x802, "BakaAutoLockpicking.esm"sv);
			BakaAutoLock_Perks_Unbreakable =
				TESDataHandler->LookupForm<RE::BGSListForm>(0x803, "BakaAutoLockpicking.esm"sv);
			BakaAutoLock_Perks_WaxKey =
				TESDataHandler->LookupForm<RE::BGSListForm>(0x804, "BakaAutoLockpicking.esm"sv);
			BakaAutoLock_Items_Lockpick =
				TESDataHandler->LookupForm<RE::BGSListForm>(0x805, "BakaAutoLockpicking.esm"sv);
			BakaAutoHack_Perks_Base =
				TESDataHandler->LookupForm<RE::BGSListForm>(0x806, "BakaAutoLockpicking.esm"sv);
			BakaAutoHack_Perks_WaxKey =
				TESDataHandler->LookupForm<RE::BGSListForm>(0x807, "BakaAutoLockpicking.esm"sv);
			BakaAutoHack_Attempts =
				TESDataHandler->LookupForm<RE::ActorValueInfo>(0x900, "BakaAutoLockpicking.esm"sv);
			LGND_LockPickSweetSpot =
				TESDataHandler->LookupForm<RE::ActorValueInfo>(0x1F3CAB, "Fallout4.esm"sv);
		}
	}
}
