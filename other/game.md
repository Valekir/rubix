# Game

After creating new game or loading from save player starts game. On the screen you can see the cube sweep as well as some information about available commands to control the game.


During the game, the user can save or load the session, as well as go to the menu. You can read about the menu here -> [Menu](menu.md)

<img src='https://github.com/Valekir/rubix/blob/master/other/game_1.gif?raw=true'/>

<img src='https://github.com/Valekir/rubix/blob/master/other/game_2.gif?raw=true'/>

### Moves notation:

#### Face rotations:
+ F (front): the face facing the solver.
+ B (back): the back face.
+ R (right): the right face.
+ L (left): the left face.
+ U (up): the upper face.
+ D (down): the face opposite to the upper face.
+ M (middle): the middle layer parallel to the R & L faces.
+ E (equator): the middle layer parallel to the U & D faces.
+ S (side): the middle layer parallel to the F & B faces.

The letter means turning a single turn, (90 degrees), clockwise, the corresponding face.\

Letter followed by an apostrophe mark (') means turning the corresponding face a single turn, (90 degrees), counter-clockwise. (ex. R' U')\

Letter followed by "2" means turning the corresponding face 2 single turns (180 degrees) (ex. R2 = R R)\

Command can be written with or without delimitters
ex. RURU = R U R U = R_U_R_U

#### Cube Rotations
These are not actual turns, and is meant to instruct a rotation of whole the cube.
There are 3 possible axes to rotate the cube on: The X, Y & Z axes.
+ x : rotating the cube on the X axis. (R & L faces remain intact)
+ y : rotating the cube on the Y axis. (U & D faces remain intact)
+ z : rotating the cube on the Z axis. (F & B faces remain intact)
