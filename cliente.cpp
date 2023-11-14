# include <stdio.h>
# include <stdlib.h>
# include <iostream>
# include "./include/utils.h"

int main(int argc, char const *argv[])
{
    
    std::cout<<"Cliente iniciado\n";

    auto serverConnection=initClient("127.0.0.1",15000);

    return 0;
}
