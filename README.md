# Project 2 – CITM UPC

This repository is part of the **Project 2 course** at the  
**CITM (Center for Imaging and Multimedia Technology), Universitat Politècnica de Catalunya (UPC).**

---

# What Remains of You

This game is a turn-based RPG where players explore handcrafted levels filled with undead and enemy soldiers. Between expeditions, you return to town to upgrade gear, recover, and hire companions for future battles.

The game is built in **C++** using a custom engine powered by **SDL3** for rendering and **Box2D** for physics.

---

## Contributors

- **Sofia Barja**: Technical artist, artist, designer
- **Joel Marin**: Engine programmer, mechanics programmer
- **Kirsten Neubauer**: Narrative Designer, artist
- **Clara Sanchez**: Mechanics and narrative designer, artist
- **Yin Ye**: Programmer, systems and narrative designer, artist

---

## This Release

In this release, the following features have been implemented:

**GENERAL**
- ✅Game resolution: minimum 1280x720px@60fps, support for windowed mode and fullscreen
- ✅Include GOD mode (F10)
- ✅Turn-based battle system

**SCREEN: LOGO**
- ✅Team logo
- ✅AUDIO: Sound FX characteristic for the logo

**SCREEN: TITLE**
- ✅Game title
- ✅Game SAVE/LOAD state to/from external file
- ✅AUDIO: Sound FX characteristic for the title appearing
- ✅AUDIO: Music track specific for the screen
- ✅UI: Main game menu
- ✅UI: AUDIO: Buttons/controls fx feedback sounds

**GAMEPLAY**
- ✅Implement a level with at least 3 locations to visit
- ✅Implement **2 Playable Characters**, available as the base Team Party
- ✅Implement **2 NPC Characters**
- ✅Implement at least **4 Dialogs** with branching with NPC characters
- ✅INPUTS: Keyboard inputs supported
- ✅UI: PAUSE menu available on pause
- ✅AUDIO: Music track specific for the screen
- ✅AUDIO: Music effects fade-in/fade-out, lower volume on PAUSE
  
**SCREEN: BATTLE**
- ✅Implement at least **2 Types of Enemies** with different stats
- ✅Implement turn-based combat system, players and enemies should alternate attacks
- ✅UI: Implement a minimal menu to choose next player actions

**MORE IMPLEMENTED FEATURES**
- UI: Basic game config options implemented with custom font (screen options)
- UI: OPTION: GRAPHICS: Full Screen mode selection (screen options)
- UI: OPTION: AUDIO: Music volume slider (screen options)
- UI: OPTION: AUDIO: Fx sounds volume slider (screen options)

---
## Known Issues

- The dialog box and its options are drawn on top of the pause menu when player is interacting with someone/something
- Stats don't get saved by the save system
- If a party member dies in battle, it comes back alive when exiting the dungeon
- The lever is only visual for the moment, it has no other function
- Sometimes when clicking on the intro screen, you can accidentally press buttons from the main menu

---

## How to Play / Controls

### Gameplay Controls
| Action | Key |
|:--------|:----|
| Move Up | W/UP |
| Move Left | A/LEFT |
| Move Down | S/DOWN |
| Move Right | D/RIGHT |
| Interact | E |
| Pick dialogue option | Mouse LEFT BT |

### Debug Controls
| Action | Key |
|:--------|:----|
| God Mode + show collisions | F10 |
| Rebel refuge scene | 1 |
| Shop scene | 2 |
| Tutorial dungeon scene | 3 |

---

## 📦 Installation / Execution

1. Download the latest `.zip` file from the [Releases](../../releases) section.  
2. Extract all files to a folder.  
3. Run `[WhatRemainsOfYou].exe`.  

---

## Technical Details

- **Language:** C++
- **Engine:** Custom-built engine
- **Rendering:** SDL3
- **Physics:** Box2D

---

## External Assets
- Pixel Crawler Tileset by Anokolisa: https://anokolisa.itch.io/free-pixel-art-asset-pack-topdown-tileset-rpg-16x16-sprites
- Rogue Fantasy "catacombs" Tileset by szadiard: https://szadiart.itch.io/rogue-fantasy-catacombs
- Home Interior Asset Pack - Phonky Phone: https://mologo.itch.io/home-interior-asset-pack-free
- Leander Typography by Tension Type: https://www.dafont.com/leander.font
- Dark Cinematic by NikitaKondrashev: https://pixabay.com/music/suspense-dark-cinematic-509801/
- Waltz in C (Classical, Piano, Soundtrack) by Ashot_Danielyan: https://pixabay.com/music/modern-classical-waltz-in-c-classical-piano-soundtrack-368728/
- Follows ( Dark Ambient ) by AmbientAUDIOVISION: https://pixabay.com/music/ambient-follows-dark-ambient-194926/
- Opening Old Elevator Door by ryanharding95 (Freesound): https://pixabay.com/sound-effects/film-special-effects-opening-old-elevator-door-84624/

All original assets in this project are licensed under CC BY 4.0.
