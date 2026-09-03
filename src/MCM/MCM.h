#pragma once

namespace MCM
{
	class Settings
	{
	public:
		class General
		{
		public:
			inline static REX::INI::Bool bEnable{ "General"sv, "bEnable"sv, true };
		};

		class AutoHack
		{
		public:
			class Rolls
			{
			public:
				inline static REX::INI::I32 iLockDC0{ "AutoHackRNG"sv, "iLockDC0"sv, 40 };
				inline static REX::INI::I32 iLockDC1{ "AutoHackRNG"sv, "iLockDC1"sv, 60 };
				inline static REX::INI::I32 iLockDC2{ "AutoHackRNG"sv, "iLockDC2"sv, 80 };
				inline static REX::INI::I32 iLockDC3{ "AutoHackRNG"sv, "iLockDC3"sv, 100 };
				inline static REX::INI::I32 iRollMin{ "AutoHackRNG"sv, "iRollMin"sv, 1 };
				inline static REX::INI::I32 iRollMax{ "AutoHackRNG"sv, "iRollMax"sv, 100 };
				inline static REX::INI::I32 iModPerStat{ "AutoHackRNG"sv, "iModPerStat"sv, 3 };
				inline static REX::INI::I32 iModPerLuck{ "AutoHackRNG"sv, "iModPerLuck"sv, 1 };
				inline static REX::INI::I32 iModPerPerk{ "AutoHackRNG"sv, "iModPerPerk"sv, 20 };
				inline static REX::INI::I32 iModBonus{ "AutoHackRNG"sv, "iModBonus"sv, 0 };
				inline static REX::INI::Str sStatName{ "AutoHackRNG"sv, "sStatName"sv, "Intelligence"s };
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

			inline static REX::INI::Bool bEnableAutoHack{ "AutoHack"sv, "bEnableAutoHack"sv, true };
			inline static REX::INI::Bool bActivateTERM{ "AutoHack"sv, "bActivateTERM"sv, true };
			inline static REX::INI::Bool bExperienceFromKeys{ "AutoHack"sv, "bExperienceFromKeys"sv, true };
			inline static REX::INI::Bool bForceUnbreakable{ "AutoHack"sv, "bForceUnbreakable"sv, false };
			inline static REX::INI::Bool bForceWaxKey{ "AutoHack"sv, "bForceWaxKey"sv, false };
			inline static REX::INI::Bool bNoCrime{ "AutoHack"sv, "bNoCrime"sv, false };
			inline static REX::INI::Bool bNoKeyUse{ "AutoHack"sv, "bNoKeyUse"sv, false };
			inline static REX::INI::Bool bNoPickGates{ "AutoHack"sv, "bNoPickGates"sv, true };
			inline static REX::INI::Bool bShowRollResults{ "AutoHack"sv, "bShowRollResults"sv, false };
			inline static REX::INI::I32  iDetectionEventSuccess{ "AutoHack"sv, "iDetectionEventSuccess"sv, 0 };
			inline static REX::INI::I32  iDetectionEventFailure{ "AutoHack"sv, "iDetectionEventFailure"sv, 0 };
		};

		class AutoPick
		{
		public:
			class Rolls
			{
			public:
				inline static REX::INI::I32 iLockDC0{ "AutoPickRNG"sv, "iLockDC0"sv, 40 };
				inline static REX::INI::I32 iLockDC1{ "AutoPickRNG"sv, "iLockDC1"sv, 60 };
				inline static REX::INI::I32 iLockDC2{ "AutoPickRNG"sv, "iLockDC2"sv, 80 };
				inline static REX::INI::I32 iLockDC3{ "AutoPickRNG"sv, "iLockDC3"sv, 100 };
				inline static REX::INI::I32 iRollMin{ "AutoPickRNG"sv, "iRollMin"sv, 1 };
				inline static REX::INI::I32 iRollMax{ "AutoPickRNG"sv, "iRollMax"sv, 100 };
				inline static REX::INI::I32 iModPerStat{ "AutoPickRNG"sv, "iModPerStat"sv, 3 };
				inline static REX::INI::I32 iModPerLuck{ "AutoPickRNG"sv, "iModPerLuck"sv, 1 };
				inline static REX::INI::I32 iModPerPerk{ "AutoPickRNG"sv, "iModPerPerk"sv, 20 };
				inline static REX::INI::I32 iModBonus{ "AutoPickRNG"sv, "iModBonus"sv, 0 };
				inline static REX::INI::Str sStatName{ "AutoPickRNG"sv, "sStatName"sv, "Perception"s };
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

			inline static REX::INI::Bool bEnableAutoPick{ "AutoPick"sv, "bEnableAutoPick"sv, true };
			inline static REX::INI::Bool bActivateCONT{ "AutoPick"sv, "bActivateCONT"sv, false };
			inline static REX::INI::Bool bActivateDOOR{ "AutoPick"sv, "bActivateDOOR"sv, true };
			inline static REX::INI::Bool bExperienceFromKeys{ "AutoPick"sv, "bExperienceFromKeys"sv, true };
			inline static REX::INI::Bool bForceUnbreakable{ "AutoPick"sv, "bForceUnbreakable"sv, false };
			inline static REX::INI::Bool bForceWaxKey{ "AutoPick"sv, "bForceWaxKey"sv, false };
			inline static REX::INI::Bool bNoCrime{ "AutoPick"sv, "bNoCrime"sv, false };
			inline static REX::INI::Bool bNoKeyUse{ "AutoPick"sv, "bNoKeyUse"sv, false };
			inline static REX::INI::Bool bNoPickGates{ "AutoPick"sv, "bNoPickGates"sv, true };
			inline static REX::INI::Bool bShowRollResults{ "AutoPick"sv, "bShowRollResults"sv, false };
			inline static REX::INI::I32  iDetectionEventSuccess{ "AutoPick"sv, "iDetectionEventSuccess"sv, 0 };
			inline static REX::INI::I32  iDetectionEventFailure{ "AutoPick"sv, "iDetectionEventFailure"sv, 0 };
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
			public REX::TSingleton<EventHandler>
		{
		public:
			virtual RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent& a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) override
			{
				if (!a_event.opening && a_event.menuName == "PauseMenu"sv)
				{
					MCM::Settings::Update();
				}

				return RE::BSEventNotifyControl::kContinue;
			}
		};

		inline static bool bRegistered{ false };
	};
}
