#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>

using namespace std;
using namespace sf;

class Player
{
public:
    Player(int);
    Texture texture;
    Sprite sprite;
    int frame = 0, row = 0, fC = 0, mAt, rotation = 0;
    int uDCount = 0, rotCount = 0;
    double upDown = 0;
    bool moving = false, flying = false, boots = false, boosting = false, ramp = false;
    ifstream hFile;
    ifstream rFile;
    bool fToggle;
    void update();
    void setFrame();
    void rampey(int);
    void startFlying();
};

Player::Player(int m)
{
    mAt = m;
    if(!texture.loadFromFile("Terry.png"))
        cout << "yikers" << endl;
    sprite.setTexture(texture);
    sprite.scale(.25f,.25f);
    sprite.setOrigin(592,592);
    sprite.setPosition(300+592/5,560+592/4);
    hFile.open("height.txt");
    rFile.open("rotation.txt");
}

void Player::update()
{
    sprite.setTextureRect(IntRect(1024*frame,1024*row,1024,1024));
    int x = sprite.getPosition().x;
    int y = sprite.getPosition().y;
    //upDown = 0;

    if(!flying)
    {
        upDown = 0;
        if(Keyboard::isKeyPressed(Keyboard::Right) and fC % mAt == 0)
        {
            moving = true;
            /*cout << "uD: " << uDCount << endl;
            uDCount = 0;
            cout << "rot: " << rotCount << endl;
            rotCount = 0;*/
        }
    }
    if(flying)
    {
        upDown -= 0.5;
        //cout << upDown << endl;
        if(Keyboard::isKeyPressed(Keyboard::Right))
        {
            sprite.rotate(1);
        }
        if(Keyboard::isKeyPressed(Keyboard::Left))
        {
            sprite.rotate(-1);
        }
    }
    /*if(Keyboard::isKeyPressed(Keyboard::Up) and fC % mAt == 0)
        {upDown = 5; uDCount += upDown;}
    if(Keyboard::isKeyPressed(Keyboard::Down) and fC % mAt == 0)
        {upDown = -5; uDCount += upDown;}
    if(Keyboard::isKeyPressed(Keyboard::A) and fC % mAt == 0)
        {sprite.rotate(-1); rotCount += -1;}
    if(Keyboard::isKeyPressed(Keyboard::D) and fC % mAt == 0)
        {sprite.rotate(1); rotCount += 1;}*/
    fC++;
}

void Player::setFrame()
{
    if(!boots)
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
        if(flying)
        {
            row = 1;
            frame = 0;
        }
        if(!flying and !moving)
        {
            row = 0;
            frame = 0;
        }
    }
}

void Player::rampey(int distance)
{
    hFile >> upDown;
    cout << upDown << endl;
    rFile >> rotation;
    cout << rotation << endl;
    sprite.rotate(rotation);
    if (distance < 2200)
        upDown -= 1;
    else upDown += 1;
}

void Player::startFlying()
{
    flying = true;
    moving = false;
    upDown = 30;
}

class Background
{
public:
    Background(int);
    Texture texture;
    Sprite sprite1;
    Sprite sprite2;
    int fC = 0, mAt, mFor = 10, distance = 0, tY = 1080*2.2;
    void update(bool, int, Player &terry);
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

void Background::update(bool moving, int uD, Player &terry)
{
    if(terry.flying)
    {
        sprite1.setTextureRect(IntRect(0,tY-uD,1920,1080));
        sprite2.setTextureRect(IntRect(0,tY-uD,1920,1080));
    }
    else if(fC % mAt == 0 and moving)
    {
        sprite1.setTextureRect(IntRect(0,tY-uD,1920,1080));
        sprite2.setTextureRect(IntRect(0,tY-uD,1920,1080));
    }

    tY = sprite1.getTextureRect().top;
    int x1 = sprite1.getPosition().x;
    int y1 = sprite1.getPosition().y;
    int x2 = sprite2.getPosition().x;
    int y2 = sprite2.getPosition().y;

    if(moving and fC % mAt == 0 or terry.flying)
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
    }
    fC++;
}

class Water
{
public:
    Water(int);
    Texture texture;
    Sprite sprite1;
    Sprite sprite2;
    int fC = 0, mAt, mFor = 30, distance = 0;
    void update(bool, int, Player &terry);
};

Water::Water(int m)
{
    mAt = m;
    if(!texture.loadFromFile("Dino Water.png"))
        cout << "yikers" << endl;
    sprite1.setTexture(texture);
    sprite1.setPosition(0,1600);
    sprite2.setTexture(texture);
    sprite2.setPosition(1920,1600);
    sprite1.setTextureRect(IntRect(0,0,1920,1080));
    sprite2.setTextureRect(IntRect(0,0,1920,1080));
}

void Water::update(bool moving, int uD, Player &terry)
{
    int x1 = sprite1.getPosition().x;
    int y1 = sprite1.getPosition().y;
    int x2 = sprite2.getPosition().x;
    int y2 = sprite2.getPosition().y;

    if(moving and fC % mAt == 0 or terry.flying)
    {
        if(x1 <= -1920)
        {
            sprite1.setPosition(1920-mFor,y1+uD);
        }
        else
        {
            sprite1.setPosition(x1-mFor,y1+uD);
        }
        if(x2 <= -1920)
        {
            sprite2.setPosition(1920-mFor,y2+uD);
        }
        else
        {
            sprite2.setPosition(x2-mFor,y2+uD);
        }
        distance += mFor;
    }
    fC++;
}


class Ground
{
public:
    Ground(int);
    Texture grass;
    Texture ramp;
    Texture water;
    Sprite sprite1;
    Sprite sprite2;
    Sprite sprite3;
    Sprite sprite4;
    int fC = 0, mAt, mFor = 30, distance = 0;
    void update(bool, int, Player &terry);
};

Ground::Ground(int m)
{
    mAt = m;
    if(!grass.loadFromFile("Ground Pixelated.png"))
        cout << "yikers" << endl;
    if(!ramp.loadFromFile("Ramp Pixelated.png"))
        cout << "yikers" << endl;
    if(!water.loadFromFile("Dino Water.png"))
        cout << "yikers" << endl;
    sprite1.setTexture(grass);
    sprite1.setPosition(0,700);
    sprite2.setTexture(ramp);
    sprite2.setPosition(1920,700);
    sprite3.setTexture(ramp);
    sprite3.setPosition(1920*2,700);
    sprite4.setTexture(water);
    sprite4.setPosition(1920,1700);
}

void Ground::update(bool moving, int uD, Player &terry)
{
    if(distance == 3360)
    {
        terry.startFlying();
    }
    cout << sprite4.getPosition().y << endl;
    if(sprite4.getPosition().y < 400)
        terry.flying = false;
    sprite1.setTextureRect(IntRect(0,0,1920,1080));
    sprite2.setTextureRect(IntRect(0,0,1920,1080));
    sprite4.setTextureRect(IntRect(0,0,1920,1080));
    int x1 = sprite1.getPosition().x;
    int y1 = sprite1.getPosition().y;
    int x2 = sprite2.getPosition().x;
    int y2 = sprite2.getPosition().y;
    int x3 = sprite3.getPosition().x;
    int y3 = sprite3.getPosition().y;
    int x4 = sprite4.getPosition().x;
    int y4 = sprite4.getPosition().y;
    if(moving)
        {
            if(distance >= 1530 and distance <= 3330)
            {
                //terry.rampey(distance);
                terry.ramp = true;
            }
            else{terry.ramp = false;}
        }
    if(fC % mAt == 0 and moving or terry.flying)
    {
            if(terry.ramp) terry.rampey(distance);
            sprite1.setPosition(x1-mFor,y1+terry.upDown);
            sprite2.setPosition(x2-mFor,y2+terry.upDown);
            sprite3.setPosition(x3-mFor,y3+terry.upDown);
            sprite4.setPosition(x4-mFor,y4+terry.upDown);
            distance += mFor;
                cout << distance << endl;
    }
    fC++;
}

int main()
{
    int initialSpeed = 3;

    Player terry(initialSpeed);

    Background bg(initialSpeed);

    Ground ground(initialSpeed);

    Water water(initialSpeed);

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
                if(event.key.code == Keyboard::Right)
                terry.moving = false;
        }

    terry.setFrame();
    terry.update();
    ground.update(terry.moving, terry.upDown, terry);
    bg.update(terry.moving, terry.upDown, terry);
    water.update(terry.moving, terry.upDown, terry);

    window.clear(Color(27,81,99));

    window.draw(bg.sprite1);
    window.draw(bg.sprite2);

    if(ground.distance <= 4000)
    {
        window.draw(water.sprite1);
        window.draw(water.sprite2);
    }


    window.draw(ground.sprite1);
    window.draw(ground.sprite2);
    window.draw(ground.sprite4);
    //window.draw(ground.sprite3);

    window.draw(terry.sprite);

    if(ground.distance > 4000)
    {
        window.draw(water.sprite1);
        window.draw(water.sprite2);
    }

    window.display();
    }
}

