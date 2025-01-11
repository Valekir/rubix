# Rubix
## Introduction
Rubix is a C++ implementation of a Rubik’s Cube simulator. This project aims to provide a comprehensive tool for Rubik’s Cube enthusiasts, puzzle solvers, and anyone interested in the algorithmic aspects of solving complex puzzles. 

## Demo
<img src='https://github.com/Valekir/rubix/blob/master/other/demo.gif'/>

## Installation
````
git clone https://github.com/Valekir/rubix
cd rubix
mkdir -p build
cd build
cmake ..
make
./main
````

## Features

This game also implements some additional features such as:

1. Ability to save the game and continue from the same position;

2. Customizable cube scramble - you can choose number of moves for scramble (difficulty);

3. Different cube size - from 2 to 5;

4. Customizable cube colors - you can pick any 8-bit color for any side (even 6 same)

5. Menu navigation with arrows;

6. Timer (display of the elapsed time it take you to solve cube);


## Requirements
- C++17 or higher
- Unix
- Ncurses
- Git