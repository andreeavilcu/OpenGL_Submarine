#include "SubmarineProgram.h"

int main()
{
    SubmarineProgram* submarineProgram = SubmarineProgram::getInstance();
    submarineProgram->runProgram();
    
    delete submarineProgram;
    return 0;
}
