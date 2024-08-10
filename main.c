#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

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
//Print intro
void printIntro(){
    printf("\nThe Yellow player has four (04) pieces named Y1, Y2, Y3, and Y4 \n");
    printf("The BLue player has four (04) pieces named B1, B2, B3, and B4 \n");
    printf("The Red player has four (04) pieces named R1, R2, R3, and R4 \n");
    printf("The Green player has four (04) pieces named G1, G2, G3, and G4 \n");
}
// Returns a random value between 1 and 6 to imitate the dice roll
int rollDie() {
    return (rand() % 6) + 1;
}

//Returns 1 if the player should move clockwise and -1 if anti-clockwise
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
    printf("%s player has the highest roll and will begin the game.\n", players[maxIndex]->color);

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

//Checks if any players are still at base
bool arePiecesAtBase(struct Player *player){
    if(player->piece1.isAtBase){
        return true;
    }
    if(player->piece2.isAtBase){
        return true;
    }
    if(player->piece3.isAtBase){
        return true;
    }
    if(player->piece4.isAtBase){
        return true;
    }
    return false;
}
//Print the number or players at the base and on the board
void getPlayerCount(struct Player *player){
    struct Piece* pieces[4] = {&player->piece1, &player->piece2, &player->piece3, &player->piece4};

    //Calculates how many players are at the base
    int playersAtBaseCount =0;
    for(int j=0; j<4; j++){
        if(pieces[j]->isAtBase){
            playersAtBaseCount++;
        }
    }
    printf("%s player now has %d/4 on the board and %d/4 pieces on the base\n", player->color, 4 - playersAtBaseCount, playersAtBaseCount);
}

//Move a player piece to the base and set it's direction
void drawPiece(struct Player *player){
    struct Piece* pieces[4] = {&player->piece1, &player->piece2, &player->piece3, &player->piece4};
    
    for(int i = 0; i < 4; i++) {
        if (pieces[i]->isAtBase) {
            pieces[i]->isAtBase = false; //Sets isAtBase to false
            pieces[i]->direction = shouldMoveClockwise(); //Sets piece direction

            printf("%s player moves piece %s to the starting point.\n", player->color, pieces[i]->pieceId);
            
            getPlayerCount(player);

            return;
        }
    }
}

//Move  player piece based on the value rolled and the direction of movement
void movePiece(int steps, struct Piece *piece, char color[10]){
    int prevLocation = piece->location;
    piece->location = (52 + piece->location + piece->direction * steps) % 52;

    printf("%s moves piece %s from location L%d to L%d by %d units in ", color,piece->pieceId, prevLocation, piece->location, steps);
    piece->direction == 1? printf("Clockwise Direction\n\n"):printf("AntiClockwise Direction \n\n");
}

//Checks if a block can be created
bool canBlock(struct Player *player, int steps){
    struct Piece pieces[4] = {player->piece1, player->piece2, player->piece3, player->piece4};

    //Loops through the pieces to check if moving the piece can create a block
    for(int i = 0; i<3; i++){ 
        for(int j=i+1; j<4; j++){
            int newLocation = (52 + pieces[i].location + pieces[i].direction * steps) % 52;
            if( newLocation == pieces[j].location){
                printf("A block can be been created");
                return true;
            }
        }
    }
    return false;
}

//Creates a block
void createBlock(int steps){

}

//Checks if a piece of another player is caputrable
bool canCapture(){

    return false;
}

//Captures the piece of another player
void capturePiece(struct Player *player){

}

//Defining behaviour for each colored player

//Yellow Player Behaviour
void yellowPlayerBehaviour(struct Player *yellow, int consecutiveSixes){

    int dieValue = rollDie();
    printf("Yellow has rolled %d\n", dieValue);

    //Checks if there are any players in the base and moves them to the starting point. If there arent any, then moves the piece
    if(dieValue == 6){
        consecutiveSixes++;
        
        //Ends the players turn and passes it to the next player if he rolls a 6 for the third time
        if (consecutiveSixes == 3) {
            printf("Yellow rolled three consecutive 6s! Turn is passed to the next player.\n");
            return; // Ends the player's turn
        }

        if(arePiecesAtBase(yellow)){
            drawPiece(yellow);
            yellowPlayerBehaviour(yellow, consecutiveSixes);
        }else{
            movePiece(dieValue, &yellow->piece1, yellow->color);
            yellowPlayerBehaviour(yellow, consecutiveSixes);
        } 

    }else{
        if(!yellow->piece1.isAtBase){
        movePiece(dieValue, &yellow->piece1, yellow->color);
        }
    }
}

void bluePlayerBehaviour(){
    printf("This is the blue player's AI\n");
}

void redPlayerBehaviour(){
    printf("This is the red player's AI\n");
}

void greenPlayerBehaviour(){
    printf("This is the green player's AI\n");
}

//Call player behviour based on the piece's color
void getBehaviour(char color[10], struct Player *player) {
    int consecutiveSixes = 0;
    if (strcmp(color, "Yellow") == 0) {
        yellowPlayerBehaviour(player, consecutiveSixes);

    } else if (strcmp(color, "Blue") == 0) {
        bluePlayerBehaviour();

    } else if (strcmp(color, "Red") == 0) {
        redPlayerBehaviour();

    } else {
        greenPlayerBehaviour();
    }
}
 
 //Starts the game looping through each player or each turn until 3 players have brought all 4 pieces hom
void startGame(
    struct Player *player1, 
    struct Player *player2,
    struct Player *player3,
    struct Player *player4
){
    struct Player* players[4] = {player1, player2, player3, player4};
    int currentTurn = 1;
    int count = 0;

    printf("\n");

    while (true){
        for(int i = 0; i<4; i++){
            if(players[i]->playOrder == currentTurn)
            {
                getBehaviour(players[i]->color, players[i]);
                printf("\n");

                // for(int j=0; j<4; j++){
                //     getPlayerCount(players[j]);
                // }

                currentTurn = (currentTurn % 4) + 1;
                break; 
            }    
        }
        

        //Loop controller Remove this!!!!!!!!!!!
        count++;
        if(count >40){
                break;
        }
    }
}

void playLudo() {
    srand(time(NULL)); // Seed the random number generator
    // Initialize players
    struct Player yellow = {
        {0, 1, true, "Y1"}, {0, 1, true, "Y2"}, {0, 1, true, "Y3"}, {0, 1, true, "Y4"}, //Setting location, base, and name
        0, "Yellow" //Setting playorder and color
    };
    struct Player blue = {
        {13, 1, true, "B1"}, {13, 1, true, "B2"}, {13, 1, true, "B3"}, {13, 1, true, "B4"},
        0, "Blue"
    };
    struct Player red = {
        {26, 1, true, "R1"}, {26, 1, true, "R2"}, {26, 1, true, "R3"}, {26, 1, true, "R4"},
        0, "Red"
    };
    struct Player green = {
        {39, 1, true, "G1"}, {39, 1, true, "G2"}, {39, 1, true, "G3"}, {39, 1, true, "G4"},
        0, "Green"
    };

    printIntro();
    playerOrder(&yellow, &blue, &red, &green);
    startGame(&yellow, &blue, &red, &green);
    // drawPlayer(&yellow);
    // movePiece(6, &yellow.piece1, yellow.color);

    printf("\n");
}

//-----------Main Function------------------//
int main() {
    playLudo();
    return 0;
}