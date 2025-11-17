# STWEAKS

A comprehensive gameplay mod that overhauls various parts of the game.
TLDR: A mod full of random gameplay related functions i thought were neat

## Mod Description:

### 1. Damage ranges:
Skyrim has flat damage, every hit deals the same damage minus some rounding errors. Stweaks changes that and adds some randomness to the damage output.

### 2. Follower damage scaling
The more followers you have, the less damage you and they deal. This can be offset a bit with a high speech skill level.

### 3. Actor Fade
If enemies stand in low light and are far enough away, they will get hidden from sight making stealth archery a tad harder

### 4. Sneak jump limit
Lowers your jump height if you're sneaking

### 5. Mass-based jump height
The higher your mass actor value, the lower your jump height

### 6. Sneak stamina drain
Sneaking adds a spell that drains stamina. On top of that, there's also a sneak archer nerf by draining your stamina while sneaking and standing still if you have a bow out.

### 7. Quest item nerf
Quest items have no weight for the player. In certain mod setups, this can give you an unfair advantage. This makes quest items deal 0.5% of their original damage. Exceptions can be set in a json file.

### 8. Resistance changes
Magic can be rough early game in some setups. This lowers your enemies' magic resistance by 20% until you are level 5.

### 9. Cast stamina cost
Casting spells cost stamina and attempting to cast without stamina will fail the cast

### 10. Attack stamina cost
Normal attacks cost stamina, based on the weapon weight. This can be adjusted with perks

### 11. Interrupt casting on hit
Power Attacks can interrupt spells in vanilla. With this setting, normal attacks, arrows, bolts and non-concentration spells can do that as well.

### 12. Stamina regeneration changes
In vanilla, your stamina regeneration is tied to the max stamina you have. This makes late game stamina management basically non-existent cause the regen outheals the stamina cost. this unties it from the max stamina and uses a flat base for the regen

### 13. Magicka regeneration changes
same as above but for  magicka

### 14. One-shot protection
prevent one-shots and always stay alive with at least 20% health when being hit while having full health. affects player and enemies

### 15. Damage caps
clamps damage to 5 times the weapon damage for normal attacks and 10 times for power attacks. also clamps spells to 5 times their base magnitude. 
this setting and the above do not affect sneak attacks.

### 16. Skill-based cast speed
The higher your skill at the school of magic, the faster you cast the spells. it starts with half speed and goes up to 3 times speed by default

### 17. Level up low-level enemies
Skyrim allows level 3 enemies to spawn when the player is level 50. This is boring imo, so, the mod sets the level of any enemy up to 10 levels below the player. Every type of enemy.

### 18. Automatic attribute growth
Attributes level upon usage. if you are between 40 and 95% of a stat, you will gain xp for that stat over time. if you have 100xp, it will level the stat by 1. Works for Health/Magicka/Stamina/Carry Weight

### 19. Mass changes with equipment
the heavier the equipment you/npcs wear, the higher your mass stat becomes. Used for different calcs in the mod but not mandatory

### 20. Jump stamina cost
Jumping costs stamina. the higher your mass, the higher the cost

### 21. Tall grass enhances sneaking
If you stand on a LandTexture defined in a json as tall grass, you get a perk that enhances sneak by 60%

### 22. Ethereal damage prevention fix
I noticed that the hit that breaks the ethereal effect, sometimes deals damage in vanilla. I fixed this oversight and now, the hit that breaks ethereal always deals 0 damage as the description of the effect makes it sound like it should

### 23. Curses
Disease like curses you can catch when being bit by any spell in the game. 

### 24. Stamina and magicka base regen values
In vanilla, Stamina and Magicka regen is based on your max value which makes high level attribute consumption often lower than the regen. 
now attribute regen is based on a flat base.

Everything can be toggled separately and there are some value adjustments like: The new base for the regen, that upper and lower limits for the damage ranges, toggling changing curses, curse chance, stamina cost modifiers and a few more.

## Building the mod:

### Requirements
* [XMake](https://xmake.io) [2.8.2+]
* C++23 Compiler (MSVC, Clang-CL)

## Getting Started
```bat
git clone --recurse-submodules https://github.com/Styyx1/Stweaks-Rewrite Stweaks
cd Stweaks
```

### Build
To build the project, run the following command:
```bat
xmake build
```

> ***Note:*** *This will generate a `build/windows/` directory in the **project's root directory** with the build output.*

### Build Output (Optional)
If you want to redirect the build output, set one of or both of the following environment variables:

- Path to a Skyrim install folder: `XSE_TES5_GAME_PATH`

- Path to a Mod Manager mods folder: `XSE_TES5_MODS_PATH`

### Project Generation (Optional)
If you want to generate a Visual Studio project, run the following command:
```bat
xmake project -k vsxmake
```

> ***Note:*** *This will generate a `vsxmakeXXXX/` directory in the **project's root directory** using the latest version of Visual Studio installed on the system.*

### Upgrading Packages (Optional)
If you want to upgrade the project's dependencies, run the following commands:
```bat
xmake repo --update
xmake require --upgrade
```
