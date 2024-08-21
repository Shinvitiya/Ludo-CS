#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

//--------Defining Structures-----------//

int testCaptures = 0; //Remove this
int testBlocks = 0;

struct Piece {
    int location; //Tracks which cell the piece is curerntly on
    int direction; //Tracks is the player should move clockwise or anticlockwise
    int steps; // Keeps track of the steps the player has taken with respective to reaching the home straight---- 54 in anticlockwise direction and 52 in clockwise direction
    int captures; 
    bool isAtBase; //Tracks if the piece is drawn or at the base. 
    bool isInBlock;
    char pieceId[3];
};

struct Block {
    struct Piece *pieces[4];
    int direction;
    int location;
    int dieValueDivider;
    int steps;
    bool isNull; 
};

struct Player {
    struct Piece piece1;
    struct Piece piece2;
    struct Piece piece3;
    struct Piece piece4;

    //Two Blocks are called since the maximum number if blocks a player can create is 2
    struct Block block1; 
    struct Block block2;
    
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

//Checks if piece can move(Due to blocks or not sufficient blocks in home straight)
bool canMove(){
    return true;
}

//Checks if the opponents have created a block and whether the piece cqn move forward or not
bool isPathBlocked(){
    return false;
}

//Move  player piece based on the value rolled and the direction of movement
void movePiece(int steps, struct Piece *piece, char color[10]){
    int prevLocation = piece->location;
    piece->location = (52 + piece->location + piece->direction * steps) % 52;
    piece->steps = piece->steps + steps;

    printf("%s moves piece %s from location L%d to L%d by %d units in ", color,piece->pieceId, prevLocation, piece->location, steps);
    piece->direction == 1? printf("Clockwise Direction\n\n"):printf("AntiClockwise Direction \n\n");
}

int getBlockDirection(struct Block *block){

    // for{int i=0; i<4; i++}{
    //     block.
    // }
    return;
}

int getBlockStepDividers(struct Block *block){
    return;
}

int getBlockSteps(struct Block *block){
    return;
}

bool canCreateBlock(struct Player *player, int steps){

    struct Piece pieces[4] = {player->piece1, player->piece2, player->piece3, player->piece4};
    struct Block blocks[2] = {player->block1, player->block2};

    //When the player has no blocks, checks if a block can be made by moving a piece;
    if(player->block1.isNull && player->block2.isNull){

        for(int i = 0; i<3; i++){ 
            for(int j=i+1; j<4; j++){
                int newLocation = (52 + pieces[i].location + pieces[i].direction * steps) % 52;
                if( newLocation == pieces[j].location){
                    printf("A block can be been created");
                    return true;
                }
            }
        }
    }

    //When a player has two blocks checks if moving one block can create a larger block with the other
    if(!player->block1.isNull && !player->block2.isNull){

        int newBlock1Location = (52 + player->block1.location + player->block1.direction * (steps / player->block1.dieValueDivider));
        int newBlock2Location = (52 + player->block2.location + player->block2.direction * (steps / player->block2.dieValueDivider));

        if( newBlock1Location == player->block2.location || newBlock2Location == player->block2.location){
            return true;
        }
    }

    //When one block exists, checks if moving a piece or block can create a larger block
    for(int i=0; i<2; i++){

        //Checks if the block exists
        if(!blocks[i].isNull){

            for(int j=0; j<4; j++){
                //Checks for pieces that are not in a block
                if(!pieces[j].isInBlock){

                    int newBlockLocation = (52 + blocks[i].location + blocks->direction * (steps / blocks[i].dieValueDivider)) %52; //New location of the block if you decide to move it
                    int newPieceLocation = (52 + pieces[j]. location + pieces[j].direction * steps) %52; //New location of the piece if you decide you move it

                    //Checks if moving the block to a new location or moving a piece to a new location can create a (larger) block
                    if(newBlockLocation == pieces[j].location || newPieceLocation == blocks[i].location){
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

//Creates a block
void createBlock(struct Player *player, int steps){

    struct Piece* pieces[4] = {&player->piece1, &player->piece2, &player->piece3, &player->piece4};
    struct Block* blocks[4] = {&player->block1, &player->block2};


    //Moves a piece that isnt in a block, to create a block.
    for(int i=0; i<3; i++){

        //If the piece is already in a block, moves the loop to the next piece   
        if(pieces[i]->isInBlock){
            continue;
        }
        int newLocation = (52 + pieces[i]->location + pieces[i]->direction * steps) % 52;

        for(int j=0; j<4; j++){
            //Moves to the next piece if the two pieces are the same
            if( pieces[i] == pieces[j]){
                continue;
            }    

            if(newLocation == pieces[j]->location){
                pieces[i]->isInBlock = true;
                pieces[i]->location = newLocation;
                pieces[i]->steps += steps;

                //If both blocks of the player are empty, add the values to block1
                if(player->block1.isNull && player->block2.isNull){

                    player->block1.pieces[0] = pieces[i];
                    player->block1.pieces[1] = pieces[j];
                    player->block1.isNull = false;
                    //Set block Direction, location and steps, step divider
                    return;
                }

                //If the first block is occpied and the new location doesnt match the location of the first block
                if(player->block2.isNull && !player->block1.isNull && player->block1.location == !newLocation){
                    player->block2.pieces[0] = pieces[i];
                    player->block2.pieces[1] = pieces[j];
                    player->block2.isNull = false;
                    //Set block Direction, location and steps, step divider
                    return;
                }

                //If both blocks are not empty, adds the values to the relevant block
                for(int k=0; k<2; k++){
                    if(!blocks[k]->isNull && blocks[k]->location == newLocation){  //Check login Here

                        for (int l = 0; l < 4; l++) {
                            if (blocks[k]->pieces[l] == NULL) {
                                blocks[k]->pieces[l] = pieces[i];
                                break;
                            }
                        }

                        //Set block Direction, location and steps, step divider
                        return;
                    }
                }
            }
        }
    }

    //If there exists no piece that isnt in a block, then all pieces are in a block
    //Since createBlock() is only implemented when canCreateBlock() returns, it means, the players have two blocks each with two pieces
    int block1NewLocation = (52 + player->block1.location + player->block1.direction * steps /player->block1.dieValueDivider) % 52;
    int block2NewLocation = (52 + player->block2.location + player->block2.direction * steps /player->block2.dieValueDivider) % 52;

    if(block1NewLocation == player->block2.location){
        for(int i=0; i < 4; i++){
            for(int j=0; j<4; j++){
                if(player->block2.pieces[j] == NULL && player->block1.pieces[i] != NULL){
                    
                    player->block2.pieces[j] = player->block1.pieces[i];
                    player->block1.pieces[i] = NULL;

                    player->block1.dieValueDivider = 1;
                    player->block1.direction = 1;
                    player->block1.isNull = true;
                    player->block1.location = 0;
                    player->block1.steps = 0;
                }
            }
        }

        //Code to calculate block direction, steps step divider etc
        return;
    }
    if(block2NewLocation == player->block1.location){
        for(int i=0; i < 4; i++){
            for(int j=0; j<4; j++){
                if(player->block1.pieces[j] == NULL && player->block2.pieces[i] != NULL){
                    
                    player->block1.pieces[j] = player->block2.pieces[i];
                    player->block2.pieces[i] = NULL;
                    player->block2.dieValueDivider = 1;
                    player->block2.direction = 1;
                    player->block2.isNull = true;
                    player->block2.location = 0;
                    player->block2.steps = 0;
                }
            }
        }

        //Sets new direction, steps etc of block, step divider
        return;
    }
}

void moveBlock(){

    return;
}

//Checks if a piece of another player can be captured
bool canCapture(struct Piece *currentPiece, struct Player *opponent, int steps) {

    // struct Piece* currentPieces[4] = {&currentPlayer->piece1, &currentPlayer->piece2, &currentPlayer->piece3, &currentPlayer->piece4};

    struct Piece* opponentPieces[4] = {&opponent->piece1, &opponent->piece2, &opponent->piece3, &opponent->piece4};

    for(int i =0; i<4; i++){

        int newLocation = (52+ currentPiece->location + (steps * currentPiece->direction)) %52;

        if((newLocation == opponentPieces[i]->location) && (opponentPieces[i]->steps > 1) ){
            return true;
        }
    }
    return false;
}

//Finds the opponent pieces that can be captured and moves a piece that can capture it
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
                    if(((newLocation == opponentPieces[k]->location) && !currentPlayerPieces[j]->isAtBase && opponentPieces[k]->steps > 1)){

                        //Moving the current player's piece to capture the opponent's piece and increments captures by 1
                        currentPlayerPieces[j]->location = newLocation;
                        currentPlayerPieces[j]->steps += steps;
                        currentPlayerPieces[j]->captures += 1;

                        printf("%s piece %s lands on square L%d, captures %s piece %s and returns it to base", currentPlayer->color, currentPlayerPieces[j]->pieceId, newLocation, players[i]->color, opponentPieces[k]->pieceId);

                        //Resetting the opponent's piece that was captured
                        opponentPieces[k]->isAtBase = true;
                        opponentPieces[k]->location = 0;
                        opponentPieces[k]->steps = 0;
                        opponentPieces[k]->captures = 0;
                        opponentPieces[k]->isInBlock = false;
                        opponentPieces[k]->direction = 1;

                        testCaptures = 1; //Remove this

                        return;
                    }
                }
            }
        }
    }
}

//Moves piece (X) by the based on the die roll and any captures opponents piece if it is on the square X lands on 
void moveOrCapture(int steps, struct Piece *playerPiece, char color[10], struct Player *allPlayers[4]){

    movePiece(steps, playerPiece, color); //Change to normal
    for(int i=0; i<4; i++){

        if (strcmp(color, allPlayers[i]->color) != 0) {

            struct Piece *opponentPieces[4] = {&allPlayers[i]->piece1, &allPlayers[i]->piece2, &allPlayers[i]->piece3, &allPlayers[i]->piece4};
        
            for(int j=0; j<4; j++){
                if(playerPiece->location == opponentPieces[j]->location){
                    opponentPieces[j]->captures = 0;
                    opponentPieces[j]->direction = 1;
                    opponentPieces[j]->isAtBase = true;
                    opponentPieces[j]->isInBlock = false;
                    opponentPieces[j]->location = 0;
                    opponentPieces[j]->steps = 0;

                    printf("%s piece %s lands on square L%d, captures %s piece %s and returns it to base",color, playerPiece->pieceId, playerPiece->location, allPlayers[i]->color, opponentPieces[j]->pieceId);
                }
            }
        }        
    }
    return;
}

//Defining behaviour for each colored player

//Yellow Player Behaviour
void yellowPlayerBehaviour(struct Player *yellow, int consecutiveSixes, struct Player* players[4]) {
    int dieValue = rollDie();
    printf("Yellow has rolled %d\n", dieValue);

    struct Piece *playerPieces[4] = {&yellow->piece1, &yellow->piece2, &yellow->piece3, &yellow->piece4};
    struct Piece *max = NULL; // Initialize to NULL

    bool capturePossible = false;
    for (int i = 0; i < 4; i++) {
        for(int j=0; j<4; j++){
            if (players[i] != yellow && canCapture(playerPieces[i], players[j], dieValue) ) {
                capturePossible = true;
                break;
            }
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

            // Find the closest piece to home that is not at the base
            for(int i = 0; i < 4; i++){
                if(!playerPieces[i]->isAtBase && (max == NULL || playerPieces[i]->steps >  max->steps) && canMove() ){
                    max = playerPieces[i];
                }
            }

            // Move the piece if found
            if(max != NULL){
                movePiece(dieValue, max, yellow->color);
            }else{
                printf("No pieces can be moved\n");
            }
        }
        yellowPlayerBehaviour(yellow, consecutiveSixes, players);

    } else {

        if (capturePossible) {
            capturePiece(players, yellow, dieValue);
            yellowPlayerBehaviour(yellow, consecutiveSixes, players);
        } 
        else {
            // Find the closest piece to home that is not at the base
            for(int i = 0; i < 4; i++){
                if(!playerPieces[i]->isAtBase && (max == NULL || playerPieces[i]->steps > max->steps) && canMove() ){
                    max = playerPieces[i];
                }
            }

            // Move the piece if found
            if(max != NULL){
                movePiece(dieValue, max, yellow->color);
            }else{
                printf("No pieces can be moved\n");
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

void greenPlayerBehaviour(struct Player *green, int consecutiveSixes, struct Player* players[4]){

    struct Piece *playerPieces[4] = {&green->piece1, &green->piece2, &green->piece3, &green->piece4};
    struct Piece *max = NULL;

    int dieValue = rollDie();

    printf("Green has rolled %d\n", dieValue);

    if(dieValue == 6){ //Logic when player rolls a six
        consecutiveSixes++;

        if (consecutiveSixes == 3) {
            printf("Green rolled three consecutive 6s! Turn is passed to the next player.\n");
            //Insert logic to break blocks if any
            greenPlayerBehaviour(green, consecutiveSixes, players);
            return;
        }

        //Moves a piece to create a block if a block can be created
        if(canCreateBlock(green, dieValue)){
            createBlock(green, dieValue); 
            greenPlayerBehaviour(green, consecutiveSixes, players);
            return;
        }

        //Draws a piece out from the base if a block cannot be created
        if(arePiecesAtBase(green)){
            drawPiece(green);
            greenPlayerBehaviour(green, consecutiveSixes, players);
            return;
        }

        //If there is a block, move it
        if(true){
            // moveBlock();
            return;
        }

        //Moves the piece closest to home when a block cant be created, an existing block cant be moved and no pieces are at base

        //Sets the peices that is drawn from base, is closest to home, can move and is not in a block to max
        for(int i = 0; i < 4; i++){
            if((max == NULL || playerPieces[i]->steps > max->steps) && (playerPieces[i]->isAtBase) && canMove()){
                max = playerPieces[i];
            }
        }

        if(max != NULL){
            // Move the max piece if found
            moveOrCapture(dieValue, max, green->color, players);
            
        }else{
            //breaks a block as no other piece can be moved 
            if(max != NULL){ 
                max->isInBlock = false; //Breaks max piece from the block
                moveOrCapture(dieValue, max, green->color, players);

            }else{
                printf("No pieces can be moved\n");
            } 
        }
        greenPlayerBehaviour(green, consecutiveSixes, players);

    }else{ //Logic when player rolls a valie that isn't a six
        printf("This logic isnt set yet\n");
    }


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
        greenPlayerBehaviour(player, consecutiveSixes, players);
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
        if(count >100){
                break;
        }
    }
}

void playLudo() {
    srand(time(NULL)); // Seed the random number generator
    // Initialize players
    struct Player yellow = {
        {0, 1, 0, 0, true, false, "Y1"}, //Piece - location,direction,steps,captures,isAtBase,isInBlock,pieceID
        {0, 1, 0, 0, true, false, "Y2"}, 
        {0, 1, 0, 0, true, false, "Y3"}, 
        {0, 1, 0, 0, true, false, "Y4"},

        { {NULL, NULL, NULL, NULL}, 1, 0, 1, 0, true},  //Block - pieces, direction, location, isNUll
        { {NULL, NULL, NULL, NULL}, 1, 0, 1, 0, true},
        0, "Yellow" //Playorder, Player Color
    };

    struct Player blue = {
        {13, 1, 0, 0, true, false, "B1"}, 
        {13, 1, 0, 0, true, false, "B2"}, 
        {13, 1, 0, 0, true, false, "B3"}, 
        {13, 1, 0, 0, true, false, "B4"},

        { {NULL, NULL, NULL, NULL}, 1, 0, 1, 0, true},
        { {NULL, NULL, NULL, NULL}, 1, 0, 1, 0, true},

        0, "Blue"
    };

    struct Player red = {
        {26, 1, 0, 0, true, false, "R1"}, 
        {26, 1, 0, 0, true, false, "R2"}, 
        {26, 1, 0, 0, true, false, "R3"}, 
        {26, 1, 0, 0, true, false, "R4"},
        
        { {NULL, NULL, NULL, NULL}, 1, 0, 1, 0, true},
        { {NULL, NULL, NULL, NULL}, 1, 0, 1, 0, true},

        0, "Red"
    };

    struct Player green = {
        {39, 1, 0, 0, true, false, "G1"},
        {39, 1, 0, 0, true, false, "G2"}, 
        {39, 1, 0, 0, true, false, "G3"}, 
        {39, 1, 0, 0, true, false, "G4"},
        
        { {NULL, NULL, NULL, NULL}, 1, 0, 1, 0, true},
        { {NULL, NULL, NULL, NULL}, 1, 0, 1, 0, true},

        0, "Green"
    };

    printIntro();
    playerOrder(&yellow, &blue, &red, &green);
    startGame(&yellow, &blue, &red, &green);
    printf("Green steps: %d\n", green.piece1.steps);

    printf("Test value is: %d\n", testCaptures);
    printf("Test value is: %d\n", testBlocks);

    printf("\n");
}

//-----------Main Function------------------//
int main() {
    playLudo();
    return 0;
}