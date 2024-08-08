#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

//--------Defining Structures-----------//

struct Piece {
    int location; //Tracks which cell the piece is curerntly on
    int direction; //Tracks is the player should move clockwise or anticlockwise
    bool isAtBase; //Tracks if the piece is drawn or at the base. 
    char pieceId[2];
};

struct Player {
    struct Piece piece1;
    struct Piece piece2;
    struct Piece piece3;
    struct Piece piece4;
    
    int playOrder;
    char color[10];
};

//-------Defining Functions--------------//

// Returns a random value between 1 and 6 to imitate the dice roll
int rollDie() {
    return (rand() % 6) + 1;
}

//Returns true if the player should move clockwise and false if anti-clockwise
int shouldMoveClockwise(){
    int direction = (rand() % 2);
    return (direction == 0? 1 : -1);
}

// Returns the playing order based on the value of the dice roll
//Input  player parameters in clockwise order starting from yellow
void playerOrder(
    struct Player *player1,
    struct Player *player2, 
    struct Player *player3, 
    struct Player *player4
){
    struct Player* players[4] = {player1, player2, player3, player4};
    int rolls[4];

    // Initial roll for all players 
    for(int i = 0; i < 4; i++){
        rolls[i] = rollDie();
    }

    //Making sure there are no ties in the rolls and rerolling if there are 
    int numberOfTies;
    do {
        numberOfTies = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = i + 1; j < 4; j++) {

                if (rolls[i] == rolls[j]) {
                    rolls[i] = rollDie();
                    rolls[j] = rollDie();
                    numberOfTies++;
                }
            }
        }
    } while (numberOfTies > 0);

    // Print final rolls used to calculate play order
    printf("\n");
    for(int i = 0; i < 4; i++){
        printf("%s rolls %d\n", players[i]->color, rolls[i]);
    }
    printf("\n");

    int maxIndex = 0;
    for (int i = 1; i < 4; i++) {
        if (rolls[i] > rolls[maxIndex]) {
            maxIndex = i;
        }
    }

    //Assign play orders based on the highest roll and then the clockwise order
    for (int i = 0; i < 4; i++) {
        players[(maxIndex + i) % 4]->playOrder = i + 1;
    }

    //Displaying the player order
    printf("%s player has the highest roll and will begin the game.\n", players[0]->color);

    printf("The order of a single round is ");
    for (int i = 0; i < 4; i++) {
        int index = (maxIndex + i) % 4;
        if(i==3){
            printf("and ");
        }

        printf("%s", players[index]->color);

        if(i<3){
            printf(", ");
        }else{
            printf(".\n");
        }
    }

}

//Move a player piece to the base and set it's direction
void drawPlayer(struct Player *player){
    struct Piece* pieces[4] = {&player->piece1, &player->piece2, &player->piece3, &player->piece4};
    
    for(int i = 0; i < 4; i++) {
        if (!pieces[i]->isAtBase) {
            pieces[i]->isAtBase = true; //Sets base to true
            pieces[i]->direction = shouldMoveClockwise(); //Sets piece direction
            printf("\n%s player moves piece %s to base\n", player->color, pieces[i]->pieceId);
            return;
        }
    }
}

//Move  player piece based on the value rolled and the direction of movement
void movePiece(int steps, struct Piece *piece, char color[10]){
    int prevLocation = piece->location;
    piece->location = (52 + piece->location + piece->direction * steps) % 52;

    printf("\n%s moves piece %s from location L%d to L%d by %d units in ", color,piece->pieceId, prevLocation, piece->location, steps);
    piece->direction == 1? printf("Clockwise Direction"):printf("AntiClockwise Direction \n");
}


bool hasPlayerWon(struct Player *yellow,struct Player *blue, struct Player *red, struct Player *green){

    struct Player* players[4] = {&yellow, &blue, &red, &green}; 
}

void startGame(){

}

void playLudo() {
    srand(time(NULL)); // Seed the random number generator
    // Initialize players
    struct Player yellow = {
        {0, 1, false, "P1"}, {0, 1, false, "P2"}, {0, 1, false, "P3"}, {0, 1, false, "P4"}, //Setting location, base, and name
        0, "Yellow" //Setting playorder and color
    };
    struct Player blue = {
        {13, 1, false, "P1"}, {13, 1, false, "P2"}, {13, 1, false, "P3"}, {13, 1, false, "P4"},
        0, "Blue"
    };
    struct Player red = {
        {26, 1, false, "P1"}, {26, 1, false, "P2"}, {26, 1, false, "P3"}, {26, 1, false, "P4"},
        0, "Red"
    };
    struct Player green = {
        {39, 1, false, "P1"}, {39, 1, false, "P2"}, {39, 1, false, "P3"}, {39, 1, false, "P4"},
        0, "Green"
    };

    playerOrder(&yellow, &blue, &red, &green);
    drawPlayer(&yellow);
    movePiece(6, &yellow.piece1, yellow.color);

    printf("\n");
}

//-----------Main Function------------------//
int main() {
    playLudo();
    return 0;
}