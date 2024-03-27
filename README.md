# ![logo](https://raw.githubusercontent.com/azerothcore/azerothcore.github.io/master/images/logo-github.png) AzerothCore

## Starter Guild

- Latest build status with azerothcore:

[![Build Status](https://github.com/azerothcore/mod-starter-guild/workflows/core-build/badge.svg)](https://github.com/azerothcore/mod-starter-guild)

## Description

This module automatically assigns new characters to a specific guild depending on the faction.

## Usage

- Create a new guild ingame for each faction with `.guild create` or directly into the database.
- Edit the configuration file.
- Create a new character and open the guild tab.
- Keep in mind that the limit of players is 1000 per guild.

## SQL example to create the guild

- Check that the ids do not exist, and in this case, the owner will be the first creating player, generally the administrator.

```sql
DELETE FROM `guild` WHERE `guildid` IN (1, 2);
INSERT INTO `guild` (`guildid`, `name`, `leaderguid`, `motd`) VALUES
(1, "Horde Starting Guild", 1, "Lok'Tar Ogar!"),
(2, "Alliance Starting Guild", 1, "Fight together, for the alliance");
```

## Credits

* [stygiancore](  http://stygianthebest.github.io ): (Author of the module).
* [SoulSeekkor]( https://github.com/SoulSeekkor ) cleaned up deprecation warnings and fixed naming.

* AzerothCore: [repository](https://github.com/azerothcore) - [website](http://azerothcore.org/) - [discord chat community](https://discord.gg/PaqQRkd)

## License

- This code and content is released under the [GNU AGPL v3](https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3).
