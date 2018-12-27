#include <SFML/Graphics.hpp>
#include <time.h>
#include <string>
#include <sstream>
#include <iostream>
#include "enemy.hpp"
#include "item.hpp"

using namespace sf;
using namespace std;

const int M = 25; //fix vertical length
const int N = 40; //fix horizontal length

int grid[M][N] = {0}; // initailize
int ts = 18; //tile size

void drop(int y, int x) //recursion for keep enemy's area being black
{
    if (grid[y][x] == 0) grid[y][x] = -1;
    if (grid[y-1][x] == 0) drop(y-1,x);
    if (grid[y+1][x] == 0) drop(y+1,x);
    if (grid[y][x-1] == 0) drop(y,x-1);
    if (grid[y][x+1] == 0) drop(y,x+1);
}

int area() // calculate the area
{
    int deduct = -126;
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            if (grid[i][j] == 1)
                deduct += 1;
    return deduct;
}

string score() // calculate the area and convert it into string
{
    stringstream ss;
    int deduct = -126;
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            if (grid[i][j] == 1)
                deduct += 1;
    int area = deduct;
    ss << area;
    string str_area;
    ss >> str_area;
    return (str_area);
}

int main()
{
    srand(time(0));

    RenderWindow window(VideoMode(N*ts, M*ts),"Final project");
    window.setFramerateLimit(60);

    Texture t1,t2,t3,t4,t5; // load images from file
    t1.loadFromFile("images/tiles.png");
    t2.loadFromFile("images/gameover.png");
    t3.loadFromFile("images/enemy.png");
    t4.loadFromFile("images/item.png");
    t5.loadFromFile("images/win.png");

    Sprite sTile(t1),sGameover(t2),sEnemy(t3),sItem(t4),sWin(t5); //turn images into sprites
    sGameover.setPosition(110,110); //setting the position of gameover
    sEnemy.setOrigin(20,20); //setting the origin position of enemy
    sWin.setPosition(160,90);

    int enemycount = 10; // setting the number of enemy
    enemy a[10];
    int itemcount = 1; // setting the number of item
    item b[3];

    bool Game = true; // boolean if the game exist
    bool Win = false;
    int x = 0, y = 0, dx = 0, dy = 0; //setting the Red point position
    float timer = 0, delay = 0.07; // setting dalaytime for point
    Clock clock; // import the time
    float timer1 = 0, delay1 = 2;
    Clock clock1;

    for (int i = 0; i < M; i++) // setting the bounds into color blue
        for (int j = 0; j < N; j++)
            if (i == 0 || j ==0 || i == M-1 || j == N-1)
                grid[i][j] = 1;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart(); // in order to get the difference of time
        timer += time;

        float time1 = clock1.getElapsedTime().asSeconds(); // in order to get the difference of time
        clock1.restart();
        timer1 += time1;


        Event event;
        while(window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }

            if (event.type == Event::KeyPressed) //setting press the F1 to restart
                if (event.key.code == Keyboard::F1)
                {
                    for (int i = 1; i < M - 1; i++)
                        for (int j = 1; j < N - 1; j++)
                            grid[i][j] = 0;

                    enemycount = 4;
                    x = 0;
                    y = 0;
                    Game = true;
                    Win = false;
                }

            if (event.type == Event::KeyPressed) //setting press the esc to quit
                if (event.key.code == Keyboard::Escape)
                {
                    window.close();
                }
        }
        if (area() >= 874) Win = true;
        if (Keyboard::isKeyPressed(Keyboard::Left)) {dx = -1; dy = 0;}; //move the point
        if (Keyboard::isKeyPressed(Keyboard::Right)) {dx = 1; dy = 0;};
	    if (Keyboard::isKeyPressed(Keyboard::Up)) {dx = 0; dy = -1;};
		if (Keyboard::isKeyPressed(Keyboard::Down)) {dx = 0 ; dy= 1;};

        if (!Game) continue; // if the game is false, then break out the loop

        if (timer1 > delay1) // if time is larger than setting time, items will change its position
        {
            for (int i = 0; i < itemcount; i++)
            {
                do
                {
                    if (area() >= 873) break;
                    b[i].change_place();

                }while(grid[b[i].gety()][b[i].getx()] != 0);
            }
            timer1 = 0;
        }


        if (timer > delay) // if timer larger than setting time, it can move the setting distance
        {
            x += dx;
            y += dy;

            if (x < 0) x = 0; // avoid the target out off the bounds
            if (x > N - 1) x = N -1;
            if (y < 0) y = 0;
            if (y > M - 1) y = M -1;

            if (grid[y][x] == 2) Game = false; //if next step is red, it will bring game over
            if (grid[y][x] == 0) grid[y][x] = 2; // if next step is black, make it red.

            timer = 0; // in order make the motion slow
        }

        for (int i = 0; i < enemycount; i++) // make every enemy move
        {
            a[i].motion_for_x();
            if (grid[a[i].gety()/ts][a[i].getx()/ts] == 1)
            {
                a[i].change_diretion_x(); // if enemy touch the bounds, it will rebound.
                a[i].motion_for_x();
            }
            a[i].motion_for_y();
            if (grid[a[i].gety()/ts][a[i].getx()/ts] == 1)
            {
                a[i].change_diretion_y(); // if enemy touch the bounds, it will rebound.
                a[i].motion_for_y();
            }
        }

        if (grid[y][x] == 1) // if moving to the bounds
        {
            dx = 0; //setting target static
            dy = 0;

            for (int i = 0; i < enemycount; i++) // avoid the enemy become black
                drop(a[i].gety()/ts, a[i].getx()/ts);

            for (int i = 0; i < itemcount; i++) // avoid the item become black
                drop(b[i].gety(), b[i].getx());

            for (int i = 0; i < M; i++) // bound the unlimited area
                for (int j = 0; j < N; j++)
                {
                    if (grid[i][j] == -1) grid[i][j] = 0;
                    else grid[i][j] = 1;
                }
        }

        for (int i = 0; i < enemycount; i++) // if enemy touch the route, game over
            if (grid[a[i].gety()/ts][a[i].getx()/ts] == 2)
                {
                    Game = false;
                }

        for (int i = 0; i < itemcount; i++) // if item touch the route, decrease enemy and change position
            if (grid[b[i].gety()][b[i].getx()] == 2)
                {
                    enemycount--;
                    do
                    {
                        b[i].change_place();
                    }while(grid[b[i].gety()][b[i].getx()] == 1);

                }


        // draw
        window.clear();
        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++)
            {
                if (grid[i][j]==0) continue; // backgound already is black, so nothing to do
                if (grid[i][j]==1) sTile.setTextureRect(IntRect(0, 0, ts, ts)); // other point set to blue
                if (grid[i][j]==2) sTile.setTextureRect(IntRect(54, 0, ts, ts)); // main point set to color red
                sTile.setPosition(j*ts, i*ts);
                window.draw(sTile);
            }

        sTile.setTextureRect(IntRect(36,0,ts,ts)); // the moving point set to color green
        sTile.setPosition(x*ts, y*ts);
        window.draw(sTile);

        sEnemy.rotate(10); //make every enemy leave at different angle
        for (int i = 0; i < enemycount; i++)
        {
            sEnemy.setPosition(a[i].getx(),a[i].gety());
            window.draw(sEnemy);
        }

        for (int i = 0; i < itemcount; i++)
        {
            sItem.setPosition(b[i].getx()*ts,b[i].gety()*ts);
            window.draw(sItem);
        }

        if (!Game && (area() < 874)) window.draw(sGameover); // if lose, show the game over
        if (Win) window.draw(sWin);
        window.display();
    }
    cout<<"score: "<<score()<<"/874";
    return 0;
}
