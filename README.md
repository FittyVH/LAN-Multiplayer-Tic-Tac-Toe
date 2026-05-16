# LAN Multiplayer Tic-Tac-Toe

A multiplayer Tic-Tac-Toe game built in C++ using WinSock TCP networking.

This project uses a client-server architecture where players can connect over the same WiFi/LAN network and play turn-based Tic-Tac-Toe directly through the terminal.

---

# Features
* Multiplayer over LAN/WiFi
* Client-server networking architecture
* TCP socket communication using WinSock
* Real-time board synchronization
* Turn-based gameplay system
* Win and draw detection
* Input validation
* Shared game-state packet serialization
* Dynamic server IP connection
* Separate client and server executables

# Tech Stack
* C++
* WinSock2
* Visual Studio
* TCP Networking

# How It Works

## Server
The server:
* Hosts the game session
* Waits for 2 players to connect
* Maintains the authoritative game state
* Validates moves
* Detects wins/draws
* Synchronizes the board between clients

## Client
The client:
* Connects to the host using IP address
* Receives synchronized game state packets
* Sends player moves to server
* Displays updated board in terminal

---

# Networking Architecture
The game uses:
* TCP sockets for reliable communication
* Shared binary packet serialization
* Centralized server authority model

A shared `GameState` struct is sent between server and clients to avoid TCP stream desynchronization issues.

```cpp
struct GameState
{
    char board[9];
    int currentTurn;
    bool gameOver;
};
```

---

# Building the Project

## Requirements
* Windows
* Visual Studio
* WinSock2

## Steps
1. Open solution in Visual Studio
2. Set configuration to `Release`
3. Build solution

Generated executables:

```txt
host-server.exe
tic-tac-toe.exe
```

---

# How To Play

## Host

1. Run:

```txt
host-server.exe
```

2. Server displays local IP address

3. Share IP with players

---

## Players

1. Run:

```txt
tic-tac-toe.exe
```

2. Enter host IP address

3. Start playing

---

# Example Gameplay

```txt
X | O | X
--+---+--
4 | O | 6
--+---+--
7 | 8 | X
```

---

# Future Improvements
I would like to add
* GUI version using SFML/SDL
* Chat system
* Spectator mode
* Minimax AI opponent
* More game logic like keeping track of score

---

# Author

Built by F Vh.

