# ASCII Ping Pong in C

This project implements a Ping Pong game in C, featuring an ASCII-based graphical interface. The game mechanics and program functionality are managed using signals and child processes, with a temporary file (`tempfile`) used for handling inter-process communication.

> **Note**: This project is still under development and is not yet complete.

## Features

- **ASCII Interface**: The game is displayed in the terminal using ASCII graphics, providing a simple yet engaging gameplay experience.
- **Signal Handling**: The game logic is implemented using signals to synchronize the actions of the processes.
- **Child Processes**: The game runs across multiple processes, enabling a smooth simulation of the ball and paddles' movements.
- **Temporary File**: A temporary file is used to store and manage information between processes, ensuring effective communication.

## Requirements

- **Operating System**: The game is developed for Linux environments.
- **Compiler**: GCC (GNU Compiler Collection).
- **Tools**: Standard C libraries for handling signals and processes.
