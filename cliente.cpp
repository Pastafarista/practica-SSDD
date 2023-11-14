#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "./utils.h"

int main(int argc, char const *argv[])
{
    
    // iniciar conexión con el servidor
    auto serverConnection=initClient("127.0.0.1",15000);


    return 0;
}
