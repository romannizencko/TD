#include "Towers.h"
#include "Enemies.h"


void GunTower::Attack(Mob2 &Enemy) {
    if(ShotCooldown.getElapsedTime().asMilliseconds() > t_speedAttack) {
        Enemy.receiveDamage(t_damage);
        ShotCooldown.restart();
    }
    t_angle = std::atan2(Enemy.getY() - getY(), Enemy.getX() - getX()) * 180 / 3.1415;
    
}
