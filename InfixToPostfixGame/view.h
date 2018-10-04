#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>
#include <QTimer>
#include <SFML/Graphics.hpp>
#include <QHBoxLayout>
#include "Box2D/Box2D.h"
#include "model.h"
#include <QString>
#include <QMouseEvent>
#include <QMessageBox>
#include <QPixmap>
#include <QEventLoop>
#include <SFML/Graphics.hpp>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QImage>
#include <iostream>
#include <QAnimationDriver>
#include <string>
#include <SFML/Graphics.hpp>

// set up proper file path to images and text files used
// for the OS being used.
#ifdef Q_OS_WIN
   #define PATH "../InfixToPostfixGame/"
   #define VIDFLAG true
#else
   #define PATH "../../../../InfixToPostfixGame/"
   #define VIDFLAG false
#endif

namespace Ui {
class View;
}

class View : public QMainWindow
{
    Q_OBJECT

public:
    explicit View(Model& model, QWidget *parent = 0);
    ~View();

signals:
    void askForWorld();
    void grabBody(float, float);
    void moveBody(float, float);
    void checkAnswer();
    void releaseBody();

private slots:
    void drawSprite(float,float, float,float, QString);
    void refreshTexture();
    void switchBodySelected();
    void NextFrame();

    void updateProgressBar(int progress);
    void updateProgressBarKnight(int progress);

private:
    Ui::View *ui;
    sf::RenderTexture gameTexture;
    QEventLoop loop;
    QTimer spriteClock;
    int dragonSpriteWidth;
    int dragonSpriteHeight;
    int dragonSpriteNumFrames;
    int knightSpriteWidth;
    int knightSpriteHeight;
    int knightSpriteNumFrames;
    sf::Sprite* curKnightSprite;
    sf::Sprite* curDragonSprite;
    sf::Sprite KnightIdleSprite;
    sf::Texture KnightIdleSpriteSheet;
    sf::Sprite KnightAttackSprite;
    sf::Texture KnightAttackSpriteSheet;
    sf::Sprite KnightBlockSprite;
    sf::Texture KnightBlockSpriteSheet;
    sf::Sprite KnightDeathSprite;
    sf::Texture KnightDeathSpriteSheet;
    sf::Sprite DragonIdleSprite;
    sf::Texture DragonIdleSpriteSheet;
    sf::Sprite DragonAttackSprite;
    sf::Texture DragonAttackSpriteSheet;
    sf::Sprite DragonDamagedSprite;
    sf::Texture DragonDamagedSpriteSheet;
    sf::Sprite DragonDeathSprite;
    sf::Texture DragonDeathSpriteSheet;
    int knightAttackNum;
    bool knightAttacking;
    bool dragonDead;
    bool knightBlocking;
    bool knightDead;
    int knightBlockNum;

    void changeEquation(QString);
    void setStartGameButtons();
    void setEndGameButtons();
    void enableSubmit();
    void disableSubmit();
    void displayWinMessage(QString, bool);
    void displayLossMessage(QString, bool);

    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void playTutorial();
    void delay(int);

    bool bodySelected;

};

#endif // VIEW_H

