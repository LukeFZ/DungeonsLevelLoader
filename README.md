# DungeonsLevelLoader

A custom level loader for Minecraft: Dungeons. (Modified version of flat20's [dungeoneer](https://github.com/flat20/dungeoneer))

# How to use this Tool?


1. First, make sure you have placed your custom level mod .pak file in the "~mods" folder of your Minecraft: Dungeons installation.
2. Next, unzip the [Latest Release](https://github.com/LukeFZ/DungeonsLevelLoader/releases/latest) of this tool into a new folder.
If this is your first time playing custom levels, you need to also place the "Table.pak" into your "~mods" folder from step 1.

3. Place the "customlevels.json" file created by the [Dungeons Mod Merger](https://github.com/LukeFZ/DungeonsModMerger) next to the "LevelLoader.exe".

4. Start Minecraft: Dungeons. On the Main Menu, double-click the LevelLoader.exe to start it.
After a short time, a blue-ish window should appear with a list of numbers and the custom level name next to them. 
These are the numbers corresponding to each loaded level, which you need to enter after clicking on the new table in the lobby.

5. After entering the number and selecting the wanted difficulty/threat level values, press the button "Start Game!".
The game should now display the creeperwoods loading screen, and after a bit, load your selected custom level!

# Troubleshooting
  - The Level Loads infinitely:
	  This problem means that the level messed up something while creating the level itself. 
	  If you are the level dev yourself, most of these issues can be debugged by using the console command "Test.Level.GetUsedBlocks *levelName*" while using the level loader,
	  and seeing what error it produces.
  
  - Clicking on the "Start Game!" button does nothing and/or closes the menu
	  This happens if you forgot to run the levelloader.exe in step 7.
	  Just restart Dungeons and start the levelloader.exe again.
    
  - The level you wanted to play isnt in the list:
    Try remerging the mods together and copy the resulting "customlevels.json" file to the level loader.
    If it still doesnt occur, make sure you merged the right mods. 
