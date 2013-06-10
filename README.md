GLtris
======

Goals:
------

A simple tetris game with:

- fixed resolution

- OpenGL graphics/SDL input and windowing

- keyboard input

- text rendering with Freetype

- scoring

- preview of next block in side menu

Notes:

* Collision of blocks will be done with a map of block locations in an array, everytime the block about to be placed moves down, the new location will be checked first.

* In order to conform to the Tetris guidelines, resolution is 480x480 with the side menu filling the right portion of the screen and the game filling the left the grid is 10x22 with 2 of the 22 hidden at the top

* Each block is therefore 24x24

TODO:
--------------------------------

* Rotation - DONE!
* Wall kicks - DONE!
* Row clearance - DONE!
* Game reset on failure (no more room) - DONE!.
* Levels/speed changes - DONE!
* "Non-locking soft drop"
* "Hold" function
* scoring - DONE!
* Side menu
* background?