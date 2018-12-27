#include <stdlib.h>
#include "enemy.hpp"


enemy::enemy()
{
    dx = 4 - rand()%8;
    dy = 4 - rand()%8;
}

void enemy::change_diretion_x()
{
    dx = -dx;
}

void enemy::change_diretion_y()
{
    dy = -dy;
}

void enemy::motion_for_x()
{
    x += dx;
}

void enemy::motion_for_y()
{
    y += dy;
}
