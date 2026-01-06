const { MessageEmbed } = require('discord.js')
    
module.exports = {
    name: 'help',
    description: 'Sends command help.',
    execute(message) {
        const embed = new MessageEmbed()
            .setTitle('Command Help')
            .setDescription('Sends command help.')
            .addFields(
                {name: 'Help', value: 'Sends command help.'}
            )
            .setTimestamp()
            .setColor('#FFFFFF')
    
        message.reply({embeds: [embed], allowedMentions: {repliedUser: false}})
    }
}