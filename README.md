# STWEAKS

A comprehensive gameplay mod that overhauls various parts of the game.
TLDR: A mod full of random gameplay related functions i thought were neat

## Mod Description:

### Damage ranges:
Skyrim has flat damage, every hit deals the same damage minus some rounding errors. Stweaks changes that and adds some randomness to the damage output.

## Follower damage scaling
The more followers you have, the less damage you and they deal. This can be offset a bit with a high speech skill level.

## Sneak jump limit
Lowers your jump height if you're sneaking

## Mass-based jump height
The higher your mass actor value, the lower your jump height

## Sneak stamina drain
Sneaking adds a spell that drains stamina. On top of that, there's also a sneak archer nerf by draining your stamina while sneaking and standing still if you have a bow out.

## Quest item nerf
Quest items have no weight for the player. In certain mod setups, this can give you an unfair advantage. This makes quest items deal 0.5% of their original damage. Exceptions can be set in a json file.

## Resistance changes
Magic can be rough early game in some setups. This lowers your enemies' magic resistance by 20% until you are level 5.

## Cast stamina cost
Casting spells cost stamina and attempting to cast without stamina will fail the cast

## Attack stamina cost
Normal attacks cost stamina, based on the weapon weight. This can be adjusted with perks

## Interrupt casting on hit
Power Attacks can interrupt spells in vanilla. With this setting, normal attacks, arrows, bolts and non-concentration spells can do that as well.

## Stamina regeneration changes
In vanilla, your stamina regeneration is tied to the max stamina you have. This makes late game stamina management basically non-existent cause the regen outheals the stamina cost. this unties it from the max stamina and uses a flat base for the regen

## Magicka regeneration changes
same as above but for  magicka

## One-shot protection
prevent one-shots and always stay alive with at least 20% health when being hit while having full health. affects player and enemies

## Damage caps
clamps damage to 5 times the weapon damage for normal attacks and 10 times for power attacks. also clamps spells to 5 times their base magnitude. 
this setting and the above do not affect sneak attacks.

Skill-based cast speed

Level up low-level enemies

Automatic attribute growth

Mass changes with equipment

Jump stamina cost

Tall grass enhances sneaking

Ethereal damage prevention fix

Curses

Curse chance

Curse swap cooldown

Magic stamina cost divider

Stamina and magicka base regen values


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
