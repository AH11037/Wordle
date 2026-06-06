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
#include <iostream>
#include <vector>

constexpr float width = 672;
constexpr float height = 804;
constexpr float gap = 12;
constexpr float sqSize = 120;
constexpr float step = sqSize + gap;
sf::Font font;

class Word {
  struct letter {
    Word &bigBoss;

    sf::RectangleShape box;
    sf::Text character;

    sf::Color green = sf::Color(108, 169, 101);
    sf::Color gray = sf::Color(120, 124, 127);
    sf::Color yellow = sf::Color(200, 182, 83);

    letter(char c, float column, Word &parentWord) : bigBoss(parentWord) {

      box.setFillColor(sf::Color::Black);
      box.setOutlineThickness(3.f);
      box.setOutlineColor(sf::Color::White);
      box.setSize(sf::Vector2f(sqSize, sqSize));

      box.setPosition(
          sf::Vector2f((column * step) + gap, (bigBoss.row * step) + gap));

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
  int currentLetter = 0;
  std::vector<letter> letters;

public:
  Word(int row) {
    this->row = row;

    for (int i = 0; i < 5; i++)
      letters.emplace_back(' ', i, *this);
  }

  void render(sf::RenderWindow &window) {
    for (auto &letter : letters) {
      window.draw(letter.box);
      window.draw(letter.character);
    }
  }

  void input(char c) {
    if (currentLetter < 5) {
      letters[currentLetter].changeLetter(c);
      currentLetter++;
    } else
      std::cout << "NOPE";
  }

  void removeLetter() {
    if (currentLetter > 0) {
      currentLetter--;
      letters[currentLetter].changeLetter(' ');
    }
  }
};

int main(int, char **) {
  int currentGuess = 0;
  font.loadFromMemory(franklin_normal_700_1_, 51376);

  std::vector<Word> words;
  for (int i = 0; i <= 5; i++) {
    words.push_back(Word(i));
  }
  sf::RenderWindow window(sf::VideoMode({static_cast<unsigned int>(width),
                                         static_cast<unsigned int>(height)}),
                          "Wordle", sf::Style::Titlebar | sf::Style::Close);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {

      if (event.type == sf::Event::Closed)
        window.close();

      else if (event.type == sf::Event::TextEntered) {
        if (event.text.unicode >= 'a' && event.text.unicode <= 'z') {
          words[currentGuess].input(
              toupper(static_cast<char>(event.text.unicode)));
        }
      }

      else if (event.type == sf::Event::KeyPressed) {

        if (event.key.code == sf::Keyboard::Enter) {
          if (currentGuess < words.size() - 1)
            currentGuess++;
        }

        else if (event.key.code == sf::Keyboard::BackSpace) {
          words[currentGuess].removeLetter();
        }
      }
    }

    window.clear();

    for (auto &w : words) {
      w.render(window);
    }

    window.display();
  }
}
