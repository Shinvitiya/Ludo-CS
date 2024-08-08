#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void initializeGame(){

    srand(time(NULL)); // Seed the random number generator
    
    int result = rollDie();
    printf("The Die Rolled: %d\n", result);
    return 0;
}

int rollDie(){
    return (rand() % 6) +1;
}


