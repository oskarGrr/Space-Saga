#include <time.h>
#include <stdlib.h>

void mainLoop();
void titleScreenLoop();

int main()
{
    srand(time(NULL));
    titleScreenLoop();
    mainLoop();
}