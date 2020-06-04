#ifndef Level_h
#define Level_h
#include "Enemies.h"
#include <sstream>
const std::string TD_Font = "C:\\spr\\TD_Font.ttf";
const std::string menu_background = "C:\\spr\\menu_background.png";
const std::string TD_Font1 = "C:\\spr\\TD_Font.ttf";

class StatusBar {

private:
    float s_x;
    float s_y;
    int s_textSize;
    std::wostringstream s_hp;
    std::wostringstream s_money;
    std::wostringstream s_wave;
    std::wstring t_health;
    std::wstring t_money;
    std::wstring t_wave;
    sf::Font s_font;
    sf::Text s_textHP;
    sf::Text s_textMONEY;
public:
    StatusBar() {
        s_x = 0;
        s_y = 0;
        int s_textSize = 20;
        s_hp << 10;
        s_money << 20;
        t_health = L"Здоровье: ";
        t_money = L"Деньги: ";
        t_wave = L"Следующая волна: ";
        s_font.loadFromFile(TD_Font);
        s_textHP.setFont(s_font);
        s_textHP.setPosition(s_x, s_y);
        s_textHP.setFillColor(sf::Color::White);
        s_textHP.setCharacterSize(s_textSize);
        
        s_textMONEY.setFont(s_font);
        s_textMONEY.setPosition(s_x, s_y+s_textSize);
        s_textMONEY.setFillColor(sf::Color::White);
        s_textMONEY.setCharacterSize(s_textSize);
    }
    
    StatusBar(float x, float y, int hp, int money): StatusBar() {
        s_x = x;
        s_y = y;
        s_hp << hp;
        s_money << money;
    }
    
    void Update(int hp, int money) {
        s_hp.str(L"");
        s_money.str(L"");
        s_hp << t_health << hp;
        s_money << t_money << money;
        s_textHP.setString(s_hp.str());
        s_textMONEY.setString(s_money.str());
        
    }
    
    void Draw(sf::RenderWindow *Window) {
        Window->draw(s_textHP);
        Window->draw(s_textMONEY);
    }
    
    void NextWave(int i) {
        
    }
};

class Level {
private:
    int GAMER_HP;
    int GAMER_MONEY;
    GameMap Map;
    BlankField *Towers;
    EnemyWave **Mobs;
    TowerShop ShopGUI;
    StatusBar Player_status;
    int l_wavesCount;
    int l_currentWave;
    bool l_isPause;
    int mouse_x;
    int mouse_y;
    
public:
    Level() {
        GAMER_HP = 10;
        GAMER_MONEY = 100;
        l_wavesCount = 10;
        l_currentWave = 0;
        l_isPause = false;
        Towers = new BlankField(1280, 720);
        Mobs = new EnemyWave*[l_wavesCount];
        for(int i = 0; i < l_wavesCount; i++) {
            Mobs[i] = new EnemyWave(i);
            Mobs[i]->setPathForMobs(Towers);
        }
    }
    
    void Update(int x, int y) {
        if (!l_isPause) {
            mouse_x = x;
            mouse_y = y;
            if (ShopGUI.getDragged()) ShopGUI.SetMouseC(mouse_x, mouse_y);
            Towers->Update(mouse_x, mouse_y);
            if(Mobs[l_currentWave]->isWaveEnded()) if(l_currentWave < l_wavesCount-1) {
                l_currentWave++;
                Mobs[l_currentWave]->zeroWaveCounter();
                Mobs[l_currentWave]->setPathForMobs(Towers);
            }
            Mobs[l_currentWave]->Update(&GAMER_HP, &GAMER_MONEY);
            ShopGUI.Update();
            Player_status.Update(GAMER_HP, GAMER_MONEY);
            Shooting();
        }
    }
    
    void Draw(sf::RenderWindow *Window) {
        Window->draw(Map.getSprite());
        Towers->Draw(Window);
        Mobs[l_currentWave]->Draw(Window);
        ShopGUI.Draw(Window);
        Player_status.Draw(Window);
    }
    
    void LeftClick() {
        if (mouse_x > 80 && mouse_y > 80 && mouse_x < 1200 && mouse_y < 640) {
            Towers->towerUpgrade(mouse_x, mouse_y, &GAMER_MONEY);
            if (ShopGUI.getDragged()) {
                Towers->blankClicked(mouse_x, mouse_y, ShopGUI.selectedT(), &GAMER_MONEY);
                if(!Mobs[l_currentWave]->setPathForMobs(Towers) || Towers->pathCalc(55).empty()) Towers->DeleteTower(mouse_x, mouse_y, &GAMER_MONEY);
            }
        }
        ShopGUI.DragOff();
        if (mouse_x > 0 && mouse_y > 640 && mouse_x < 160 && mouse_y < 720) {
            ShopGUI.Clicked(mouse_x, mouse_y);
        }
    }
    
    void RightClick() {
    }
    
    int getHP() { return GAMER_HP; }
    bool isGameWinned() {
        if(Mobs[l_currentWave]->isWaveEnded() && l_currentWave == l_wavesCount - 1 && GAMER_HP > 0) return true;
        return false;
    }
    
    void Pause() {
        if (l_isPause) {
            Mobs[l_currentWave]->zeroCounter();
            l_isPause = false;
        } else {
            l_isPause = true;
        }
        
    }
    
    void StartGame() {
        Mobs[l_currentWave]->zeroCounter();
        Mobs[l_currentWave]->zeroWaveCounter();
    }
    
    void Shooting() {
        for(int i = 0; i < Towers->getCount(); i++) {
            if(!Towers->getIsTower(i)) continue;
            for(int j = 0; j < Mobs[l_currentWave]->getSpawnedCount(); j++) {
                if(!Mobs[l_currentWave]->isMobSpawned(j)) continue;
                if(checkEnemyInRadius(Towers->getTower(i), Mobs[l_currentWave]->getEnemy(j))) {
                    AttackE(Towers->getTower(i), Mobs[l_currentWave]->getEnemy(j));
                }
            }
        }
    }
    
    bool checkEnemyInRadius(Blank const *Tower, Mob2 const *Enemy) const {
        if(Enemy->getX()+40 < Tower->getX()+40 + Tower->getRadius() && Enemy->getX()+40 > Tower->getX()+40 - Tower->getRadius()) {
            if(Enemy->getY()+40 < Tower->getY()+40 + Tower->getRadius() && Enemy->getY()+40 > Tower->getY()+40 - Tower->getRadius()) {
                return true;
            }
        }
        return false;
    }
    
    void AttackE(Blank *Tower, Mob2 *Enemy) {
        Tower->Attack(*Enemy);
    }
    
    ~Level() {
        delete Towers;
        for(int i = 0; i < l_wavesCount; i++) {
            delete Mobs[i];
        }
        delete [] Mobs;
    }
};

class Menu {
private:
    Level **levels;
    int mouse_x;
    int mouse_y;
    int current_level;
    int number_of_levels;
    int button_width;
    int button_height;
    sf::RectangleShape startButton;
    sf::Texture t_background;
    sf::Sprite s_background;
    sf::Font game_font;
    sf::Text game_name;
    sf::Text startButton_text;
    bool is_lose;
    sf::Text losing;
    
    bool isMenuOpened;
public:
    Menu() {
        current_level = 0;
        number_of_levels = 1;
        levels = new Level*[number_of_levels];
        for(int i = 0; i < number_of_levels; i++) {
            levels[i] = new Level();
        }
        isMenuOpened = true;
        is_lose = false;
        button_width = 320;
        button_height = 80;
        t_background.loadFromFile(menu_background);
        s_background.setTexture(t_background);
        startButton.setSize(sf::Vector2f(button_width, button_height));
        startButton.setFillColor(sf::Color::Transparent);
        startButton.setOutlineColor(sf::Color::Black);
        startButton.setOutlineThickness(-3);
        startButton.setPosition((s_background.getGlobalBounds().width-button_width)/2, 200);
        game_font.loadFromFile(TD_Font1);
        game_name.setFont(game_font);
        game_name.setString("Tower Defence");
        game_name.setCharacterSize(40);
        game_name.setPosition((s_background.getGlobalBounds().width-game_name.getGlobalBounds().width)/2, 50);

        startButton_text.setFont(game_font);
        startButton_text.setString(L"Начать игру");
        startButton_text.setCharacterSize(20);
        startButton_text.setPosition(startButton.getGlobalBounds().left+(startButton.getGlobalBounds().width - startButton_text.getGlobalBounds().width)/2, startButton.getGlobalBounds().top+(startButton.getGlobalBounds().height - startButton_text.getGlobalBounds().height)/2);
        
        losing.setFont(game_font);
        losing.setString(L"Вы проиграли!!!");
        losing.setCharacterSize(30);
        losing.setPosition((s_background.getGlobalBounds().width-losing.getGlobalBounds().width)/2, 400);
    }
    
    void Draw(sf::RenderWindow *Window) {
        if(isMenuOpened) {
            Window->draw(s_background);
            Window->draw(game_name);
            if(!is_lose) {
                Window->draw(startButton);
                Window->draw(startButton_text);
            } else {
                Window->draw(losing);
            }
        } else {
            levels[current_level]->Draw(Window);
        }
    }
    
    void Update(int x, int y) {
        mouse_x = x;
        mouse_y = y;
        if(!isMenuOpened) {
            levels[current_level]->Update(mouse_x, mouse_y);
            if(levels[current_level]->getHP() <= 0) {
                isMenuOpened = true;
                losing.setString(L"Вы проиграли!!!");
                is_lose = true;
            }
            if(levels[current_level]->isGameWinned()) {
                isMenuOpened = true;
                losing.setString(L"Вы выиграли!!!");
                is_lose = true;
            }
        }
    }
    
    void Input(sf::Event event, float *MouseTime) {
        if(!isMenuOpened) {
            if (event.type == event.MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                levels[current_level]->LeftClick();
                *MouseTime = 0;
            }
            if (event.type == event.MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
                levels[current_level]->RightClick();
                *MouseTime = 0;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                levels[current_level]->Pause();
                *MouseTime = 0;
            }
        } else {
            if (event.type == event.MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                button_pressed();
                *MouseTime = 0;
            }
        }
    }
    
    void button_pressed() {
        int left = startButton.getGlobalBounds().left;
        int top = startButton.getGlobalBounds().top;
        if(mouse_x > left && mouse_x < left + button_width && mouse_y > top && mouse_y < top + button_height) {
            isMenuOpened = false;
            levels[current_level]->StartGame();
        }
    }
};
#endif /* Level_h */
