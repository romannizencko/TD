#ifndef Towers_h
#define Towers_h
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <queue>
const std::string background_1 = "C:\\spr\\background.png";
const std::string Blank_1 = "C:\\spr\\Blank.png";
const std::string Blank_2 = "C:\\spr\\Blank.png";
const std::string exit_1 = "C:\\spr\\exit.png";
const std::string start_1 = "C:\\spr\\start.png";
const std::string GunTower_1 = "C:\\spr\\tower_body.png";
const std::string GunTower_gun_1 = "C:\\spr\\tower_gun.png";
const std::string GunTower_2 = "C:\\spr\\tower_body.png";
const std::string GunTower_gun_2 = "C:\\spr\\tower_gun2.png";
const std::string towershop_1 = "C:\\spr\\towershop.png";
class GameMap {
protected:
    sf::Texture m_Texture;
    sf::Sprite Sprite;
public:
    GameMap() {
        m_Texture.loadFromFile(background_1);
        Sprite.setTexture(m_Texture);
        Sprite.setPosition(0, 0);
    }
    
    sf::Sprite getSprite() { return Sprite; }
};


class GameObject {
protected:
    float m_x;
    float m_y;
    sf::Texture m_texture;
    sf::Sprite m_sprite;
public:
    GameObject() {
        m_texture.loadFromFile(Blank_1);
        m_sprite.setTexture(m_texture);
        m_x = 0;
        m_y = 0;
    }
    GameObject(float x, float y) : GameObject() {
        m_x = x;
        m_y = y;
    }
    
    float getX() const { return m_x; }
    float getY() const { return m_y; }
    
    void setX(float x) { m_x = x; m_sprite.setPosition(m_x, m_y); }
    void setY(float y) { m_y = y; m_sprite.setPosition(m_x, m_y); }
    
    virtual void Update() = 0;
    virtual void Draw(sf::RenderWindow *Window) = 0;
    virtual void onClick() = 0;
    
    sf::Sprite getSprite() { return m_sprite; }
};

class Mob2;

class BlankTower : public GameObject {
protected:
    bool b_isTower;
    sf::Texture m_guntexture;
    sf::Sprite m_gunsprite;
    int t_damage;
    int t_speedAttack;
    int t_attackRange;
    int t_level;
    int t_maxLevel;
    int t_price;
    int t_upgradePrice;
    double t_angle;
    float en_x;
    float en_y;
public:
    BlankTower() {}
    BlankTower(int x, int y) : BlankTower() {}
    
    bool isTower() { return b_isTower; }
    
    void Update() {
        m_gunsprite.setRotation(t_angle);
        m_sprite.setPosition(m_x, m_y);
        m_gunsprite.setPosition(m_x + 40, m_y + 40);
    }
    
    void setX(float x) {
        m_x = x;
        m_sprite.setPosition(m_x, m_y);
        m_gunsprite.setPosition(m_x + 40, m_y + 40);
    }
    
    void setY(float y) {
        m_y = y;
        m_sprite.setPosition(m_x, m_y);
        m_gunsprite.setPosition(m_x + 40, m_y + 40);
    }
    
    void Draw(sf::RenderWindow *Window) {
        Window->draw(m_sprite);
        Window->draw(m_gunsprite);
    }
    void onClick() {}
    virtual void gunRot(int x, int y) = 0;
    virtual void Attack(Mob2 &Enemy) = 0;
};

class Blank : public BlankTower {
public:
    Blank() {
        m_texture.loadFromFile(Blank_2);
        m_sprite.setTexture(m_texture);
        m_x = 0;
        m_y = 0;
        b_isTower = false;
    }
    
    Blank(float x, float y) : Blank() {
        m_x = x;
        m_y = y;
        m_sprite.setPosition(m_x, m_y);
    }
    
    void Draw(sf::RenderWindow *Window) {
        m_sprite.setPosition(m_x, m_y);
        Window->draw(m_sprite);
    }
    int getRadius() const {
        return t_attackRange;
    }
    int getPrice() { return t_price; }
    int getUpgradePrice() { return t_upgradePrice; }
    int getLevel() { return t_level; }
    int getMaxLevel() { return t_maxLevel; }
    void Update() {
        m_sprite.setPosition(m_x, m_y);
    }
    
    void gunRot(int x, int y) {}
    void Attack(Mob2 &Enemy) {}
};

class CastleBlank : public Blank {
public:
    CastleBlank() {
        m_texture.loadFromFile(exit_1);
        m_sprite.setTexture(m_texture);
        m_x = 0;
        m_y = 320;
        b_isTower = false;
    }
};

class StartBlank : public Blank {
public:
    StartBlank() {
        m_texture.loadFromFile(start_1);
        m_sprite.setTexture(m_texture);
        m_x = 1200;
        m_y = 320;
        b_isTower = false;
    }
};

class GunTower : public Blank {
private:
    sf::Clock ShotCooldown;
public:
    GunTower() {
        m_texture.loadFromFile(GunTower_1);
        m_guntexture.loadFromFile(GunTower_gun_1);
        m_sprite.setTexture(m_texture);
        m_sprite.setTextureRect(sf::IntRect(0, 0, 80, 80));
        m_gunsprite.setTexture(m_guntexture);
        m_gunsprite.setTextureRect(sf::IntRect(0, 0, 80, 80));
        m_gunsprite.setOrigin(40, 40);
        t_damage = 10;
        t_speedAttack = 700;
        t_attackRange = 160;
        t_price = 25;
        t_upgradePrice = 50;
        t_level = 0;
        t_maxLevel = 2;
        t_angle = 0;
        b_isTower = true;
    }
    
    GunTower(float x, float y) : GunTower() {
        m_x = x;
        m_y = y;
        m_sprite.setPosition(m_x, m_y);
        m_gunsprite.setPosition(m_x + 40, m_y + 40);
    }
    
    void Attack(Mob2 &Enemy);
    
    void Update() {
        m_gunsprite.setRotation(t_angle);
        m_sprite.setPosition(m_x, m_y);
        m_gunsprite.setPosition(m_x + 40, m_y + 40);
    }
    
    void Draw(sf::RenderWindow *Window) {
        Window->draw(m_sprite);
        Window->draw(m_gunsprite);
    }
    
    void onClick() {
        if (t_level < t_maxLevel) t_level++;
        if (t_level == 1) {
            m_sprite.setTextureRect(sf::IntRect(80, 0, 80, 80));
            m_gunsprite.setTextureRect(sf::IntRect(80, 0, 80, 80));
            t_damage = 15;
            t_upgradePrice = 30;
            t_speedAttack = 500;
            t_attackRange = 180;
        }
        else if (t_level == 2) {
            m_sprite.setTextureRect(sf::IntRect(160, 0, 80, 80));
            m_gunsprite.setTextureRect(sf::IntRect(160, 0, 80, 80));
            t_damage = 20;
            t_speedAttack = 300;
            t_attackRange = 200;
        }
    }
    
    void gunRot(int x, int y) {}
};

class GunTower2 : public GunTower {
public:
    GunTower2() {
        m_texture.loadFromFile(GunTower_2);
        m_guntexture.loadFromFile(GunTower_gun_2);
        m_sprite.setTexture(m_texture);
        m_gunsprite.setTexture(m_guntexture);
        m_sprite.setTextureRect(sf::IntRect(0, 0, 80, 80));
        m_gunsprite.setTextureRect(sf::IntRect(0, 0, 80, 80));
        m_gunsprite.setOrigin(40, 40);
        t_damage = 30;
        t_speedAttack = 1500;
        t_attackRange = 240;
        t_price = 40;
        t_upgradePrice = 60;
        t_level = 0;
        t_maxLevel = 2;
        t_angle = 0;
        b_isTower = true;
    }
    GunTower2(float x, float y) : GunTower2() {
        m_x = x;
        m_y = y;
        m_sprite.setPosition(m_x, m_y);
        m_gunsprite.setPosition(m_x + 40, m_y + 40);
    }
    
    void onClick() {
        if (t_level < t_maxLevel) t_level++;
        if (t_level == 1) {
            m_sprite.setTextureRect(sf::IntRect(80, 0, 80, 80));
            m_gunsprite.setTextureRect(sf::IntRect(80, 0, 80, 80));
            t_damage = 50;
            t_upgradePrice = 100;
            t_speedAttack = 1100;
            t_attackRange = 260;
        }
        else if (t_level == 2) {
            m_sprite.setTextureRect(sf::IntRect(160, 0, 80, 80));
            m_gunsprite.setTextureRect(sf::IntRect(160, 0, 80, 80));
            t_damage = 80;
            t_speedAttack = 600;
            t_attackRange = 280;
        }
    }
};

class BlankField {
private:
    Blank **test;
    int m_count;
    CastleBlank B_Finish;
    StartBlank B_Start;
    int b_start;
    int b_finish;
    struct LessPath {
        bool operator()(const int a, const int b) const {
            int a_x = a / 14;
            int a_y = a - a / 14 * 14;
            int b_x = b / 14;
            int b_y = b - b / 14 * 14;
            return (abs(3 - a_x) + a_y) > (abs(3 - b_x) + b_y);
        }
    };
public:
    BlankField(int width, int height) {
        width -= 160;
        height -= 160;
        b_start = 55;
        b_finish = 42;
        m_count = (width * height) / 6400;
        test = new Blank*[m_count];
        int x = 80, y = 80;
        for (int i = 0; i < m_count; i++) {
            test[i] = new Blank(x, y);
            
            if (x < width) {
                x += 80;
            }
            else {
                x = 80;
                y += 80;
            }
        }
    }
    
    void Draw(sf::RenderWindow *Window) {
        for (int i = 0; i < m_count; i++) {
            test[i]->Draw(Window);
        }
        B_Finish.Draw(Window);
        B_Start.Draw(Window);
    }
    
    int getCount() { return m_count; }
    
    bool getIsTower(int i) const { if (i < m_count) return test[i]->isTower(); }
    
    Blank* getTower(int i) { if (i < m_count) return test[i]; }
    
    void blankClicked(int x, int y, int st, int *money) {
        x = ((int)x / 80) * 80;
        y = ((int)y / 80) * 80;
        int i = ((x / 80) - 1) + 14 * ((y / 80) - 1);
        if (!test[i]->isTower() && i != b_start && i != b_finish) {
            delete test[i];
            if (st == 0) test[i] = new GunTower(x, y);
            if (st == 1) test[i] = new GunTower2(x, y);
            if(*money < test[i]->getPrice()) {
                delete test[i];
                test[i] = new Blank(x, y);
            } else {
                *money-=test[i]->getPrice();
            }
        }
    }
    
    std::map<int, int> pathCalc(int startBlank) {
        int graph[7][14], t, m = 0, x, y;
        for (int i = 0; i < 7; i++)for (int n = 0; n < 14; n++) graph[i][n] = -1;
        std::priority_queue <int, std::vector<int>, LessPath> gig;
        gig.push(startBlank);
        x = startBlank / 14;
        y = startBlank - startBlank / 14 * 14;
        graph[x][y] = m;
        
        while (!gig.empty()) {
            t = gig.top();
            if (t == 42) break;
            x = t / 14;
            y = t - t / 14 * 14;
            gig.pop();
            m++;
            if (y > 0 && y < 15)if (graph[x][y - 1] < 0 && !test[t - 1]->isTower()) {
                gig.push(t - 1);
                graph[x][y - 1] = m;
            }
            if (y > -1 && y < 13)if (graph[x][y + 1] < 0 && !test[t + 1]->isTower()) {
                gig.push(t + 1);
                graph[x][y + 1] = m;
            }
            if (x > -1 && x < 6)if (graph[x + 1][y] < 0 && !test[t + 14]->isTower()) {
                gig.push(t + 14);
                graph[x + 1][y] = m;
            }
            if (x > 0 && x < 8)if (graph[x - 1][y] < 0 && !test[t - 14]->isTower()) {
                gig.push(t - 14);
                graph[x - 1][y] = m;
            }
        }
        
        //std::cout << startBlank << ": " << std::endl;
        //for (int i = 0; i < 7; i++) {
        //    for (int n = 0; n < 14; n++) {
        //        printf("%2.i ", graph[i][n]);
        //    }
        //    std::cout << std::endl;
        //}
        //std::cout << std::endl;
        
        std::map<int, int> path;
        if(graph[3][0] == -1) {
            return path;
        } else {
            path.insert(std::pair<int, int>(graph[3][0], b_finish));
            std::map<int, int>::const_iterator last = path.cbegin();
            while (true) {
                last = path.begin();
                x = last->second / 14;
                y = last->second - last->second / 14 * 14;
                if (last->second == startBlank) break;
                if (x > -1 && x < 6)if (graph[x + 1][y] < graph[x][y] && graph[x + 1][y] != -1) {
                    path.insert(std::pair<int, int>(graph[x + 1][y], (x + 1) * 14 + y));
                }
                if (x > 0 && x < 8)if (graph[x - 1][y] < graph[x][y] && graph[x - 1][y] != -1) {
                    path.insert(std::pair<int, int>(graph[x - 1][y], (x - 1) * 14 + y));
                }
                if (y > 0 && y < 15)if (graph[x][y - 1] < graph[x][y] && graph[x][y - 1] != -1) {
                    path.insert(std::pair<int, int>(graph[x][y - 1], x * 14 + y - 1));
                }
                if (y > -1 && y < 13)if (graph[x][y + 1] < graph[x][y] && graph[x][y + 1] != -1) {
                    path.insert(std::pair<int, int>(graph[x][y + 1], x * 14 + y + 1));
                }
            }
        //std::cout << std::endl;
        //for (std::map<int, int>::iterator it = path.begin(); it != path.end(); it++) std::cout << it->second << " ";
        //std::cout << std::endl;
        return path;
        }
    }
    
    void towerUpgrade(int x, int y, int *money) {
        int i = ((x / 80) - 1) + 14 * ((y / 80) - 1);
        if(*money >= test[i]->getUpgradePrice() && test[i]->getLevel() < test[i]->getMaxLevel()) {
            *money-=test[i]->getUpgradePrice();
            test[i]->onClick();
        }
    }
    
    void Update(int x, int y) {
        for (int i = 0; i < m_count; i++) {
            test[i]->Update();
        }
    }
    
    bool checkForTower(int i) {
        return test[i]->isTower();
    }
    
    void DeleteTower(int x, int y, int *money) {
        int i = ((x / 80) - 1) + 14 * ((y / 80) - 1);
        *money+=test[i]->getPrice();
        delete test[i];
        test[i] = new Blank(x/80*80, y/80*80);
    }
    
    ~BlankField() {
        for (int i = 0; i < m_count; i++) {
            delete test[i];
        }
        delete[] test;
    }
};

class TowerShop : public GameObject {
private:
    BlankTower **bt;
    int bt_size;
    int selectedTower;
    bool dragged;
    int mouseX;
    int mouseY;
public:
    TowerShop() {
        m_texture.loadFromFile(towershop_1);
        m_sprite.setTexture(m_texture);
        dragged = false;
        m_x = 0;
        m_y = 640;
        selectedTower = 0;
        mouseX = 0;
        mouseY = 0;
        bt_size = 2;
        m_sprite.setPosition((float)m_x, (float)m_y);
        m_sprite.setTextureRect(sf::IntRect(0, 0, 160, 80));
        bt = new BlankTower*[bt_size];
        bt[0] = new GunTower(0, 640);
        bt[1] = new GunTower2(80, 640);
    }
    
    void Draw(sf::RenderWindow *Window) {
        for (int i = 0; i < bt_size; i++) {
            bt[i]->Draw(Window);
        }
        Window->draw(m_sprite);
        
    }
    
    int selectedT() { return selectedTower; }
    
    void SetMouseC(int x, int y) {
        mouseX = x;
        mouseY = y;
    }
    
    void Update() {
        if (dragged) {
            bt[selectedTower]->setX(mouseX - 40);
            bt[selectedTower]->setY(mouseY - 40);
        }
    }
    
    void DragOff() {
        dragged = false;
        bt[selectedTower]->setX(selectedTower * 80);
        bt[selectedTower]->setY(640);
    }
    
    bool getDragged() { return dragged; }
    
    void onClick() {}
    
    void Clicked(int x, int y) {
        if (dragged) {
            dragged = false;
        }
        else {
            dragged = true;
        }
        x = (int)x / 80;
        if (x < bt_size) selectedTower = x;

    }
    
};

#endif /* Towers_h */
