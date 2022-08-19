#pragma once

#include "Hooks/Hooks.h"
#include "MCM/MCM.h"

namespace Papyrus
{
	class BakaAutoLock
	{
	public:
		static bool Register(RE::BSScript::IVirtualMachine* a_vm)
		{
			a_vm->BindNativeMethod(CLASS_NAME, "GetVersion", GetVersion, true);
			a_vm->BindNativeMethod(CLASS_NAME, "ShowRollModifiers", ShowRollModifiers);
			a_vm->BindNativeMethod(CLASS_NAME, "UpdateSettings", UpdateSettings);
			logger::info(FMT_STRING("Registered funcs for class {:s}"sv), CLASS_NAME);

			return true;
		}

	private:
		static constexpr char CLASS_NAME[] = "BakaAutoLock";

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

		static void UpdateSettings(std::monostate)
		{
			MCM::Settings::Update();
		}
	};
}
