#include <stdio.h>
#include <stdlib.h>
#include "Os.h"

int main()
{

    init();
    create_area();
    read_area();
    //display(area,NULL,"FreeArea");
    //firstfit();
    //bestfit();
    worstfit();
    retrieve();
    display(area,NULL,"FreeArea");
    display(NULL,thread,"Thread");
    destroy();
    return 0;
}
