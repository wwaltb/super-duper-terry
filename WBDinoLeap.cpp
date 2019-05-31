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
    double upDown;
    bool moving, flying, boots, boosting, ramp, flew;
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
    frame = 0; row = 0; fC = 0; rotation = 0; uDCount = 0; rotCount = 0;
    upDown = 0; money = 100000, changeAt = 0, changeF = 0;
    moving = false; flying = false; boots = false; boosting = false; ramp = false; flew = false;
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

    if(!flew)
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
        upDown -= .3;
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
    //Testing terry positions:
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
        if(flying)
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
    cout << upDown << endl;
    rFile >> rotation;
    const float aX = 2.82e-14;
    const float bX = -2.8219e-10;
    const float cX = 9.9868e-7;
    const float dX = -0.00134363;
    const float eX = 0.179261;
    const float fX = 603.695;
    upDown = aX*pow(distance, 5) + bX*pow(distance, 4) + cX*pow(distance, 3) + dX*pow(distance, 2) + eX*distance + fX;
    cout << upDown << endl << endl;
    sprite.rotate(rotation);
    if (distance < 2200)
        upDown -= 1;
    if (distance > 1850 and distance < 2146)
        upDown += 1.5;
    else upDown += 1;
}

void Player::startFlying()
{
    flew = true;
    flying = true;
    moving = false;
    upDown = 20+(24-(mAt*3));
    changeF = 24-mAt*3;
    sprite.setRotation(-45);
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
        //cout << "bg " << x1 + 1920 - x2 << endl;
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
        //cout << "w " << x1 + 1920 - x2 << endl;
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
    //cout << sprite4.getPosition().y << endl;
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

    if(distance == 2010 and fC % mAt == 0)
        terry.changeAt = mAt - 1;

    if(distance == 2940 and fC % mAt == 0)
        terry.changeAt = mAt - 1;

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
            //cout << distance << endl;
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
    if(!texture.loadFromFile(n))
        cout << "yikes" << endl;
    if(!texture2.loadFromFile("Shop Bar.png"))
        cout << "yikes" << endl;
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
        if(mouse.isButtonPressed(Mouse::Left))
        {
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
            }
            break;
        case 1 :
            if(terry.money > speed1)
            {
                speed++;
                frame++;
                terry.money -= speed1;
            }
            break;
        case 2 :
            if(terry.money > speed2)
            {
                speed++;
                frame++;
                terry.money -= speed2;
            }
            break;
        case 3 :
            if(terry.money > speed3)
            {
                speed++;
                frame++;
                terry.money -= speed3;
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
            }
            break;
        case 1 :
            if(terry.money > boots1)
            {
                boots++;
                frame++;
                terry.money -= boots1;
            }
            break;
        case 2 :
            if(terry.money > boots2)
            {
                boots++;
                frame++;
                terry.money -= boots2;
            }
            break;
        case 3 :
            if(terry.money > boots3)
            {
                boots++;
                frame++;
                terry.money -= boots3;
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
            }
            break;
        case 1 :
            if(terry.money > fuel1)
            {
                fuel++;
                frame++;
                terry.money -= fuel1;
            }
            break;
        case 2 :
            if(terry.money > fuel2)
            {
                fuel++;
                frame++;
                terry.money -= fuel2;
            }
            break;
        case 3 :
            if(terry.money > fuel3)
            {
                fuel++;
                frame++;
                terry.money -= fuel3;
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
            }
            break;
        case 1 :
            if(terry.money > wings1)
            {
                wings++;
                frame++;
                terry.money -= wings1;
            }
            break;
        case 2 :
            if(terry.money > wings2)
            {
                wings++;
                frame++;
                terry.money -= wings2;
            }
            break;
        case 3 :
            if(terry.money > wings3)
            {
                wings++;
                frame++;
                terry.money -= wings3;
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
    if(!texture.loadFromFile("Arrow.png"))
        cout << "yikes" << endl;
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


void changeFrame(int, Player &terry, Background &bg, Ground &ground, Water &water);
void changeFor(int, Background &bg, Ground &ground, Water &water);
string num2str(double);

int main()
{
    int initialSpeed = 8;

    //cin >> initialSpeed;

    Player terry(initialSpeed);
    Background bg(initialSpeed);
    Ground ground(initialSpeed);
    Water water(initialSpeed);

    shopButton shopB1(1, "Speed.png", 378-21*2, 309-14*2);
    shopButton shopB2(2, "Boots.png", 378-21*2, 444-14*2);
    shopButton shopB3(3, "Fuel.png",  378-21*2, 579-14*2);
    shopButton shopB4(4, "Wings.png", 378-21*2, 719-14*2);
    Arrow arrow(1495, 772);

    Font font;
    if(!font.loadFromFile("DroidBB.ttf"))
        cout << "yikes" << endl;

    bool titleScreen = false, shopMenu = true, gameState = false, deathScreen = false;

    RenderWindow window(VideoMode(1920,1080),"Terry", Style::Fullscreen);
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

    if(shopMenu)
    {
        Texture bgTxtr;
        Text moneyText;

        moneyText.setFont(font);
        moneyText.setCharacterSize(100);
        moneyText.setPosition(352,170);
        moneyText.setString(num2str(double(terry.money)));

        if(!bgTxtr.loadFromFile("Dino Store.png"))
            cout << "yikes" << endl;
        Sprite shopBG;
        shopBG.setTexture(bgTxtr);

        shopB1.update(terry);
        shopB2.update(terry);
        shopB3.update(terry);
        shopB4.update(terry);
        gameState = arrow.update();
        switch(shopB1.speed)
        {
        case 1 :
            changeFrame(7, terry, bg, ground, water);
            break;
        case 2 :
            changeFrame(6, terry, bg, ground, water);
            break;
        case 3 :
            changeFrame(5, terry, bg, ground, water);
            break;
        case 4 :
            changeFrame(4, terry, bg, ground, water);
            break;
        default :
            break;
        }

        if(gameState)
        {
            cout << "gameState" << endl;
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

        window.display();
    }

    if(gameState)
    {
       if(terry.changeAt > 0)
        {
            changeFrame(terry.changeAt, terry, bg, ground, water);
            terry.changeAt = 0;
        }
        if(terry.changeF > 0)
        {
            changeFor(terry.changeF, bg, ground, water);
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
}

void changeFrame(int f, Player &terry, Background &bg, Ground &ground, Water &water)
{
    terry.mAt = f;
    bg.mAt = f;
    ground.mAt = f;
    water.mAt = f;
}

void changeFor(int f, Background &bg, Ground &ground, Water &water)
{
    bg.mFor += f;
    ground.mFor += f;
    water.mFor += f;
}

string num2str(double n)
{
    stringstream ss;
    ss << "$" << n;
    return ss.str();
}
