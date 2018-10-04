#include "model.h"


using namespace std;

Model::Model(QObject *parent) : QObject(parent)
{

    world = new b2World(b2Vec2(0.f,9.8f));
    scale = 30.f;


    // fill equations object with equations stored in txt files
    //equations.equations_from_file("easy", "../InfixToPostfixGame/easyEq.txt");
    //equations.equations_from_file("medium", "../InfixToPostfixGame/medEq.txt");
    //equations.equations_from_file("hard", "../InfixToPostfixGame/hardEq.txt");
    currentDifficulty = "easy";
    correctAnswersCount = 0;
    wrongAnswerCount = 0;

    std::string path(PATH);
    std::cout << path << std::endl;

    // fill equations object with equations stored in txt files
    equations.equations_from_file("easy", path + "easyEq.txt");
    equations.equations_from_file("medium", path + "medEq.txt");
    equations.equations_from_file("hard", path + "hardEq.txt");
}


void Model::startButtonClicked()
{
    QString s;

    Dragonhealth = 0;
    Knighthealth = 0;

    s = next_equation("easy");

    emit changeEquation(s);

    emit setStartGameButtons();

    emit updateProgressBarSignal(6);
    emit updateProgressBarKnightSignal(3);

    setBodies();
}

void Model::submitButtonClicked()
{
    emit disableSubmit();

    checkUserAnswer();

    //figure out what got submitted, put it here. for now, we will assume whenever submit is hit the answer is correct
    QString submittedString = userAnswer;

    //check if submitted string is correct. for testing right now, assume it is
    if(submittedString == answer){
        correctAnswersCount++;
        Dragonhealth++;
        //if theyve answered 5 answers correctly
        if(correctAnswersCount >= 2){
            //if the difficulty is easy, upgrade it
            if(currentDifficulty == "easy"){
                currentDifficulty = "medium";
                emit displayWinMessage("Thou hast angered the dragon. \n Difficulty increased.\n \n New Difficulty: Medium", false);
            }
            //if the difficulty is medium, upgrade it
            else if(currentDifficulty == "medium"){
                currentDifficulty = "hard";
                emit displayWinMessage("Thou needest a bigger challenge? \n Difficulty increased.\n \n New Difficulty: Hard", false);
            }
            else{
                emit displayWinMessage("Thou hast slay'd ye ol terrible beast  \n and saved the Kingdom! \n \n Press play to start a new adventure.", true);
                emit changeEquation("");
                emit setEndGameButtons();
                correctAnswersCount = 0;
                currentDifficulty = "easy";
                wrongAnswerCount = 0;
                emit updateProgressBarSignal(6 - Dragonhealth);
                Knighthealth = 0;
                Dragonhealth = 0;
                return;
                //they finished the game, maybe do something cool, idk
            }
            //set the answer count back to zero, emit equation of new difficulty type
            correctAnswersCount = 0;
            QString nextString = next_equation(currentDifficulty);
            emit changeEquation(nextString);
        }
        //have not yet answered 5 correctly
        else{
            //emit equation of same difficulty type

            emit displayWinMessage("Smite the beast! \n \n Current Difficulty: " + getDifficulty(), false);
            QString nextString = next_equation(currentDifficulty);
            emit changeEquation(nextString);
        }
    }
    //submitted string is the wrong answer
    else{
        wrongAnswerCount++;
        Knighthealth++;
        if(wrongAnswerCount >= 3){
            gameLost();
            emit updateProgressBarKnightSignal(3 - Knighthealth);
            return;
        }
        else{
            emit displayLossMessage("Dost thou even hoist? \n Try again, but be careful.\n \n Current Difficulty: " + getDifficulty(), false);
        }
    }
    setBodies();

    emit updateProgressBarSignal(6 - Dragonhealth);
    emit updateProgressBarKnightSignal(3 - Knighthealth);
}

/*******************
createBody
Recieves: 4 floats, 1 int, 1 qstring, 1 bool
Returns: void
Purpose: create a body in the world.
********************/
void Model::createBody(float x, float y, int bodyType, float width, float height, QString name, bool movable){
    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(x/scale, y/scale);

    // set the type of the body
    if(bodyType==1)
        bodyDef.type = b2_staticBody;
    else if(bodyType==2)
        bodyDef.type = b2_kinematicBody;
    else
        bodyDef.type = b2_dynamicBody;

    b2Body* body = world->CreateBody(&bodyDef);

    // set the size and properties of the body
    b2PolygonShape shape;
    shape.SetAsBox((width/2)/scale,(height/2)/scale);
    b2FixtureDef fixtureDef;
    fixtureDef.density = 1.f;
    fixtureDef.friction = 0.7f;
    fixtureDef.shape = &shape;
    if(currentDifficulty == "easy"){
        fixtureDef.restitution = 1.05;
    }
    else{
        fixtureDef.restitution = 1.1;
    }
    //fixtureDef.restitution = 1.1;
    body->CreateFixture(&fixtureDef);

    // store additional information about the body
    // movable shows if this body can be moved by user
    spriteInfo* sprite = new spriteInfo(width, height, name, movable);
    body->SetUserData(sprite);
}


/*******************
sendWorld
Recieves:
Returns: void
Purpose: Send information about bodies in the world to the View
         The information contains the coordinate, width, height, name of each body, so View can describe sprite with this data
********************/
void Model::sendWorld(){
    for(b2Body* iter = world->GetBodyList(); iter!=0;iter=iter->GetNext()){
        spriteInfo* aa = static_cast<spriteInfo*>(iter->GetUserData());
        emit sendB2Body(iter->GetPosition().x*scale,iter->GetPosition().y*scale,aa->width, aa->height, aa->name);
    }
    world->Step(1/30.f, 8,3);
}




/*******************
setBodies
Recieves:
Returns: void
Purpose: Set bodies in the world
********************/
void Model::setBodies(){

    QString name;

    // Remove all the bodies if there is any
    for(b2Body* iter= world->GetBodyList();iter!=0; iter=iter->GetNext()){
        world->DestroyBody(iter);
    }

    // create walls to prevent bodies from getting out of the window
    createBody(600/2.f,360.f,1,700,45, "ground", false);
    createBody(50.f,431/2.f, 1, 45, 331, "leftWall", false);
    createBody(570.f,431/2.f, 1, 45, 331, "rightWall",false);
    createBody(600/2.f,40.f, 1, 550,45,"ceil", false);

    createBody(600/2.f,0.f,1,700,5, "top", false);
    createBody(600/2.f,430.f,1,700,5, "bottom", false);


    // alphabets and numbers for answer are generated in random locations.
    for(int i=0; i<equation.length() ; i++){
        if(equation[i] != '(' && equation[i] != ')'){
            name = QString(equation[i]);
            createBody(50+ rand()*490.f/(RAND_MAX +1.0),50+ rand()*280.f/(RAND_MAX +1.0), 3, 45,45, name, true);
        }
    }
}


/*********************
precedence
Recieves: character
Returns: integer
Purpose: assignes a integer value for the precedence of an operator.
         1 for + and -, 2 for * and /. Assumes only these operators
         are supported.
*********************/
int Model::precedence(char op)
{
    if(op == '*' || op == '/')
    {
        return 2;
    }
    else
    {
        return 1;
    }
}

/*******************
infix_to_postfix
Recieves: string
Returns: string
Purpose: convert the string infix expression recieved into
         postfix notation and return it. Assumes that
         expression recieved is valid.
********************/
std::string Model::infix_to_postfix(const std::string exp)
{
    std::stack<char> st;
    std::string post;

    // iterate through each character to convert expression
    for(std::string::const_iterator itr = exp.begin(); itr != exp.end(); itr++)
    {
        switch(*itr)
        {
            // character is '(' then save on stack
            case '(':
                st.push(*itr);
                break;

            // character is ')' then pop the stack until the opening paren is found
            case ')':
                while(st.top() != '(')
                {
                    post += st.top();
                    st.pop();
                }
                // remove open paren
                st.pop();
                break;

            // character is a operator then check if ones of greater precedence
            // are on the stack
            case '+':
            case '-':
            case '*':
            case '/':
                while(!st.empty() && st.top() != '(' && precedence(*itr) <= precedence(st.top()))
                {
                    post += st.top();
                    st.pop();
                }
                // add new operator
                st.push(*itr);
                break;

            // character not an operator then it is an operand so add to end of postfix expression
            default:
                post += *itr;
                break;

        }// end switch

    }// exp string for

    // append remaing characters in stack to post expression
    while(!st.empty())
    {
        post += st.top();
        st.pop();
    }


    return post;

}


/**********************
 * next_equation
 * Recieves: string
 * Returns: QString
 * Purpose: get a new equation of a specified diffuculty, compute and store
 *          the postfix answer, and return the infix equation in a QString
 ***********************/
QString Model::next_equation(std::string diff)
{
    QString str;
    std::string eq;
    std::string post;

    // get new equation of specific difficulty
    eq = equations.equation_select(diff);

    equation =  QString::fromStdString(eq); //

    // compute and store the postfix answer
    post = infix_to_postfix(eq);
    answer = QString::fromStdString(post);

    // copy infix equation into QString to return
    str = QString::fromStdString(eq);
    cout << post << endl;

    return str;
}

/*******************
checkAnswerBox
Recieves: 2 floats
Returns: void
Purpose: check if there is already a body in which you want to put the currently seletecd body
         if there is, put it in the game board
********************/
void Model::checkAnswerBox(float x, float y){
    for(b2Body* iter = world->GetBodyList(); iter != 0; iter=iter->GetNext()){
        if(iter->GetPosition().x==x && iter->GetPosition().y ==y){
            iter->SetTransform(b2Vec2(x, 180/scale), iter->GetAngle());
            iter->SetType(b2_dynamicBody);
            iter->SetActive(true);
            break;
        }
    }
}

/*******************
checkAnswerBox
Recieves: 2 floats
Returns: void
Purpose: receives the coordinate of where user clicked, convert them to world scale and find any body is close to it.
********************/
void Model::findBodyToGrab(float x, float y){

    // look at all the bodies until it finds one to grab
    for(b2Body* iter = world->GetBodyList();iter!=0; iter=iter->GetNext()){
        // if it finds one
        if( abs(x/scale-iter->GetPosition().x)<1 && abs(y/scale-iter->GetPosition().y)<1){
            // curBody get the pointer of the body to move it
            if(static_cast<spriteInfo*>(iter->GetUserData())->movable==true){
                curBody = iter;
                curBody->SetType(b2_staticBody);
                curBody->SetTransform(b2Vec2(x/scale,y/scale), curBody->GetAngle());

                // send signal that user grabbed a body
                emit grabbedBody();
                break;
            }
        }
    }
}

/*******************
setBodyPosition
Recieves: 2 floats
Returns: void
Purpose: move curBody to (x/scale, y/scale)
********************/
void Model::setBodyPosition(float x, float y){

    // if the body tries to get out of the frame, reloacte it
    if(curBody != NULL){
        if(x<=55)
            x=57;
        else if(x>=565)
            x=560;

        if(y<=35)
            y=40;
        else if(y>=406)
            y=406;


        curBody->SetTransform(b2Vec2(x/scale,y/scale),curBody->GetAngle());
    }
}

/*******************
dropBody
Recieves:
Returns: void
Purpose: when user release the mouse button, user cannot move the mody anymore
********************/
void Model::dropBody(){

    float x=curBody->GetPosition().x*scale;
    float y=curBody->GetPosition().y*scale;

    // if body is dropped somewhere in answer box, it goes to where it should be to organize answer box
    if(x>=115 && x<=515 && y>=355 && y<= 425 ){
        int answerLength = answer.length();
        for(int i=0; i<answerLength; i++){
            if(x<= 115+ (i+1)*405.f/answerLength){
                checkAnswerBox((115+(i+0.5)*405.f/answerLength)/scale,390/scale);
                curBody->SetTransform(b2Vec2( (115+(i+0.5)*405.f/answerLength)/scale,390/scale),curBody->GetAngle());
                break;
            }
        }
    }

 // if a body is dropped in the game board, body type is set to dynamic. Otherwise, to static.
 if(y< 355)
     curBody->SetType(b2_dynamicBody);
 else
     curBody->SetType(b2_staticBody);

    curBody = NULL;

    // check answer box to decide whether enables submit button or not
    checkUserAnswer();

    int count = 0;
    for(int i = 0; i < userAnswer.length(); i++){
        if(userAnswer[i] != " "){
            count++;
        }
    }

    if(count == answer.length()){
        emit enableSubmit();
    }
    else{
        emit disableSubmit();
    }
}

/*******************
checkUserAnswer
Recieves:
Returns: void
Purpose: check answer box and get what user put
********************/
void Model::checkUserAnswer(){
    userAnswer = QString("");


    for(b2Body* iter= world->GetBodyList(); iter != 0; iter=iter->GetNext()){

        if(iter->GetPosition().y==390/scale){

            for(int i=0; i< answer.length(); i++){
                if(iter->GetPosition().x>= (115+i*405.f/answer.length())/scale && iter->GetPosition().x <= (115+(i+1)*405.f/answer.length())/scale){

                    userAnswer[i]= (static_cast<spriteInfo*>(iter->GetUserData())->name)[0];
                    break;
                }
            }
        }
    }
}

void Model::gameLost(){
    correctAnswersCount = 0;
    wrongAnswerCount = 0;
    currentDifficulty = "easy";

    //clear bodies
    for(b2Body* iter= world->GetBodyList();iter!=0; iter=iter->GetNext()){
        world->DestroyBody(iter);
    }

    emit displayLossMessage("O great and powerful dragon lives on! \n \n Press the start button to try again.", true);

    emit changeEquation("");
    emit setEndGameButtons();
}

QString Model::getDifficulty(){
    if(currentDifficulty == "easy"){
        return "Easy";
    }
    else if(currentDifficulty == "medium"){
        return "Medium";
    }
    else{
        return "Hard";
    }
}


