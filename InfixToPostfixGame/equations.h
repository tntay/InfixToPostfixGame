// This file contains the declaration of the equations class.
// The equation class can read valid arithmetic equations
// from a text file and will store them in a array. There
// will be three arrays: easy, medium, and hard. These
// arrays will contain arithmetic expressions of varying
// complexities. The euqations class will also allow random
// equations to be selected from one of the diffuculty vectors.
// The arithmetic expressions will only contain the operators +,
// -, *, / and will allow both numeric and alphabetic operands.

#ifndef EQUATIONS_H
#define EQUATIONS_H

#include <string>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <time.h>

class Equations
{
public:
    /* constructor */
    Equations();

    /* accessor function */

    /****************************
    equation_selct
    Recieves: string of which array to get equation from.
    Returns: string equation
    Purpose: select a random equation from the array of the diffuculty
             specified by the parameter. Parameter expected is one of three
             choices: easy, medium, hard. Function will do its best to not
             return the same equation to often by keeping track of the past
             two recent equations returned so as to not return those again.
    ****************************/
    std::string equation_select(std::string diff);


    /* mutator function */

    /****************************
    equations_from_file
    Recieves: string of which diffuculty to store equations in,
              string that contains the filename to read from.
    Returns: N/A
    Purpose: fill the array specified with the diffuculty recieved with the
             equations in the text file recieved. Diffuculty parameter expects one
             of three choices: easy, medium, hard.
    *****************************/
    void equations_from_file(std::string diff, std::string filename);


private:
    // attributes
    std::vector<std::string> easy;
    std::vector<std::string> medium;
    std::vector<std::string> hard;
    int prevIndex;
    int prevIndex2;

};

#endif // EQUATIONS_H
