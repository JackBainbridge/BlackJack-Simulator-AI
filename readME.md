# BlackJack Simulator AI ♤
This project explores the implementation of AI in Blackjack using C++. It has the ability to be played manually or via an AI using QLearning as its reinforcement learning. 

Begin from scratch and train 250,000 hands for a new QTable stored in SQLite database, or use the sample one provided.

### Technologies Used 🖥️
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

### Hard Reset (if you encounter build issues): 🔁
Clean the build environment to ensure CMake actually sees your changes.
```
cd /mnt/c/Development/Projects/C++_BlackJack/build
rm -rf *
cmake ..
make
```

### Running the Project 🚀
```
./BlackJackAI [Train-AI-or-Not] [Play-Manual-or-Not] [Display-GUI]
```

**Arguments:**

- **Train-AI:** ```0``` = Train new model, ```1``` = Load existing model
- **Play-Mode:** ```0``` = Manual play, ```1``` = AI plays
- **Display-GUI:** ```0``` = Console only, ```1``` = GUI display

**Examples:**
```
./BlackjackAI 0 0 0    # Train AI, you play manually, console only
./BlackjackAI 1 1 0    # Load AI, AI plays, console only
./BlackjackAI 0 0 1    # Train AI, you play manually, with GUI
./BlackjackAI 1 1 1    # Load AI, AI plays, with GUI
```
### How the AI works 🧠

#### Q-Learning State Definition: 
Each game state is represented by three values:

- Player's current hand total (e.g., 12-21)
- Dealer's visible card value (e.g., 2-11)
- Whether the player has an Ace that can be counted as 1 (soft hand flag)

#### Actions:
    0 = Stand (Stop drawing)
    1 = Hit (Draw another card)

#### Q-Table: 
A table that stores values for each (state, action) pair. Higher values indicate better decisions for that situation. For example, the AI learns that hitting when you have 12 and the dealer shows a 6 is generally good, so that Q-value is high.

#### Training Process:
The Silent Trainer runs 250,000 simulated hands using epsilon-greedy strategy:

- **Exploitation (80%)**: AI picks the best-known action
- **Exploration (20%)**: AI tries random actions to discover new strategies

#### After Training:
Once trained, the Q-Table is saved to a SQLite database (blackjack_brain.db). When you play, the AI looks up each state in the Q-Table and uses the learned knowledge to make decisions — no randomness, just playing optimally.

### Game Features ✨
- Interactive GUI with card images
- Manual player input (H for hit, S for stand)
- AI decision-making with trained Q-Learning model
- Blackjack detection (instant win on 21)
- Win/Loss/Tie determination
- Play multiple rounds in one session

### Game Screen
![Game Sample](/assets/docs/game_sample.png)

