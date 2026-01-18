#pragma once

namespace MCM
{
	class Settings
	{
	public:
		class General
		{
		public:
			inline static REX::INI::Bool bEnable{ "General", "bEnable", true };
		};

		class AutoHack
		{
		public:
			class Rolls
			{
			public:
				inline static REX::INI::I32 iLockDC0{ "AutoHackRNG", "iLockDC0", 40 };
				inline static REX::INI::I32 iLockDC1{ "AutoHackRNG", "iLockDC1", 60 };
				inline static REX::INI::I32 iLockDC2{ "AutoHackRNG", "iLockDC2", 80 };
				inline static REX::INI::I32 iLockDC3{ "AutoHackRNG", "iLockDC3", 100 };
				inline static REX::INI::I32 iRollMin{ "AutoHackRNG", "iRollMin", 1 };
				inline static REX::INI::I32 iRollMax{ "AutoHackRNG", "iRollMax", 100 };
				inline static REX::INI::I32 iModPerStat{ "AutoHackRNG", "iModPerStat", 3 };
				inline static REX::INI::I32 iModPerLuck{ "AutoHackRNG", "iModPerLuck", 1 };
				inline static REX::INI::I32 iModPerPerk{ "AutoHackRNG", "iModPerPerk", 20 };
				inline static REX::INI::I32 iModBonus{ "AutoHackRNG", "iModBonus", 0 };
				inline static REX::INI::Str sStatName{ "AutoHackRNG", "sStatName", "Intelligence"s };
			};

			class Runtime
			{
			public:
				inline static RE::ActorValueInfo* BakaAutoHackVoice{ nullptr };
				inline static RE::ActorValueInfo* BakaAutoHackAttempts{ nullptr };
				inline static RE::BGSListForm*    Items_Lockpick{ nullptr };
				inline static RE::BGSListForm*    Perks_Mod{ nullptr };
				inline static RE::BGSListForm*    Perks_Unbreakable{ nullptr };
				inline static RE::BGSListForm*    Perks_WaxKey{ nullptr };
			};

			inline static REX::INI::Bool bEnableAutoHack{ "AutoHack", "bEnableAutoHack", true };
			inline static REX::INI::Bool bActivateTERM{ "AutoHack", "bActivateTERM", true };
			inline static REX::INI::Bool bExperienceFromKeys{ "AutoHack", "bExperienceFromKeys", true };
			inline static REX::INI::Bool bForceUnbreakable{ "AutoHack", "bForceUnbreakable", false };
			inline static REX::INI::Bool bForceWaxKey{ "AutoHack", "bForceWaxKey", false };
			inline static REX::INI::Bool bNoCrime{ "AutoHack", "bNoCrime", false };
			inline static REX::INI::Bool bNoKeyUse{ "AutoHack", "bNoKeyUse", false };
			inline static REX::INI::Bool bNoPickGates{ "AutoHack", "bNoPickGates", true };
			inline static REX::INI::Bool bShowRollResults{ "AutoHack", "bShowRollResults", false };
			inline static REX::INI::I32  iDetectionEventSuccess{ "AutoHack", "iDetectionEventSuccess", 0 };
			inline static REX::INI::I32  iDetectionEventFailure{ "AutoHack", "iDetectionEventFailure", 0 };
		};

		class AutoPick
		{
		public:
			class Rolls
			{
			public:
				inline static REX::INI::I32 iLockDC0{ "AutoPickRNG", "iLockDC0", 40 };
				inline static REX::INI::I32 iLockDC1{ "AutoPickRNG", "iLockDC1", 60 };
				inline static REX::INI::I32 iLockDC2{ "AutoPickRNG", "iLockDC2", 80 };
				inline static REX::INI::I32 iLockDC3{ "AutoPickRNG", "iLockDC3", 100 };
				inline static REX::INI::I32 iRollMin{ "AutoPickRNG", "iRollMin", 1 };
				inline static REX::INI::I32 iRollMax{ "AutoPickRNG", "iRollMax", 100 };
				inline static REX::INI::I32 iModPerStat{ "AutoPickRNG", "iModPerStat", 3 };
				inline static REX::INI::I32 iModPerLuck{ "AutoPickRNG", "iModPerLuck", 1 };
				inline static REX::INI::I32 iModPerPerk{ "AutoPickRNG", "iModPerPerk", 20 };
				inline static REX::INI::I32 iModBonus{ "AutoPickRNG", "iModBonus", 0 };
				inline static REX::INI::Str sStatName{ "AutoPickRNG", "sStatName", "Perception"s };
			};

			class Runtime
			{
			public:
				inline static RE::ActorValueInfo* BakaAutoPickVoice{ nullptr };
				inline static RE::ActorValueInfo* LGND_LockPickSweetSpot{ nullptr };
				inline static RE::BGSListForm*    Items_Lockpick{ nullptr };
				inline static RE::BGSListForm*    Perks_Mod{ nullptr };
				inline static RE::BGSListForm*    Perks_Unbreakable{ nullptr };
				inline static RE::BGSListForm*    Perks_WaxKey{ nullptr };
			};

			inline static REX::INI::Bool bEnableAutoPick{ "AutoPick", "bEnableAutoPick", true };
			inline static REX::INI::Bool bActivateCONT{ "AutoPick", "bActivateCONT", false };
			inline static REX::INI::Bool bActivateDOOR{ "AutoPick", "bActivateDOOR", true };
			inline static REX::INI::Bool bExperienceFromKeys{ "AutoPick", "bExperienceFromKeys", true };
			inline static REX::INI::Bool bForceUnbreakable{ "AutoPick", "bForceUnbreakable", false };
			inline static REX::INI::Bool bForceWaxKey{ "AutoPick", "bForceWaxKey", false };
			inline static REX::INI::Bool bNoCrime{ "AutoPick", "bNoCrime", false };
			inline static REX::INI::Bool bNoKeyUse{ "AutoPick", "bNoKeyUse", false };
			inline static REX::INI::Bool bNoPickGates{ "AutoPick", "bNoPickGates", true };
			inline static REX::INI::Bool bShowRollResults{ "AutoPick", "bShowRollResults", false };
			inline static REX::INI::I32  iDetectionEventSuccess{ "AutoPick", "iDetectionEventSuccess", 0 };
			inline static REX::INI::I32  iDetectionEventFailure{ "AutoPick", "iDetectionEventFailure", 0 };
		};

		static void Update()
		{
			PreUpdate();

			const auto ini = REX::INI::SettingStore::GetSingleton();
			ini->Init(
				"Data/MCM/Config/BakaAutoLockpicking/settings.ini",
				"Data/MCM/Settings/BakaAutoLockpicking.ini");
			ini->Load();

			PosUpdate();
		}

	private:
		static void PosUpdate()
		{
			if (auto PlayerCharacter = RE::PlayerCharacter::GetSingleton())
			{
				PlayerCharacter->SetActorValue(*AutoHack::Runtime::BakaAutoHackVoice, 1.0f);
				PlayerCharacter->SetActorValue(*AutoPick::Runtime::BakaAutoPickVoice, 1.0f);

				if (MCM::Settings::General::bEnable)
				{
					if (MCM::Settings::AutoHack::bEnableAutoHack && MCM::Settings::AutoHack::bNoPickGates)
					{
						PlayerCharacter->SetActorValue(*AutoHack::Runtime::BakaAutoHackVoice, 0.0f);
					}

					if (MCM::Settings::AutoPick::bNoPickGates)
					{
						PlayerCharacter->SetActorValue(*AutoHack::Runtime::BakaAutoHackVoice, 0.0f);
					}
				}
			}
		}

		static void PreUpdate()
		{
			if (bRegistered)
			{
				return;
			}

			if (auto UI = RE::UI::GetSingleton())
			{
				UI->RegisterSink<RE::MenuOpenCloseEvent>(EventHandler::GetSingleton());
				bRegistered = true;
			}
		}

		class EventHandler :
			public RE::BSTEventSink<RE::MenuOpenCloseEvent>,
			public REX::Singleton<EventHandler>
		{
		public:
			virtual RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent& a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) override
			{
				if (a_event.menuName == "PauseMenu" && !a_event.opening)
				{
					MCM::Settings::Update();
				}

				return RE::BSEventNotifyControl::kContinue;
			}
		};

		inline static bool bRegistered{ false };
	};
}
