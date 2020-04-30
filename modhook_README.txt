1. Open Explorer/etc to your game directory, typically this is of one:

     C:\Program Files\Steam\steamapps\common\Deus Ex Human Revolution Director's Cut
     C:\Program Files (x86)\Steam\steamapps\common\Deus Ex Human Revolution Director's Cut

   If you changed it, you should know where it is.


2. Delete or backup DFEngine.dll in the game directory.
   If you want the in-game ads, rename it RealDFEngine.dll and
   it will load it in.


3. Extract DFEngine.dll to the game directory. This is the Mod Hook.


4. Extract the mods directory to the game directory.


5. If you've done everything correctly, you should have something like this:

     ..\Steam\steamapps\common\Deus Ex Human Revolution Director's Cut\DFEngine.dll
     ..\Steam\steamapps\common\Deus Ex Human Revolution Director's Cut\RealDFEngine.dll (if you chose to do this)
     ..\Steam\steamapps\common\Deus Ex Human Revolution Director's Cut\mods\gibbed_debug.000
     ..\Steam\steamapps\common\Deus Ex Human Revolution Director's Cut\mods\gibbed_mapselector.000


6. You're done. Start the game already!


7. Enjoy!


8. If the game is patched, Mod Hook will likely stop working. It will
   gracefully disable itself in this situation. You will need to look
   for an updated version when this happens.


Rick aka gibbed
http://gib.me/

PS: Yes, using the Mod Hook means you can drop arbitrary *.000 archives in the
    mods directory to get them loaded into the game. Go hog wild! File
    overriding within the mods directory is probably alphabetized by archive
    file name, I have not tested that, but files within archives in the mods
    directory will always override files in the BIGFILE.000 archive.
