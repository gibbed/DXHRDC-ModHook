# Gibbed Mod Hook for DXHRDC*
*\* Deus Ex: Human Revolution - Director's Cut, because the title is stupid long.*

Modernized version of my original ModHook for DXHRDC, released circa 2013.

This adds functionality to the game so that all `*.000` files in the `mods` subdirectory in the game directory are loaded, so we don't have to touch the original archives at all.

## Compatibility

* 2.0.0.0, Steam, original release
* 2.0.0.0, Steam, patch #1
* 2.0.0.0, GOG

## Install

1. (Optional) Make a backup copy of the original `DFEngine.dll`.
1. Simply copy and replace the original `DFEngine.dll` in the game directory with the one from ModHook.

### DFEngine.dll?

This is the code responsible for in-game advertising, as seen on billboards and such. If you'd like to keep this "feature" working, put a copy of the original `DFEngine.dll` in the game directory named `RealDFEngine.dll`.
