#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <windows.h>
#include <set>
#include <cmath>
#include <algorithm>
using namespace sf;
using namespace std;

// ==================== Функция для растягивания фона на весь экран ====================
void setFullScreenBackground(Sprite& sprite, const RenderWindow& window) {
    if (sprite.getTexture() != nullptr) {
        Vector2u textureSize = sprite.getTexture()->getSize();
        Vector2u windowSize = window.getSize();

        if (textureSize.x > 0 && textureSize.y > 0) {
            float scaleX = float(windowSize.x) / float(textureSize.x);
            float scaleY = float(windowSize.y) / float(textureSize.y);
            sprite.setScale(scaleX, scaleY);
            sprite.setPosition(0, 0);
        }
    }
}

// ==================== СТРУКТУРЫ ДЛЯ КРОССВОРДА ====================

struct GridCell {
    char letter;
    bool isPartOfWord;
    int wordId;
    bool isFound;

    GridCell() : letter(' '), isPartOfWord(false), wordId(-1), isFound(false) {}
};

struct CrosswordWord {
    string word;
    int startRow;
    int startCol;
    bool isHorizontal;
    bool isFound;

    CrosswordWord() : startRow(0), startCol(0), isHorizontal(true), isFound(false) {}
    CrosswordWord(string w, int r, int c, bool horiz) : word(w), startRow(r), startCol(c), isHorizontal(horiz), isFound(false) {}
};

class CrosswordGrid {
private:
    vector<vector<GridCell>> grid;
    vector<CrosswordWord> words;
    int rows, cols;
    Font* font;

public:
    CrosswordGrid(int r, int c, Font* f) : rows(r), cols(c), font(f) {
        grid.resize(rows, vector<GridCell>(cols));
    }

    bool addWord(const CrosswordWord& newWord) {
        int r = newWord.startRow;
        int c = newWord.startCol;
        string word = newWord.word;
        bool horiz = newWord.isHorizontal;

        if (horiz && c + word.length() > cols) return false;
        if (!horiz && r + word.length() > rows) return false;

        for (size_t i = 0; i < word.length(); i++) {
            int currentRow = horiz ? r : r + i;
            int currentCol = horiz ? c + i : c;
            if (grid[currentRow][currentCol].isPartOfWord) {
                if (grid[currentRow][currentCol].letter != word[i]) return false;
            }
        }

        words.push_back(newWord);
        int wordId = words.size() - 1;

        for (size_t i = 0; i < word.length(); i++) {
            int currentRow = horiz ? r : r + i;
            int currentCol = horiz ? c + i : c;
            grid[currentRow][currentCol].letter = word[i];
            grid[currentRow][currentCol].isPartOfWord = true;
            grid[currentRow][currentCol].wordId = wordId;
        }
        return true;
    }

    bool findAndMarkWord(const string& foundWord) {
        for (auto& word : words) {
            if (!word.isFound && word.word == foundWord) {
                word.isFound = true;
                for (int i = 0; i < word.word.length(); i++) {
                    int row = word.isHorizontal ? word.startRow : word.startRow + i;
                    int col = word.isHorizontal ? word.startCol + i : word.startCol;
                    grid[row][col].isFound = true;
                }
                return true;
            }
        }
        return false;
    }

    bool isComplete() {
        for (const auto& word : words) {
            if (!word.isFound) return false;
        }
        return true;
    }

    void draw(RenderWindow& window, float startX, float startY, float cellSize) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                float x = startX + j * cellSize;
                float y = startY + i * cellSize;
                RectangleShape cell(Vector2f(cellSize - 2, cellSize - 2));
                cell.setPosition(x + 1, y + 1);

                if (grid[i][j].isPartOfWord) {
                    // Клетка принадлежит слову
                    if (grid[i][j].isFound) {
                        // Слово найдено - показываем зеленую клетку с буквой
                        cell.setFillColor(Color(100, 200, 100, 200));
                        cell.setOutlineThickness(2);
                        cell.setOutlineColor(Color(100, 100, 100));
                        window.draw(cell);

                        Text letterText;
                        letterText.setFont(*font);
                        letterText.setString(string(1, grid[i][j].letter));
                        letterText.setCharacterSize(cellSize * 0.5);
                        letterText.setFillColor(Color::White);
                        letterText.setStyle(Text::Bold);
                        FloatRect textRect = letterText.getLocalBounds();
                        letterText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                        letterText.setPosition(x + cellSize / 2, y + cellSize / 2);
                        window.draw(letterText);
                    }
                    else {
                        // Слово не найдено - показываем светлую клетку с вопросительным знаком
                        cell.setFillColor(Color(240, 240, 240, 200));
                        cell.setOutlineThickness(2);
                        cell.setOutlineColor(Color(100, 100, 100));
                        window.draw(cell);

                        Text questionMark;
                        questionMark.setFont(*font);
                        questionMark.setString("?");
                        questionMark.setCharacterSize(cellSize * 0.4);
                        questionMark.setFillColor(Color(150, 150, 150));
                        questionMark.setStyle(Text::Bold);
                        FloatRect qRect = questionMark.getLocalBounds();
                        questionMark.setOrigin(qRect.left + qRect.width / 2.0f, qRect.top + qRect.height / 2.0f);
                        questionMark.setPosition(x + cellSize / 2, y + cellSize / 2);
                        window.draw(questionMark);
                    }
                }
                else {
                    // Пустая клетка (не принадлежит слову) - темная
                    cell.setFillColor(Color(50, 50, 50, 100));
                    cell.setOutlineThickness(1);
                    cell.setOutlineColor(Color(150, 150, 150));
                    window.draw(cell);
                }
            }
        }

        // Рисуем границы сетки
        RectangleShape border(Vector2f(cols * cellSize, rows * cellSize));
        border.setPosition(startX, startY);
        border.setFillColor(Color::Transparent);
        border.setOutlineThickness(3);
        border.setOutlineColor(Color::White);
        window.draw(border);
    }

    int getWordCount() const { return words.size(); }
    int getFoundWordCount() const {
        int count = 0;
        for (const auto& word : words) if (word.isFound) count++;
        return count;
    }
};

// ==================== Класс для кнопки ====================
class Button {
public:
    RectangleShape shape;
    Text text;
    bool isHovered;
    Vector2f originalSize;
    Vector2f originalPosition;
    float pressAnimation;
    bool isAnimating;
    float targetScale;
    float currentScale;

    Button(const std::string& label, const Font& font, unsigned int fontSize,
        Vector2f position, Vector2f size, Color normalColor, Color hoverColor)
        : isHovered(false), originalSize(size), originalPosition(position),
        normalColor(normalColor), hoverColor(hoverColor), pressAnimation(1.0f), isAnimating(false),
        targetScale(1.0f), currentScale(1.0f) {
        shape.setSize(size);
        shape.setPosition(position);
        shape.setFillColor(normalColor);
        shape.setOutlineThickness(2);
        shape.setOutlineColor(Color::White);
        text.setFont(font);
        text.setString(label);
        text.setCharacterSize(fontSize);
        text.setFillColor(Color::White);
        text.setStyle(Text::Bold);
        updateTextPosition();
    }

    void updateTextPosition() {
        FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        text.setPosition(shape.getPosition().x + shape.getSize().x / 2.0f, shape.getPosition().y + shape.getSize().y / 2.0f);
    }

    void update(Vector2i mousePos, bool mouseButtonPressed, float deltaTime) {
        FloatRect bounds = shape.getGlobalBounds();
        bool wasHovered = isHovered;
        isHovered = bounds.contains(static_cast<Vector2f>(mousePos));
        if (isHovered && !wasHovered) targetScale = 1.05f;
        else if (!isHovered && wasHovered) targetScale = 1.0f;

        if (currentScale != targetScale) {
            currentScale += (targetScale - currentScale) * deltaTime * 10;
            if (std::abs(currentScale - targetScale) < 0.01f) currentScale = targetScale;
            shape.setScale(currentScale, currentScale);
            float newWidth = originalSize.x * currentScale;
            float newHeight = originalSize.y * currentScale;
            shape.setPosition(originalPosition.x + (originalSize.x - newWidth) / 2, originalPosition.y + (originalSize.y - newHeight) / 2);
            text.setScale(currentScale, currentScale);
            updateTextPosition();
        }

        if (isHovered && mouseButtonPressed && !isAnimating) {
            isAnimating = true;
            pressAnimation = 0.95f;
            shape.setSize(Vector2f(originalSize.x * pressAnimation, originalSize.y * pressAnimation));
            shape.setPosition(originalPosition.x + (originalSize.x - shape.getSize().x) / 2, originalPosition.y + (originalSize.y - shape.getSize().y) / 2);
            updateTextPosition();
        }
        else if (!mouseButtonPressed && isAnimating) {
            isAnimating = false;
            pressAnimation = 1.0f;
            shape.setSize(originalSize);
            shape.setPosition(originalPosition);
            updateTextPosition();
        }

        if (!isAnimating && pressAnimation < 1.0f) {
            pressAnimation += deltaTime * 8;
            if (pressAnimation > 1.0f) pressAnimation = 1.0f;
            shape.setSize(Vector2f(originalSize.x * pressAnimation, originalSize.y * pressAnimation));
            shape.setPosition(originalPosition.x + (originalSize.x - shape.getSize().x) / 2, originalPosition.y + (originalSize.y - shape.getSize().y) / 2);
            updateTextPosition();
        }

        if (isHovered && !isAnimating) shape.setFillColor(hoverColor);
        else if (!isHovered && !isAnimating) shape.setFillColor(normalColor);
    }

    bool isClicked(Vector2i mousePos, bool mouseButtonPressed) {
        return isHovered && mouseButtonPressed;
    }

    void draw(RenderWindow& window) {
        window.draw(shape);
        window.draw(text);
    }

private:
    Color normalColor;
    Color hoverColor;
};

// ==================== Класс для окна победы ====================
class WinWindow {
public:
    RectangleShape panel;
    Text titleText;
    Text scoreText;
    Text wordsText;
    Button nextButton;
    Button menuButton;
    bool active;
    int currentDifficulty;
    float panelAlpha;
    bool isClosing;

    WinWindow(const Font& font)
        : active(false), currentDifficulty(1), panelAlpha(0), isClosing(false),
        nextButton("NEXT LEVEL", font, 32, Vector2f(0, 0), Vector2f(320, 65), Color(0, 100, 0), Color(0, 200, 0)),
        menuButton("BACK TO MENU", font, 32, Vector2f(0, 0), Vector2f(320, 65), Color(100, 0, 0), Color(200, 0, 0)) {
        panel.setSize(Vector2f(600, 450));
        panel.setFillColor(Color(50, 50, 50, 0));
        panel.setOutlineThickness(3);
        panel.setOutlineColor(Color::White);
        titleText.setFont(font);
        titleText.setString("LEVEL COMPLETE!");
        titleText.setCharacterSize(45);
        titleText.setFillColor(Color::Yellow);
        titleText.setStyle(Text::Bold);
        scoreText.setFont(font);
        scoreText.setCharacterSize(35);
        scoreText.setFillColor(Color::White);
        wordsText.setFont(font);
        wordsText.setCharacterSize(35);
        wordsText.setFillColor(Color::White);
    }

    void show(int difficulty, int score, int wordsFound, int totalWords) {
        active = true;
        currentDifficulty = difficulty;
        panelAlpha = 0;
        isClosing = false;
        scoreText.setString("SCORE: " + to_string(score));
        wordsText.setString("WORDS FOUND: " + to_string(wordsFound) + " / " + to_string(totalWords));
    }

    void updatePosition(Vector2f center) {
        panel.setPosition(center.x - 300, center.y - 225);
        titleText.setOrigin(titleText.getLocalBounds().left + titleText.getLocalBounds().width / 2.0f,
            titleText.getLocalBounds().top + titleText.getLocalBounds().height / 2.0f);
        titleText.setPosition(center.x, center.y - 140);
        scoreText.setOrigin(scoreText.getLocalBounds().left + scoreText.getLocalBounds().width / 2.0f,
            scoreText.getLocalBounds().top + scoreText.getLocalBounds().height / 2.0f);
        scoreText.setPosition(center.x, center.y - 50);
        wordsText.setOrigin(wordsText.getLocalBounds().left + wordsText.getLocalBounds().width / 2.0f,
            wordsText.getLocalBounds().top + wordsText.getLocalBounds().height / 2.0f);
        wordsText.setPosition(center.x, center.y + 10);
        nextButton.originalPosition = Vector2f(center.x - 160, center.y + 100);
        nextButton.originalSize = Vector2f(320, 65);
        nextButton.shape.setPosition(nextButton.originalPosition);
        nextButton.shape.setSize(nextButton.originalSize);
        nextButton.updateTextPosition();
        menuButton.originalPosition = Vector2f(center.x - 160, center.y + 180);
        menuButton.originalSize = Vector2f(320, 65);
        menuButton.shape.setPosition(menuButton.originalPosition);
        menuButton.shape.setSize(menuButton.originalSize);
        menuButton.updateTextPosition();
    }

    void update(float deltaTime) {
        if (!active) return;
        if (!isClosing && panelAlpha < 230) {
            panelAlpha += deltaTime * 500;
            if (panelAlpha > 230) panelAlpha = 230;
            panel.setFillColor(Color(50, 50, 50, (Uint8)panelAlpha));
            titleText.setFillColor(Color(255, 255, 0, (Uint8)panelAlpha));
            scoreText.setFillColor(Color(255, 255, 255, (Uint8)panelAlpha));
            wordsText.setFillColor(Color(255, 255, 255, (Uint8)panelAlpha));
        }
        if (isClosing) {
            if (panelAlpha > 0) {
                panelAlpha -= deltaTime * 400;
                if (panelAlpha < 0) panelAlpha = 0;
                panel.setFillColor(Color(50, 50, 50, (Uint8)panelAlpha));
                titleText.setFillColor(Color(255, 255, 0, (Uint8)panelAlpha));
                scoreText.setFillColor(Color(255, 255, 255, (Uint8)panelAlpha));
                wordsText.setFillColor(Color(255, 255, 255, (Uint8)panelAlpha));
            }
            else {
                active = false;
                isClosing = false;
            }
        }
    }

    void updateButtons(Vector2i mousePos, bool mouseButtonPressed, float deltaTime) {
        if (!active || isClosing) return;
        nextButton.update(mousePos, mouseButtonPressed, deltaTime);
        menuButton.update(mousePos, mouseButtonPressed, deltaTime);
    }

    void draw(RenderWindow& window) {
        if (!active) return;
        window.draw(panel);
        window.draw(titleText);
        window.draw(scoreText);
        window.draw(wordsText);
        if (!isClosing) {
            nextButton.draw(window);
            menuButton.draw(window);
        }
    }

    bool isNextClicked(Vector2i mousePos, bool mousePressed) {
        return active && !isClosing && mousePressed && nextButton.isClicked(mousePos, true);
    }

    bool isMenuClicked(Vector2i mousePos, bool mousePressed) {
        return active && !isClosing && mousePressed && menuButton.isClicked(mousePos, true);
    }

    void startClosing() { if (active) isClosing = true; }
    void hide() { active = false; isClosing = false; }
};

// ==================== Класс для анимированной всплывающей надписи ====================
class FloatingWord {
public:
    Text text;
    Vector2f velocity;
    float lifetime;
    float alpha;
    bool active;
    float scale;

    FloatingWord(const std::string& word, const Font& font, Vector2f startPos, Color color = Color::Green)
        : lifetime(2.0f), alpha(255), active(true), scale(0.5f) {
        text.setFont(font);
        text.setString(word);
        text.setCharacterSize(40);
        text.setFillColor(color);
        text.setStyle(Text::Bold);
        FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        text.setPosition(startPos);
        velocity = Vector2f((rand() % 100 - 50) / 50.0f, -2.0f);
    }

    void update(float deltaTime) {
        if (!active) return;
        lifetime -= deltaTime;
        if (lifetime <= 0) {
            active = false;
            return;
        }
        text.move(velocity.x * deltaTime * 100, velocity.y * deltaTime * 100);
        alpha = 255 * (lifetime / 2.0f);
        Color col = text.getFillColor();
        text.setFillColor(Color(col.r, col.g, col.b, (Uint8)alpha));
        if (scale < 1.0f) {
            scale += deltaTime * 3;
            if (scale > 1.0f) scale = 1.0f;
            text.setScale(scale, scale);
        }
    }

    void draw(RenderWindow& window) { if (active) window.draw(text); }
};

// ==================== Класс для анимации нажатия на кружок ====================
class CirclePressAnimation {
public:
    CircleShape circle;
    float lifetime;
    bool active;
    float scale;

    CirclePressAnimation(Vector2f position, float radius) : lifetime(0.5f), active(true), scale(1.0f) {
        circle.setRadius(radius);
        circle.setOrigin(radius, radius);
        circle.setPosition(position);
        circle.setFillColor(Color::Transparent);
        circle.setOutlineThickness(4);
        circle.setOutlineColor(Color::Yellow);
    }

    void update(float deltaTime) {
        if (!active) return;
        lifetime -= deltaTime;
        if (lifetime <= 0) {
            active = false;
            return;
        }
        float progress = 1.0f - (lifetime / 0.5f);
        scale = 1.0f + progress * 0.8f;
        circle.setScale(scale, scale);
        Uint8 alpha = 255 * (1.0f - progress);
        circle.setOutlineColor(Color(255, 255, 0, alpha));
    }

    void draw(RenderWindow& window) { if (active) window.draw(circle); }
};

// ==================== Перечисление состояний игры ====================
enum class GameState { MAIN_MENU, DIFFICULTY_MENU, GAMEPLAY, RULES_MENU };

// Структура для хранения прогресса уровня
struct LevelProgress {
    set<string> availableBonusWords;
    set<string> foundBonusWords;
    set<string> mainWords;
    set<string> foundMainWords;
    int currentScore;
    int totalMainWords;
    bool levelCompleted;
    LevelProgress() : currentScore(0), totalMainWords(0), levelCompleted(false) {}
};



// Функция для создания толстой линии между двумя точками
RectangleShape createThickLine(Vector2f start, Vector2f end, float thickness, Color color) {
    Vector2f direction = end - start;
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);
    RectangleShape line(Vector2f(length, thickness));
    line.setFillColor(color);
    float angle = atan2(direction.y, direction.x) * 180 / 3.14159f;
    line.setRotation(angle);
    line.setPosition(start);
    line.setOrigin(0, thickness / 2);
    return line;
}

void resetCurrentSelection(vector<bool>& isPressed, vector<CircleShape>& smallCircles,
    vector<int>& pressOrder, vector<RectangleShape>& thickLines, int& lastPressedIndex) {
    for (int i = 0; i < isPressed.size(); i++) {
        isPressed[i] = false;
        smallCircles[i].setFillColor(Color(100, 100, 100));
    }
    pressOrder.clear();
    thickLines.clear();
    lastPressedIndex = -1;
}

// Функции для настройки кроссвордов по уровням
void setupEasyCrossword(CrosswordGrid& crossword) {
    // Горизонтальные слова
    crossword.addWord(CrosswordWord("NEST", 1, 5, true));   // строка 1, колонка 5
    crossword.addWord(CrosswordWord("SON", 6, 1, true));    // строка 6, колонка 1
    crossword.addWord(CrosswordWord("TOE", 4, 3, true));    // строка 4, колонка 3
    crossword.addWord(CrosswordWord("TEN", 3, 5, true));    // строка 3, колонка 5

    // Вертикальные слова
    crossword.addWord(CrosswordWord("NOTE", 1, 5, false));   // вертикально от (1,5) до (4,5)
    crossword.addWord(CrosswordWord("STONE", 3, 3, false));  // вертикально от (3,3) до (7,3)
}

void setupMediumCrossword(CrosswordGrid& crossword) {
    // Горизонтальные слова
    crossword.addWord(CrosswordWord("TRACE", 0, 1, true));     // строка 0, колонка 1: T R A C E
    crossword.addWord(CrosswordWord("ACTOR", 2, 4, true));     // строка 2, колонка 5: A C T O R
    crossword.addWord(CrosswordWord("CREATOR", 6, 1, true));   // строка 6, колонка 1: C R E A T O R

    // Вертикальные слова
    crossword.addWord(CrosswordWord("CRATE", 0, 4, false));     // вертикально от (0,4) до (4,4): C R A T E
    crossword.addWord(CrosswordWord("REACTOR", 0, 2, false));   // вертикально от (0,2) до (6,2): R E A C T O R
    crossword.addWord(CrosswordWord("CARROT", 4, 7, false));    // вертикально от (4,7) до (9,7): C A R R O T
}

void setupHardCrossword(CrosswordGrid& crossword) {
    // Горизонтальные слова
    crossword.addWord(CrosswordWord("CRISP", 1, 0, true));      // строка 1, колонка 0: C R I P S
    crossword.addWord(CrosswordWord("PREDICT", 3, 2, true));    // строка 3, колонка 2: P R E D I C T
    crossword.addWord(CrosswordWord("PRIEST", 5, 3, true));     // строка 5, колонка 3: P R I E S T

    // Вертикальные слова
    crossword.addWord(CrosswordWord("STRIPED", 5, 7, false));    // вертикально от (5,7) до (11,7): S T R I P E D
    crossword.addWord(CrosswordWord("SCRIPTED", 1, 3, false));   // вертикально от (1,5) до (9,5): S C R I P T E D
}

// ==================== Главная функция ====================
int main() {
    SetConsoleOutputCP(65001);
    setlocale(LC_ALL, "ru");
    srand(time(0));
    int number_background = 0;
    int totalScore = 0;

    // ========== НАСТРОЙКА УРОВНЕЙ ==========
    set<string> easyMainWords = { "NEST", "NOTE", "TEN", "TOE", "STONE", "SON" };
    set<string> easyBonusWords = { "NET", "NOT", "ONE", "ONSET", "SENT", "SONNET", "TONE", "TONES", "NOSE", "ONES", "SONE", "TON" };
    set<string> mediumMainWords = { "TRACE", "REACTOR", "CRATE", "ACTOR", "CREATOR", "CARROT" };
    set<string> mediumBonusWords = { "ACE", "ACT", "ACTOR", "ARC", "ARE", "ART", "ATE", "CAR", "CARE", "CARROT", "CART", "CAT", "COAT", "CRATE", "CREATOR", "EAR", "EAT", "ERA", "ERR", "OR", "ORATE", "RACE", "RAT", "RATE", "REACT", "REACTOR", "ROAR", "ROT", "ROUTE", "TAR", "TEA", "TEAR", "TO", "TOE", "TOR", "TRACE", "TREE" };
    set<string> hardMainWords = { "CRISP", "SCRIPTED", "PREDICT", "PRIEST", "STRIPED" };
    set<string> hardBonusWords = { "CRIP", "CRIPS", "CRISP", "CRISPT", "CITED", "CREPT", "CREDIT", "DEPICT", "DICE", "DIE", "DIP", "DIPS", "DIRECT", "DIRT", "DISC", "DRIES", "DRIP", "DRIPS", "EDIT", "ICED", "ICE", "PER", "PERT", "PRICE", "PRICED", "PRIDE", "PRIED", "PRIEST", "PRIME", "PRIMED", "PRINT", "PRINTED", "RICE", "RIPE", "RIPED", "RIPS", "SCRIP", "SCRIPT", "SCRIPTED", "SIP", "SIR", "SITE", "SITED", "SPICE", "SPICED", "SPIDER", "SPIRE", "SPIRED", "SPIRIT", "SPRITE", "STEP", "STIR", "STRIDE", "STRIP", "STRIPE", "STRIPED", "TIDE", "TIED", "TIER", "TIERS", "TIP", "TIPS", "TIRED", "TRIP", "TRIPE", "TRIPS" };

    set<string> easyMainFull = easyMainWords;
    set<string> mediumMainFull = mediumMainWords;
    set<string> hardMainFull = hardMainWords;
    set<string> easyBonusFull = easyBonusWords;
    set<string> mediumBonusFull = mediumBonusWords;
    set<string> hardBonusFull = hardBonusWords;

    LevelProgress easyProgress;
    LevelProgress mediumProgress;
    LevelProgress hardProgress;

    easyProgress.totalMainWords = easyMainWords.size();
    mediumProgress.totalMainWords = mediumMainWords.size();
    hardProgress.totalMainWords = hardMainWords.size();
    easyProgress.mainWords = easyMainWords;
    mediumProgress.mainWords = mediumMainWords;
    hardProgress.mainWords = hardMainWords;
    easyProgress.availableBonusWords = easyBonusWords;
    mediumProgress.availableBonusWords = mediumBonusWords;
    hardProgress.availableBonusWords = hardBonusWords;

    LevelProgress* currentProgress = &easyProgress;
    int currentDifficultyLevel = 1;

    vector<char> easyLetters = { 'S', 'O', 'T', 'E', 'N' };
    vector<char> mediumLetters = { 'A', 'C', 'E', 'O', 'R', 'R', 'T' };
    vector<char> hardLetters = { 'C', 'D', 'E', 'I', 'P', 'R', 'S', 'T' };

    ContextSettings settings;
    settings.antialiasingLevel = 8;
    RenderWindow window(VideoMode({ 1440, 1024 }), "Word Game", Style::Close, settings);
    window.setFramerateLimit(60);

    Clock deltaClock;
    float deltaTime = 0;

    // ========== Загрузка ресурсов ==========
    Font font;
    if (!font.loadFromFile("arial.ttf")) {
        if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
            cout << "Ошибка загрузки шрифта!\n";
            return -1;
        }
    }

    WinWindow winWindow(font);

    Texture backgroundTextureMenu;
    Texture backgroundTextureEasy;
    Texture backgroundTextureMedium;
    Texture backgroundTextureHard;
    Sprite backgroundSprite;

    if (!backgroundTextureMenu.loadFromFile("background.png")) cout << "Предупреждение: background.png не найден\n";
    if (!backgroundTextureEasy.loadFromFile("background_easy.png")) cout << "Предупреждение: background_easy.png не найден\n";
    if (!backgroundTextureMedium.loadFromFile("background_medium.png")) cout << "Предупреждение: background_medium.png не найден\n";
    if (!backgroundTextureHard.loadFromFile("background_hard.png")) cout << "Предупреждение: background_hard.png не найден\n";

    backgroundSprite.setTexture(backgroundTextureMenu);
    setFullScreenBackground(backgroundSprite, window);

    Texture logoTexture;
    Sprite logoSprite;
    if (logoTexture.loadFromFile("logo.png")) {
        logoSprite.setTexture(logoTexture);
        float maxWidth = 900.0f;
        float scale = min(1.0f, maxWidth / logoTexture.getSize().x);
        logoSprite.setScale(scale, scale);

        FloatRect bounds = logoSprite.getLocalBounds();
        logoSprite.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
        logoSprite.setPosition(window.getSize().x / 2.0f, 180.0f);
    }

    // ========== ЗАГРУЗКА МУЗЫКИ ==========
    Music menuMusic, easyMusic, mediumMusic, hardMusic;
    if (menuMusic.openFromFile("menu_music.mp3")) { menuMusic.setLoop(true); menuMusic.setVolume(50); }
    else cout << "Предупреждение: menu_music.mp3 не найден\n";
    if (easyMusic.openFromFile("easy_music.mp3")) { easyMusic.setLoop(true); easyMusic.setVolume(50); }
    else cout << "Предупреждение: easy_music.mp3 не найден\n";
    if (mediumMusic.openFromFile("medium_music.mp3")) { mediumMusic.setLoop(true); mediumMusic.setVolume(50); }
    else cout << "Предупреждение: medium_music.mp3 не найден\n";
    if (hardMusic.openFromFile("hard_music.mp3")) { hardMusic.setLoop(true); hardMusic.setVolume(50); }
    else cout << "Предупреждение: hard_music.mp3 не найден\n";

    // ========== Создание кнопок ==========
    Vector2f buttonSize(200, 60);
    float centerX = float(window.getSize().x) / 2.0f;

    Button playButton("PLAY", font, 36, Vector2f(centerX - buttonSize.x / 2.0f, 400.0f), buttonSize, Color(0, 100, 0), Color(0, 200, 0));
    Button rulesButton("RULES", font, 36, Vector2f(centerX - buttonSize.x / 2.0f, 480.0f), buttonSize, Color(0, 0, 100), Color(0, 0, 200));
    Button exitButton("EXIT", font, 36, Vector2f(centerX - buttonSize.x / 2.0f, 560.0f), buttonSize, Color(100, 0, 0), Color(200, 0, 0));
    Button easyButton("EASY", font, 36, Vector2f(centerX - buttonSize.x / 2.0f, 280.0f), buttonSize, Color(0, 0, 100), Color(0, 0, 200));
    Button mediumButton("MEDIUM", font, 36, Vector2f(centerX - buttonSize.x / 2.0f, 360.0f), buttonSize, Color(0, 0, 100), Color(0, 0, 200));
    Button hardButton("HARD", font, 36, Vector2f(centerX - buttonSize.x / 2.0f, 440.0f), buttonSize, Color(0, 0, 100), Color(0, 0, 200));
    Button backButton("BACK TO MENU", font, 32, Vector2f(centerX - 140.0f, 520.0f), Vector2f(280.0f, 55.0f), Color(80, 80, 80), Color(150, 150, 150));
    Button resetButton("RESET", font, 28, Vector2f(float(window.getSize().x) - 150.0f, float(window.getSize().y) - 100.0f), Vector2f(130.0f, 60.0f), Color(100, 100, 100), Color(150, 150, 150));
    Button submitButton("SUBMIT", font, 28, Vector2f(float(window.getSize().x) - 300.0f, float(window.getSize().y) - 100.0f), Vector2f(130.0f, 60.0f), Color(0, 100, 0), Color(0, 200, 0));


    GameState currentState = GameState::MAIN_MENU;
    bool mousePressed = false;
    bool menuMusicPlaying = false;
    bool showRectangle = false;

    RectangleShape gameRectangle;
    gameRectangle.setSize(Vector2f(500, 150));
    gameRectangle.setPosition(float(window.getSize().x) - 530.0f, float(window.getSize().y) - 1000.0f);
    gameRectangle.setFillColor(Color(128, 128, 128));
    gameRectangle.setOutlineThickness(2);
    gameRectangle.setOutlineColor(Color::White);

    Text totalScoreText;
    totalScoreText.setFont(font);
    totalScoreText.setString("TOTAL SCORE: 0");
    totalScoreText.setCharacterSize(36);
    totalScoreText.setFillColor(Color::White);
    totalScoreText.setStyle(Text::Bold);
    totalScoreText.setPosition(float(window.getSize().x) - 500.0f, float(window.getSize().y) - 980.0f);

    Text wordsFoundText;
    wordsFoundText.setFont(font);
    wordsFoundText.setString("WORDS FOUND: 0");
    wordsFoundText.setCharacterSize(30);
    wordsFoundText.setFillColor(Color::Yellow);
    wordsFoundText.setStyle(Text::Bold);
    wordsFoundText.setPosition(float(window.getSize().x) - 500.0f, float(window.getSize().y) - 920.0f);

    // ========== ЭКРАН ПРАВИЛ ==========
    Text rulesTitle;
    rulesTitle.setFont(font);
    rulesTitle.setString("GAME RULES");
    rulesTitle.setCharacterSize(60);
    rulesTitle.setFillColor(Color::Black);
    rulesTitle.setStyle(Text::Bold);
    rulesTitle.setPosition(window.getSize().x / 2 - 150, 50);

    struct RuleBlock { Text title; vector<Text> lines; };
    vector<RuleBlock> ruleBlocks;
    RuleBlock block1, block2, block3, block4, block5, block6;

    block1.title.setFont(font); block1.title.setString("HOW TO PLAY"); block1.title.setCharacterSize(32); block1.title.setFillColor(Color::Red); block1.title.setStyle(Text::Bold);
    vector<string> block1Lines = { "Click on the circles with", "letters in any order", "Each letter can be used once", "until you RESET or SUBMIT", "Find all MAIN words to win!", "Bonus words give extra points", "Press ESCAPE to exit to level select" };
    for (const auto& line : block1Lines) { Text t; t.setFont(font); t.setString(line); t.setCharacterSize(20); t.setFillColor(Color::Black); block1.lines.push_back(t); }
    ruleBlocks.push_back(block1);

    block2.title.setFont(font); block2.title.setString("SUBMIT BUTTON"); block2.title.setCharacterSize(32); block2.title.setFillColor(Color::Red); block2.title.setStyle(Text::Bold);
    vector<string> block2Lines = { "Press SUBMIT to check", "your word", "MAIN words - required to win", "BONUS words - extra points", "Points = number of letters" };
    for (const auto& line : block2Lines) { Text t; t.setFont(font); t.setString(line); t.setCharacterSize(20); t.setFillColor(Color::Black); block2.lines.push_back(t); }
    ruleBlocks.push_back(block2);

    block3.title.setFont(font); block3.title.setString("RESET BUTTON"); block3.title.setCharacterSize(32); block3.title.setFillColor(Color::Red); block3.title.setStyle(Text::Bold);
    vector<string> block3Lines = { "Clears your current", "letter selection", "Allows you to start", "forming a new word" };
    for (const auto& line : block3Lines) { Text t; t.setFont(font); t.setString(line); t.setCharacterSize(20); t.setFillColor(Color::Black); block3.lines.push_back(t); }
    ruleBlocks.push_back(block3);

    block4.title.setFont(font); block4.title.setString("STATISTICS"); block4.title.setCharacterSize(32); block4.title.setFillColor(Color::Red); block4.title.setStyle(Text::Bold);
    vector<string> block4Lines = { "TOTAL SCORE - all points", "from all levels", "WORDS FOUND - total words", "found across all levels", "Progress saves for each", "difficulty level separately" };
    for (const auto& line : block4Lines) { Text t; t.setFont(font); t.setString(line); t.setCharacterSize(20); t.setFillColor(Color::Black); block4.lines.push_back(t); }
    ruleBlocks.push_back(block4);

    block5.title.setFont(font); block5.title.setString("TIPS"); block5.title.setCharacterSize(32); block5.title.setFillColor(Color::Red); block5.title.setStyle(Text::Bold);
    vector<string> block5Lines = { "Try to find all MAIN words!", "Words cannot be used twice", "Longer words = more points", "Bonus words help increase score" };
    for (const auto& line : block5Lines) { Text t; t.setFont(font); t.setString(line); t.setCharacterSize(20); t.setFillColor(Color::Black); block5.lines.push_back(t); }
    ruleBlocks.push_back(block5);

    block6.title.setFont(font); block6.title.setString("DIFFICULTY"); block6.title.setCharacterSize(32); block6.title.setFillColor(Color::Red); block6.title.setStyle(Text::Bold);
    vector<string> block6Lines = { "EASY: 5 letters (S,O,T,E,N)", "MEDIUM: 7 letters (A,C,E,O,R,R,T)", "HARD: 8 letters (C,D,E,I,P,R,S,T)"};
    for (const auto& line : block6Lines) { Text t; t.setFont(font); t.setString(line); t.setCharacterSize(20); t.setFillColor(Color::Black); block6.lines.push_back(t); }
    ruleBlocks.push_back(block6);

    float startX = 100, startY = 140, colWidth = 550, rowHeight = 300;
    vector<pair<int, Vector2f>> blockPositions = { {0, Vector2f(startX, startY)}, {1, Vector2f(startX + colWidth*0.8, startY)}, {2, Vector2f(startX + colWidth * 1.6, startY)}, {3, Vector2f(startX, startY + rowHeight)}, {4, Vector2f(startX + colWidth*0.8, startY + rowHeight)}, {5, Vector2f(startX + colWidth * 1.6, startY + rowHeight)} };
    for (const auto& pos : blockPositions) {
        int idx = pos.first;
        ruleBlocks[idx].title.setPosition(pos.second.x + 20, pos.second.y);
        float yOffset = pos.second.y + 50;
        for (auto& line : ruleBlocks[idx].lines) { line.setPosition(pos.second.x + 20, yOffset); yOffset += 30; }
    }

    Text goodLuckText;
    goodLuckText.setFont(font);
    goodLuckText.setString(" GOOD LUCK AND HAVE FUN! ");
    goodLuckText.setCharacterSize(36);
    goodLuckText.setFillColor(Color::Black);
    goodLuckText.setStyle(Text::Bold);
    goodLuckText.setPosition(window.getSize().x / 2 - 280, window.getSize().y - 250);

    vector<RectangleShape> ruleBackgrounds;
    for (const auto& pos : blockPositions) {
        RectangleShape bg;
        bg.setSize(Vector2f(420, 260));
        bg.setPosition(pos.second.x, pos.second.y);
        bg.setFillColor(Color(255, 255, 255, 230));
        bg.setOutlineThickness(2);
        bg.setOutlineColor(Color(150, 150, 150));
        ruleBackgrounds.push_back(bg);
    }

    Button rulesBackButton("BACK TO MENU", font, 36, Vector2f(centerX - 140.0f, 880.0f), Vector2f(290.0f, 65.0f), Color(80, 80, 80), Color(150, 150, 150));

    // ========== КРУГ С БУКВАМИ ==========
    const int circleRadius = 150;
    const int smallCircleRadius = 34;
    const float lineThickness = 8.0f;

    Vector2f centerPosition(window.getSize().x / 2.0f, window.getSize().y - 250);
    CircleShape bigCircle;
    bigCircle.setRadius(circleRadius);
    bigCircle.setOrigin(circleRadius, circleRadius);
    bigCircle.setPosition(centerPosition);
    bigCircle.setFillColor(Color(50, 50, 50, 200));
    bigCircle.setOutlineThickness(3);
    bigCircle.setOutlineColor(Color::White);
    bigCircle.setPointCount(100);

    int currentNumLetters = 5;
    vector<CircleShape> smallCircles;
    vector<Text> letterTexts;
    vector<char> currentLetters;
    vector<Vector2f> letterPositions;
    vector<bool> isPressed;
    vector<int> pressOrder;
    vector<RectangleShape> thickLines;
    vector<FloatingWord> floatingWords;
    vector<CirclePressAnimation> pressAnimations;
    int lastPressedIndex = -1;

    auto updateCircle = [&](int numLetters, const vector<char>& letters) {
        currentNumLetters = numLetters;
        smallCircles.clear();
        letterTexts.clear();
        letterPositions.clear();
        isPressed.clear();
        pressOrder.clear();
        thickLines.clear();
        float angleStep = 360.0f / numLetters;
        float innerRadius = circleRadius - 55;
        smallCircles.resize(numLetters);
        letterTexts.resize(numLetters);
        letterPositions.resize(numLetters);
        isPressed.resize(numLetters, false);
        currentLetters = letters;
        for (int i = 0; i < numLetters; i++) {
            float angle = i * angleStep * 3.14159f / 180.0f;
            float x = centerPosition.x + innerRadius * cos(angle);
            float y = centerPosition.y + innerRadius * sin(angle);
            letterPositions[i] = Vector2f(x, y);
            smallCircles[i].setRadius(smallCircleRadius);
            smallCircles[i].setOrigin(smallCircleRadius, smallCircleRadius);
            smallCircles[i].setPosition(letterPositions[i]);
            smallCircles[i].setFillColor(Color(100, 100, 100));
            smallCircles[i].setOutlineThickness(2);
            smallCircles[i].setOutlineColor(Color::White);
            letterTexts[i].setFont(font);
            letterTexts[i].setString(string(1, letters[i]));
            letterTexts[i].setCharacterSize(32);
            letterTexts[i].setFillColor(Color::White);
            letterTexts[i].setStyle(Text::Bold);
            FloatRect textRect = letterTexts[i].getLocalBounds();
            letterTexts[i].setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
            letterTexts[i].setPosition(letterPositions[i]);
        }
        };
    updateCircle(5, easyLetters);

    int completedStart = 0;
    if (easyProgress.levelCompleted) completedStart++;
    if (mediumProgress.levelCompleted) completedStart++;
    if (hardProgress.levelCompleted) completedStart++;

    // ========== ПЕРЕМЕННЫЕ ДЛЯ КРОССВОРДА ==========
    CrosswordGrid easyCrossword(10, 10, &font);
    CrosswordGrid mediumCrossword(10, 10, &font);
    CrosswordGrid hardCrossword(12, 12, &font);
    setupEasyCrossword(easyCrossword);
    setupMediumCrossword(mediumCrossword);
    setupHardCrossword(hardCrossword);
    CrosswordGrid* currentCrossword = nullptr;

    // ========== ГЛАВНЫЙ ЦИКЛ ==========
    while (window.isOpen()) {
        deltaTime = deltaClock.restart().asSeconds();
        if (deltaTime > 0.033f) deltaTime = 0.033f;

        Event event;
        bool mouseButtonPressed = false;

        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                mousePressed = true;
                mouseButtonPressed = true;
            }
            if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
                mousePressed = false;
            }
        }

        Vector2i mousePos = Mouse::getPosition(window);

        int completedLevels = 0;
        if (easyProgress.levelCompleted) completedLevels++;
        if (mediumProgress.levelCompleted) completedLevels++;
        if (hardProgress.levelCompleted) completedLevels++;

        if (currentState == GameState::MAIN_MENU) {
            if (!menuMusicPlaying && menuMusic.openFromFile("menu_music.mp3")) { menuMusic.play(); menuMusicPlaying = true; }
            playButton.update(mousePos, mouseButtonPressed, deltaTime);
            rulesButton.update(mousePos, mouseButtonPressed, deltaTime);
            exitButton.update(mousePos, mouseButtonPressed, deltaTime);
            if (mousePressed && playButton.isClicked(mousePos, true)) { currentState = GameState::DIFFICULTY_MENU; showRectangle = true; mousePressed = false; }
            else if (mousePressed && rulesButton.isClicked(mousePos, true)) { currentState = GameState::RULES_MENU; mousePressed = false; }
            else if (mousePressed && exitButton.isClicked(mousePos, true)) { window.close(); }
        }
        else if (currentState == GameState::RULES_MENU) {
            if (!menuMusicPlaying && menuMusic.openFromFile("menu_music.mp3")) { menuMusic.play(); menuMusicPlaying = true; }
            rulesBackButton.update(mousePos, mouseButtonPressed, deltaTime);
            if (mousePressed && rulesBackButton.isClicked(mousePos, true)) { currentState = GameState::MAIN_MENU; mousePressed = false; }
        }
        else if (currentState == GameState::DIFFICULTY_MENU) {
            if (!menuMusicPlaying && menuMusic.openFromFile("menu_music.mp3")) { menuMusic.play(); menuMusicPlaying = true; }
            easyButton.update(mousePos, mouseButtonPressed, deltaTime);
            mediumButton.update(mousePos, mouseButtonPressed, deltaTime);
            hardButton.update(mousePos, mouseButtonPressed, deltaTime);
            backButton.update(mousePos, mouseButtonPressed, deltaTime);

            if (easyProgress.levelCompleted) { easyButton.shape.setFillColor(Color(0, 70, 0)); easyButton.text.setFillColor(Color(150, 255, 150)); }
            else { easyButton.shape.setFillColor(Color(0, 0, 100)); easyButton.text.setFillColor(Color::White); }
            if (mediumProgress.levelCompleted) { mediumButton.shape.setFillColor(Color(0, 70, 0)); mediumButton.text.setFillColor(Color(150, 255, 150)); }
            else { mediumButton.shape.setFillColor(Color(0, 0, 100)); mediumButton.text.setFillColor(Color::White); }
            if (hardProgress.levelCompleted) { hardButton.shape.setFillColor(Color(0, 70, 0)); hardButton.text.setFillColor(Color(150, 255, 150)); }
            else { hardButton.shape.setFillColor(Color(0, 0, 100)); hardButton.text.setFillColor(Color::White); }

            if (mousePressed) {
                if (easyButton.isClicked(mousePos, true)) {
                    currentState = GameState::GAMEPLAY;
                    number_background = 1;
                    currentDifficultyLevel = 1;
                    currentProgress = &easyProgress;
                    currentCrossword = &easyCrossword;
                    updateCircle(5, easyLetters);
                    resetCurrentSelection(isPressed, smallCircles, pressOrder, thickLines, lastPressedIndex);
                    floatingWords.clear();
                    pressAnimations.clear();
                    backgroundSprite.setTexture(backgroundTextureEasy);
                    menuMusic.stop();
                    if (easyMusic.openFromFile("easy_music.mp3")) { easyMusic.play(); menuMusicPlaying = false; }
                    if (currentProgress->levelCompleted) floatingWords.emplace_back("LEVEL ALREADY COMPLETED!", font, Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f), Color::Magenta);
                    mousePressed = false;
                }
                else if (mediumButton.isClicked(mousePos, true)) {
                    currentState = GameState::GAMEPLAY;
                    number_background = 2;
                    currentDifficultyLevel = 2;
                    currentProgress = &mediumProgress;
                    currentCrossword = &mediumCrossword;
                    updateCircle(7, mediumLetters);
                    resetCurrentSelection(isPressed, smallCircles, pressOrder, thickLines, lastPressedIndex);
                    floatingWords.clear();
                    pressAnimations.clear();
                    backgroundSprite.setTexture(backgroundTextureMedium);
                    menuMusic.stop();
                    if (mediumMusic.openFromFile("medium_music.mp3")) { mediumMusic.play(); menuMusicPlaying = false; }
                    if (currentProgress->levelCompleted) floatingWords.emplace_back("LEVEL ALREADY COMPLETED!", font, Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f), Color::Magenta);
                    mousePressed = false;
                }
                else if (hardButton.isClicked(mousePos, true)) {
                    currentState = GameState::GAMEPLAY;
                    number_background = 3;
                    currentDifficultyLevel = 3;
                    currentProgress = &hardProgress;
                    currentCrossword = &hardCrossword;
                    updateCircle(8, hardLetters);
                    resetCurrentSelection(isPressed, smallCircles, pressOrder, thickLines, lastPressedIndex);
                    floatingWords.clear();
                    pressAnimations.clear();
                    backgroundSprite.setTexture(backgroundTextureHard);
                    menuMusic.stop();
                    if (hardMusic.openFromFile("hard_music.mp3")) { hardMusic.play(); menuMusicPlaying = false; }
                    if (currentProgress->levelCompleted) floatingWords.emplace_back("LEVEL ALREADY COMPLETED!", font, Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f), Color::Magenta);
                    mousePressed = false;
                }
                else if (mousePressed && backButton.isClicked(mousePos, true)) {
                    // ========== СБРОС ВСЕГО ПРОГРЕССА ==========
                    // Сброс легкого уровня
                    easyProgress = LevelProgress();
                    easyProgress.totalMainWords = 6; // количество основных слов для легкого уровня
                    easyProgress.mainWords = { "NEST", "NOTE", "TEN", "TOE", "STONE", "SON" };
                    easyProgress.availableBonusWords = { "NET", "NOT", "ONE", "ONSET", "SENT", "SONNET", "TONE", "TONES", "NOSE", "ONES", "SONE", "TON" };

                    // Сброс среднего уровня
                    mediumProgress = LevelProgress();
                    mediumProgress.totalMainWords = 6; // количество основных слов для среднего уровня
                    mediumProgress.mainWords = { "TRACE", "REACTOR", "CRATE", "ACTOR", "CREATOR", "CARROT" };
                    mediumProgress.availableBonusWords = { "ACE", "ACT", "ACTOR", "ARC", "ARE", "ART", "ATE", "CAR", "CARE", "CARROT", "CART", "CAT", "COAT", "CRATE", "CREATOR", "EAR", "EAT", "ERA", "ERR", "OR", "ORATE", "RACE", "RAT", "RATE", "REACT", "REACTOR", "ROAR", "ROT", "ROUTE", "TAR", "TEA", "TEAR", "TO", "TOE", "TOR", "TRACE", "TREE" };

                    // Сброс сложного уровня
                    hardProgress = LevelProgress();
                    hardProgress.totalMainWords = 5; // количество основных слов для сложного уровня
                    hardProgress.mainWords = { "CRISP", "SCRIPTED", "PREDICT", "PRIEST", "STRIPED" };
                    hardProgress.availableBonusWords = { "CRIP", "CRIPS", "CRISP", "CRISPT", "CITED", "CREPT", "CREDIT", "DEPICT", "DICE", "DIE", "DIP", "DIPS", "DIRECT", "DIRT", "DISC", "DRIES", "DRIP", "DRIPS", "EDIT", "ICED", "ICE", "PER", "PERT", "PRICE", "PRICED", "PRIDE", "PRIED", "PRIEST", "PRIME", "PRIMED", "PRINT", "PRINTED", "RICE", "RIPE", "RIPED", "RIPS", "SCRIP", "SCRIPT", "SCRIPTED", "SIP", "SIR", "SITE", "SITED", "SPICE", "SPICED", "SPIDER", "SPIRE", "SPIRED", "SPIRIT", "SPRITE", "STEP", "STIR", "STRIDE", "STRIP", "STRIPE", "STRIPED", "TIDE", "TIED", "TIER", "TIERS", "TIP", "TIPS", "TIRED", "TRIP", "TRIPE", "TRIPS" };

                    // Сброс общего счета
                    totalScore = 0;

                    // Пересоздание кроссвордов (чтобы очистить найденные слова)
                    easyCrossword = CrosswordGrid(10, 10, &font);
                    mediumCrossword = CrosswordGrid(10, 10, &font);
                    hardCrossword = CrosswordGrid(12, 12, &font);
                    setupEasyCrossword(easyCrossword);
                    setupMediumCrossword(mediumCrossword);
                    setupHardCrossword(hardCrossword);

                    // Возврат в главное меню
                    currentState = GameState::MAIN_MENU;
                    number_background = 0;
                    showRectangle = false;
                    resetCurrentSelection(isPressed, smallCircles, pressOrder, thickLines, lastPressedIndex);
                    floatingWords.clear();
                    pressAnimations.clear();
                    mousePressed = false;
                    backgroundSprite.setTexture(backgroundTextureMenu);
                    setFullScreenBackground(backgroundSprite, window);
                    easyMusic.stop();
                    mediumMusic.stop();
                    hardMusic.stop();
                    if (menuMusic.openFromFile("menu_music.mp3")) { menuMusic.play(); menuMusicPlaying = true; }
                }
            }
        }
        else if (currentState == GameState::GAMEPLAY) {
            winWindow.updatePosition(Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f));
            winWindow.update(deltaTime);
            winWindow.updateButtons(mousePos, mouseButtonPressed, deltaTime);

            if (winWindow.active) {
                if (winWindow.isNextClicked(mousePos, mousePressed)) {
                    winWindow.startClosing();
                    if (currentDifficultyLevel == 1 && !mediumProgress.levelCompleted) {
                        currentState = GameState::GAMEPLAY;
                        number_background = 2;
                        currentDifficultyLevel = 2;
                        currentProgress = &mediumProgress;
                        currentCrossword = &mediumCrossword;
                        updateCircle(7, mediumLetters);
                        resetCurrentSelection(isPressed, smallCircles, pressOrder, thickLines, lastPressedIndex);
                        floatingWords.clear();
                        pressAnimations.clear();
                        backgroundSprite.setTexture(backgroundTextureMedium);
                        easyMusic.stop();
                        if (mediumMusic.openFromFile("medium_music.mp3")) mediumMusic.play();
                    }
                    else if (currentDifficultyLevel == 2 && !hardProgress.levelCompleted) {
                        currentState = GameState::GAMEPLAY;
                        number_background = 3;
                        currentDifficultyLevel = 3;
                        currentProgress = &hardProgress;
                        currentCrossword = &hardCrossword;
                        updateCircle(8, hardLetters);
                        resetCurrentSelection(isPressed, smallCircles, pressOrder, thickLines, lastPressedIndex);
                        floatingWords.clear();
                        pressAnimations.clear();
                        backgroundSprite.setTexture(backgroundTextureHard);
                        setFullScreenBackground(backgroundSprite, window);
                        mediumMusic.stop();
                        if (hardMusic.openFromFile("hard_music.mp3")) hardMusic.play();
                    }
                    else {
                        currentState = GameState::DIFFICULTY_MENU;
                        winWindow.hide();
                        backgroundSprite.setTexture(backgroundTextureMenu);
                        setFullScreenBackground(backgroundSprite, window);
                        hardMusic.stop();
                        if (menuMusic.openFromFile("menu_music.mp3")) { menuMusic.play(); menuMusicPlaying = true; }
                    }
                    mousePressed = false;
                }
                else if (winWindow.isMenuClicked(mousePos, mousePressed)) {
                    winWindow.startClosing();
                    currentState = GameState::DIFFICULTY_MENU;
                    mousePressed = false;
                    backgroundSprite.setTexture(backgroundTextureMenu);
                    setFullScreenBackground(backgroundSprite, window);
                    easyMusic.stop();
                    mediumMusic.stop();
                    hardMusic.stop();
                    if (menuMusic.openFromFile("menu_music.mp3")) { menuMusic.play(); menuMusicPlaying = true; }
                }
            }

            for (auto it = floatingWords.begin(); it != floatingWords.end();) {
                it->update(deltaTime);
                if (!it->active) it = floatingWords.erase(it);
                else ++it;
            }
            for (auto it = pressAnimations.begin(); it != pressAnimations.end();) {
                it->update(deltaTime);
                if (!it->active) it = pressAnimations.erase(it);
                else ++it;
            }

            size_t totalFound = easyProgress.foundMainWords.size() + easyProgress.foundBonusWords.size() +
                mediumProgress.foundMainWords.size() + mediumProgress.foundBonusWords.size() +
                hardProgress.foundMainWords.size() + hardProgress.foundBonusWords.size();
            wordsFoundText.setString("WORDS FOUND: " + to_string(totalFound));
            totalScore = easyProgress.currentScore + mediumProgress.currentScore + hardProgress.currentScore;
            totalScoreText.setString("TOTAL SCORE: " + to_string(totalScore));

            resetButton.update(mousePos, mouseButtonPressed, deltaTime);
            submitButton.update(mousePos, mouseButtonPressed, deltaTime);

            if (!winWindow.active) {
                if (mousePressed && resetButton.isClicked(mousePos, true)) {
                    resetCurrentSelection(isPressed, smallCircles, pressOrder, thickLines, lastPressedIndex);
                    floatingWords.emplace_back("RESET!", font, Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f), Color::Yellow);
                    mousePressed = false;
                }
                else if (mousePressed && submitButton.isClicked(mousePos, true)) {
                    string currentWord = "";
                    for (int idx : pressOrder) currentWord += currentLetters[idx];

                    if (!currentWord.empty() && !currentProgress->levelCompleted) {
                        bool isMainWord = (currentProgress->mainWords.find(currentWord) != currentProgress->mainWords.end());
                        bool isBonusWord = (!isMainWord && currentProgress->availableBonusWords.find(currentWord) != currentProgress->availableBonusWords.end());
                        bool alreadyFoundMain = (currentProgress->foundMainWords.find(currentWord) != currentProgress->foundMainWords.end());
                        bool alreadyFoundBonus = (currentProgress->foundBonusWords.find(currentWord) != currentProgress->foundBonusWords.end());

                        if (isMainWord && !alreadyFoundMain) {
                            int points = currentWord.length();
                            currentProgress->currentScore += points;
                            currentProgress->foundMainWords.insert(currentWord);
                            if (currentCrossword != nullptr) currentCrossword->findAndMarkWord(currentWord);
                            floatingWords.emplace_back("MAIN: " + currentWord + " +" + to_string(points), font, Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f), Color::Green);
                            resetCurrentSelection(isPressed, smallCircles, pressOrder, thickLines, lastPressedIndex);
                            if (currentProgress->foundMainWords.size() == currentProgress->totalMainWords) {
                                currentProgress->levelCompleted = true;
                                int wordsFoundCount = currentProgress->foundMainWords.size() + currentProgress->foundBonusWords.size();
                                int newCompletedLevels = 0;
                                if (easyProgress.levelCompleted) newCompletedLevels++;
                                if (mediumProgress.levelCompleted) newCompletedLevels++;
                                if (hardProgress.levelCompleted) newCompletedLevels++;
                                float globalProgress = newCompletedLevels / 3.0f;
                                winWindow.show(currentDifficultyLevel, currentProgress->currentScore, wordsFoundCount, currentProgress->totalMainWords + currentProgress->foundBonusWords.size());
                                winWindow.updatePosition(Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f));
                            }
                        }
                        else if (isBonusWord && !alreadyFoundBonus) {
                            int points = currentWord.length();
                            currentProgress->currentScore += points;
                            currentProgress->foundBonusWords.insert(currentWord);
                            currentProgress->availableBonusWords.erase(currentWord);
                            floatingWords.emplace_back("BONUS: " + currentWord + " +" + to_string(points), font, Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f), Color(100, 200, 100));
                            resetCurrentSelection(isPressed, smallCircles, pressOrder, thickLines, lastPressedIndex);
                        }
                        else if (alreadyFoundMain || alreadyFoundBonus) {
                            floatingWords.emplace_back(currentWord + " - ALREADY FOUND!", font, Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f), Color(255, 165, 0));
                            resetCurrentSelection(isPressed, smallCircles, pressOrder, thickLines, lastPressedIndex);
                        }
                        else {
                            floatingWords.emplace_back("\"" + currentWord + "\" - WRONG!", font, Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f), Color::Red);
                            resetCurrentSelection(isPressed, smallCircles, pressOrder, thickLines, lastPressedIndex);
                        }
                    }
                    else if (currentWord.empty()) {
                        floatingWords.emplace_back("SELECT LETTERS FIRST!", font, Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f), Color::Red);
                    }
                    else if (currentProgress->levelCompleted) {
                        floatingWords.emplace_back("LEVEL ALREADY COMPLETED!", font, Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f), Color::Magenta);
                    }
                    mousePressed = false;
                }
                else if (mousePressed && !currentProgress->levelCompleted) {
                    for (int i = 0; i < currentNumLetters; i++) {
                        FloatRect circleBounds(smallCircles[i].getPosition().x - smallCircleRadius, smallCircles[i].getPosition().y - smallCircleRadius, smallCircleRadius * 2, smallCircleRadius * 2);
                        if (circleBounds.contains(static_cast<Vector2f>(mousePos))) {
                            if (!isPressed[i]) {
                                isPressed[i] = true;
                                pressOrder.push_back(i);
                                smallCircles[i].setFillColor(Color::Green);
                                pressAnimations.emplace_back(letterPositions[i], smallCircleRadius);
                                thickLines.clear();
                                for (size_t j = 0; j < pressOrder.size(); j++) {
                                    if (j > 0) {
                                        int prevIndex = pressOrder[j - 1];
                                        thickLines.push_back(createThickLine(letterPositions[prevIndex], letterPositions[pressOrder[j]], lineThickness, Color::Yellow));
                                    }
                                }
                            }
                            break;
                        }
                    }
                    mousePressed = false;
                }
            }

            if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                currentState = GameState::DIFFICULTY_MENU;
                number_background = 0;
                resetCurrentSelection(isPressed, smallCircles, pressOrder, thickLines, lastPressedIndex);
                floatingWords.clear();
                pressAnimations.clear();
                winWindow.hide();
                backgroundSprite.setTexture(backgroundTextureMenu);
                setFullScreenBackground(backgroundSprite, window);
                easyMusic.stop();
                mediumMusic.stop();
                hardMusic.stop();
                if (menuMusic.openFromFile("menu_music.mp3")) { menuMusic.play(); menuMusicPlaying = true; }
            }
        }

        // ========== ОТРИСОВКА ==========
        window.clear(Color::Black);
        window.draw(backgroundSprite);

        if (currentState == GameState::MAIN_MENU && logoSprite.getTexture() != nullptr) window.draw(logoSprite);

        if (currentState == GameState::DIFFICULTY_MENU) {
            easyButton.draw(window);
            mediumButton.draw(window);
            hardButton.draw(window);
            backButton.draw(window);
        }
        else if (currentState == GameState::RULES_MENU) {
            for (auto& bg : ruleBackgrounds) window.draw(bg);
            window.draw(rulesTitle);
            for (auto& block : ruleBlocks) {
                window.draw(block.title);
                for (auto& line : block.lines) window.draw(line);
            }
            window.draw(goodLuckText);
            rulesBackButton.draw(window);
        }
        else if (currentState == GameState::MAIN_MENU) {
            playButton.draw(window);
            rulesButton.draw(window);
            exitButton.draw(window);
        }

        if (showRectangle && currentState != GameState::DIFFICULTY_MENU && currentState != GameState::MAIN_MENU) {
            window.draw(gameRectangle);
            window.draw(totalScoreText);
            window.draw(wordsFoundText);
        }

        if (currentState == GameState::GAMEPLAY) {
            if (currentCrossword != nullptr) {
                float crosswordX = 100.0f, crosswordY = 100.0f, cellSize = 50.0f;
                currentCrossword->draw(window, crosswordX, crosswordY, cellSize);
            }
            window.draw(bigCircle);
            for (auto& line : thickLines) window.draw(line);
            for (int i = 0; i < currentNumLetters; i++) {
                window.draw(smallCircles[i]);
                window.draw(letterTexts[i]);
            }
            resetButton.draw(window);
            submitButton.draw(window);
            for (auto& anim : pressAnimations) anim.draw(window);
            for (auto& word : floatingWords) word.draw(window);
            winWindow.draw(window);
        }

        window.display();
    }

    return 0;
}