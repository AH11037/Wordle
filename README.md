# Wordle Clone
A desktop implementation of the popular Wordle game built with C++ and SFML. 

Players have 6 attempts to guess a radomly selected five-letter word. After each guess, tiles change colour to indicate whether letters are correct, present elsewhere in the word, or absent entirely.

This idea came to me whilst I was on a walk and it didn't seem too dififcult to do & here we are now.

## Screenshot
<img width="675" height="1032" alt="image" src="https://github.com/user-attachments/assets/1571f5fa-8f91-444d-87f7-be6fd535b0a4" /> <br>
As you can tell, I am extremely goated at this game

## Downloading & Running
### Clone the Repository

```bash
git clone https://github.com/USERNAME/REPOSITORY.git
cd REPOSITORY
```

### Requirements

To build the project from source, you will need:

* A C++17 compatible compiler
* SFML
* Git

### Building

Compile the project using your preferred build system or IDE.

#### Example (g++)

```bash
g++ -std=c++17 *.cpp -o Wordle \
-lsfml-graphics \
-lsfml-window \
-lsfml-system
```

### Running

#### Windows

```bash
.\Wordle.exe
```
Or double click the `exe` like a sane person.
#### Linux / macOS

```bash
./Wordle
```

### Gameplay

1. Enter a five-letter word using your keyboard.
2. Press `Enter` to submit your guess.
3. Tile colours indicate how close your guess is:

   *  $${\color{green}Green}$$ — Correct letter in the correct position
   *  $${\color{yellow}Yellow}$$ — Correct letter in the wrong position
   *  $${\color{grey}Grey}$$ — Letter is not present in the answer
4. Guess the word within six attempts to win.

### Controls

| Key          | Action                 |
| ------------ | ---------------------- |
| A-Z          | Enter letters          |
| Backspace    | Delete the last letter |
| Enter        | Submit current guess   |
| Left Control | Start a new game       |

## How it works
When the application starts:
1. A dictionary file is loaded into memory (this contains all of the words used by the program)
2. A random 5-letter word is selected as the answer
3. The game board is generated with 6 rows of letter tiles
4. Player input is processed through SFML event handling


After a guess is submitted:
 - The tile changes colour based on the letter
   - You've played Wordle before you should know this


The game continues until the player either:
 - Correctly guesses the word and wins
 - Uses all 6 attempts and loses like a bozo

## Features
 - Random word selection
 - Dictionary validation
 - Wordle-style colour feedback
 - Input validation
 - Win and loss detection
 - Restart functionality
 - Custom embedded font support
 - Graphical interface built with SFML
 - Lots of depression from realising that Wordle is difficult to beat

## Technical Details
### Dictionary Storage
Words are loaded from a text file and stored in:
 - `std::unordered_set` for fast word validation
 - `std::vector` for random word selection

### Word validation
Submitted words are checked against the loaded dictionary before being accepted

### Guess evaluation
The game uses a two-pass validation system:
1. Correct letters in correct positions are identified first
2. Remaining letters are checked for partial matches

This ensures duplicate letters are handled correctly.

## What I learned
 - Managin game state
 - Random word generation
 - Effecient dictionary lookups with hash tables
 - User input validation
 - How to make tables in markdown (⌐■_■)
 - The extents of the indomitable human spirit

## Limitations
 - Only supports five-letter words
 - The `guesses.txt` file must be present
 - No on-screen keyboard
 - No animations or transition effects
   - They need lots of maths to do and I am not bothered to do that
 - Limited visual customisation

## Future improvements
 - Add an on-screen keyboard
 - Add animations in general
 - Add difficulty settings
   - I am not really too sure how
 - Support multiple word lengths
 - Track player statistics
 - Improve UI styling
 - Add sound effects
 - Clean up the code a bit more
   - Not a bit more a LOT more this code is nasty
