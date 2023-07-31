#pragma once

namespace MCM
{
	class Settings
	{
	public:
		class LockGeneral
		{
		public:
			inline static bool bActivateContAfterPick{ false };
			inline static bool bActivateDoorAfterPick{ true };
			inline static bool bGiveWaxKeys{ false };
			inline static bool bIgnoreHasKey{ false };
			inline static bool bIgnoreLockGates{ false };
			inline static bool bLockpickingCrimeCheck{ true };
			inline static bool bModEnabled{ true };
			inline static bool bShowRollResults{ false };
			inline static bool bUnbreakableLockpicks{ false };

			inline static std::int32_t iDetectionEventFailureLevel{ 0 };
			inline static std::int32_t iDetectionEventSuccessLevel{ 0 };
			inline static std::int32_t iSkillIndex{ 1 };
			inline static std::string_view sSkillName{ "Perception" };
		};

		class LockRolls
		{
		public:
			inline static std::int32_t iDCNovice{ 8 };
			inline static std::int32_t iDCAdvanced{ 12 };
			inline static std::int32_t iDCExpert{ 16 };
			inline static std::int32_t iDCMaster{ 20 };
			inline static std::int32_t iPlayerDiceMin{ 1 };
			inline static std::int32_t iPlayerDiceMax{ 20 };
			inline static std::int32_t iBonusPerBonus{ 0 };
			inline static std::int32_t iBonusPerLucky{ 5 };
			inline static std::int32_t iBonusPerPerks{ 1 };
			inline static std::int32_t iBonusPerSkill{ 2 };
		};

		class HackGeneral
		{
		public:
			inline static bool bActivateTermAfterHack{ true };
			inline static bool bGivePass{ false };
			inline static bool bIgnoreHasPass{ false };
			inline static bool bIgnoreHackGates{ false };
			inline static bool bHackingCrimeCheck{ true };
			inline static bool bModEnabled{ true };
			inline static bool bShowRollResults{ false };
			inline static bool bNoTimeouts{ false };

			inline static std::int32_t iDetectionEventFailureLevel{ 0 };
			inline static std::int32_t iDetectionEventSuccessLevel{ 0 };
			inline static std::int32_t iSkillIndex{ 4 };
			inline static std::string_view sSkillName{ "Intelligence" };
		};

		class HackRolls
		{
		public:
			inline static std::int32_t iDCNovice{ 8 };
			inline static std::int32_t iDCAdvanced{ 12 };
			inline static std::int32_t iDCExpert{ 16 };
			inline static std::int32_t iDCMaster{ 20 };
			inline static std::int32_t iPlayerDiceMin{ 1 };
			inline static std::int32_t iPlayerDiceMax{ 20 };
			inline static std::int32_t iBonusPerBonus{ 0 };
			inline static std::int32_t iBonusPerLucky{ 5 };
			inline static std::int32_t iBonusPerPerks{ 1 };
			inline static std::int32_t iBonusPerSkill{ 2 };
		};

		class Formatting
		{
		public:
			inline static std::string sBonus;
			inline static std::string sLucky;
			inline static std::string sPerks;
			inline static std::string sSkill;
			inline static std::string sTotal;
			inline static std::string sShowRollResults;
		};

		static void Update()
		{
			if (m_FirstRun)
			{
				GetTranslationStrings();
				m_FirstRun = false;
			}

			m_ini_base.LoadFile("Data/MCM/Config/BakaAutoLockpicking/settings.ini");
			m_ini_user.LoadFile("Data/MCM/Settings/BakaAutoLockpicking.ini");

			// LockGeneral
			GetModSettingBool("LockGeneral", "bActivateContAfterPick", LockGeneral::bActivateContAfterPick);
			GetModSettingBool("LockGeneral", "bActivateDoorAfterPick", LockGeneral::bActivateDoorAfterPick);
			GetModSettingBool("LockGeneral", "bGiveWaxKeys", LockGeneral::bGiveWaxKeys);
			GetModSettingBool("LockGeneral", "bIgnoreHasKey", LockGeneral::bIgnoreHasKey);
			GetModSettingBool("LockGeneral", "bIgnoreLockGates", LockGeneral::bIgnoreLockGates);
			GetModSettingBool("LockGeneral", "bLockpickingCrimeCheck", LockGeneral::bLockpickingCrimeCheck);
			GetModSettingBool("LockGeneral", "bModEnabled", LockGeneral::bModEnabled);
			GetModSettingBool("LockGeneral", "bShowRollResults", LockGeneral::bShowRollResults);
			GetModSettingBool("LockGeneral", "bUnbreakableLockpicks", LockGeneral::bUnbreakableLockpicks);
			GetModSettingLong("LockGeneral", "iDetectionEventFailureLevel", LockGeneral::iDetectionEventFailureLevel);
			GetModSettingLong("LockGeneral", "iDetectionEventSuccessLevel", LockGeneral::iDetectionEventSuccessLevel);
			GetModSettingLong("LockGeneral", "iSkillIndex", LockGeneral::iSkillIndex);
			GetModSettingChar("LockGeneral", "sSkillName", LockGeneral::sSkillName);

			// LockRolls
			GetModSettingLong("LockRolls", "iDCNovice", LockRolls::iDCNovice);
			GetModSettingLong("LockRolls", "iDCAdvanced", LockRolls::iDCAdvanced);
			GetModSettingLong("LockRolls", "iDCExpert", LockRolls::iDCExpert);
			GetModSettingLong("LockRolls", "iDCMaster", LockRolls::iDCMaster);
			GetModSettingLong("LockRolls", "iPlayerDiceMin", LockRolls::iPlayerDiceMin);
			GetModSettingLong("LockRolls", "iPlayerDiceMax", LockRolls::iPlayerDiceMax);
			GetModSettingLong("LockRolls", "iBonusPerBonus", LockRolls::iBonusPerBonus);
			GetModSettingLong("LockRolls", "iBonusPerPerks", LockRolls::iBonusPerPerks);
			GetModSettingLong("LockRolls", "iBonusPerSkill", LockRolls::iBonusPerSkill);

			// HackGeneral
			GetModSettingBool("HackGeneral", "bActivateTermAfterHack", HackGeneral::bActivateTermAfterHack);
			GetModSettingBool("HackGeneral", "bGivePass", HackGeneral::bGivePass);
			GetModSettingBool("HackGeneral", "bIgnoreHasPass", HackGeneral::bIgnoreHasPass);
			GetModSettingBool("HackGeneral", "bIgnoreHackGates", HackGeneral::bIgnoreHackGates);
			GetModSettingBool("HackGeneral", "bHackingCrimeCheck", HackGeneral::bHackingCrimeCheck);
			GetModSettingBool("HackGeneral", "bModEnabled", HackGeneral::bModEnabled);
			GetModSettingBool("HackGeneral", "bShowRollResults", HackGeneral::bShowRollResults);
			GetModSettingBool("HackGeneral", "bNoTimeouts", HackGeneral::bNoTimeouts);
			GetModSettingLong("HackGeneral", "iDetectionEventFailureLevel", HackGeneral::iDetectionEventFailureLevel);
			GetModSettingLong("HackGeneral", "iDetectionEventSuccessLevel", HackGeneral::iDetectionEventSuccessLevel);
			GetModSettingLong("HackGeneral", "iSkillIndex", HackGeneral::iSkillIndex);
			GetModSettingChar("HackGeneral", "sSkillName", HackGeneral::sSkillName);

			// HackRolls
			GetModSettingLong("HackRolls", "iDCNovice", HackRolls::iDCNovice);
			GetModSettingLong("HackRolls", "iDCAdvanced", HackRolls::iDCAdvanced);
			GetModSettingLong("HackRolls", "iDCExpert", HackRolls::iDCExpert);
			GetModSettingLong("HackRolls", "iDCMaster", HackRolls::iDCMaster);
			GetModSettingLong("HackRolls", "iPlayerDiceMin", HackRolls::iPlayerDiceMin);
			GetModSettingLong("HackRolls", "iPlayerDiceMax", HackRolls::iPlayerDiceMax);
			GetModSettingLong("HackRolls", "iBonusPerBonus", HackRolls::iBonusPerBonus);
			GetModSettingLong("HackRolls", "iBonusPerPerks", HackRolls::iBonusPerPerks);
			GetModSettingLong("HackRolls", "iBonusPerSkill", HackRolls::iBonusPerSkill);

			m_ini_base.Reset();
			m_ini_user.Reset();
		}

		inline static bool m_FirstRun{ true };

	private:
		static void GetModSettingChar(const std::string& a_section, const std::string& a_setting, std::string_view& a_value)
		{
			auto base = m_ini_base.GetValue(a_section.c_str(), a_setting.c_str(), a_value.data());
			auto user = m_ini_user.GetValue(a_section.c_str(), a_setting.c_str(), base);
			a_value = user;
		}

		static void GetModSettingBool(const std::string& a_section, const std::string& a_setting, bool& a_value)
		{
			auto base = m_ini_base.GetBoolValue(a_section.c_str(), a_setting.c_str(), a_value);
			auto user = m_ini_user.GetBoolValue(a_section.c_str(), a_setting.c_str(), base);
			a_value = user;
		}

		static void GetModSettingLong(const std::string& a_section, const std::string& a_setting, std::int32_t& a_value)
		{
			auto base = m_ini_base.GetLongValue(a_section.c_str(), a_setting.c_str(), a_value);
			auto user = m_ini_user.GetLongValue(a_section.c_str(), a_setting.c_str(), base);
			a_value = static_cast<std::int32_t>(user);
		}

		static void GetTranslationStrings()
		{
			if (auto BSScaleformManager = RE::BSScaleformManager::GetSingleton(); BSScaleformManager && BSScaleformManager->loader)
			{
				if (auto BSScaleformTranslator =
				        static_cast<RE::BSScaleformTranslator*>(
							BSScaleformManager->loader->GetStateAddRef(
								RE::Scaleform::GFx::State::StateType::kTranslator)))
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

					FetchTranslation(BSScaleformTranslator, L"$BakaAL_Message_Bonus", Formatting::sBonus);
					FetchTranslation(BSScaleformTranslator, L"$BakaAL_Message_Lucky", Formatting::sLucky);
					FetchTranslation(BSScaleformTranslator, L"$BakaAL_Message_Perks", Formatting::sPerks);
					FetchTranslation(BSScaleformTranslator, L"$BakaAL_Message_Skill", Formatting::sSkill);
					FetchTranslation(BSScaleformTranslator, L"$BakaAL_Message_Total", Formatting::sTotal);
					FetchTranslation(BSScaleformTranslator, L"$BakaAL_Message_ShowRollResults", Formatting::sShowRollResults);
				}
			}
		}

		inline static CSimpleIniA m_ini_base{ true };
		inline static CSimpleIniA m_ini_user{ true };
	};
}
