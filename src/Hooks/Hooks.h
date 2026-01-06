#pragma once

#include "MCM/MCM.h"

namespace Hooks
{
	class BakaAutoShared
	{
	public:
		static bool PlayerHasItem(RE::TESForm* a_form)
		{
			if (!a_form)
			{
				return false;
			}

			if (auto list = a_form->As<RE::BGSListForm>())
			{
				return PlayerHasItem(list);
			}

			if (auto object = a_form->As<RE::TESBoundObject>())
			{
				return PlayerHasItem(object);
			}

			return false;
		}

		static bool PlayerHasItem(RE::BGSListForm* a_list)
		{
			if (!a_list)
			{
				return false;
			}

			for (auto& iter : a_list->arrayOfForms)
			{
				if (PlayerHasItem(iter))
				{
					return true;
				}
			}

			return false;
		}

		static bool PlayerHasItem(RE::TESBoundObject* a_object)
		{
			if (!a_object)
			{
				return false;
			}

			auto PlayerCharacter = RE::PlayerCharacter::GetSingleton();
			return (PlayerCharacter->GetInventoryObjectCount(a_object) > 0);
		}

		static bool PlayerHasPerk(RE::TESForm* a_form)
		{
			if (!a_form)
			{
				return false;
			}

			if (auto list = a_form->As<RE::BGSListForm>())
			{
				return PlayerHasPerk(list);
			}

			if (auto perk = a_form->As<RE::BGSPerk>())
			{
				return PlayerHasPerk(perk);
			}

			return false;
		}

		static bool PlayerHasPerk(RE::BGSListForm* a_list)
		{
			if (!a_list)
			{
				return false;
			}

			for (auto& iter : a_list->arrayOfForms)
			{
				if (PlayerHasPerk(iter))
				{
					return true;
				}
			}

			return false;
		}

		static bool PlayerHasPerk(RE::BGSPerk* a_perk)
		{
			if (!a_perk)
			{
				return false;
			}

			auto PlayerCharacter = RE::PlayerCharacter::GetSingleton();
			return (PlayerCharacter->GetPerkRank(a_perk) > 0);
		}

		static void ShowMessage(const char* a_settingName, std::string_view a_formatText = "", const char* a_sound = nullptr)
		{
			auto GameSettingCollection = RE::GameSettingCollection::GetSingleton();
			if (!GameSettingCollection)
			{
				return;
			}

			auto setting = GameSettingCollection->settings.find(a_settingName);
			if (setting != GameSettingCollection->settings.end())
			{
				if (setting->second)
				{
					auto message = setting->second->GetString();
					if (!message.empty())
					{
						if (!a_formatText.empty())
						{
							auto formatted = std::vformat(message, std::make_format_args(a_formatText));
							RE::SendHUDMessage::ShowHUDMessage(formatted.data(), a_sound, true, true);
							return;
						}

						RE::SendHUDMessage::ShowHUDMessage(message.data(), a_sound, true, true);
					}
				}
			}
		}
	};

	class BakaAutoHack
	{
	public:
		static void Install()
		{
			REL::Relocation target{ RE::BGSTerminal::VTABLE[0] };
			_Activate = target.write_vfunc(0x40, reinterpret_cast<std::uintptr_t>(hkActivate));
		}

		static void ShowRollModifiers()
		{
			MCM::Settings::Load(false);

			auto SkillVal = GetRollModifier_Skill();
			auto SkillMsg = std::vformat(MCM::Settings::Formatting::sSkill, std::make_format_args(SkillVal));

			auto LuckyVal = GetRollModifier_Lucky();
			auto LuckyMsg = std::vformat(MCM::Settings::Formatting::sLucky, std::make_format_args(LuckyVal));

			auto PerksVal = GetRollModifier_Perks();
			auto PerksMsg = std::vformat(MCM::Settings::Formatting::sPerks, std::make_format_args(PerksVal));

			auto BonusVal = GetRollModifier_Bonus();
			auto BonusMsg = std::vformat(MCM::Settings::Formatting::sBonus, std::make_format_args(BonusVal));

			auto TotalVal = SkillVal + LuckyVal + PerksVal + BonusVal;
			auto TotalMsg = std::vformat(MCM::Settings::Formatting::sTotal, std::make_format_args(TotalVal));

			auto msg = std::vformat("<font face='$DebugTextFont'>{}\n{}\n{}\n{}\n{}</font>"sv, std::make_format_args(SkillMsg, LuckyMsg, PerksMsg, BonusMsg, TotalMsg));
			if (auto MessageMenuManager = RE::MessageMenuManager::GetSingleton())
			{
				MessageMenuManager->Create("", msg.data(), nullptr, RE::WARNING_TYPES::kDefault, "$OK");
			}
		}

	private:
		static bool hkActivate(RE::BGSTerminal* a_this, RE::TESObjectREFR* a_itemActivated, RE::TESObjectREFR* a_actionRef, RE::TESBoundObject* a_objectToGet, std::int32_t a_count)
		{
			auto PlayerCharacter = RE::PlayerCharacter::GetSingleton();
			if (!PlayerCharacter || PlayerCharacter != a_actionRef)
			{
				return _Activate(a_this, a_itemActivated, a_actionRef, a_objectToGet, a_count);
			}

			if (!MCM::Settings::HackGeneral::bModEnabled.GetValue())
			{
				return _Activate(a_this, a_itemActivated, a_actionRef, a_objectToGet, a_count);
			}

			if (PlayerCharacter->interactingState != RE::INTERACTING_STATE::kNotInteracting)
			{
				return _Activate(a_this, a_itemActivated, a_actionRef, a_objectToGet, a_count);
			}

			if (RE::BGSTerminal::IsTerminalRefInUse(a_itemActivated))
			{
				return _Activate(a_this, a_itemActivated, a_actionRef, a_objectToGet, a_count);
			}

			if (auto Lock = a_itemActivated->GetLock())
			{
				if ((Lock->flags & 1) == 0)
				{
					return _Activate(a_this, a_itemActivated, a_actionRef, a_objectToGet, a_count);
				}

				auto LockKey = Lock->key;
				auto LockLvl = a_this->GetHackDifficultyLockLevel(a_itemActivated);
				switch (LockLvl)
				{
				case RE::LOCK_LEVEL::kEasy:
				case RE::LOCK_LEVEL::kAverage:
				case RE::LOCK_LEVEL::kHard:
				case RE::LOCK_LEVEL::kVeryHard:
				{
					if (!MCM::Settings::HackGeneral::bIgnoreHasPass.GetValue())
					{
						if (LockKey && BakaAutoShared::PlayerHasItem(LockKey))
						{
							return _Activate(a_this, a_itemActivated, a_actionRef, a_objectToGet, a_count);
						}
					}
					break;
				}

				default:
					return _Activate(a_this, a_itemActivated, a_actionRef, a_objectToGet, a_count);
				}

				if (!MCM::Settings::HackGeneral::bNoTimeouts.GetValue())
				{
					if (PlayerCharacter->IsLockedOutOfTerminal(a_itemActivated->GetHandle()))
					{
						BakaAutoShared::ShowMessage("sTerminalLockout");
						return false;
					}
				}

				if (!MCM::Settings::HackGeneral::bIgnoreHackGates.GetValue())
				{
					if (!RE::GamePlayFormulas::CanHackGateCheck(LockLvl))
					{
						BakaAutoShared::ShowMessage("sHackingGateFail");
						return false;
					}
				}

				auto LockVal = GetLockDifficultyClass(LockLvl);
				auto RollMin = MCM::Settings::HackRolls::iPlayerDiceMin.GetValue();
				auto RollMax = std::max(RollMin, MCM::Settings::HackRolls::iPlayerDiceMax.GetValue());
				auto RollMod = GetRollModifier();
				auto RollVal = effolkronium::random_thread_local::get<std::int32_t>(RollMin, RollMax);

				if (MCM::Settings::HackGeneral::bShowRollResults.GetValue())
				{
					auto results = std::vformat(MCM::Settings::Formatting::sShowRollResults, std::make_format_args(LockVal, RollVal, RollMod));
					REX::INFO("{}"sv, results);
					RE::SendHUDMessage::ShowHUDMessage(results.data(), nullptr, false, false);
				}

				RollVal += RollMod;
				if (RollVal >= LockVal)
				{
					UnlockObject(a_itemActivated);
					HandleExperience(LockLvl);
					HandleWaxKey(LockKey);

					if (MCM::Settings::HackGeneral::iDetectionEventSuccessLevel.GetValue())
					{
						PlayerCharacter->currentProcess->SetActorsDetectionEvent(PlayerCharacter, a_itemActivated->data.location, MCM::Settings::HackGeneral::iDetectionEventSuccessLevel.GetValue(), a_itemActivated);
					}

					if (MCM::Settings::HackGeneral::bActivateTermAfterHack.GetValue())
					{
						return _Activate(a_this, a_itemActivated, a_actionRef, a_objectToGet, a_count);
					}
					else
					{
						HandlePostActivate(a_itemActivated);
						return true;
					}
				}
				else
				{
					UnlockObjectFail(a_itemActivated);

					if (MCM::Settings::HackGeneral::iDetectionEventFailureLevel.GetValue())
					{
						PlayerCharacter->currentProcess->SetActorsDetectionEvent(PlayerCharacter, a_itemActivated->data.location, MCM::Settings::HackGeneral::iDetectionEventFailureLevel.GetValue(), a_itemActivated);
					}

					HandlePostActivate(a_itemActivated);
					return true;
				}
			}

			return _Activate(a_this, a_itemActivated, a_actionRef, a_objectToGet, a_count);
		}

		static std::int32_t GetLockDifficultyClass(RE::LOCK_LEVEL a_lockLevel)
		{
			switch (a_lockLevel)
			{
			case RE::LOCK_LEVEL::kAverage:
				return MCM::Settings::HackRolls::iDCAdvanced.GetValue();
			case RE::LOCK_LEVEL::kHard:
				return MCM::Settings::HackRolls::iDCExpert.GetValue();
			case RE::LOCK_LEVEL::kVeryHard:
				return MCM::Settings::HackRolls::iDCMaster.GetValue();
			default:
				return MCM::Settings::HackRolls::iDCNovice.GetValue();
			}
		}

		static std::int32_t GetRollModifier_Skill()
		{
			auto PlayerCharacter = RE::PlayerCharacter::GetSingleton();
			auto SkillLvl = PlayerCharacter->GetActorValue(*GetSkillFromIndex());
			return (MCM::Settings::HackRolls::iBonusPerSkill.GetValue() > 0) ? static_cast<std::int32_t>(floorf(SkillLvl / MCM::Settings::HackRolls::iBonusPerSkill.GetValue())) : 0;
		}

		static std::int32_t GetRollModifier_Lucky()
		{
			auto LuckyVal = RE::PlayerCharacter::GetSingleton()->GetActorValue(*(RE::ActorValue::GetSingleton()->luck));
			return (MCM::Settings::HackRolls::iBonusPerLucky.GetValue() > 0) ? static_cast<std::int32_t>(floorf(LuckyVal / MCM::Settings::HackRolls::iBonusPerLucky.GetValue())) : 0;
		}

		static std::int32_t GetRollModifier_Perks()
		{
			std::int32_t result{ 0 };
			for (auto& form : Forms::BakaAutoHack_Perks_Base->arrayOfForms)
			{
				if (BakaAutoShared::PlayerHasPerk(form))
				{
					result += MCM::Settings::HackRolls::iBonusPerPerks.GetValue();
				}
			}

			return result;
		}

		static std::int32_t GetRollModifier_Bonus()
		{
			return MCM::Settings::HackRolls::iBonusPerBonus.GetValue();
		}

		static std::int32_t GetRollModifier()
		{
			std::int32_t result{ 0 };
			result += GetRollModifier_Skill();
			result += GetRollModifier_Perks();
			result += GetRollModifier_Bonus();
			return result;
		}

		static RE::ActorValueInfo* GetSkillFromIndex()
		{
			auto ActorValue = RE::ActorValue::GetSingleton();
			switch (MCM::Settings::HackGeneral::iSkillIndex.GetValue())
			{
			case 0:
				return ActorValue->strength;
			case 1:
				return ActorValue->perception;
			case 2:
				return ActorValue->endurance;
			case 3:
				return ActorValue->charisma;
			case 4:
				return ActorValue->intelligence;
			case 5:
				return ActorValue->agility;
			case 6:
				return ActorValue->luck;
			default:
				return RE::TESForm::GetFormByEditorID<RE::ActorValueInfo>(MCM::Settings::HackGeneral::sSkillName.GetValue());
			}
		}

		static void HandleCrime(RE::TESObjectREFR* a_terminal)
		{
			if (!a_terminal)
			{
				return;
			}

			if (!a_terminal->GetHasOwner())
			{
				return;
			}

			auto PlayerCharacter = RE::PlayerCharacter::GetSingleton();
			if (a_terminal->IsAnOwner(PlayerCharacter, true, false))
			{
				return;
			}

			if (auto ProcessLists = RE::ProcessLists::GetSingleton())
			{
				std::uint32_t LOSCount{ 1 };
				if (ProcessLists->RequestHighestDetectionLevelAgainstActor(PlayerCharacter, LOSCount) > 0)
				{
					auto owner = a_terminal->GetOwner();
					PlayerCharacter->TrespassAlarm(a_terminal, owner, -1);
				}
			}
		}

		static void HandleExperience(RE::LOCK_LEVEL a_lockLevel)
		{
			RE::DifficultyLevel Difficulty{ RE::DifficultyLevel::kEasy };
			switch (a_lockLevel)
			{
			case RE::LOCK_LEVEL::kAverage:
				Difficulty = RE::DifficultyLevel::kNormal;
				break;
			case RE::LOCK_LEVEL::kHard:
				Difficulty = RE::DifficultyLevel::kHard;
				break;
			case RE::LOCK_LEVEL::kVeryHard:
				Difficulty = RE::DifficultyLevel::kVeryHard;
				break;
			default:
				break;
			}

			auto reward =
				RE::GamePlayFormulas::GetExperienceReward(
					RE::GamePlayFormulas::ExperienceActivity::kHackComputer,
					Difficulty,
					-1.0f);
			RE::PlayerCharacter::GetSingleton()->RewardExperience(reward, false, nullptr, nullptr);
		}

		static void HandlePostActivate(RE::TESObjectREFR* a_terminal)
		{
			auto PlayerCharacter = RE::PlayerCharacter::GetSingleton();
			PlayerCharacter->currentProcess->ProcessGreet(
				PlayerCharacter,
				RE::DIALOGUE_TYPE::kMiscellaneous,
				RE::DIALOGUE_SUBTYPE::kMisc_Player_Activate_Terminals,
				a_terminal,
				nullptr,
				false,
				false,
				false,
				false);

			if (MCM::Settings::HackGeneral::bHackingCrimeCheck.GetValue())
			{
				HandleCrime(a_terminal);
			}
		}

		static void HandleWaxKey(RE::TESKey* a_key)
		{
			if (a_key && PlayerHasWaxKey())
			{
				if (!BakaAutoShared::PlayerHasItem(a_key))
				{
					auto PlayerCharacter = RE::PlayerCharacter::GetSingleton();
					PlayerCharacter->AddObjectToContainer(a_key, nullptr, 1, nullptr, RE::ITEM_REMOVE_REASON::kNone);
				}
			}
		}

		static bool PlayerHasWaxKey()
		{
			if (MCM::Settings::HackGeneral::bGivePass.GetValue())
			{
				return true;
			}

			return BakaAutoShared::PlayerHasPerk(Forms::BakaAutoHack_Perks_WaxKey);
		}

		static void UnlockObject(RE::TESObjectREFR* a_terminal)
		{
			a_terminal->GetLock()->SetLocked(false);
			a_terminal->AddLockChange();

			// if (auto TerminalHacked = RE::TerminalHacked::GetEventSource())
			// {
			// 	TerminalHacked->Notify(RE::TerminalHacked::Event{ a_terminal->GetHandle() });
			// }

			if (auto BGSStoryEventManager = RE::BGSStoryEventManager::GetSingleton())
			{
				RE::BGSHackTerminal BGSHackTerminal{ a_terminal, 1 };
				BGSStoryEventManager->AddEvent(BGSHackTerminal);
			}

			RE::UIUtils::PlayMenuSound("UITerminalPasswordGood");
		}

		static void UnlockObjectFail(RE::TESObjectREFR* a_terminal)
		{
			if (!MCM::Settings::HackGeneral::bNoTimeouts.GetValue())
			{
				float HackingGuesses = 4.0f;
				RE::BGSEntryPoint::HandleEntryPoint(RE::BGSEntryPoint::ENTRY_POINT::kModHackingGuesses, RE::PlayerCharacter::GetSingleton(), &HackingGuesses);

				float HackingAttempt = a_terminal->GetActorValue(*Forms::BakaAutoHack_Attempts);
				if (++HackingAttempt >= HackingGuesses)
				{
					a_terminal->SetActorValue(*Forms::BakaAutoHack_Attempts, 0.0f);
					RE::PlayerCharacter::GetSingleton()->LockOutOfTerminal(a_terminal->GetHandle());
					BakaAutoShared::ShowMessage("sTerminalLockout");
				}
				else
				{
					a_terminal->SetActorValue(*Forms::BakaAutoHack_Attempts, HackingAttempt);
				}
			}

			if (auto BGSStoryEventManager = RE::BGSStoryEventManager::GetSingleton())
			{
				RE::BGSHackTerminal BGSHackTerminal{ a_terminal, 0 };
				BGSStoryEventManager->AddEvent(BGSHackTerminal);
			}

			RE::UIUtils::PlayMenuSound("UITerminalPasswordBad");
		}

	protected:
		inline static REL::Relocation<decltype(hkActivate)> _Activate;
	};

	class BakaAutoLock
	{
	public:
		static void Install()
		{
			REL::Relocation<std::uintptr_t> targetCONT{ REL::ID(879097), 0x1E9 };
			REL::Relocation<std::uintptr_t> targetDOOR{ REL::ID(1278339), 0x35D };

			auto& trampoline = F4SE::GetTrampoline();
			trampoline.write_call<5>(targetCONT.address(), TryUnlockObjectImpl);
			trampoline.write_call<5>(targetDOOR.address(), TryUnlockObjectImpl);

			hkHasObjects<879097, 0x114>::Install();
			hkHasObjects<1278339, 0x26C>::Install();
		}

		static void ShowRollModifiers()
		{
			MCM::Settings::Load(false);

			auto SkillVal = GetRollModifier_Skill();
			auto SkillMsg = std::vformat(MCM::Settings::Formatting::sSkill, std::make_format_args(SkillVal));

			auto LuckyVal = GetRollModifier_Lucky();
			auto LuckyMsg = std::vformat(MCM::Settings::Formatting::sLucky, std::make_format_args(LuckyVal));

			auto PerksVal = GetRollModifier_Perks();
			auto PerksMsg = std::vformat(MCM::Settings::Formatting::sPerks, std::make_format_args(PerksVal));

			auto BonusVal = GetRollModifier_Bonus();
			auto BonusMsg = std::vformat(MCM::Settings::Formatting::sBonus, std::make_format_args(BonusVal));

			auto TotalVal = SkillVal + LuckyVal + PerksVal + BonusVal;
			auto TotalMsg = std::vformat(MCM::Settings::Formatting::sTotal, std::make_format_args(TotalVal));

			auto msg = std::vformat("<font face='$DebugTextFont'>{}\n{}\n{}\n{}\n{}</font>"sv, std::make_format_args(SkillMsg, LuckyMsg, PerksMsg, BonusMsg, TotalMsg));
			if (auto MessageMenuManager = RE::MessageMenuManager::GetSingleton())
			{
				MessageMenuManager->Create("", msg.data(), nullptr, RE::WARNING_TYPES::kDefault, "$OK");
			}
		}

	private:
		template <std::uint64_t ID, std::ptrdiff_t OFFSET>
		class hkHasObjects
		{
		public:
			static void Install()
			{
				REL::Relocation<std::uintptr_t> target{ REL::ID(ID), OFFSET };
				auto& trampoline = F4SE::GetTrampoline();
				_HasObjects = trampoline.write_call<5>(target.address(), HasObjects);
			}

		private:
			static bool HasObjects(RE::Actor* a_this, void* a_arg2, std::int32_t a_arg3, std::int32_t a_arg4, std::uint32_t a_arg5, std::int32_t& a_arg6)
			{
				if (MCM::Settings::LockGeneral::bIgnoreHasKey.GetValue())
				{
					if (a_this == RE::PlayerCharacter::GetSingleton())
					{
						return false;
					}
				}

				return _HasObjects(a_this, a_arg2, a_arg3, a_arg4, a_arg5, a_arg6);
			}

			inline static REL::Relocation<decltype(HasObjects)> _HasObjects;
		};

		static bool TryUnlockObjectImpl(RE::PlayerCharacter* a_this, RE::TESObjectREFR* a_refr, bool)
		{
			if (!a_refr)
			{
				return false;
			}

			auto Lock = a_refr->GetLock();
			if (!Lock)
			{
				return false;
			}

			auto LockKey = Lock->key;
			auto LockLvl = Lock->GetLockLevel(nullptr);
			switch (LockLvl)
			{
			case RE::LOCK_LEVEL::kEasy:
			case RE::LOCK_LEVEL::kAverage:
			case RE::LOCK_LEVEL::kHard:
			case RE::LOCK_LEVEL::kVeryHard:
				break;

			case RE::LOCK_LEVEL::kRequiresKey:
			{
				if (BakaAutoShared::PlayerHasItem(LockKey))
				{
					UnlockObject(a_refr, false);
					BakaAutoShared::ShowMessage("sOpenWithKey", LockKey->GetFullName());
					return false;
				}

				auto SettingName = LockKey ? "sAutoLockPickKeyOnly" : "sAutoLockPickNoKey";
				BakaAutoShared::ShowMessage(SettingName, LockKey ? LockKey->GetFullName() : "");
				return false;
			}

			case RE::LOCK_LEVEL::kTerminal:
			{
				BakaAutoShared::ShowMessage("sAutoLockPickTerminal");
				return false;
			}

			case RE::LOCK_LEVEL::kInaccessible:
			case RE::LOCK_LEVEL::kBarred:
			case RE::LOCK_LEVEL::kChained:
			{
				BakaAutoShared::ShowMessage("sAutoLockPickInaccessible");
				return false;
			}

			default:
				return false;
			}

			if (!PlayerHasLockpicks())
			{
				BakaAutoShared::ShowMessage("sAutoLockPickNoPicks");

				if (BakaAutoShared::PlayerHasItem(LockKey))
				{
					UnlockObject(a_refr, false);
					BakaAutoShared::ShowMessage("sOpenWithKey", LockKey->GetFullName());
				}

				return false;
			}

			if (!MCM::Settings::LockGeneral::bIgnoreLockGates.GetValue())
			{
				if (!RE::GamePlayFormulas::CanPickLockGateCheck(LockLvl))
				{
					BakaAutoShared::ShowMessage("sAutoLockPickGateFail");
					return false;
				}
			}

			if (!MCM::Settings::LockGeneral::bModEnabled.GetValue())
			{
				RE::LockpickingMenu::OpenLockpickingMenu(a_refr);
				return false;
			}

			auto LockVal = GetLockDifficultyClass(LockLvl);
			auto RollMin = MCM::Settings::LockRolls::iPlayerDiceMin.GetValue();
			auto RollMax = std::max(RollMin, MCM::Settings::LockRolls::iPlayerDiceMax.GetValue());
			auto RollMod = GetRollModifier();
			auto RollVal = effolkronium::random_thread_local::get<std::int32_t>(RollMin, RollMax);

			if (MCM::Settings::LockGeneral::bShowRollResults.GetValue())
			{
				auto results = std::vformat(MCM::Settings::Formatting::sShowRollResults, std::make_format_args(LockVal, RollVal, RollMod));
				REX::INFO("{}"sv, results);
				RE::SendHUDMessage::ShowHUDMessage(results.data(), nullptr, false, false);
			}

			RollVal += RollMod;
			if (RollVal >= LockVal)
			{
				UnlockObject(a_refr);
				HandleExperience(LockLvl);
				HandleWaxKey(LockKey);

				if (MCM::Settings::LockGeneral::iDetectionEventSuccessLevel.GetValue())
				{
					a_this->currentProcess->SetActorsDetectionEvent(a_this, a_refr->data.location, MCM::Settings::LockGeneral::iDetectionEventSuccessLevel.GetValue(), a_refr);
				}
			}
			else
			{
				HandleLockpickRemoval();

				if (MCM::Settings::LockGeneral::iDetectionEventFailureLevel.GetValue())
				{
					a_this->currentProcess->SetActorsDetectionEvent(a_this, a_refr->data.location, MCM::Settings::LockGeneral::iDetectionEventFailureLevel.GetValue(), a_refr);
				}
			}

			if (MCM::Settings::LockGeneral::bLockpickingCrimeCheck.GetValue())
			{
				HandleCrime(a_refr);
			}

			return false;
		}

		static std::int32_t GetLockDifficultyClass(RE::LOCK_LEVEL a_lockLevel)
		{
			switch (a_lockLevel)
			{
			case RE::LOCK_LEVEL::kAverage:
				return MCM::Settings::LockRolls::iDCAdvanced.GetValue();
			case RE::LOCK_LEVEL::kHard:
				return MCM::Settings::LockRolls::iDCExpert.GetValue();
			case RE::LOCK_LEVEL::kVeryHard:
				return MCM::Settings::LockRolls::iDCMaster.GetValue();
			default:
				return MCM::Settings::LockRolls::iDCNovice.GetValue();
			}
		}

		static std::int32_t GetRollModifier_Skill()
		{
			auto PlayerCharacter = RE::PlayerCharacter::GetSingleton();
			auto SkillMod = PlayerCharacter->GetActorValue(*Forms::LGND_LockPickSweetSpot);
			auto SkillLvl = PlayerCharacter->GetActorValue(*GetSkillFromIndex());
			auto SkillVal = SkillLvl * (1.0f + ((SkillMod) / 10.0f));
			return MCM::Settings::LockRolls::iBonusPerSkill.GetValue() > 0 ? static_cast<std::int32_t>(floorf(SkillVal / MCM::Settings::LockRolls::iBonusPerSkill.GetValue())) : 0;
		}

		static std::int32_t GetRollModifier_Lucky()
		{
			auto LuckyVal = RE::PlayerCharacter::GetSingleton()->GetActorValue(*(RE::ActorValue::GetSingleton()->luck));
			return MCM::Settings::LockRolls::iBonusPerLucky.GetValue() > 0 ? static_cast<std::int32_t>(floorf(LuckyVal / MCM::Settings::LockRolls::iBonusPerLucky.GetValue())) : 0;
		}

		static std::int32_t GetRollModifier_Perks()
		{
			std::int32_t result{ 0 };
			for (auto& form : Forms::BakaAutoLock_Perks_Base->arrayOfForms)
			{
				if (BakaAutoShared::PlayerHasPerk(form))
				{
					result += MCM::Settings::LockRolls::iBonusPerPerks.GetValue();
				}
			}

			return result;
		}

		static std::int32_t GetRollModifier_Bonus()
		{
			return MCM::Settings::LockRolls::iBonusPerBonus.GetValue();
		}

		static std::int32_t GetRollModifier()
		{
			std::int32_t result{ 0 };
			result += GetRollModifier_Skill();
			result += GetRollModifier_Perks();
			result += GetRollModifier_Bonus();
			return result;
		}

		static RE::ActorValueInfo* GetSkillFromIndex()
		{
			auto ActorValue = RE::ActorValue::GetSingleton();
			switch (MCM::Settings::LockGeneral::iSkillIndex.GetValue())
			{
			case 0:
				return ActorValue->strength;
			case 1:
				return ActorValue->perception;
			case 2:
				return ActorValue->endurance;
			case 3:
				return ActorValue->charisma;
			case 4:
				return ActorValue->intelligence;
			case 5:
				return ActorValue->agility;
			case 6:
				return ActorValue->luck;
			default:
				return RE::TESForm::GetFormByEditorID<RE::ActorValueInfo>(MCM::Settings::LockGeneral::sSkillName.GetValue());
			}
		}

		static void HandleCrime(RE::TESObjectREFR* a_refr)
		{
			auto owner = a_refr->GetOwner();
			if (!owner)
			{
				if (a_refr->GetFormType() != RE::ENUM_FORM_ID::kDOOR)
				{
					return;
				}

				if (auto ExtraTeleport = a_refr->extraList->GetByType<RE::ExtraTeleport>())
				{
					if (ExtraTeleport->teleportData)
					{
						if (auto LinkedDoor = ExtraTeleport->teleportData->linkedDoor.get())
						{
							if (auto ParentCell = LinkedDoor->GetParentCell())
							{
								owner = ParentCell->GetOwner();
							}
						}
					}
				}
			}

			if (!owner)
			{
				return;
			}

			auto PlayerCharacter = RE::PlayerCharacter::GetSingleton();
			if (auto ProcessLists = RE::ProcessLists::GetSingleton())
			{
				std::uint32_t LOSCount{ 1 };
				if (ProcessLists->RequestHighestDetectionLevelAgainstActor(PlayerCharacter, LOSCount) > 0)
				{
					auto CrimeChance{ 1.0f };
					RE::BGSEntryPoint::HandleEntryPoint(RE::BGSEntryPoint::ENTRY_POINT::kModLockpickingCrimeChance, PlayerCharacter, a_refr, &CrimeChance);

					auto Chance = 0.0f;
					if (Chance < CrimeChance)
					{
						auto Prison = PlayerCharacter->currentPrisonFaction;
						if (Prison && Prison->crimeData.crimeValues.escapeCrimeGold)
						{
							PlayerCharacter->SetEscaping(true, false);
						}
						else
						{
							PlayerCharacter->TrespassAlarm(a_refr, owner, -1);
							BakaAutoShared::ShowMessage("sLockpickingCaught");
						}
					}
				}
			}
		}

		static void HandleExperience(RE::LOCK_LEVEL a_lockLevel)
		{
			auto reward = RE::GamePlayFormulas::GetLockXPReward(a_lockLevel);
			RE::PlayerCharacter::GetSingleton()->RewardExperience(reward, false, nullptr, nullptr);
		}

		static void HandleLockpickRemoval()
		{
			if (PlayerHasBreakable())
			{
				for (auto& iter : Forms::BakaAutoLock_Items_Lockpick->arrayOfForms)
				{
					if (BakaAutoShared::PlayerHasItem(iter))
					{
						RE::UIUtils::PlayMenuSound("UILockpickingPickBreak");

						RE::TESObjectREFR::RemoveItemData removeItemData{ iter, 1 };
						RE::PlayerCharacter::ScopedInventoryChangeMessageContext ctx{ true, true };
						RE::PlayerCharacter::GetSingleton()->RemoveItem(removeItemData);
					}
				}
			}
			else
			{
				RE::UIUtils::PlayMenuSound("UILockpickingPickMovement");
			}
		}

		static void HandleWaxKey(RE::TESKey* a_key)
		{
			if (a_key && PlayerHasWaxKey())
			{
				if (!BakaAutoShared::PlayerHasItem(a_key))
				{
					auto PlayerCharacter = RE::PlayerCharacter::GetSingleton();
					PlayerCharacter->AddObjectToContainer(a_key, nullptr, 1, nullptr, RE::ITEM_REMOVE_REASON::kNone);
				}
			}
		}

		static bool PlayerHasBreakable()
		{
			if (MCM::Settings::LockGeneral::bUnbreakableLockpicks.GetValue())
			{
				return false;
			}

			if (BakaAutoShared::PlayerHasPerk(Forms::BakaAutoLock_Perks_Unbreakable))
			{
				return false;
			}

			return true;
		}

		static bool PlayerHasLockpicks()
		{
			return BakaAutoShared::PlayerHasItem(Forms::BakaAutoLock_Items_Lockpick);
		}

		static bool PlayerHasWaxKey()
		{
			if (MCM::Settings::LockGeneral::bGiveWaxKeys.GetValue())
			{
				return true;
			}

			return BakaAutoShared::PlayerHasPerk(Forms::BakaAutoLock_Perks_WaxKey);
		}

		static void UnlockObject(RE::TESObjectREFR* a_refr, bool a_picked = true)
		{
			a_refr->GetLock()->SetLocked(false);
			a_refr->AddLockChange();

			if (a_picked)
			{
				if (auto LocksPicked = RE::LocksPicked::GetEventSource())
				{
					LocksPicked->Notify(RE::LocksPicked::Event{});
				}

				if (auto BGSStoryEventManager = RE::BGSStoryEventManager::GetSingleton())
				{
					auto bIsCrime = a_refr->IsCrimeToActivate();
					RE::BGSPickLockEvent BGSLockPickEvent{ RE::PlayerCharacter::GetSingleton(), a_refr, bIsCrime };
					BGSStoryEventManager->AddEvent(BGSLockPickEvent);
				}
			}

			RE::UIUtils::PlayMenuSound("UILockpickingUnlock");
			if ((MCM::Settings::LockGeneral::bActivateContAfterPick.GetValue() && (a_refr->data.objectReference && a_refr->data.objectReference->GetFormType() == RE::ENUM_FORM_ID::kCONT)) || (MCM::Settings::LockGeneral::bActivateDoorAfterPick.GetValue() && (a_refr->data.objectReference && a_refr->data.objectReference->GetFormType() == RE::ENUM_FORM_ID::kDOOR)))
			{
				a_refr->ActivateRef(RE::PlayerCharacter::GetSingleton(), nullptr, 1, false, false, false);
			}
		}
	};
}
