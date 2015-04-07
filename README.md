# Ixthil
Timeline:  Summer 2012

A 2D vertical shooter with influences from Galaga and the Touhou series.

Overview
--------

Ixthil is a 2D vertical shooter mixing a style of Galaga and the Touhou series.  At its core, players control a ship flying through space, shooting at enemies and avoiding attacks.  In Ixthil, the enemies can be deadly, but the player is equipped with six different weapons that can be upgraded over time.  In addition, the levels are fixed with a specific ending.  Secret routes and powerups for the more dedicated players are available.




Gameplay
--------

Controls:
Z - Shoot
X - Bomb
A/S - Switch weapon
C - Use a Blank
D - Activate Energy
ESC - Pause / back

In Ixthil, the shoot button can be continuously held--repeatedly tapping the button will not increase fire rate.  To balance this, when the player is not shooting, nearby items left behind by enemies will be pulled towards the player.

Items which can be dropped included colored upgrades for each of the six colored weapons--a color upgrades its matching weapon.  In addition, some white or "Blank" upgrades appear.  These can be collected and later applied
to the weapon of the player's choice as needed.  Bombs will also appear, which clear out all bullets and damages enemies near the player.  Health upgrades exist--both health and shield can be upgraded and recovered by
items.  When taking damage, it will first be removed from the shield (the blue bar), which recharges over time.  If the shield is low (white), then damage will go to the health (red bar) instead.  A brief invincibility occurs when the player is hit.

When hit, the player's currently selected weapon will go down by a level. The weapon levels (strengths) are shown in a weapon grid at the bottom right.  If a weapon's level going down to zero, then the game will automatically switch to a usable weapon.  The default red attack is guaranteed to always have at least one level of strength.  When obtaining or using an upgrade, the corresponding weapon (current if using a Blank) will go up by half a level--represented by a smaller square in the weapon grid.  A weapon needs to go up by a full level before its effects change.

Occasionally enemies will drop Energy--flashing colored dots.  This accumulates in a third bar on the interface showing how much Energy the player has.  The player can activate their Energy, which will continually drain it until it runs out or the player deactivates it.  While Energy is being used, all player weapons will be increased by a level, and all weapons are usable.

Enemies will shoot with a fixed color throughout their life.  If an enemy is killed with this color or the oppposite color (see below for what is considered opposite), then a Dark enemy will appear.  This enemy is typically stronger than the other enemy, which results in a risk if the player causes the Dark enemy to appear.  The risk however does lead to the player gaining Energy upon killing the enemy.  This in fact is the only way to collect Energy.  Killing a Dark enemy does not cause another one to appear.

For the purposes of Ixthil, opposite color pairings are considered to be Red and Cyan, Yellow and Blue, and Green and Purple.  For an in-game reminder, the pause screen provides a summary of the color wheel used, with opposite colors on opposite sides of the wheel.

If the player collects enough energy, the energy bar will switch from white to an animated rainbow.  Upon reaching certain points in the game, if the player has enough energy, a portal will appear which the player can choose to fly into.  Entering a portal takes the player to Subspace, where only Dark enemies appear and a special boss enemy appears.  If the player can survive and leave Subspace, they will obtain a Crystal which grants a special effect.  Each Crystal grants different powers, and it is left to the player to discover these powers.  Dying in Subspace does
not lead to the player returning to normal space--a death in Subspace is a death overall.

Depending on how many Crystals are obtained, the player can reach extra bonus levels at the end of the game.  These will occur naturally in the gameplay.  Once the player reaches the end based on the Crystals collected, the credits will roll.



Historical Notes on Ixthil
--------------

The original Ixthil was my first experience learning Python, in summer 2009.  I decided to learn Pygame at the same time and just make a simple 2D shooter engine to get a basic grasp of the fundamentals.  Over time, I came up with a few gameplay ideas that I rather liked and decided to try to make the simple project into a full game.

Since this was my first Python project, much of the game was organized and hacked together as needed.  One of the biggest defects in this is that all vector math was done by hand--rather than calling say a dot product fuction such as dot(x, y), I instead manually typed in x[0] * x[0] + x[1] * x[1] every time.

Eventually I ported the game over to C++ to give to some family members as Christmas gifts and distribute to friends who did not have Python, and gain some general performance improvements.  Due to the Christmas deadline, it was
a fairly blind translation, and many of the hacks from Python were copied over.  As a result, the code is a horrible mess, but it contains everything that was ever implemented in any version of Ixthil.

The hacked together C++ version exists as /ixthil_old on GitHub.  Work was performed a couple years later on completely redoing the Ixthil engine, trying to make a "perfect" or ideal project according to software engineering
guidelines.  Eventually work was stopped as graduate school began, but can be seen in the /ixthil project on GitHub.  Much of the refactoring was also done in the hopes of reusing much of the core engine in later games.



Technical Details
---------------

The following details apply to the current Ixthil project.

The background effect was achieved by using a cellular automata.  Each cell has two data values--which color it is, and how bright it is.  Each frame bullets will update the nearby area to full brightness and the bullet's color.  During the Subspace sequences, calculations on brightness are not performed; otherwise, the cells fade over time by averaging their brightness with their neighbors.

Some overhead management was included to keep track of which areas of the cellular automaton were updated at each frame.  This worked as a sort of "lazy" computation method, which ultimately helped improve the framerate (60fps to 30fps).  Additionally, compared to the 12,228 cells in the automaton which need to be updated, this method reduced the number of updates to 500-4,500 per frame (http://ixthil.tumblr.com/post/23662257632/today-was-mostly-getting-the-background-and ).

Enemies are now soft coded.  Levels are stored in text files in the levels/ subdirectory.  The LevelSelector class manages switching between levels, while LevelReader parses files.

Overall system management has been decoupled from the rest of the game.  The new System class handles top-level management of interaction with SDL.  Here we call any initalization/deinitialization functions.  Additionally, input is kept track of here, with InputListener classes for other classes to interface with the system.  This abstracts away core game components needing to know about button to key mappings or even what type of input is being used (keyboard vs. joystick).  Frameworks are included for later plans of adding joystick support.



Goals
---------

- Finish the menu system (including pause and option menus)
- Joystick input
- Support for "dark" enemies and energy
- Demo mode on title screen
- Tutorial for new players
- Clean up the interface
- Finish the storyline with secret endings
- Credits "level"
- Replace art and music assets with original artwork and music



Credits
---------

One of the biggest influences on Ixthil's development was the Touhou series of vertical shooters by Zun.  These games had a strong focus on bullet dodging.  I wanted to put more emphasis on the player's ability to aim and control how fast enemies died.  The weapon system was based on the one from Cave Story, where the current player's weapon would weaken when the player was hit.  Some obvious modifications were made to adapt to the gameplay pace of Ixthil.

The idea of Crystals and Subspace were based on the Chaos Emeralds from the Sonic the Hedgehog games on the Sega Genesis.  In addition, the ending music from Sonic the Hedgehog 2 was used as the credits music for much of Ixthil's history, with intentions to eventually replace it with music that did not break any copyrights.

Much of the art came from Galaga, Perfect Cherry Blossom of the Touhou series, and Cave Story.  The rest of it was drawn (poorly) by Ramona Seay in either Microsoft Paint or GIMP.

The different colored weapons and upgrades dropping from enemies came from Zero Wing.  This was previously used in a vertical shooter as part of a high school project, which a couple of the enemies and one of the bosses also come from.

Most of the music is free music from the Newgrounds Audio Portal.  Since this is a non-profit project, all music can be used freely.
