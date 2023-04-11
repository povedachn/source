#include "ScriptPCH.h"

class Anuncio_boss : public PlayerScript
{
public:
Anuncio_boss() : PlayerScript ("Anuncio_boss") {} 

	void OnCreatureKill(Player* player, Creature* boss) 
	{
		if (boss->GetEntry() == 100000 || boss->GetEntry() == 100005 || boss->GetEntry() == 4949 || boss->GetEntry() == 29611)
		{
			char msg[250];
	
			if(player->GetGroup())
			{
				sprintf(msg,"|CFF7BBEF7[Anuncio de Boss]|r:|cffff0000 %s|r y su grupo han matado a |CFF18BE00[%s]|r !!!", player->GetName(),boss->GetName());
			}
			else
			{
				sprintf(msg,"|CFF7BBEF7[Anuncio de Boss]|r:|cffff0000 %s|r ha matado a |CFF18BE00[%s]|r !!!", player->GetName(),boss->GetName());
			}
		sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
		}
	}
};

void AddSC_Anuncio_boss()
{
	new Anuncio_boss();
}