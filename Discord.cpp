#include "DiscordHandler.hpp"
#include "Discord_Register.h"
#include "Discord_RPC.h"
#include <chrono>
#include "./Common.hpp"
#include "./GTA/Invoker/Natives.hpp"
#pragma comment(lib, "discord-rpc.lib")

using namespace Chim;

void discordhandler::Init() {
	DiscordEventHandlers handlers;
	std::memset(&handlers, 0, sizeof(handlers));
	Discord_Initialize("922913066855841852", &handlers, 1, "0");
}

void discordhandler::Tick() {
Optimized:

	static int64_t StartTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	DiscordRichPresence discordPresence;
	std::memset(&discordPresence, 0, sizeof(discordPresence));
	if (NETWORK::NETWORK_IS_SESSION_STARTED()) {
		discordPresence.state = "Online";
		discordPresence.partySize = NETWORK::NETWORK_GET_NUM_CONNECTED_PLAYERS();
		discordPresence.partyMax = 32;
	}
	else {
		discordPresence.state = "Story Mode";
	}
	discordPresence.details = "Modding With Doraemon Menu";
	discordPresence.startTimestamp = StartTime;
	discordPresence.largeImageKey = "minidora";
	discordPresence.largeImageText = "Doraemon Mod Menu";
	discordPresence.button1name = "Soon";
	discordPresence.button1link = "https://www.youtube.com/watch?v=fmVz2U1oI8o&list=PLzX09b9kZsrt-eygvL3Zcl2LMxz-9W4ga";
	discordPresence.instance = 1;

	Discord_UpdatePresence(&discordPresence);
}

void discordhandler::Shutdown() {
	Discord_ClearPresence();
	Discord_Shutdown();
}