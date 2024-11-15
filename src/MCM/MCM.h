#pragma once

namespace MCM::Settings
{
	namespace LockGeneral
	{
		static REX::INI::Bool bActivateContAfterPick{ "LockGeneral", "bActivateContAfterPick", false };
		static REX::INI::Bool bActivateDoorAfterPick{ "LockGeneral", "bActivateDoorAfterPick", true };
		static REX::INI::Bool bGiveWaxKeys{ "LockGeneral", "bGiveWaxKeys", false };
		static REX::INI::Bool bIgnoreHasKey{ "LockGeneral", "bIgnoreHasKey", false };
		static REX::INI::Bool bIgnoreLockGates{ "LockGeneral", "bIgnoreLockGates", false };
		static REX::INI::Bool bLockpickingCrimeCheck{ "LockGeneral", "bLockpickingCrimeCheck", true };
		static REX::INI::Bool bModEnabled{ "LockGeneral", "bModEnabled", true };
		static REX::INI::Bool bShowRollResults{ "LockGeneral", "bShowRollResults", false };
		static REX::INI::Bool bUnbreakableLockpicks{ "LockGeneral", "bUnbreakableLockpicks", false };

		static REX::INI::I32 iDetectionEventFailureLevel{ "LockGeneral", "iDetectionEventFailureLevel", 0 };
		static REX::INI::I32 iDetectionEventSuccessLevel{ "LockGeneral", "iDetectionEventSuccessLevel", 0 };
		static REX::INI::I32 iSkillIndex{ "LockGeneral", "iSkillIndex", 1 };
		static REX::INI::Str sSkillName{ "LockGeneral", "sSkillName", "Perception"s };
	}

	namespace LockRolls
	{
		static REX::INI::I32 iDCNovice{ "LockRolls", "iDCNovice", 8 };
		static REX::INI::I32 iDCAdvanced{ "LockRolls", "iDCAdvanced", 12 };
		static REX::INI::I32 iDCExpert{ "LockRolls", "iDCExpert", 16 };
		static REX::INI::I32 iDCMaster{ "LockRolls", "iDCMaster", 20 };
		static REX::INI::I32 iPlayerDiceMin{ "LockRolls", "iPlayerDiceMin", 1 };
		static REX::INI::I32 iPlayerDiceMax{ "LockRolls", "iPlayerDiceMax", 20 };
		static REX::INI::I32 iBonusPerBonus{ "LockRolls", "iBonusPerBonus", 0 };
		static REX::INI::I32 iBonusPerLucky{ "LockRolls", "iBonusPerLucky", 5 };
		static REX::INI::I32 iBonusPerPerks{ "LockRolls", "iBonusPerPerks", 1 };
		static REX::INI::I32 iBonusPerSkill{ "LockRolls", "iBonusPerSkill", 2 };
	}

	namespace HackGeneral
	{
		static REX::INI::Bool bActivateTermAfterHack{ "HackGeneral", "bActivateTermAfterHack", true };
		static REX::INI::Bool bGivePass{ "HackGeneral", "bGivePass", false };
		static REX::INI::Bool bIgnoreHasPass{ "HackGeneral", "bIgnoreHasPass", false };
		static REX::INI::Bool bIgnoreHackGates{ "HackGeneral", "bIgnoreHackGates", false };
		static REX::INI::Bool bHackingCrimeCheck{ "HackGeneral", "bHackingCrimeCheck", true };
		static REX::INI::Bool bModEnabled{ "HackGeneral", "bModEnabled", true };
		static REX::INI::Bool bShowRollResults{ "HackGeneral", "bShowRollResults", false };
		static REX::INI::Bool bNoTimeouts{ "HackGeneral", "bNoTimeouts", false };

		static REX::INI::I32 iDetectionEventFailureLevel{ "HackGeneral", "iDetectionEventFailureLevel", 0 };
		static REX::INI::I32 iDetectionEventSuccessLevel{ "HackGeneral", "iDetectionEventSuccessLevel", 0 };
		static REX::INI::I32 iSkillIndex{ "HackGeneral", "iSkillIndex", 4 };
		static REX::INI::Str sSkillName{ "HackGeneral", "sSkillName", "Intelligence"s };
	}

	namespace HackRolls
	{
		static REX::INI::I32 iDCNovice{ "HackRolls", "iDCNovice", 8 };
		static REX::INI::I32 iDCAdvanced{ "HackRolls", "iDCAdvanced", 12 };
		static REX::INI::I32 iDCExpert{ "HackRolls", "iDCExpert", 16 };
		static REX::INI::I32 iDCMaster{ "HackRolls", "iDCMaster", 20 };
		static REX::INI::I32 iPlayerDiceMin{ "HackRolls", "iPlayerDiceMin", 1 };
		static REX::INI::I32 iPlayerDiceMax{ "HackRolls", "iPlayerDiceMax", 20 };
		static REX::INI::I32 iBonusPerBonus{ "HackRolls", "iBonusPerBonus", 0 };
		static REX::INI::I32 iBonusPerLucky{ "HackRolls", "iBonusPerLucky", 5 };
		static REX::INI::I32 iBonusPerPerks{ "HackRolls", "iBonusPerPerks", 1 };
		static REX::INI::I32 iBonusPerSkill{ "HackRolls", "iBonusPerSkill", 2 };
	}

	namespace Formatting
	{
		inline static std::string sBonus;
		inline static std::string sLucky;
		inline static std::string sPerks;
		inline static std::string sSkill;
		inline static std::string sTotal;
		inline static std::string sShowRollResults;

		static void Load()
		{
			auto BSScaleformManager = RE::BSScaleformManager::GetSingleton();
			if (BSScaleformManager && BSScaleformManager->loader)
			{
				if (auto BSScaleformTranslator = static_cast<RE::BSScaleformTranslator*>(BSScaleformManager->loader->GetStateAddRef(RE::Scaleform::GFx::State::StateType::kTranslator)))
				{
					auto FetchTranslation = [](RE::BSScaleformTranslator* a_trns, const wchar_t* a_key, std::string& a_output)
					{
						auto it = a_trns->translator.translationMap.find(a_key);
						if (it != a_trns->translator.translationMap.end())
						{
							a_output.resize(512);
							sprintf_s(a_output.data(), 512, "%ws", it->second.data());
						}
					};

					FetchTranslation(BSScaleformTranslator, L"$BakaAL_Message_Bonus", sBonus);
					FetchTranslation(BSScaleformTranslator, L"$BakaAL_Message_Lucky", sLucky);
					FetchTranslation(BSScaleformTranslator, L"$BakaAL_Message_Perks", sPerks);
					FetchTranslation(BSScaleformTranslator, L"$BakaAL_Message_Skill", sSkill);
					FetchTranslation(BSScaleformTranslator, L"$BakaAL_Message_Total", sTotal);
					FetchTranslation(BSScaleformTranslator, L"$BakaAL_Message_ShowRollResults", sShowRollResults);
				}
			}
		}
	}

	static void Load(bool a_firstRun)
	{
		const auto ini = REX::INI::SettingStore::GetSingleton();
		if (a_firstRun)
		{
			ini->Init(
				"Data/F4SE/plugins/BakaAutoLockpicking.ini",
				"Data/F4SE/plugins/BakaAutoLockpickingCustom.ini");
			Formatting::Load();
		}

		ini->Load();
	}
}
