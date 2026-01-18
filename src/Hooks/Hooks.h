#pragma once

#include "MCM/MCM.h"

namespace Hooks
{
	class detail
	{
	public:
		class Player
		{
		public:
			static auto GetPerkCount(RE::BGSListForm* a_formList)
			{
				std::int32_t result{ 0 };
				if (a_formList)
				{
					if (auto PlayerCharacter = RE::PlayerCharacter::GetSingleton())
					{
						a_formList->ForEachForm(
							[&](RE::TESForm* a_form)
							{
								if (auto perk = a_form->As<RE::BGSPerk>();
									perk && PlayerCharacter->GetPerkRank(perk) != 0)
								{
									result++;
								}

								return RE::BSContainer::ForEachResult::kContinue;
							});
					}
				}

				return result;
			}

			static auto GetValue(RE::ActorValueInfo* a_info)
			{
				if (a_info)
				{
					if (auto PlayerCharacter = RE::PlayerCharacter::GetSingleton())
					{
						return PlayerCharacter->GetActorValue(*a_info);
					}
				}

				return 0.0f;
			} 

			static bool HasObject(RE::TESBoundObject* a_object)
			{
				if (a_object)
				{
					if (auto PlayerCharacter = RE::PlayerCharacter::GetSingleton())
					{
						RE::PACKAGE_OBJECT_TYPE matchID{ RE::PACKAGE_OBJECT_TYPE::kNone };
						return PlayerCharacter->HasObjects(a_object, RE::PACKAGE_OBJECT_TYPE::kNone, 1, 0, matchID);
					}
				}

				return false;
			}

			static bool HasPerk(RE::BGSListForm* a_formList)
			{
				auto result{ false };
				if (a_formList)
				{
					if (auto PlayerCharacter = RE::PlayerCharacter::GetSingleton())
					{
						a_formList->ForEachForm(
							[&](RE::TESForm* a_form)
							{
								if (auto perk = a_form->As<RE::BGSPerk>();
									perk && PlayerCharacter->GetPerkRank(perk) != 0)
								{
									result = true;
									return RE::BSContainer::ForEachResult::kStop;
								}

								return RE::BSContainer::ForEachResult::kContinue;
							});
					}
				}

				return result;
			}

			static bool HasPerk(RE::BGSPerk* a_perk)
			{
				if (a_perk)
				{
					if (auto PlayerCharacter = RE::PlayerCharacter::GetSingleton())
					{
						return PlayerCharacter->GetPerkRank(a_perk) != 0;
					}
				}

				return false;
			}
		};

		static bool IsExempt()
		{
			if (!MCM::Settings::General::bEnable)
			{
				return true;
			}

			return false;
		}

		static void ShowMessage(std::string_view a_setting, const char* a_format = nullptr, const char* a_sound = nullptr)
		{
			auto GameSettingCollection = RE::GameSettingCollection::GetSingleton();
			if (!GameSettingCollection)
				return;

			auto GameSetting = GameSettingCollection->GetSetting(a_setting);
			if (!GameSetting)
				return;

			auto message = GameSetting->GetString();
			if (message.empty())
				return;

			if (a_format)
			{
				auto vformat = std::vformat(message, std::make_format_args(a_format));
				RE::SendHUDMessage::ShowHUDMessage(vformat.data(), a_sound, true, true);
			}
			else
			{
				RE::SendHUDMessage::ShowHUDMessage(message.data(), a_sound, true, true);
			}
		}
	};

	template <class T>
	class AutoBase :
		public REX::Singleton<AutoBase<T>>
	{
	public:
		static std::int32_t GetLockDC(RE::LOCK_LEVEL a_lockLevel)
		{
			switch (a_lockLevel)
			{
			case RE::LOCK_LEVEL::kAverage:
				return T::Rolls::iLockDC1;
			case RE::LOCK_LEVEL::kHard:
				return T::Rolls::iLockDC2;
			case RE::LOCK_LEVEL::kVeryHard:
				return T::Rolls::iLockDC3;
			default:
				return T::Rolls::iLockDC0;
			}
		}

		static std::int32_t GetRollModStat()
		{
			auto value = detail::Player::GetValue(GetStatByName());
			return static_cast<std::int32_t>(T::Rolls::iModPerStat * value);
		}

		static std::int32_t GetRollModLuck()
		{
			auto value = detail::Player::GetValue(GetStatByName("Luck"s));
			return static_cast<std::int32_t>(T::Rolls::iModPerLuck * value);
		}

		static std::int32_t GetRollModPerk()
		{
			auto value = detail::Player::GetPerkCount(T::Runtime::Perks_Mod);
			return static_cast<std::int32_t>(T::Rolls::iModPerPerk * value);
		}

		static std::int32_t GetRollMod()
		{
			auto result{ 0 };
			result += GetRollModStat();
			result += GetRollModLuck();
			result += GetRollModPerk();
			result += T::Rolls::iModBonus;
			return result;
		}

		static std::int32_t GetRollRNG()
		{
			auto min = std::max<std::int32_t>(1, T::Rolls::iRollMin);
			auto max = std::max<std::int32_t>(min, T::Rolls::iRollMax);
			return effolkronium::random_thread_local::get<std::int32_t>(min, max);
		}

		static RE::ActorValueInfo* GetStatByName()
		{
			return GetStatByName(T::Rolls::sStatName);
		}

		static RE::ActorValueInfo* GetStatByName(std::string a_name)
		{
			return RE::TESForm::GetFormByEditorID<RE::ActorValueInfo>(a_name);
		}
	
		static void HandleBreak(const char* a_break, const char* a_noBreak)
		{
			if (HasUnbreakable())
			{
				return RE::UIUtils::PlayMenuSound(a_noBreak);
			}

			if (T::Runtime::Items_Lockpick)
			{
				if (T::Runtime::Items_Lockpick->arrayOfForms.size() == 0 &&
					T::Runtime::Items_Lockpick->scriptAddedFormCount == 0)
				{
					return RE::UIUtils::PlayMenuSound(a_noBreak);
				}

				T::Runtime::Items_Lockpick->ForEachForm(
					[&](RE::TESForm* a_form)
					{
						if (auto object = a_form->As<RE::TESBoundObject>();
							object && detail::Player::HasObject(object))
						{
							if (auto PlayerCharacter = RE::PlayerCharacter::GetSingleton())
							{
								RE::TESObjectREFR::RemoveItemData                        rid{ object, 1 };
								RE::PlayerCharacter::ScopedInventoryChangeMessageContext ctx{ true, true };
								PlayerCharacter->RemoveItem(rid);
								RE::UIUtils::PlayMenuSound(a_break);
							}

							return RE::BSContainer::ForEachResult::kStop;
						}

						return RE::BSContainer::ForEachResult::kContinue;
					});
			}
		}

		static void HandleDetection(RE::TESObjectREFR* a_refr, std::int32_t a_value)
		{
			if (a_value > 0)
			{
				if (auto PlayerCharacter = RE::PlayerCharacter::GetSingleton())
				{
					PlayerCharacter->currentProcess->SetActorsDetectionEvent(PlayerCharacter, a_refr->data.location, a_value, a_refr);
				}
			}
		}

		static bool HandleRoll(RE::LOCK_LEVEL a_lockLevel)
		{
			auto LockMod = GetLockDC(a_lockLevel);
			auto RollMod = GetRollMod();
			auto RollRNG = GetRollRNG();

			if (T::bShowRollResults)
			{
				REX::INFO("DC: {} | Roll: {}+{}", LockMod, RollRNG, RollMod);
			}

			return ((RollRNG + RollMod) >= LockMod);
		}

		static void HandleWaxKey(RE::TESBoundObject* a_object)
		{
			if (a_object && !detail::Player::HasObject(a_object) && HasWaxKey())
			{
				if (auto PlayerCharacter = RE::PlayerCharacter::GetSingleton())
				{
					PlayerCharacter->AddObjectToContainer(a_object, nullptr, 1, nullptr, RE::ITEM_REMOVE_REASON::kNone);
				}
			}
		}

		static bool HasLockpick()
		{
			if (T::Runtime::Items_Lockpick)
			{
				if (T::Runtime::Items_Lockpick->arrayOfForms.size() == 0 &&
					T::Runtime::Items_Lockpick->scriptAddedFormCount == 0)
				{
					return true;
				}

				auto result{ false };
				T::Runtime::Items_Lockpick->ForEachForm(
					[&](RE::TESForm* a_form)
					{
						if (auto object = a_form->As<RE::TESBoundObject>();
							object && detail::Player::HasObject(object))
						{
							result = true;
							return RE::BSContainer::ForEachResult::kStop;
						}

						return RE::BSContainer::ForEachResult::kContinue;
					});
				return result;
			}

			return false;
		}

		static bool HasUnbreakable()
		{
			if (T::bForceUnbreakable)
			{
				return true;
			}

			return detail::Player::HasPerk(T::Runtime::Perks_Unbreakable);
		}

		static bool HasWaxKey()
		{
			if (T::bForceWaxKey)
			{
				return true;
			}

			return detail::Player::HasPerk(T::Runtime::Perks_WaxKey);
		}
	};

	class AutoHack :
		public AutoBase<MCM::Settings::AutoHack>
	{
	private:
		static bool CanAutoHack(RE::LOCK_LEVEL a_lockLevel)
		{
			if (MCM::Settings::AutoHack::bNoPickGates)
			{
				return true;
			}

			return RE::GamePlayFormulas::CanHackGateCheck(a_lockLevel);
		}

		static void HandleExperience(RE::LOCK_LEVEL a_lockLevel)
		{
			RE::DifficultyLevel DifficultyLevel{ std::to_underlying(a_lockLevel) + 1 };
			if (auto PlayerCharacter = RE::PlayerCharacter::GetSingleton())
			{
				auto value = RE::GamePlayFormulas::GetExperienceReward(RE::GamePlayFormulas::ExperienceActivity::kHackComputer, DifficultyLevel, -1.0f);
				PlayerCharacter->RewardExperience(value, false, nullptr, nullptr);
			}
		}

		static void HandleStoryManager(RE::TESObjectREFR* a_refr, bool a_hacked)
		{
			if (auto BGSStoryEventManager = RE::BGSStoryEventManager::GetSingleton())
			{
				RE::BGSHackTerminal BGSHackTerminal{ a_refr, static_cast<std::uint32_t>(a_hacked) };
				BGSStoryEventManager->AddEvent(BGSHackTerminal);
			}
		}

		static void HandleUnlockFail(RE::TESObjectREFR* a_refr)
		{
			if (!HasUnbreakable())
			{
				if (auto PlayerCharacter = RE::PlayerCharacter::GetSingleton())
				{
					float hackingGuesses{ 4.0f };
					RE::BGSEntryPoint::HandleEntryPoint(RE::BGSEntryPoint::ENTRY_POINT::kModHackingGuesses, PlayerCharacter, &hackingGuesses);

					float hackingAttempt = a_refr->GetActorValue(*MCM::Settings::AutoHack::Runtime::BakaAutoHackAttempts);
					if (++hackingAttempt >= hackingGuesses)
					{
						a_refr->SetActorValue(*MCM::Settings::AutoHack::Runtime::BakaAutoHackAttempts, 0.0f);
						PlayerCharacter->LockOutOfTerminal(a_refr->GetHandle());
						detail::ShowMessage("sTerminalLockout");
					}
					else
					{
						a_refr->SetActorValue(*MCM::Settings::AutoHack::Runtime::BakaAutoHackAttempts, hackingAttempt);
					}
				}
			}

			HandleStoryManager(a_refr, true);
			RE::UIUtils::PlayMenuSound("UITerminalPasswordBad");
		}

		static void HandleUnlockPass(RE::TESObjectREFR* a_refr)
		{
			a_refr->GetLock()->SetLocked(false);
			a_refr->AddLockChange();

			if (auto TerminalHacked = RE::TerminalHacked::GetEventSource())
			{
				RE::TerminalHacked::Event event{ a_refr->GetHandle() };
				TerminalHacked->Notify(event);
			}

			HandleStoryManager(a_refr, true);
			RE::UIUtils::PlayMenuSound("UITerminalPasswordGood");
		}

		static void PostActivate(RE::TESObjectREFR* a_refr)
		{
			if (auto PlayerCharacter = RE::PlayerCharacter::GetSingleton())
			{
				PlayerCharacter->currentProcess->ProcessGreet(
					PlayerCharacter,
					RE::DIALOGUE_TYPE::kMiscellaneous,
					RE::DIALOGUE_SUBTYPE::kMisc_Player_Activate_Terminals,
					a_refr,
					nullptr,
					false,
					false,
					false,
					false);

				if (!MCM::Settings::AutoHack::bNoCrime)
				{
					if (a_refr &&
						a_refr->GetHasOwner() &&
						!a_refr->IsAnOwner(PlayerCharacter, true, false))
					{
						if (auto ProcessLists = RE::ProcessLists::GetSingleton())
						{
							std::uint32_t count{ 1 };
							if (ProcessLists->RequestHighestDetectionLevelAgainstActor(PlayerCharacter, count) > 0)
							{
								auto owner = a_refr->GetOwner();
								PlayerCharacter->TrespassAlarm(a_refr, owner, -1);
							}
						}
					}
				}
			}
		}

		class hkActivate
		{
		private:
			static bool Activate(
				RE::BGSTerminal*    a_this,
				RE::TESObjectREFR*  a_itemActivated,
				RE::Actor*          a_actionRef,
				RE::TESBoundObject* a_objectToGet,
				std::int32_t        a_count)
			{
				if (!a_actionRef)
				{
					return false;
				}

				if (detail::IsExempt())
				{
					return _Activate0(a_this, a_itemActivated, a_actionRef, a_objectToGet, a_count);
				}

				if (!MCM::Settings::AutoHack::bEnableAutoHack)
				{
					return _Activate0(a_this, a_itemActivated, a_actionRef, a_objectToGet, a_count);
				}

				if (a_actionRef->formID != 0x14)
				{
					return _Activate0(a_this, a_itemActivated, a_actionRef, a_objectToGet, a_count);
				}

				if (a_actionRef->interactingState != RE::INTERACTING_STATE::kNotInteracting)
				{
					return _Activate0(a_this, a_itemActivated, a_actionRef, a_objectToGet, a_count);
				}

				if (RE::BGSTerminal::IsTerminalRefInUse(a_itemActivated))
				{
					detail::ShowMessage("sObjectInUse");
					return false;
				}

				if (auto Lock = a_itemActivated->GetLock())
				{
					if ((Lock->flags & 1) == 0)
					{
						return _Activate0(a_this, a_itemActivated, a_actionRef, a_objectToGet, a_count);
					}

					auto LockLevel = a_this->GetHackDifficultyLockLevel(a_itemActivated);
					switch (LockLevel)
					{
					case RE::LOCK_LEVEL::kEasy:
					case RE::LOCK_LEVEL::kAverage:
					case RE::LOCK_LEVEL::kHard:
					case RE::LOCK_LEVEL::kVeryHard:
						if (Lock->key && detail::Player::HasObject(Lock->key) && !MCM::Settings::AutoHack::bNoKeyUse)
						{
							if (MCM::Settings::AutoHack::bExperienceFromKeys)
							{
								HandleExperience(LockLevel);
							}

							return _Activate0(a_this, a_itemActivated, a_actionRef, a_objectToGet, a_count);
						}
						break;

					default:
						return _Activate0(a_this, a_itemActivated, a_actionRef, a_objectToGet, a_count);
					}

					if (!MCM::Settings::AutoHack::bForceUnbreakable)
					{
						if (auto PlayerCharacter = RE::PlayerCharacter::GetSingleton();
							PlayerCharacter && PlayerCharacter->IsLockedOutOfTerminal(a_itemActivated->GetHandle()))
						{
							detail::ShowMessage("sTerminalLockout");
							return false;
						}
					}

					if (!CanAutoHack(LockLevel))
					{
						detail::ShowMessage("sHackingGateFail");
						return false;
					}

					if (HandleRoll(LockLevel))
					{
						HandleUnlockPass(a_itemActivated);
						HandleDetection(a_itemActivated, MCM::Settings::AutoHack::iDetectionEventSuccess);
						HandleExperience(LockLevel);
						HandleWaxKey(Lock->key);

						if (MCM::Settings::AutoHack::bActivateTERM)
						{
							return _Activate0(a_this, a_itemActivated, a_actionRef, a_objectToGet, a_count);
						}
						else
						{
							PostActivate(a_itemActivated);
							return true;
						}
					}
					else
					{
						HandleUnlockFail(a_itemActivated);
						HandleBreak(nullptr, nullptr);
						HandleDetection(a_itemActivated, MCM::Settings::AutoHack::iDetectionEventFailure);
						PostActivate(a_itemActivated);
						return true;
					}
				}

				return _Activate0(a_this, a_itemActivated, a_actionRef, a_objectToGet, a_count);
			}

			inline static REL::HookVFT _Activate0{ RE::BGSTerminal::VTABLE[0], 0x40, Activate };  // BGSTerminal::Activate
		};
	};

	class AutoPick :
		public AutoBase<MCM::Settings::AutoPick>
	{
	private:
		static bool CanAutoPick(RE::LOCK_LEVEL a_lockLevel)
		{
			if (MCM::Settings::AutoPick::bNoPickGates)
			{
				return true;
			}

			return RE::GamePlayFormulas::CanPickLockGateCheck(a_lockLevel);
		}

		static void HandleCrime(RE::TESObjectREFR* a_refr)
		{
			auto owner = a_refr->GetOwner();
			if (!owner)
			{
				if (a_refr->data.objectReference &&
					a_refr->data.objectReference->GetFormType() == RE::ENUM_FORM_ID::kDOOR)
				{
					if (auto ExtraTeleport = a_refr->extraList->GetByType<RE::ExtraTeleport>();
						ExtraTeleport && ExtraTeleport->teleportData)
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
				return;

			if (auto ProcessLists = RE::ProcessLists::GetSingleton())
			{
				if (auto PlayerCharacter = RE::PlayerCharacter::GetSingleton())
				{
					std::uint32_t count{ 1 };
					if (ProcessLists->RequestHighestDetectionLevelAgainstActor(PlayerCharacter, count) > 0)
					{
						auto crimeChance{ 1.0f };
						RE::BGSEntryPoint::HandleEntryPoint(RE::BGSEntryPoint::ENTRY_POINT::kModLockpickingCrimeChance, PlayerCharacter, a_refr, &crimeChance);

						auto crimeRoll = effolkronium::random_thread_local::get<float>(0.0f, 1.0f);
						if (crimeRoll < crimeChance)
						{
							if (PlayerCharacter->currentPrisonFaction &&
								PlayerCharacter->currentPrisonFaction->crimeData.crimeValues.escapeCrimeGold > 0)
							{
								PlayerCharacter->SetEscaping(true, false);
							}
							else
							{
								PlayerCharacter->TrespassAlarm(a_refr, owner, -1);
								detail::ShowMessage("sLockpickingCaught");
							}
						}
					}
				}
			}
		}

		static void HandleExperience(RE::LOCK_LEVEL a_lockLevel)
		{
			if (auto PlayerCharacter = RE::PlayerCharacter::GetSingleton())
			{
				auto value = RE::GamePlayFormulas::GetLockXPReward(a_lockLevel);
				PlayerCharacter->RewardExperience(value, false, nullptr, nullptr);
			}
		}

		static void HandleUnlock(RE::TESObjectREFR* a_refr, bool a_picked = true)
		{
			a_refr->GetLock()->SetLocked(false);
			a_refr->AddLockChange();

			RE::UIUtils::PlayMenuSound("UILockpickingUnlock");

			if (auto PlayerCharacter = RE::PlayerCharacter::GetSingleton())
			{
				if (a_picked)
				{
					if (auto LocksPicked = RE::LocksPicked::GetEventSource())
					{
						RE::LocksPicked::Event event{};
						LocksPicked->Notify(event);
					}

					if (auto BGSStoryEventManger = RE::BGSStoryEventManager::GetSingleton())
					{
						RE::BGSPickLockEvent BGSPickLockEvent{ PlayerCharacter, a_refr, a_refr->IsCrimeToActivate() };
						BGSStoryEventManger->AddEvent(BGSPickLockEvent);
					}
				}

				if (a_refr->data.objectReference)
				{
					switch (a_refr->data.objectReference->GetFormType())
					{
					case RE::ENUM_FORM_ID::kCONT:
						if (MCM::Settings::AutoPick::bActivateCONT)
							a_refr->ActivateRef(PlayerCharacter, nullptr, 1, false, false, false);
						break;
					case RE::ENUM_FORM_ID::kDOOR:
						if (MCM::Settings::AutoPick::bActivateDOOR)
							a_refr->ActivateRef(PlayerCharacter, nullptr, 1, false, false, false);
						break;
					default:
						break;
					}
				}
			}
		}

		static void HandleUnlockWithKey(RE::TESObjectREFR* a_refr)
		{
			auto Lock = a_refr->GetLock();
			if (Lock->key)
			{
				if (detail::Player::HasObject(Lock->key))
				{
					detail::ShowMessage("sOpenWithKey", Lock->key->GetFullName());

					HandleUnlock(a_refr, false);
					if (MCM::Settings::AutoPick::bExperienceFromKeys)
					{
						HandleExperience(Lock->GetLockLevel(nullptr));
					}
				}
				else
				{
					detail::ShowMessage("sAutoLockPickKeyOnly", Lock->key->GetFullName());
				}
			}
			else
			{
				detail::ShowMessage("sAutoLockPickNoKey");
			}
		}

		class hkHasObjects
		{
		private:
			static bool HasObjects(
				[[maybe_unused]] RE::Actor*               a_this,
				[[maybe_unused]] RE::TESBoundObject*      a_obj,
				[[maybe_unused]] RE::PACKAGE_OBJECT_TYPE  a_formID,
				[[maybe_unused]] std::int32_t             a_number,
				[[maybe_unused]] std::uint32_t            a_id,
				[[maybe_unused]] RE::PACKAGE_OBJECT_TYPE& a_matchID)
			{
				if (detail::IsExempt())
				{
					return _HasObjects0(a_this, a_obj, a_formID, a_number, a_id, a_matchID);
				}

				if (a_this &&
					a_this->formID != 0x14)
				{
					return _HasObjects0(a_this, a_obj, a_formID, a_number, a_id, a_matchID);
				}

				if (!HasLockpick())
				{
					return _HasObjects0(a_this, a_obj, a_formID, a_number, a_id, a_matchID);
				}

				if (!MCM::Settings::AutoPick::bNoKeyUse)
				{
					return _HasObjects0(a_this, a_obj, a_formID, a_number, a_id, a_matchID);
				}

				return false;
			}

			inline static REL::Hook _HasObjects0{ REL::ID(2198652), 0x111, HasObjects };  // TESObjectCONT::Activate
			inline static REL::Hook _HasObjects1{ REL::ID(2198689), 0x2A7, HasObjects };  // TESObjectDOOR::Activate
		};

		class hkTryUnlockObject
		{
		private:
			static bool TryUnlockObject(
				[[maybe_unused]] RE::PlayerCharacter* a_this,
				[[maybe_unused]] RE::TESObjectREFR*   a_refr,
				[[maybe_unused]] bool                 a_auto)
			{
				if (detail::IsExempt())
				{
					return _TryUnlockObject0(a_this, a_refr, a_auto);
				}

				auto Lock = a_refr->GetLock();
				if (!Lock)
				{
					return false;
				}

				auto LockLevel = Lock->GetLockLevel(nullptr);
				switch (LockLevel)
				{
				case RE::LOCK_LEVEL::kEasy:
				case RE::LOCK_LEVEL::kAverage:
				case RE::LOCK_LEVEL::kHard:
				case RE::LOCK_LEVEL::kVeryHard:
					break;

				case RE::LOCK_LEVEL::kRequiresKey:
					HandleUnlockWithKey(a_refr);
					return false;

				case RE::LOCK_LEVEL::kTerminal:
					detail::ShowMessage("sAutoLockPickTerminal");
					return false;

				case RE::LOCK_LEVEL::kInaccessible:
				case RE::LOCK_LEVEL::kBarred:
				case RE::LOCK_LEVEL::kChained:
					detail::ShowMessage("sAutoLockPickInaccessible");
					return false;

				default:
					return false;
				}

				if (!HasLockpick())
				{
					detail::ShowMessage("sAutoLockPickNoPicks");
					return false;
				}

				if (!CanAutoPick(LockLevel))
				{
					detail::ShowMessage("sAutoLockPickGateFail");
					return false;
				}

				if (!MCM::Settings::AutoPick::bEnableAutoPick)
				{
					RE::LockpickingMenu::OpenLockpickingMenu(a_refr);
					return false;
				}

				if (HandleRoll(LockLevel))
				{
					HandleUnlock(a_refr);
					HandleDetection(a_refr, MCM::Settings::AutoPick::iDetectionEventSuccess);
					HandleExperience(LockLevel);
					HandleWaxKey(Lock->key);
				}
				else
				{
					HandleBreak("UILockpickingPickBreak", "UILockpickingPickMovement");
					HandleDetection(a_refr, MCM::Settings::AutoPick::iDetectionEventFailure);
				}

				if (!MCM::Settings::AutoPick::bNoCrime)
				{
					HandleCrime(a_refr);
				}

				return false;
			}

			inline static REL::Hook _TryUnlockObject0{ REL::ID(2198652), 0x1C0, TryUnlockObject };  // TESObjectCONT::Activate
			inline static REL::Hook _TryUnlockObject1{ REL::ID(2198689), 0x397, TryUnlockObject };  // TESObjectDOOR::Activate
		};
	};
}
