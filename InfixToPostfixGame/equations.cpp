// This file contains the impementation of the Equations
// class.

#include "equations.h"

/* constuctor */
Equations::Equations()
{
    srand(time(NULL));
    prevIndex = 0;
    prevIndex2 = 0;
}


/* equation_select */
std::string Equations::equation_select(std::string diff)
{
    std::string expr;
    int index;

    // decide which array we are going to get an equation from
    // and get a random index in the range of the array we are
    // indexing into.
    if(diff == "easy")
    {
        // get a random index that is different than the last two indexes used
        index = rand() % easy.size();
        while(index == prevIndex || index == prevIndex2)
        {
            index = rand() % easy.size();
        }

        // update the previous indexes
        prevIndex2 = prevIndex;
        prevIndex = index;

        return easy[index];
    }
    else if(diff == "medium")
    {
        // get a random index that is different than the last two indexes used
        index = rand() % medium.size();
        while(index == prevIndex || index == prevIndex2)
        {
            index = rand() % easy.size();
        }

        // update the previous indexes
        prevIndex2 = prevIndex;
        prevIndex = index;

        return medium[index];
    }
    else if(diff == "hard")
    {
        // get a random index that is different than the last two indexes used
        index = rand() % hard.size();
        while(index == prevIndex || index == prevIndex2)
        {
            index = rand() % hard.size();
        }

        // update the previous indexes
        prevIndex2 = prevIndex;
        prevIndex = index;

        return hard[index];
    }
    else
    {
        return "Invalid Diffuculty";
    }

}


/* equations_from_file */
void Equations::equations_from_file(std::string diff, std::string filename)
{
    std::vector<std::string> *ptr;
    std::ifstream infile;
    std::string str;

    // check which array to fill
    if(diff == "easy")
    {
        ptr = &easy;
    }
    else if(diff == "medium")
    {
        ptr = &medium;
    }
    else if(diff == "hard")
    {
        ptr = &hard;
    }
    else
    {
        return;
    }

    // open the file
    infile.open(filename);

    // check if file was opened
    if(!infile)
    {
        std::exit(12);
        return;
    }

    // read from file
    while(infile >> str)
    {
        ptr->push_back(str);
    }

    // close file
    infile.close();
}


