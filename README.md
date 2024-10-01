TODO: Rename this repository

# Survival Arena something 2

## Gameplay description
Single player and co-op game.

Top down perspective, isometric view.
Level size depends but will probably be up to 10 screen sizes in width and height

Player is able to melee attack with a sword or shoot with a gun. Both weapons are aimed. Player turns with the "look" controller (mouse, joystick, touchscreen, etc.)

Enemies have simple follow and attack AI. 
They spawn randomly and infinetely until player loses. Every time newly spawned enemies get atk and def boosts. 
When defeated, enemies dissapear and sometimes drop an item.
Each enemy type drops a different item.
Items: hp potion, attack boost up, defence boost up. 

When player loses, they get to save their score in the leaderboard 

Extra features in the future:
Sword and board gameplay option
Online leaderboard
Touchscreen support
Android release

# GAMEPLAY LOOP:
enemies drop weapons randomly. every N enemies defeated or so.

at the end of every wave player has a chance to store items for storage

after player's defeat, player gets sent back some levels or waves, lose inventory but gets to keep what's in storage

every time levels are kind of randomized (enemies, building placement)

buildings placed in level can be used for strategy or cover (for example, a wall to get away from enemies for a moment or a canon)

player will have abilities that affect enemy status, movement, attention, etc.


## Notes

ICombatCharacter
GetCombatComponent();
GetHealthComponent();
GetWeaponComponent();

Controllers
Controller handles input and translates it to PlayerCharacter. Then PlayerCharacter could be
reference controllers instead of characters!

Kameros valdymas
Su desiniu joysticku valdoma kamera aplink zaideja
Aukstis nesivaldo arba valdosi atskirai

UE_LOG
naujos macro funkcijos ue logui.
LOG_ERROR(Text, Args)
LOG_ALL(Text, Args)
etc

Gamemode Couch co-op


