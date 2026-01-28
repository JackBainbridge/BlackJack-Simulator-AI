# BlackJack Simulator AI ♤

Each player and dealer in the blackjack with AI game has a hand in playing cards. The AI algorithm that controls the game decides what to do based on the cards in each player's hand. The blackjack with AI project explores the implementation of AI in Blackjack using C++. The Monte Carlo method is a well-known technique of learning by simulated trial and error, while the Q-Learning algorithm is a form of reinforcement learning.

### Technologies Required 🖥️
- C++ programming language
- Object Oriented Programming (OOPS)
- Data Structures & Algorithms
- Database Management Systems
- OpenCV
- SQL
- Reinforcement Learning Algorithms: Q Learning, and Machine Learning.
- Graphical User Interface (GUI)

### Building the Project 🛠️

For **Release** build (optimized, no debug symbols)
```
From the /build directory:
cmake -DCMAKE_BUILD_TYPE=Release ..
```

For **Debug** build (optimized, no debug symbols)
```
From the /build directory:
cmake -DCMAKE_BUILD_TYPE=Debug ..
```

### Hard reset if build issues 🔁
Clean the build environment to ensure CMake actually sees your changes.
```
cd /mnt/c/Development/Projects/C++_BlackJack/build
rm -rf *
cmake ..
make
```

### Running the Project 🚀
Once built, we can execute it:

```
/Projects/C++_BlackJack/build$ ./BlackjackAI 
--- Blackjack AI Initializing ---
SQLite Version: 3.45.1
Environment check passed!
```

### CommandLine Arguments
./BlackJackAI [Train-AI-or-Not] [Play-Manual-or-Not]

**Train-AI-or-Not**: 0 = Train, 1 = Load.
**Play-Manual-or-Not**: 0 = Manual (You Play), 1 = AI (AI Plays)

Example to play game, train before game and play manually: 
```./BlackJackAI 0 0```

Example to play game, load from existing db file and AI will  play: 
```./BlackJackAI 1 1```

### Q-Learning Implementation 
**State Definition:** 
Each game state is represented by three values:

    Player's current hand total (e.g., 12-21)
    Dealer's visible card value (e.g., 2-11)
    Whether the player has an Ace that can be counted as 1 (soft hand flag)

Actions: The AI can take two actions:

    Hit (action = 1): Draw another card
    Stand (action = 0): Stop drawing

**Q-Table**: A table that stores values for each (state, action) pair. Higher values indicate better decisions for that situation. For example, the AI learns that hitting when you have 12 and the dealer shows a 6 is generally good, so that Q-value is high.

**Training Process**
Silent Trainer (runSilentTrainer()) runs 250,000 simulated hands where:

- The AI plays hands against itself without human input
- For each action taken, the Q-value is updated based on the outcome (win/loss)
- The **epsilon-greedy** strategy balances exploration vs exploitation:
    
    **Exploitation**: Most of the time, the AI picks the action with the highest Q-value (plays optimally)
    
    **Exploration**: Sometimes (epsilon %), it picks a random action to discover new strategies

**After Training**
Once trained, the Q-Table is saved to a SQLite database (blackjack_brain.db). When you play, the AI looks up each state in the Q-Table and uses the learned knowledge to make decisions — no randomness, just playing optimally.