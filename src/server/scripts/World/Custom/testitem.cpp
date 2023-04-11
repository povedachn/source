#include "ScriptPCH.h"
#include "Spell.h"

class item_teleport : public ItemScript
{
    public:
		 item_teleport() : ItemScript("item_teleport") {}

		bool OnUse(Player * player, Item * item, SpellCastTargets const& targets)
		 {
			player->PlayerTalkClass->ClearMenus();
			player->ADD_GOSSIP_ITEM(6, "establos", GOSSIP_SENDER_MAIN, 1);
			player->ADD_GOSSIP_ITEM(6, "vendedor", GOSSIP_SENDER_MAIN, 2);
			//player->ADD_GOSSIP_ITEM(6, "Compra - 3", GOSSIP_SENDER_MAIN, 3);
			player->ADD_GOSSIP_ITEM(6, "submenus", GOSSIP_SENDER_MAIN, 4);
			player->SEND_GOSSIP_MENU(1, item->GetGUID());
			return true;
		 }
};

class item_teleport2 : public PlayerScript
{
    public:
		 item_teleport2()
			 : PlayerScript("item_teleport2")
		 {
		 }

		void OnGossipSelect(Player* player, Item* item, uint32 sender, uint32 action)
		 {
			player->PlayerTalkClass->ClearMenus();
			switch (action)
				{
					case 1:
						player->GetSession()->SendStablePet(1337357);
					break;

					case 2:
						player->GetSession()->SendListInventory(MAKE_NEW_GUID(500101, 1337432, HIGHGUID_UNIT));				
					break;

					case 3:
						player->GetSession()->SendListInventory(MAKE_NEW_GUID(1337432, 1337432, HIGHGUID_UNIT));				
					break;
					
					case 4:
						player->ADD_GOSSIP_ITEM(6, "Compra - 5", GOSSIP_SENDER_MAIN, 1);
						player->ADD_GOSSIP_ITEM(6, "Compra - 6", GOSSIP_SENDER_MAIN, 2);
						player->ADD_GOSSIP_ITEM(6, "Compra - 7", GOSSIP_SENDER_MAIN, 3);
						player->ADD_GOSSIP_ITEM(6, "Compra - 8", GOSSIP_SENDER_MAIN, 4);				
					break;
				}
			return;
		 }
};

void AddSC_item_teleport()
{
   new item_teleport();
}