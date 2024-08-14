#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

//--------Defining Structures-----------//
int test = 0;

struct Piece {
    int location; //Tracks which cell the piece is curerntly on
    int direction; //Tracks is the player should move clockwise or anticlockwise
    int steps; // Keeps track of the steps the player has taken with respective to reaching the home straight---- 54 in anticlockwise direction and 52 in clockwise direction
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
            pieces[i]->steps = 1;

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
    piece->steps = piece->steps + steps;

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

//Checks if the opponents have created a block and whether the piece cqn move forward or not
bool isPathBlocked(){
    return false;
}

//Creates a block
void createBlock(int steps){

}

//Checks if a piece of another player can be captured
bool canCapture(struct Player *currentPlayer, struct Player *opponent, int steps) {

    struct Piece* currentPieces[4] = {&currentPlayer->piece1, &currentPlayer->piece2, &currentPlayer->piece3, &currentPlayer->piece4};

    struct Piece* opponentPieces[4] = {&opponent->piece1, &opponent->piece2, &opponent->piece3, &opponent->piece4};

    for(int i =0; i<4; i++){
        for(int j=0; j <4; j++){

            int newLocation = (52+ currentPieces[i]->location + (steps * currentPieces[i]->direction)) %52;

            if((newLocation == opponentPieces[j]->location) && (opponentPieces[j]->steps > 1) ){
                printf("can capture\n");
                return true;
            }
        }
    }
    return false;
}

// Function to capture opponent's pieces
void capturePiece(struct Player *players[4], struct Player *currentPlayer, int steps) {
    struct Piece* currentPlayerPieces[4] = {&currentPlayer->piece1, &currentPlayer->piece2, &currentPlayer->piece3, &currentPlayer->piece4};

    //Looping thorugh all the opponent players
    for(int i=0; i<4; i++){
        if(players[i] != currentPlayer){
            struct Piece* opponentPieces[4] = {&players[i]->piece1, &players[i]->piece2, &players[i]->piece3, &players[i]->piece4};
            
            //Looping through the all the pieces of the current Player
            for(int j=0; j<4; j++){
                int prevLocation = currentPlayerPieces[j]->location;
                int newLocation =  (52 + currentPlayerPieces[j]->location + (steps * currentPlayerPieces[j]->direction)) % 52;

                //Looping thorugh all the pieces of the opponent 
                for(int k=0; k<4; k++){
                    if(((newLocation == opponentPieces[k]->location) && opponentPieces[k]->steps > 1)){

                        //Moving the current player's piece to capture the opponent's piece
                        currentPlayerPieces[j]->location = newLocation;
                        currentPlayerPieces[j]->steps += steps;
                        printf("%s piece %s lands on square L%d, captures %s piece %s and returns it to base", currentPlayer->color, currentPlayerPieces[j]->pieceId, newLocation, players[i]->color, opponentPieces[k]->pieceId);

                        //Resetting the opponent's piece that was captured
                        opponentPieces[k]->isAtBase = true;
                        opponentPieces[k]->location = 0;
                        opponentPieces[k]->steps = 0;

                        test = 1;

                        return;
                    }
                }
            }
        }
    }
}

//Defining behaviour for each colored player

//Yellow Player Behaviour
void yellowPlayerBehaviour(struct Player *yellow, int consecutiveSixes, struct Player* players[4]) {
    int dieValue = rollDie();
    printf("Yellow has rolled %d\n", dieValue);

    bool capturePossible = false;
    for (int i = 0; i < 4; i++) {
        if (players[i] != yellow && canCapture(yellow, players[i], dieValue) ) {
            capturePossible = true;
            break;
        }
    }

    if (dieValue == 6) {
        consecutiveSixes++;

        if (consecutiveSixes == 3) {
            printf("Yellow rolled three consecutive 6s! Turn is passed to the next player.\n");
            return;
        }

        if (arePiecesAtBase(yellow)) {
            drawPiece(yellow);

        } else if (capturePossible) {
            capturePiece(players, yellow, dieValue);

        } else {
            movePiece(dieValue, &yellow->piece1, yellow->color);
        }
        yellowPlayerBehaviour(yellow, consecutiveSixes, players);

    } else {
        if (!yellow->piece1.isAtBase) {
            if (capturePossible) {
                capturePiece(players, yellow, dieValue);
            } else {
                movePiece(dieValue, &yellow->piece1, yellow->color);
            }
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
void getBehaviour(char color[10], struct Player *player, struct Player* players[4]) {
    int consecutiveSixes = 0;

    if (strcmp(color, "Yellow") == 0) {
        yellowPlayerBehaviour(player, consecutiveSixes, players);

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
                getBehaviour(players[i]->color, players[i], players);
                printf("\n");

                currentTurn = (currentTurn % 4) + 1;
                break; 
            }    
        }
        

        //Loop controller Remove this!!!!!!!!!!!
        count++;
        if(count >200){
                break;
        }
    }
}

void playLudo() {
    srand(time(NULL)); // Seed the random number generator
    // Initialize players
    struct Player yellow = {
        {0, 1, 0, true, "Y1"}, 
        {0, 1, 0, true, "Y2"}, 
        {0, 1, 0, true, "Y3"}, 
        {0, 1, 0, true, "Y4"}, // Setting location, direction, steps, base, and name
        0, "Yellow" // Setting playorder and color
    };

    struct Player blue = {
        {13, 1, 0, true, "B1"}, 
        {13, 1, 0, true, "B2"}, 
        {13, 1, 0, true, "B3"}, 
        {13, 1, 0, true, "B4"},
        0, "Blue"
    };

    struct Player red = {
        {26, 1, 0, true, "R1"}, 
        {26, 1, 0, true, "R2"}, 
        {26, 1, 0, true, "R3"}, 
        {26, 1, 0, true, "R4"},
        0, "Red"
    };

    struct Player green = {
        {0, 1, 3, false, "G1"}, //39, 1,0
        {39, 1, 0, true, "G2"}, 
        {39, 1, 0, true, "G3"}, 
        {39, 1, 0, true, "G4"},
        0, "Green"
    };

    printIntro();
    playerOrder(&yellow, &blue, &red, &green);
    startGame(&yellow, &blue, &red, &green);
    printf("Green steps: %d\n", green.piece1.steps);
    printf("Test value is: %d\n", test);

    printf("\n");
}

//-----------Main Function------------------//
int main() {
    playLudo();
    return 0;
}