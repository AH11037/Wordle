#include "franklin-normal-700 (1).hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Window.hpp>
#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

constexpr float width = 672;
constexpr float height = 1004;
constexpr float gap = 12;
constexpr float sqSize = 120;
constexpr float step = sqSize + gap;
constexpr int maxGuess = 6;

std::string checkerFile("../../../guesses.txt");
sf::Font font;
std::string answer = "COINS";
sf::Text message;
bool win = false;
bool gameover = false;
std::unordered_set<std::string> dictionaryWords;
std::vector<std::string> wordList;

auto centerText = [](sf::Text &text, float y) {
  sf::FloatRect bounds = text.getLocalBounds();

  text.setOrigin(bounds.left + bounds.width / 2.f,
                 bounds.top + bounds.height / 2.f);

  text.setPosition(width / 2.f, y);
};

void error(const std::string e) {
  message.setFillColor(sf::Color::Red);
  message.setString(e);
  centerText(message, 950);
}

std::string randomise() {
  if (wordList.empty())
    return "COINS";

  return wordList[std::rand() % wordList.size()];
}

class Word {
  struct letter {
    int row;

    sf::RectangleShape box;
    sf::Text character;

    sf::Color green = sf::Color(108, 169, 101);
    sf::Color gray = sf::Color(120, 124, 127);
    sf::Color yellow = sf::Color(200, 182, 83);

    letter(char c, float column, int row) : row(row) {

      box.setFillColor(sf::Color::Black);
      box.setOutlineThickness(3.f);
      box.setOutlineColor(sf::Color::White);
      box.setSize(sf::Vector2f(sqSize, sqSize));

      box.setPosition(sf::Vector2f((column * step) + gap, (row * step) + gap));

      character.setCharacterSize(sqSize - 40);
      character.setFillColor(sf::Color::White);
      character.setFont(font);
      changeLetter(c);

      sf::FloatRect bounds = character.getLocalBounds();
      character.setOrigin(bounds.left + bounds.width / 2.f,
                          bounds.top + bounds.height / 2.f);

      character.setPosition(box.getPosition().x + sqSize / 2.f,
                            box.getPosition().y + sqSize / 2.f);
    }

    void changeLetter(char character) {
      this->character.setString(character);
      recenter();
    }

    void invalid() {
      box.setOutlineColor(gray);
      box.setFillColor(gray);
    }

    void valid() {
      box.setOutlineColor(green);
      box.setFillColor(green);
    }

    void kinda() {
      box.setOutlineColor(yellow);
      box.setFillColor(yellow);
    }

    void recenter() {
      sf::FloatRect bounds = character.getLocalBounds();

      character.setOrigin(bounds.left + bounds.width / 2.f,
                          bounds.top + bounds.height / 2.f);

      character.setPosition(box.getPosition().x + sqSize / 2.f,
                            box.getPosition().y + sqSize / 2.f);
    }
  };

  int row;
  std::vector<letter> letters;

public:
  std::string theWord = "";
  Word(int row) : row(row) {

    for (int i = 0; i < 5; i++)
      letters.emplace_back(' ', i, row);
  }

  void render(sf::RenderWindow &window) const {
    for (auto &letter : letters) {
      window.draw(letter.box);
      window.draw(letter.character);
    }
  }

  void input(char c) {
    if (theWord.length() < 5) {
      letters[theWord.length()].changeLetter(c);
      theWord += c;
      std::cout << theWord << "\n";
    }
  }

  void removeLetter() {
    if (theWord.length() > 0) {
      theWord.pop_back();
      letters[theWord.length()].changeLetter(' ');
    }
  }

bool submit() {

    bool found =
        dictionaryWords.find(theWord) != dictionaryWords.end();

    if (!found) {
        error("Invalid word!");

        for (int i = 0; i < 5; i++)
            removeLetter();

        return false;
    }

    message.setFillColor(sf::Color::White);
    message.setString("Type a word to play!");
    centerText(message, 950);

    bool usedAnswer[5] = {false};

    for (int i = 0; i < 5; i++) {
        if (theWord[i] == answer[i]) {
            letters[i].valid();
            usedAnswer[i] = true;
        }
    }

    for (int i = 0; i < 5; i++) {

        if (theWord[i] == answer[i])
            continue;

        bool matched = false;

        for (int j = 0; j < 5; j++) {

            if (!usedAnswer[j] && theWord[i] == answer[j]) {
                letters[i].kinda();
                usedAnswer[j] = true;
                matched = true;
                break;
            }
        }

        if (!matched)
            letters[i].invalid();
    }

    if (theWord == answer)
        win = true;

    return true;
}

  void reset() {
    for (auto &l : letters) {
      l.box.setOutlineColor(sf::Color::White);
      l.box.setFillColor(sf::Color::Black);
      l.changeLetter(' ');
    }
    theWord.clear();
  }
};

void createBoard(std::vector<Word> &words) {
  words.clear();
  for (int i = 0; i < maxGuess; i++)
    words.emplace_back(i);
}

void loadDictionary() {
  std::ifstream file(checkerFile);

  if (!file) {
    std::cout << "Words file is missing!\n";
    std::exit(1);
  }

  std::string line;

  while (std::getline(file, line)) {
    for (auto &c : line)
      c = std::toupper(c);
    dictionaryWords.insert(line);
    wordList.push_back(line);
  }
}

int main(int, char **) {
  std::srand(std::time(nullptr));

  if (!font.loadFromMemory(franklin_normal_700_1_, 51376)) {
    std::cout << "Failed to load font\n";
    return 1;
  }

  loadDictionary();

  size_t currentGuess = 0;

  std::vector<Word> words;
  sf::Text title;

  sf::RenderWindow window(sf::VideoMode({static_cast<unsigned int>(width),
                                         static_cast<unsigned int>(height)}),
                          "Wordle", sf::Style::Titlebar | sf::Style::Close);

  answer = randomise();

  createBoard(words);

  title.setCharacterSize(sqSize - 40);
  title.setFillColor(sf::Color::White);
  title.setFont(font);
  title.setString("WORDLE");
  centerText(title, 850);

  message.setCharacterSize(40);
  message.setFillColor(sf::Color::White);
  message.setFont(font);
  message.setString("Type a word to play!");
  centerText(message, 950);

  while (window.isOpen()) {

    if (win) {
      message.setFillColor(sf::Color::Green);
      message.setString("You win!");
      centerText(message, 950);
    }

    sf::Event event;
    while (window.pollEvent(event)) {

      if (event.type == sf::Event::Closed)
        window.close();

      else if (event.type == sf::Event::TextEntered && !(win || gameover) &&
               currentGuess < maxGuess) {
        if (std::isalpha(static_cast<char>(event.text.unicode))) {
          words[currentGuess].input(
              toupper(static_cast<char>(event.text.unicode)));
        }
      }

      else if (event.type == sf::Event::KeyPressed) {

        if (event.key.code == sf::Keyboard::Enter && !(win || gameover) &&
            currentGuess < maxGuess) {
          if (words[currentGuess].theWord.length() == 5) {
            if (words[currentGuess].submit()) {
              currentGuess++;
            }
          }
        }

        else if (event.key.code == sf::Keyboard::BackSpace &&
                 !(win || gameover) && currentGuess < maxGuess) {
          words[currentGuess].removeLetter();
        }

        else if (event.key.code == sf::Keyboard::LControl) {
          win = false;
          gameover = false;
          currentGuess = 0;

          createBoard(words);

          answer = randomise();
          message.setFillColor(sf::Color::White);
          message.setString("Type a word to play!");
          centerText(message, 950);
        }
      }
    }

    if (!win && currentGuess >= maxGuess) {
      gameover = true;
      message.setFillColor(sf::Color::Red);
      message.setString("You lose! The answer was " + answer + "!");
      centerText(message, 950);
    }

    window.clear();

    for (auto &w : words) {
      w.render(window);
    }
    window.draw(title);
    window.draw(message);
    window.display();
  }
}