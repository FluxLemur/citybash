# CityBash
CityBash is simeple [real time strategy game](https://en.wikipedia.org/wiki/Real-time_strategy) written as a server to facillitate the implementation and integration of AI players.

## Overview
The player controls _one_ city with assets and capabilities.

- Assets
  - Gold
  - Military
- Capabilities
  - Upgrade city
  - Increase military power
  - Attack other cities

## Gameplay
- World layout
  - Cities are placed at random in a 2D space such that distances between cities are variable.
  - Among the cities that belong to players, the world also contains some randomly placed barbarian cities.
- Gold Usage
  - Each city has a base income rate of gold at the start of the game.
  - A player can spend gold to increase their:
    * City level
    * Military power
- City Level
  - The player's city starts at level 1.
  - Higher level cities yield higher gold income, and have a larger defense multiplier.
  - Level 10 is the highest.
- Miltary
  - The city houses a player's army. Take city A that has 20 soldiers.
  - An attack happens in 3 steps. Let’s say city A orders 10 soliders to attack on city B (the other 10 will remain defending the city).
    1. Nothing happens for some time _T_ab_, proportional to the distance between cities A and B. The army is moving.
    2. The 10 soliders of A's army attack the soliders that are currently in B's city. Both sides sustain losses. If the attackers succeed, they take some gold with them.
    3. After _T_ab_ more time, the remaining soliders of the original 10 return to city A. If they are carrying gold from a successful attack, the gold is added to A’s hold.
- Ending Conditions
  - The game stops when either:
    1. A player upgrades to a level 10 city
    2. 20 minutes have passed
  - Players are awarded points based on city level, gold, and army size, and then ranked by point-count.

## Game Mechanics
- Cities are placed at random on square. Distance between cities can wrap araound the edges of the square (forming a torus).
  - Side length 100 units. Mean distance between 2 cities is ~45, with a standard deviation of ~21.
- For a city with Level N:
  - Income = N gold/sec
  - Defense multiplier = log(1 + N) (log base 2)
  - Cost to upgrade to level N+1 city = 10 * 2^N gold, and takes 20 * N seconds
- Armies
  - [Insert javascript battle simulator]
  - Training a soldier costs 10 gold and takes 30 seconds. Soldiers can be trained asynchronously.
  - Armies move 1 distance unit/sec

## Server Interface
The game runs as a server. Before play has begun, players connect to the server and give their city name. An adminitrator begins the game once all players are connected and ready. Once the game begins, the server sends a message to all players indicating the game has started. Players can send a selection of commands to list information about their city and the world, as well as commands to upgrade their city, train soldiers, and make attacks. Eventually, the server notifies all players that the game is over.

The server response is given below each respective player message. Note that every command must begin with the player's hash, given to them when they first join the game.
  - `[city hash] WORLD`
    - `[city name] [city level] [distance to city]`
    - `[city name] [city level] [distance to city]`
    - ... (newline delimited)
  - `[city hash] CITY`
    - `LEVEL [current city level]`
    - `GOLD [current gold amount]`
    - `ARMY [# of soliders in city]`
  - `[city hash] COSTS`
    - `UPGRADE COST [gold to upgrade city]`
    - `TRAINING COST [gold to train soldier]`
  - `[city hash] UPGRADE`
    - `UPGRADE SUCCESS` if current gold >= update cost
    - `UPGRADE FAILURE` otherwise
  - `[city hash] TRAIN [# soldiers]`
    - `TRAIN [# soldiers] SUCCESS` if current gold >= cost of soldiers
    - `INVALID TRAIN. USAGE: [city hash] TRAIN [# soldiers]` otherwise
  - `[city hash] ATTACK [city-name] [# soldiers]`
    - `ATTACK [city-name] [# soldiers] SUCCESS` if city name valid and soldiers in [1, # soldiers in city]
    - `INVALID ATTACK. USAGE: [city hash] ATTACK [other city name] [# soldiers]\n`

## Special Features in Development
- Barbarians
  - Can players team up against one barbarian city?
- Different types of military units

## Implementation
The engine is implemented in C++. We attempt to follow the
[Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html).

Simulations of the game mechanics are in Python. A guiding philosophy is that
decisions should be "data-informed" rather than arbitrary.
