#include "View.h"
#include "ui_View.h"

using namespace std;

View::View(Model& model, QWidget *parent) : QMainWindow(parent), ui(new Ui::View)
{

    ui->setupUi(this);

    //ui->messageLabel->setAttribute(Qt::WA_TranslucentBackground);
    ui->dragon->setAttribute(Qt::WA_TranslucentBackground);
    ui->knight->setAttribute(Qt::WA_TranslucentBackground);
    ui->messageLabel->setWordWrap(true);

    ui->messageLabel->setText("Objective: convert infix expression \n into a postfix expression in order to \n slay the dragon. \n \n Press Start to Begin");
    ui->equationLabel->setText("");

    connect(ui->startButton, &QPushButton::clicked, &model, &Model::startButtonClicked);
    connect(ui->submitButton, &QPushButton::clicked, &model, &Model::submitButtonClicked);
    connect(ui->tutorialButton, &QPushButton::clicked, this, &View::playTutorial);

    connect(&model, &Model::changeEquation, this, &View::changeEquation);
    connect(&model, &Model::setStartGameButtons, this, &View::setStartGameButtons);

    connect(&model, &Model::setEndGameButtons, this, &View::setEndGameButtons);

    connect(&model, &Model::enableSubmit, this, &View::enableSubmit);
    connect(&model, &Model::disableSubmit, this, &View::disableSubmit);

    connect(&model, &Model::displayLossMessage, this, &View::displayLossMessage);
    connect(&model, &Model::displayWinMessage, this, &View::displayWinMessage);

    connect(this, &View::askForWorld, &model, &Model::sendWorld);
    connect(&model, &Model::sendB2Body, this, &View::drawSprite);

    connect(this, &View::grabBody, &model, &Model::findBodyToGrab);
    connect(&model, &Model::grabbedBody, this, &View::switchBodySelected);
    connect(this, &View::moveBody, &model, &Model::setBodyPosition);

    connect(this, &View::releaseBody, &model, &Model::dropBody);

    connect(&model, &Model::updateProgressBarSignal, this, &View::updateProgressBar);
    connect(&model, &Model::updateProgressBarKnightSignal, this, &View::updateProgressBarKnight);

    // Brady Sprite Stuff
    knightBlocking = false;
    knightDead = false;
    knightBlockNum = 0;
    knightAttacking = false;
    knightAttackNum = 0;
    std::string path(PATH);
    knightSpriteHeight = knightSpriteWidth = 84;
    knightSpriteNumFrames = 4;
    dragonSpriteWidth = 156;
    dragonSpriteHeight = 212;
    dragonSpriteNumFrames = 12;
    //KNIGHT**********
    //IDLE
    KnightIdleSpriteSheet.loadFromFile(path +"images/KnightIdleBigger.png");
    sf::IntRect rectKnightIdle(0, 0, 84, 84);
    KnightIdleSprite.setTexture(KnightIdleSpriteSheet);
    KnightIdleSprite.setTextureRect(rectKnightIdle);
    //ATTACK
    KnightAttackSpriteSheet.loadFromFile(path +"images/KnightAttackBigger.png");
    sf::IntRect rectKnightAttack(0, 0, 160, 84);
    KnightAttackSprite.setTexture(KnightAttackSpriteSheet);
    KnightAttackSprite.setTextureRect(rectKnightAttack);
    //BLOCK
    KnightBlockSpriteSheet.loadFromFile(path +"images/KnightBlockBigger.png");
    sf::IntRect rectKnightBlock(0, 0, 84, 84);
    KnightBlockSprite.setTexture(KnightBlockSpriteSheet);
    KnightBlockSprite.setTextureRect(rectKnightBlock);
    //DEATH
    KnightDeathSpriteSheet.loadFromFile(path +"images/KnightDeathBigger.png");
    sf::IntRect rectKnightDeath(0, 0, 84, 84);
    KnightDeathSprite.setTexture(KnightDeathSpriteSheet);
    KnightDeathSprite.setTextureRect(rectKnightDeath);

    //DRAGON**********
    //IDLE
    DragonIdleSpriteSheet.loadFromFile(path +"images/DragonIdleBigger.png");
    sf::IntRect rectDragonIdle(0, 0, 156, 212);
    DragonIdleSprite.setTexture(DragonIdleSpriteSheet);
    DragonIdleSprite.setTextureRect(rectDragonIdle);
    //DAMAGED
    DragonDamagedSpriteSheet.loadFromFile(path +"images/DragonDamagedBigger.png");
    sf::IntRect rectDragonDamaged(0, 0, 280, 280);
    DragonDamagedSprite.setTexture(DragonDamagedSpriteSheet);
    DragonDamagedSprite.setTextureRect(rectDragonDamaged);
    //ATTACK
    DragonAttackSpriteSheet.loadFromFile(path +"images/DragonAttackBigger.png");
    sf::IntRect rectDragonAttack(0, 0, 156, 212);
    DragonAttackSprite.setTexture(DragonAttackSpriteSheet);
    DragonAttackSprite.setTextureRect(rectDragonAttack);
    //DEATH
    DragonDeathSpriteSheet.loadFromFile(path +"images/DragonDeathBigger.png");
    sf::IntRect rectDragonDeath(0, 0, 280, 280);
    DragonDeathSprite.setTexture(DragonDeathSpriteSheet);
    DragonDeathSprite.setTextureRect(rectDragonDeath);

    //SET DEFAULT ANIMATION
    curKnightSprite = &KnightIdleSprite;
    curDragonSprite = &DragonIdleSprite;

    //CONNECT & START
    connect(&spriteClock, &QTimer::timeout, this, &View::NextFrame);
    spriteClock.setInterval(250);
    //spriteClock.start();

    bodySelected=false;
    gameTexture.create(631, 431); // set the texture to draw sprites
    refreshTexture();
}

View::~View()
{
    delete ui;
}

void View::changeEquation(QString s){
    ui->equationLabel->setText(s);
}

void View::NextFrame()
{
    //KNIGHT ATTACK
    if(knightAttacking && knightAttackNum == 10)
    {

        knightAttacking = false;
        knightAttackNum = 0;
        knightSpriteHeight = knightSpriteWidth = 84;
        knightSpriteNumFrames = 4;
        dragonSpriteWidth = 156;
        dragonSpriteHeight = 212;
        dragonSpriteNumFrames = 12;
        curKnightSprite = &KnightIdleSprite;
        curDragonSprite = &DragonIdleSprite;
        if(dragonDead)
        {
            spriteClock.stop();
            return;
        }
    }
    else if(knightAttacking)
    {

        if(knightAttackNum == 6)
        {
            if(dragonDead)
            {
                dragonSpriteWidth = 280;
                dragonSpriteHeight = 280;
                dragonSpriteNumFrames = 5;
                curDragonSprite = &DragonDeathSprite;
            }
            else
            {
                dragonSpriteWidth = 280;
                dragonSpriteHeight = 280;
                dragonSpriteNumFrames = 4;
                curDragonSprite = &DragonDamagedSprite;
            }
        }
        knightAttackNum++;
    }
    // KNIGHT BLOCK
    else if(knightBlocking && knightBlockNum == 7)
    {
        knightBlocking = false;
        knightBlockNum = 0;
        knightSpriteHeight = knightSpriteWidth = 84;
        knightSpriteNumFrames = 4;
        dragonSpriteWidth = 156;
        dragonSpriteHeight = 212;
        dragonSpriteNumFrames = 12;
        curKnightSprite = &KnightIdleSprite;
        curDragonSprite = &DragonIdleSprite;
        if(knightDead)
        {
            spriteClock.stop();
            return;
        }
    }
    else if(knightBlocking)
    {
        if(knightBlockNum == 2)
        {
            dragonSpriteWidth = 280;
            dragonSpriteHeight = 280;
            dragonSpriteNumFrames = 4;
            curDragonSprite = &DragonAttackSprite;
            if(knightDead)
            {
                knightSpriteHeight=84;
                knightSpriteWidth=84;
                knightSpriteNumFrames=9;
                curKnightSprite = &KnightDeathSprite;
            }
        }
        knightBlockNum++;
    }

    sf::RenderTexture knightTexture;
    sf::RenderTexture dragonTexture;

    //KNIGHT
    knightTexture.create(knightSpriteWidth,knightSpriteHeight);
    knightTexture.clear(sf::Color(0,0,0,0));
    sf::IntRect rectSourceSpriteKnight(curKnightSprite->getTextureRect());
    if (rectSourceSpriteKnight.left == knightSpriteWidth*(knightSpriteNumFrames-1))
        rectSourceSpriteKnight.left = 0;
    else
        rectSourceSpriteKnight.left += knightSpriteWidth;

    curKnightSprite->setTextureRect(rectSourceSpriteKnight);

    knightTexture.draw(*curKnightSprite);
    knightTexture.display();
    sf::Texture rt = knightTexture.getTexture();
    sf::Image irt = rt.copyToImage();
    const uint8_t *pp = irt.getPixelsPtr();
    QImage qi(pp, knightSpriteWidth, knightSpriteHeight, QImage::Format_ARGB32);
    qi= qi.rgbSwapped();

    ui->knight->setPixmap(QPixmap::fromImage(qi));


    //DRAGON
    dragonTexture.create(dragonSpriteWidth,dragonSpriteHeight);
    dragonTexture.clear(sf::Color(0,0,0,0));
    sf::IntRect rectSourceSprite(curDragonSprite->getTextureRect());
    if (rectSourceSprite.left == dragonSpriteWidth*(dragonSpriteNumFrames-1))
        rectSourceSprite.left = 0;
    else
        rectSourceSprite.left += dragonSpriteWidth;

    curDragonSprite->setTextureRect(rectSourceSprite);

    dragonTexture.draw(*curDragonSprite);
    dragonTexture.display();
    sf::Texture rtDragon = dragonTexture.getTexture();
    sf::Image irtDragon = rtDragon.copyToImage();
    const uint8_t *ppDragon = irtDragon.getPixelsPtr();
    QImage qiDragon(ppDragon, dragonSpriteWidth, dragonSpriteHeight, QImage::Format_ARGB32);
    qiDragon= qiDragon.rgbSwapped();

    ui->dragon->setPixmap(QPixmap::fromImage(qiDragon));

}

void View::setStartGameButtons(){
    dragonDead = false;
    spriteClock.start();
    ui->startButton->setEnabled(false);
    ui->messageLabel->setText("");
    std::string path(PATH);
    QString correctPathForStart = QString::fromStdString(path);

    correctPathForStart.append("images/PlayButtonGray.png");
    QPixmap pixmap(correctPathForStart);
    QIcon ButtonIcon(pixmap);
    ui->startButton->setIcon(ButtonIcon);
    ui->startButton->setIconSize(pixmap.rect().size());
}

void View::setEndGameButtons(){
    ui->startButton->setEnabled(true);
    ui->submitButton->setEnabled(false);
    std::string path(PATH);
    QString correctPathForStart = QString::fromStdString(path);
    QString correctPathForSubmit = QString::fromStdString(path);

    correctPathForStart.append("images/PlayButton.png");
    QPixmap pixmap(correctPathForStart);
    QIcon ButtonIcon(pixmap);
    ui->startButton->setIcon(ButtonIcon);
    ui->startButton->setIconSize(pixmap.rect().size());

    correctPathForSubmit.append("images/SubmitButtonGray.png");
    QPixmap pixmap2(correctPathForSubmit);
    QIcon ButtonIcon2(pixmap2);
    ui->submitButton->setIcon(ButtonIcon2);
    ui->submitButton->setIconSize(pixmap2.rect().size());
}

/*******************
refreshTexture
Recieves:
Returns: void
Purpose: describe the world in the game label
********************/
void View::refreshTexture(){
    gameTexture.clear(sf::Color::Transparent);

    // get information about world
    emit askForWorld();

    gameTexture.display();

    // convert texture to image
    sf::Texture rt = gameTexture.getTexture();
    sf::Image irt = rt.copyToImage();
    const uint8_t *pp = irt.getPixelsPtr();
    QImage qi(pp, 631, 431, QImage::Format_ARGB32);
    qi= qi.rgbSwapped();

    ui->game->setPixmap(QPixmap::fromImage(qi));

    // execute this function in 33/1000 second
    QTimer::singleShot(33, this, SLOT(refreshTexture()));
}

/*******************
drawSprite
Recieves: 4 floats, 1 QString
Returns: void
Purpose: draw sprite
********************/
void View::drawSprite(float x, float y, float width, float height, QString name){
    sf::Sprite sprite;
    sf::Texture spriteTexture;
    std::string path(PATH);

    // name of a body decide what image it will use
    if(name == QString("1"))
        spriteTexture.loadFromFile(path + "images/one.png");
    else if(name == QString("2"))
        spriteTexture.loadFromFile(path + "images/two.png");
    else if(name == QString("3"))
        spriteTexture.loadFromFile(path + "images/three.png");
    else if(name == QString("4"))
        spriteTexture.loadFromFile(path + "images/four.png");
    else if(name == QString("5"))
        spriteTexture.loadFromFile(path + "images/five.png");
    else if(name == QString("6"))
        spriteTexture.loadFromFile(path + "images/six.png");
    else if(name == QString("7"))
        spriteTexture.loadFromFile(path + "images/seven.png");
    else if(name == QString("8"))
        spriteTexture.loadFromFile(path + "images/eight.png");
    else if(name == QString("9"))
        spriteTexture.loadFromFile(path + "images/nine.png");
    else if(name == QString("0"))
        spriteTexture.loadFromFile(path + "images/zero.png");
    else if(name == QString("+"))
        spriteTexture.loadFromFile(path + "images/plus.png");
    else if(name == QString("-"))
        spriteTexture.loadFromFile(path + "images/minus.png");
    else if(name == QString("*"))
        spriteTexture.loadFromFile(path + "images/multiply.png");
    else if(name == QString("/"))
        spriteTexture.loadFromFile(path + "images/divide.png");
    else if(name == QString("a"))
        spriteTexture.loadFromFile(path + "images/a.png");
    else if(name == QString("b"))
        spriteTexture.loadFromFile(path + "images/b.png");
    else if(name == QString("c"))
        spriteTexture.loadFromFile(path + "images/c.png");
    else if(name == QString("d"))
        spriteTexture.loadFromFile(path + "images/d.png");
    else if(name == QString("e"))
        spriteTexture.loadFromFile(path + "images/e.png");
    else{
    }

    sprite.setTexture(spriteTexture);
    sprite.setOrigin(width/2.f,height/2.f);
    sprite.setPosition(x,y);

    gameTexture.draw(sprite);
}

/*******************
switchBodySelected
Recieves:
Returns: void
Purpose: tells if any body is selected
********************/
void View::switchBodySelected(){
    bodySelected =true;
}

void View::displayLossMessage(QString message, bool knightState)
{
    knightDead = knightState;
    std::string path(PATH);
    knightSpriteHeight=84;
    knightSpriteWidth=84;
    knightSpriteNumFrames=7;
    curKnightSprite = &KnightBlockSprite;
    knightBlocking = true;
    ui->messageLabel->setText(message);
    delay(1500);
}

void View::delay(int time)
{
     QTimer::singleShot(time, &loop, SLOT(quit()));
     loop.exec();
}

void View::displayWinMessage(QString message, bool dragonState){
    dragonDead=dragonState;
    std::string path(PATH);
    knightSpriteHeight=84;
    knightSpriteWidth=160;
    knightSpriteNumFrames=10;
    curKnightSprite = &KnightAttackSprite;
    knightAttacking = true;
    ui->messageLabel->setText(message);
    delay(1500);
}

/*******************
mousePressEvent
Recieves: QMouseEvent*
Returns: void
Purpose: send the coordinate to the model to grab a body
********************/
void View::mousePressEvent(QMouseEvent* event){
    emit grabBody(event->x()-10.f, event->y()-140.f);
}

/*******************
mouseMoveEvent
Recieves: QMouseEvent*
Returns: void
Purpose: send the coordinate to move the selected body
********************/
void View::mouseMoveEvent(QMouseEvent* event){

    if(bodySelected==true){
        emit moveBody(event->x()-10.f, event->y()-140.f);
    }

}

/*******************
mouseReleaseEvent
Recieves: QMouseEvent*
Returns: void
Purpose: drop body
********************/
void View::mouseReleaseEvent(QMouseEvent* event){
    if(bodySelected==true){
        bodySelected=false;
        emit releaseBody();
    }
}

void View::enableSubmit(){
    ui->submitButton->setEnabled(true);
    std::string path(PATH);
    QString correctPathForSubmit = QString::fromStdString(path);

    correctPathForSubmit.append("images/SubmitButton.png");
    QPixmap pixmap2(correctPathForSubmit);
    QIcon ButtonIcon2(pixmap2);
    ui->submitButton->setIcon(ButtonIcon2);
    ui->submitButton->setIconSize(pixmap2.rect().size());
}

void View::disableSubmit(){
    ui->submitButton->setEnabled(false);
    std::string path(PATH);
    QString correctPathForSubmit = QString::fromStdString(path);

    correctPathForSubmit.append("images/SubmitButtonGray.png");
    QPixmap pixmap2(correctPathForSubmit);
    QIcon ButtonIcon2(pixmap2);
    ui->submitButton->setIcon(ButtonIcon2);
    ui->submitButton->setIconSize(pixmap2.rect().size());
}

void View::updateProgressBar(int progress)
{
    ui->progressBar->setValue(progress);
}

void View::updateProgressBarKnight(int progress)
{
    ui->progressBarKnight->setValue(progress);
}

void View::playTutorial()
{   
    std::string path(PATH);
    QString correctPathForStart = QString::fromStdString(path);

    if(VIDFLAG)
    {
        QMediaPlayer *player = new QMediaPlayer;
        QVideoWidget* vw = new QVideoWidget();

        player->setVideoOutput(vw);

        player->setMedia(QUrl::fromLocalFile("../InfixToPostfixGame/images/tutorial_video.avi"));
        player->setVolume(50);

        vw->setGeometry(100, 100, 600, 300);
        vw->setFixedSize(vw->width(), vw->height());
        vw->show();

        player->play();
        //delete player;
        //delete vw;
    }

    else
    {
        correctPathForStart.append("images/tutorial_videoboth.mp4");
        QString abs = QCoreApplication::applicationDirPath();
        abs.append("/");
        abs.append(correctPathForStart);

        QMediaPlayer *player = new QMediaPlayer;
        QVideoWidget* vw = new QVideoWidget();

        player->setVideoOutput(vw);

        player->setMedia(QUrl::fromLocalFile(abs));
        player->setVolume(50);

        vw->setGeometry(100, 100, 600, 300);
        vw->setFixedSize(vw->width(), vw->height());
        vw->show();

        player->play();
        //delete player;
        //delete vw;
    }

}

