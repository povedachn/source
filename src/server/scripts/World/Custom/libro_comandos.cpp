#include "ScriptPCH.h"

class libro_comandos : public ItemScript
{
public:
libro_comandos() : ItemScript ("libro_comandos") {} 

	bool OnUse(Player * player, Item * item, SpellCastTargets const& targets)
	{
		player->PlayerTalkClass->ClearMenus();
		
		player->ADD_GOSSIP_ITEM(3, "Comando: .arena", GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM(3, "Comando: .challenge", GOSSIP_SENDER_MAIN, 2);
		player->ADD_GOSSIP_ITEM(3, "Comando: .utility", GOSSIP_SENDER_MAIN, 3);
		player->SEND_GOSSIP_MENU(7, item->GetGUID());
		return true;
	}
	
	bool OnGossipSelect(Player* player, Item* item, uint32 sender, uint32 action)
	{
		player->PlayerTalkClass->ClearMenus();
		
		switch(action)
		{
			case 0: // Menú principal
			{
				player->PlayerTalkClass->ClearMenus();
				
				player->ADD_GOSSIP_ITEM(3, "Comando: .arena", GOSSIP_SENDER_MAIN, 1);
				player->ADD_GOSSIP_ITEM(3, "Comando: .challenge", GOSSIP_SENDER_MAIN, 2);
				player->ADD_GOSSIP_ITEM(3, "Comando: .utility", GOSSIP_SENDER_MAIN, 3);
				player->SEND_GOSSIP_MENU(7, item->GetGUID());
				return true;
			}
			
			/************************************ COMANDO .ARENA - INICIO **********************************/
			
			case 1: // Comando: .arena
			{
				player->PlayerTalkClass->ClearMenus();

				player->ADD_GOSSIP_ITEM(3, "Comando: .arena 2v2", GOSSIP_SENDER_MAIN, 4);
				player->ADD_GOSSIP_ITEM(3, "Comando: .arena 3v3", GOSSIP_SENDER_MAIN, 5);
				player->ADD_GOSSIP_ITEM(2, "<-- Volver al Menú Principal", GOSSIP_SENDER_MAIN, 0);
				player->SEND_GOSSIP_MENU(8, item->GetGUID());
				return true;
			} break;
			
			case 4: // Comando: .arena 2v2
			{
				player->PlayerTalkClass->ClearMenus();

				player->ADD_GOSSIP_ITEM(2, "<- Volver", GOSSIP_SENDER_MAIN, 1);
				player->ADD_GOSSIP_ITEM(2, "<-- Volver al Menú Principal", GOSSIP_SENDER_MAIN, 0);
				player->SEND_GOSSIP_MENU(9, item->GetGUID());
				return true;
			} break;
			
			case 5: // Comando: .arena 3v3
			{
				player->PlayerTalkClass->ClearMenus();

				player->ADD_GOSSIP_ITEM(2, "<- Volver", GOSSIP_SENDER_MAIN, 1);
				player->ADD_GOSSIP_ITEM(2, "<-- Volver al Menú Principal", GOSSIP_SENDER_MAIN, 0);
				player->SEND_GOSSIP_MENU(10, item->GetGUID());
				return true;
			} break;
			
			/************************************ COMANDO .ARENA - FIN *************************************/


			/************************************ COMANDO .CHALLENGE - INICIO ******************************/
			
			case 2: // Comando: .challenge
			{
				player->PlayerTalkClass->ClearMenus();

				player->ADD_GOSSIP_ITEM(3, "Comando: .challenge modo", GOSSIP_SENDER_MAIN, 6);
				player->ADD_GOSSIP_ITEM(3, "Comando: .challenge 1c1 nombre_rival", GOSSIP_SENDER_MAIN, 7);
				player->ADD_GOSSIP_ITEM(3, "Comando: .challenge 2c2 nombre_rival1 nombre_rival2", GOSSIP_SENDER_MAIN, 8);
				player->ADD_GOSSIP_ITEM(3, "Comando: .challenge 3c3 nombre_rival1 nombre_rival2 nombre_rival3", GOSSIP_SENDER_MAIN, 9);
				player->ADD_GOSSIP_ITEM(3, "Comando: .challenge aceptar", GOSSIP_SENDER_MAIN, 10);
				player->ADD_GOSSIP_ITEM(3, "Comando: .challenge on", GOSSIP_SENDER_MAIN, 11);
				player->ADD_GOSSIP_ITEM(3, "Comando: .challenge off", GOSSIP_SENDER_MAIN, 12);
				player->ADD_GOSSIP_ITEM(2, "<-- Volver al Menú Principal", GOSSIP_SENDER_MAIN, 0);
				player->SEND_GOSSIP_MENU(8, item->GetGUID());
				return true;
			} break;
			
			case 6: // Comando: .challenge modo
			{
				player->PlayerTalkClass->ClearMenus();

				player->ADD_GOSSIP_ITEM(2, "<- Volver", GOSSIP_SENDER_MAIN, 2);
				player->ADD_GOSSIP_ITEM(2, "<-- Volver al Menú Principal", GOSSIP_SENDER_MAIN, 0);
				player->SEND_GOSSIP_MENU(11, item->GetGUID());
				return true;
			} break;
			
			case 7: // Comando: .challenge 1c1 nombre_rival
			{
				player->PlayerTalkClass->ClearMenus();

				player->ADD_GOSSIP_ITEM(2, "<- Volver", GOSSIP_SENDER_MAIN, 2);
				player->ADD_GOSSIP_ITEM(2, "<-- Volver al Menú Principal", GOSSIP_SENDER_MAIN, 0);
				player->SEND_GOSSIP_MENU(12, item->GetGUID());
				return true;
			} break;
			
			case 8: // Comando: .challenge 2c2 nombre_rival1 nombre_rival2
			{
				player->PlayerTalkClass->ClearMenus();

				player->ADD_GOSSIP_ITEM(2, "<- Volver", GOSSIP_SENDER_MAIN, 2);
				player->ADD_GOSSIP_ITEM(2, "<-- Volver al Menú Principal", GOSSIP_SENDER_MAIN, 0);
				player->SEND_GOSSIP_MENU(13, item->GetGUID());
				return true;
			} break;
			
			case 9: // Comando: .challenge 3c3 nombre_rival1 nombre_rival2 nombre_rival3
			{
				player->PlayerTalkClass->ClearMenus();

				player->ADD_GOSSIP_ITEM(2, "<- Volver", GOSSIP_SENDER_MAIN, 2);
				player->ADD_GOSSIP_ITEM(2, "<-- Volver al Menú Principal", GOSSIP_SENDER_MAIN, 0);
				player->SEND_GOSSIP_MENU(14, item->GetGUID());
				return true;
			} break;
			
			case 10: // Comando: .challenge aceptar
			{
				player->PlayerTalkClass->ClearMenus();

				player->ADD_GOSSIP_ITEM(2, "<- Volver", GOSSIP_SENDER_MAIN, 2);
				player->ADD_GOSSIP_ITEM(2, "<-- Volver al Menú Principal", GOSSIP_SENDER_MAIN, 0);
				player->SEND_GOSSIP_MENU(15, item->GetGUID());
				return true;
			} break;
			
			case 11: // Comando: .challenge on
			{
				player->PlayerTalkClass->ClearMenus();

				player->ADD_GOSSIP_ITEM(2, "<- Volver", GOSSIP_SENDER_MAIN, 2);
				player->ADD_GOSSIP_ITEM(2, "<-- Volver al Menú Principal", GOSSIP_SENDER_MAIN, 0);
				player->SEND_GOSSIP_MENU(16, item->GetGUID());
				return true;
			} break;
			
			case 12: // Comando: .challenge off
			{
				player->PlayerTalkClass->ClearMenus();

				player->ADD_GOSSIP_ITEM(2, "<- Volver", GOSSIP_SENDER_MAIN, 2);
				player->ADD_GOSSIP_ITEM(2, "<-- Volver al Menú Principal", GOSSIP_SENDER_MAIN, 0);
				player->SEND_GOSSIP_MENU(17, item->GetGUID());
				return true;
			} break;
			
			/************************************ COMANDO .CHALLENGE - FIN *********************************/
			
			/************************************ COMANDO .UTILITY - INICIO ********************************/
			
			case 3: // Comando: .utility
			{
				player->PlayerTalkClass->ClearMenus();

				player->ADD_GOSSIP_ITEM(3, "Comando: .utility race", GOSSIP_SENDER_MAIN, 13);
				player->ADD_GOSSIP_ITEM(3, "Comando: .utility faction", GOSSIP_SENDER_MAIN, 14);
				player->ADD_GOSSIP_ITEM(3, "Comando: .utility customize", GOSSIP_SENDER_MAIN, 15);
				player->ADD_GOSSIP_ITEM(3, "Comando: .utility mmr", GOSSIP_SENDER_MAIN, 16);
				player->ADD_GOSSIP_ITEM(2, "<-- Volver al Menú Principal", GOSSIP_SENDER_MAIN, 0);
				player->SEND_GOSSIP_MENU(8, item->GetGUID());
				return true;
			} break;
			
			case 13: // Comando: .utility race
			{
				player->PlayerTalkClass->ClearMenus();

				player->ADD_GOSSIP_ITEM(2, "<- Volver", GOSSIP_SENDER_MAIN, 3);
				player->ADD_GOSSIP_ITEM(2, "<-- Volver al Menú Principal", GOSSIP_SENDER_MAIN, 0);
				player->SEND_GOSSIP_MENU(18, item->GetGUID());
				return true;
			} break;
			
			case 14: // Comando: .utility faction
			{
				player->PlayerTalkClass->ClearMenus();

				player->ADD_GOSSIP_ITEM(2, "<- Volver", GOSSIP_SENDER_MAIN, 3);
				player->ADD_GOSSIP_ITEM(2, "<-- Volver al Menú Principal", GOSSIP_SENDER_MAIN, 0);
				player->SEND_GOSSIP_MENU(19, item->GetGUID());
				return true;
			} break;
			
			case 15: // Comando: .utility customize
			{
				player->PlayerTalkClass->ClearMenus();

				player->ADD_GOSSIP_ITEM(2, "<- Volver", GOSSIP_SENDER_MAIN, 3);
				player->ADD_GOSSIP_ITEM(2, "<-- Volver al Menú Principal", GOSSIP_SENDER_MAIN, 0);
				player->SEND_GOSSIP_MENU(20, item->GetGUID());
				return true;
			} break;
			
			case 16: // Comando: .utility mmr
			{
				player->PlayerTalkClass->ClearMenus();

				player->ADD_GOSSIP_ITEM(2, "<- Volver", GOSSIP_SENDER_MAIN, 3);
				player->ADD_GOSSIP_ITEM(2, "<-- Volver al Menú Principal", GOSSIP_SENDER_MAIN, 0);
				player->SEND_GOSSIP_MENU(21, item->GetGUID());
				return true;
			} break;
			
			/************************************ COMANDO .UTILITY - FIN ***********************************/
		}
	}
};

void AddSC_libro_comandos()
{
	new libro_comandos();
}