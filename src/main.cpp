#include "Forms/Forms.h"
#include "Hooks/Hooks.h"

namespace
{
	void MessageCallback(F4SE::MessagingInterface::Message* a_msg)
	{
		switch (a_msg->type)
		{
		case F4SE::MessagingInterface::kGameDataReady:
			Forms::Load();
			break;
		default:
			break;
		}
	}
}

F4SE_PLUGIN_LOAD(const F4SE::LoadInterface* a_f4se)
{
	F4SE::Init(a_f4se, { .trampoline = true, .trampolineSize = 32 });
	F4SE::GetMessagingInterface()->RegisterListener(MessageCallback);
	return true;
}
