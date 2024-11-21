#pragma once

#include "Hooks/Hooks.h"
#include "MCM/MCM.h"

namespace Papyrus::BakaAutoLock
{
	static constexpr auto CLASS_NAME{ "BakaAutoLock"sv };

	enum
	{
		kVersion = 1
	};

	static std::int32_t GetVersion(std::monostate)
	{
		return kVersion;
	}

	static void ShowRollModifiers(std::monostate)
	{
		Hooks::BakaAutoLock::ShowRollModifiers();
	}

	static void ShowRollModifiersHack(std::monostate)
	{
		Hooks::BakaAutoHack::ShowRollModifiers();
	}

	static void UpdateSettings(std::monostate)
	{
		MCM::Settings::Load(false);
	}

	static bool Register(RE::BSScript::IVirtualMachine* a_vm)
	{
		a_vm->BindNativeMethod(CLASS_NAME, "GetVersion", GetVersion, true);
		a_vm->BindNativeMethod(CLASS_NAME, "ShowRollModifiers", ShowRollModifiers);
		a_vm->BindNativeMethod(CLASS_NAME, "ShowRollModifiersHack", ShowRollModifiersHack);
		a_vm->BindNativeMethod(CLASS_NAME, "UpdateSettings", UpdateSettings);
		F4SE::log::info("Registered funcs for class {}"sv, CLASS_NAME);

		return true;
	}
}
