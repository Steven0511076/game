#include "item.hpp"
#include <stdlib.h>
#include <time.h>


item::item()
{
    x = 1+rand()%38;
    y = 1+rand()%23;
}


void item::change_place()
{
    x = rand()%40;
    y = rand()%25;
}
