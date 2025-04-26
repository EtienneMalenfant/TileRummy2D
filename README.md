![GPLv3 License](https://img.shields.io/badge/License-GPLv3-blue.svg)

# Tile Rummy 2D

This is a project I made in college as a final integrative project for my program.
It is a 2D game made with SFML in C++ and it's played against the computer.

Here is a preview of what it look like :
![game screenshot](assets/game_screenshot.png)

It is all in French for the moments, but if I see some interest from the general public I will translate the UI and comments in the source code.

I made this public repo so I can show my project to others, as well as to offer a way of installation for people interested in trying the game.

## Installation instructions

*Soon...*


### Compiling instruction

**Installing compiling tool and required libraries**

Arch systems
```
sudo pacman -S base-devel cmake git libxrandr freetype2 mesa libxcursor libudev0-shim
```

Debian systems
```
sudo apt install build-essential cmake git libxrandr-dev libfreetype6-dev libgl1-mesa-dev libxcursor-dev libudev-dev
```

**Compiling**

1. Clone this git repo `git clone https://github.com/EtienneMalenfant/TileRummy2D`
2. Cd into it `cd TileRummy2D`
3. Run the install script `./install.sh`

## Game rules

Rummy2D is a tile-based game inspired by classic Rummy mechanics. The goal is to form valid **sets** and **runs** using numbered tiles.

Vocabulary :
- A set or a run is a **meld**
- In my code, a **run** is a **sequence**

**Basic Rules**

- A set consists of three or four tiles of the same number but in different colors.
  - Example: 🔵7, 🔴7, 🟡7 (valid set)

- A run consists of three or more consecutive numbers of the same color.
  - Example: 🔵5, 🔵6, 🔵7 (valid run)

- Each player starts with 14 tiles and takes turns adding to the board.

- The first time a player play some tiles, he must place for a total of at least 30 points.
  - In this first turn, he must only place new melds (sets or runs) from his hand. He can't add any tiles to any existing meld placed by other players.

- Players can rearrange existing tiles on the board to create new valid combinations.

- The first player to place all their tiles wins.

For more details on general Rummy-style tile games, see the [wikipedia](https://en.wikipedia.org/wiki/Rummikub).


## License
This project is licensed under the GNU General Public License v3.0 (GPL-3.0).
You are free to use, modify, and distribute this software as long as any derivative work is also licensed under GPL-3.0.
