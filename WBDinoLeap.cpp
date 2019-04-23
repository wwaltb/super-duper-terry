#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;

class Player
{
public:
    Player();
    Texture texture;
    Sprite sprite;
    int frame = 0, row = 0, fC = 0;
    bool moving = false, flying = false, boots = false, boosting = false;
    bool fToggle;
    void update();
    void setFrame(int);
private:

};

Player::Player()
{
    if(!texture.loadFromFile("Terry.png"))
        cout << "yikers" << endl;
    sprite.setTexture(texture);
    sprite.scale(.25f,.25f);
}

void Player::update()
{
    sprite.setTextureRect(IntRect(1024*frame,1024*row,1024,1024));
    int x = sprite.getPosition().x;
    int y = sprite.getPosition().y;
    if(Keyboard::isKeyPressed(Keyboard::Left) and fC % 8 == 0)
        {
            sprite.setPosition(x-20,y);
            moving = true;
        }
    if(Keyboard::isKeyPressed(Keyboard::Right) and fC % 8 == 0)
        {
            sprite.setPosition(x+20,y);
            moving = true;
        }
    if(Keyboard::isKeyPressed(Keyboard::Up))
        sprite.setPosition(x,y-5);
    if(Keyboard::isKeyPressed(Keyboard::Down))
        sprite.setPosition(x,y+5);

}

void Player::setFrame(int fC1)
{
    if(moving)
    {
        row = 0;
        if(fC % 8 == 0)
        {
                 if(frame == 0) frame = 1;
            else if(frame == 1) frame = 2;
            else frame = 0;
        }
    }
    else
    {
        row = 0;
        frame = 0;
    }
}

int main()
{
    Player terry;
    int frameCounter = 0;

    RenderWindow window(VideoMode(1920,1080),"Terry");
    window.setFramerateLimit(60);

    while(window.isOpen())
    {
        Event event;
        while(window.pollEvent(event))
        {
            if(event.type == Event::Closed)
                window.close();
            if(event.type == Event::KeyReleased)
                if(event.key.code == Keyboard::Left or event.key.code == Keyboard::Right)
                terry.moving = false;
        }
    terry.setFrame(frameCounter);
    terry.update();
    window.clear(Color::White);
    window.draw(terry.sprite);
    window.display();
    terry.fC++;
    }


}
