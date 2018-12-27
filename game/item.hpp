#ifndef ITEM_H
#define ITEM_H

#include "enemy.hpp"

class item: public enemy{

public:
    item();
    void change_place();

protected:
    float timer;
};


#endif // ITEM_H
