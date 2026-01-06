

    const Discord = require('discord.js');
    const client = new Discord.Client({ intents: ['DIRECT_MESSAGES', 'GUILDS', 'GUILD_BANS', 'GUILD_MEMBERS', 'GUILD_MESSAGES', 'GUILD_WEBHOOKS'], partials: ['CHANNEL', 'MESSAGE'] });
    const prefix = '?'
    const fs = require('fs')
     
    client.commands = new Discord.Collection();
     
    // Filter Commands //
    const commandFiles = fs.readdirSync('./commands/').filter(file => file.endsWith('.js'));
     
    // Fetch Commands //
    for (const file of commandFiles) {
        const command = require(`./commands/${file}`);
     
        client.commands.set(command.name, command);
    }
     
    client.on('ready', () => {
        console.log(`Bot is online!`);
        client.user.setPresence({ activities: [{ name: 'with commands' }] });
    });
     
    client.on('messageCreate', async message => {
        // Define Command Components //
        const args = message.content.slice(prefix.length).split(/ +/);
        const command = args.shift().toLowerCase()
     
        if (command == 'help') {
            client.commands.get('help').execute(message)
        }
    })
     
    client.login('token')