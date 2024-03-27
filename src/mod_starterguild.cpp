#include "mod_starterguild.h"

void StarterGuild::OnLogin(Player* player)
{
    if (sConfigMgr->GetOption<bool>("StarterGuild.Enable", true))
    {
        if (sConfigMgr->GetOption<bool>("StarterGuild.Announce", true))
        {
            ChatHandler(player->GetSession()).SendSysMessage("This server is running the |cff4CFF00StarterGuild |rmodule.");
        }

        uint8 level = sConfigMgr->GetOption<uint8>("StarterGuild.Level", 0);

        if (level > 0 && player->getLevel() == level && !player->GetGuild())
        {
            addPlayerToGuild(player);
        }
    }
}

void StarterGuild::OnFirstLogin(Player* player)
{
    if (sConfigMgr->GetOption<bool>("StarterGuild.Enable", true) && (sConfigMgr->GetOption<uint8>("StarterGuild.Level", 0) == 0))
    {
        addPlayerToGuild(player);
    }
}

void StarterGuild::addPlayerToGuild(Player* player)
{
    const uint32 GUILD_ID_HORDE = sConfigMgr->GetOption<uint32>("StarterGuild.Horde", 1);
    const uint32 GUILD_ID_ALLIANCE = sConfigMgr->GetOption<uint32>("StarterGuild.Alliance", 2);

    Guild* guild = sGuildMgr->GetGuildById(player->GetTeamId() == TEAM_ALLIANCE ? GUILD_ID_ALLIANCE : GUILD_ID_HORDE);

    // If a guild is present, assign the character to the guild; otherwise skip assignment.
    if (guild)
    {
        if (guild->GetMemberCount() <= guildMemberCount)
        {
            guild->AddMember(player->GetGUID());

            // Inform the player they have joined the guild
            std::ostringstream ss;
            ss << "Welcome to the " << player->GetGuildName() << " guild " << player->GetName() << "!";
            ChatHandler(player->GetSession()).SendSysMessage(ss.str().c_str());
        }
        else
        {
            ChatHandler(player->GetSession()).SendSysMessage("The brotherhood has exceeded its membership limit.");
        }
    }
    else
    {
        ChatHandler(player->GetSession()).SendSysMessage("Brotherhood does not exist.");
    }
}
