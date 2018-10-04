#ifndef MODEL_H
#define MODEL_H
#include <QString>
#include <QObject>
#include "Box2D/Box2D.h"
#include <QTimer>
#include "spriteinfo.h"
#include <iostream>
#include <stack>
#include "equations.h"
#include <cstdlib>
#include <QMessageBox>
#include <QDir>

using namespace std;

// set up proper file path to images and text files used
// for the OS being used.
#ifdef Q_OS_WIN
   #define PATH "../InfixToPostfixGame/"
#else
   #define PATH "../../../../InfixToPostfixGame/"
#endif


class Model : public QObject
{
    Q_OBJECT
signals:
    void changeEquation(QString);
    void setStartGameButtons();
    void setEndGameButtons();
    void enableSubmit();
    void disableSubmit();
    void displayWinMessage(QString, bool);
    void displayLossMessage(QString, bool);
    void updateProgressBarSignal(int);
    void updateProgressBarKnightSignal(int);

    //Yoonki
    void sendB2Body(float,float,float,float,QString);
    void grabbedBody();

public slots:
    void startButtonClicked();
    void submitButtonClicked();
    void sendWorld();
    void findBodyToGrab(float,float);
    void setBodyPosition(float, float);
    void dropBody();

private slots:


private:
    //Yoonki
    b2World* world;
    b2Body* curBody;
    float scale;
    QTimer timer;
    int count;
    QString answer;  // Trenton
    Equations equations;  // Trenton
    std::string currentDifficulty;
    int correctAnswersCount;
    int wrongAnswerCount;
    QString equation;
    QString userAnswer;
    int Dragonhealth;
    int Knighthealth;


    void checkAnswerBox(float, float);
    void checkUserAnswer();

    std::string infix_to_postfix(const std::string exp);  // Trenton
    int precedence(char op);  // Trenton
    QString next_equation(std::string diff);  // Trenton

    void createBody(float,float, int, float,float, QString, bool);
    void setBodies();
    void gameLost();
    QString getDifficulty();


public:
    explicit Model(QObject *parent = nullptr);

};

#endif // MODEL_H
