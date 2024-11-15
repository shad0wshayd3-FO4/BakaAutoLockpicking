#include "Forms/Forms.h"
#include "Hooks/Hooks.h"
#include "Scripts/Papyrus.h"

namespace
{
	void MessageHandler(F4SE::MessagingInterface::Message* a_msg)
	{
		switch (a_msg->type)
		{
		case F4SE::MessagingInterface::kPostLoad:
		{
			Hooks::BakaAutoHack::Install();
			Hooks::BakaAutoLock::Install();
			break;
		}
		case F4SE::MessagingInterface::kInputLoaded:
		{
			MCM::Settings::Load(true);
			break;
		}
		case F4SE::MessagingInterface::kGameDataReady:
		{
			if (static_cast<bool>(a_msg->data))
			{
				Forms::Register();
			}
			break;
		}
		default:
			break;
		}
	}
}

F4SEPluginLoad(const F4SE::LoadInterface* a_F4SE)
{
	F4SE::Init(a_F4SE);
	F4SE::AllocTrampoline(256);

	F4SE::GetMessagingInterface()->RegisterListener(MessageHandler);
	F4SE::GetPapyrusInterface()->Register(Papyrus::BakaAutoLock::Register);

	return true;
}
