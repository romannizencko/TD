#ifndef App_h
#define App_h

#include "Level.h"

class GameEngine {
private:
    sf::RenderWindow Window;
    Menu menu;
    
    
    void input(sf::Event event, float *MouseTime) {
        if (*MouseTime > 0.1) {
            menu.Input(event, MouseTime);
        }
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) { Window.close(); }
        
    }
    
    void draw() {
        Window.clear(sf::Color::Black);
        menu.Draw(&Window);
        Window.display();
    }
    
    void update(int x, int y) {
        menu.Update(x, y);
    }
    
public:
    GameEngine() {
        Window.create(sf::VideoMode(1280, 720), "TowerDefence ver. Beta 1", sf::Style::Close);
        
    }
    
    ~GameEngine() {
    }
    
    void Run() {
        sf::Clock clock;
        float MouseTime = 0;
        float dtAsSeconds;
        sf::Time dt;
        sf::Vector2i pixelPos;
        while (Window.isOpen()) {
            sf::Event event;
            while (Window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    Window.close();
            }
            dt = clock.restart();
            dtAsSeconds = dt.asSeconds();
            MouseTime += dtAsSeconds;
            pixelPos = sf::Mouse::getPosition(Window);
            input(event, &MouseTime);
            update(pixelPos.x, pixelPos.y);
            draw();
        }
    }
};


#endif /* App_h */
