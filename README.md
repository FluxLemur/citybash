# CityBash
CityBash is simple [real time strategy
game](https://en.wikipedia.org/wiki/Real-time_strategy) written as a server to
facilitate the implementation and integration of AI players.

## Overview
The player controls _one_ city with assets and capabilities.

- Assets
  - Gold
  - Army
- Capabilities
  - Upgrade city
  - Increase army size
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
- Military
  - The city houses a player's army. Take city A that has 20 soldiers.
  - An attack happens in 3 steps. Let’s say city A orders 10 soldiers to attack on city B (the other 10 will remain defending the city).
    1. Nothing happens for some time _T_ab_, proportional to the distance between cities A and B. The army is moving.
    2. The 10 soliders of A's army attack the soliders that are currently in B's city. Both sides sustain losses. If the attackers succeed, they take some gold with them.
    3. After _T_ab_ more time, the remaining soliders of the original 10 return to city A. If they are carrying gold from a successful attack, the gold is added to A’s hold.
- Ending Conditions
  - The game stops when either:
    1. A player upgrades to a level 10 city
    2. 20 minutes have passed
  - Players are awarded points based on city level, gold, and army size, and then ranked by point-count.

## Game Mechanics
- Cities are placed at random on a 50x50 square world.
- For a city with Level N:
  - Income = [in development]
  - Defense multiplier = [in development]
  - Cost to upgrade to level N+1 city = [in development]
- Armies
  - [Insert javascript battle simulator]
  - Training a soldier costs 5 gold and takes 5 seconds. Soldiers can be trained asynchronously.
  - Armies move 1 distance unit/sec

## Game Client
The shell script `run_client.sh` provides a simple game client that a player
can use to connect to the game server.

NOTE: the script has a dependency on
[`rlwrap`](https://github.com/hanslub42/rlwrap), a readline wrapper for user
input. It can easily be installed with `apt-get`. If you prefer not to use it,
edit `run_client.sh` and remove it.

## Server Interface
The game runs as a server. The administrator first gives each player a unique
key which the player uses to join the game with the following command:
  - `[player key] join [city name]`

Once the game begins, players can send a selection of commands to list
information about their city and the world, as well as commands to upgrade
their city, train soldiers, and make attacks.

The server response is given below each respective player message.
  - `[player key] WORLD`
    - `[city name] [city level] [distance to city]`
    - `[city name] [city level] [distance to city]`
    - ... (newline delimited)
  - `[player key] CITY`
    - `LEVEL [current city level]`
    - `GOLD [current gold amount]`
    - `INCOME [current gold income]`
    - `ARMY [# of soldiers in city]`
  - `[player key] COSTS`
    - `UPGRADE [gold to upgrade city] [time to upgrade city]`
    - `TRAIN [gold to train soldier] [time to train soldier]`
  - `[player key] UPGRADE`
    - `UPGRADE SUCCESS` if current gold >= upgrade cost
    - `UPGRADE FAILURE [# current gold] < [gold needed]` otherwise
  - `[player key] TRAIN [# soldiers]`
    - `TRAIN [# soldiers] SUCCESS` if current gold >= cost of soldiers
    - `INVALID TRAIN. USAGE: [player key] TRAIN [# soldiers]` otherwise
  - `[player key] ATTACK [city-name] [# soldiers]`
    - `ATTACK [city-name] [# soldiers] SUCCESS` if city name valid and soldiers in [1, # soldiers in city]
    - `INVALID ATTACK. USAGE: [player key] ATTACK [other city name] [# soldiers]\n`

When the game is over, the server rejects player commands, and the
administrator can announce the results.

## Special Features in Development
- Barbarian cities
- Different types of military units

## Implementation
The engine is implemented in C++. We attempt to follow the
[Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html).

Simulations of the game mechanics are in Python (see `simulations/`). A guiding
philosophy is that decisions should be "data-informed" rather than arbitrary.
