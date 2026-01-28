# BlackJack Simulator AI ♤

Each player and dealer in the blackjack with AI game has a hand in playing cards. The AI algorithm that controls the game decides what to do based on the cards in each player's hand. The blackjack with AI project explores the implementation of AI in Blackjack using C++. The Monte Carlo method is a well-known technique of learning by simulated trial and error, while the Q-Learning algorithm is a form of reinforcement learning.

### Technologies Required: 
- C++ programming language
- Object Oriented Programming (OOPS)
- Data Structures & Algorithms
- Database Management Systems
- OpenCV
- SQL
- Reinforcement Learning Algorithms: Q Learning, and Machine Learning.
- Graphical User Interface (GUI)

### Building the Project

- From the /build directory, execute the CMakeLists.txt file at the root of the project via ```cmake ..```.

This should generate an output similar to the following and other files required in the /build directory:
```
-- The C compiler identification is GNU 13.3.0
-- The CXX compiler identification is GNU 13.3.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/cc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Found OpenCV: /usr (found version "4.6.0") 
-- Found PkgConfig: /usr/bin/pkg-config (found version "1.8.1") 
-- Checking for module 'sqlite3'
--   Found sqlite3, version 3.45.1
-- Configuring done (2.3s)
-- Generating done (0.1s)
-- Build files have been written to: /mnt/c/Development/Projects/C++_BlackJack/build
```

- Once CMakeLists has been executed, run ```make``` from the /build directory to build and executable BlackjackAI file.

- Once built, we can execute it.

```
/Projects/C++_BlackJack/build$ make
[ 50%] Building CXX object CMakeFiles/BlackjackAI.dir/src/main.cpp.o
[100%] Linking CXX executable BlackjackAI
[100%] Built target BlackjackAI

/Projects/C++_BlackJack/build$ ./BlackjackAI 
--- Blackjack AI Initializing ---
SQLite Version: 3.45.1
Environment check passed!
```