#
# Copyright (C) 2008 - 2011 Trinity <http://www.trinitycore.org/>
#
# Copyright (C) 2010 - 2013 Myth Project <http://mythprojectnetwork.blogspot.com/>
#
# Myth Project's source is based on the Trinity Project source, you can find the
# link to that easily in Trinity Copyrights. Myth Project is a private community.
# To get access, you either have to donate or pass a developer test.
# You may not share Myth Project's sources! For personal use only.
#

file(GLOB_RECURSE scripts_easternkingdoms EasternKingdoms/*.cpp EasternKingdoms/*.h)
file(GLOB_RECURSE scripts_kalimdor Kalimdor/*.cpp Kalimdor/*.h)
file(GLOB_RECURSE scripts_northrend Northrend/*.cpp Northrend/*.h)
file(GLOB_RECURSE scripts_outland Outland/*.cpp Outland/*.h)
file(GLOB_RECURSE scripts_world World/*.cpp World/*.h)
file(GLOB_RECURSE scripts_spells Spells/*.cpp Spells/*.h)
file(GLOB_RECURSE scripts_outdoorpvp OutdoorPvP/*.cpp OutdoorPvP/*.h)
file(GLOB_RECURSE scripts_commands Commands/*.cpp Commands/*.h)

source_group(EasternKingdoms FILES ${scripts_easternkingdoms})
source_group(Kalimdor FILES ${scripts_kalimdor})
source_group(Northrend FILES ${scripts_northrend})
source_group(Outland FILES ${scripts_outland})
source_group(World FILES ${scripts_world})
source_group(Spells FILES ${scripts_spells})
source_group(OutdoorPvP FILES ${scripts_outdoorpvp})
source_group(Commands FILES ${scripts_commands})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(scripts_STAT_SRCS
  PrecompiledHeaders/ScriptPCH.cpp
  PrecompiledHeaders/ScriptPCH.h)

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  ${scripts_easternkingdoms}
  ${scripts_kalimdor}
  ${scripts_northrend}
  ${scripts_outland}
  ${scripts_world}
  World/Custom/npc_waypoint.cpp
  World/Custom/Transmogrification.h
  World/Custom/Transmogrification.cpp
  World/Custom/Transmogrifier.cpp
  World/Custom/Anuncio_boss.cpp
  World/Custom/ArenaFastStart.cpp
  World/Custom/boss_sylvanas.cpp
  World/Custom/boss_malfurion.cpp
  World/Custom/comandos_jugadores.cpp
  World/Custom/defensor_shop.cpp
  World/Custom/duel_reset.cpp
  World/Custom/gameobjects_shop.cpp
  World/Custom/gh.cpp
  World/Custom/GMItems.cpp
  World/Custom/npc_ah.cpp
  World/Custom/npc_beastmaster.cpp
  World/Custom/npc_buff.cpp
  World/Custom/npc_duel.cpp
  World/Custom/npc_enchanter.cpp
  World/Custom/npc_skill.cpp
  World/Custom/pvptitle.cpp
  World/Custom/racha.cpp
  World/Custom/skill_npc.cpp
  World/Custom/Top2v2y3v3.cpp
  World/Custom/vipcommands.cpp
  World/Custom/arena_top_teams.cpp
  World/Custom/anuncio_gm.cpp
  World/Custom/probador_monturas_custom.cpp
  World/Custom/npc_enchant.cpp
  World/Custom/cs_fake.cpp
  World/Custom/channel_factions.cpp
  World/Custom/arenaqueue.cpp
  World/Custom/IslaPvP.cpp
  World/Custom/Teleporter.cpp
  World/Custom/cofre_gurubashi.cpp
  World/Custom/S1m421_HyjalBandSystem.cpp
  World/Custom/player_abuse.cpp
  World/Custom/vendedor_pvp.cpp
  World/Custom/fin_season.cpp
  World/Custom/testitem.cpp
  World/Custom/itemgossip.cpp
  World/Custom/itemgossiphelp.cpp
  World/Custom/itemgossiphelp.h
  World/Custom/npc_arena1v1.cpp
  World/Custom/npc_arena1v1.h
  World/Custom/ChallengeScript.cpp
  World/Custom/libro_comandos.cpp
  
  ${scripts_spells}
  ${scripts_outdoorpvp}
  ${scripts_commands}
  
  ../game/AI/ScriptedAI/ScriptedEscortAI.cpp
  ../game/AI/ScriptedAI/ScriptedCreature.cpp
  ../game/AI/ScriptedAI/ScriptedFollowerAI.cpp
  ../game/AI/ScriptedAI/ScriptedSimpleAI.cpp
)

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/dep/mersennetwister
  ${CMAKE_SOURCE_DIR}/dep/zlib
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/dep/g3dlite/include
  ${CMAKE_SOURCE_DIR}/src/server/shared/Configuration
  ${CMAKE_SOURCE_DIR}/src/server/shared/Cryptography
  ${CMAKE_SOURCE_DIR}/src/server/shared/Database
  ${CMAKE_SOURCE_DIR}/src/server/shared/DataStores
  ${CMAKE_SOURCE_DIR}/src/server/shared/Debugging
  ${CMAKE_SOURCE_DIR}/src/server/shared/Dynamic/LinkedReference
  ${CMAKE_SOURCE_DIR}/src/server/shared/Dynamic
  ${CMAKE_SOURCE_DIR}/src/server/shared/Logging
  ${CMAKE_SOURCE_DIR}/src/server/shared/Packets
  ${CMAKE_SOURCE_DIR}/src/server/shared/Threading
  ${CMAKE_SOURCE_DIR}/src/server/shared/Utilities
  ${CMAKE_SOURCE_DIR}/src/server/collision
  ${CMAKE_SOURCE_DIR}/src/server/collision/Management
  ${CMAKE_SOURCE_DIR}/src/server/collision/Models
  ${CMAKE_SOURCE_DIR}/src/server/pathfinding
  ${CMAKE_SOURCE_DIR}/src/server/pathfinding/detour
  ${CMAKE_SOURCE_DIR}/src/server/pathfinding/recast
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/shared/Database
  ${CMAKE_SOURCE_DIR}/src/server/game/Accounts
  ${CMAKE_SOURCE_DIR}/src/server/game/Achievements
  ${CMAKE_SOURCE_DIR}/src/server/game/Addons
  ${CMAKE_SOURCE_DIR}/src/server/game/AI
  ${CMAKE_SOURCE_DIR}/src/server/game/AI/CoreAI
  ${CMAKE_SOURCE_DIR}/src/server/game/AI/EventAI
  ${CMAKE_SOURCE_DIR}/src/server/game/AI/ScriptedAI
  ${CMAKE_SOURCE_DIR}/src/server/game/AI/SmartScripts
  ${CMAKE_SOURCE_DIR}/src/server/game/AuctionHouse
  ${CMAKE_SOURCE_DIR}/src/server/game/Battlegrounds
  ${CMAKE_SOURCE_DIR}/src/server/game/Battlegrounds/Zones
  ${CMAKE_SOURCE_DIR}/src/server/game/Chat
  ${CMAKE_SOURCE_DIR}/src/server/game/Chat/Channels
  ${CMAKE_SOURCE_DIR}/src/server/game/Conditions
  ${CMAKE_SOURCE_DIR}/src/server/shared/Configuration
  ${CMAKE_SOURCE_DIR}/src/server/game/Combat
  ${CMAKE_SOURCE_DIR}/src/server/game/DataStores
  ${CMAKE_SOURCE_DIR}/src/server/game/DungeonFinding
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/Corpse
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/Creature
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/DynamicObject
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/Item
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/Item/Container
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/GameObject
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/Object
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/Object/Updates
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/Pet
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/Player
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/Transport
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/Unit
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/Vehicle
  ${CMAKE_SOURCE_DIR}/src/server/game/Events
  ${CMAKE_SOURCE_DIR}/src/server/game/Globals
  ${CMAKE_SOURCE_DIR}/src/server/game/Grids
  ${CMAKE_SOURCE_DIR}/src/server/game/Grids/Cells
  ${CMAKE_SOURCE_DIR}/src/server/game/Grids/Notifiers
  ${CMAKE_SOURCE_DIR}/src/server/game/Groups
  ${CMAKE_SOURCE_DIR}/src/server/game/Guilds
  ${CMAKE_SOURCE_DIR}/src/server/game/Instances
  ${CMAKE_SOURCE_DIR}/src/server/game/LookingForGroup
  ${CMAKE_SOURCE_DIR}/src/server/game/Loot
  ${CMAKE_SOURCE_DIR}/src/server/game/Mails
  ${CMAKE_SOURCE_DIR}/src/server/game/Miscellaneous
  ${CMAKE_SOURCE_DIR}/src/server/game/Maps
  ${CMAKE_SOURCE_DIR}/src/server/game/Movement
  ${CMAKE_SOURCE_DIR}/src/server/game/Movement/Pathfinding
  ${CMAKE_SOURCE_DIR}/src/server/game/Movement/MovementGenerators
  ${CMAKE_SOURCE_DIR}/src/server/game/Movement/Waypoints
  ${CMAKE_SOURCE_DIR}/src/server/game/Opcodes
  ${CMAKE_SOURCE_DIR}/src/server/game/OutdoorPvP
  ${CMAKE_SOURCE_DIR}/src/server/game/PlayerMovementChecker
  ${CMAKE_SOURCE_DIR}/src/server/game/Pools
  ${CMAKE_SOURCE_DIR}/src/server/game/PrecompiledHeaders
  ${CMAKE_SOURCE_DIR}/src/server/game/Quests
  ${CMAKE_SOURCE_DIR}/src/server/game/Reputation
  ${CMAKE_SOURCE_DIR}/src/server/game/Scripting
  ${CMAKE_SOURCE_DIR}/src/server/game/Server
  ${CMAKE_SOURCE_DIR}/src/server/game/Server/Protocol
  ${CMAKE_SOURCE_DIR}/src/server/game/Server/Protocol/Handlers
  ${CMAKE_SOURCE_DIR}/src/server/game/Skills
  ${CMAKE_SOURCE_DIR}/src/server/game/Spells
  ${CMAKE_SOURCE_DIR}/src/server/game/Spells/Auras
  ${CMAKE_SOURCE_DIR}/src/server/game/Texts
  ${CMAKE_SOURCE_DIR}/src/server/game/Tickets
  ${CMAKE_SOURCE_DIR}/src/server/game/Tools
  ${CMAKE_SOURCE_DIR}/src/server/game/Weather
  ${CMAKE_SOURCE_DIR}/src/server/game/World
  ${CMAKE_SOURCE_DIR}/src/server/game/Warden
  ${CMAKE_SOURCE_DIR}/src/server/scripts/OutdoorPvP
  ${CMAKE_CURRENT_SOURCE_DIR}/PrecompiledHeaders
  ${ACE_INCLUDE_DIR}
  ${MYSQL_INCLUDE_DIR}
  ${OPENSSL_INCLUDE_DIR}
)

add_library(scripts STATIC ${scripts_STAT_SRCS})

if(CMAKE_COMPILER_IS_GNUCXX)
  add_precompiled_header(scripts ${CMAKE_CURRENT_SOURCE_DIR}/PrecompiledHeaders/ScriptPCH.h)
elseif(MSVC)
  add_native_precompiled_header(scripts ${CMAKE_CURRENT_SOURCE_DIR}/PrecompiledHeaders/ScriptPCH)
endif()
