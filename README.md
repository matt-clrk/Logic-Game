# Summary Of Program
This program is designed after the logic game Lights Out. The user will start with a start up screen that will display the instructions for the game and to start playing the user must press "s" to start. Once the user presses "s" a random configuration of lights will be either turned on or off. The program uses a checkExist() function and toggleTouchingTiles() function to then turn off the light that was clicked and any adjacent tiles. Through the processUserInput() function the program will detect if a square is being hovered over and will which square is clicked on. To win the game the user must turn off all lights on the board and if the user does so a win screen will be displayed. Added features include the a tally of the number of clicks to turn lights off and total time it takes to win the game.
# Languages used
This project involves extensive use of C++ and graphics libraries.
# Work Not Authored by myself
This project was a collaborative project between JJ Baker and myself. I authored the initialization of the board, logic for turning adjacent lights off, and logic for randomization of lights. I also implemented all of the base game functions. The CMakeLists and starter graphics code were authored by University of Vermont Computer Science professor, Lisa Dion.
# Installations necessary 
Necessary installations include: OpenGL graphics libraries and the installation of shaders and fonts.

