#ifndef Enemies_h
#define Enemies_h
#include "Towers.h"

const std::string mob1_1 = "C:\\spr\\mob1.png";
const std::string mob2_1 = "C:\\spr\\mob2.png";
const std::string mob3_1 = "C:\\spr\\mob3.png";

class HealthBar {
private:
    float h_x;
    float h_y;
    float b_hp;
    float h_maxHP;
    float width;
    int height;
    sf::RectangleShape h_greenbar;
    sf::RectangleShape h_redbar;
    sf::RectangleShape h_border;
public:
    HealthBar() {
        h_x = 0;
        h_y = 0;
        b_hp = 100;
        h_maxHP = 100;
        width = 80 * (b_hp/h_maxHP);
        height = 10;
        h_greenbar.setSize(sf::Vector2f(width, height));
        h_redbar.setSize(sf::Vector2f(width, height));
        h_border.setSize(sf::Vector2f(width, height));
        h_greenbar.setPosition(h_x, h_y);
        h_redbar.setPosition(h_x, h_y);
        h_border.setPosition(h_x, h_y);

    }
    HealthBar(int hp, int maxhp, int x, int y): HealthBar() {
        h_x = x;
        h_y = y+80;
        h_greenbar.setPosition(h_x, h_y);
        h_redbar.setPosition(h_x, h_y);
        h_border.setPosition(h_x, h_y);
        h_maxHP = maxhp;
        b_hp = hp;
        width = 80*(b_hp/h_maxHP);
        h_greenbar.setSize(sf::Vector2f(width, height));
        h_greenbar.setFillColor(sf::Color::Green);
        h_redbar.setSize(sf::Vector2f(width, height));
        h_redbar.setFillColor(sf::Color::Red);
        h_border.setSize(sf::Vector2f(width, height));
        h_border.setFillColor(sf::Color::Transparent);
        h_border.setOutlineColor(sf::Color::Black);
        h_border.setOutlineThickness(-1);
    }
    
    void Update(int hp, int x, int y) {
        h_x = x;
        h_y = y+80;
        h_greenbar.setPosition(h_x, h_y);
        h_redbar.setPosition(h_x, h_y);
        h_border.setPosition(h_x, h_y);
        b_hp = hp;
        width = 80 * (b_hp/h_maxHP);
        h_greenbar.setSize(sf::Vector2f(width, height));
        h_greenbar.setFillColor(sf::Color(0, 255, 0, 255*(b_hp/h_maxHP)));
        h_redbar.setSize(sf::Vector2f(width, height));
        
    }
    
    void Draw(sf::RenderWindow *Window) {
        Window->draw(h_redbar);
        Window->draw(h_greenbar);
        Window->draw(h_border);
    }
};

class Mob2 : public GameObject {
protected:
    int e_hp;
    int e_maxhp;
    float e_speed;
    int e_moneyWeight;
    bool e_spawned;
    int e_blankNumber;
    int e_nextBlank;
    std::map<int, int> path;
    HealthBar *e_HPbar;
public:
    Mob2() {
        setX(1120);
        setY(320);
        e_moneyWeight = 7;
        e_spawned = false;
        m_texture.loadFromFile(mob2_1);
        m_sprite.setTexture(m_texture);
        e_hp = e_maxhp = 100;
        e_speed = 5;
        e_blankNumber = 55;
        e_nextBlank = 55;
    }
    Mob2(float k): Mob2() {
        e_hp = e_maxhp = e_hp*k;
        e_moneyWeight = e_moneyWeight*k;
        e_speed = e_speed*(k/2);
        e_HPbar = new HealthBar(e_hp, e_maxhp, getX(), getY());
        
    }
    
    void receiveDamage(int damage) {
        if(e_hp < damage) {
            e_hp = 0;
        } else {
            e_hp-=damage;
        }
    }
    
    void spawn() { e_spawned = true; }
    void despawn(int i, int *GAMER_CH) {
        if(i == 1) *GAMER_CH-=1;
        if(i == 2) *GAMER_CH+=e_moneyWeight;
        e_hp = 0;
        e_spawned = false;
        
    }
    bool isSpawned() { return e_spawned; }
    bool isDead() { return e_hp <= 0; }
    int getHealth() { return e_hp; }
    int getPosition() { return e_blankNumber; }
    int getWeight() { return e_moneyWeight; }
    void setNextBlank(int next) { if(e_blankNumber == e_nextBlank) e_nextBlank = next; }
    int getNextBlank() { return e_nextBlank; }
    bool receivePath(std::map<int, int> path) {
        if(!path.empty()) {
            this->path = path;
            return true;
        } else {
            return false;
        }
    }
    
    int nextBlank(int current) {
        if (!path.empty()) {
            std::map<int, int>::iterator it = path.begin();
            for(;it != path.end(); it++) {
                if(it->second == current) {
                    it++;
                    break;
                }
            }
        if(it != path.end())
            return it->second;
        }
        return current;
    }
    
    void Update() {}
    void Update(float moveTime) {
        if (e_spawned) {
            setNextBlank(nextBlank(e_blankNumber));
            float n_x = (e_nextBlank - e_nextBlank / 14 * 14) * 80 + 80;
            float n_y = e_nextBlank / 14 * 80 + 80;
            if (e_blankNumber - 1 == e_nextBlank) {
                if(getX() > n_x) {
                    setX(getX() - e_speed * moveTime);
                } else if(getX() - e_speed * moveTime <= n_x) {
                    setX(n_x);
                }
            }
            if (e_blankNumber + 1 == e_nextBlank) {
                if(getX() < n_x) {
                    setX(getX() + e_speed * moveTime);
                } else if(getX() + e_speed * moveTime >= n_x) {
                    setX(n_x);
                }
            }
            if (e_blankNumber - 14 == e_nextBlank) {
                if(getY() > n_y) {
                    setY(getY() - e_speed * moveTime);
                } else if(getY() - e_speed * moveTime <= n_y){
                    setY(n_y);
                }
            }
            if (e_blankNumber + 14 == e_nextBlank) {
                if(getY() < n_y) {
                    setY(getY() + e_speed * moveTime);
                } else if(getY() + e_speed * moveTime >= n_y) {
                    setY(n_y);
                }
            }
            m_sprite.setPosition(m_x, m_y);
            e_HPbar->Update(e_hp, getX(), getY());
            int x = getX();
            int y = getY();
            if(x > 1200 || x < 0 || y > 640 || y < 0) {
                e_blankNumber = path.begin()->second;
                setX(n_x);
                setY(n_y);
                setNextBlank(nextBlank(e_blankNumber));
            }
            if ( getX() == n_x && getY() == n_y) e_blankNumber = e_nextBlank;
        }
    }
    
    void onClick() {}
    
    void Draw(sf::RenderWindow *Window) {
        if (e_spawned) {
            Window->draw(m_sprite);
            e_HPbar->Draw(Window);
        }
    }
    
    ~Mob2() {
    }
    
};
class Mob1 :
    public Mob2
{
public:
    Mob1()
    {
        setX(1120);
        setY(320);
        e_moneyWeight = 15;
        e_spawned = false;
        m_texture.loadFromFile(mob1_1);
        m_sprite.setTexture(m_texture);
        e_hp = e_maxhp = 150;
        e_speed = 5;
        e_blankNumber = 55;
        e_nextBlank = 55;
    }

    Mob1(float k) : Mob1()
    {
        e_hp = e_maxhp = e_hp * k;
        e_moneyWeight = e_moneyWeight * k;
        e_speed = e_speed * (k / 2);
        e_HPbar = new HealthBar(e_hp, e_maxhp, getX(), getY());

    }
};
 
class Mob3 :
    public Mob2
{
public:
    Mob3()
    {
        setX(1120);
        setY(320);
        e_moneyWeight = 20;
        e_spawned = false;
        m_texture.loadFromFile(mob3_1);
        m_sprite.setTexture(m_texture);
        e_hp = e_maxhp = 200;
        e_speed = 5;
        e_blankNumber = 55;
        e_nextBlank = 55;
    }

    Mob3(float k) : Mob3()
    {
        e_hp = e_maxhp = e_hp * k;
        e_moneyWeight = e_moneyWeight * k;
        e_speed = e_speed * (k / 2);
        e_HPbar = new HealthBar(e_hp, e_maxhp, getX(), getY());

    }
};

class EnemyWave {
private:
    Mob2 **be;
    int e_count;
    int e_spawnedCount;
    float e_spawnPause;
    sf::Clock e_waveclock;
    sf::Clock e_moveclock;
    
public:
    EnemyWave() {
        e_spawnPause = 8;
        e_count = 10;
        e_spawnedCount = 0;
        e_waveclock.restart();
        e_moveclock.restart();
    }
    EnemyWave(float diff): EnemyWave() {
        e_spawnPause = e_spawnPause*(1/(diff+1));
        e_count = (diff+1)*5;
        be = new Mob2*[e_count];
        for (int i = 0; i < e_count/3; i++)  {

            be[i] = new Mob2(diff+1);
        } 
        for (int i = e_count/3; i < e_count/3*2; i++) {

            be[i] = new Mob1(diff + 1);
        }
        for (int i = e_count/3*2; i < e_count; i++) {

            be[i] = new Mob3(diff + 1);
        }
    }
    
    ~EnemyWave() {
        for (int i = 0; i < e_count; i++) {
            delete be[i];
        }
        delete[] be;
    }
    
    int getSpawnedCount() const { return e_spawnedCount; }
    
    Mob2* getEnemy(int i) const {
        if(i < e_count) {
            if(be[i]->isSpawned()) {
                return be[i];
            }
        }
    }
    
    bool isMobSpawned(int i) { return be[i]->isSpawned(); }
    bool isWaveEnded() {
        for(int i = 0; i < e_count; i++) {
            if(!be[i]->isDead()) return false;
        }
        return true;
    }
    
    bool setPathForMobs (BlankField *Towers) {
        for(int i = 0; i < e_count; i++) { 
            if(Towers->pathCalc(be[i]->getNextBlank()).empty()) {
                return false;
            } else {
                be[i]->receivePath(Towers->pathCalc(be[i]->getNextBlank()));
            }
        }
        return true;
    }
    
    void Update(int *GAMER_HP, int *GAMER_MONEY) {
        if (e_waveclock.getElapsedTime().asSeconds() > e_spawnPause && e_spawnedCount < e_count) { //spawn of all mobs
            e_waveclock.restart();
            be[e_spawnedCount]->spawn();
            e_spawnedCount++;
        }
        
        if (e_moveclock.getElapsedTime().asMilliseconds() > 10) { //moving of all mobs
            for (int i = 0; i < e_count; i++) {
                if (be[i]->getPosition() == 42) {
                    if(be[i]->isSpawned()) be[i]->despawn(1, GAMER_HP); //finishing of path
                    continue;
                }
                if (be[i]->getHealth() < 1) {
                    if(be[i]->isSpawned()) be[i]->despawn(2, GAMER_MONEY); //killing
                    continue;
                }
                be[i]->Update(e_moveclock.getElapsedTime().asMilliseconds());
                
            }
            e_moveclock.restart();
        }
    }
    
    void Draw(sf::RenderWindow *Window) {
        for (int i = 0; i < e_count; i++) be[i]->Draw(Window);
    }
    
    void zeroCounter() {
        e_moveclock.restart();
    }
    void zeroWaveCounter() {
        e_waveclock.restart();
    }
};
#endif /* Enemies_h */
