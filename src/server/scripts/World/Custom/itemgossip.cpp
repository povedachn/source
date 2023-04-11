#include "ScriptPCH.h"
#include "Group.h"
#include "Battleground.h"
#include "GroupMgr.h"
#include "Chat.h"
#include "BattlegroundMgr.h"
#include "WorldPacket.h"
#include "CreatureTextMgr.h"
#include "ArenaTeam.h"
#include "ArenaTeamMgr.h"
#include "Player.h"
#include "Transmogrification.h"

#include "itemgossiphelp.h"

#define sT  sTransmogrification
#define GTS session->GetTrinityString

int Evento_Escalada_Teleport_item()
{
	QueryResult evento_escalada_item = WorldDatabase.PQuery("SELECT Habilitado_Teleport FROM eventos_custom WHERE id = 1");

	if (!evento_escalada_item)
		return 0;
	else
	{
		Field* fields = evento_escalada_item->Fetch();
			return fields[0].GetUInt32();
	}
}

int Evento_Escalada_Teleport_laberinto_item_item()
{
	QueryResult evento_escalada_laberinto_item = WorldDatabase.PQuery("SELECT Habilitado_Teleport_laberinto FROM eventos_custom WHERE id = 1");

	if (!evento_escalada_laberinto_item)
		return 0;
	else
	{
		Field* fields = evento_escalada_laberinto_item->Fetch();
			return fields[0].GetUInt32();
	}
}

int Evento_Laberinto1_item()
{
	QueryResult evento_laberinto1_item = WorldDatabase.PQuery("SELECT evento_laberinto1 FROM eventos_custom WHERE id = 1");

	if (!evento_laberinto1_item)
		return 0;
	else
	{
		Field* fields = evento_laberinto1_item->Fetch();
			return fields[0].GetUInt32();
	}
}

int Evento_Escalada1_item()
{
	QueryResult evento_escalada1_item = WorldDatabase.PQuery("SELECT evento_escalada1 FROM eventos_custom WHERE id = 1");

	if (!evento_escalada1_item)
		return 0;
	else
	{
		Field* fields = evento_escalada1_item->Fetch();
			return fields[0].GetUInt32();
	}
}

int Evento_Escalada2_item()
{
	QueryResult evento_escalada2_item = WorldDatabase.PQuery("SELECT evento_escalada2 FROM eventos_custom WHERE id = 1");

	if (!evento_escalada2_item)
		return 0;
	else
	{
		Field* fields = evento_escalada2_item->Fetch();
			return fields[0].GetUInt32();
	}
}

class itemgossip : public ItemScript
{
    public:
		 itemgossip() : ItemScript("itemgossip") {}

		void ShowTransmogItems(Player* player, Item* item, uint8 slot) // Only checks bags while can use an item from anywhere in inventory
		{
			WorldSession* session = player->GetSession();
			Item* oldItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
			if (oldItem)
			{
				uint32 limit = 0;
				uint32 price = sT->GetSpecialPrice(oldItem->GetTemplate());
				price *= sT->GetScaledCostModifier();
				price += sT->GetCopperCost();
				std::ostringstream ss;
				ss << std::endl;
				if (sT->GetRequireToken())
					ss << std::endl << std::endl << sT->GetTokenAmount() << " x " << sT->GetItemLink(sT->GetTokenEntry(), session);

				for (uint8 i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; ++i)
				{
					if (limit > MAX_OPTIONS)
						break;
					Item* newItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i);
					if (!newItem)
						continue;
					if (!sT->CanTransmogrifyItemWithItem(player, oldItem->GetTemplate(), newItem->GetTemplate()))
						continue;
					if (sT->GetFakeEntry(oldItem->GetGUID()) == newItem->GetEntry())
						continue;
					++limit;
					player->ADD_GOSSIP_ITEM_EXTENDED(1, sT->GetItemIcon(newItem->GetEntry(), 30, 30, -15, 0)+sT->GetItemLink(newItem, session), slot, newItem->GetGUIDLow(), "Usar este objeto para transfigurar har""\xC3\xA1"" que se te lige y no puedas comerciar con el.\n""\xc2\xbf""Realmente deseas continuar?\n\n"+sT->GetItemIcon(newItem->GetEntry(), 40, 40, -15, -10)+sT->GetItemLink(newItem, session)+ss.str(), price, false);
				}

				for (uint8 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; ++i)
				{
					Bag* bag = player->GetBagByPos(i);
					if (!bag)
						continue;
					for (uint32 j = 0; j < bag->GetBagSize(); ++j)
					{
						if (limit > MAX_OPTIONS)
							break;
						Item* newItem = player->GetItemByPos(i, j);
						if (!newItem)
							continue;
						if (!sT->CanTransmogrifyItemWithItem(player, oldItem->GetTemplate(), newItem->GetTemplate()))
							continue;
						if (sT->GetFakeEntry(oldItem->GetGUID()) == newItem->GetEntry())
							continue;
						++limit;
						player->ADD_GOSSIP_ITEM_EXTENDED(1, sT->GetItemIcon(newItem->GetEntry(), 30, 30, -15, 0)+sT->GetItemLink(newItem, session), slot, newItem->GetGUIDLow(), "Usar este objeto para transfigurar har""\xC3\xA1"" que se te lige y no puedas comerciar con el.\n""\xc2\xbf""Realmente deseas continuar?\n\n"+sT->GetItemIcon(newItem->GetEntry(), 40, 40, -15, -10)+sT->GetItemLink(newItem, session)+ss.str(), price, false);
					}
				}
			}

			player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface/PaperDollInfoFrame/UI-GearManager-LeaveItem-Opaque:30:30:-15:0|tRemover transfiguraci""\xC3\xB3""n", EQUIPMENT_SLOT_END+3, slot, "Deseas remover la transfiguraci""\xC3\xB3""n de la ranura?", 0, false);
			player->ADD_GOSSIP_ITEM(1, "|TInterface/PaperDollInfoFrame/UI-GearManager-Undo:30:30:-15:0|tActualizar Men""\xC3\xBA""", EQUIPMENT_SLOT_END, slot);
			player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/Ability_Spy:30:30:-15:0|tVolver", EQUIPMENT_SLOT_END+1, 0);
			player->SEND_GOSSIP_MENU(1, item->GetGUID());
		}
		
		bool MenuPrincipalItemGossip(Player* player, Item* item)
		{
			player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Feather_06:30:30:-15:0|tTaxi", GOSSIP_SENDER_MAIN, 800);
			player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Armor_Helm_Plate_Naxxramas_RaidWarrior_C_01:30:30:-15:0|tTrainers & Vendors S7", GOSSIP_SENDER_MAIN, 801);
			player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Misc_Bag_17:30:30:-15:0|tItem Vendors", GOSSIP_SENDER_MAIN, 802);
			player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_spyglass_03:30:30:-15:0|tArena Utilities", GOSSIP_SENDER_MAIN, 803);
			player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/trade_engraving:30:30:-15:0|tUtility NPCs", GOSSIP_SENDER_MAIN, 804);

			player->SEND_GOSSIP_MENU(1, item->GetGUID());
			return true;
		}

		bool OnUse(Player * player, Item * item, SpellCastTargets const& targets)
		{
			player->PlayerTalkClass->ClearMenus();
			if (player->isInCombat())
			{
				ChatHandler(player).PSendSysMessage(LANG_YOU_IN_COMBAT);
				return true;
			}

			if(player->GetMap()->IsBattlegroundOrArena() || player->HasStealthAura() || player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || player->isDead())
			{
				ChatHandler(player).PSendSysMessage("No puedes hacer eso estando en BG, Arena o estando en sigilo.");
				return true;
			}

			if (player->GetZoneId() == 14)
			{
				ChatHandler(player).PSendSysMessage("No puedes hacer eso estando en la Zona PvP.");
				return true;
			}

			player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Feather_06:30:30:-15:0|tTaxi", GOSSIP_SENDER_MAIN, 800);
			player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Armor_Helm_Plate_Naxxramas_RaidWarrior_C_01:30:30:-15:0|tTrainers & Vendors S7", GOSSIP_SENDER_MAIN, 801);
			player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Misc_Bag_17:30:30:-15:0|tItem Vendors", GOSSIP_SENDER_MAIN, 802);
			player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_spyglass_03:30:30:-15:0|tArena Utilities", GOSSIP_SENDER_MAIN, 803);
			player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/trade_engraving:30:30:-15:0|tUtility NPCs", GOSSIP_SENDER_MAIN, 804);

			player->SEND_GOSSIP_MENU(1, item->GetGUID());
			return true;
		}

		bool OnGossipSelect(Player* player, Item* item, uint32 sender, uint32 action)
		{
			WorldSession* session = player->GetSession();
			player->PlayerTalkClass->ClearMenus();

			if (player->isInCombat())
			{
				ChatHandler(player).PSendSysMessage(LANG_YOU_IN_COMBAT);
				player->CLOSE_GOSSIP_MENU();
				return true;
			}

			if(player->GetMap()->IsBattlegroundOrArena() || player->HasStealthAura() || player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || player->isDead())
			{
				ChatHandler(player).PSendSysMessage("No puedes hacer eso estando en BG, Arena o estando en sigilo.");
				player->CLOSE_GOSSIP_MENU();
				return true;
			}

			if (player->GetZoneId() == 14)
			{
				ChatHandler(player).PSendSysMessage("No puedes hacer eso estando en la Zona PvP.");
				player->CLOSE_GOSSIP_MENU();
				return true;
			}

			switch (sender)
			{
					player->PlayerTalkClass->ClearMenus();
					case EQUIPMENT_SLOT_END: // Show items you can use
					{
						ShowTransmogItems(player, item, action);
						return true;
					} break;

					case EQUIPMENT_SLOT_END+1: // Main menu
					{
						MenuPrincipalTransfigurador(player, item);
						return true;
					} break;

					case EQUIPMENT_SLOT_END+2: // Remove Transmogrifications
					{
						bool removed = false;
						SQLTransaction trans = CharacterDatabase.BeginTransaction();
						for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; ++slot)
						{
							if (Item* newItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
							{
								if (!sT->GetFakeEntry(newItem->GetGUID()))
									continue;
								sT->DeleteFakeEntry(player, slot, newItem, &trans);
								removed = true;
							}
						}
						if (removed)
						{
							session->SendAreaTriggerMessage(GTS(LANG_ERR_UNTRANSMOG_OK));
							CharacterDatabase.CommitTransaction(trans);
						}
						else
							session->SendNotification(LANG_ERR_UNTRANSMOG_NO_TRANSMOGS);
						MenuPrincipalTransfigurador(player, item);
						return true;
					} break;

					case EQUIPMENT_SLOT_END+3: // Remove Transmogrification from single item
					{
						if (Item* newItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, action))
						{
							if (sT->GetFakeEntry(newItem->GetGUID()))
							{
								sT->DeleteFakeEntry(player, action, newItem);
								session->SendAreaTriggerMessage(GTS(LANG_ERR_UNTRANSMOG_OK));
							}
							else
								session->SendNotification(LANG_ERR_UNTRANSMOG_NO_TRANSMOGS);
						}
						OnGossipSelect(player, item, EQUIPMENT_SLOT_END, action);
						return true;
					} break;

				#ifdef PRESETS

					case EQUIPMENT_SLOT_END+4: // Presets menu
					{
						if (!sT->GetEnableSets())
						{
							MenuPrincipalTransfigurador(player, item);
							return true;
						}
						if (sT->GetEnableSetInfo())
							player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Misc_Book_11:30:30:-15:0|tExplicaci""\xC3\xB3""n de los Sets", EQUIPMENT_SLOT_END+10, 0);
						for (Transmogrification::presetIdMap::const_iterator it = sT->presetByName[player->GetGUID()].begin(); it != sT->presetByName[player->GetGUID()].end(); ++it)
							player->ADD_GOSSIP_ITEM(1, "|TInterface/RAIDFRAME/UI-RAIDFRAME-MAINASSIST:30:30:-15:0|t"+it->second, EQUIPMENT_SLOT_END+6, it->first);

						if (sT->presetByName[player->GetGUID()].size() < sT->GetMaxSets())
							player->ADD_GOSSIP_ITEM(1, "|TInterface/GuildBankFrame/UI-GuildBankFrame-NewTab:30:30:-15:0|tGuardar Set", EQUIPMENT_SLOT_END+8, 0);
						player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/Ability_Spy:30:30:-15:0|tVolver", EQUIPMENT_SLOT_END+1, 0);
						player->SEND_GOSSIP_MENU(1, item->GetGUID());
						return true;
					} break;

					case EQUIPMENT_SLOT_END+5: // Use preset
					{
						if (!sT->GetEnableSets())
						{
							MenuPrincipalTransfigurador(player, item);
							return true;
						}
						// action = presetID
						for (Transmogrification::slotMap::const_iterator it = sT->presetById[player->GetGUID()][action].begin(); it != sT->presetById[player->GetGUID()][action].end(); ++it)
						{
							if (Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, it->first))
								sT->PresetTransmog(player, item, it->second, it->first);
						}
						OnGossipSelect(player, item, EQUIPMENT_SLOT_END+4, action);
						return true;
					} break;

					case EQUIPMENT_SLOT_END+6: // view preset
					{
						if (!sT->GetEnableSets())
						{
							MenuPrincipalTransfigurador(player, item);
							return true;
						}
						// action = presetID
						for (Transmogrification::slotMap::const_iterator it = sT->presetById[player->GetGUID()][action].begin(); it != sT->presetById[player->GetGUID()][action].end(); ++it)
							player->ADD_GOSSIP_ITEM(1, sT->GetItemIcon(it->second, 30, 30, -15, 0)+sT->GetItemLink(it->second, session), sender, action);

						player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface/PaperDollInfoFrame/UI-GearManager-ItemIntoBag:30:30:-15:0|tUsar Set", EQUIPMENT_SLOT_END+5, action, "Usar este Set para transfigurar har""\xC3\xA1"" que los objetos se te ligen y no puedas comerciar con ellos.\n""\xc2\xbf""Realmente deseas continuar?\n\n"+sT->presetByName[player->GetGUID()][action], 0, false);
						player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface/PaperDollInfoFrame/UI-GearManager-LeaveItem-Opaque:30:30:-15:0|tBorrar Set", EQUIPMENT_SLOT_END+7, action, """\xc2\xbf""Est""\xC3\xA1""s seguro de querer borrar "+sT->presetByName[player->GetGUID()][action]+"?", 0, false);
						player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/Ability_Spy:30:30:-15:0|tVolver", EQUIPMENT_SLOT_END+4, 0);

						player->SEND_GOSSIP_MENU(1, item->GetGUID());
						return true;
					} break;

					case EQUIPMENT_SLOT_END+7: // Delete preset
					{
						if (!sT->GetEnableSets())
						{
							MenuPrincipalTransfigurador(player, item);
							return true;
						}
						// action = presetID
						CharacterDatabase.PExecute("DELETE FROM `custom_transmogrification_sets` WHERE Owner = %u AND PresetID = %u",  player->GetGUIDLow(), action);
						sT->presetById[player->GetGUID()][action].clear();
						sT->presetById[player->GetGUID()].erase(action);
						sT->presetByName[player->GetGUID()].erase(action);

						OnGossipSelect(player, item, EQUIPMENT_SLOT_END+4, 0);
						return true;
					} break;

					case EQUIPMENT_SLOT_END+8: // Save preset
					{
						if (!sT->GetEnableSets() || sT->presetByName[player->GetGUID()].size() >= sT->GetMaxSets())
						{
							MenuPrincipalTransfigurador(player, item);
							return true;
						}
						uint32 cost = 0;
						bool canSave = false;
						for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; ++slot)
						{
							if (!sT->GetSlotName(slot, session))
								continue;
							if (Item* newItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
							{
								uint32 entry = sT->GetFakeEntry(newItem->GetGUID());
								if (!entry)
									continue;
								const ItemTemplate* temp = sObjectMgr->GetItemTemplate(entry);
								if (!temp)
									continue;
								if (!sT->SuitableForTransmogrification(player, temp)) // no need to check?
									continue;
								cost += sT->GetSpecialPrice(temp);
								canSave = true;
								player->ADD_GOSSIP_ITEM(1, sT->GetItemIcon(entry, 30, 30, -15, 0)+sT->GetItemLink(entry, session), EQUIPMENT_SLOT_END+8, 0);
							}
						}
						if (canSave)
							player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface/GuildBankFrame/UI-GuildBankFrame-NewTab:30:30:-15:0|tGuardar Set", EQUIPMENT_SLOT_END+4, 0, "Pon un nombre a tu Set", cost*sT->GetSetCostModifier()+sT->GetSetCopperCost(), true);
						player->ADD_GOSSIP_ITEM(1, "|TInterface/PaperDollInfoFrame/UI-GearManager-Undo:30:30:-15:0|tActualizar Men""\xC3\xBA""", sender, action);
						player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/Ability_Spy:30:30:-15:0|tVolver", EQUIPMENT_SLOT_END+4, 0);
						player->SEND_GOSSIP_MENU(1, item->GetGUID());
						return true;
					} break;

					case EQUIPMENT_SLOT_END+10: // Set info
					{
						player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/Inv_misc_map02:30:30:-15:0|tVolver al Menú Principal", EQUIPMENT_SLOT_END+1, 0);
						player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/Ability_Spy:30:30:-15:0|tVolver", EQUIPMENT_SLOT_END+4, 0);
						player->SEND_GOSSIP_MENU(50001, item->GetGUID());
						return true;
					} break;
				#endif

					case EQUIPMENT_SLOT_END+9: // Transmog info
					{
						player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/Inv_misc_map02:30:30:-15:0|tVolver al Menú Principal", EQUIPMENT_SLOT_END+1, 0);
						player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/Ability_Spy:30:30:-15:0|tOlv""\xC3\xAD""dalo", EQUIPMENT_SLOT_END+11, 0);
						player->SEND_GOSSIP_MENU(50000, item->GetGUID());
						return true;
					} break;

					case EQUIPMENT_SLOT_END+11: // Menu Olvidalo
					{
						player->CLOSE_GOSSIP_MENU();
						return true;
					} break;

					default: // Transmogrify
					{
						if (!sender && !action)
						{
							MenuPrincipalTransfigurador(player, item);
							return true;
						}
						// sender = slot, action = display
						TransmogTrinityStrings res = sT->Transmogrify(player, MAKE_NEW_GUID(action, 0, HIGHGUID_ITEM), sender);
						if (res == LANG_ERR_TRANSMOG_OK)
						{
							session->SendAreaTriggerMessage(GTS(LANG_ERR_TRANSMOG_OK));
							MenuPrincipalTransfigurador(player, item);
						}
						/*else
							session->SendNotification(res);*/
						// OnGossipSelect(player, creature, EQUIPMENT_SLOT_END, sender);
						// ShowTransmogItems(player, creature, sender);
						//player->CLOSE_GOSSIP_MENU(); // Wait for SetMoney to get fixed, issue #10053
					} break;
			}

			switch (action)
			{
				case 0: // Envía al Menú principal
				{
					MenuPrincipalItemGossip(player, item);
					return true;
				} break;

				case 5150:
				{
					MenuPrincipalTransfigurador(player, item);
					return true;
				} break;

				/*case 5150:
				{
					player->ADD_GOSSIP_ITEM(5, "------------------------------------", GOSSIP_SENDER_MAIN, 0);
					player->ADD_GOSSIP_ITEM(7, "Por diversos motivos, no se ha podido implementar este NPC en el Libro Mágico, deberéis transfigurar vuestros objetos en el Transfigurador el cual se encuentra en el Shop.", GOSSIP_SENDER_MAIN, 0);
					player->ADD_GOSSIP_ITEM(5, "------------------------------------", GOSSIP_SENDER_MAIN, 0);
					player->SEND_GOSSIP_MENU(1, item->GetGUID());
					return true;
				} break;*/

				/////////////////////////////////////////////// PRINCIPIO - TAXI ///////////////////////////////////////////////

				case 800: // Taxi - Accion
				{
					player->PlayerTalkClass->ClearMenus();

					// Si esta volando se para
					if (player->isInFlight())
					{
						player->GetMotionMaster()->MovementExpired();
						player->CleanupAfterTaxiFlight();
					}
					// Solo se guarda si no esta volando
					else
						player->SaveRecallPosition();

					Group* grupo = player->GetGroup();

					std::ostringstream ss;

					ss << "|TInterface/ICONS/INV_BannerPVP_03:30:30:-15:0|t|cff9b0000Zona PvP"/* (Hay " << Jugadores_item() << (Jugadores_item() != 1 ? " jugadores" : " jugador") << " dentro)"*/;

					player->PlayerTalkClass->ClearMenus();
					player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Misc_Bag_Soulbag:30:30:-15:0|t|cff2e2efeShops", GOSSIP_SENDER_MAIN, 200);
					player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface/ICONS/Achievement_WorldEvent_Reindeer:30:30:-15:0|t|cff8410a4Shop Monturas Custom", GOSSIP_SENDER_MAIN, 201, "|CFFFFFF01""\xc2\xbf""Est""\xC3\xA1""s seguro de querer ir a la Shop Monturas Custom?|R", 0, false);
					player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface/RAIDFRAME/UI-RAIDFRAME-MAINASSIST:30:30:-15:0|tZona de Duelos", GOSSIP_SENDER_MAIN, 202, "|CFFFFFF01""\xc2\xbf""Est""\xC3\xA1""s seguro de querer ir a la Zona de Duelos?|R", 0, false);
					player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/Achievement_WorldEvent_Mistletoe:30:30:-15:0|t|cff006800Eventos", GOSSIP_SENDER_MAIN, 203);
					if (grupo)
						player->ADD_GOSSIP_ITEM(1, ss.str().c_str(), GOSSIP_SENDER_MAIN, 204);
					else
						player->ADD_GOSSIP_ITEM_EXTENDED(1, ss.str().c_str(), GOSSIP_SENDER_MAIN, 205, "|CFFFFFF01""\xc2\xbf""Deseas ir a la Zona PvP?|R", 0, false);
					player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface/ICONS/Achievement_character_troll_male:30:30:-15:0|tArena de Gurubashi", GOSSIP_SENDER_MAIN, 210, "|CFFFFFF01""\xc2\xbf""Est""\xC3\xA1""s seguro de querer ir a la Arena Gurubashi?\n NOTA: El Evento de la Arqueta se realiza aquí(en Gurubashi) cada día a las 16:30, 19:30 y 23:30 Hora del Servidor.|R", 0, false);
					player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\Temp:30:30:-15:0|t|rCiudades", GOSSIP_SENDER_MAIN, 212);
					player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\Achievement_BG_returnXflags_def_WSG:30:30:-15:0|t|rMenú principal del Libro", GOSSIP_SENDER_MAIN, 0);

					player->PlayerTalkClass->SendGossipMenu(1, item->GetGUID());
					return true;
				} break;
				
				case 212:
				{
					if (player->GetTeam() == HORDE)
					{
						player->ADD_GOSSIP_ITEM_EXTENDED(2, "Orgrimmar", GOSSIP_SENDER_MAIN, 213, "|CFFFFFF01""\xc2\xbf""Est""\xC3\xA1""s seguro de querer ir a Orgrimmar?", 0, false);
						player->ADD_GOSSIP_ITEM_EXTENDED(2, "Cima del Trueno", GOSSIP_SENDER_MAIN, 214, "|CFFFFFF01""\xc2\xbf""Est""\xC3\xA1""s seguro de querer ir a Cima del Trueno?", 0, false);
						player->ADD_GOSSIP_ITEM_EXTENDED(2, "Entra""\xC3\xB1""as", GOSSIP_SENDER_MAIN, 215, "|CFFFFFF01""\xc2\xbf""Est""\xC3\xA1""s seguro de querer ir a Entra""\xC3\xB1""s?", 0, false);
						player->ADD_GOSSIP_ITEM_EXTENDED(2, "Lunargenta", GOSSIP_SENDER_MAIN, 216, "|CFFFFFF01""\xc2\xbf""Est""\xC3\xA1""s seguro de querer ir a Lunargenta?", 0, false);
					}
					else
					{
						player->ADD_GOSSIP_ITEM_EXTENDED(2, "Ventormenta", GOSSIP_SENDER_MAIN, 217, "|CFFFFFF01""\xc2\xbf""Est""\xC3\xA1""s seguro de querer ir a Ventormenta?", 0, false);
						player->ADD_GOSSIP_ITEM_EXTENDED(2, "Darnassus", GOSSIP_SENDER_MAIN, 218, "|CFFFFFF01""\xc2\xbf""Est""\xC3\xA1""s seguro de querer ir a Darnassus?", 0, false);
						player->ADD_GOSSIP_ITEM_EXTENDED(2, "Forjaz", GOSSIP_SENDER_MAIN, 219, "|CFFFFFF01""\xc2\xbf""Est""\xC3\xA1""s seguro de querer ir a Forjaz?", 0, false);
						player->ADD_GOSSIP_ITEM_EXTENDED(2, "El Exodar", GOSSIP_SENDER_MAIN, 220, "|CFFFFFF01""\xc2\xbf""Est""\xC3\xA1""s seguro de querer ir a El Exodar?", 0, false);					
					}

					player->ADD_GOSSIP_ITEM_EXTENDED(2, "Dalaran", GOSSIP_SENDER_MAIN, 221, "|CFFFFFF01""\xc2\xbf""Est""\xC3\xA1""s seguro de querer ir a Dalaran?", 0, false);
					player->ADD_GOSSIP_ITEM_EXTENDED(2, "Shattrath", GOSSIP_SENDER_MAIN, 222, "|CFFFFFF01""\xc2\xbf""Est""\xC3\xA1""s seguro de querer ir a Shattrath?", 0, false);					
					player->ADD_GOSSIP_ITEM(7, "<-- Volver al Men""\xC3\xBA"" principal", GOSSIP_SENDER_MAIN, 800);

					player->PlayerTalkClass->SendGossipMenu(1, item->GetGUID());
					return true;
				} break;

				case 213:
				{
					player->TeleportTo(1, 1601.08f, -4378.69f, 9.9846f, 2.14362f);
					player->CLOSE_GOSSIP_MENU();
					return true;
				} break;
				
				case 214:
				{
					player->TeleportTo(1, -1274.45f, 71.8601f, 128.159f, 2.80623f);
					player->CLOSE_GOSSIP_MENU();
					return true;
				} break;
				
				case 215:
				{
					player->TeleportTo(0, 1633.75f, 240.167f, -43.1034f, 6.26128f);
					player->CLOSE_GOSSIP_MENU();
					return true;
				} break;
				
				case 216:
				{
					player->TeleportTo(530, 9738.28f, -7454.19f, 13.5605f, 0.043914f);
					player->CLOSE_GOSSIP_MENU();
					return true;
				} break;
				
				case 217:
				{
					player->TeleportTo(0, -8842.09f, 626.358f, 94.0867f, 3.61363f);
					player->CLOSE_GOSSIP_MENU();
					return true;
				} break;
				
				case 218:
				{
					player->TeleportTo(1, 9869.91f, 2493.58f, 1315.88f, 2.78897f);
					player->CLOSE_GOSSIP_MENU();
					return true;
				} break;
				
				case 219:
				{
					player->TeleportTo(0, -4900.47f, -962.585f, 501.455f, 5.40538f);
					player->CLOSE_GOSSIP_MENU();
					return true;
				} break;
				
				case 220:
				{
					player->TeleportTo(530, -3864.92f, -11643.7f, -137.644f, 5.50862f);
					player->CLOSE_GOSSIP_MENU();
					return true;
				} break;
				
				case 221:
				{
					player->TeleportTo(571, 5809.55f, 503.975f, 657.526f, 2.38338f);
					player->CLOSE_GOSSIP_MENU();
					return true;
				} break;
				
				case 222:
				{
					player->TeleportTo(530, -1887.62f, 5359.09f, -12.4279f, 4.40435f);
					player->CLOSE_GOSSIP_MENU();
					return true;
				} break;
				
				case 201:
				{
					player->TeleportTo(0, -12024.9f, -1702.49f, 39.3102f, 0.553043f);
					player->CLOSE_GOSSIP_MENU();
					return true;
				} break;

				case 206:
				{
					if (player->HasItemCount(1404, 1) || player->HasItemCount(13209, 1))
					{
					    player->TeleportTo(530, 10389.4f, -6403.07f, 161.052f, 3.48574f);
						player->CLOSE_GOSSIP_MENU();
						return true;
					}
					else
					{
					   	ChatHandler(player->GetSession()).PSendSysMessage("Debes ser VIP para poder acceder a esta zona.");
                        player->GetSession()->SendAreaTriggerMessage("No eres VIP.");
						player->CLOSE_GOSSIP_MENU();
						return true;
					}
				} break;

				case 205:
				{
					uint32 guid = GUID_LOPART(player->GetGUIDLow());
					if(QueryResult resultGroup = CharacterDatabase.PQuery("SELECT guid FROM group_member WHERE memberGuid=%u", guid))
						if(Group* group = sGroupMgr->GetGroupByDbStoreId((*resultGroup)[0].GetUInt32()))
							player->RemoveFromGroup(group, player->GetGUIDLow());
					int i = urand(0, MAX_PLAYER_SPAWN_POINTS - 1);
					player->TeleportTo(1, playerSpawnPoint[i].GetPositionX(), playerSpawnPoint[i].GetPositionY(), playerSpawnPoint[i].GetPositionZ(), playerSpawnPoint[i].GetOrientation());
					player->CastSpell(player, 8611, false);
					player->CLOSE_GOSSIP_MENU();
					return true;
				} break;

				case 204:
				{
					player->ADD_GOSSIP_ITEM_EXTENDED(2, "Ir a Zona PvP - Solo", GOSSIP_SENDER_MAIN, 205, "|CFFFFFF01""\xc2\xbf""Deseas ir a la Zona PvP?\nSi aceptas, serás expulsado del grupo en el que estés.|R", 0, false);
					player->ADD_GOSSIP_ITEM_EXTENDED(2, "Ir a Zona PvP - Grupo", GOSSIP_SENDER_MAIN, 208, "|CFFFFFF01""\xc2\xbf""Deseas ir a la Zona PvP?\nTodo tu grupo será teletransportado a la Zona PvP, a la misma posicion que tú.\nNOTA: no se permiten más de 2 jugadores por grupo.|R", 0, false);
					player->ADD_GOSSIP_ITEM(7, "<-- Volver al Men""\xC3\xBA"" principal", GOSSIP_SENDER_MAIN, 800);

					player->PlayerTalkClass->SendGossipMenu(1, item->GetGUID());
					return true;
				}
				break;

				case 208:
				{
					Group* grupo = player->GetGroup();
					if (grupo && grupo->GetMembersCount() > 2)
					{
						ChatHandler(player).PSendSysMessage("No puedes ir con un grupo mayor de 2 personas a la Zona PvP.");
						player->CLOSE_GOSSIP_MENU();
						return true;
					}
					else
					{
						Group* grupo = player->GetGroup();
						int i = urand(0, MAX_PLAYER_SPAWN_POINTS - 1);
						if (grupo)
							{
								for(GroupReference *itr = grupo->GetFirstMember(); itr != NULL; itr = itr->next())
								{
									Player* pl = itr->getSource();

									if(!pl || !pl->GetSession())
										continue;

									if(pl->IsBeingTeleported() == true)
									{
									    ChatHandler(player->GetSession()).PSendSysMessage("Un miembro de tu grupo ya está siendo teletransportado.");
										return false;
									}

									if(pl->isInFlight())
									{
										pl->GetMotionMaster()->MovementExpired();
										pl->CleanupAfterTaxiFlight();
									}
									else
										pl->SaveRecallPosition();

									pl->TeleportTo(1, playerSpawnPoint[i].GetPositionX(), playerSpawnPoint[i].GetPositionY(), playerSpawnPoint[i].GetPositionZ(), playerSpawnPoint[i].GetOrientation());
									pl->CastSpell(pl, 8611, false);
								}
							}
						else
							player->TeleportTo(1, playerSpawnPoint[i].GetPositionX(), playerSpawnPoint[i].GetPositionY(), playerSpawnPoint[i].GetPositionZ(), playerSpawnPoint[i].GetOrientation());
						player->CastSpell(player, 8611, false);
						player->CLOSE_GOSSIP_MENU();
						return true;
					}
				} break;

				case 202:
					player->CLOSE_GOSSIP_MENU();
					player->TeleportTo(530, -3330.32f, 4935.4f, -99.0472f, 3.057f);
				break;

				case 210:
					player->CLOSE_GOSSIP_MENU();
					player->TeleportTo(0, -13244.107422f, 197.539185f, 30.955976f, 1.090968f);
				break;

				case 207:
					player->CLOSE_GOSSIP_MENU();
					player->TeleportTo(1, -2315.19951f, -413.394012f, -6.3581f, 1.916640f);
				break;

				// Menu Shop
				case 200:
				{
				    player->ADD_GOSSIP_ITEM_EXTENDED(2, "|cff006800Shop", GOSSIP_SENDER_MAIN, 207, "|CFFFFFF01""\xc2\xbf""Est""\xC3\xA1""s seguro de querer ir a la Shop?|R", 0, false);
			        player->ADD_GOSSIP_ITEM_EXTENDED(2, "|cff2e2efeShop VIP", GOSSIP_SENDER_MAIN, 206, "|CFFFFFF01""\xc2\xbf""Est""\xC3\xA1""s seguro de querer ir a la Shop VIP?|R", 0, false);
					player->ADD_GOSSIP_ITEM(7, "<-- Volver al Men""\xC3\xBA"" principal", GOSSIP_SENDER_MAIN, 800);
					player->PlayerTalkClass->SendGossipMenu(1, item->GetGUID());
					return true;
				} break;

				// Menu Eventos
				case 203:
				    player->ADD_GOSSIP_ITEM_EXTENDED(2, "Evento Laberinto", GOSSIP_SENDER_MAIN, 209, "|CFFFFFF01Solamente podrás ir si está activado el evento que lo activa un MJ. En el momento en que lo activa tendrás 2 minutos para poder ir. Si estás en grupo, todo el grupo se teletransportará contigo al mismo sitio.|R", 0, false);
			        //player->ADD_GOSSIP_ITEM_EXTENDED(2, "Evento de la Arqueta", GOSSIP_SENDER_MAIN, 210, "|CFFFFFF01""\xc2\xbf""Est""\xC3\xA1""s seguro de querer ir a la Arena Gurubashi?\n NOTA: Este evento es cada día a las 16:30, 19:30 y 23:30 Hora del Servidor.|R", 0, false);
					player->ADD_GOSSIP_ITEM(2, "Evento de Escalada", GOSSIP_SENDER_MAIN, 211);
					//player->ADD_GOSSIP_ITEM(2, "El Conflicto de Hyjal", GOSSIP_SENDER_MAIN, 25);
					player->ADD_GOSSIP_ITEM(7, "<-- Volver al Men""\xC3\xBA"" principal", GOSSIP_SENDER_MAIN, 800);
					
					player->PlayerTalkClass->SendGossipMenu(1, item->GetGUID());
					break;

				// Evento de Escalada
				case 211:
				{
					if(Evento_Escalada_Teleport_item() == 0)
					{
						ChatHandler(player).PSendSysMessage("No puedes ir a este lugar porque aún no está activado el Evento de Escalada. Este evento lo inicia un MJ cuando él lo vea conveniente y siempre con previo aviso.");
						player->CLOSE_GOSSIP_MENU();
						return true;
					}
					else if(Evento_Escalada_Teleport_item() == 1)
					{
						if(Evento_Escalada1_item() == 1 && Evento_Escalada2_item() == 0)
							player->TeleportTo(0, -1311.385132f, -1263.237427f, 164.758301f, 3.251770f);
						else if(Evento_Escalada2_item() == 1 && Evento_Escalada1_item() == 0)
							player->TeleportTo(0, 4302.835449f, -2844.920654f, 5.528601f, 5.331080f);
						else
							ChatHandler(player).PSendSysMessage("Hay dos o más eventos de escalada activados al mismo tiempo, díselo a un administrador.");

						player->CLOSE_GOSSIP_MENU();
						return true;
					}
				} break;

				// Evento Laberito
				case 209:
				{
					if(Evento_Escalada_Teleport_laberinto_item_item() == 0)
					{
						ChatHandler(player).PSendSysMessage("No puedes ir a este lugar porque aún no está activado el Evento Laberinto. Este evento lo inicia un MJ cuando él lo vea conveniente y siempre con previo aviso.");
						player->CLOSE_GOSSIP_MENU();
						return true;
					}
					else if(Evento_Escalada_Teleport_laberinto_item_item() == 1)
					{
						if(Evento_Laberinto1_item() == 1)
							player->TeleportTo(1, -8592.215820f, -1179.367554f, 8.877270f, 2.427462f);

						player->CLOSE_GOSSIP_MENU();
						return true;
					}
				} break;
				
				// Anuncio Hyjal
				/*case 25:
				    ChatHandler(player->GetSession()).PSendSysMessage("El Conflicto de Hyjal se encuentra actualmente en desarrollo, os informaremos cuando se encuentre finalizado. El motivo es porque es demasiado complicado y se necesita más gente en el servidor, cuando haya más gente se activará");
					player->GetSession()->SendAreaTriggerMessage("El Conflicto de Hyjal no se encuentra disponible por el momento.");
					player->CLOSE_GOSSIP_MENU();
				    return true;*/

				/////////////////////////////////////////////// FIN - TAXI ///////////////////////////////////////////////

				case 801: // Trainers & Vendors S7 - Submenu
				{
					player->ADD_GOSSIP_ITEM(7, "|TInterface\\icons\\INV_Misc_Book_07:30:30:-15:0|tTrainer Global", GOSSIP_SENDER_MAIN, 100);
					if (player->getClass() == CLASS_HUNTER)
						player->ADD_GOSSIP_ITEM(5, "|TInterface\\icons\\INV_Sword_98:30:30:-15:0|tVendor S7, Glifos & Generador", GOSSIP_SENDER_MAIN, 109);
					else
						player->ADD_GOSSIP_ITEM(5, "|TInterface\\icons\\INV_Sword_98:30:30:-15:0|tGlifos & Vendor S7", GOSSIP_SENDER_MAIN, 101);
					player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\Achievement_BG_returnXflags_def_WSG:30:30:-15:0|t|rMenú principal del Libro", GOSSIP_SENDER_MAIN, 0);

					player->SEND_GOSSIP_MENU(1, item->GetGUID());
					return true;
				} break;

				/////////////////////////////////////////////// PRINCIPIO - TRAINER GLOBAL ///////////////////////////////////////////////

				case 100: // Trainer Global - Accion
				{
					player->ADD_GOSSIP_ITEM(3, "|TInterface\\icons\\INV_Inscription_Tradeskill01:30:30:-15:0|tAprender TODO", GOSSIP_SENDER_MAIN, 720);
					if(player->GetSpecsCount() == 1 && !(player->getLevel() < sWorld->getIntConfig(CONFIG_MIN_DUALSPEC_LEVEL)))
						player->ADD_GOSSIP_ITEM(3, "|TInterface\\icons\\Ability_DualWield:30:30:-15:0|tDoble especializacion de talentos", GOSSIP_SENDER_MAIN, 719);
					player->ADD_GOSSIP_ITEM(1, "|TInterface\\icons\\Ability_Mage_StudentOfTheMind:30:30:-15:0|tReiniciar mis talentos", GOSSIP_SENDER_MAIN, 718);
					if(player->getClass() == CLASS_HUNTER && player->GetPet())
						player->ADD_GOSSIP_ITEM(1, "|TInterface\\icons\\Ability_Mage_StudentOfTheMind:30:30:-15:0|tReiniciar talentos de mi mascota", GOSSIP_SENDER_MAIN, 717);
					player->ADD_GOSSIP_ITEM(1, "|TInterface\\icons\\Achievement_BG_returnXflags_def_WSG:30:30:-15:0|tMenú principal del Libro", GOSSIP_SENDER_MAIN, 0);
					player->SEND_GOSSIP_MENU(1, item->GetGUID());
				} break;

				case 719:
				{
					player->CLOSE_GOSSIP_MENU();
					player->CastSpell(player, 63680, true, NULL, NULL, player->GetGUID());
					player->CastSpell(player, 63624, true, NULL, NULL, player->GetGUID());
					player->GetSession()->SendAreaTriggerMessage("Doble especializacion aprendida");
				} break;

				case 718:
				{
					player->CLOSE_GOSSIP_MENU();
					player->resetTalents(true);
					player->SendTalentsInfoData(false);
					player->GetSession()->SendAreaTriggerMessage("Talentos reiniciados");
				} break;

				case 717:
				{
					player->ResetPetTalents();
					player->GetSession()->SendAreaTriggerMessage("Talentos de tu mascota reiniciados");
					player->CLOSE_GOSSIP_MENU();
				} break;

				case 720: // Class Trainers
				{
					aprender_habilidad_clase(player);
					player->CLOSE_GOSSIP_MENU();
				} break;

				/////////////////////////////////////////////// FIN - TRAINER GLOBAL ///////////////////////////////////////////////

				case 109: //Vendor S7 & Generador - Submenu
				{
					player->ADD_GOSSIP_ITEM(5, "|cff00ff00|TInterface\\icons\\INV_Misc_Food_Vendor_Rice:30:30:-15:0|t|rVendor S7", GOSSIP_SENDER_MAIN, 102);
					//player->ADD_GOSSIP_ITEM(7, "|cff00ff00|TInterface\\icons\\INV_Misc_PenguinPet:30:30:-15:0|t|rGenerador de mascotas", GOSSIP_SENDER_MAIN, 103);
					player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\Achievement_BG_returnXflags_def_WSG:30:30:-15:0|t|rMenú principal del Libro", GOSSIP_SENDER_MAIN, 0);

					player->SEND_GOSSIP_MENU(1, item->GetGUID());
					return true;
				} break;

				case 101: // Glifos & Vendor S7 - Accion
				{
					switch (player->getClass())
					{
						case CLASS_WARRIOR:
						{
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\INV_Inscription_MajorGlyph01:30:30:-15:0|t|rGlifos", GOSSIP_SENDER_MAIN, 95024);
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\INV_CHEST_PLATE_23:30:30:-15:0|t|rSeason 7 - Set", GOSSIP_SENDER_MAIN, 500010);
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\INV_Misc_Cape_18:30:30:-15:0|t|rSeason 7 - OffSet", GOSSIP_SENDER_MAIN, 34038);
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\INV_Axe_85:30:30:-15:0|t|rSeason 7 - Armas", GOSSIP_SENDER_MAIN, 500122);
							player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/Ability_Spy:30:30:-15:0|tVolver", GOSSIP_SENDER_MAIN, 801);
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\Achievement_BG_returnXflags_def_WSG:30:30:-15:0|t|rMenú principal del Libro", GOSSIP_SENDER_MAIN, 0);

							player->SEND_GOSSIP_MENU(1, item->GetGUID());
							return true;
						} break;

						case CLASS_PALADIN:
						{
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\INV_Inscription_MajorGlyph01:30:30:-15:0|t|rGlifos", GOSSIP_SENDER_MAIN, 95019);
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\INV_CHEST_PLATE_23:30:30:-15:0|t|rSeason 7 - Set", GOSSIP_SENDER_MAIN, 500010);
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\INV_Misc_Cape_18:30:30:-15:0|t|rSeason 7 - OffSet", GOSSIP_SENDER_MAIN, 34038);
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\INV_Axe_85:30:30:-15:0|t|rSeason 7 - Armas", GOSSIP_SENDER_MAIN, 500122);
							player->ADD_GOSSIP_ITEM(6, "|cff00ff00|TInterface\\icons\\INV_Relics_LibramofHope:30:30:-15:0|t|rReliquias", GOSSIP_SENDER_MAIN, 500093);
							player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/Ability_Spy:30:30:-15:0|tVolver", GOSSIP_SENDER_MAIN, 801);
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\Achievement_BG_returnXflags_def_WSG:30:30:-15:0|t|rMenú principal del Libro", GOSSIP_SENDER_MAIN, 0);

							player->SEND_GOSSIP_MENU(1, item->GetGUID());
							return true;
						} break;

						/*case CLASS_HUNTER:
						{
							player->ADD_GOSSIP_ITEM(6, "Vendor S7", GOSSIP_SENDER_MAIN, 102);
							player->ADD_GOSSIP_ITEM(6, "Generador de mascotas", GOSSIP_SENDER_MAIN, 103);
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\Achievement_BG_returnXflags_def_WSG:30:30:-15:0|t|rMenú principal del Libro", GOSSIP_SENDER_MAIN, 0);

							player->SEND_GOSSIP_MENU(1, item->GetGUID());
							return true;
						} break;*/

						case CLASS_ROGUE:
						{
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\INV_Inscription_MajorGlyph01:30:30:-15:0|t|rGlifos", GOSSIP_SENDER_MAIN, 95021);
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\INV_CHEST_PLATE_23:30:30:-15:0|t|rSeason 7 - Set", GOSSIP_SENDER_MAIN, 500010);
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\INV_Misc_Cape_18:30:30:-15:0|t|rSeason 7 - OffSet", GOSSIP_SENDER_MAIN, 34038);
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\INV_Axe_85:30:30:-15:0|t|rSeason 7 - Armas", GOSSIP_SENDER_MAIN, 500122);
							player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/Ability_Spy:30:30:-15:0|tVolver", GOSSIP_SENDER_MAIN, 801);
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\Achievement_BG_returnXflags_def_WSG:30:30:-15:0|t|rMenú principal del Libro", GOSSIP_SENDER_MAIN, 0);

							player->SEND_GOSSIP_MENU(1, item->GetGUID());
							return true;
						} break;

						case CLASS_PRIEST:
						{
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\INV_Inscription_MajorGlyph01:30:30:-15:0|t|rGlifos", GOSSIP_SENDER_MAIN, 95020);
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\INV_CHEST_PLATE_23:30:30:-15:0|t|rSeason 7 - Set", GOSSIP_SENDER_MAIN, 500010);
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\INV_Misc_Cape_18:30:30:-15:0|t|rSeason 7 - OffSet", GOSSIP_SENDER_MAIN, 34038);
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\INV_Axe_85:30:30:-15:0|t|rSeason 7 - Armas", GOSSIP_SENDER_MAIN, 500122);
							player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/Ability_Spy:30:30:-15:0|tVolver", GOSSIP_SENDER_MAIN, 801);
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\Achievement_BG_returnXflags_def_WSG:30:30:-15:0|t|rMenú principal del Libro", GOSSIP_SENDER_MAIN, 0);

							player->SEND_GOSSIP_MENU(1, item->GetGUID());
							return true;
						} break;

						case CLASS_DEATH_KNIGHT:
						{
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\INV_Inscription_MajorGlyph01:30:30:-15:0|t|rGlifos", GOSSIP_SENDER_MAIN, 95015);
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\INV_CHEST_PLATE_23:30:30:-15:0|t|rSeason 7 - Set", GOSSIP_SENDER_MAIN, 500010);
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\INV_Misc_Cape_18:30:30:-15:0|t|rSeason 7 - OffSet", GOSSIP_SENDER_MAIN, 34038);
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\INV_Axe_85:30:30:-15:0|t|rSeason 7 - Armas", GOSSIP_SENDER_MAIN, 500122);
							player->ADD_GOSSIP_ITEM(6, "|cff00ff00|TInterface\\icons\\INV_Relics_LibramofHope:30:30:-15:0|t|rReliquias", GOSSIP_SENDER_MAIN, 500093);
							player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/Ability_Spy:30:30:-15:0|tVolver", GOSSIP_SENDER_MAIN, 801);
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\Achievement_BG_returnXflags_def_WSG:30:30:-15:0|t|rMenú principal del Libro", GOSSIP_SENDER_MAIN, 0);

							player->SEND_GOSSIP_MENU(1, item->GetGUID());
							return true;
						} break;

						case CLASS_SHAMAN:
						{
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\INV_Inscription_MajorGlyph01:30:30:-15:0|t|rGlifos", GOSSIP_SENDER_MAIN, 95022);
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\INV_CHEST_PLATE_23:30:30:-15:0|t|rSeason 7 - Set", GOSSIP_SENDER_MAIN, 500010);
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\INV_Misc_Cape_18:30:30:-15:0|t|rSeason 7 - OffSet", GOSSIP_SENDER_MAIN, 34038);
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\INV_Axe_85:30:30:-15:0|t|rSeason 7 - Armas", GOSSIP_SENDER_MAIN, 500122);
							player->ADD_GOSSIP_ITEM(6, "|cff00ff00|TInterface\\icons\\INV_Relics_LibramofHope:30:30:-15:0|t|rReliquias", GOSSIP_SENDER_MAIN, 500093);
							player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/Ability_Spy:30:30:-15:0|tVolver", GOSSIP_SENDER_MAIN, 801);
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\Achievement_BG_returnXflags_def_WSG:30:30:-15:0|t|rMenú principal del Libro", GOSSIP_SENDER_MAIN, 0);

							player->SEND_GOSSIP_MENU(1, item->GetGUID());
							return true;
						} break;

						case CLASS_MAGE:
						{
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\INV_Inscription_MajorGlyph01:30:30:-15:0|t|rGlifos", GOSSIP_SENDER_MAIN, 95018);
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\INV_CHEST_PLATE_23:30:30:-15:0|t|rSeason 7 - Set", GOSSIP_SENDER_MAIN, 500010);
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\INV_Misc_Cape_18:30:30:-15:0|t|rSeason 7 - OffSet", GOSSIP_SENDER_MAIN, 34038);
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\INV_Axe_85:30:30:-15:0|t|rSeason 7 - Armas", GOSSIP_SENDER_MAIN, 500122);
							player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/Ability_Spy:30:30:-15:0|tVolver", GOSSIP_SENDER_MAIN, 801);
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\Achievement_BG_returnXflags_def_WSG:30:30:-15:0|t|rMenú principal del Libro", GOSSIP_SENDER_MAIN, 0);

							player->SEND_GOSSIP_MENU(1, item->GetGUID());
							return true;
						} break;

						case CLASS_WARLOCK:
						{
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\INV_Inscription_MajorGlyph01:30:30:-15:0|t|rGlifos", GOSSIP_SENDER_MAIN, 95023);
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\INV_CHEST_PLATE_23:30:30:-15:0|t|rSeason 7 - Set", GOSSIP_SENDER_MAIN, 500010);
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\INV_Misc_Cape_18:30:30:-15:0|t|rSeason 7 - OffSet", GOSSIP_SENDER_MAIN, 34038);
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\INV_Axe_85:30:30:-15:0|t|rSeason 7 - Armas", GOSSIP_SENDER_MAIN, 500122);
							player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/Ability_Spy:30:30:-15:0|tVolver", GOSSIP_SENDER_MAIN, 801);
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\Achievement_BG_returnXflags_def_WSG:30:30:-15:0|t|rMenú principal del Libro", GOSSIP_SENDER_MAIN, 0);

							player->SEND_GOSSIP_MENU(1, item->GetGUID());
							return true;
						} break;

						case CLASS_DRUID:
						{
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\INV_Inscription_MajorGlyph01:30:30:-15:0|t|rGlifos", GOSSIP_SENDER_MAIN, 95016);
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\INV_CHEST_PLATE_23:30:30:-15:0|t|rSeason 7 - Set", GOSSIP_SENDER_MAIN, 500010);
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\INV_Misc_Cape_18:30:30:-15:0|t|rSeason 7 - OffSet", GOSSIP_SENDER_MAIN, 34038);
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\INV_Axe_85:30:30:-15:0|t|rSeason 7 - Armas", GOSSIP_SENDER_MAIN, 500122);
							player->ADD_GOSSIP_ITEM(6, "|cff00ff00|TInterface\\icons\\INV_Relics_LibramofHope:30:30:-15:0|t|rReliquias", GOSSIP_SENDER_MAIN, 500093);
							player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/Ability_Spy:30:30:-15:0|tVolver", GOSSIP_SENDER_MAIN, 801);
							player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\Achievement_BG_returnXflags_def_WSG:30:30:-15:0|t|rMenú principal del Libro", GOSSIP_SENDER_MAIN, 0);

							player->SEND_GOSSIP_MENU(1, item->GetGUID());
							return true;
						} break;
					}
					player->SEND_GOSSIP_MENU(1, item->GetGUID());
					return true;
				} break;

				case 102: //Trainer & Vendor S7 HUNTER - Submenu 3
				{
					player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\INV_Inscription_MajorGlyph01:30:30:-15:0|t|rGlifos", GOSSIP_SENDER_MAIN, 95017);
					player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\INV_CHEST_PLATE_23:30:30:-15:0|t|rSeason 7 - Set", GOSSIP_SENDER_MAIN, 500010);
					player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\INV_Misc_Cape_18:30:30:-15:0|t|rSeason 7 - OffSet", GOSSIP_SENDER_MAIN, 34038);
					player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\INV_Axe_85:30:30:-15:0|t|rSeason 7 - Armas", GOSSIP_SENDER_MAIN, 500122);
					player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/Ability_Spy:30:30:-15:0|tVolver", GOSSIP_SENDER_MAIN, 109);
					player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\Achievement_BG_returnXflags_def_WSG:30:30:-15:0|t|rMenú principal del Libro", GOSSIP_SENDER_MAIN, 0);

					player->SEND_GOSSIP_MENU(1, item->GetGUID());
					return true;
				} break;

				case 103: //Generador de mascotas - Accion
				{
					player->ADD_GOSSIP_ITEM(4, "|cff00ff00|TInterface\\icons\\Inv_box_petcarrier_01:30:30:-15:0|t|r Obtener una mascota.", GOSSIP_SENDER_MAIN, 500);
					if (player->CanTameExoticPets())
					{
						player->ADD_GOSSIP_ITEM(4, "|cff00ff00|TInterface\\icons\\Inv_box_birdcage_01:30:30:-15:0|t|r Obtener una mascota exotica.", GOSSIP_SENDER_MAIN, 501);
					}
					player->ADD_GOSSIP_ITEM(2, "|cff00ff00|TInterface\\icons\\Ability_hunter_mendpet:30:30:-15:0|t|r Ir al establo.", GOSSIP_SENDER_MAIN, 716);
					player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\Achievement_BG_returnXflags_def_WSG:30:30:-15:0|t|rMenú principal del Libro", GOSSIP_SENDER_MAIN, 0);

					player->SEND_GOSSIP_MENU(1, item->GetGUID());
					return true;
				} break;

				case 500:
				{
					player->ADD_GOSSIP_ITEM(6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_bat:26:26:-15:0|t|r Murci""\xC3\xA9""lago.", GOSSIP_SENDER_MAIN, 503);
					player->ADD_GOSSIP_ITEM(6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_bear:26:26:-15:0|t|r Oso.", GOSSIP_SENDER_MAIN, 504);
					player->ADD_GOSSIP_ITEM(6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_boar:26:26:-15:0|t|r Jabal""\xC3\xAD"".", GOSSIP_SENDER_MAIN, 505);
					player->ADD_GOSSIP_ITEM(6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_cat:26:26:-15:0|t|r Felino.", GOSSIP_SENDER_MAIN, 506);
					player->ADD_GOSSIP_ITEM(6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_vulture:26:26:-15:0|t|r Buitre.", GOSSIP_SENDER_MAIN, 507);
					player->ADD_GOSSIP_ITEM(6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_crab:26:26:-15:0|t|r Cangrejo.", GOSSIP_SENDER_MAIN, 508);
					player->ADD_GOSSIP_ITEM(6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_crocolisk:26:26:-15:0|t|r Cocodrilo.", GOSSIP_SENDER_MAIN, 509);
					player->ADD_GOSSIP_ITEM(6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_dragonhawk:26:26:-15:0|t|r Dracohalc""\xC3\xB3""n.", GOSSIP_SENDER_MAIN, 510);
					player->ADD_GOSSIP_ITEM(6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_gorilla:26:26:-15:0|t|r Gorila.", GOSSIP_SENDER_MAIN, 511);
					player->ADD_GOSSIP_ITEM(6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_wolf:26:26:-15:0|t|r Lobo.", GOSSIP_SENDER_MAIN, 512);
					player->ADD_GOSSIP_ITEM(6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_hyena:26:26:-15:0|t|r Hiena.", GOSSIP_SENDER_MAIN, 513);
					player->ADD_GOSSIP_ITEM(6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_moth:26:26:-15:0|t|r Polilla.", GOSSIP_SENDER_MAIN, 514);
					player->ADD_GOSSIP_ITEM(6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_owl:26:26:-15:0|t|r Ave Rapaz.", GOSSIP_SENDER_MAIN, 515);
					player->ADD_GOSSIP_ITEM(4, "Siguiente pagina. ->", GOSSIP_SENDER_MAIN, 502);
					player->ADD_GOSSIP_ITEM(2, "<- Volver", GOSSIP_SENDER_MAIN, 103);
					player->SEND_GOSSIP_MENU(1, item->GetGUID());
					return true;
				} break;

				case 502:
				{
					player->ADD_GOSSIP_ITEM(6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_raptor:26:26:-15:0|t|r Dinosaurio.", GOSSIP_SENDER_MAIN, 516);
					player->ADD_GOSSIP_ITEM(6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_ravager:26:26:-15:0|t|r Devastador.", GOSSIP_SENDER_MAIN, 517);
					player->ADD_GOSSIP_ITEM(6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_tallstrider:26:26:-15:0|t|r Zancaalta.", GOSSIP_SENDER_MAIN, 518);
					player->ADD_GOSSIP_ITEM(6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_scorpid:26:26:-15:0|t|r Escorpi""\xC3\xB3""n.", GOSSIP_SENDER_MAIN, 519);
					player->ADD_GOSSIP_ITEM(6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_spider:26:26:-15:0|t|r Tar""\xC3\xA1""ntula.", GOSSIP_SENDER_MAIN, 520);
					player->ADD_GOSSIP_ITEM(6, "|cff00ff00|TInterface\\icons\\Ability_hunter_cobrastrikes:26:26:-15:0|t|r Serpiente.", GOSSIP_SENDER_MAIN, 521);
					player->ADD_GOSSIP_ITEM(6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_turtle:26:26:-15:0|t|r Tortuga.", GOSSIP_SENDER_MAIN, 522);
					player->ADD_GOSSIP_ITEM(6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_wasp:26:26:-15:0|t|r Avispa.", GOSSIP_SENDER_MAIN, 523);
					player->ADD_GOSSIP_ITEM(4, "<- Pagina Anterior.", GOSSIP_SENDER_MAIN, 500);
					player->ADD_GOSSIP_ITEM(2, "<- Volver al Menú Principal", GOSSIP_SENDER_MAIN, 103);
					player->SEND_GOSSIP_MENU(1, item->GetGUID());
					return true;
				} break;

				case 501:
				{
					player->ADD_GOSSIP_ITEM(6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_chimera:26:26:-15:0|t|r Quimera Alavil.", GOSSIP_SENDER_MAIN, 524);
					player->ADD_GOSSIP_ITEM(6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_corehound:26:26:-15:0|t|r Can del N""\xC3\xBA""cleo.", GOSSIP_SENDER_MAIN, 525);
					player->ADD_GOSSIP_ITEM(6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_devilsaur:26:26:-15:0|t|r Demosaurio.", GOSSIP_SENDER_MAIN, 526);
					player->ADD_GOSSIP_ITEM(6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_rhino:26:26:-15:0|t|r Rinoceronte.", GOSSIP_SENDER_MAIN, 527);
					player->ADD_GOSSIP_ITEM(6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_silithid:26:26:-15:0|t|r Sil""\xC3\xAD""tido.", GOSSIP_SENDER_MAIN, 528);
					player->ADD_GOSSIP_ITEM(6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_worm:26:26:-15:0|t|r Gusano.", GOSSIP_SENDER_MAIN, 529);
					player->ADD_GOSSIP_ITEM(6, "|cff00ff00|TInterface\\icons\\Ability_druid_catform:26:26:-15:0|t|r Loque'nahak.", GOSSIP_SENDER_MAIN, 530);
					player->ADD_GOSSIP_ITEM(6, "|cff00ff00|TInterface\\icons\\Spell_nature_spiritwolf:26:26:-15:0|t|r Skoll.", GOSSIP_SENDER_MAIN, 531);
					player->ADD_GOSSIP_ITEM(6, "|cff00ff00|TInterface\\icons\\Spell_shadow_spectralsight:26:26:-15:0|t|r Gondria.", GOSSIP_SENDER_MAIN, 532);
					player->ADD_GOSSIP_ITEM(4, "<- Conseguir una mascota normal.", GOSSIP_SENDER_MAIN, 500);
					player->ADD_GOSSIP_ITEM(2, "<- Volver al Menú Principal.", GOSSIP_SENDER_MAIN, 103);
					player->SEND_GOSSIP_MENU(1, item->GetGUID());
                } break;

				case 716:
				{
					player->GetSession()->SendStablePet(item->GetGUID());
					return true;
				} break;

				case 524:
					CreatePet(player, item, 21879);
					return true;
				break;
				case 525:
					CreatePet(player, item, 21108);
					return true;
				break;
				case 526:
					CreatePet(player, item, 20931);
					return true;
				break;
				case 527:
					CreatePet(player, item, 30445);
					return true;
				break;
				case 528:
					CreatePet(player, item, 5460);
					return true;
				break;
				case 529:
					CreatePet(player, item, 30148);
					return true;
				break;
				case 530:
					CreatePet(player, item, 32517);
					return true;
				break;
				case 531:
					CreatePet(player, item, 35189);
					return true;
				break;
				case 532:
					CreatePet(player, item, 33776);
					return true;
				break;
				case 520:
					CreatePet(player, item, 2349);
					return true;
				break;
				case 510:
					CreatePet(player, item, 27946);
					return true;
				break;
				case 503:
					CreatePet(player, item, 28233);
					return true;
				break;
				case 517:
					CreatePet(player, item, 17199);
					return true;
				break;
				case 516:
					CreatePet(player, item, 14821);
					return true;
				break;
				case 521:
					CreatePet(player, item, 28358);
					return true;
				break;
				case 504:
					CreatePet(player, item, 29319);
					return true;
				break;
				case 505:
					CreatePet(player, item, 29996);
					return true;
				break;
				case 523:
					CreatePet(player, item, 28085);
					return true;
				break;
				case 506:
					CreatePet(player, item, 28097);
					return true;
				break;
				case 507:
					CreatePet(player, item, 26838);
					return true;
				break;
				case 508:
					CreatePet(player, item, 24478);
					return true;
				break;
				case 509:
					CreatePet(player, item, 1417);
					return true;
				break;
				case 511:
					CreatePet(player, item, 28213);
					return true;
				break;
				case 512:
					CreatePet(player, item, 29452);
					return true;
				break;
				case 513:
					CreatePet(player, item, 13036);
					return true;
				break;
				case 514:
					CreatePet(player, item, 27421);
					return true;
				break;
				case 515:
					CreatePet(player, item, 23136);
					return true;
				break;
				case 518:
					CreatePet(player, item, 22807);
					return true;
				break;
				case 519:
					CreatePet(player, item, 9698);
					return true;
				break;
				case 522:
					CreatePet(player, item, 25482);
					return true;
				break;

				case 500010: //Season 7 - Set - Accion
				case 34038: //Season 7 - OffSet - Accion
				case 500122: //Season 7 - Armas - Accion
				case 500093: //Season 7 - Armas - Accion
				case 95024: //Glifos WARR - Accion
				case 95019: //Glifos PALA - Accion
				case 95021: //Glifos ROGUE - Accion
				case 95020: //Glifos PRIEST - Accion
				case 95015: //Glifos DK - Accion
				case 95022: //Glifos SHAMAN - Accion
				case 95018: //Glifos MAGE - Accion
				case 95023: //Glifos LOCK - Accion
				case 95016: //Glifos DRUID - Accion
				case 95017: //Glifos HUNTER - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), action);
					return true;
				} break;

				case 802: //Item Vendors - Submenu
				{
					player->ADD_GOSSIP_ITEM(5, "|cff00ff00|TInterface\\icons\\inv_misc_frostemblem_01:30:30:-15:0|t|rFree PvE Items", GOSSIP_SENDER_MAIN, 400);
					player->ADD_GOSSIP_ITEM(5, "|cff00ff00|TInterface\\icons\\inv_helmet_160:30:30:-15:0|t|rS8 & 264 Items", GOSSIP_SENDER_MAIN, 401);
					player->ADD_GOSSIP_ITEM(5, "|cff00ff00|TInterface\\icons\\inv_gizmo_03:30:30:-15:0|t|rVariados", GOSSIP_SENDER_MAIN, 402);
					player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\Achievement_BG_returnXflags_def_WSG:30:30:-15:0|t|rMenú principal del Libro", GOSSIP_SENDER_MAIN, 0);

					player->SEND_GOSSIP_MENU(1, item->GetGUID());
					return true;
				} break;

				case 400: //Free PvE Items - Submenu
				{
					player->ADD_GOSSIP_ITEM(5, "PvE Armas", GOSSIP_SENDER_MAIN, 403);
					player->ADD_GOSSIP_ITEM(5, "PvE Armaduras", GOSSIP_SENDER_MAIN, 404);
					player->ADD_GOSSIP_ITEM(2, "<- Volver", GOSSIP_SENDER_MAIN, 802);
					player->ADD_GOSSIP_ITEM(2, "<-- Menú principal del Libro", GOSSIP_SENDER_MAIN, 0);
					player->SEND_GOSSIP_MENU(1, item->GetGUID());
					return true;
				} break;

				case 403: //Pve Armas - Submenu
				{
					player->ADD_GOSSIP_ITEM(5, "Espadas", GOSSIP_SENDER_MAIN, 405);
					player->ADD_GOSSIP_ITEM(5, "Hachas", GOSSIP_SENDER_MAIN, 406);
					player->ADD_GOSSIP_ITEM(5, "Mazas", GOSSIP_SENDER_MAIN, 407);
					player->ADD_GOSSIP_ITEM(5, "A distancia", GOSSIP_SENDER_MAIN, 408);
					player->ADD_GOSSIP_ITEM(6, "Bastones", GOSSIP_SENDER_MAIN, 700011);
					player->ADD_GOSSIP_ITEM(6, "Dagas", GOSSIP_SENDER_MAIN, 700037);
					player->ADD_GOSSIP_ITEM(6, "Armas de Asta", GOSSIP_SENDER_MAIN, 700060);
					player->ADD_GOSSIP_ITEM(6, "Armas de Puño", GOSSIP_SENDER_MAIN, 700038);
					player->ADD_GOSSIP_ITEM(6, "Escudos", GOSSIP_SENDER_MAIN, 700033);
					player->ADD_GOSSIP_ITEM(6, "Reliquias", GOSSIP_SENDER_MAIN, 700034);
					player->ADD_GOSSIP_ITEM(6, "Off Hand", GOSSIP_SENDER_MAIN, 700007);
					player->ADD_GOSSIP_ITEM(2, "<- Volver", GOSSIP_SENDER_MAIN, 400);
					player->ADD_GOSSIP_ITEM(2, "<-- Menú principal del Libro", GOSSIP_SENDER_MAIN, 0);
					player->SEND_GOSSIP_MENU(1, item->GetGUID());
					return true;
				} break;


				case 405: // Espadas - Submenu2
				{
					player->ADD_GOSSIP_ITEM(6, "Espadas - 1 Mano", GOSSIP_SENDER_MAIN, 700004);
					player->ADD_GOSSIP_ITEM(6, "Espadas - 2 Manos", GOSSIP_SENDER_MAIN, 700008);
					player->ADD_GOSSIP_ITEM(2, "<- Volver", GOSSIP_SENDER_MAIN, 403);
					player->ADD_GOSSIP_ITEM(2, "<-- Menú principal del Libro", GOSSIP_SENDER_MAIN, 0);
					player->SEND_GOSSIP_MENU(1, item->GetGUID());
					return true;
				} break;

				case 406: // Hachas - Submenu 2
				{
					player->ADD_GOSSIP_ITEM(6, "Hachas - 1 Mano", GOSSIP_SENDER_MAIN, 700006);
					player->ADD_GOSSIP_ITEM(6, "Hachas - 2 Manos", GOSSIP_SENDER_MAIN, 700010);
					player->ADD_GOSSIP_ITEM(2, "<- Volver", GOSSIP_SENDER_MAIN, 403);
					player->ADD_GOSSIP_ITEM(2, "<-- Menú principal del Libro", GOSSIP_SENDER_MAIN, 0);
					player->SEND_GOSSIP_MENU(1, item->GetGUID());
					return true;
				} break;

				case 407: // Mazas - Submenu 2
				{
					player->ADD_GOSSIP_ITEM(6, "Mazas - 1 Mano", GOSSIP_SENDER_MAIN, 419);
					player->ADD_GOSSIP_ITEM(6, "Mazas - 2 Manos", GOSSIP_SENDER_MAIN, 420);
					player->ADD_GOSSIP_ITEM(2, "<- Volver", GOSSIP_SENDER_MAIN, 403);
					player->ADD_GOSSIP_ITEM(2, "<-- Menú principal del Libro", GOSSIP_SENDER_MAIN, 0);
					player->SEND_GOSSIP_MENU(1, item->GetGUID());
					return true;
				} break;

				case 419: // Mazas - 1 Mano - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 700005);
					return true;
				} break;

				case 420: // Mazas - 2 Manos - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 700009);
					return true;
				} break;

				case 408: // A distancia - Submenu 2
				{
					player->ADD_GOSSIP_ITEM(6, "Arcos", GOSSIP_SENDER_MAIN, 700036);
					player->ADD_GOSSIP_ITEM(6, "Ballestas", GOSSIP_SENDER_MAIN, 700040);
					player->ADD_GOSSIP_ITEM(6, "Varitas", GOSSIP_SENDER_MAIN, 700068);
					player->ADD_GOSSIP_ITEM(6, "Cuchillos Arrojadizos", GOSSIP_SENDER_MAIN, 700013);
					player->ADD_GOSSIP_ITEM(2, "<- Volver", GOSSIP_SENDER_MAIN, 403);
					player->ADD_GOSSIP_ITEM(2, "<-- Menú principal del Libro", GOSSIP_SENDER_MAIN, 0);
					player->SEND_GOSSIP_MENU(1, item->GetGUID());
					return true;
				} break;

				case 700004: // Espadas - 1 Mano - Accion
				case 700008: // Espadas - 2 Manos - Accion
				case 700006: // Hachas - 1 Mano - Accion
				case 700010: // Hachas - 2 Manos - Accion
				case 700036: // A distancia - Arcos - Accion
				case 700040: // A distancia - Ballestas - Accion
				case 700068: // A distancia - Varitas - Accion
				case 700013: // A distancia - Cuchillos Arrojadizos - Accion
				case 700011: // Bastones - Accion
				case 700037: // Dagas - Accion
				case 700060: // Armas de Asta - Accion
				case 700038: // Armas de Puño - Accion
				case 700033: // Escudos - Accion
				case 700034: // Reliquias - Accion
				case 700007: // Off Hand - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), action);
					return true;
				} break;

				case 404: //PvE Armaduras - Submenu
				{
					player->ADD_GOSSIP_ITEM(6, "Set Tier 10", GOSSIP_SENDER_MAIN, 426);
					player->ADD_GOSSIP_ITEM(6, "Brazales", GOSSIP_SENDER_MAIN, 427);
					player->ADD_GOSSIP_ITEM(6, "Cinturones", GOSSIP_SENDER_MAIN, 428);
					player->ADD_GOSSIP_ITEM(6, "Botas", GOSSIP_SENDER_MAIN, 429);
					player->ADD_GOSSIP_ITEM(6, "Capas", GOSSIP_SENDER_MAIN, 430);
					player->ADD_GOSSIP_ITEM(6, "Collares", GOSSIP_SENDER_MAIN, 431);
					player->ADD_GOSSIP_ITEM(6, "Anillos", GOSSIP_SENDER_MAIN, 432);
					player->ADD_GOSSIP_ITEM(6, "Abalorios", GOSSIP_SENDER_MAIN, 433);
					player->ADD_GOSSIP_ITEM(2, "<- Volver", GOSSIP_SENDER_MAIN, 400);
					player->ADD_GOSSIP_ITEM(2, "<-- Menú principal del Libro", GOSSIP_SENDER_MAIN, 0);
					player->SEND_GOSSIP_MENU(1, item->GetGUID());
					return true;
				} break;

				case 426: // Set Tier 10 - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 700014);
					return true;
				} break;

				case 427: // Brazales - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 700018);
					return true;
				} break;

				case 428: // Cinturones - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 700017);
					return true;
				} break;

				case 429: // Botas - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 700070);
					return true;
				} break;

				case 430: // Capas - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 700032);
					return true;
				} break;

				case 431: // Collares - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 700039);
					return true;
				} break;

				case 432: // Anillos - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 700019);
					return true;
				} break;

				case 433: // Abalorios - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 700012);
					return true;
				} break;

				case 401: // S8 & 264 Items - Submenu
				{
					player->ADD_GOSSIP_ITEM(5, "PvP", GOSSIP_SENDER_MAIN, 434);
					player->ADD_GOSSIP_ITEM(5, "PvE", GOSSIP_SENDER_MAIN, 435);
					player->ADD_GOSSIP_ITEM(5, "Otros", GOSSIP_SENDER_MAIN, 436);
					player->ADD_GOSSIP_ITEM(2, "<- Volver", GOSSIP_SENDER_MAIN, 802);
					player->ADD_GOSSIP_ITEM(2, "<-- Menú principal del Libro", GOSSIP_SENDER_MAIN, 0);
					player->SEND_GOSSIP_MENU(1, item->GetGUID());
					return true;
				} break;

				case 434: // PvP - Submenu
				{
					player->ADD_GOSSIP_ITEM(6, "Season 8", GOSSIP_SENDER_MAIN, 437);
					player->ADD_GOSSIP_ITEM(6, "Armas PvP 264", GOSSIP_SENDER_MAIN, 438);
					player->ADD_GOSSIP_ITEM(6, "Armas PvP 277", GOSSIP_SENDER_MAIN, 439);
					player->ADD_GOSSIP_ITEM(6, "Off-Set PvP", GOSSIP_SENDER_MAIN, 440);
					player->ADD_GOSSIP_ITEM(2, "<- Volver", GOSSIP_SENDER_MAIN, 401);
					player->ADD_GOSSIP_ITEM(2, "<-- Menú principal del Libro", GOSSIP_SENDER_MAIN, 0);
					player->SEND_GOSSIP_MENU(1, item->GetGUID());
					return true;
				} break;

				case 437: // Season 8 - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 700090);
					return true;
				} break;

				case 438: // Armas PvP 264 - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500107);
					return true;
				} break;

				case 439: // Armas PvP 277 - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500109);
					return true;
				} break;

				case 440: // Off-Set PvP - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 700056);
					return true;
				} break;

				case 435: // PvE - Submenu
				{
					player->ADD_GOSSIP_ITEM(6, "Set PvE Heróico", GOSSIP_SENDER_MAIN, 441);
					player->ADD_GOSSIP_ITEM(6, "Armas PvE", GOSSIP_SENDER_MAIN, 442);
					player->ADD_GOSSIP_ITEM(6, "Off-Set PvE", GOSSIP_SENDER_MAIN, 443);
					player->ADD_GOSSIP_ITEM(6, "Complementos PvE", GOSSIP_SENDER_MAIN, 444);
					player->ADD_GOSSIP_ITEM(6, "Non-Set PvE", GOSSIP_SENDER_MAIN, 445);
					player->ADD_GOSSIP_ITEM(2, "<- Volver", GOSSIP_SENDER_MAIN, 401);
					player->ADD_GOSSIP_ITEM(2, "<-- Menú principal del Libro", GOSSIP_SENDER_MAIN, 0);
					player->SEND_GOSSIP_MENU(1, item->GetGUID());
					return true;
				} break;

				case 441: // Set PvE Heróico PvP - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 700094);
					return true;
				} break;

				case 442: // Armas PvE - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 700096);
					return true;
				} break;

				case 443: // Off-Set PvE - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 700097);
					return true;
				} break;

				case 444: // Complementos PvE - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 700098);
					return true;
				} break;

				case 445: // Non-Set PvE - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 700095);
					return true;
				} break;

				case 436: // Otros - Submenu
				{
					player->ADD_GOSSIP_ITEM(6, "Reliquias", GOSSIP_SENDER_MAIN, 446);
					player->ADD_GOSSIP_ITEM(6, "Monturas", GOSSIP_SENDER_MAIN, 447);
					player->ADD_GOSSIP_ITEM(6, "Camisas Custom", GOSSIP_SENDER_MAIN, 448);
					player->ADD_GOSSIP_ITEM(2, "<- Volver", GOSSIP_SENDER_MAIN, 401);
					player->ADD_GOSSIP_ITEM(2, "<-- Menú principal del Libro", GOSSIP_SENDER_MAIN, 0);
					player->SEND_GOSSIP_MENU(1, item->GetGUID());
					return true;
				} break;

				case 446: // Reliquias - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 700055);
					return true;
				} break;

				case 447: // Monturas - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 26308);
					return true;
				} break;

				case 448: // Camisas Custom - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 600019);
					return true;
				} break;

				case 402: // Variados - Submenu
				{
					player->ADD_GOSSIP_ITEM(5, "Gemas & Mejoras", GOSSIP_SENDER_MAIN, 449);
					player->ADD_GOSSIP_ITEM(5, "Utilidades", GOSSIP_SENDER_MAIN, 450);
					player->ADD_GOSSIP_ITEM(5, "Monturas & Mascotas", GOSSIP_SENDER_MAIN, 451);
					player->ADD_GOSSIP_ITEM(6, "Camisas", GOSSIP_SENDER_MAIN, 452);
					player->ADD_GOSSIP_ITEM(6, "Tabardos", GOSSIP_SENDER_MAIN, 453);
					player->ADD_GOSSIP_ITEM(2, "<- Volver", GOSSIP_SENDER_MAIN, 802);
					player->ADD_GOSSIP_ITEM(2, "<-- Menú principal del Libro", GOSSIP_SENDER_MAIN, 0);
					player->SEND_GOSSIP_MENU(1, item->GetGUID());
					return true;
				} break;

				case 449: // Gemas & Mejoras - Submenu
				{
					player->ADD_GOSSIP_ITEM(5, "Gemas", GOSSIP_SENDER_MAIN, 454);
					player->ADD_GOSSIP_ITEM(5, "Encantamientos", GOSSIP_SENDER_MAIN, 455);
					player->ADD_GOSSIP_ITEM(5, "Buffos", GOSSIP_SENDER_MAIN, 456);
					player->ADD_GOSSIP_ITEM(2, "<- Volver", GOSSIP_SENDER_MAIN, 402);
					player->ADD_GOSSIP_ITEM(2, "<-- Menú principal del Libro", GOSSIP_SENDER_MAIN, 0);
					player->SEND_GOSSIP_MENU(1, item->GetGUID());
					return true;
				} break;

				case 454: // Gemas - Submenu
				{
					player->ADD_GOSSIP_ITEM(6, "Meta", GOSSIP_SENDER_MAIN, 457);
					player->ADD_GOSSIP_ITEM(6, "Rojo", GOSSIP_SENDER_MAIN, 458);
					player->ADD_GOSSIP_ITEM(6, "Azul", GOSSIP_SENDER_MAIN, 459);
					player->ADD_GOSSIP_ITEM(6, "Amarillo", GOSSIP_SENDER_MAIN, 460);
					player->ADD_GOSSIP_ITEM(6, "Morado", GOSSIP_SENDER_MAIN, 461);
					player->ADD_GOSSIP_ITEM(6, "Naranja", GOSSIP_SENDER_MAIN, 462);
					player->ADD_GOSSIP_ITEM(6, "Verde", GOSSIP_SENDER_MAIN, 463);
					player->ADD_GOSSIP_ITEM(6, "Centelleante", GOSSIP_SENDER_MAIN, 464);
					player->ADD_GOSSIP_ITEM(2, "<- Volver", GOSSIP_SENDER_MAIN, 449);
					player->ADD_GOSSIP_ITEM(2, "<-- Menú principal del Libro", GOSSIP_SENDER_MAIN, 0);
					player->SEND_GOSSIP_MENU(1, item->GetGUID());
					return true;
				} break;

				case 457: // Meta - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500059);
					return true;
				} break;

				case 458: // Rojo - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500092);
					return true;
				} break;

				case 459: // Azul - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500014);
					return true;
				} break;

				case 460: // Amarillo - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500112);
					return true;
				} break;

				case 461: // Morado - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500073);
					return true;
				} break;

				case 462: // Naranja - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500067);
					return true;
				} break;

				case 463: // Verde - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500041);
					return true;
				} break;

				case 464: // Centelleante - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500071);
					return true;
				} break;

				case 455: // Encantamientos - Submenu
				{
					player->ADD_GOSSIP_ITEM(6, "Encantamientos Permanentes", GOSSIP_SENDER_MAIN, 465);
					player->ADD_GOSSIP_ITEM(6, "Encantamientos de Armas", GOSSIP_SENDER_MAIN, 466);
					player->ADD_GOSSIP_ITEM(6, "Encantamientos de Armaduras", GOSSIP_SENDER_MAIN, 467);
					player->ADD_GOSSIP_ITEM(2, "<- Volver", GOSSIP_SENDER_MAIN, 449);
					player->ADD_GOSSIP_ITEM(2, "<-- Menú principal del Libro", GOSSIP_SENDER_MAIN, 0);
					player->SEND_GOSSIP_MENU(1, item->GetGUID());
					return true;
				} break;

				case 465: // Encantamientos Permanentes - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500149);
					return true;
				} break;

				case 466: // Encantamientos de Armas - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500136);
					return true;
				} break;

				case 467: // Encantamientos de Armaduras - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500137);
					return true;
				} break;

				case 456: // Buffos - Submenu
				{
					player->ADD_GOSSIP_ITEM(6, "Pegaminos", GOSSIP_SENDER_MAIN, 468);
					player->ADD_GOSSIP_ITEM(6, "Mejoras temporales", GOSSIP_SENDER_MAIN, 469);
					player->ADD_GOSSIP_ITEM(6, "Pociones", GOSSIP_SENDER_MAIN, 470);
					player->ADD_GOSSIP_ITEM(6, "Elixires", GOSSIP_SENDER_MAIN, 471);
					player->ADD_GOSSIP_ITEM(6, "Frascos", GOSSIP_SENDER_MAIN, 472);
					player->ADD_GOSSIP_ITEM(2, "<- Volver", GOSSIP_SENDER_MAIN, 449);
					player->ADD_GOSSIP_ITEM(2, "<-- Menú principal del Libro", GOSSIP_SENDER_MAIN, 0);
					player->SEND_GOSSIP_MENU(1, item->GetGUID());
					return true;
				} break;

				case 468: // Pegaminos - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500096);
					return true;
				} break;

				case 469: // Mejoras temporales - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500050);
					return true;
				} break;

				case 470: // Pociones - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500135);
					return true;
				} break;

				case 471: // Elixires - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500029);
					return true;
				} break;

				case 472: // Frascos - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500038);
					return true;
				} break;

				case 450: // Utilidades - Submenu
				{
					player->ADD_GOSSIP_ITEM(6, "Comida y bebida", GOSSIP_SENDER_MAIN, 473);
					player->ADD_GOSSIP_ITEM(6, "Mochilas", GOSSIP_SENDER_MAIN, 474);
					player->ADD_GOSSIP_ITEM(6, "Componentes", GOSSIP_SENDER_MAIN, 475);
					player->ADD_GOSSIP_ITEM(6, "Proyectiles", GOSSIP_SENDER_MAIN, 476);
					player->ADD_GOSSIP_ITEM(6, "Vendas", GOSSIP_SENDER_MAIN, 477);
					player->ADD_GOSSIP_ITEM(2, "<- Volver", GOSSIP_SENDER_MAIN, 402);
					player->ADD_GOSSIP_ITEM(2, "<-- Menú principal del Libro", GOSSIP_SENDER_MAIN, 0);
					player->SEND_GOSSIP_MENU(1, item->GetGUID());
					return true;
				} break;

				case 473: // Comida y bebida - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500145);
					return true;
				} break;

				case 474: // Mochilas - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500020);
					return true;
				} break;

				case 475: // Componentes - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500091);
					return true;
				} break;

				case 476: // Proyectiles - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500072);
					return true;
				} break;

				case 477: // Vendas - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500012);
					return true;
				} break;

				case 451: // Monturas & Mascotas - Submenu
				{
					player->ADD_GOSSIP_ITEM(5, "Monturas", GOSSIP_SENDER_MAIN, 478);
					player->ADD_GOSSIP_ITEM(6, "Mascotas", GOSSIP_SENDER_MAIN, 483);
					player->ADD_GOSSIP_ITEM(2, "<- Volver", GOSSIP_SENDER_MAIN, 402);
					player->ADD_GOSSIP_ITEM(2, "<-- Menú principal del Libro", GOSSIP_SENDER_MAIN, 0);
					player->SEND_GOSSIP_MENU(1, item->GetGUID());
					return true;
				} break;

				case 478: // Monturas - Submenu
				{
					player->ADD_GOSSIP_ITEM(6, "Monturas normales", GOSSIP_SENDER_MAIN, 480);
					player->ADD_GOSSIP_ITEM(6, "Monturas épicas", GOSSIP_SENDER_MAIN, 481);
					player->ADD_GOSSIP_ITEM(6, "Monturas voladoras", GOSSIP_SENDER_MAIN, 482);
					player->ADD_GOSSIP_ITEM(2, "<- Volver", GOSSIP_SENDER_MAIN, 451);
					player->ADD_GOSSIP_ITEM(2, "<-- Menú principal del Libro", GOSSIP_SENDER_MAIN, 0);
					player->SEND_GOSSIP_MENU(1, item->GetGUID());
					return true;
				} break;

				case 480: // Monturas normales - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500062);
					return true;
				} break;

				case 481: // Monturas épicas - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500033);
					return true;
				} break;

				case 482: // Monturas voladoras - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500039);
					return true;
				} break;

				case 483: // Mascotas - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500069);
					return true;
				} break;

				case 452: // Camisas - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500099);
					return true;
				} break;

				case 453: // Tabardos - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500101);
					return true;
				} break;

				case 803: //Arena Utilities - Submenu
				{
					/*player->ADD_GOSSIP_ITEM(7, "Apuntador de Arenas", GOSSIP_SENDER_MAIN, 600);
					player->ADD_GOSSIP_ITEM(7, "Creador de Grupos de Arenas", GOSSIP_SENDER_MAIN, 601);*/
					player->ADD_GOSSIP_ITEM(7, "|cff00ff00|TInterface\\icons\\INV_Misc_Eye_04:30:30:-15:0|t|rArena Spectator", GOSSIP_SENDER_MAIN, 602);
					//player->ADD_GOSSIP_ITEM(7, "|cff00ff00|TInterface\\icons\\inv_misc_note_03:30:30:-15:0|t|rTop de Arenas", GOSSIP_SENDER_MAIN, 603);
					player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\Achievement_BG_returnXflags_def_WSG:30:30:-15:0|t|rMenú principal del Libro", GOSSIP_SENDER_MAIN, 0);

					player->SEND_GOSSIP_MENU(1, item->GetGUID());
					return true;
				} break;

				/*case 600: // Apuntador de Arenas - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500101);
					return true;
				} break;

				case 601: // Creador de Grupos de Arenas - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500101);
					return true;
				} break;*/

				case 602: // Arena Spectator - Submenu
				{
					player->ADD_GOSSIP_ITEM(9, "|cff00ff00|TInterface\\icons\\Achievement_arena_2v2_7:30:30:-15:0|t|rPartidas: 2c2", GOSSIP_SENDER_MAIN, 1000);
					player->ADD_GOSSIP_ITEM(9, "|cff00ff00|TInterface\\icons\\Achievement_arena_3v3_7:30:30:-15:0|t|rPartidas: 3c3", GOSSIP_SENDER_MAIN, 2000);
					player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\Achievement_BG_returnXflags_def_WSG:30:30:-15:0|t|rMenú principal del Libro", GOSSIP_SENDER_MAIN, 0);
					player->SEND_GOSSIP_MENU(1, item->GetGUID());
					return true;
				} break;

				////////////////////////////////////////////   TOP DE ARENAS - INICIO     ////////////////////////////////////////////

				case 603: // Top de Arenas - Submenu
				{
					player->ADD_GOSSIP_ITEM(4,"|cff00ff00|TInterface\\icons\\spell_chargepositive:30:30:-15:0|t|rTop 10: Arenas 2c2", GOSSIP_SENDER_MAIN, ARENA_2V2_LADDER);
					//player->ADD_GOSSIP_ITEM_EXTENDED(4, "|cff00ff00|TInterface\\icons\\Achievement_Arena_2v2_4:30:30:-15:0|t|rEstadísticas de un Jugador", GOSSIP_SENDER_MAIN, 700, "Inserta el nombre del jugador y pulsa el botón ""Aceptar""", 0, true);
					//player->ADD_GOSSIP_ITEM_EXTENDED(0, "|cff00ff00|TInterface\\icons\\Achievement_Arena_2v2_6:30:30:-15:0|t|rEstadísticas de un Equipo de Arenas", GOSSIP_SENDER_MAIN, 701, "Inserta el nombre del equipo y pulsa el botón ""Aceptar""", 0, true);
					player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\Achievement_BG_returnXflags_def_WSG:30:30:-15:0|t|rMenú principal del Libro", GOSSIP_SENDER_MAIN, 0);
					player->SEND_GOSSIP_MENU(1, item->GetGUID());
					return true;
				} break;

				case 4+1700:
					SendSoloInfo(player, item, GetArenaTeamId_item(sender, 2), sender, 2);
					return true;
				break;

				case 5+1700:
					SendSoloInfo(player, item, GetArenaTeamId_item(sender, 3), sender, 3);
					return true;
				break;

				case 6+1700:
					SendSoloInfo(player, item, GetArenaTeamId_item(sender, 5), sender, 5);
					return true;
				break;

				case ARENA_GOODBYE:
				{
					player->PlayerTalkClass->SendCloseGossip();
					return true;
				} break;

				case ARENA_2V2_LADDER:
				{
					uint32 teamType = optionToTeamType(action);
					QueryResult result = CharacterDatabase.PQuery("SELECT arenaTeamId, name, rating FROM arena_team WHERE type = '%u' ORDER BY rating DESC LIMIT %u;", teamType, ARENA_MAX_RESULTS);

					if(!result)
					{
						player->ADD_GOSSIP_ITEM(7, "Olvídalo", GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
						player->SEND_GOSSIP_MENU(1, item->GetGUID());
					}
					else
					{
						std::string name;
						uint32 teamId, rating, rank = 1;
						player->ADD_GOSSIP_ITEM(0, "Lista del Top de Arenas:", GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
						do
						{
							Field *fields = result->Fetch();
							teamId = fields[0].GetUInt32();
							name = fields[1].GetString();
							rating = fields[2].GetUInt32();

							std::stringstream buffer;
							buffer << rank << ". " << name;
							buffer << ": " << "|cffe30000" << rating << "|r" << " de índice";
							player->ADD_GOSSIP_ITEM(4, buffer.str(), GOSSIP_SENDER_MAIN, ARENA_START_TEAM_LOOKUP + teamId);

							rank++;
						}
						while(result->NextRow());
						player->ADD_GOSSIP_ITEM(7, "Olvídalo", GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
						player->SEND_GOSSIP_MENU(1, item->GetGUID());
					}
					return true;
				} break;

				default:
				{
					if (action > ARENA_START_TEAM_LOOKUP && action < 9900)
					{
						uint32 teamId = action - ARENA_START_TEAM_LOOKUP;
						QueryResult result = CharacterDatabase.PQuery("SELECT name, rating, seasonWins, seasonGames - seasonWins, weekWins, weekGames - weekWins, rank, captainGuid, type FROM arena_team WHERE arenaTeamId = '%u'", teamId);

						if(!result)
						{
							player->GetSession()->SendNotification("Equipo de arenas no encontrado");
							player->PlayerTalkClass->SendCloseGossip();
							return true;
						}

						Field *fields = result->Fetch();
						std::string name = fields[0].GetString();
						uint32 rating = fields[1].GetUInt32();
						uint32 seasonWins = fields[2].GetUInt32();
						uint32 seasonLosses = fields[3].GetUInt32();
						uint32 weekWins = fields[4].GetUInt32();
						uint32 weekLosses = fields[5].GetUInt32();
						uint32 rank = fields[6].GetUInt32();
						uint32 captainGuid = fields[7].GetUInt32();
						uint32 type = fields[8].GetUInt32();
						uint32 parentOption = teamTypeToOption(type);

						std::string seasonWinPercentage = getWinPercent_item(seasonWins, seasonLosses);
						std::string weekWinPercentage = getWinPercent_item(weekWins, weekLosses);

						std::stringstream buf;
						buf << "Nombre del equipo: " << "|cffff8000" << name << "|r";
						player->ADD_GOSSIP_ITEM(7, buf.str(), GOSSIP_SENDER_MAIN, parentOption);
						buf.str("");

						buf << "Índice: " << "|cffe30000" << rating << "|r" << " (Rango: " << "|cffe30000" << rank << "|r" << ", Tipo: " << "|cffe30000" << type << "c" << type << "|r"")";
						player->ADD_GOSSIP_ITEM(4, buf.str(), GOSSIP_SENDER_MAIN, parentOption);
						buf.str("");

						/*buf << "Total de la Semana: " << "|cffe30000" << weekWins << "-" << weekLosses << "|r"" (" << "|cffe30000" << weekWinPercentage << "|r" << "victorias)";
						player->ADD_GOSSIP_ITEM(4, buf.str(), GOSSIP_SENDER_MAIN, parentOption);
						buf.str("");*/

						buf << "Total de la Temporada: " << "|cffe30000" << seasonWins << "-" << seasonLosses << "|r" << " (" << "|cffe30000" << seasonWinPercentage << "|r" << "victorias)";
						player->ADD_GOSSIP_ITEM(4, buf.str(), GOSSIP_SENDER_MAIN, parentOption);

						QueryResult members = CharacterDatabase.PQuery("SELECT  a.guid, a.personalRating, a.weekWins, a.weekGames - a.weekWins, a.seasonWins, a.seasonGames - a.seasonWins, c.name, c.race, c.class, c.level FROM arena_team_member a LEFT JOIN characters c ON c.guid = a.guid WHERE arenaTeamId = '%u' ORDER BY a.guid = '%u' DESC, a.seasonGames DESC, c.name ASC", teamId, captainGuid);
						if(!members)
						{
							player->ADD_GOSSIP_ITEM(7, "No se encuentran miembros en el equipo", GOSSIP_SENDER_MAIN, parentOption);
						}
						else
						{
							uint32 memberPos = 1;
							uint32 memberCount = members->GetRowCount();
							uint32 guid, personalRating, level;
							std::string name, race, Class;

							buf.str("");
							buf << "----- " << memberCount << ((memberCount == 1) ? " miembro" : " miembros") << ((memberCount == 1) ? " encontrado" : " encontrados") << " -----";
							player->ADD_GOSSIP_ITEM(0, buf.str(), GOSSIP_SENDER_MAIN, parentOption);
							do
							{
								fields = members->Fetch();
								guid = fields[0].GetUInt32();
								personalRating = fields[1].GetUInt32();
								weekWins= fields[2].GetUInt32();
								weekLosses = fields[3].GetUInt32();
								seasonWins = fields[4].GetUInt32();
								seasonLosses = fields[5].GetUInt32();
								name = fields[6].GetString();
								race = raceToString_item(fields[7].GetUInt8());
								Class = classToString_item(fields[8].GetUInt8());
								level = fields[9].GetUInt32();

								seasonWinPercentage = getWinPercent_item(seasonWins, seasonLosses);
								weekWinPercentage = getWinPercent_item(weekWins, weekLosses);

								buf.str("");
								buf << memberPos << ". ";
								if (guid == captainGuid)
									buf <<  "Capitan del Equipo: ";

								buf << name << ", " /*<< getPlayerStatus_item(guid)*/;
								player->ADD_GOSSIP_ITEM(7, buf.str(), GOSSIP_SENDER_MAIN, parentOption);
								buf.str("");

								buf << race << " " << Class << ", " << "|cffe30000" << personalRating << "|r" << " de índice personal";
								player->ADD_GOSSIP_ITEM(4, buf.str(), GOSSIP_SENDER_MAIN, parentOption);
								buf.str("");

								/*buf << "Semana: " << "|cffe30000" << weekWins << "-" << weekLosses << "|r" << " (" << "|cffe30000" << weekWinPercentage << "|r" << "victorias), " << "|cffe30000" << (weekWins + weekLosses) << "|r" << " jugadas";
								player->ADD_GOSSIP_ITEM(4, buf.str(), GOSSIP_SENDER_MAIN, parentOption);
								buf.str("");*/

								buf << "Temporada: " << "|cffe30000" << seasonWins << "-" << seasonLosses << "|r" << " (" << "|cffe30000" << seasonWinPercentage << "|r" << "victorias), " << "|cffe30000" << (seasonWins + seasonLosses) << "|r" << " jugadas";
								player->ADD_GOSSIP_ITEM(4, buf.str(), GOSSIP_SENDER_MAIN, parentOption);
								memberPos++;
							}
							while(members->NextRow());
						}
						player->SEND_GOSSIP_MENU(1, item->GetGUID());
						return true;
					}
				} break;

				////////////////////////////////////////////   TOP DE ARENAS - FIN        ////////////////////////////////////////////

				case 804: //Utility NPCs - Submenu
				{
					player->ADD_GOSSIP_ITEM(7, "|cff00ff00|TInterface\\icons\\inv_enchant_disenchant:30:30:-15:0|t|rEnchanter", GOSSIP_SENDER_MAIN, 900);
					player->ADD_GOSSIP_ITEM(7, "|cff00ff00|TInterface\\icons\\achievement_reputation_03:30:30:-15:0|t|rIntercambiador de Puntos", GOSSIP_SENDER_MAIN, 901);
					//player->ADD_GOSSIP_ITEM(7, "Guildmaster", GOSSIP_SENDER_MAIN, 902);
					player->ADD_GOSSIP_ITEM(7, "|cff00ff00|TInterface\\icons\\achievement_reputation_argentcrusader:30:30:-15:0|t|rTítulos por Objetos de la Zona PvP", GOSSIP_SENDER_MAIN, 903);
					player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/Inv_misc_note_01:30:30:-15:0|tProfesiones", GOSSIP_SENDER_MAIN, 805);
					player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/Spell_holy_consumemagic:30:30:-15:0|tTransfiguración", GOSSIP_SENDER_MAIN, 806);
					player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\Achievement_BG_returnXflags_def_WSG:30:30:-15:0|t|rMenú principal del Libro", GOSSIP_SENDER_MAIN, 0);

					player->SEND_GOSSIP_MENU(1, item->GetGUID());
					return true;
				} break;

				////////////////////////////////////////////   ENCHANTER - INICIO        ////////////////////////////////////////////

				case 900: // Enchanter - Submenu
				{
					bool count = false;

					if(isEquiped(player, EQUIPMENT_SLOT_HEAD))
					{
						Item* newItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD);
						uint32 entry = sT->GetFakeEntry(newItem->GetGUID());
						std::string icon = entry ? sT->GetItemIcon(entry, 30, 30, -15, 0) : sT->GetItemIcon(newItem->GetEntry(), 30, 30, -15, 0);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, icon+"Encantar Casco", EQUIPMENT_SLOT_HEAD, 7000+2);
						count = true;
					}

					if(isEquiped(player, EQUIPMENT_SLOT_SHOULDERS))
					{
						Item* newItem2 = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS);
						uint32 entry2 = sT->GetFakeEntry(newItem2->GetGUID());
						std::string icon2 = entry2 ? sT->GetItemIcon(entry2, 30, 30, -15, 0) : sT->GetItemIcon(newItem2->GetEntry(), 30, 30, -15, 0);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, icon2+"Encantar Hombreras", EQUIPMENT_SLOT_SHOULDERS, 7000+3);
						count = true;
					}

					if(isEquiped(player, EQUIPMENT_SLOT_BACK))
					{
						Item* newItem3 = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK);
						uint32 entry3 = sT->GetFakeEntry(newItem3->GetGUID());
						std::string icon3 = entry3 ? sT->GetItemIcon(entry3, 30, 30, -15, 0) : sT->GetItemIcon(newItem3->GetEntry(), 30, 30, -15, 0);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, icon3+"Encantar Espalda", EQUIPMENT_SLOT_BACK, 7000+4);
						count = true;
					}

					if(isEquiped(player, EQUIPMENT_SLOT_CHEST))
					{
						Item* newItem4 = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST);
						uint32 entry4 = sT->GetFakeEntry(newItem4->GetGUID());
						std::string icon4 = entry4 ? sT->GetItemIcon(entry4, 30, 30, -15, 0) : sT->GetItemIcon(newItem4->GetEntry(), 30, 30, -15, 0);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, icon4+"Encantar Pechera", EQUIPMENT_SLOT_CHEST, 7000+5);
						count = true;
					}

					if(isEquiped(player, EQUIPMENT_SLOT_WRISTS))
					{
						Item* newItem5 = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS);
						uint32 entry5 = sT->GetFakeEntry(newItem5->GetGUID());
						std::string icon5 = entry5 ? sT->GetItemIcon(entry5, 30, 30, -15, 0) : sT->GetItemIcon(newItem5->GetEntry(), 30, 30, -15, 0);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, icon5+"Encantar Mu""\xC3\xB1""ecas", EQUIPMENT_SLOT_WRISTS, 7000+6);
						count = true;
					}

					if(isEquiped(player, EQUIPMENT_SLOT_HANDS))
					{
						Item* newItem6 = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS);
						uint32 entry6 = sT->GetFakeEntry(newItem6->GetGUID());
						std::string icon6 = entry6 ? sT->GetItemIcon(entry6, 30, 30, -15, 0) : sT->GetItemIcon(newItem6->GetEntry(), 30, 30, -15, 0);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, icon6+"Encantar Guantes", EQUIPMENT_SLOT_HANDS, 7000+7);
						count = true;
					}

					if(isEquiped(player, EQUIPMENT_SLOT_LEGS))
					{
						Item* newItem7 = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS);
						uint32 entry7 = sT->GetFakeEntry(newItem7->GetGUID());
						std::string icon7 = entry7 ? sT->GetItemIcon(entry7, 30, 30, -15, 0) : sT->GetItemIcon(newItem7->GetEntry(), 30, 30, -15, 0);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, icon7+"Encantar Piernas", EQUIPMENT_SLOT_LEGS, 7000+8);
						count = true;
					}

					if(isEquiped(player, EQUIPMENT_SLOT_FEET))
					{
						Item* newItem8 = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET);
						uint32 entry8 = sT->GetFakeEntry(newItem8->GetGUID());
						std::string icon8 = entry8 ? sT->GetItemIcon(entry8, 30, 30, -15, 0) : sT->GetItemIcon(newItem8->GetEntry(), 30, 30, -15, 0);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, icon8+"Encantar Botas", EQUIPMENT_SLOT_FEET, 7000+9);
						count = true;
					}

					if (player->HasSkill(SKILL_ENCHANTING) && player->GetSkillValue(SKILL_ENCHANTING) == 450)
					{
						if(isEquiped(player, EQUIPMENT_SLOT_FINGER1))
						{
							Item* newItem9 = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER1);
							uint32 entry9 = sT->GetFakeEntry(newItem9->GetGUID());
							std::string icon9 = entry9 ? sT->GetItemIcon(entry9, 30, 30, -15, 0) : sT->GetItemIcon(newItem9->GetEntry(), 30, 30, -15, 0);
							player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, icon9+"Encantar Anillo 1", EQUIPMENT_SLOT_FINGER1, 7000+10);
							count = true;
						}

						if(isEquiped(player, EQUIPMENT_SLOT_FINGER2))
						{
							Item* newItem10 = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER2);
							uint32 entry10 = sT->GetFakeEntry(newItem10->GetGUID());
							std::string icon10 = entry10 ? sT->GetItemIcon(entry10, 30, 30, -15, 0) : sT->GetItemIcon(newItem10->GetEntry(), 30, 30, -15, 0);
							player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, icon10+"Encantar Anillo 2", EQUIPMENT_SLOT_FINGER2, 7000+11);
							count = true;
						}
					}

					if(isEquiped(player, EQUIPMENT_SLOT_MAINHAND))
					{
						Item* newItem11 = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
						uint32 entry11 = sT->GetFakeEntry(newItem11->GetGUID());
						std::string icon11 = entry11 ? sT->GetItemIcon(entry11, 30, 30, -15, 0) : sT->GetItemIcon(newItem11->GetEntry(), 30, 30, -15, 0);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, icon11+"Encantar Mano Principal", EQUIPMENT_SLOT_MAINHAND, 7000+12);
						count = true;
					}

					if(isEquiped(player, EQUIPMENT_SLOT_OFFHAND))
					{
						Item* newItem12 = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
						uint32 entry12 = sT->GetFakeEntry(newItem12->GetGUID());
						std::string icon12 = entry12 ? sT->GetItemIcon(entry12, 30, 30, -15, 0) : sT->GetItemIcon(newItem12->GetEntry(), 30, 30, -15, 0);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, icon12+"Encantar Mano Secundaria", EQUIPMENT_SLOT_OFFHAND, 7000+13);
						count = true;
					}

					player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\Achievement_BG_returnXflags_def_WSG:30:30:-15:0|t|rMenú principal del Libro", GOSSIP_SENDER_MAIN, 0);
					//player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "|TInterface/ICONS/Ability_Spy:30:30:-15:0|tOlv""\xC3\xAD""dalo", GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
					player->SEND_GOSSIP_MENU(68, item->GetGUID());

					if(!count)
						ChatHandler(player).PSendSysMessage("|cfffcedbbNo tienes objetos equipados que se puedan encantar.|r");
					return true;
				} break;

				case 7000+1:
					OnGossipSelect(player, item, 1, 900);
                break;

				case 7000+2:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "<- Volver al Men""\xC3\xBA"" principal", GOSSIP_SENDER_MAIN, 7000+1);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+50 AP y 20 Critico", GOSSIP_SENDER_MAIN, 7000+21);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+50 AP y 20 Temple", GOSSIP_SENDER_MAIN, 7000+16);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+30 SP y 10 MP5", GOSSIP_SENDER_MAIN, 7000+19);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+30 SP y 20 Critico", GOSSIP_SENDER_MAIN, 7000+20);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+29 SP y 20 Temple", GOSSIP_SENDER_MAIN, 7000+17);
                /*player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Arcanum de la Luna Eclipsada", GOSSIP_SENDER_MAIN, 7000+23);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Arcanum del alma de la llama", GOSSIP_SENDER_MAIN, 7000+24);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Arcanum de la sombra huida", GOSSIP_SENDER_MAIN, 7000+25);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Arcanum del alma helada", GOSSIP_SENDER_MAIN, 7000+26);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Arcanum de amparo toxico", GOSSIP_SENDER_MAIN, 7000+27);*/
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+30 Aguante y 25 Temple", GOSSIP_SENDER_MAIN, 7000+18);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+37 Aguante y 20 Defensa", GOSSIP_SENDER_MAIN, 7000+22);
                player->SEND_GOSSIP_MENU(68, item->GetGUID());
                break;
				case 7000+16:
					Enchant(player, item, EQUIPMENT_SLOT_HEAD, 3795);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+17:
					Enchant(player, item, EQUIPMENT_SLOT_HEAD, 3797);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+18:
					Enchant(player, item, EQUIPMENT_SLOT_HEAD, 3842);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+19:
					Enchant(player, item, EQUIPMENT_SLOT_HEAD, 3819);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+20:
					Enchant(player, item, EQUIPMENT_SLOT_HEAD, 3820);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+21:
					Enchant(player, item, EQUIPMENT_SLOT_HEAD, 3817);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+22:
					Enchant(player, item, EQUIPMENT_SLOT_HEAD, 3818);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+23:
					Enchant(player, item, EQUIPMENT_SLOT_HEAD, 3815);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+24:
					Enchant(player, item, EQUIPMENT_SLOT_HEAD, 3816);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+25:
					Enchant(player, item, EQUIPMENT_SLOT_HEAD, 3814);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+26:
					Enchant(player, item, EQUIPMENT_SLOT_HEAD, 3812);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+27:
					Enchant(player, item, EQUIPMENT_SLOT_HEAD, 3813);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+3:
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "<- Volver al Men""\xC3\xBA"" principal", GOSSIP_SENDER_MAIN, 7000+1);
					if (player->HasSkill(SKILL_INSCRIPTION) && player->GetSkillValue(SKILL_INSCRIPTION) == 450)
					{
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+120 AP y 15 Critico", GOSSIP_SENDER_MAIN, 7000+31);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+70 SP y 8 MP5", GOSSIP_SENDER_MAIN, 7000+32);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+70 SP y 15 Critico", GOSSIP_SENDER_MAIN, 7000+34);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+70 Esquivar y 15 Defensa", GOSSIP_SENDER_MAIN, 7000+33);
					}
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+30 Aguante y 15 Temple", GOSSIP_SENDER_MAIN, 7000+28);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+40 AP y 15 Temple", GOSSIP_SENDER_MAIN, 7000+29);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+40 AP y 15 Critico", GOSSIP_SENDER_MAIN, 7000+195);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+23 SP y 15 Temple", GOSSIP_SENDER_MAIN, 7000+30);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+24 SP y 15 Critico", GOSSIP_SENDER_MAIN, 7000+197);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+24 SP y 8 MP5", GOSSIP_SENDER_MAIN, 7000+196);
					player->SEND_GOSSIP_MENU(68, item->GetGUID());
					break;
				case 7000+28:
					Enchant(player, item, EQUIPMENT_SLOT_SHOULDERS, 3852);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+29:
					Enchant(player, item, EQUIPMENT_SLOT_SHOULDERS, 3793);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+30:
					Enchant(player, item, EQUIPMENT_SLOT_SHOULDERS, 3794);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+31:
					Enchant(player, item, EQUIPMENT_SLOT_SHOULDERS, 3835);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+32:
					Enchant(player, item, EQUIPMENT_SLOT_SHOULDERS, 3836);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+33:
					Enchant(player, item, EQUIPMENT_SLOT_SHOULDERS, 3837);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+34:
					Enchant(player, item, EQUIPMENT_SLOT_SHOULDERS, 3838);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+195:
					Enchant(player, item, EQUIPMENT_SLOT_SHOULDERS, 3808);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+196:
					Enchant(player, item, EQUIPMENT_SLOT_SHOULDERS, 3809);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+197:
					Enchant(player, item, EQUIPMENT_SLOT_SHOULDERS, 3810);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+4:
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "<- Volver al Men""\xC3\xBA"" principal", GOSSIP_SENDER_MAIN, 7000+1);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+10 Agilidad y Sigilo aumentado", GOSSIP_SENDER_MAIN, 7000+35);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+22 Agilidad", GOSSIP_SENDER_MAIN, 7000+40);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+23 Indice de Celeridad", GOSSIP_SENDER_MAIN, 7000+38);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+35 Penetracion de hechizos", GOSSIP_SENDER_MAIN, 7000+41);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+10 Espiritu y -2% Amenaza", GOSSIP_SENDER_MAIN, 7000+36);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+225 Armadura", GOSSIP_SENDER_MAIN, 7000+39);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+16 Indice de Defensa", GOSSIP_SENDER_MAIN, 7000+37);
					if (player->HasSkill(SKILL_TAILORING) && player->GetSkillValue(SKILL_TAILORING) == 450)
					{
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Bordado de resplandor oscuro", GOSSIP_SENDER_MAIN, 7000+42);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Bordado de tejido de luz", GOSSIP_SENDER_MAIN, 7000+43);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Bordado de guardia de espada", GOSSIP_SENDER_MAIN, 7000+44);
					}
					player->SEND_GOSSIP_MENU(68, item->GetGUID());
					break;
				case 7000+35:
					Enchant(player, item, EQUIPMENT_SLOT_BACK, 3256);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+36:
					Enchant(player, item, EQUIPMENT_SLOT_BACK, 3296);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+37:
					Enchant(player, item, EQUIPMENT_SLOT_BACK, 1951);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+38:
					Enchant(player, item, EQUIPMENT_SLOT_BACK, 3831);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+39:
					Enchant(player, item, EQUIPMENT_SLOT_BACK, 3294);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+40:
					Enchant(player, item, EQUIPMENT_SLOT_BACK, 1099);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+41:
					Enchant(player, item, EQUIPMENT_SLOT_BACK, 3243);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+42:
					Enchant(player, item, EQUIPMENT_SLOT_BACK, 3728);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+43:
					Enchant(player, item, EQUIPMENT_SLOT_BACK, 3722);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+44:
					Enchant(player, item, EQUIPMENT_SLOT_BACK, 3730);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+5:
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "<- Volver al Men""\xC3\xBA"" principal", GOSSIP_SENDER_MAIN, 7000+1);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+10 Todas las estadisticas", GOSSIP_SENDER_MAIN, 7000+47);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+275 Puntos de salud", GOSSIP_SENDER_MAIN, 7000+48);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+10 MP5", GOSSIP_SENDER_MAIN, 7000+49);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+20 Temple", GOSSIP_SENDER_MAIN, 7000+50);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+20 Indice de Defensa", GOSSIP_SENDER_MAIN, 7000+51);
					player->SEND_GOSSIP_MENU(68, item->GetGUID());
					break;
				case 7000+47:
					Enchant(player, item, EQUIPMENT_SLOT_CHEST, 3832);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+48:
					Enchant(player, item, EQUIPMENT_SLOT_CHEST, 3297);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+49:
					Enchant(player, item, EQUIPMENT_SLOT_CHEST, 2381);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+50:
					Enchant(player, item, EQUIPMENT_SLOT_CHEST, 3245);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+51:
					Enchant(player, item, EQUIPMENT_SLOT_CHEST, 1953);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+6:
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "<- Volver al Men""\xC3\xBA"" principal", GOSSIP_SENDER_MAIN, 7000+1);
					if (player->HasSkill(SKILL_LEATHERWORKING) && player->GetSkillValue(SKILL_LEATHERWORKING) == 450)
					{
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Forro de pelaje de Aguante", GOSSIP_SENDER_MAIN, 7000+52);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Forro de pelaje de Poder con Hechizos", GOSSIP_SENDER_MAIN, 7000+53);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Forro de pelaje de Poder de Ataque", GOSSIP_SENDER_MAIN, 7000+54);
					}
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+18 Espiritu", GOSSIP_SENDER_MAIN, 7000+55);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+15 Pericia", GOSSIP_SENDER_MAIN, 7000+56);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+6 Todas las estadisticas", GOSSIP_SENDER_MAIN, 7000+57);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+16 Intelecto", GOSSIP_SENDER_MAIN, 7000+58);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+30 Poder con hechizos", GOSSIP_SENDER_MAIN, 7000+122);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+50 Poder de ataque", GOSSIP_SENDER_MAIN, 7000+123);
					player->SEND_GOSSIP_MENU(68, item->GetGUID());
					break;
				case 7000+52:
					Enchant(player, item, EQUIPMENT_SLOT_WRISTS, 3757);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+53:
					Enchant(player, item, EQUIPMENT_SLOT_WRISTS, 3758);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+54:
					Enchant(player, item, EQUIPMENT_SLOT_WRISTS, 3756);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+55:
					Enchant(player, item, EQUIPMENT_SLOT_WRISTS, 1147);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+56:
					Enchant(player, item, EQUIPMENT_SLOT_WRISTS, 3231);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+57:
					Enchant(player, item, EQUIPMENT_SLOT_WRISTS, 2661);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+58:
					Enchant(player, item, EQUIPMENT_SLOT_WRISTS, 1119);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+122:
					Enchant(player, item, EQUIPMENT_SLOT_WRISTS, 2332);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+123:
					Enchant(player, item, EQUIPMENT_SLOT_WRISTS, 3845);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+7:
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "<- Volver al Men""\xC3\xBA"" principal", GOSSIP_SENDER_MAIN, 7000+1);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+16 Critico", GOSSIP_SENDER_MAIN, 7000+59);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+10 Indice de Parada y 2% Amenaza", GOSSIP_SENDER_MAIN, 7000+60);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+44 Poder de ataque", GOSSIP_SENDER_MAIN, 7000+61);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+28 Poder con hechizos", GOSSIP_SENDER_MAIN, 7000+120);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+20 Agilidad", GOSSIP_SENDER_MAIN, 7000+62);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+20 Indice de Golpe", GOSSIP_SENDER_MAIN, 7000+63);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+15 Indice de Pericia", GOSSIP_SENDER_MAIN, 7000+64);
					player->SEND_GOSSIP_MENU(68, item->GetGUID());
					break;
				case 7000+59:
					Enchant(player, item, EQUIPMENT_SLOT_HANDS, 3249);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+60:
					Enchant(player, item, EQUIPMENT_SLOT_HANDS, 3253);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+61:
					Enchant(player, item, EQUIPMENT_SLOT_HANDS, 1603);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+62:
					Enchant(player, item, EQUIPMENT_SLOT_HANDS, 3222);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+63:
					Enchant(player, item, EQUIPMENT_SLOT_HANDS, 3234);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+64:
					Enchant(player, item, EQUIPMENT_SLOT_HANDS, 3231);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+120:
					Enchant(player, item, EQUIPMENT_SLOT_HANDS, 3246);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+121:
					Enchant(player, item, EQUIPMENT_SLOT_HANDS, 3603);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+8:
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "<- Volver al Men""\xC3\xBA"" principal", GOSSIP_SENDER_MAIN, 7000+1);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+40 Temple y 28 Aguante", GOSSIP_SENDER_MAIN, 7000+65);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+55 Aguante y 22 Agilidad", GOSSIP_SENDER_MAIN, 7000+66);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+75 AP y 22 Critico", GOSSIP_SENDER_MAIN, 7000+67);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+50 SP y 20 Espiritu", GOSSIP_SENDER_MAIN, 7000+68);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+50 SP y 30 Aguante", GOSSIP_SENDER_MAIN, 7000+69);
					player->SEND_GOSSIP_MENU(68, item->GetGUID());
					break;
				case 7000+65:
					Enchant(player, item, EQUIPMENT_SLOT_LEGS, 3853);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+66:
					Enchant(player, item, EQUIPMENT_SLOT_LEGS, 3822);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+67:
					Enchant(player, item, EQUIPMENT_SLOT_LEGS, 3823);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+68:
					Enchant(player, item, EQUIPMENT_SLOT_LEGS, 3719);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+69:
					Enchant(player, item, EQUIPMENT_SLOT_LEGS, 3721);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+9:
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "<- Volver al Men""\xC3\xBA"" principal", GOSSIP_SENDER_MAIN, 7000+1);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+32 Poder de ataque", GOSSIP_SENDER_MAIN, 7000+70);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+15 Aguante y Velocidad aumentada", GOSSIP_SENDER_MAIN, 7000+71);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+16 Agilidad", GOSSIP_SENDER_MAIN, 7000+72);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+18 Espiritu", GOSSIP_SENDER_MAIN, 7000+73);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+7 Salud y Mana cada 5s", GOSSIP_SENDER_MAIN, 7000+74);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+12 Indice de golpe y 12 Critico", GOSSIP_SENDER_MAIN, 7000+75);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+22 Aguante", GOSSIP_SENDER_MAIN, 7000+76);
					//player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Propulsiones de nitro", GOSSIP_SENDER_MAIN, 7000+77);
					player->SEND_GOSSIP_MENU(68, item->GetGUID());
					break;
				case 7000+70:
					Enchant(player, item, EQUIPMENT_SLOT_FEET, 1597);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+71:
					Enchant(player, item, EQUIPMENT_SLOT_FEET, 3232);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+72:
					Enchant(player, item, EQUIPMENT_SLOT_FEET, 983);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+73:
					Enchant(player, item, EQUIPMENT_SLOT_FEET, 1147);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+74:
					Enchant(player, item, EQUIPMENT_SLOT_FEET, 3244);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+75:
					Enchant(player, item, EQUIPMENT_SLOT_FEET, 3826);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+76:
					Enchant(player, item, EQUIPMENT_SLOT_FEET, 1075);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+77:
					Enchant(player, item, EQUIPMENT_SLOT_FEET, 3606);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+10:
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "<- Volver al Men""\xC3\xBA"" principal", GOSSIP_SENDER_MAIN, 7000+1);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Asalto", GOSSIP_SENDER_MAIN, 7000+78);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Poder con hechizos superior", GOSSIP_SENDER_MAIN, 7000+79);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Aguante", GOSSIP_SENDER_MAIN, 7000+80);
					player->SEND_GOSSIP_MENU(68, item->GetGUID());
					break;
				case 7000+78:
					Enchant(player, item, EQUIPMENT_SLOT_FINGER1, 3839);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+79:
					Enchant(player, item, EQUIPMENT_SLOT_FINGER1, 3840);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+80:
					Enchant(player, item, EQUIPMENT_SLOT_FINGER1, 3791);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+11:
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "<- Volver al Men""\xC3\xBA"" principal", GOSSIP_SENDER_MAIN, 7000+1);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Asalto", GOSSIP_SENDER_MAIN, 7000+81);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Poder con hechizos superior", GOSSIP_SENDER_MAIN, 7000+82);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Aguante", GOSSIP_SENDER_MAIN, 7000+83);
					player->SEND_GOSSIP_MENU(68, item->GetGUID());
					break;
				case 7000+81:
					Enchant(player, item, EQUIPMENT_SLOT_FINGER2, 3839);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+82:
					Enchant(player, item, EQUIPMENT_SLOT_FINGER2, 3840);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+83:
					Enchant(player, item, EQUIPMENT_SLOT_FINGER2, 3791);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+12:
				{
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "<- Volver al Men""\xC3\xBA"" principal", GOSSIP_SENDER_MAIN, 7000+1);

					Item* item_20 = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
					if (!item_20)
						return true;

					if (validItem(2, item_20))
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Masacre", GOSSIP_SENDER_MAIN, 7000+98);
					if (validItem(3, item_20))
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Poder con Hechizos superior", GOSSIP_SENDER_MAIN, 7000+99);
					if (validItem(1, item_20))
					{
						if (!validItem(2, item_20))
							player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Potencia Superior", GOSSIP_SENDER_MAIN, 7000+89);
						if (!validItem(3, item_20))
							player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Poder con Hechizos Poderoso", GOSSIP_SENDER_MAIN, 7000+88);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Precision", GOSSIP_SENDER_MAIN, 7000+85);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Agilidad Excepcional", GOSSIP_SENDER_MAIN, 7000+94);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Espiritu Excepcional", GOSSIP_SENDER_MAIN, 7000+95);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Guardia Titan", GOSSIP_SENDER_MAIN, 7000+84);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Rabiar", GOSSIP_SENDER_MAIN, 7000+86);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Magia Negra", GOSSIP_SENDER_MAIN, 7000+87);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Verdugo", GOSSIP_SENDER_MAIN, 7000+96);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Mangosta", GOSSIP_SENDER_MAIN, 7000+97);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Rompehielo", GOSSIP_SENDER_MAIN, 7000+90);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Guardian de vida", GOSSIP_SENDER_MAIN, 7000+91);
						//player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Cadena de titanio", GOSSIP_SENDER_MAIN, 7000+199);
						if (player->HasSkill(SKILL_ENCHANTING) && player->GetSkillValue(SKILL_ENCHANTING) == 450)
						{
							player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Drenador de sangre", GOSSIP_SENDER_MAIN, 7000+92);
							player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Amparo de hojas", GOSSIP_SENDER_MAIN, 7000+93);
						}
					}

					player->SEND_GOSSIP_MENU(68, item->GetGUID());
				}   break;
				case 7000+84:
					Enchant(player, item, EQUIPMENT_SLOT_MAINHAND, 3851, 1);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+85:
					Enchant(player, item, EQUIPMENT_SLOT_MAINHAND, 3788, 1);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+86:
					Enchant(player, item, EQUIPMENT_SLOT_MAINHAND, 3789, 1);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+87:
					Enchant(player, item, EQUIPMENT_SLOT_MAINHAND, 3790, 1);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+88:
					Enchant(player, item, EQUIPMENT_SLOT_MAINHAND, 3834, 1);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+89:
					Enchant(player, item, EQUIPMENT_SLOT_MAINHAND, 3833, 1);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+90:
					Enchant(player, item, EQUIPMENT_SLOT_MAINHAND, 3239, 1);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+91:
					Enchant(player, item, EQUIPMENT_SLOT_MAINHAND, 3241, 1);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+92:
					Enchant(player, item, EQUIPMENT_SLOT_MAINHAND, 3870, 1);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+93:
					Enchant(player, item, EQUIPMENT_SLOT_MAINHAND, 3869, 1);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+94:
					Enchant(player, item, EQUIPMENT_SLOT_MAINHAND, 1103, 1);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+95:
					Enchant(player, item, EQUIPMENT_SLOT_MAINHAND, 3844, 1);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+96:
					Enchant(player, item, EQUIPMENT_SLOT_MAINHAND, 3225, 1);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+97:
					Enchant(player, item, EQUIPMENT_SLOT_MAINHAND, 2673, 1);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+98:
					Enchant(player, item, EQUIPMENT_SLOT_MAINHAND, 3827, 2);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+99:
					Enchant(player, item, EQUIPMENT_SLOT_MAINHAND, 3854, 3);
					OnGossipSelect(player, item, 1, 900);
					break;
				/*case 7000+199:
					Enchant(player, item, EQUIPMENT_SLOT_MAINHAND, 3731, 1);
					OnGossipSelect(player, item, 1, 900);
					break;*/
				case 7000+13:
				{
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "<- Volver al Men""\xC3\xBA"" principal", GOSSIP_SENDER_MAIN, 7000+1);

					Item* item_21 = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
					if (!item_21)
						return true;

					if (validItem(1, item_21))
					{
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Potencia Superior", GOSSIP_SENDER_MAIN, 7000+111);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Poder con Hechizos Poderoso", GOSSIP_SENDER_MAIN, 7000+110);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "+25 Indice de Golpe y Golpe Critico", GOSSIP_SENDER_MAIN, 7000+107);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Agilidad Excepcional", GOSSIP_SENDER_MAIN, 7000+116);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Espiritu Excepcional", GOSSIP_SENDER_MAIN, 7000+117);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Guardia Titan", GOSSIP_SENDER_MAIN, 7000+106);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Rabiar", GOSSIP_SENDER_MAIN, 7000+108);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Magia Negra", GOSSIP_SENDER_MAIN, 7000+109);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Verdugo", GOSSIP_SENDER_MAIN, 7000+118);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Mangosta", GOSSIP_SENDER_MAIN, 7000+119);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Rompehielo", GOSSIP_SENDER_MAIN, 7000+112);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Guardian de vida", GOSSIP_SENDER_MAIN, 7000+113);
						//player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Cadena de titanio", GOSSIP_SENDER_MAIN, 7000+198);

						if (player->HasSkill(SKILL_ENCHANTING) && player->GetSkillValue(SKILL_ENCHANTING) == 450)
						{
							player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Drenador de sangre", GOSSIP_SENDER_MAIN, 7000+92);
							player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Amparo de hojas", GOSSIP_SENDER_MAIN, 7000+93);
						}
					}
					if (validItem(4, item_21))
					{
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Encantar Escudo: +25 Intelecto", GOSSIP_SENDER_MAIN, 7000+101);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Encantar Escudo: +15 Bloqueo", GOSSIP_SENDER_MAIN, 7000+102);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Encantar Escudo: +36 Valor de Bloqueo", GOSSIP_SENDER_MAIN, 7000+105);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Encantar Escudo: +20 Defensa", GOSSIP_SENDER_MAIN, 7000+100);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Encantar Escudo: +18 Aguante", GOSSIP_SENDER_MAIN, 7000+104);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Encantar Escudo: +12 Temple", GOSSIP_SENDER_MAIN, 7000+103);
					}

					player->SEND_GOSSIP_MENU(68, item->GetGUID());
				}   break;
				case 7000+100:
					Enchant(player, item, EQUIPMENT_SLOT_OFFHAND, 1952, 4);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+101:
					Enchant(player, item, EQUIPMENT_SLOT_OFFHAND, 1128, 4);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+102:
					Enchant(player, item, EQUIPMENT_SLOT_OFFHAND, 2655, 4);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+103:
					Enchant(player, item, EQUIPMENT_SLOT_OFFHAND, 3229, 4);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+104:
					Enchant(player, item, EQUIPMENT_SLOT_OFFHAND, 1071, 4);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+105:
					Enchant(player, item, EQUIPMENT_SLOT_OFFHAND, 2653, 4);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+106:
					Enchant(player, item, EQUIPMENT_SLOT_OFFHAND, 3851, 1);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+107:
					Enchant(player, item, EQUIPMENT_SLOT_OFFHAND, 3788, 1);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+108:
					Enchant(player, item, EQUIPMENT_SLOT_OFFHAND, 3789, 1);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+109:
					Enchant(player, item, EQUIPMENT_SLOT_OFFHAND, 3790, 1);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+110:
					Enchant(player, item, EQUIPMENT_SLOT_OFFHAND, 3834, 1);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+111:
					Enchant(player, item, EQUIPMENT_SLOT_OFFHAND, 3833, 1);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+112:
					Enchant(player, item, EQUIPMENT_SLOT_OFFHAND, 3239, 1);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+113:
					Enchant(player, item, EQUIPMENT_SLOT_OFFHAND, 3241, 1);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+114:
					Enchant(player, item, EQUIPMENT_SLOT_OFFHAND, 3870, 1);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+115:
					Enchant(player, item, EQUIPMENT_SLOT_OFFHAND, 3869, 1);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+116:
					Enchant(player, item, EQUIPMENT_SLOT_OFFHAND, 1103, 1);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+117:
					Enchant(player, item, EQUIPMENT_SLOT_OFFHAND, 3844, 1);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+118:
					Enchant(player, item, EQUIPMENT_SLOT_OFFHAND, 3225, 1);
					OnGossipSelect(player, item, 1, 900);
					break;
				case 7000+119:
					Enchant(player, item, EQUIPMENT_SLOT_OFFHAND, 2673, 1);
					OnGossipSelect(player, item, 1, 900);
					break;
				/*case 7000+198:
					Enchant(player, item, EQUIPMENT_SLOT_OFFHAND, 3731, 1);
					OnGossipSelect(player, item, 1, 900);
					break;*/

				////////////////////////////////////////////   ENCHANTER - FIN        ////////////////////////////////////////////

				////////////////////////////////////////////   INTERCAMBIADOR PUNTOS - INICIO        /////////////////////////////

				case 901: //Intercambiador de Puntos - Submenu
				{
					player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\Achievement_arena_2v2_1:30:30:-15:0|t|r 7500 Honor -> 250 Arenas", GOSSIP_SENDER_MAIN, 5000);
					player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\Achievement_arena_2v2_2:30:30:-15:0|t|r 250 Arenas -> 5000 Honor", GOSSIP_SENDER_MAIN, 5001);
					player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\Achievement_BG_returnXflags_def_WSG:30:30:-15:0|t|rMenú principal del Libro", GOSSIP_SENDER_MAIN, 0);

					player->SEND_GOSSIP_MENU(1, item->GetGUID());
					return true;
				} break;

				case 5000:
				{
					if (player->GetArenaPoints() < sWorld->getIntConfig(CONFIG_MAX_ARENA_POINTS))
					{
						if (player->GetHonorPoints() >= 7500)
						{
							player->ModifyHonorPoints(-7500);
							player->ModifyArenaPoints(250);
							player->GetSession()->SendNotification("Intercambio realizado");
							ChatHandler(player->GetSession()).PSendSysMessage("Intercambio realizado.");
							OnGossipSelect(player, item, 1, 901);
						}
						else
						{
							ChatHandler(player->GetSession()).PSendSysMessage("No tienes los suficientes puntos de Honor.");
							OnGossipSelect(player, item, 1, 901);
						}
					}
					else
					{
						ChatHandler(player->GetSession()).PSendSysMessage("Ya has alcanzado el máximo número de puntos de Arenas.");
						OnGossipSelect(player, item, 1, 901);
					}
				} break;

				case 5001:
				{
					if (player->GetHonorPoints() < sWorld->getIntConfig(CONFIG_MAX_HONOR_POINTS))
					{
						if (player->GetArenaPoints() >= 250)
						{
							player->ModifyArenaPoints(-250);
							player->ModifyHonorPoints(5000);
							player->GetSession()->SendNotification("Intercambio realizado");
							ChatHandler(player->GetSession()).PSendSysMessage("Intercambio realizado.");
							OnGossipSelect(player, item, 1, 901);
						}
						else
						{
							ChatHandler(player->GetSession()).PSendSysMessage("No tienes los suficientes puntos de Arenas.");
							OnGossipSelect(player, item, 1, 901);
						}
					}
					else
					{
						ChatHandler(player->GetSession()).PSendSysMessage("Ya has alcanzado el máximo número de puntos de Honor.");
						OnGossipSelect(player, item, 1, 901);
					}
				} break;

				////////////////////////////////////////////   INTERCAMBIADOR PUNTOS - FIN        /////////////////////////////

				/*case 902: // Guildmaster - Accion
				{
					player->ADD_GOSSIP_ITEM(8, "Desaría crear un símbolo de hermandad.", GOSSIP_SENDER_MAIN, 904);
					player->ADD_GOSSIP_ITEM(7, "¿Cómo puedo crear una hermandad?", GOSSIP_SENDER_MAIN, 905);
					player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\Achievement_BG_returnXflags_def_WSG:30:30:-15:0|t|rMenú principal del Libro", GOSSIP_SENDER_MAIN, 0);

					player->PlayerTalkClass->SendGossipMenu(1, item->GetGUID());
					return true;
				} break;

				case 904: // Desaría crear un símbolo de hermandad. - Accion
					player->GetSession()->SendTabardVendorActivate(item->GetGUID());
					return true;
				break;

				case 905: // ¿Cómo puedo crear una hermandad? - Accion
					//player->PlayerTalkClass->SendCloseGossip();
					player->GetSession()->SendPetitionShowList(item->GetGUID());
					return true;
				break;*/

				////////////////////////////////////////////   TITULOS POR TOKENS - INICIO        /////////////////////////////

				case 903: //Títulos por Objetos de la Zona PvP - Submenu
				{
					player->ADD_GOSSIP_ITEM(4, "|cff00ff00|TInterface\\icons\\INV_Misc_QuestionMark.png:30:30:-15:0|t|rInformación", GOSSIP_SENDER_MAIN, 5100);
					if(player->GetTeam() == ALLIANCE)
						player->ADD_GOSSIP_ITEM(4, "|cff00ff00|TInterface\\icons\\achievement_featsofstrength_gladiator_08:30:30:-15:0|t|rTítulos", GOSSIP_SENDER_MAIN, 5101);
					else
						player->ADD_GOSSIP_ITEM(4, "|cff00ff00|TInterface\\icons\\achievement_featsofstrength_gladiator_08:30:30:-15:0|t|rTítulos", GOSSIP_SENDER_MAIN, 5102);
					player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\Achievement_BG_returnXflags_def_WSG:30:30:-15:0|t|rMenú principal del Libro", GOSSIP_SENDER_MAIN, 0);
					player->PlayerTalkClass->SendGossipMenu(1, item->GetGUID());
					return true;
				} break;

				case 5100: //Información - Acción
				{
					player->ADD_GOSSIP_ITEM(4, "|TInterface/PaperDollInfoFrame/UI-GearManager-Undo:30:30:-15:0|tVolver al Menú Principal", GOSSIP_SENDER_MAIN, 5103);
					player->ADD_GOSSIP_ITEM(4, "|TInterface/PaperDollInfoFrame/UI-GearManager-LeaveItem-Opaque:30:30:-15:0|tOlvídalo", GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
					player->PlayerTalkClass->SendGossipMenu(6, item->GetGUID());
					return true;
				} break;

				case 5103: // Volver al Menú Principal Titulos - Accion
					OnGossipSelect(player, item, 1, 903);
				break;

				// Menu Ali Titles
                case 5101: // Títulos 1 - Submenu
					player->ADD_GOSSIP_ITEM_EXTENDED(4, "|cff00ff00|TInterface\\icons\\achievement_pvp_a_01:30:30:-15:0|t|r|CFF0000FFSoldado|R", GOSSIP_SENDER_MAIN, 5104, "|CFFFFFF01""\xc2\xbf""Deseas comprar el título |CFF87CEFASoldado|CFFFFFF01?|R\n\n"+sT->GetItemIcon(32569, 28, 28, -3, -1)+sT->GetItemLink(32569, player->GetSession())+" x40", 0, false);
					player->ADD_GOSSIP_ITEM_EXTENDED(4, "|cff00ff00|TInterface\\icons\\achievement_pvp_a_02:30:30:-15:0|t|r|CFF0000FFCabo|R", GOSSIP_SENDER_MAIN, 5105, "|CFFFFFF01""\xc2\xbf""Deseas comprar el título |CFF87CEFACabo|CFFFFFF01?|R\n\n"+sT->GetItemIcon(32569, 28, 28, -3, -6)+sT->GetItemLink(32569, player->GetSession())+" x60\n\n+\n\n|cff00ff00|TInterface\\icons\\achievement_pvp_a_01:30:30:-5:-6|t|r|CFF0066CCSoldado|R", 0, false);
					player->ADD_GOSSIP_ITEM_EXTENDED(4, "|cff00ff00|TInterface\\icons\\achievement_pvp_a_03:30:30:-15:0|t|r|CFF0000FFSargento|R", GOSSIP_SENDER_MAIN, 5106, "|CFFFFFF01""\xc2\xbf""Deseas comprar el título |CFF87CEFASargento|CFFFFFF01?|R\n\n"+sT->GetItemIcon(32569, 28, 28, -3, -6)+sT->GetItemLink(32569, player->GetSession())+" x60\n\n+\n\n|cff00ff00|TInterface\\icons\\achievement_pvp_a_02:30:30:-5:-6|t|r|CFF0066CCCabo|R", 0, false);
					player->ADD_GOSSIP_ITEM_EXTENDED(4, "|cff00ff00|TInterface\\icons\\achievement_pvp_a_04:30:30:-15:0|t|r|CFF0000FFSargento primero|R", GOSSIP_SENDER_MAIN, 5107, "|CFFFFFF01""\xc2\xbf""Deseas comprar el título |CFF87CEFASargento primero|CFFFFFF01?|R\n\n"+sT->GetItemIcon(15704, 28, 28, -3, -6)+sT->GetItemLink(15704, player->GetSession())+" x3\n\n+\n\n|cff00ff00|TInterface\\icons\\achievement_pvp_a_03:30:30:-5:-6|t|r|CFF0066CCSargento|R", 0, false);
					player->ADD_GOSSIP_ITEM_EXTENDED(4, "|cff00ff00|TInterface\\icons\\achievement_pvp_a_10:30:30:-15:0|t|r|CFF0000FFTeniente coronel|R", GOSSIP_SENDER_MAIN, 5108, "|CFFFFFF01""\xc2\xbf""Deseas comprar el título |CFF87CEFATeniente coronel|CFFFFFF01?|R\n\n"+sT->GetItemIcon(32569, 28, 28, -3, -6)+sT->GetItemLink(32569, player->GetSession())+" x260\n\n+\n\n|cff00ff00|TInterface\\icons\\achievement_pvp_a_04:30:30:-5:-6|t|r|CFF0066CCSargento primero|R", 0, false);
					player->ADD_GOSSIP_ITEM_EXTENDED(4, "|cff00ff00|TInterface\\icons\\achievement_pvp_a_11:30:30:-15:0|t|r|CFF0000FFComandante|R", GOSSIP_SENDER_MAIN, 5109, "|CFFFFFF01""\xc2\xbf""Deseas comprar el título |CFF87CEFAComandante|CFFFFFF01?|R\n\n"+sT->GetItemIcon(32569, 28, 28, -3, -6)+sT->GetItemLink(32569, player->GetSession())+" x280\n\n+\n\n|cff00ff00|TInterface\\icons\\achievement_pvp_a_10:30:30:-5:-6|t|r|CFF0066CCTeniente coronel|R", 0, false);
					player->ADD_GOSSIP_ITEM_EXTENDED(4, "|cff00ff00|TInterface\\icons\\achievement_pvp_a_12:30:30:-15:0|t|r|CFF0000FFMariscal|R", GOSSIP_SENDER_MAIN, 5110, "|CFFFFFF01""\xc2\xbf""Deseas comprar el título |CFF87CEFAMariscal|CFFFFFF01?|R\n\n"+sT->GetItemIcon(32569, 28, 28, -3, -6)+sT->GetItemLink(32569, player->GetSession())+" x480\n\n+\n\n|cff00ff00|TInterface\\icons\\achievement_pvp_a_11:30:30:-5:-6|t|r|CFF0066CCComandante|R", 0, false);
					player->ADD_GOSSIP_ITEM_EXTENDED(4, "|cff00ff00|TInterface\\icons\\achievement_pvp_a_14:30:30:-15:0|t|r|CFF0000FFGran Mariscal|R", GOSSIP_SENDER_MAIN, 5111, "|CFFFFFF01""\xc2\xbf""Deseas comprar el título |CFF87CEFAGran mariscal|CFFFFFF01?|R\n\n"+sT->GetItemIcon(32569, 28, 28, -3, -13)+sT->GetItemLink(32569, player->GetSession())+" x720\n\n+\n\n"+sT->GetItemIcon(15704, 28, 28, -3, -13)+sT->GetItemLink(15704, player->GetSession())+" x25\n\n+\n\n"+"|cff00ff00|TInterface\\icons\\achievement_pvp_a_13:30:30:-5:-13|t|r|CFF0066CCMariscal|R", 0, false);
					player->ADD_GOSSIP_ITEM(4, "|TInterface/PaperDollInfoFrame/UI-GearManager-Undo:30:30:-15:0|tVolver al Menú principal", GOSSIP_SENDER_MAIN, 5103);
					player->ADD_GOSSIP_ITEM(4, "|TInterface/PaperDollInfoFrame/UI-GearManager-LeaveItem-Opaque:30:30:-15:0|tOlvídalo", GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
					player->PlayerTalkClass->SendGossipMenu(1, item->GetGUID());
					return true;
                break;

				// Menu Horda Titles
				case 5102:
					player->ADD_GOSSIP_ITEM_EXTENDED(4, "|cff00ff00|TInterface\\icons\\achievement_pvp_h_01:30:30:-15:0|t|r|CFF0000FFRastreador|R", GOSSIP_SENDER_MAIN, 5112, "|CFFFFFF01""\xc2\xbf""Deseas comprar el título |CFF87CEFARastreador|CFFFFFF01?|R\n\n"+sT->GetItemIcon(32569, 28, 28, -3, -1)+sT->GetItemLink(32569, player->GetSession())+" x40", 0, false);
					player->ADD_GOSSIP_ITEM_EXTENDED(4, "|cff00ff00|TInterface\\icons\\achievement_pvp_h_02:30:30:-15:0|t|r|CFF0000FFBruto|R", GOSSIP_SENDER_MAIN, 5113, "|CFFFFFF01""\xc2\xbf""Deseas comprar el título |CFF87CEFABruto|CFFFFFF01?|R\n\n"+sT->GetItemIcon(32569, 28, 28, -3, -6)+sT->GetItemLink(32569, player->GetSession())+" x60\n\n+\n\n|cff00ff00|TInterface\\icons\\achievement_pvp_h_01:30:30:-5:-6|t|r|CFF0066CCRastreador|R", 0, false);
					player->ADD_GOSSIP_ITEM_EXTENDED(4, "|cff00ff00|TInterface\\icons\\achievement_pvp_h_03:30:30:-15:0|t|r|CFF0000FFCapataz|R", GOSSIP_SENDER_MAIN, 5114, "|CFFFFFF01""\xc2\xbf""Deseas comprar el título |CFF87CEFACapataz|CFFFFFF01?|R\n\n"+sT->GetItemIcon(32569, 28, 28, -3, -6)+sT->GetItemLink(32569, player->GetSession())+" x60\n\n+\n\n|cff00ff00|TInterface\\icons\\achievement_pvp_h_02:30:30:-5:-6|t|r|CFF0066CCBruto|R", 0, false);
					player->ADD_GOSSIP_ITEM_EXTENDED(4, "|cff00ff00|TInterface\\icons\\achievement_pvp_h_04:30:30:-15:0|t|r|CFF0000FFCapataz primero|R", GOSSIP_SENDER_MAIN, 5115, "|CFFFFFF01""\xc2\xbf""Deseas comprar el título |CFF87CEFACapataz primero|CFFFFFF01?|R\n\n"+sT->GetItemIcon(15704, 28, 28, -3, -6)+sT->GetItemLink(15704, player->GetSession())+" x3\n\n+\n\n|cff00ff00|TInterface\\icons\\achievement_pvp_h_03:30:30:-5:-6|t|r|CFF0066CCCapataz|R", 0, false);
					player->ADD_GOSSIP_ITEM_EXTENDED(4, "|cff00ff00|TInterface\\icons\\achievement_pvp_h_10:30:30:-15:0|t|r|CFF0000FFCampeón|R", GOSSIP_SENDER_MAIN, 5116, "|CFFFFFF01""\xc2\xbf""Deseas comprar el título |CFF87CEFACampeón|CFFFFFF01?|R\n\n"+sT->GetItemIcon(32569, 28, 28, -3, -6)+sT->GetItemLink(32569, player->GetSession())+" x260\n\n+\n\n|cff00ff00|TInterface\\icons\\achievement_pvp_h_04:30:30:-5:-6|t|r|CFF0066CCCapataz primero|R", 0, false);
					player->ADD_GOSSIP_ITEM_EXTENDED(4, "|cff00ff00|TInterface\\icons\\achievement_pvp_h_11:30:30:-15:0|t|r|CFF0000FFGeneral|R", GOSSIP_SENDER_MAIN, 5117, "|CFFFFFF01""\xc2\xbf""Deseas comprar el título |CFF87CEFAGeneral|CFFFFFF01?|R\n\n"+sT->GetItemIcon(32569, 28, 28, -3, -6)+sT->GetItemLink(32569, player->GetSession())+" x280\n\n+\n\n|cff00ff00|TInterface\\icons\\achievement_pvp_h_10:30:30:-5:-6|t|r|CFF0066CCCampeón|R", 0, false);
					player->ADD_GOSSIP_ITEM_EXTENDED(4, "|cff00ff00|TInterface\\icons\\achievement_pvp_h_13:30:30:-15:0|t|r|CFF0000FFSe""\xC3\xB1""or de la Guerra|R", GOSSIP_SENDER_MAIN, 5118, "|CFFFFFF01""\xc2\xbf""Deseas comprar el título |CFF87CEFASe""\xC3\xB1""or de la Guerra|CFFFFFF01?|R\n\n"+sT->GetItemIcon(32569, 28, 28, -3, -6)+sT->GetItemLink(32569, player->GetSession())+" x480\n\n+\n\n|cff00ff00|TInterface\\icons\\achievement_pvp_h_11:30:30:-5:-6|t|r|CFF0066CCGeneral|R", 0, false);
					player->ADD_GOSSIP_ITEM_EXTENDED(4, "|cff00ff00|TInterface\\icons\\achievement_pvp_h_14:30:30:-15:0|t|r|CFF0000FFGran Se""\xC3\xB1""or de la Guerra|R", GOSSIP_SENDER_MAIN, 5119, "|CFFFFFF01""\xc2\xbf""Deseas comprar el título |CFF87CEFAGran Se""\xC3\xB1""or de la Guerra|CFFFFFF01?|R\n\n"+sT->GetItemIcon(32569, 28, 28, -3, -13)+sT->GetItemLink(32569, player->GetSession())+" x720\n\n+\n\n"+sT->GetItemIcon(15704, 28, 28, -3, -13)+sT->GetItemLink(15704, player->GetSession())+" x25\n\n+\n\n"+"|cff00ff00|TInterface\\icons\\achievement_pvp_h_13:30:30:-5:-13|t|r|CFF0066CCSe""\xC3\xB1""or de la Guerra|R", 0, false);
					player->ADD_GOSSIP_ITEM(4, "|TInterface/PaperDollInfoFrame/UI-GearManager-Undo:30:30:-15:0|tVolver al Menú principal", GOSSIP_SENDER_MAIN, 5103);
					player->ADD_GOSSIP_ITEM(4, "|TInterface/PaperDollInfoFrame/UI-GearManager-LeaveItem-Opaque:30:30:-15:0|tOlvídalo", GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
					player->PlayerTalkClass->SendGossipMenu(1, item->GetGUID());
					return true;
                break;

				case 5104: // Soldado Ali
				{
					CharTitlesEntry const* titleadditeado = sCharTitlesStore.LookupEntry(1);
					if (player->HasTitle(2) || player->HasTitle(3) || player->HasTitle(4) || player->HasTitle(10) || player->HasTitle(11) || player->HasTitle(12) || player->HasTitle(14))
					{
						ChatHandler(player).PSendSysMessage("|CFFFF0000Ya dispones de un título de mayor categoría.|R");
						OnGossipSelect(player, item, 0, 5101);
					}
					else
					{
						if (player->HasTitle(titleadditeado))
						{
							ChatHandler(player).PSendSysMessage("|CFFFF0000Ya dispones de ese título.|R");
							OnGossipSelect(player, item, 0, 5101);
						}
						else
						{
							if (player->HasItemCount(32569, 40))
							{
								player->DestroyItemCount(32569, 40, true, false);
								player->SetTitle(titleadditeado);
								ChatHandler(player).PSendSysMessage("|CFFFF0000Título añadido correctamente.|R");
								OnGossipSelect(player, item, 0, 5101);
							}
							else
							{
								ChatHandler(player).PSendSysMessage("|CFFFF0000No tienes suficentes |CFFff8000[Token Zona PvP]|CFFFF0000, necesitas al menos 40.|R");
								OnGossipSelect(player, item, 0, 5101);
							}
						}
					}
					return true;
				}
				break;

				case 5105: // Cabo Ali
				{
					CharTitlesEntry const* titleadditeado = sCharTitlesStore.LookupEntry(2);
					CharTitlesEntry const* titleborrado = sCharTitlesStore.LookupEntry(1);
					if (player->HasTitle(3) || player->HasTitle(4) || player->HasTitle(10) || player->HasTitle(11) || player->HasTitle(12) || player->HasTitle(14))
					{
						ChatHandler(player).PSendSysMessage("|CFFFF0000Ya dispones de un título de mayor categoría.|R");
						OnGossipSelect(player, item, 0, 5101);
					}
					else
					{
						if (player->HasTitle(titleadditeado))
						{
							ChatHandler(player).PSendSysMessage("|CFFFF0000Ya dispones de ese título.|R");
							OnGossipSelect(player, item, 0, 5101);
						}
						else
						{
							if (!player->HasItemCount(32569, 60))
							{
								ChatHandler(player).PSendSysMessage("|CFFFF0000No tienes suficentes |CFFff8000[Token Zona PvP]|CFFFF0000, necesitas al menos 60.|R");
								OnGossipSelect(player, item, 0, 5101);
							}

							if (!player->HasTitle(1))
							{
								ChatHandler(player).PSendSysMessage("|CFFFF0000No tienes el título de |CFF0066CCSoldado|R|CFFFF0000.|R");
								OnGossipSelect(player, item, 0, 5101);
							}
						}
						if (player->HasItemCount(32569, 60) && player->HasTitle(1))
						{
							player->DestroyItemCount(32569, 60, true, false);
							player->SetTitle(titleborrado, true);
							player->SetTitle(titleadditeado);
							ChatHandler(player).PSendSysMessage("|CFFFF0000Título añadido correctamente.|R");
							OnGossipSelect(player, item, 0, 5101);
						}
					}
					return true;
				} break;

				case 5106: // Sargento Ali
				{
					CharTitlesEntry const* titleadditeado = sCharTitlesStore.LookupEntry(3);
					CharTitlesEntry const* titleborrado = sCharTitlesStore.LookupEntry(2);
					if (player->HasTitle(4) || player->HasTitle(10) || player->HasTitle(11) || player->HasTitle(12) || player->HasTitle(14))
					{
						ChatHandler(player).PSendSysMessage("|CFFFF0000Ya dispones de un título de mayor categoría.|R");
						OnGossipSelect(player, item, 0, 5101);
					}
					else
					{
						if (player->HasTitle(titleadditeado))
						{
							ChatHandler(player).PSendSysMessage("|CFFFF0000Ya dispones de ese título.|R");
							OnGossipSelect(player, item, 0, 5101);
						}
						else
						{
							if (!player->HasItemCount(32569, 70))
							{
								ChatHandler(player).PSendSysMessage("|CFFFF0000No tienes suficentes |CFFff8000[Token Zona PvP]|CFFFF0000, necesitas al menos 70.|R");
								OnGossipSelect(player, item, 0, 5101);
							}

							if (!player->HasTitle(2))
							{
								ChatHandler(player).PSendSysMessage("|CFFFF0000No tienes el título de |CFF0066CCCabo|R|CFFFF0000.|R");
								OnGossipSelect(player, item, 0, 5101);
							}
						}

						if (player->HasItemCount(32569, 70) && player->HasTitle(2))
						{
							player->DestroyItemCount(32569, 70, true, false);
							player->SetTitle(titleborrado, true);
							player->SetTitle(titleadditeado);
							ChatHandler(player).PSendSysMessage("|CFFFF0000Título añadido correctamente.|R");
							OnGossipSelect(player, item, 0, 5101);
						}
					}
					return true;
				} break;

				case 5107: // Sargento primero Ali
				{
					CharTitlesEntry const* titleadditeado = sCharTitlesStore.LookupEntry(4);
					CharTitlesEntry const* titleborrado = sCharTitlesStore.LookupEntry(3);
					if (player->HasTitle(10) || player->HasTitle(11) || player->HasTitle(12) || player->HasTitle(14))
					{
						ChatHandler(player).PSendSysMessage("|CFFFF0000Ya dispones de un título de mayor categoría.|R");
						OnGossipSelect(player, item, 0, 5101);
					}
					else
					{
						if (player->HasTitle(titleadditeado))
						{
							ChatHandler(player).PSendSysMessage("|CFFFF0000Ya dispones de ese título.|R");
							OnGossipSelect(player, item, 0, 5101);
						}
						else
						{
							if (!player->HasItemCount(15704, 3))
							{
								ChatHandler(player).PSendSysMessage("|CFFFF0000No tienes suficentes |CFFff8000[Insignia del Valeroso]|CFFFF0000, necesitas al menos 3.|R");
								OnGossipSelect(player, item, 0, 5101);
							}

							if (!player->HasTitle(3))
							{
								ChatHandler(player).PSendSysMessage("|CFFFF0000No tienes el título de |CFF0066CCSargento|R|CFFFF0000.|R");
								OnGossipSelect(player, item, 0, 5101);
							}
						}

						if (player->HasItemCount(15704, 3) && player->HasTitle(3))
						{
							player->DestroyItemCount(15704, 3, true, false);
							player->SetTitle(titleborrado, true);
							player->SetTitle(titleadditeado);
							ChatHandler(player).PSendSysMessage("|CFFFF0000Título añadido correctamente.|R");
							OnGossipSelect(player, item, 0, 5101);
						}
					}
					return true;
				} break;

				case 5108: // Teniente Coronel Ali
				{
					CharTitlesEntry const* titleadditeado = sCharTitlesStore.LookupEntry(10);
					CharTitlesEntry const* titleborrado = sCharTitlesStore.LookupEntry(4);
					if (player->HasTitle(11) || player->HasTitle(12) || player->HasTitle(14))
					{
						ChatHandler(player).PSendSysMessage("|CFFFF0000Ya dispones de un título de mayor categoría.|R");
						OnGossipSelect(player, item, 0, 5101);
					}
					else
					{
						if (player->HasTitle(titleadditeado))
						{
							ChatHandler(player).PSendSysMessage("|CFFFF0000Ya dispones de ese título.|R");
							OnGossipSelect(player, item, 0, 5101);
						}
						else
						{
							if (!player->HasItemCount(32569, 260))
							{
								ChatHandler(player).PSendSysMessage("|CFFFF0000No tienes suficentes |CFFff8000[Token Zona PvP]|CFFFF0000, necesitas al menos 260.|R");
								OnGossipSelect(player, item, 0, 5101);
							}

							if (!player->HasTitle(4))
							{
								ChatHandler(player).PSendSysMessage("|CFFFF0000No tienes el título de |CFF0066CCSargento primero|R|CFFFF0000.|R");
								OnGossipSelect(player, item, 0, 5101);
							}
						}

						if (player->HasItemCount(32569, 260) && player->HasTitle(4))
						{
							player->DestroyItemCount(32569, 260, true, false);
							player->SetTitle(titleborrado, true);
							player->SetTitle(titleadditeado);
							ChatHandler(player).PSendSysMessage("|CFFFF0000Título añadido correctamente.|R");
							OnGossipSelect(player, item, 0, 5101);
						}
					}
					return true;
				} break;

				case 5109: // Comandante Ali
				{
					CharTitlesEntry const* titleadditeado = sCharTitlesStore.LookupEntry(11);
					CharTitlesEntry const* titleborrado = sCharTitlesStore.LookupEntry(10);
					if (player->HasTitle(12) || player->HasTitle(14))
					{
						ChatHandler(player).PSendSysMessage("|CFFFF0000Ya dispones de un título de mayor categoría.|R");
						OnGossipSelect(player, item, 0, 5101);
					}
					else
					{
						if (player->HasTitle(titleadditeado))
						{
							ChatHandler(player).PSendSysMessage("|CFFFF0000Ya dispones de ese título.|R");
							OnGossipSelect(player, item, 0, 5101);
						}
						else
						{
							if (!player->HasItemCount(32569, 280))
							{
								ChatHandler(player).PSendSysMessage("|CFFFF0000No tienes suficentes |CFFff8000[Token Zona PvP]|CFFFF0000, necesitas al menos 280.|R");
								OnGossipSelect(player, item, 0, 5101);
							}

							if (!player->HasTitle(10))
							{
								ChatHandler(player).PSendSysMessage("|CFFFF0000No tienes el título de |CFF0066CCTeniente coronel|R|CFFFF0000.|R");
								OnGossipSelect(player, item, 0, 5101);
							}
						}

						if (player->HasItemCount(32569, 280) && player->HasTitle(10))
						{
							player->DestroyItemCount(32569, 280, true, false);
							player->SetTitle(titleborrado, true);
							player->SetTitle(titleadditeado);
							ChatHandler(player).PSendSysMessage("|CFFFF0000Título añadido correctamente.|R");
							OnGossipSelect(player, item, 0, 5101);
						}
					}
					return true;
				} break;

				case 5110: // Mariscal Ali
				{
					CharTitlesEntry const* titleadditeado = sCharTitlesStore.LookupEntry(12);
					CharTitlesEntry const* titleborrado = sCharTitlesStore.LookupEntry(11);
					if (player->HasTitle(14))
					{
						ChatHandler(player).PSendSysMessage("|CFFFF0000Ya dispones de un título de mayor categoría.|R");
						OnGossipSelect(player, item, 0, 5101);
					}
					else
					{
						if (player->HasTitle(titleadditeado))
						{
							ChatHandler(player).PSendSysMessage("|CFFFF0000Ya dispones de ese título.|R");
							OnGossipSelect(player, item, 0, 5101);
						}
						else
						{
							if (!player->HasItemCount(32569, 480))
							{
								ChatHandler(player).PSendSysMessage("|CFFFF0000No tienes suficentes |CFFff8000[Token Zona PvP]|CFFFF0000, necesitas al menos 480.|R");
								OnGossipSelect(player, item, 0, 5101);
							}

							if (!player->HasTitle(11))
							{
								ChatHandler(player).PSendSysMessage("|CFFFF0000No tienes el título de |CFF0066CCComandante|R|CFFFF0000.|R");
								OnGossipSelect(player, item, 0, 5101);
							}
						}

						if (player->HasItemCount(32569, 480) && player->HasTitle(11))
						{
							player->DestroyItemCount(32569, 480, true, false);
							player->SetTitle(titleborrado, true);
							player->SetTitle(titleadditeado);
							ChatHandler(player).PSendSysMessage("|CFFFF0000Título añadido correctamente.|R");
							OnGossipSelect(player, item, 0, 5101);
						}
					}
					return true;
				} break;

				case 5111: // Gran Mariscal Ali
				{
					CharTitlesEntry const* titleadditeado = sCharTitlesStore.LookupEntry(13);
					CharTitlesEntry const* titleborrado = sCharTitlesStore.LookupEntry(11);
					if (player->HasTitle(titleadditeado))
					{
						ChatHandler(player).PSendSysMessage("|CFFFF0000Ya dispones de ese título.|R");
						OnGossipSelect(player, item, 0, 5101);
					}
					else
					{
						if (!player->HasItemCount(32569, 720))
						{
							ChatHandler(player).PSendSysMessage("|CFFFF0000No tienes suficentes |CFFff8000[Token Zona PvP]|CFFFF0000, necesitas al menos 720.|R");
							OnGossipSelect(player, item, 0, 5101);
						}

						if (!player->HasItemCount(15704, 25))
						{
							ChatHandler(player).PSendSysMessage("|CFFFF0000No tienes suficentes |CFFff8000[Insignia del Valeroso]|CFFFF0000, necesitas al menos 25.|R");
							OnGossipSelect(player, item, 0, 5101);
						}

						if (!player->HasTitle(11))
						{
							ChatHandler(player).PSendSysMessage("|CFFFF0000No tienes el título de |CFF0066CCMariscal|R|CFFFF0000.|R");
							OnGossipSelect(player, item, 0, 5101);
						}
					}

					if (player->HasItemCount(32569, 720) && player->HasItemCount(15704, 25) && player->HasTitle(11))
					{
						player->DestroyItemCount(32569, 720, true, false);
						player->DestroyItemCount(15704, 25, true, false);
						player->SetTitle(titleborrado, true);
						player->SetTitle(titleadditeado);
						ChatHandler(player).PSendSysMessage("|CFFFF0000Título añadido correctamente.|R");
						OnGossipSelect(player, item, 0, 5101);
					}
					return true;
				} break;

				case 5112: // Rastreador Horda
				{
					CharTitlesEntry const* titleadditeado = sCharTitlesStore.LookupEntry(15);
					if (player->HasTitle(16) || player->HasTitle(17) || player->HasTitle(18) || player->HasTitle(24) || player->HasTitle(25) || player->HasTitle(27) || player->HasTitle(28))
					{
						ChatHandler(player).PSendSysMessage("|CFFFF0000Ya dispones de un título de mayor categoría.|R");
						OnGossipSelect(player, item, 0, 5102);
					}
					else
					{
						if (player->HasTitle(titleadditeado))
						{
							ChatHandler(player).PSendSysMessage("|CFFFF0000Ya dispones de ese título.|R");
							OnGossipSelect(player, item, 0, 5102);
						}
						else
						{
							if (player->HasItemCount(32569, 40))
							{
								player->DestroyItemCount(32569, 40, true, false);
								player->SetTitle(titleadditeado);
								ChatHandler(player).PSendSysMessage("|CFFFF0000Título añadido correctamente.|R");
								OnGossipSelect(player, item, 0, 5102);
							}
							else
							{
								ChatHandler(player).PSendSysMessage("|CFFFF0000No tienes suficentes |CFFff8000[Token Zona PvP]|CFFFF0000, necesitas al menos 40.|R");
								OnGossipSelect(player, item, 0, 5102);
							}
						}
					}
					return true;
				} break;

				case 5113: // Bruto Horda
				{
					CharTitlesEntry const* titleadditeado = sCharTitlesStore.LookupEntry(16);
					CharTitlesEntry const* titleborrado = sCharTitlesStore.LookupEntry(15);
					if (player->HasTitle(17) || player->HasTitle(18) || player->HasTitle(24) || player->HasTitle(25) || player->HasTitle(27) || player->HasTitle(28))
					{
						ChatHandler(player).PSendSysMessage("|CFFFF0000Ya dispones de un título de mayor categoría.|R");
						OnGossipSelect(player, item, 0, 5102);
					}
					else
					{
						if (player->HasTitle(titleadditeado))
						{
							ChatHandler(player).PSendSysMessage("|CFFFF0000Ya dispones de ese título.|R");
							OnGossipSelect(player, item, 0, 5102);
						}
						else
						{
							if (!player->HasItemCount(32569, 60))
							{
								ChatHandler(player).PSendSysMessage("|CFFFF0000No tienes suficentes |CFFff8000[Token Zona PvP]|CFFFF0000, necesitas al menos 60.|R");
								OnGossipSelect(player, item, 0, 5102);
							}

							if (!player->HasTitle(15))
							{
								ChatHandler(player).PSendSysMessage("|CFFFF0000No tienes el título de |CFF0066CCRastreador|R|CFFFF0000.|R");
								OnGossipSelect(player, item, 0, 5102);
							}
						}
						if (player->HasItemCount(32569, 60) && player->HasTitle(15))
						{
							player->DestroyItemCount(32569, 60, true, false);
							player->SetTitle(titleborrado, true);
							player->SetTitle(titleadditeado);
							ChatHandler(player).PSendSysMessage("|CFFFF0000Título añadido correctamente.|R");
							OnGossipSelect(player, item, 0, 5102);
						}
					}
					return true;
				} break;

				case 5114: // Capataz Horda
				{
					CharTitlesEntry const* titleadditeado = sCharTitlesStore.LookupEntry(17);
					CharTitlesEntry const* titleborrado = sCharTitlesStore.LookupEntry(16);
					if (player->HasTitle(18) || player->HasTitle(24) || player->HasTitle(25) || player->HasTitle(27) || player->HasTitle(28))
					{
						ChatHandler(player).PSendSysMessage("|CFFFF0000Ya dispones de un título de mayor categoría.|R");
						OnGossipSelect(player, item, 0, 5102);
					}
					else
					{
						if (player->HasTitle(titleadditeado))
						{
							ChatHandler(player).PSendSysMessage("|CFFFF0000Ya dispones de ese título.|R");
							OnGossipSelect(player, item, 0, 5102);
						}
						else
						{
							if (!player->HasItemCount(32569, 70))
							{
								ChatHandler(player).PSendSysMessage("|CFFFF0000No tienes suficentes |CFFff8000[Token Zona PvP]|CFFFF0000, necesitas al menos 70.|R");
								OnGossipSelect(player, item, 0, 5102);
							}

							if (!player->HasTitle(16))
							{
								ChatHandler(player).PSendSysMessage("|CFFFF0000No tienes el título de |CFF0066CCBruto|R|CFFFF0000.|R");
								OnGossipSelect(player, item, 0, 5102);
							}
						}

						if (player->HasItemCount(32569, 70) && player->HasTitle(16))
						{
							player->DestroyItemCount(32569, 70, true, false);
							player->SetTitle(titleborrado, true);
							player->SetTitle(titleadditeado);
							ChatHandler(player).PSendSysMessage("|CFFFF0000Título añadido correctamente.|R");
							OnGossipSelect(player, item, 0, 5102);
						}
					}
					return true;
				} break;

				case 5115: // Capataz primero Horda
				{
					CharTitlesEntry const* titleadditeado = sCharTitlesStore.LookupEntry(18);
					CharTitlesEntry const* titleborrado = sCharTitlesStore.LookupEntry(17);
					if (player->HasTitle(24) || player->HasTitle(25) || player->HasTitle(27) || player->HasTitle(28))
					{
						ChatHandler(player).PSendSysMessage("|CFFFF0000Ya dispones de un título de mayor categoría.|R");
						OnGossipSelect(player, item, 0, 5102);
					}
					else
					{
						if (player->HasTitle(titleadditeado))
						{
							ChatHandler(player).PSendSysMessage("|CFFFF0000Ya dispones de ese título.|R");
							OnGossipSelect(player, item, 0, 5102);
						}
						else
						{
							if (!player->HasItemCount(15704, 3))
							{
								ChatHandler(player).PSendSysMessage("|CFFFF0000No tienes suficentes |CFFff8000[Insignia del Valeroso]|CFFFF0000, necesitas al menos 3.|R");
								OnGossipSelect(player, item, 0, 5102);
							}

							if (!player->HasTitle(17))
							{
								ChatHandler(player).PSendSysMessage("|CFFFF0000No tienes el título de |CFF0066CCCapataz|R|CFFFF0000.|R");
								OnGossipSelect(player, item, 0, 5102);
							}
						}

						if (player->HasItemCount(15704, 3) && player->HasTitle(17))
						{
							player->DestroyItemCount(15704, 3, true, false);
							player->SetTitle(titleborrado, true);
							player->SetTitle(titleadditeado);
							ChatHandler(player).PSendSysMessage("|CFFFF0000Título añadido correctamente.|R");
							OnGossipSelect(player, item, 0, 5102);
						}
					}
					return true;
				} break;

				case 5116: // Campeón Horda
				{
					CharTitlesEntry const* titleadditeado = sCharTitlesStore.LookupEntry(24);
					CharTitlesEntry const* titleborrado = sCharTitlesStore.LookupEntry(18);
					if (player->HasTitle(25) || player->HasTitle(27) || player->HasTitle(28))
					{
						ChatHandler(player).PSendSysMessage("|CFFFF0000Ya dispones de un título de mayor categoría.|R");
						OnGossipSelect(player, item, 0, 5102);
					}
					else
					{
						if (player->HasTitle(titleadditeado))
						{
							ChatHandler(player).PSendSysMessage("|CFFFF0000Ya dispones de ese título.|R");
							OnGossipSelect(player, item, 0, 5102);
						}
						else
						{
							if (!player->HasItemCount(32569, 260))
							{
								ChatHandler(player).PSendSysMessage("|CFFFF0000No tienes suficentes |CFFff8000[Token Zona PvP]|CFFFF0000, necesitas al menos 260.|R");
								OnGossipSelect(player, item, 0, 5102);
							}

							if (!player->HasTitle(18))
							{
								ChatHandler(player).PSendSysMessage("|CFFFF0000No tienes el título de |CFF0066CCCapataz primero|R|CFFFF0000.|R");
								OnGossipSelect(player, item, 0, 5102);
							}
						}

						if (player->HasItemCount(32569, 260) && player->HasTitle(18))
						{
							player->DestroyItemCount(32569, 260, true, false);
							player->SetTitle(titleborrado, true);
							player->SetTitle(titleadditeado);
							ChatHandler(player).PSendSysMessage("|CFFFF0000Título añadido correctamente.|R");
							OnGossipSelect(player, item, 0, 5102);
						}
					}
					return true;
				} break;

				case 5117: // General Horda
				{
					CharTitlesEntry const* titleadditeado = sCharTitlesStore.LookupEntry(25);
					CharTitlesEntry const* titleborrado = sCharTitlesStore.LookupEntry(24);
					if (player->HasTitle(27) || player->HasTitle(28))
					{
						ChatHandler(player).PSendSysMessage("|CFFFF0000Ya dispones de un título de mayor categoría.|R");
						OnGossipSelect(player, item, 0, 5102);
					}
					else
					{
						if (player->HasTitle(titleadditeado))
						{
							ChatHandler(player).PSendSysMessage("|CFFFF0000Ya dispones de ese título.|R");
							OnGossipSelect(player, item, 0, 5102);
						}
						else
						{
							if (!player->HasItemCount(32569, 280))
							{
								ChatHandler(player).PSendSysMessage("|CFFFF0000No tienes suficentes |CFFff8000[Token Zona PvP]|CFFFF0000, necesitas al menos 280.|R");
								OnGossipSelect(player, item, 0, 5102);
							}

							if (!player->HasTitle(24))
							{
								ChatHandler(player).PSendSysMessage("|CFFFF0000No tienes el título de |CFF0066CCCampeón|R|CFFFF0000.|R");
								OnGossipSelect(player, item, 0, 5102);
							}
						}

						if (player->HasItemCount(32569, 280) && player->HasTitle(24))
						{
							player->DestroyItemCount(32569, 280, true, false);
							player->SetTitle(titleborrado, true);
							player->SetTitle(titleadditeado);
							ChatHandler(player).PSendSysMessage("|CFFFF0000Título añadido correctamente.|R");
							OnGossipSelect(player, item, 0, 5102);
						}
					}
					return true;
				} break;

				case 5118: // Señor de la guerra
				{
					CharTitlesEntry const* titleadditeado = sCharTitlesStore.LookupEntry(27);
					CharTitlesEntry const* titleborrado = sCharTitlesStore.LookupEntry(25);
					if (player->HasTitle(28))
					{
						ChatHandler(player).PSendSysMessage("|CFFFF0000Ya dispones de un título de mayor categoría.|R");
						OnGossipSelect(player, item, 0, 5102);
					}
					else
					{
						if (player->HasTitle(titleadditeado))
						{
							ChatHandler(player).PSendSysMessage("|CFFFF0000Ya dispones de ese título.|R");
							OnGossipSelect(player, item, 0, 5102);
						}
						else
						{
							if (!player->HasItemCount(32569, 480))
							{
								ChatHandler(player).PSendSysMessage("|CFFFF0000No tienes suficentes |CFFff8000[Token Zona PvP]|CFFFF0000, necesitas al menos 480.|R");
								OnGossipSelect(player, item, 0, 5102);
							}

							if (!player->HasTitle(25))
							{
								ChatHandler(player).PSendSysMessage("|CFFFF0000No tienes el título de |CFF0066CCGeneral|R|CFFFF0000.|R");
								OnGossipSelect(player, item, 0, 5102);
							}
						}

						if (player->HasItemCount(32569, 480) && player->HasTitle(25))
						{
							player->DestroyItemCount(32569, 480, true, false);
							player->SetTitle(titleborrado, true);
							player->SetTitle(titleadditeado);
							ChatHandler(player).PSendSysMessage("|CFFFF0000Título añadido correctamente.|R");
							OnGossipSelect(player, item, 0, 5102);
						}
					}
					return true;
				} break;

				case 5119: // Gran Señor de la Guerra Horda
				{
					CharTitlesEntry const* titleadditeado = sCharTitlesStore.LookupEntry(28);
					CharTitlesEntry const* titleborrado = sCharTitlesStore.LookupEntry(27);
					if (player->HasTitle(titleadditeado))
					{
						ChatHandler(player).PSendSysMessage("|CFFFF0000Ya dispones de ese título.|R");
						OnGossipSelect(player, item, 0, 5102);
					}
					else
					{
						if (!player->HasItemCount(32569, 720))
						{
							ChatHandler(player).PSendSysMessage("|CFFFF0000No tienes suficentes |CFFff8000[Token Zona PvP]|CFFFF0000, necesitas al menos 720.|R");
							OnGossipSelect(player, item, 0, 5102);
						}

						if (!player->HasItemCount(15704, 25))
						{
							ChatHandler(player).PSendSysMessage("|CFFFF0000No tienes suficentes |CFFff8000[Insignia del Valeroso]|CFFFF0000, necesitas al menos 25.|R");
							OnGossipSelect(player, item, 0, 5102);
						}

						if (!player->HasTitle(27))
						{
							ChatHandler(player).PSendSysMessage("|CFFFF0000No tienes el título de |CFF0066CCSe""\xC3\xB1""or de la Guerra|R|CFFFF0000.|R");
							OnGossipSelect(player, item, 0, 5102);
						}
					}

					if (player->HasItemCount(32569, 720) && player->HasItemCount(15704, 25) && player->HasTitle(27))
					{
						player->DestroyItemCount(32569, 720, true, false);
						player->DestroyItemCount(15704, 25, true, false);
						player->SetTitle(titleborrado, true);
						player->SetTitle(titleadditeado);
						ChatHandler(player).PSendSysMessage("|CFFFF0000Título añadido correctamente.|R");
						OnGossipSelect(player, item, 0, 5102);
					}
					return true;
				} break;

				////////////////////////////////////////////   TITULOS POR TOKENS - FIN          /////////////////////////////

				////////////////////////////////////////////   PROFESIONES - INICIO	             /////////////////////////////

				case 805: // Profesiones - Submenu
				{
					player->ADD_GOSSIP_ITEM(7, "|cff00ff00|TInterface\\icons\\Achievement_profession_fishing_oldmanbarlowned:30:30:-15:0|t|rMaestro de Profesiones", GOSSIP_SENDER_MAIN, 5137);
					player->ADD_GOSSIP_ITEM(5, "|cff00ff00|TInterface\\icons\\inv_elemental_primal_water:30:30:-15:0|t|rObjetos de Profesiones", GOSSIP_SENDER_MAIN, 5138);
					player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\Achievement_BG_returnXflags_def_WSG:30:30:-15:0|t|rMenú principal del Libro", GOSSIP_SENDER_MAIN, 0);
                    player->PlayerTalkClass->SendGossipMenu(1, item->GetGUID());
					return true;
				} break;

				case 5137: // Maestro de Profesiones - Submenu
				{
					player->ADD_GOSSIP_ITEM(5, "[Profesiones Primarias] ->", GOSSIP_SENDER_MAIN, 5120);
					player->ADD_GOSSIP_ITEM(5, "[Profesiones Secundarias] ->", GOSSIP_SENDER_MAIN, 5121);
					player->ADD_GOSSIP_ITEM(2, "<- Volver", GOSSIP_SENDER_MAIN, 805);
					player->ADD_GOSSIP_ITEM(2, "<-- Menú principal del Libro", GOSSIP_SENDER_MAIN, 0);
                    player->PlayerTalkClass->SendGossipMenu(1, item->GetGUID());
					return true;
				} break;

				case 5120: // [Profesiones Primarias] -> - Submenu
				{
					player->ADD_GOSSIP_ITEM(3, "Alquimia", GOSSIP_SENDER_MAIN, 5123);
                    player->ADD_GOSSIP_ITEM(3, "Herreria", GOSSIP_SENDER_MAIN, 5124);
					player->ADD_GOSSIP_ITEM(3, "Peleteria", GOSSIP_SENDER_MAIN, 5125);
                    player->ADD_GOSSIP_ITEM(3, "Sastreria", GOSSIP_SENDER_MAIN, 5126);
                    player->ADD_GOSSIP_ITEM(3, "Ingenieria", GOSSIP_SENDER_MAIN, 5127);
                    player->ADD_GOSSIP_ITEM(3, "Encantador", GOSSIP_SENDER_MAIN, 5128);
                    player->ADD_GOSSIP_ITEM(3, "Joyeria", GOSSIP_SENDER_MAIN, 5129);
                    player->ADD_GOSSIP_ITEM(3, "Inscripcion", GOSSIP_SENDER_MAIN, 5130);
                    player->ADD_GOSSIP_ITEM(3, "Mineria", GOSSIP_SENDER_MAIN, 5131);
                    player->ADD_GOSSIP_ITEM(3, "Herboristeria", GOSSIP_SENDER_MAIN, 5132);
                    player->ADD_GOSSIP_ITEM(3, "Desuello", GOSSIP_SENDER_MAIN, 5133);
					player->ADD_GOSSIP_ITEM(2, "<- Volver al Menu Principal.", GOSSIP_SENDER_MAIN, 5122);
					player->ADD_GOSSIP_ITEM(2, "Olvidalo", GOSSIP_SENDER_MAIN, ARENA_GOODBYE);

                    player->PlayerTalkClass->SendGossipMenu(1, item->GetGUID());
					return true;
				} break;

				case 5122: //<- Volver al Menu Principal. Profesiones - Accion
					OnGossipSelect(player, item, 1, 5137);
					return true;
				break;

				case 5121: // [Profesiones Primarias] -> - Submenu
				{
					player->ADD_GOSSIP_ITEM(3, "Cocinero", GOSSIP_SENDER_MAIN, 5134);
                    player->ADD_GOSSIP_ITEM(3, "Primeros Auxilios", GOSSIP_SENDER_MAIN, 5135);
					player->ADD_GOSSIP_ITEM(3, "Pesca", GOSSIP_SENDER_MAIN, 5136);
					player->ADD_GOSSIP_ITEM(2, "<- Volver al Menu Principal.", GOSSIP_SENDER_MAIN, 5122);
					player->ADD_GOSSIP_ITEM(2, "Olvidalo", GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
                    player->PlayerTalkClass->SendGossipMenu(1, item->GetGUID());
					return true;
				} break;

				case 5123:
                    CompleteLearnProfession(player, item, SKILL_ALCHEMY);
					player->CLOSE_GOSSIP_MENU();
					return true;
                break;

                case 5124:
                    CompleteLearnProfession(player, item, SKILL_BLACKSMITHING);
					player->CLOSE_GOSSIP_MENU();
					return true;
                break;

                case 5125:
                    CompleteLearnProfession(player, item, SKILL_LEATHERWORKING);
					player->CLOSE_GOSSIP_MENU();
					return true;
                break;

                case 5126:
                    CompleteLearnProfession(player, item, SKILL_TAILORING);
					player->CLOSE_GOSSIP_MENU();
					return true;
                break;

                case 5127:
					CompleteLearnProfession(player, item, SKILL_ENGINERING);
					player->CLOSE_GOSSIP_MENU();
					return true;
                break;

                case 5128:
                    CompleteLearnProfession(player, item, SKILL_ENCHANTING);
					player->CLOSE_GOSSIP_MENU();
					return true;
                break;

                case 5129:
                    CompleteLearnProfession(player, item, SKILL_JEWELCRAFTING);
					player->CLOSE_GOSSIP_MENU();
					return true;
                break;

                case 5130:
                    CompleteLearnProfession(player, item, SKILL_INSCRIPTION);
					player->CLOSE_GOSSIP_MENU();
					return true;
                break;

				case 5134:
                    CompleteLearnProfession(player, item, SKILL_COOKING);
					player->CLOSE_GOSSIP_MENU();
					return true;
                break;

                case 5135:
                    CompleteLearnProfession(player, item, SKILL_FIRST_AID);
					player->CLOSE_GOSSIP_MENU();
					return true;
                break;

                case 5131:
                    CompleteLearnProfession(player, item, SKILL_MINING);
					player->CLOSE_GOSSIP_MENU();
					return true;
                break;

                case 5132:
                    CompleteLearnProfession(player, item, SKILL_HERBALISM);
					player->CLOSE_GOSSIP_MENU();
					return true;
                break;

                case 5133:
                    CompleteLearnProfession(player, item, SKILL_SKINNING);
					player->CLOSE_GOSSIP_MENU();
					return true;
                break;

				case 5136:
					CompleteLearnProfession(player, item, SKILL_FISHING);
					player->CLOSE_GOSSIP_MENU();
					return true;
                break;

				case 5138: // Objetos de Profesiones - Submenu
					player->ADD_GOSSIP_ITEM(6, "Minería y Herrería", GOSSIP_SENDER_MAIN, 500060);
                    player->ADD_GOSSIP_ITEM(6, "Peletería", GOSSIP_SENDER_MAIN, 500022);
					player->ADD_GOSSIP_ITEM(6, "Cuero", GOSSIP_SENDER_MAIN, 500054);
					player->ADD_GOSSIP_ITEM(6, "Encantamiento", GOSSIP_SENDER_MAIN, 500031);
					player->ADD_GOSSIP_ITEM(6, "Joyería", GOSSIP_SENDER_MAIN, 500052);
					player->ADD_GOSSIP_ITEM(6, "Sastrería", GOSSIP_SENDER_MAIN, 500018);
					player->ADD_GOSSIP_ITEM(6, "Inscripción", GOSSIP_SENDER_MAIN, 500048);
					player->ADD_GOSSIP_ITEM(6, "Alquimia", GOSSIP_SENDER_MAIN, 500028);
					player->ADD_GOSSIP_ITEM(6, "Piedras Preciosas", GOSSIP_SENDER_MAIN, 500040);
					player->ADD_GOSSIP_ITEM(6, "Herboristería", GOSSIP_SENDER_MAIN, 500043);
					player->ADD_GOSSIP_ITEM(6, "Cocina", GOSSIP_SENDER_MAIN, 500058);
					player->ADD_GOSSIP_ITEM(2, "<- Volver", GOSSIP_SENDER_MAIN, 805);
					player->ADD_GOSSIP_ITEM(2, "<-- Menú principal del Libro", GOSSIP_SENDER_MAIN, 0);
                    player->PlayerTalkClass->SendGossipMenu(1, item->GetGUID());
					return true;
                break;

				case 500060: // Minería y Herrería - Accion
				case 500022: // Peletería - Accion
				case 500054: // Cuero - Accion
				case 500031: // Encantamiento - Accion
				case 500052: // Joyería - Accion
				case 500018: // Sastrería - Accion
				case 500048: // Inscripción - Accion
				case 500028: // Alquimia - Accion
				case 500040: // Piedras Preciosas - Accion
				case 500043: // Herboristería - Accion
				case 500058: // Cocina - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), action);
					return true;
				} break;

				////////////////////////////////////////////   PROFESIONES - FIN	             /////////////////////////////

				////////////////////////////////////////////   TRANSFIGURACION - INICIO          /////////////////////////////

				case 806: // Transfiguración
				{
					player->ADD_GOSSIP_ITEM(7, "|cff00ff00|TInterface\\icons\\Spell_shaman_spectraltransformation:30:30:-15:0|t|rTransfigurador", GOSSIP_SENDER_MAIN, 5150);
					player->ADD_GOSSIP_ITEM(5, "|cff00ff00|TInterface\\icons\\inv_helmet_157:30:30:-15:0|t|rObjetos de Transfiguración", GOSSIP_SENDER_MAIN, 5151);
					player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\Achievement_BG_returnXflags_def_WSG:30:30:-15:0|t|rMenú principal del Libro", GOSSIP_SENDER_MAIN, 0);
                    player->PlayerTalkClass->SendGossipMenu(1, item->GetGUID());
					return true;
				} break;

				case 5151: // Objetos de Transfiguración - Submenu
					player->ADD_GOSSIP_ITEM(6, "|CFFFF0000Armas Especiales|r", GOSSIP_SENDER_MAIN, 5152);
                    player->ADD_GOSSIP_ITEM(6, "Armas Legendarias", GOSSIP_SENDER_MAIN, 5153);
					player->ADD_GOSSIP_ITEM(5, "Seasons PvP", GOSSIP_SENDER_MAIN, 5154);
					player->ADD_GOSSIP_ITEM(5, "Tiers PvE", GOSSIP_SENDER_MAIN, 5155);
					player->ADD_GOSSIP_ITEM(6, "Armas de la TBC", GOSSIP_SENDER_MAIN, 5156);
					player->ADD_GOSSIP_ITEM(6, "Armaduras de Anh'Quiraj", GOSSIP_SENDER_MAIN, 5157);
					player->ADD_GOSSIP_ITEM(6, "Armas Variadas", GOSSIP_SENDER_MAIN, 5158);
					player->ADD_GOSSIP_ITEM(6, "Objetos Variados", GOSSIP_SENDER_MAIN, 5159);
					player->ADD_GOSSIP_ITEM(6, "Armas Extra", GOSSIP_SENDER_MAIN, 5198);
					player->ADD_GOSSIP_ITEM(6, "Armaduras Extra", GOSSIP_SENDER_MAIN, 5199);
					if(player->GetTeam() == HORDE)
					{
						player->ADD_GOSSIP_ITEM(6, "Armas de Legado", GOSSIP_SENDER_MAIN, 5197);
						player->ADD_GOSSIP_ITEM(6, "Armaduras de Legado", GOSSIP_SENDER_MAIN, 5196);
					}
					else
					{
						player->ADD_GOSSIP_ITEM(6, "Armas de Legado", GOSSIP_SENDER_MAIN, 5195);
						player->ADD_GOSSIP_ITEM(6, "Armaduras de Legado", GOSSIP_SENDER_MAIN, 5194);
					}
					player->ADD_GOSSIP_ITEM(6, "Conjuntos recolor WOTLK", GOSSIP_SENDER_MAIN, 5193);
					player->ADD_GOSSIP_ITEM(6, "Conjuntos recolor TBC", GOSSIP_SENDER_MAIN, 5192);
					player->ADD_GOSSIP_ITEM(2, "<- Volver", GOSSIP_SENDER_MAIN, 806);
					player->ADD_GOSSIP_ITEM(2, "<-- Menú principal del Libro", GOSSIP_SENDER_MAIN, 0);
                    player->PlayerTalkClass->SendGossipMenu(1, item->GetGUID());
					return true;
                break;
				
				case 5193: // Conjuntos recolor WOTLK - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500204);
					return true;
				} break;
				
				case 5192: // Conjuntos recolor TBC - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500205);
					return true;
				} break;
				
				case 5197: // Armas de legado - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 12794);
					return true;
				} break;
				
				case 5196: // Armaduras de legado - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 12795);
					return true;
				} break;
				
				case 5195: // Armas de legado - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 12784);
					return true;
				} break;
				
				case 5194: // Armaduras de legado - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 12785);
					return true;
				} break;
				
				case 5198: // Armas Especiales - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500207);
					return true;
				} break;
				
				case 5199: // Armaduras Extra - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500206);
					return true;
				} break;

				case 5152: // Armas Especiales - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 26309);
					return true;
				} break;

				case 5153: // Armas Legendarias - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500056);
					return true;
				} break;

				case 5154: // Seasons PvP - Submenu
					player->ADD_GOSSIP_ITEM(5, "Season 6", GOSSIP_SENDER_MAIN, 5160);
                    player->ADD_GOSSIP_ITEM(5, "Season 5", GOSSIP_SENDER_MAIN, 5161);
					player->ADD_GOSSIP_ITEM(5, "Season 4", GOSSIP_SENDER_MAIN, 5162);
					player->ADD_GOSSIP_ITEM(5, "Season 3", GOSSIP_SENDER_MAIN, 5163);
					player->ADD_GOSSIP_ITEM(5, "Season 2", GOSSIP_SENDER_MAIN, 5164);
					player->ADD_GOSSIP_ITEM(5, "Season 1", GOSSIP_SENDER_MAIN, 5165);
					player->ADD_GOSSIP_ITEM(2, "<- Volver", GOSSIP_SENDER_MAIN, 806);
					player->ADD_GOSSIP_ITEM(2, "<-- Menú principal del Libro", GOSSIP_SENDER_MAIN, 0);
                    player->PlayerTalkClass->SendGossipMenu(1, item->GetGUID());
					return true;
                break;

				case 5156: // Armas de la TBC - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 12246);
					return true;
				} break;

				case 5157: // Armaduras de Anh'Quiraj - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 30098);
					return true;
				} break;

				case 5158: // Armas Variadas - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500098);
					return true;
				} break;

				case 5159: // Objetos Variados - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500065);
					return true;
				} break;

				case 5160: // Seasons PvP - Season 6 - Submenu
					player->ADD_GOSSIP_ITEM(6, "Armas", GOSSIP_SENDER_MAIN, 5166);
                    player->ADD_GOSSIP_ITEM(6, "Armaduras", GOSSIP_SENDER_MAIN, 5167);
					player->ADD_GOSSIP_ITEM(2, "<- Volver.", GOSSIP_SENDER_MAIN, 5154);
					player->ADD_GOSSIP_ITEM(2, "<-- Menú principal del Libro", GOSSIP_SENDER_MAIN, 0);
                    player->PlayerTalkClass->SendGossipMenu(1, item->GetGUID());
					return true;
                break;

				case 5166: // Season 6 - Armas - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500121);
					return true;
				} break;

				case 5167: // Season 6 - Armaduras - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500009);
					return true;
				} break;

				case 5161: // Seasons PvP - Season 5 - Submenu
					player->ADD_GOSSIP_ITEM(6, "Armas", GOSSIP_SENDER_MAIN, 5168);
                    player->ADD_GOSSIP_ITEM(6, "Armaduras", GOSSIP_SENDER_MAIN, 5169);
					player->ADD_GOSSIP_ITEM(2, "<- Volver.", GOSSIP_SENDER_MAIN, 5154);
					player->ADD_GOSSIP_ITEM(2, "<-- Menú principal del Libro", GOSSIP_SENDER_MAIN, 0);
                    player->PlayerTalkClass->SendGossipMenu(1, item->GetGUID());
					return true;
                break;

				case 5168: // Season 5 - Armas - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500120);
					return true;
				} break;

				case 5169: // Season 5 - Armaduras - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500006);
					return true;
				} break;

				case 5162: // Seasons PvP - Season 4 - Submenu
					player->ADD_GOSSIP_ITEM(6, "Armas", GOSSIP_SENDER_MAIN, 5170);
                    player->ADD_GOSSIP_ITEM(6, "Armaduras", GOSSIP_SENDER_MAIN, 5171);
					player->ADD_GOSSIP_ITEM(2, "<- Volver.", GOSSIP_SENDER_MAIN, 5154);
					player->ADD_GOSSIP_ITEM(2, "<-- Menú principal del Libro", GOSSIP_SENDER_MAIN, 0);
                    player->PlayerTalkClass->SendGossipMenu(1, item->GetGUID());
					return true;
                break;

				case 5170: // Season 4 - Armas - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500119);
					return true;
				} break;

				case 5171: // Season 4 - Armaduras - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500005);
					return true;
				} break;

				case 5163: // Seasons PvP - Season 3 - Submenu
					player->ADD_GOSSIP_ITEM(6, "Armas", GOSSIP_SENDER_MAIN, 5172);
                    player->ADD_GOSSIP_ITEM(6, "Armaduras", GOSSIP_SENDER_MAIN, 5173);
					player->ADD_GOSSIP_ITEM(2, "<- Volver.", GOSSIP_SENDER_MAIN, 5154);
					player->ADD_GOSSIP_ITEM(2, "<-- Menú principal del Libro", GOSSIP_SENDER_MAIN, 0);
                    player->PlayerTalkClass->SendGossipMenu(1, item->GetGUID());
					return true;
                break;

				case 5172: // Season 3 - Armas - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500118);
					return true;
				} break;

				case 5173: // Season 3 - Armaduras - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500004);
					return true;
				} break;

				case 5164: // Seasons PvP - Season 2 - Submenu
					player->ADD_GOSSIP_ITEM(6, "Armas", GOSSIP_SENDER_MAIN, 5174);
                    player->ADD_GOSSIP_ITEM(6, "Armaduras", GOSSIP_SENDER_MAIN, 5175);
					player->ADD_GOSSIP_ITEM(2, "<- Volver.", GOSSIP_SENDER_MAIN, 5154);
					player->ADD_GOSSIP_ITEM(2, "<-- Menú principal del Libro", GOSSIP_SENDER_MAIN, 0);
                    player->PlayerTalkClass->SendGossipMenu(1, item->GetGUID());
					return true;
                break;

				case 5174: // Season 2 - Armas - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500117);
					return true;
				} break;

				case 5175: // Season 2 - Armaduras - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500003);
					return true;
				} break;

				case 5165: // Seasons PvP - Season 1 - Submenu
					player->ADD_GOSSIP_ITEM(6, "Armas", GOSSIP_SENDER_MAIN, 5176);
                    player->ADD_GOSSIP_ITEM(6, "Armaduras", GOSSIP_SENDER_MAIN, 5177);
					player->ADD_GOSSIP_ITEM(2, "<- Volver.", GOSSIP_SENDER_MAIN, 5154);
					player->ADD_GOSSIP_ITEM(2, "<-- Menú principal del Libro", GOSSIP_SENDER_MAIN, 0);
                    player->PlayerTalkClass->SendGossipMenu(1, item->GetGUID());
					return true;
                break;

				case 5176: // Season 1 - Armas - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500116);
					return true;
				} break;

				case 5177: // Season 1 - Armaduras - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500002);
					return true;
				} break;

				case 5155: // Tiers PvE - Submenu
					player->ADD_GOSSIP_ITEM(6, "Tier 8.5", GOSSIP_SENDER_MAIN, 5178);
                    player->ADD_GOSSIP_ITEM(6, "Tier 8", GOSSIP_SENDER_MAIN, 5179);
					player->ADD_GOSSIP_ITEM(6, "Tier 7.5", GOSSIP_SENDER_MAIN, 5180);
					player->ADD_GOSSIP_ITEM(6, "Tier 7", GOSSIP_SENDER_MAIN, 5181);
					player->ADD_GOSSIP_ITEM(6, "Tier 6", GOSSIP_SENDER_MAIN, 5182);
					player->ADD_GOSSIP_ITEM(6, "Tier 5", GOSSIP_SENDER_MAIN, 5183);
					player->ADD_GOSSIP_ITEM(6, "Tier 4", GOSSIP_SENDER_MAIN, 5184);
					player->ADD_GOSSIP_ITEM(6, "Tier 3", GOSSIP_SENDER_MAIN, 5185);
					player->ADD_GOSSIP_ITEM(6, "Tier 2", GOSSIP_SENDER_MAIN, 5186);
					player->ADD_GOSSIP_ITEM(6, "Tier 1", GOSSIP_SENDER_MAIN, 5187);
					player->ADD_GOSSIP_ITEM(2, "<- Volver.", GOSSIP_SENDER_MAIN, 5151);
					player->ADD_GOSSIP_ITEM(2, "<-- Menú principal del Libro", GOSSIP_SENDER_MAIN, 0);
                    player->PlayerTalkClass->SendGossipMenu(1, item->GetGUID());
					return true;
                break;

				case 5178: // Tier 8.5 - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500084);
					return true;
				} break;

				case 5179: // Tier 8 - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500083);
					return true;
				} break;

				case 5180: // Tier 7.5 - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500082);
					return true;
				} break;

				case 5181: // Tier 7 - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500081);
					return true;
				} break;

				case 5182: // Tier 6 - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500080);
					return true;
				} break;

				case 5183: // Tier 5 - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500079);
					return true;
				} break;

				case 5184: // Tier 4 - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500078);
					return true;
				} break;

				case 5185: // Tier 3 - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500077);
					return true;
				} break;

				case 5186: // Tier 2 - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500076);
					return true;
				} break;

				case 5187: // Tier 1 - Accion
				{
					player->GetSession()->SendListInventory(item->GetGUID(), 500075);
					return true;
				} break;
			} ///* Final Switch Action *///

			// Arena Spectator - Accion
			if (action >= 1000 && action < 2000)
			{
				ShowPage(player, action - 1000, false);
				player->SEND_GOSSIP_MENU(1, item->GetGUID());
			}
			else if (action >= 2000 && action < 3000)
			{
				ShowPage(player, action - 2000, true);
				player->SEND_GOSSIP_MENU(1, item->GetGUID());
			}
			else
			{
				uint64 guid = action - 3000;
				if (Player* target = ObjectAccessor::FindPlayer(guid))
				{
					ChatHandler handler(player);
					char const* pTarget = target->GetName();
					itemgossip::HandleSpectateCommand(&handler, pTarget);
				}
			}
			return true;
		}

		bool OnGossipSelectCode(Player* player, Item* item, uint32 sender, uint32 action, const char* code)
		{
			player->PlayerTalkClass->ClearMenus();

			if (player->isInCombat())
			{
				ChatHandler(player).PSendSysMessage(LANG_YOU_IN_COMBAT);
				player->CLOSE_GOSSIP_MENU();
				return true;
			}

			if(player->GetMap()->IsBattlegroundOrArena() || player->HasStealthAura() || player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || player->isDead())
			{
				ChatHandler(player).PSendSysMessage("No puedes hacer eso estando en BG, Arena o estando en sigilo.");
				player->CLOSE_GOSSIP_MENU();
				return true;
			}

			if (player->GetZoneId() == 14)
			{
				ChatHandler(player).PSendSysMessage("No puedes hacer eso estando en la Zona PvP.");
				player->CLOSE_GOSSIP_MENU();
				return true;
			}

			switch(action)
			{
				case 700:
				{
					SendOptions(player, item, code);
					return true;
				} break;

				case 701:
				{
					SendTeamInfo(player, item, code);
					return true;
				} break;
			}

			/*if (sender || action)
				return true; // should never happen*/

			if (!sT->GetEnableSets())
			{
				OnGossipSelect(player, item, GOSSIP_SENDER_MAIN, 5150);
				return true;
			}

			std::string name(code);
			if (name.find('"') != std::string::npos || name.find('\\') != std::string::npos)
				player->GetSession()->SendNotification(LANG_PRESET_ERR_INVALID_NAME);
			else
			{
				for (uint8 presetID = 0; presetID < sT->GetMaxSets(); ++presetID) // should never reach over max
				{
					if (sT->presetByName[player->GetGUID()].find(presetID) != sT->presetByName[player->GetGUID()].end())
						continue; // Just remember never to use presetByName[pGUID][presetID] when finding etc!

					int32 cost = 0;
					std::map<uint8, uint32> items;
					for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; ++slot)
					{
						if (!sT->GetSlotName(slot, player->GetSession()))
							continue;
						if (Item* newItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
						{
							uint32 entry = sT->GetFakeEntry(newItem->GetGUID());
							if (!entry)
								continue;
							const ItemTemplate* temp = sObjectMgr->GetItemTemplate(entry);
							if (!temp)
								continue;
							if (!sT->SuitableForTransmogrification(player, temp))
								continue;
							cost += sT->GetSpecialPrice(temp);
							items[slot] = entry;
						}
					}

					if (items.empty())
						break; // no transmogrified items were found to be saved

					cost *= sT->GetSetCostModifier();
					cost += sT->GetSetCopperCost();
					if (!player->HasEnoughMoney(cost))
					{
						player->GetSession()->SendNotification(LANG_ERR_TRANSMOG_NOT_ENOUGH_MONEY);
						break;
					}

					std::ostringstream ss;
					for (std::map<uint8, uint32>::iterator it = items.begin(); it != items.end(); ++it)
					{
						ss << uint32(it->first) << ' ' << it->second << ' ';
						sT->presetById[player->GetGUID()][presetID][it->first] = it->second;
					}
					sT->presetByName[player->GetGUID()][presetID] = name; // Make sure code doesnt mess up SQL!
					CharacterDatabase.PExecute("REPLACE INTO `custom_transmogrification_sets` (`Owner`, `PresetID`, `SetName`, `SetData`) VALUES (%u, %u, \"%s\", \"%s\")",  player->GetGUIDLow(), uint32(presetID), name.c_str(), ss.str().c_str());

					if (cost)
						player->ModifyMoney(cost);
					break;
				}
			}
			//player->CLOSE_GOSSIP_MENU(); // Wait for SetMoney to get fixed, issue #10053
			OnGossipSelect(player, item, EQUIPMENT_SLOT_END+4, action);
		}

		bool MenuPrincipalTransfigurador(Player * player, Item * item)
		{
			player->PlayerTalkClass->ClearMenus();
			WorldSession* session = player->GetSession();
			if (sT->GetEnableTransmogInfo())
				player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Misc_Book_11:30:30:-15:0|tExplicaci""\xC3\xB3""n de la transfiguraci""\xC3\xB3""n", EQUIPMENT_SLOT_END+9, 0);
			for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; ++slot)
			{
				if (const char* slotName = sT->GetSlotName(slot, session))
				{
					Item* newItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
					uint32 entry = newItem ? sT->GetFakeEntry(newItem->GetGUID()) : 0;
					std::string icon = entry ? sT->GetItemIcon(entry,30,30,-15,0) : sT->GetSlotIcon(slot, 30, 30, -15, 0);
					player->ADD_GOSSIP_ITEM(1, icon+std::string(slotName), EQUIPMENT_SLOT_END, slot);
				}
			}
			
			#ifdef PRESETS
				if (sT->GetEnableSets())
					player->ADD_GOSSIP_ITEM(1,"|TInterface/ICONS/INV_Misc_EngGizmos_19:30:30:-15:0|tAdministrador de Sets", EQUIPMENT_SLOT_END+4, 0);
			#endif
			
			player->ADD_GOSSIP_ITEM_EXTENDED(1, "|TInterface/PaperDollInfoFrame/UI-GearManager-LeaveItem-Opaque:30:30:-15:0|tRemover las transfiguraciones", EQUIPMENT_SLOT_END+2, 0, """\xc2\xbf""Deseas remover las transfiguraciones de todos los objetos equipados?", 0, false);
			player->ADD_GOSSIP_ITEM(1, "|TInterface/PaperDollInfoFrame/UI-GearManager-Undo:30:30:-15:0|tActualizar Men""\xC3\xBA""", EQUIPMENT_SLOT_END+1, 0);
			player->ADD_GOSSIP_ITEM(1, "|cff00ff00|TInterface\\icons\\Achievement_BG_returnXflags_def_WSG:30:30:-15:0|t|rMenú principal del Libro", GOSSIP_SENDER_MAIN, 0);

			player->SEND_GOSSIP_MENU(1, item->GetGUID());
			return true;
		}
			
		static bool HandleSpectateCommand(ChatHandler* handler, const char *args)
		{
            Player* target;
            uint64 target_guid;
            std::string target_name;
            if (!handler->extractPlayerTarget((char*)args, &target, &target_guid, &target_name))
                return true;

            Player* player = handler->GetSession()->GetPlayer();
            if (target == player || target_guid == player->GetGUID())
            {
                handler->SendSysMessage(LANG_CANT_TELEPORT_SELF);
                handler->SetSentErrorMessage(true);
                return true;
            }

            if (player->isInCombat())
			{
				ChatHandler(player).PSendSysMessage(LANG_YOU_IN_COMBAT);
				return true;
			}

			if(player->GetMap()->IsBattlegroundOrArena())
			{
				ChatHandler(player).PSendSysMessage(LANG_SPEC_ON_ARENA_OR_BG);
				handler->SetSentErrorMessage(true);
				return true;
			}
			
			if(player->HasStealthAura() || player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || player->isDead())
			{
				ChatHandler(player).PSendSysMessage("No puedes hacer eso estando en sigilo.");
				handler->SetSentErrorMessage(true);
				return true;
			}
			
			if (player->GetZoneId() == 14)
			{
				ChatHandler(player).PSendSysMessage("No puedes hacer eso estando en la Zona PvP.");
				return true;
			}

			if (player->isSpectator())
			{
				handler->SendSysMessage(LANG_SPEC_ALREADY_SPECTATOR);
				handler->SetSentErrorMessage(true);
				return true;
			}

            if (!target)
            {
                handler->SendSysMessage(LANG_PLAYER_NOT_EXIST_OR_OFFLINE);
                handler->SetSentErrorMessage(true);
                return true;
            }

            if (player->GetPet())
            {
                handler->PSendSysMessage("Debes guardar tu mascota.");
                handler->SetSentErrorMessage(true);
                return true;
            }

			if (player->IsMounted())
			{
                handler->PSendSysMessage("Debes bajarte de tu montura.");
                handler->SetSentErrorMessage(true);
                return true;
            }

			if (player->InBattlegroundQueue() || !player->CanJoinToBattleground() || player->isUsingLfg())
			{
                handler->PSendSysMessage("Debes abandonar la cola de Arenas o BG.");
                handler->SetSentErrorMessage(true);
                return true;
            }

            if (player->GetMap()->IsBattlegroundOrArena() && !player->isSpectator())
            {
                handler->PSendSysMessage("Ya estas en un campo de batalla.");
                handler->SetSentErrorMessage(true);
                return true;
            }

            Map* cMap = target->GetMap();
            if (!cMap->IsBattleArena())
            {
                handler->PSendSysMessage("Jugador no encontrado en la arena");
                handler->SetSentErrorMessage(true);
                return true;
            }

            if (player->GetMap()->IsBattleground())
            {
                handler->PSendSysMessage("No puedes hacer eso mientras estas en un campo de batalla.");
                handler->SetSentErrorMessage(true);
                return true;
            }

            // all's well, set bg id
            // when porting out from the bg, it will be reset to 0
            player->SetBattlegroundId(target->GetBattlegroundId(), target->GetBattlegroundTypeId());
            // remember current position as entry point for return at bg end teleportation
            if (!player->GetMap()->IsBattlegroundOrArena())
                player->SetBattlegroundEntryPoint();

            if (target->isSpectator())
            {
                handler->PSendSysMessage("No puedes hacer eso, el jugador es un espectador.");
                handler->SetSentErrorMessage(true);
                return true;
            }

            // stop flight if need
            if (player->isInFlight())
            {
                player->GetMotionMaster()->MovementExpired();
                player->CleanupAfterTaxiFlight();
            }
            // save only in non-flight case
            else
                player->SaveRecallPosition();

            // search for two teams
            Battleground *bGround = target->GetBattleground();
            if (bGround->isRated())
            {
                uint32 slot = bGround->GetArenaType() - 2;
                if (bGround->GetArenaType() > 3)
                    slot = 2;
                uint32 firstTeamID = target->GetArenaTeamId(slot);
                uint32 secondTeamID = 0;
                Player *firstTeamMember  = target;
                Player *secondTeamMember = NULL;
                for (Battleground::BattlegroundPlayerMap::const_iterator itr = bGround->GetPlayers().begin(); itr != bGround->GetPlayers().end(); ++itr)
                    if (Player* tmpPlayer = ObjectAccessor::FindPlayer(itr->first))
                    {
                        if (tmpPlayer->isSpectator())
                            continue;

                        uint32 tmpID = tmpPlayer->GetArenaTeamId(slot);
                        if (tmpID != firstTeamID && tmpID > 0)
                        {
                            secondTeamID = tmpID;
                            secondTeamMember = tmpPlayer;
                            break;
                        }
                    }

                if (firstTeamID > 0 && secondTeamID > 0 && secondTeamMember)
                {
                    ArenaTeam *firstTeam  = sArenaTeamMgr->GetArenaTeamById(firstTeamID);
                    ArenaTeam *secondTeam = sArenaTeamMgr->GetArenaTeamById(secondTeamID);
                    if (firstTeam && secondTeam)
                    {
                        handler->PSendSysMessage("Has entrado en una arena puntuada.");
                        handler->PSendSysMessage("Equipos:");
                        handler->PSendSysMessage("%s - %s", firstTeam->GetName().c_str(), secondTeam->GetName().c_str());
                        handler->PSendSysMessage("%u(%u) - %u(%u)", firstTeam->GetRating(), firstTeam->GetAverageMMR(firstTeamMember->GetGroup()),
                                                                    secondTeam->GetRating(), secondTeam->GetAverageMMR(secondTeamMember->GetGroup()));
                    }
                }
            }

			//Custom
			handler->PSendSysMessage("|CFFFF0000Recuerda que dispones de los comandos:|R");
			handler->PSendSysMessage("|CFF87CEFA.spectate player |CFF800080#nombre_jugador|R : Sirve para espectar a un jugador en concreto");
			handler->PSendSysMessage("|CFF87CEFA.spectate view |CFF800080#nombre_jugador|R : Pasas a ver la misma vista que el jugador #nombre_jugador");
			handler->PSendSysMessage("|CFF87CEFA.spectate reset|R : Reinicia la cámara por si has puesto el comando .spectate view");
			handler->PSendSysMessage("|CFF87CEFA.spectate leave|R : Dejas de espectar la Arena y vuelves a tu posición original.");

			ItemPosCountVec off_dest;
			ItemPosCountVec off_dest_2;
			//Item* RangedItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_RANGED);
			Item* FirstItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
			Item* OffItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

			/*uint8 off_msg = player->CanStoreItem(NULL_BAG, NULL_SLOT, off_dest, RangedItem, false);
			if(off_msg == EQUIP_ERR_OK)
			{
				player->RemoveItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_RANGED, true);
				player->StoreItem(off_dest, RangedItem, true);
			}*/

			InventoryResult off_msg_2 = player->CanStoreItem(NULL_BAG, NULL_SLOT, off_dest, FirstItem, false);
			if(off_msg_2 == EQUIP_ERR_OK)
			{
				player->RemoveItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND, true);
				player->StoreItem(off_dest, FirstItem, true);
			}
			else
                player->SendEquipError(off_msg_2, FirstItem, NULL);

			InventoryResult off_msg_3 = player->CanStoreItem(NULL_BAG, NULL_SLOT, off_dest_2, OffItem, false);
			if(off_msg_3 == EQUIP_ERR_OK)
			{
				player->RemoveItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND, true);
				player->StoreItem(off_dest_2, OffItem, true);
			}
			else
                player->SendEquipError(off_msg_3, OffItem, NULL);

            // to point to see at target with same orientation
            float x, y, z;
            target->GetContactPoint(player, x, y, z);

            player->TeleportTo(target->GetMapId(), x, y, z, player->GetAngle(target), TELE_TO_GM_MODE);
			//std::string chrNameLink = handler->playerLink(target_name);
			//handler->PSendSysMessage("Has aparecido ante %u", chrNameLink.c_str());
            player->SetPhaseMask(target->GetPhaseMask(), true);
            player->SetSpectate(true);
            target->GetBattleground()->AddSpectator(player->GetGUID());

            return true;
		}
};

void AddSC_itemgossip()
{
   new itemgossip();
}