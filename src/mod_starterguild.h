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


#include "Chat.h"
#include "Configuration/Config.h"
#include "GuildMgr.h"
#include "Player.h"
#include "ScriptMgr.h"

#define Welcome_Name "Notice"
uint32 guildMemberCount = 1000;

class StarterGuild : public PlayerScript
{
public:
    StarterGuild() : PlayerScript("StarterGuild", {
        PLAYERHOOK_ON_LOGIN,
        PLAYERHOOK_ON_FIRST_LOGIN,
        PLAYERHOOK_ON_LEVEL_CHANGED
    }) { }

    void OnPlayerLogin(Player* player) override;
    void OnPlayerFirstLogin(Player* player) override;
    void addPlayerToGuild(Player* player);
    void OnPlayerLevelChanged(Player * player, uint8 previousLevel) override;

};

void AddStarterGuildScripts()
{
    new StarterGuild();
}
