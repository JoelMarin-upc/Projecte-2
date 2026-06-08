# Project 2 – CITM UPC

This repository is part of the **Project 2 course** at the  
**CITM (Center for Imaging and Multimedia Technology), Universitat Politècnica de Catalunya (UPC).**

---

# What Remains of You

This game is a turn-based RPG where players explore handcrafted levels filled with undead and enemy soldiers. Between expeditions, you can return to the refuge to upgrade gear, recover, and recruit companions for future battles.

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
- ✅ All menus are animated, they appear/disappear from screen with a transition.
- ✅ The game is completely playable with a gamepad, it is possible to navigate the menus and select the different options. The UI elements still keep their states.

**MARKETING AND DISTRIBUTION**
- ✅ Game comes on an installer. Also the game executable includes a custom icon and executable properties.
- ✅ GitHub webpage with some customization for the game. It includes the trailer embedded.
- ✅ Short trailer to sell the game and show its key features. The trailer includes actual gameplay.

**BATTLE**
- ✅ Animated battle menu
- ✅ Particle system
- ✅ Items selection and usage
- ✅ Improved UI
- ✅ Undo action button

**SCREENS**
- ✅ Animated main menu
- ✅ Animated options menu
- ✅ In/out animations for the menus
- ✅ Sounds for messages and animations

**GAMEPLAY**
- ✅ Infection system
- ✅ Darkness system inside the dungeons
- ✅ Final dungeon level
- ✅ Final boss with ending for the game
- ✅ Improved inventory management

**MORE IMPLEMENTED FEATURES**
- ✅ New NPCs to recruit
- ✅ New dialogues
- ✅ Improved enemy pathfinding
- ✅ New sound FXs
- ✅ Improved Rebel Refuge map (visuals and collisions)


---
## Known Issues

- If the party members can collide with the player, they will push the player out of scenes
- Animations go faster when there's no UI
- Sometimes enemy combat textures get replaced by ones from other enemies
- Mission journal can't be opened from inside the dungeon
- Sometimes can enter combat with dead enemies
---

## How to Play / Controls

### Gameplay Controls (Keyboard)
| Action | Key |
|:--------|:----|
| Move Up | W/UP |
| Move Left | A/LEFT |
| Move Down | S/DOWN |
| Move Right | D/RIGHT |
| Interact | E |
| Pick dialogue option | Mouse LEFT BT |
| Open inventory | TAB |
| Open mission journal | J |
| Pause game | P/ESC |
| Cap FPS | F11 |

### Debug Controls
| Action | Key |
|:--------|:----|
| God Mode + show collisions | F10 |
| Rebel refuge scene | 1 |
| Shop scene | 2 |
| Tutorial dungeon scene | 3 |
| Second dungeon scene | 4 |
| Third dungeon scene | 5 |
| Kill party NPC member | K |

[Controls image](Assets/Textures/controls.png)
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
- Dark Ambient (Free Music) - "THE LONG DARK" by Scott Buckley 🇦🇺 : https://www.youtube.com/watch?v=zwyCdIF9flw
- Dark Cinematic Background Music | Suspense Instrumental Music | Music by Argsound : https://www.youtube.com/watch?v=n0EHNKM-whk
- Sad Piano (Free Music) - "Stillstand" by @Myuu 🇺🇸 : https://www.youtube.com/watch?v=qg9xayHvhcE
- [Non Copyrighted Music] Josh Lippi & The Overtimers - St. Francis [Sad] : https://www.youtube.com/watch?v=pLEZgGeTnY8
- FilmCow Royalty Free Sound Effects Library by FilmCow : https://filmcow.itch.io/filmcow-sfx
- Footstep Leaves 2 by AardsReal : https://freesound.org/people/AardsReal/sounds/842187/
- 'Undertow' [Sombre Piano & Strings CC-BY] by Scott Buckley : https://www.youtube.com/watch?v=mGpFFxNOiII
- Cinematic Tense Trailer by Cold Cinema [No Copyright Music] / Ashes - https://www.youtube.com/watch?v=NBW-iRd1R4Q

All original assets in this project are licensed under CC BY 4.0.
