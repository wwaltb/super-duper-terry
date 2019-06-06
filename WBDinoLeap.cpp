#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
using namespace sf;

class Player
{
public:
    Player(int);
    Texture texture;
    Sprite sprite;
    int frame, row, fC, mAt, rotation, changeAt, changeF, money;
    int uDCount, rotCount;
    float lastRot, currentRot;
    double upDown, gravity, bootsEfficiency, fuelMax, fuelCurrent, glideFactor;
    double glideVelo, totalUpDown, glideGrav, maxGlide;
    bool moving, flying, boots, wings, boosting, ramp, flew, gliding;
    ifstream hFile;
    ifstream rFile;
    bool fToggle;
    void update();
    void setFrame();
    void rampey(int);
    void startFlying();
    void useBoots();
    void startRun();
};

Player::Player(int m)
{
    mAt = m;
    frame = 0; row = 0; fC = 0; rotation = 0; uDCount = 0; rotCount = 0;
    upDown = 0; money = 0, changeAt = 0, changeF = 0;
    gravity = 0.3f;
    fuelMax = 0;
    fuelCurrent = fuelMax;
    maxGlide = 0;
    moving = 0; flying = 0; boots = 0; wings = 0; boosting = 0; ramp = 0; flew = 0, gliding = 0;
    texture.loadFromFile("Terry.png");
    sprite.setTexture(texture);
    sprite.setScale(.25,.25);
    sprite.setOrigin(592,592);
    sprite.setPosition(300+592/5,560+592/4);
    hFile.open("height.txt");
    rFile.open("rotation.txt");
}

void Player::startRun()
{
    moving = 0; flying = 0; boosting = 0; ramp = 0; flew = 0, gliding = 0;
    upDown = 0; fC = 0; sprite.setRotation(0); totalUpDown = 0;
    fuelCurrent = fuelMax;
    hFile.close();
    rFile.close();
    hFile.open("height.txt");
    rFile.open("rotation.txt");
}

void Player::update()
{
    sprite.setTextureRect(IntRect(1024*frame,1024*row,1024,1024));
    int x = sprite.getPosition().x;
    int y = sprite.getPosition().y;

    if(!flew)
    {
        upDown = 0;
        if(Keyboard::isKeyPressed(Keyboard::Right) and fC % mAt == 0)
        {
            moving = true;
        }
    }
    if(flying)
    {
        bool wasGliding;

        if(gliding)
            wasGliding = true;
        else
            wasGliding = false;

        lastRot = sprite.getRotation();
        currentRot = sprite.getRotation();

        if(Keyboard::isKeyPressed(Keyboard::Left))
        {
            sprite.rotate(-1);
            if(currentRot > 30 and currentRot < 90)
                gliding = true;
            else if(wasGliding and (currentRot <= 45 or currentRot >= 300))
                gliding = true;
            else gliding = false;
        }
        /*else if(wasGliding and (currentRot <= 45 or currentRot >= 300))
            gliding = true;*/
        else
            gliding = false;
        if(Keyboard::isKeyPressed(Keyboard::Space) and fuelCurrent > 0 and boots)
        {
            boosting = true;
            useBoots();
        }
        else boosting = false;

        if(Keyboard::isKeyPressed(Keyboard::Right))
        {
            sprite.rotate(1);
            gliding = false;
        }

        if(!wasGliding and upDown > 0)
            gliding = false;
        currentRot = sprite.getRotation();

        if(!wasGliding and gliding)
        {
            glideVelo = upDown;
        }

        if(gliding and wings)
        {
            glideGrav += gravity/(2+glideFactor);
            upDown = glideVelo*sin(currentRot*3.141592635/180) - glideGrav;
        }
        else
        {
            maxGlide = 0;
            glideGrav = 0;
            upDown -= gravity;
        }

    }
    totalUpDown += upDown;
    if(totalUpDown <= -600)
    {
        flying = false;
    }
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
    if(boots)
    {
        if(moving)
        {
            if(fC % mAt == 0)
            {
                     if(frame == 1) {frame = 2; row = 1;}
                else if(frame == 2) {frame = 0; row = 2;}
                else {frame = 1; row = 1;}
            }
        }
        if(boosting)
        {
            if(frame == 2) {frame = 0; row = 3;}
                     else {frame = 2; row = 2;}
        }
        else if(flying)
        {
            row = 2;
            frame = 1;
        }
        if(!flying and !moving)
        {
            row = 1;
            frame = 1;
        }
    }
}

void Player::rampey(int distance)
{
    hFile >> upDown;
    rFile >> rotation;
    sprite.rotate(rotation);
    if (distance < 2200)
        upDown -= 1;
    else upDown += 1;
}

void Player::startFlying()
{
    flew = true;
    flying = true;
    moving = false;
    sprite.setRotation(-45);
    double rot = sprite.getRotation()*(3.14159265/180);
    upDown = 20+sin(-rot)*(24-(mAt*3));
    changeF = cos(-rot)*(24-mAt*3);

}

void Player::useBoots()
{
    double rot = sprite.getRotation()*(3.14159265/180);
    if(fuelCurrent > 0)
    {
        upDown += 0.9*sin(-rot);
        changeF = 0.9*cos(-rot);
    }
    fuelCurrent -= bootsEfficiency;
}

class Background
{
public:
    Background(int);
    Texture texture;
    Sprite sprite1;
    Sprite sprite2;
    int fC = 0, mAt, dFor = 10, mFor = 10, distance = 0, tY = 1080*2.2;
    void update(bool, int, Player &terry);
    void startRun();
};

Background::Background(int m)
{
    mAt = m;
    texture.loadFromFile("DinoBG.png");
    sprite1.setTexture(texture);
    sprite1.setPosition(0,0);
    sprite2.setTexture(texture);
    sprite2.setPosition(1920,0);
    sprite1.setTextureRect(IntRect(0,tY,1920,1080));
    sprite2.setTextureRect(IntRect(0,tY,1920,1080));
}

void Background::startRun()
{
    tY = 1080*2.2;
    sprite1.setPosition(0,0);
    sprite2.setPosition(1920,0);
    sprite1.setTextureRect(IntRect(0,tY,1920,1080));
    sprite2.setTextureRect(IntRect(0,tY,1920,1080));
    mFor = 10; distance = 0; fC = 0;
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
            sprite1.setPosition(x1-mFor+3840,y1);
        }
        else
        {
            sprite1.setPosition(x1-mFor,y1);
        }
        if(x2 <= -1920)
        {
            sprite2.setPosition(x2-mFor+3840,y2);
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
    int fC = 0, mAt, dFor = 30, mFor = 30, distance = 0;
    void update(bool, int, Player &terry);
    void startRun();
};

Water::Water(int m)
{
    mAt = m;
    texture.loadFromFile("Dino Water.png");
    sprite1.setTexture(texture);
    sprite1.setPosition(0,1600);
    sprite2.setTexture(texture);
    sprite2.setPosition(1920,1600);
    sprite1.setTextureRect(IntRect(0,0,1920,1080));
    sprite2.setTextureRect(IntRect(0,0,1920,1080));
}

void Water::startRun()
{
    sprite1.setPosition(0,1600);
    sprite2.setPosition(1920,1600);
    mFor = 30; distance = 0; fC = 0;
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
            sprite1.setPosition(x1-mFor+3840,y1+uD);
        }
        else
        {
            sprite1.setPosition(x1-mFor,y1+uD);
        }
        if(x2 <= -1920)
        {
            sprite2.setPosition(x2-mFor+3840,y2+uD);
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
    int fC = 0, mAt, dFor = 30, mFor = 30, distance = 0;
    void update(bool, int, Player &terry);
    void startRun();
};

Ground::Ground(int m)
{
    mAt = m;
    grass.loadFromFile("Ground Pixelated.png");
    ramp.loadFromFile("Ramp Pixelated.png");
    water.loadFromFile("Dino Water.png");
    sprite1.setTexture(grass);
    sprite1.setPosition(0,700);
    sprite2.setTexture(ramp);
    sprite2.setPosition(1920,700);
    sprite3.setTexture(ramp);
    sprite3.setPosition(1920*2,700);
    sprite4.setTexture(water);
    sprite4.setPosition(1920,1700);
}

void Ground::startRun()
{
    sprite1.setPosition(0,700);
    sprite2.setPosition(1920,700);
    sprite3.setPosition(1920*2,700);
    sprite4.setPosition(1920,1700);
    mFor = 30; distance = 0; fC = 0;
}

void Ground::update(bool moving, int uD, Player &terry)
{
    if(distance == 3360)
    {
        terry.startFlying();
    }
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

    if(distance == 2010 and fC % mAt == 0)
        terry.changeAt = mAt - 1;

    if(distance == 2940 and fC % mAt == 0)
        terry.changeAt = mAt - 1;

    if(moving)
        {
            if(distance >= 1530 and distance <= 3330)
            {
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
    }
    fC++;
}

class Stars
{
public:
    Stars(int, int, int, Texture, Texture, Texture, Texture);
    Texture texture0;
    Texture texture1;
    Texture texture2;
    Texture texture3;
    Sprite sprite1;
    Sprite sprite2;
    int fC = 0, mAt, dFor = 10, mFor = 10, distance = 0, starY, offset, frame = 0;
    void update(bool, int, Player &terry);
    void startRun();
};

Stars::Stars(int m, int y, int x, Texture t0, Texture t1, Texture t2, Texture t3)
{
    mAt = m;
    starY = y;
    offset = x;
    texture0 = t0;
    texture1 = t1;
    texture2 = t2;
    texture3 = t3;
    sprite1.setTexture(texture0);
    sprite1.setPosition(0+offset,starY);
    sprite2.setTexture(texture0);
    sprite2.setPosition(1920+offset,starY);
    sprite1.setTextureRect(IntRect(0,0,1920,1080));
    sprite2.setTextureRect(IntRect(0,0,1920,1080));
}

void Stars::startRun()
{
    sprite1.setPosition(0+offset,starY);
    sprite2.setPosition(1920+offset,starY);
    mFor = 10; distance = 0; fC = 0;
}

void Stars::update(bool moving, int uD, Player &terry)
{
    int x1 = sprite1.getPosition().x;
    int y1 = sprite1.getPosition().y;
    int x2 = sprite2.getPosition().x;
    int y2 = sprite2.getPosition().y;

    if(fC % 20 == 0)
    {
        switch(frame)
        {
        case 0 :
            sprite1.setTexture(texture1);
            sprite2.setTexture(texture1);
            frame = 1;
            break;
        case 1 :
            sprite1.setTexture(texture2);
            sprite2.setTexture(texture2);
            frame = 2;
            break;
        case 2 :
            sprite1.setTexture(texture3);
            sprite2.setTexture(texture3);
            frame = 3;
            break;
        case 3 :
            sprite1.setTexture(texture0);
            sprite2.setTexture(texture0);
            frame = 0;
            break;
        default :
            break;
        }
    }


    if(moving and fC % mAt == 0 or terry.flying)
    {
        if(x1 <= -1920)
        {
            sprite1.setPosition(x1-mFor+3840,y1+uD);
        }
        else
        {
            sprite1.setPosition(x1-mFor,y1+uD);
        }
        if(x2 <= -1920)
        {
            sprite2.setPosition(x2-mFor+3840,y2+uD);
        }
        else
        {
            sprite2.setPosition(x2-mFor,y2+uD);
        }
        distance += mFor;
    }
    fC++;
}

class shopButton
{
public:
    shopButton(int, string, int, int);
    int whichButton;
    int xPos, yPos;
    int speed, boots, fuel, wings;
    int frame;
    bool leftButtonDown = false;
    Sprite button;
    Sprite progression;
    Texture texture;
    Texture texture2;
    void update(Player &terry);
    void speedButton(Player &terry);
    void bootsButton(Player &terry);
    void fuelButton(Player &terry);
    void wingsButton(Player &terry);
};

shopButton::shopButton(int i, string n, int x, int y)
{
    whichButton = i;
    xPos = x; yPos = y;
    speed = 0; boots = 0; fuel = 0; wings = 0;
    frame = 0;
    texture.loadFromFile(n);
    texture2.loadFromFile("Shop Bar.png");
    button.setTexture(texture);
    button.setOrigin(210,46.5);
    button.setPosition(xPos + 210,yPos + 46.5);
    progression.setTexture(texture2);
    progression.setTextureRect(IntRect(0,70*frame,420,70));
    progression.setPosition(xPos,yPos + 77);
}

void shopButton::update(Player &terry)
{
    Mouse mouse;
    int mX, mY;
    mX = mouse.getPosition().x;
    mY = mouse.getPosition().y;
    progression.setTextureRect(IntRect(0,70*frame,420,70));
    if(button.getGlobalBounds().contains(mX,mY))
    {
        //Hover state:
        button.setScale(1.05,1.05);

        //If button pressed:
        if(mouse.isButtonPressed(Mouse::Left) and !leftButtonDown)
        {
            leftButtonDown = true;
            switch(whichButton)
            {
                case 1 :
                    speedButton(terry);
                    break;
                case 2 :
                    bootsButton(terry);
                    break;
                case 3 :
                    fuelButton(terry);
                    break;
                case 4 :
                    wingsButton(terry);
                    break;
                default :
                    break;
            }
        }
    }
    else
    {
        //Non-hover state:
        button.setScale(1.f,1.f);
    }
}

void shopButton::speedButton(Player &terry)
{
    int speed0 = 1000, speed1 = 2000, speed2 = 5000, speed3 = 10000;

    switch(speed)
    {
        case 0 :
            if(terry.money > speed0)
            {
                speed++;
                frame++;
                terry.money -= speed0;
                texture.loadFromFile("Speed 1.png");
            }
            break;
        case 1 :
            if(terry.money > speed1)
            {
                speed++;
                frame++;
                terry.money -= speed1;
                texture.loadFromFile("Speed 2.png");
            }
            break;
        case 2 :
            if(terry.money > speed2)
            {
                speed++;
                frame++;
                terry.money -= speed2;
                texture.loadFromFile("Speed 3.png");
            }
            break;
        case 3 :
            if(terry.money > speed3)
            {
                speed++;
                frame++;
                terry.money -= speed3;
                texture.loadFromFile("Speed.png");
            }
            break;
        default :
            break;
    }

}
void shopButton::bootsButton(Player &terry)
{
    int boots0 = 5000, boots1 = 7500, boots2 = 12500, boots3 = 20000;

    switch(boots)
    {
        case 0 :
            if(terry.money > boots0)
            {
                boots++;
                frame++;
                terry.money -= boots0;
                terry.boots = true;
                texture.loadFromFile("Boots 1.png");
            }
            break;
        case 1 :
            if(terry.money > boots1)
            {
                boots++;
                frame++;
                terry.money -= boots1;
                texture.loadFromFile("Boots 2.png");
            }
            break;
        case 2 :
            if(terry.money > boots2)
            {
                boots++;
                frame++;
                terry.money -= boots2;
                texture.loadFromFile("Boots 3.png");
            }
            break;
        case 3 :
            if(terry.money > boots3)
            {
                boots++;
                frame++;
                terry.money -= boots3;
                texture.loadFromFile("Boots.png");
            }
            break;
        default :
            break;
    }
}
void shopButton::fuelButton(Player &terry)
{
    int fuel0 = 500, fuel1 = 1000, fuel2 = 1500, fuel3 = 2500;

    switch(fuel)
    {
        case 0 :
            if(terry.money > fuel0)
            {
                fuel++;
                frame++;
                terry.money -= fuel0;
                texture.loadFromFile("Fuel 1.png");
            }
            break;
        case 1 :
            if(terry.money > fuel1)
            {
                fuel++;
                frame++;
                terry.money -= fuel1;
                texture.loadFromFile("Fuel 2.png");
            }
            break;
        case 2 :
            if(terry.money > fuel2)
            {
                fuel++;
                frame++;
                terry.money -= fuel2;
                texture.loadFromFile("Fuel 3.png");
            }
            break;
        case 3 :
            if(terry.money > fuel3)
            {
                fuel++;
                frame++;
                terry.money -= fuel3;
                texture.loadFromFile("Fuel.png");
            }
            break;
        default :
            break;
    }
}
void shopButton::wingsButton(Player &terry)
{
    int wings0 = 10000, wings1 = 15000, wings2 = 20000, wings3 = 25000;

    switch(wings)
    {
        case 0 :
            if(terry.money > wings0)
            {
                wings++;
                frame++;
                terry.money -= wings0;
                terry.wings = true;
                terry.texture.loadFromFile("Terry Wings.png");
                texture.loadFromFile("Wings 1.png");
            }
            break;
        case 1 :
            if(terry.money > wings1)
            {
                wings++;
                frame++;
                terry.money -= wings1;
                texture.loadFromFile("Wings 2.png");
            }
            break;
        case 2 :
            if(terry.money > wings2)
            {
                wings++;
                frame++;
                terry.money -= wings2;
                texture.loadFromFile("Wings 3.png");
            }
            break;
        case 3 :
            if(terry.money > wings3)
            {
                wings++;
                frame++;
                terry.money -= wings3;
                texture.loadFromFile("Wings.png");
            }
            break;
        default :
            break;
    }
}

class Arrow
{
public:
    Arrow(int, int);
    Sprite arrow;
    Texture texture;
    bool update();
};

Arrow::Arrow(int x, int y)
{
    texture.loadFromFile("Arrow.png");
    arrow.setTexture(texture);
    arrow.setOrigin(100,100);
    arrow.setScale(0.35,0.35);
    arrow.setPosition(x+50,y+50);
}

bool Arrow::update()
{
    bool clicked = false;
    Mouse mouse;
    int mX, mY;
    mX = mouse.getPosition().x;
    mY = mouse.getPosition().y;
    if(arrow.getGlobalBounds().contains(mX,mY))
    {
        //Hover state:
        arrow.setScale(0.4,0.4);

        //If button pressed:
        if(mouse.isButtonPressed(Mouse::Left))
        {
            clicked = true;
        }
    }
    else
    {
        arrow.setScale(0.35,0.35);
    }
    return clicked;
}


void changeFrame(int, Player &terry, Background &bg, Ground &ground, Water &water, vector<Stars> &starVec);
void changeFor(int, Background &bg, Ground &ground, Water &water, vector<Stars> &starVec);
string num2str(double, bool, bool);

int main()
{
    //Creating the Game Window:
    RenderWindow window(VideoMode(1920,1080),"Terry", Style::Fullscreen);
    window.setFramerateLimit(60);
    window.clear(Color::Black);
    window.display();

    int initialSpeed = 8;

    int currentScore = 0;
    int highScore = 0;

    int idleFC = 0;
    bool idleFrame = 0;

    int titleFC = 0;

    //Creating the Game Objects:
    Player terry(initialSpeed);
    Background bg(initialSpeed);
    Ground ground(initialSpeed);
    Water water(initialSpeed);

    //Star Texures:
    Texture texture0;
    Texture texture1;
    Texture texture2;
    Texture texture3;

    texture0.loadFromFile("Stars 0.png");
    texture1.loadFromFile("Stars 1.png");
    texture2.loadFromFile("Stars 2.png");
    texture3.loadFromFile("Stars 3.png");

    //Creating a vector of stars:
    vector <Stars> starVec;
    for(int i = 1; i < 40; i++)
    {
        int starY = -3000 -1000*i;
        int offset = 100*i;
        starVec.push_back(Stars(initialSpeed, starY, offset, texture0, texture1, texture2, texture3));
    }

    //Creating the Shop Buttons:
    shopButton shopB1(1, "Speed 0.png", 378-21*2, 309-14*2);
    shopButton shopB2(2, "Boots 0.png", 378-21*2, 444-14*2);
    shopButton shopB3(3, "Fuel 0.png",  378-21*2, 579-14*2);
    shopButton shopB4(4, "Wings 0.png", 378-21*2, 719-14*2);
    Arrow arrow(1495, 772);

    Arrow dArrow(1353, 744);

    Font font;

    //Creating Textures:
    Texture fuel;
    Texture dScreenText;
    Texture tIdle;
    Texture tIdleW;
    Texture bgTxtr;
    Texture titleBG;
    Texture logo;
    Texture titleTer;

    //Loading Textures:
    font.loadFromFile("DroidBB.ttf");
    fuel.loadFromFile("Fuel Gage (3).png");
    dScreenText.loadFromFile("Death Screen.png");
    tIdle.loadFromFile("Terry Idle.png");
    tIdleW.loadFromFile("Terry Idle Wings.png");
    bgTxtr.loadFromFile("Dino Store.png");
    titleBG.loadFromFile("Dino Start BG.png");
    logo.loadFromFile("Dino Leap Logo.png");
    titleTer.loadFromFile("Terry.png");

    //Creating the Fuel Gage:
    Sprite fuelGage;
    fuelGage.setTexture(fuel);
    fuelGage.setTextureRect(IntRect(0,0,192,180));
    fuelGage.setScale(.5,.5);
    fuelGage.setPosition(59,916);

    //Creating the Death Screen:
    Sprite dScreen;
    dScreen.setTexture(dScreenText);
    dScreen.setPosition(452,206);

    //Creating the Title Screen Background:
    Sprite titleBG1;
    Sprite titleBG2;
    titleBG1.setTexture(titleBG);
    titleBG2.setTexture(titleBG);
    titleBG2.setPosition(1920,0);

    //Creating the Title Logo:
    Sprite titleLogo;
    titleLogo.setTexture(logo);
    titleLogo.setOrigin(384,192);
    titleLogo.setScale(1.4,1.4);
    titleLogo.setPosition(1920/2,200);

    //Creating the Title Terry:
    Sprite titleTerry;
    titleTerry.setTexture(titleTer);
    titleTerry.setScale(.4,.4);
    titleTerry.setTextureRect(IntRect(1024*0,1024*1,1024,1024));
    titleTerry.setPosition(1920/2-((1024*.4)/2),400);

    //Bools for which Game Scene:
    bool titleScreen = true, shopMenu = false, startRun = false, gameState = false, deathScreen = false;

    //Game Loop:
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
            if(event.type == Event::KeyReleased)
                if(event.key.code == Keyboard::Space)
                terry.boosting = false;
            if(event.type == Event::MouseButtonReleased)
                if(event.mouseButton.button == Mouse::Left)
                {
                    shopB1.leftButtonDown = false;
                    shopB2.leftButtonDown = false;
                    shopB3.leftButtonDown = false;
                    shopB4.leftButtonDown = false;
                }
        }

    if(titleScreen)
    {
        Text cont;
        cont.setFont(font);
        cont.setCharacterSize(50);
        cont.setString("[Space]");
        cont.setPosition(1920/2-cont.getGlobalBounds().width/2,1000);

        int x1 = titleBG1.getPosition().x;
        int x2 = titleBG2.getPosition().x;
        int mFor = 1;

        float yChange = sin(titleFC*3.141592635/180)/2;

        titleTerry.move(0,yChange);

        if(x1 <= -1920)
        {
            titleBG1.move(-mFor+3840,0);
        }
        else
        {
            titleBG1.move(-mFor,0);
        }
        if(x2 <= -1920)
        {
            titleBG2.move(-mFor+3840,0);
        }
        else
        {
            titleBG2.move(-mFor,0);
        }

        if(Keyboard::isKeyPressed(Keyboard::Space))
        {
            titleScreen = false;
            gameState = true;
        }

        //Clears the window;
        window.clear();

        //Drawing Title Objects:
        window.draw(titleBG1);
        window.draw(titleBG2);
        window.draw(titleTerry);
        window.draw(titleLogo);
        window.draw(cont);

        //Updates the window:
        window.display();

        titleFC++;
    }

    if(shopMenu)
    {
        Text moneyText;
        Text scoreText;

        moneyText.setFont(font);
        moneyText.setCharacterSize(100);
        moneyText.setPosition(352,170);
        moneyText.setString(num2str(double(terry.money), true, true));

        scoreText.setFont(font);
        scoreText.setCharacterSize(100);
        scoreText.setPosition(1208,170);
        scoreText.setString(num2str(double(highScore), false, true));

        Sprite shopBG;
        shopBG.setTexture(bgTxtr);

        shopB1.update(terry);
        shopB2.update(terry);
        shopB3.update(terry);
        shopB4.update(terry);

        switch(shopB1.speed)
        {
        case 1 :
            changeFrame(7, terry, bg, ground, water, starVec);
            break;
        case 2 :
            changeFrame(6, terry, bg, ground, water, starVec);
            break;
        case 3 :
            changeFrame(5, terry, bg, ground, water, starVec);
            break;
        case 4 :
            changeFrame(4, terry, bg, ground, water, starVec);
            break;
        default :
            changeFrame(initialSpeed, terry, bg, ground, water, starVec);
            break;
        }
        switch(shopB2.boots)
        {
        case 1 :
            terry.bootsEfficiency = 0.9;
            break;
        case 2 :
            terry.bootsEfficiency = 0.8;
            break;
        case 3 :
            terry.bootsEfficiency = 0.7;
            break;
        case 4 :
            terry.bootsEfficiency = 0.6;
            break;
        default :
            terry.bootsEfficiency = 1;
            break;
        }
        switch(shopB3.fuel)
        {
        case 1 :
            terry.fuelMax = 50;
            terry.fuelCurrent = 50;
            break;
        case 2 :
            terry.fuelMax = 100;
            terry.fuelCurrent = 100;
            break;
        case 3 :
            terry.fuelMax = 150;
            terry.fuelCurrent = 150;
            break;
        case 4 :
            terry.fuelMax = 200;
            terry.fuelCurrent = 200;
            break;
        default :
            terry.fuelMax = 0;
            terry.fuelCurrent = 0;
            break;
        }
        switch(shopB4.wings)
        {
        case 1 :
            terry.glideFactor = 0.14;
            break;
        case 2 :
            terry.glideFactor = 0.18;
            break;
        case 3 :
            terry.glideFactor = 0.22;
            break;
        case 4 :
            terry.glideFactor = 0.26;
            break;
        default :
            break;
        }

        Sprite terryIdle;
        if(terry.wings) terryIdle.setTexture(tIdleW);
        else terryIdle.setTexture(tIdle);
        if(idleFC % 30 == 0) idleFrame = !idleFrame;
        terryIdle.setTextureRect(IntRect(1024*idleFrame,1024*terry.boots,1024,1024));
        terryIdle.setScale(.5,.5);
        terryIdle.setPosition(920,316);

        if(arrow.update())
        {
            startRun = true;
            shopMenu = false;
        }

        window.clear();

        window.draw(shopBG);
        window.draw(shopB1.button);
        window.draw(shopB2.button);
        window.draw(shopB3.button);
        window.draw(shopB4.button);

        window.draw(shopB1.progression);
        window.draw(shopB2.progression);
        window.draw(shopB3.progression);
        window.draw(shopB4.progression);

        window.draw(arrow.arrow);
        window.draw(moneyText);
        window.draw(scoreText);
        window.draw(terryIdle);

        window.display();

        idleFC++;
    }

    if(startRun)
    {
        currentScore = 0;
        terry.startRun();
        bg.startRun();
        water.startRun();
        ground.startRun();
        startRun = false;
        gameState = true;
    }

    if(gameState)
    {
       if(terry.changeAt > 0)
        {
            changeFrame(terry.changeAt, terry, bg, ground, water, starVec);
            terry.changeAt = 0;
        }
        if(terry.changeF > 0)
        {
            changeFor(terry.changeF, bg, ground, water, starVec);
            terry.changeF = 0;
        }

        terry.setFrame();
        terry.update();
        ground.update(terry.moving, terry.upDown, terry);
        if(!terry.flew) ground.update(terry.moving, terry.upDown, terry);
        bg.update(terry.moving, terry.upDown, terry);
        if(!terry.flew) bg.update(terry.moving, terry.upDown, terry);
        water.update(terry.moving, terry.upDown, terry);
        if(!terry.flew) water.update(terry.moving, terry.upDown, terry);
        for(int i = 0; i < starVec.size(); i++)
        {
            starVec[i].update(terry.moving, terry.upDown, terry);
            if(!terry.flew) starVec[i].update(terry.moving, terry.upDown, terry);
        }


        if(terry.fuelMax > 0)
        if(terry.fuelCurrent < 0) terry.fuelCurrent = 0;
        float fuelPercent = terry.fuelCurrent/terry.fuelMax*100;
        int frame, row;

        if(fuelPercent == 100)
        {
            frame = 0;
            row = 0;
        }
        else if(fuelPercent > 80)
        {
            frame = 1;
            row = 0;
        }
        else if(fuelPercent > 60)
        {
            frame = 0;
            row = 1;
        }
        else if(fuelPercent > 40)
        {
            frame = 1;
            row = 1;
        }
        else if(fuelPercent > 0)
        {
            frame = 0;
            row = 2;
        }
        else
        {
            frame = 1;
            row = 2;
        }

        Text fuelText;
        fuelText.setFont(font);
        fuelText.setCharacterSize(100);
        fuelText.setPosition(180,904);
        fuelText.setString(num2str(terry.fuelCurrent, false, true));

        fuelGage.setTextureRect(IntRect(192*frame,180*row,192,180));

        if(terry.flying)
        {
            currentScore += 15;
            //cout << terry.sprite.getRotation() << endl;
        }

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

        for(int i = 0; i < starVec.size(); i++)
        {
            window.draw(starVec[i].sprite1);
            window.draw(starVec[i].sprite2);
        }


        window.draw(terry.sprite);

        if(ground.distance > 4000)
        {
            window.draw(water.sprite1);
            window.draw(water.sprite2);
        }

        if(terry.fuelCurrent > 0 and terry.fuelCurrent)
        {
            window.draw(fuelGage);
            window.draw(fuelText);
        }

        window.display();

        if(terry.flew and !terry.flying)
        {
            gameState = false;
            deathScreen = true;
        }
    }

    if(deathScreen)
    {
        if(currentScore > highScore)
        {
            highScore = currentScore;
        }

        float rot = terry.sprite.getRotation();
        float diveBonus;

        if(rot > 180)
        {
            diveBonus = 0.5;
        }
        else if(rot > 135)
        {
            diveBonus = 0.75;
        }
        else if(rot > 110)
        {
            diveBonus = 1.0;
        }
        else if(rot > 90)
        {
            diveBonus = 1.25;
        }
        else if(rot == 90)
        {
            diveBonus = 2;
        }
        else if(rot > 70)
        {
            diveBonus = 1.5;
        }
        else if(rot > 45)
        {
            diveBonus = 1.25;
        }
        else if(rot > 0)
        {
            diveBonus = 0.75;
        }

        cout << diveBonus << endl;

        float money = currentScore*diveBonus*0.5;

        Text score(num2str(currentScore, false, true), font, 100);
        Text distance(num2str(currentScore, false, true), font, 100);
        Text dive(num2str(diveBonus, false, false), font, 100);
        Text total(num2str(money, true, true), font, 100);

        distance.setOrigin(distance.getLocalBounds().width,distance.getLocalBounds().height);
        dive.setOrigin(dive.getLocalBounds().width,dive.getLocalBounds().height);
        total.setOrigin(total.getLocalBounds().width,total.getLocalBounds().height);

        score.setPosition(644, 200);
        distance.setPosition(1203,436);
        dive.setPosition(1203,522);
        total.setPosition(1203,697);

        if(dArrow.update())
        {
            deathScreen = false;
            shopMenu = true;
            terry.money += round(money);
        }

        window.clear(Color(27,81,99));

        window.draw(bg.sprite1);
        window.draw(bg.sprite2);
        window.draw(water.sprite1);
        window.draw(water.sprite2);

        window.draw(dScreen);
        window.draw(dArrow.arrow);
        window.draw(score);
        window.draw(distance);
        window.draw(dive);
        window.draw(total);

        window.display();
    }

    }
}

void changeFrame(int f, Player &terry, Background &bg, Ground &ground, Water &water, vector<Stars> &starVec)
{
    terry.mAt = f;
    bg.mAt = f;
    ground.mAt = f;
    water.mAt = f;
    for(int i = 0; i < starVec.size(); i++)
    {
        starVec[i].mAt = f;
    }
}

void changeFor(int f, Background &bg, Ground &ground, Water &water, vector<Stars> &starVec)
{
    bg.mFor = bg.dFor + f;
    ground.mFor = ground.dFor + f;
    water.mFor = water.dFor + f;
    for(int i = 0; i < starVec.size(); i++)
    {
        starVec[i].mFor = starVec[i].dFor + f;
    }
}

string num2str(double n, bool m, bool r)
{
    stringstream ss;
    if(r) n = round(n);
    if(m) ss << "$" << n;
    else ss << n;
    return ss.str();
}
