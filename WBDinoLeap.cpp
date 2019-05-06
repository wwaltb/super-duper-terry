#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;

class Player
{
public:
    Player(int);
    Texture texture;
    Sprite sprite;
    int frame = 0, row = 0, fC = 0, mAt, upDown = 0;
    bool moving = false, flying = false, boots = false, boosting = false;
    bool fToggle;
    void update();
    void setFrame();
    void rampey(int);
};

Player::Player(int m)
{
    mAt = m;
    if(!texture.loadFromFile("Terry.png"))
        cout << "yikers" << endl;
    sprite.setTexture(texture);
    sprite.scale(.25f,.25f);
    sprite.setPosition(300,550);
}

void Player::update()
{
    sprite.setTextureRect(IntRect(1024*frame,1024*row,1024,1024));
    int x = sprite.getPosition().x;
    int y = sprite.getPosition().y;
    upDown = 0;
    if(Keyboard::isKeyPressed(Keyboard::Left) and fC % mAt == 0)
        {
            moving = true;
        }
    if(Keyboard::isKeyPressed(Keyboard::Right) and fC % mAt == 0)
        {
            moving = true;
        }
    if(Keyboard::isKeyPressed(Keyboard::Up))
        upDown = 15;
    if(Keyboard::isKeyPressed(Keyboard::Down))
        upDown = -15;
    fC++;
}

void Player::setFrame()
{
    if(moving)
    {
        row = 0;
        if(fC % mAt == 0)
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

void Player::rampey(int distance)
{
    if(distance ){}
}

class Background
{
public:
    Background(int);
    Texture texture;
    Sprite sprite1;
    Sprite sprite2;
    int fC = 0, mAt, mFor = 10, distance = 0, tY = 1080*2.2;
    void update(bool, int);
};

Background::Background(int m)
{
    mAt = m;
    if(!texture.loadFromFile("DinoBG.png"))
        cout << "yikers" << endl;
    sprite1.setTexture(texture);
    sprite1.setPosition(0,0);
    sprite2.setTexture(texture);
    sprite2.setPosition(1920,0);
    sprite1.setTextureRect(IntRect(0,tY,1920,1080));
    sprite2.setTextureRect(IntRect(0,tY,1920,1080));
}

void Background::update(bool moving, int uD)
{
    if(fC % mAt == 0)
    {
        sprite1.setTextureRect(IntRect(0,tY-uD,1920,1080));
        sprite2.setTextureRect(IntRect(0,tY-uD,1920,1080));
    }

    tY = sprite1.getTextureRect().top;
    int x1 = sprite1.getPosition().x;
    int y1 = sprite1.getPosition().y;
    int x2 = sprite2.getPosition().x;
    int y2 = sprite2.getPosition().y;
    /*sprite1.setPosition(x1,y1-uD);
    sprite2.setPosition(x2,y2-uD);
    y1 = sprite1.getPosition().y;
    y2 = sprite2.getPosition().y;*/
    if(moving and fC % mAt == 0)
    {
        if(x1 <= -1920)
        {
            sprite1.setPosition(1920-mFor,y1);
        }
        else
        {
            sprite1.setPosition(x1-mFor,y1);
        }
        if(x2 <= -1920)
        {
            sprite2.setPosition(1920-mFor,y2);
        }
        else
        {
            sprite2.setPosition(x2-mFor,y2);
        }
        distance += mFor;
        //cout << distance << endl;
    }
    fC++;
}



class Ground
{
public:
    Ground(int);
    Texture grass;
    Texture ramp;
    Sprite sprite1;
    Sprite sprite2;
    Sprite sprite3;
    int fC = 0, mAt, mFor = 30, distance = 0;
    void update(bool, int);
};

Ground::Ground(int m)
{
    mAt = m;
    if(!grass.loadFromFile("Ground Pixelated.png"))
        cout << "yikers" << endl;
    if(!ramp.loadFromFile("Ramp Pixelated.png"))
        cout << "yikers" << endl;
    sprite1.setTexture(grass);
    sprite1.setPosition(0,700);
    sprite2.setTexture(ramp);
    sprite2.setPosition(1920,700);
    sprite3.setTexture(ramp);
    sprite3.setPosition(1920*2,700);
}

void Ground::update(bool moving, int uD)
{
    sprite1.setTextureRect(IntRect(0,0,1920,1080));
    sprite2.setTextureRect(IntRect(0,0,1920,1080));
    int x1 = sprite1.getPosition().x;
    int y1 = sprite1.getPosition().y;
    int x2 = sprite2.getPosition().x;
    int y2 = sprite2.getPosition().y;
    int x3 = sprite3.getPosition().x;
    int y3 = sprite3.getPosition().y;
    if(fC % mAt == 0)
    {
        sprite1.setPosition(x1,y1+uD);
        sprite2.setPosition(x2,y2+uD);
        sprite3.setPosition(x3,y3+uD);
        y1 = sprite1.getPosition().y;
        y2 = sprite2.getPosition().y;
        y3 = sprite3.getPosition().y;
    }
    if(moving and fC % mAt == 0)
    {
        /*if(x1 <= -1920)
        {
            sprite1.setPosition(1920-mFor,y1);
        }
        else
        {*/
            sprite1.setPosition(x1-mFor,y1);
        /*}
        if(x2 <= -1920)
        {
            sprite2.setPosition(1920-mFor,y2);
        }
        else
        {*/
            sprite2.setPosition(x2-mFor,y2);
        //}
        sprite3.setPosition(x3-mFor,y3);
        distance += mFor;
        cout << distance << endl;
    }
    fC++;
}

int main()
{
    int initialSpeed = 4;

    Player terry(initialSpeed);

    Background bg(initialSpeed);

    Ground ground(initialSpeed);

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

    terry.setFrame();
    terry.update();
    bg.update(terry.moving, terry.upDown);
    ground.update(terry.moving, terry.upDown);

    window.clear(Color(27,81,99));

    window.draw(bg.sprite1);
    window.draw(bg.sprite2);

    window.draw(ground.sprite1);
    window.draw(ground.sprite2);
    //window.draw(ground.sprite3);

    window.draw(terry.sprite);

    window.display();
    }
}


