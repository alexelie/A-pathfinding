# A-pathfinding
visual implementation of A* using SFML (C++)

Developpment done with SFML 2.4.2

HOW TO USE:

1. Build and install SFML. I used cmake for that process

2. For codeblock, open and set the project(.cbp) linker settings for debug version: sfml-window-d, sfml-graphics-d, sfml-system-d and set search directories for linker/compiler to the appropriate folder(lib for linker and incldude for compiler) inside your sfml installation folder.

3. Copy/paste sfml-graphics-d-2.dll, sfml-window-d-2.dll, sfml-system-d-2.dll in bin/debug folder of the codeblock pathfinding project

3. How to play:
   - first click sets starting point
   - second click sets end point
   - next clicks draw walls
   - press ENTER to start pathfinding
