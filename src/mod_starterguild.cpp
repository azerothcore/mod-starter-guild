#include "mod_starterguild.h"
#include "GuildMgr.h"
#include "Log.h"
#include <fmt/format.h>

void StarterGuild::OnPlayerLogin(Player* player)
{
    if (sConfigMgr->GetOption<bool>("StarterGuild.Enable", true))
        if (sConfigMgr->GetOption<bool>("StarterGuild.Announce", true))
            ChatHandler(player->GetSession()).SendSysMessage("This server is running the |cff4CFF00StarterGuild |rmodule.");
}

void StarterGuild::OnPlayerLevelChanged(Player* player, uint8 previousLevel)
{
    if (sConfigMgr->GetOption<bool>("StarterGuild.Enable", true))
    {
        auto level = sConfigMgr->GetOption<uint8>("StarterGuild.Level", 0);

        if (level > 0 && !player->GetGuild() && (player->GetLevel() == level || (player->GetLevel() > level && previousLevel < level)))
            AddPlayerToGuild(player);
    }
}

void StarterGuild::OnPlayerFirstLogin(Player* player)
{
    if (sConfigMgr->GetOption<bool>("StarterGuild.Enable", true) &&
        ((sConfigMgr->GetOption<uint8>("StarterGuild.Level", 0) == 0) || player->GetLevel() >= sConfigMgr->GetOption<uint8>("StarterGuild.Level", 0)))
        AddPlayerToGuild(player);
}

void StarterGuild::AddPlayerToGuild(Player* player)
{
    // Custom-race compatibility (Race IDs 1-31 and beyond):
    // Refresh the player's live team/faction from ChrRaces.dbc before selecting
    // a starter guild. Some custom races can reach PLAYERHOOK_ON_FIRST_LOGIN
    // with an outdated m_team value even though ChrRaces.dbc already contains
    // the correct TeamID. Guild::AddMember() compares the live team values, so
    // normalizing here prevents valid custom Horde/Alliance races from being
    // rejected on their first login.
    player->SetFactionForRace(player->getRace());
    TeamId const teamId = player->GetTeamId();

    auto const GUILD_ID_HORDE = sConfigMgr->GetOption<uint32>("StarterGuild.Horde", 1);
    auto const GUILD_ID_ALLIANCE = sConfigMgr->GetOption<uint32>("StarterGuild.Alliance", 2);

    auto const GUILD_WELCOME_TEXT_HORDE = sConfigMgr->GetOption<std::string>("StarterGuild.HordeWelcomeText", "Lok’tar ogar! Welcome to the horde starter guild {GUILD} {PLAYER}.");
    auto const GUILD_WELCOME_TEXT_ALLIANCE =
        sConfigMgr->GetOption<std::string>("StarterGuild.AllianceWelcomeText", "Welcome to the alliance starter guild {GUILD} {PLAYER}. For the Alliance!");

    Guild* guild = sGuildMgr->GetGuildById(teamId == TEAM_ALLIANCE ? GUILD_ID_ALLIANCE : GUILD_ID_HORDE);

    // If a guild is present, assign the character to the guild; otherwise skip assignment.
    if (!guild)
    {
        ChatHandler(player->GetSession()).SendSysMessage("Brotherhood does not exist.");
        return;
    }

    if (guild->GetMemberCount() > guildMemberCount)
    {
        ChatHandler(player->GetSession()).SendSysMessage("The brotherhood has exceeded its membership limit.");
        return;
    }

    if (!guild->AddMember(player->GetGUID()))
    {
        LOG_ERROR("starterguild",
            "Failed to add player {} (GUID {}) to guild {} (ID {}).",
            player->GetPlayerName(),
            player->GetGUID().ToString(),
            guild->GetName(),
            guild->GetId());
        ChatHandler(player->GetSession()).SendSysMessage("Failed to join guild.");
        return;
    }

    // Inform the player they have joined the guild
    std::string welcome_text = teamId == TEAM_ALLIANCE ? GUILD_WELCOME_TEXT_ALLIANCE : GUILD_WELCOME_TEXT_HORDE;
    std::string guild_name = guild->GetName();
    std::string player_name = player->GetPlayerName();

    welcome_text = fmt::format(fmt::runtime(welcome_text), fmt::arg("GUILD", guild_name), fmt::arg("PLAYER", player_name));

    ChatHandler(player->GetSession()).SendSysMessage(welcome_text);
}
