#pragma once
#include "Discord_Register.h"
#include "Discord_RPC.h"

namespace Chim {
    class discordhandler {
    public:
        void Init();
        void Tick();
        void Shutdown();
    };

    inline std::unique_ptr<discordhandler> g_Discord;
}