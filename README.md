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
- ✅UI buttons with multimple states (normal, hover, pressed and disabled)
- ✅Custom cursor
- ✅Sound effects


**SCREEN: LOGO**
- ✅Animated logo in/out

**SCREEN: TITLE**
- ✅Title animation with easings
- ✅Music track
- ✅Gear equipment

**GAMEPLAY**
- ✅Ability system
- ✅2 Dungeon maps with puzzles
- ✅Collectable items
- ✅Quests
- ✅Journal to check quests
- ✅Inventory system
- ✅Shop
- ✅Basic economic system
  
**SCREEN: BATTLE**
- ✅Sound FX
- ✅Turn-based combat system, players and enemies can alternate attacks
- ✅Minimal menu to choose next player actions
- ✅Music track
- ✅New layout
- ✅Charcters and enemies illustrations

**ABILITIES**
- ✅Unlock new abilities (stances) through progression
- ✅Minimal menu system (characters stats, inventory items) 

**MORE IMPLEMENTED FEATURES**
- ✅Win/Lose screens with respective music tracks
- ✅Pixel art with animations of the characters and enemies
- ✅Dialogue boxes
- ✅Saving of stats, settings, items and mission/NPCs states


---
## Known Issues

- The combat scene doesn't clean up properly after fleeing
- Pathfinding still causes a lot of fps drops
- Dialogue elements are drawn on top of the Pause Menu
- Some textures fail to load sometimes in the battle scene
- If the party members can collide with the player, they will push the player out of scenes
- After dying to an enemy the combat scene doesn't seem to clean up
- The initial dialogue always repeats even after saving and pressing continue
- Animations go faster when there's no UI

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
| Kill party NPC member | K |

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

All original assets in this project are licensed under CC BY 4.0.
