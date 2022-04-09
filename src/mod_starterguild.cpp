/*

# Starter Guild #

#### A module for AzerothCore (https://github.com/azerothcore/mod-starter-guild)
#### Originally by [StygianTheBest](https://github.com/StygianTheBest/AzerothCore-Content/tree/master/Modules)
------------------------------------------------------------------------------------------------------------------


### Description ###
------------------------------------------------------------------------------------------------------------------
This module automatically joins new players to a guild of your choice on first login.


### Data ###
------------------------------------------------------------------------------------------------------------------
- Type: Player/Server
- Script: StarterGuild
- Config: Yes
    - Enable Module
    - Enable Module Announce
    - Set ID for Horde and Alliance Guilds
- SQL: No


### Version ###
------------------------------------------------------------------------------------------------------------------
- v2022.04.09 - Fixed deprecation warnings and renamed files/updated comments
- v2017.08.01
- v2017.08.02 - Fix crash prevention when no guilds exist


### Credits ###
------------------------------------------------------------------------------------------------------------------
- [Blizzard Entertainment](http://blizzard.com)
- [TrinityCore](https://github.com/TrinityCore/TrinityCore/blob/3.3.5/THANKS)
- [SunwellCore](http://www.azerothcore.org/pages/sunwell.pl/)
- [AzerothCore](https://github.com/AzerothCore/azerothcore-wotlk/graphs/contributors)
- [AzerothCore Discord](https://discord.gg/gkt4y2x)
- [EMUDevs](https://youtube.com/user/EmuDevs)
- [AC-Web](http://ac-web.org/)
- [ModCraft.io](http://modcraft.io/)
- [OwnedCore](http://ownedcore.com/)
- [OregonCore](https://wiki.oregon-core.net/)
- [Wowhead.com](http://wowhead.com)
- [AoWoW](https://wotlk.evowow.com/)
- [SoulSeekkor](https://github.com/SoulSeekkor)


### License ###
------------------------------------------------------------------------------------------------------------------
- This code and content is released under the [GNU AGPL v3](https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3).

*/


#include "ScriptMgr.h"
#include "Player.h"
#include "GuildMgr.h"
#include "Configuration/Config.h"
#include "Chat.h"

#define Welcome_Name "Notice"

class StarterGuild : public PlayerScript
{

public:
	StarterGuild() : PlayerScript("StarterGuild") { }

	// Announce Module
	void OnLogin(Player *player)
	{
		if (sConfigMgr->GetOption<bool>("StarterGuild.Enable", true))
		{
			if (sConfigMgr->GetOption<bool>("StarterGuild.Announce", true))
			{
				ChatHandler(player->GetSession()).SendSysMessage("This server is running the |cff4CFF00StarterGuild |rmodule.");
			}

            		uint8 level = sConfigMgr->GetOption<uint8>("StarterGuild.Level", 0);
            		if (level > 0 && player->getLevel() == level && !player->GetGuild()) {
                		addPlayerToGuild(player);
            		}
		}
	}

	// Starter Guild
	void OnFirstLogin(Player* player)
	{
		// If enabled...
		if (sConfigMgr->GetOption<bool>("StarterGuild.Enable", true) && sConfigMgr->GetOption<uint8>("StarterGuild.Level", 0) == 0)
		{
        	        addPlayerToGuild(player);
		}
	}

	void addPlayerToGuild(Player* player) {
		const uint32 GUILD_ID_ALLIANCE = sConfigMgr->GetOption<uint32>("StarterGuild.Alliance", 0);
		const uint32 GUILD_ID_HORDE = sConfigMgr->GetOption<uint32>("StarterGuild.Horde", 0);

		Guild* guild = sGuildMgr->GetGuildById(player->GetTeamId() == TEAM_ALLIANCE ? GUILD_ID_ALLIANCE : GUILD_ID_HORDE);

		// If a guild is present, assign the character to the guild; otherwise skip assignment.
		if (guild)
		{
			guild->AddMember(player->GetGUID());

			// Inform the player they have joined the guild
			std::ostringstream ss;
			ss << "Welcome to the " << player->GetGuildName() << " guild " << player->GetName() << "!";
			ChatHandler(player->GetSession()).SendSysMessage(ss.str().c_str());
		}
    }
};

void AddStarterGuildScripts()
{
	new StarterGuild();
}