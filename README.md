# CityBash
CityBash is simple [real time strategy
game](https://en.wikipedia.org/wiki/Real-time_strategy) implemented as a
stand-alone server to facilitate integration of AI players.

1. [Overview](#overview)
2. [Gameplay](#gameplay)
3. [Game Mechanics](#game-mechanics)
4. [Server Interface](#server-interface)
5. [Running the Game Client](#running-the-game-client)
6. [Running the Server](#running-the-server)
7. [Implementation](#implementation)

## Overview
CityBash has no front end. The player sends commands to a game server running
on a certain host and port, and is in control of _one_ city with some assets
and capabilities.

- Assets
  - Gold
  - Army
- Capabilities
  - Upgrade city
  - Increase army size
  - Attack other cities

## Gameplay
- Gold Usage
  - Each city has a base income of gold at the start of the game.
  - A player can spend gold to:
    * Increase city level
    * Train soldiers
- City Level
  - The player's city starts at level 1.
  - Higher level cities yield higher gold income, and have a larger defense multiplier.
  - Level 5 is the highest.
- Military
  - The city houses a player's army.
  - Consider a scenario below in which City A start with 20 soldiers.
  - An attack happens in 3 steps. Let’s say city A orders 10 soldiers to attack city B (the other 10 will remain defending the city).
    1. Nothing happens for 5 seconds. The army is moving from A to B.
    2. The 10 soliders of A's army attack the soliders that are currently in B's city. Both sides sustain losses. If the attackers succeed, they take some gold with them (at most 3 gold for each soldier). Some of B's gold is safely hidden in it's _cache_ (gold is automatically stored in a city's cache).
    3. After 5 more seconds, the remaining soliders of the original 10 return to city A. If they are carrying gold from a successful attack, the gold is added to A’s hold.
  - Run `simulations/battle_sim.py` for some examples.
- Ending Conditions
  - The game stops when either:
    1. A player upgrades to a level 5 city
    2. The admin terminates after some amount of time (say, 10 minutes)

## Game Mechanics
- Armies
  - Training a soldier costs 5 gold and takes 5 seconds. Soldiers are trained asynchronously.
  - Soldiers can carry at most 3 gold each

City Level | Income (gold/s) | Defense Multiplier | Upgrade Cost (gold, time) | Cache size (gold) |
---------- | --------------- | ------------------ | ------------ | ---------- |
1 | 1 | 1.1  | 30, 20  | 5   |
2 | 2 | 1.15 | 90, 30  | 15  |
3 | 3 | 1.2  | 270, 40 | 45  |
4 | 5 | 1.25 | 810, 50 | 135 |

- Note that as soon as a city becomes level 5, the game is over.

## Server Interface
The game runs as a server. The administrator first gives each player a unique
key which the player uses to join the game with the following command:
  - `[player key] join [city_name]` (note that `city_name` cannot have spaces)

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
  - `city` can be either a city name or a city id

The server response is given below each respective player message.
  - `[player key] WORLD`
    - `WORLD`
    - `[city name] ([city id]) [city level]`
    - ... (newline delimited)
  - `[player key] CITY`
    - `CITY [city name]`
    - `LEVEL [current city level]`
    - `GOLD [current gold amount]`
    - `INCOME [current gold income]`
    - `ARMY [# of soldiers in city]`
    - `[list of notifcations]` with each item of the form `* [#] sec ago: [contents]`. The `[contents]` can be one of two options:
      - `[city name] attacked [#]/[#] took [#] gold, [#]/[#] defenders remained` These are for attacks on your city
      - `attacked [city name] [#]/[#] took [#] gold{, [#]/[#] defenders remained}` These are for attacks by your city on other cities. If you have at least 1 soldier remaining, the part of the message enclosed in `{}` is included
  - `[player key] SHORTCITY`
    - `SHORTCITY [name] LEVEL [#], GOLD [#], INCOME [#], ARMY [#]`
  - `[player key] COSTS`
    - `COSTS`
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
    - `ATTACK SENT TO [city-name] [# soldiers]`

After the round-trip time for the attack, a notification will appear in your
`CITY` response (as above). If you have many new notifications, they will all
be displayed. However, if there are no new notifications, only the 3 most recent
notifications will be displayed.

When the game is over, the server rejects player commands, and the
administrator can announce the results.

## Running the Game Client
The shell script `run_client.sh` provides a simple game client that a player
can use to connect to the game server.

NOTE: the script has a dependency on
[`rlwrap`](https://github.com/hanslub42/rlwrap), a readline wrapper for user
input. It can easily be installed with `apt-get` or `brew`. If you prefer not to
use it, edit `run_client.sh` and remove it.

### Example Usage
Join the game (ask admin for the server ip and your player key):
- `./run_client.sh server_ip 12345 player_key cool_city`

In a separate terminal, once the game starts:
- `clients/stat_client.py host_ip 12345 player_key`

Train some units:
- `t 2` (train 2 units)

(5 seconds later you will see your army increase by 2 units).

Take a look at the output of `stat_client` and pick a city to attack. For convenience, use the city id, the number in parentheses next to the city name.
- `a 1 2` (attack city id 1 with 2 soldiers)

In 10 seconds, you'll see a notification pop up in `stat_client` with the battle details. (The defender will see their battle notification after only 5 seconds).

## Running the Server
In order to build and run the server locally, you must first ensure you have the
dependencies:
- [clang++](http://clang.llvm.org/)
- [libevent2](http://libevent.org/)

The following steps will get a server up and running:

1. `make` creates the `citybash` binary - this is the server.
2. `./citybash 123` starts the server with an admin key of `123` (you can use whatever you like)
3. `cd clients/ && ./admin_client.sh` This will connect you to a the citybash server (which should running on localhost at port 12345). Send admin commands by prepending the admin key `123`.
4. See `src/game_state.h` for valid admin commands (or send some random string
and the server will respond with the valid commands).

## Implementation
The engine is implemented in C++. I attempt to follow the
[Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html).

Simulations of the game mechanics are in Python (see `simulations/`). A guiding
philosophy is that decisions should be "data-informed" rather than arbitrary.
