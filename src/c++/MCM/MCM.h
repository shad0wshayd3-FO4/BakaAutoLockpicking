#pragma once

namespace MCM
{
	class Settings
	{
	public:
		class General
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
		};

		class Formatting
		{
		public:
			inline static std::string sBonus;
			inline static std::string sPerks;
			inline static std::string sSkill;
			inline static std::string sTotal;
			inline static std::string sCriticalFailure;
			inline static std::string sCriticalSuccess;
			inline static std::string sShowRollResults;
		};

		class Rolls
		{
		public:
			inline static bool bCriticalFailure{ false };
			inline static bool bCriticalSuccess{ true };

			inline static std::int32_t iDCNovice{ 8 };
			inline static std::int32_t iDCAdvanced{ 12 };
			inline static std::int32_t iDCExpert{ 16 };
			inline static std::int32_t iDCMaster{ 20 };
			inline static std::int32_t iPlayerDiceMin{ 1 };
			inline static std::int32_t iPlayerDiceMax{ 20 };
			inline static std::int32_t iBonusPerBonus{ 0 };
			inline static std::int32_t iBonusPerPerks{ 1 };
			inline static std::int32_t iBonusPerSkill{ 2 };
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

			// General
			GetModSettingBool("General", "bActivateContAfterPick", General::bActivateContAfterPick);
			GetModSettingBool("General", "bActivateDoorAfterPick", General::bActivateDoorAfterPick);
			GetModSettingBool("General", "bGiveWaxKeys", General::bGiveWaxKeys);
			GetModSettingBool("General", "bIgnoreHasKey", General::bIgnoreHasKey);
			GetModSettingBool("General", "bIgnoreLockGates", General::bIgnoreLockGates);
			GetModSettingBool("General", "bLockpickingCrimeCheck", General::bLockpickingCrimeCheck);
			GetModSettingBool("General", "bModEnabled", General::bModEnabled);
			GetModSettingBool("General", "bShowRollResults", General::bShowRollResults);
			GetModSettingBool("General", "bUnbreakableLockpicks", General::bUnbreakableLockpicks);
			GetModSettingLong("General", "iDetectionEventFailureLevel", General::iDetectionEventFailureLevel);
			GetModSettingLong("General", "iDetectionEventSuccessLevel", General::iDetectionEventSuccessLevel);
			// GetModSettingLong("General", "iSkillIndex", General::iSkillIndex);

			// Rolls
			GetModSettingBool("Rolls", "bCriticalFailure", Rolls::bCriticalFailure);
			GetModSettingBool("Rolls", "bCriticalSuccess", Rolls::bCriticalSuccess);
			GetModSettingLong("Rolls", "iDCNovice", Rolls::iDCNovice);
			GetModSettingLong("Rolls", "iDCAdvanced", Rolls::iDCAdvanced);
			GetModSettingLong("Rolls", "iDCExpert", Rolls::iDCExpert);
			GetModSettingLong("Rolls", "iDCMaster", Rolls::iDCMaster);
			GetModSettingLong("Rolls", "iPlayerDiceMin", Rolls::iPlayerDiceMin);
			GetModSettingLong("Rolls", "iPlayerDiceMax", Rolls::iPlayerDiceMax);
			GetModSettingLong("Rolls", "iBonusPerBonus", Rolls::iBonusPerBonus);
			GetModSettingLong("Rolls", "iBonusPerPerks", Rolls::iBonusPerPerks);
			GetModSettingLong("Rolls", "iBonusPerSkill", Rolls::iBonusPerSkill);

			m_ini_base.Reset();
			m_ini_user.Reset();
		}

		inline static bool m_FirstRun{ true };

	private:
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
					FetchTranslation(BSScaleformTranslator, L"$BakaAL_Message_Perks", Formatting::sPerks);
					FetchTranslation(BSScaleformTranslator, L"$BakaAL_Message_Skill", Formatting::sSkill);
					FetchTranslation(BSScaleformTranslator, L"$BakaAL_Message_Total", Formatting::sTotal);
					FetchTranslation(BSScaleformTranslator, L"$BakaAL_Message_CriticalFailure", Formatting::sCriticalFailure);
					FetchTranslation(BSScaleformTranslator, L"$BakaAL_Message_CriticalSuccess", Formatting::sCriticalSuccess);
					FetchTranslation(BSScaleformTranslator, L"$BakaAL_Message_ShowRollResults", Formatting::sShowRollResults);
				}
			}
		}

		inline static CSimpleIniA m_ini_base{ true };
		inline static CSimpleIniA m_ini_user{ true };
	};
}
