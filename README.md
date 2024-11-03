# Minesweeper-Solver
Minesweeper Solver made in C++ that displays mines and also flags the mines.

### The tools I used:
* [IDA Pro Free](https://www.hex-rays.com/products/ida/support/download.shtml)
* [Cheat Engine](https://github.com/cheat-engine/cheat-engine)
* [Binary Ninja](https://binary.ninja/)
* Any IDE of your choice

## How to use the Exploit Script
1. Firstly you compile the binary using a c++ compiler such as clang or g++.
2. Run the Minesweeper Game.
3. Run the compiled exe binary file and press the face emoji once to see all the mines on the board.
4. Each time you start a new game, run the script again this will replace all the mines with flags.
5. Click anywhere but the mines and win!

## The Steps I took to create this
* I used cheat engine to see a live memory map of how the game works. This allowed me to understand which values are which on the minefield such as a mine being 0x8A and a flag being 0x0e.
* I used IDA Pro and cheat engine to figure out some of the game logic and noticed the mines are placed at 0x010036FA each time the game was started/restarted. Using this I injected my own shellcode using c++ to show the mines on the grid. (This address was gotten by tracking the value 10 on an easy map in cheat engine and seeing where it was overwriting when the game was restarted.)
*  So now that we know where the mines are, I decided to find out the memory addresses the minesweeper grid is generated in using cheat engine. That's how I got the values 0x1005340 (start), x10056A0 (end). by seeing a border of the number 10 around and if I clicked a tile with for example 1 in it it would show 0x41 in memory and all the tiles next to it.
*  The final step is now to flag every flag automatically, I did this by using the fact we show and marked all the mines as 0x8A on the grid so I read the grid from memory and used a loop to change every mine to a flag in the grid to automatically win the game if a non-mine tile was clicked.
  
## Pictures of Outputs / The Game
### Showing the Mines
![image](https://github.com/user-attachments/assets/8f8b50dd-63a7-4b61-a16a-cb1c16540826)

### Terminal Output to show that the mines have been replaced with flags
![image](https://github.com/user-attachments/assets/004aa58e-7783-4450-92fc-0da32896fdbb)

### Winning the Game
![image](https://github.com/user-attachments/assets/e1bda855-9db2-4455-83a8-d92a983a66d0)
