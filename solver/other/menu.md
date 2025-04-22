# Menu
The program greets the user with the main menu to select further user actions:
- Start new game
- Continue from save
- Change settings
- Exit the program

### *New game*
This button starts new game, creating cube in initial state (solved). You can read more about it here -> [Game](game.md)

### *Load game*
Here you can load previous games or delete unnecessory ones

### *Settings*
This menu allows you to change the cube however you want. You can:
+ Choose cube size (from 2 to 5)
+ Change difficulty (number of steps in scromble) from 1 to 20
+ Toggle cube scaling: size of sweep adjusts to size of console window and scales to the maximum size. By default each cell on cube is 1x1
+ Toggle timer: if on it will start automatically when player does first move after scramble. It stops when the cube is solved and shows time spent to solve
+ Change colors of each side. Colors are 8-bit, here is table that shows every color:
 <img src='https://user-images.githubusercontent.com/995050/47952855-ecb12480-df75-11e8-89d4-ac26c50e80b9.png?raw=true'>

All the settings are stored in the 'saves/game.config'

<img src='https://github.com/Valekir/rubix/blob/master/other/menu.gif?raw=true' />
