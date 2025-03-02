#include "mod_starterguild.h"
#include <regex>

void StarterGuild::OnPlayerLogin(Player* player)
{
    if (sConfigMgr->GetOption<bool>("StarterGuild.Enable", true))
        if (sConfigMgr->GetOption<bool>("StarterGuild.Announce", true))
            ChatHandler(player->GetSession()).SendSysMessage("This server is running the |cff4CFF00StarterGuild |rmodule.");
}

void StarterGuild::OnPlayerLevelChanged(Player * player, uint8 previousLevel)
{
    if (sConfigMgr->GetOption<bool>("StarterGuild.Enable", true))
    {
        auto level = sConfigMgr->GetOption<uint8>("StarterGuild.Level", 0);

        if (level > 0 && !player->GetGuild() && (player->GetLevel() == level || (player->GetLevel() > level && previousLevel < level)))
            addPlayerToGuild(player);
    }
}

void StarterGuild::OnPlayerFirstLogin(Player* player)
{
    if (sConfigMgr->GetOption<bool>("StarterGuild.Enable", true) && ((sConfigMgr->GetOption<uint8>("StarterGuild.Level", 0) == 0)||player->GetLevel()>=sConfigMgr->GetOption<uint8>("StarterGuild.Level", 0)))
        addPlayerToGuild(player);
}

void StarterGuild::addPlayerToGuild(Player* player)
{
    const uint32 GUILD_ID_HORDE = sConfigMgr->GetOption<uint32>("StarterGuild.Horde", 1);
    const uint32 GUILD_ID_ALLIANCE = sConfigMgr->GetOption<uint32>("StarterGuild.Alliance", 2);

    const std::string GUILD_WELCOME_TEXT_HORDE = sConfigMgr->GetOption<std::string>("StarterGuild.HordeWelcomeText", "Lok’tar ogar! Welcome to the horde starter guild <GUILD> <PLAYER>.");
    const std::string GUILD_WELCOME_TEXT_ALLIANCE = sConfigMgr->GetOption<std::string>("StarterGuild.AllianceWelcomeText", "Welcome to the alliance starter guild <GUILD> <PLAYER>. For the Alliance!");

    Guild* guild = sGuildMgr->GetGuildById(player->GetTeamId() == TEAM_ALLIANCE ? GUILD_ID_ALLIANCE : GUILD_ID_HORDE);

    // If a guild is present, assign the character to the guild; otherwise skip assignment.
    if (guild)
    {
        if (guild->GetMemberCount() <= guildMemberCount)
        {
            guild->AddMember(player->GetGUID());

            // Inform the player they have joined the guild
            std::string welcome_text = player->GetTeamId() == TEAM_ALLIANCE ? GUILD_WELCOME_TEXT_ALLIANCE : GUILD_WELCOME_TEXT_HORDE;

            welcome_text = std::regex_replace(welcome_text, std::regex("<GUILD>"), player->GetGuildName());
            welcome_text = std::regex_replace(welcome_text, std::regex("<PLAYER>"), player->GetName());

            ChatHandler(player->GetSession()).SendSysMessage(welcome_text.c_str());
        }
        else
            ChatHandler(player->GetSession()).SendSysMessage("The brotherhood has exceeded its membership limit.");
    }
    else
        ChatHandler(player->GetSession()).SendSysMessage("Brotherhood does not exist.");
}
