# CityBash
CityBash is simple [real time strategy
game](https://en.wikipedia.org/wiki/Real-time_strategy) written as a server to
facilitate the implementation and integration of AI players.

1. [Overview](#overview)
2. [Gameplay](#example2)
3. [Game Mechanics](#game-mechanics)
4. [Server Interface](#server-interface)
5. [Running the Game Client](#running-the-game-client)
6. [Running the Server](#running-the-server)
7. [Implementation](#implementation)

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
  - Among the cities that belong to players, the world _may_ also contains some randomly placed NPC cities.
- Gold Usage
  - Each city has a base income rate of gold at the start of the game.
  - A player can spend gold to increase their:
    * City level
    * Military power
- City Level
  - The player's city starts at level 1.
  - Higher level cities yield higher gold income, and have a larger defense multiplier.
  - Level 5 is the highest.
- Military
  - The city houses a player's army. Take city A that has 20 soldiers.
  - An attack happens in 3 steps. Let’s say city A orders 10 soldiers to attack on city B (the other 10 will remain defending the city).
    1. Nothing happens for some time `T_ab`, proportional to the distance between cities A and B. The army is moving.
    2. The 10 soliders of A's army attack the soliders that are currently in B's city. Both sides sustain losses. If the attackers succeed, they take some gold with them. Some of B's gold is safely hidden in it's _cache_. NOTE, gold is automatically stored in a city's cache.
    3. After `T_ab` more time, the remaining soliders of the original 10 return to city A. If they are carrying gold from a successful attack, the gold is added to A’s hold.
- Ending Conditions
  - The game stops when either:
    1. A player upgrades to a level 5 city
    2. 20 minutes have passed
  - Players are awarded points based on city level, gold, and army size, and then ranked by point-count.

## Game Mechanics
- Cities are placed at random on a 20x20 square world.
- Armies
  - See `simulations/battle_sim.py`
  - Training a soldier costs 5 gold and takes 5 seconds. Soldiers can be trained asynchronously.
  - Armies move 1 distance unit/sec

City Level | Income (gold/s) | Defense Multiplier | Upgrade Cost (gold) | Cache size (gold) |
---------- | --------------- | ------------------ | ------------ | ---------- |
1 | 1 | 1.2 | 30  | 5   |
2 | 2 | 1.3 | 90  | 15  |
3 | 3 | 1.4 | 270 | 45  |
4 | 5 | 1.5 | 810 | 135 |

- Note that as soon as a city becomes level 5, the game is over.

## Server Interface
The game runs as a server. The administrator first gives each player a unique
key which the player uses to join the game with the following command:
  - `[player key] join [city name]`

Once the game begins, players can send a selection of commands to list
information about their city and the world, as well as commands to upgrade
their city, train soldiers, and make attacks.

The valid player commands are (_not_ case sensitive):
- `WORLD` or `W`
- `CITY` or `C`
- `SHORTCITY` or `SC`
- `COSTS` or `CO`
- `UPGRADE` or `U`
- `TRAIN #` or `T #`
  - `TRAIN max` uses all current gold to train soldiers
- `ATTACK city #` or `A city #`

The server response is given below each respective player message.
  - `[player key] WORLD`
    - `[city name] ([city id]) [city level] [distance to city]`
    - ... (newline delimited)
  - `[player key] CITY`
    - `LEVEL [current city level]`
    - `GOLD [current gold amount]`
    - `INCOME [current gold income]`
    - `ARMY [# of soldiers in city]`
    - `[list of notifcations]` of the form `* [#] sec ago: [contents]`, where `[contents]` is of the form:
      - `[city name] attacked [#]/[#] took [#] gold, [#]/[#] defenders remained` These are for attacks on your city
      - `Attacked [city name] [#]/[#] took [#] gold{, [#]/[#] defenders remained}` These are for attacks by your city on other cities. If you have at least 1 soldier remaining, the part of the message enclosed in `{}` is included
  - `[player key] SHORTCITY`
    - `LEVEL [#], GOLD [#], INCOME [#], ARMY [#]`
  - `[player key] COSTS`
    - `UPGRADE [gold to upgrade city] gold, [time to upgrade city] sec`
    - `TRAIN [gold to train soldier] gold, [time to train soldier] sec`
  - `[player key] UPGRADE`
    - `UPGRADE SUCCESS` if current gold >= upgrade cost
    - `UPGRADE FAILURE [gold needed] > [current gold]` otherwise
  - `[player key] TRAIN [# soldiers]`
    - `TRAIN [# soldiers] SUCCESS` if current gold >= cost of soldiers
    - `INVALID TRAIN. VALID: [player key] TRAIN [# soldiers]` if wrong format or # soldiers is
    not a positive integer
    - `TRAIN FAILURE Cannot train 0 soldiers` corner case
    - `TRAIN FAILURE [# soldiers] COSTS [gold needed] > [current gold]` otherwise
  - `[player key] ATTACK [city-name] [# soldiers]`
    - `INVALID ATTACK. VALID: [player key] ATTACK [other city name] [# soldiers]`
       if syntax is invalid (not 4 tokens or # soldiers is not a positive integer)
    - `ATTACK FAILURE Cannot attack with 0 soldiers`, or
    - `ATTACK FAILURE Cannot attack your own city`, or
    - `ATTACK FAILURE No city [city-name]`, or
    - `ATTACK FAILURE [# soldiers] > [# soldiers in city]`, or
    - `ATTACK SUCCESS [city-name] [# soldiers]`

After the round-trip time for the attack, a notification is added to your city:
    - `ATTACK {WIN, LOSE} [#]/[#] return with [#] gold, [city-name] remaining: [#]/[#]`
       if none of the previous cases apply. If 0 soldiers return, the part of
       the message including and after the `,` is ommitted.

When the game is over, the server rejects player commands, and the
administrator can announce the results.

## Running the Game Client
The shell script `run_client.sh` provides a simple game client that a player
can use to connect to the game server.

NOTE: the script has a dependency on
[`rlwrap`](https://github.com/hanslub42/rlwrap), a readline wrapper for user
input. It can easily be installed with `apt-get`. If you prefer not to use it,
edit `run_client.sh` and remove it.

## Running the Server
In order to build and run the server locally, you must first ensure you have the
dependencies:
- [clang++](http://clang.llvm.org/)
- [libevent2](http://libevent.org/)

The following steps will get a server up and running:
1. `make` creates the `citybash` binary - this is the server.
2. `./citybash 123` starts the server with an admin key of `123` (you can use whatever you like)
3. `cd clients/ && ./admin_client.sh` This will connect you to a the citybash server (which should running on localhost at port 12345). Send admin commands by prepending the admin key `123`.
4. See `src/game_state.h` for valid admin commands.

## Implementation
The engine is implemented in C++. We attempt to follow the
[Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html).

Simulations of the game mechanics are in Python (see `simulations/`). A guiding
philosophy is that decisions should be "data-informed" rather than arbitrary.
