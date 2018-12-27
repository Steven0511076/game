#ifndef ENEMY_H
#define ENEMY_H

#include "object.hpp"

class enemy: public object{

public:
    enemy();
    void change_diretion_x();
    void change_diretion_y();
    void motion_for_x();
    void motion_for_y();

protected:
    int dx;
    int dy;
};


#endif // ENEMY_H
