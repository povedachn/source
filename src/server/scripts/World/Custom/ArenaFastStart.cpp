#include "ScriptPCH.h"
#include "Player.h"
#include "Battleground.h"
#include "BattlegroundMgr.h"
#include <sstream>

class FastArenaCrystal : public GameObjectScript
{
    public:

        FastArenaCrystal()
            : GameObjectScript("FastArenaCrystal")
        {
        }

        bool OnGossipHello(Player* player, GameObject* go)
        {
     		 if (player->isSpectator())
      		{
                ChatHandler(player).PSendSysMessage("No puedes hacer eso siendo un espectador.");
                return false;
    	       }

            if (Battleground *pBG = player->GetBattleground())
                if (pBG->isArena())
                    ChatHandler(player).PSendSysMessage("Jugadores que han clickeado: %u", pBG->ClickFastStart(player, go));    

            return false;
        }
};

void AddSC_fast_arena_start()
{
    new FastArenaCrystal();
}