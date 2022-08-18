#pragma once

namespace Forms
{
	static constexpr char PLUGIN_NAME[] = "BakaAutoLockpicking.esm";

	inline RE::BGSListForm* BakaAutoLock_Perks_Base{ nullptr };
	inline RE::BGSListForm* BakaAutoLock_Perks_Unbreakable{ nullptr };
	inline RE::BGSListForm* BakaAutoLock_Perks_WaxKey{ nullptr };
	inline RE::BGSListForm* BakaAutoLock_Items_Lockpick{ nullptr };

	inline RE::ActorValueInfo* LGND_LockPickSweetSpot{ nullptr };

	inline void Register()
	{
		if (auto TESDataHandler = RE::TESDataHandler::GetSingleton())
		{
			if (TESDataHandler->LookupLoadedLightModByName(PLUGIN_NAME))
			{
				BakaAutoLock_Perks_Base =
					TESDataHandler->LookupForm<RE::BGSListForm>(0x802, PLUGIN_NAME);
				BakaAutoLock_Perks_Unbreakable =
					TESDataHandler->LookupForm<RE::BGSListForm>(0x803, PLUGIN_NAME);
				BakaAutoLock_Perks_WaxKey =
					TESDataHandler->LookupForm<RE::BGSListForm>(0x804, PLUGIN_NAME);
				BakaAutoLock_Items_Lockpick =
					TESDataHandler->LookupForm<RE::BGSListForm>(0x805, PLUGIN_NAME);

				LGND_LockPickSweetSpot =
					TESDataHandler->LookupForm<RE::ActorValueInfo>(0x1F3CAB, "Fallout4.esm");
			}
		}
	}
}
