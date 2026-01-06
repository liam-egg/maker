const fs = require('node:fs'),
    images = [
        'iVBORw0KGgoAAAANSUhEUgAAAGQAAABkCAYAAABw4pVUAAAAAXNSR0IArs4c6QAAAW1JREFUeF7t1VEJADAMxNBVQf2r3WAq8vGqICQcnd29x2UMjCCZFh9EkFYPQWI9BBGkZiDG44cIEjMQw7EQQWIGYjgWIkjMQAzHQgSJGYjhWIggMQMxHAsRJGYghmMhgsQMxHAsRJCYgRiOhQgSMxDDsRBBYgZiOBYiSMxADMdCBIkZiOFYiCAxAzEcCxEkZiCGYyGCxAzEcCxEkJiBGI6FCBIzEMOxEEFiBmI4FiJIzEAMx0IEiRmI4ViIIDEDMRwLESRmIIZjIYLEDMRwLESQmIEYjoUIEjMQw7EQQWIGYjgWIkjMQAzHQgSJGYjhWIggMQMxHAsRJGYghmMhgsQMxHAsRJCYgRiOhQgSMxDDsRBBYgZiOBYiSMxADMdCBIkZiOFYiCAxAzEcCxEkZiCGYyGCxAzEcCxEkJiBGI6FCBIzEMOxEEFiBmI4FiJIzEAMx0IEiRmI4ViIIDEDMRwLESRmIIZjIYLEDMRwHuPZdZUfAIp/AAAAAElFTkSuQmCC',
        'iVBORw0KGgoAAAANSUhEUgAAAGQAAABkCAYAAABw4pVUAAAAAXNSR0IArs4c6QAAAW9JREFUeF7t1UENACAQxECwhi5cIRASVMyjp6Bps7m5z7qjYwzMgjAtPkhBrB4FwXoUpCCaAYynH1IQzACG00IKghnAcFpIQTADGE4LKQhmAMNpIQXBDGA4LaQgmAEMp4UUBDOA4bSQgmAGMJwWUhDMAIbTQgqCGcBwWkhBMAMYTgspCGYAw2khBcEMYDgtpCCYAQynhRQEM4DhtJCCYAYwnBZSEMwAhtNCCoIZwHBaSEEwAxhOCykIZgDDaSEFwQxgOC2kIJgBDKeFFAQzgOG0kIJgBjCcFlIQzACG00IKghnAcFpIQTADGE4LKQhmAMNpIQXBDGA4LaQgmAEMp4UUBDOA4bSQgmAGMJwWUhDMAIbTQgqCGcBwWkhBMAMYTgspCGYAw2khBcEMYDgtpCCYAQynhRQEM4DhtJCCYAYwnBZSEMwAhtNCCoIZwHBaSEEwAxhOCykIZgDDaSEFwQxgOC2kIJgBDKeFFAQzgOE80fz0JRtm/PoAAAAASUVORK5CYIIA',
        'iVBORw0KGgoAAAANSUhEUgAAAGQAAABkCAYAAABw4pVUAAAAAXNSR0IArs4c6QAAAW9JREFUeF7t1VENACAMxFDmA/8WkAcJKt5Hp6Bpc9mcve/qGANTEKbFBymI1aMgWI+CFEQzgPH0QwqCGcBwWkhBMAMYTgspCGYAw2khBcEMYDgtpCCYAQynhRQEM4DhtJCCYAYwnBZSEMwAhtNCCoIZwHBaSEEwAxhOCykIZgDDaSEFwQxgOC2kIJgBDKeFFAQzgOG0kIJgBjCcFlIQzACG00IKghnAcFpIQTADGE4LKQhmAMNpIQXBDGA4LaQgmAEMp4UUBDOA4bSQgmAGMJwWUhDMAIbTQgqCGcBwWkhBMAMYTgspCGYAw2khBcEMYDgtpCCYAQynhRQEM4DhtJCCYAYwnBZSEMwAhtNCCoIZwHBaSEEwAxhOCykIZgDDaSEFwQxgOC2kIJgBDKeFFAQzgOG0kIJgBjCcFlIQzACG00IKghnAcFpIQTADGE4LKQhmAMNpIQXBDGA4LaQgmAEMp4UUBDOA4bSQgmAGMJwHeF3BXU9y75gAAAAASUVORK5CYIIA'
    ]

let lastGrid

const render = () => {
    let newGrid = Array.from({ length: 6 }, () => Array.from({ length: 10 }, () => 0))
    lastGrid ??= structuredClone(newGrid)

    body.forEach(([x, y]) => newGrid[y][x] = 1)
    newGrid[applePos[1]][applePos[0]] = 2

    newGrid.forEach((row, y) => row.forEach((val, x) => { if (val !== lastGrid[y][x]) {
        const index = y * 10 + x
        fs.writeFileSync('./render/' + index + '.png', images[val], { encoding: 'base64' })
    }}))
    lastGrid = newGrid
}

let body = [[0, 0]],
    direction = 1,
    applePos = [Math.floor(Math.random() * 10), Math.floor(Math.random() * 6)]

const clear = () => Array.from({ length: 60 }).forEach((_, i) => fs.writeFileSync('./render/' + i + '.png', images[0], { encoding: 'base64' }))
const step = apple => {
    let newPos = structuredClone(body[0])
    switch (direction) {
        case 1: newPos[0] = (newPos[0] + 1) % 10; break
        case 2: newPos[1] = (newPos[1] + 1) % 6; break
        case 3: newPos[0] = (newPos[0] + 9) % 10; break
        case 4: newPos[1] = (newPos[1] + 5) % 6; break
    }
    if (body.some(([x, y]) => x == newPos[0] && y == newPos[1])) {
        clear()
        return false
    }
    body.unshift(newPos)
    if (!apple) body.pop()
    else applePos = [Math.floor(Math.random() * 10), Math.floor(Math.random() * 6)]

    setTimeout(() => {
        let nextPos = structuredClone(newPos)

        switch (direction) {
            case 1: nextPos[0] = (nextPos[0] + 1) % 10; break
            case 2: nextPos[1] = (nextPos[1] + 1) % 6; break
            case 3: nextPos[0] = (nextPos[0] + 9) % 10; break
            case 4: nextPos[1] = (nextPos[1] + 5) % 6; break
        }
        if (step([...body.slice(0, -1), nextPos].some(([x, y]) => x == applePos[0] && y == applePos[1]))) render()
    }, 2000)

    return true
}

process.stdin.setEncoding('ascii')
process.stdin.on('data', char => {
    char = char.toLowerCase().trim()
    if (['w', 'a', 's', 'd'].includes(char)) direction = ['w', 'd', 's', 'a'].indexOf(char) || 4
})

clear()
render()
step(1 == applePos[0] && 0 == applePos[1])
console.log('')
render()