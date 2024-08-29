#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

#define TOTAL_STEPS 52
//--------Defining Structures-----------//

int testCaptures = 0; //Remove this
int mysteryCellLocation = 1000; //
int landingCount = 0;
int currentBluePiece = 0;

struct Piece {
    int location; //Tracks which cell the piece is curerntly on
    int direction; //Tracks is the player should move clockwise or anticlockwise
    int steps; // Keeps track of the steps the piece has taken with respective to reaching the home straight
    int captures; //Keeps track if the piece has captured a piece
    float aura; //Tracks the aura of the piece
    bool isAtBase; //Tracks if the piece is drawn or at the base. 
    bool isAtHome; //Tracks if teh piece has reached home false,
    bool isInBlock; //Tracks if the piece is in a block.
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
//Print the number of players at the base and on the board
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

//Checks if any pieces are on the cell
bool arePiecesOnCell(int mysteryCellLocation, struct Player *players[4]){
    for(int i=0; i<4; i++){

        if(players[i]->piece1.location == mysteryCellLocation){
            return true;
        }
        if(players[i]->piece2.location == mysteryCellLocation){
            return true;
        }
        if(players[i]->piece3.location == mysteryCellLocation){
            return true;
        }
        if(players[i]->piece4.location == mysteryCellLocation){
            return true;
        }
    }

    return false;
}

//Updates the location of the myster cell
void updateMysterCell(int roundCounter, struct Player *players[4]){
    //Runs during the second round and every 4 rounds after that
    if((roundCounter-2)%4 ==0){

        //Generate a random number between 0 and 51
        int randomCellLocation = rand() % 52;

        //Checks if the new random location if the same if
        if(randomCellLocation == mysteryCellLocation || arePiecesOnCell(randomCellLocation, players)){
            updateMysterCell(roundCounter, players);
        }else{
            mysteryCellLocation = randomCellLocation;
            printf("Mystery Cell has spawned in location L%d and will be at that location for the next 4 rounds\n\n", mysteryCellLocation);
        }
    }
}

void teleportPieces(struct Piece *piece, char color[10]){
    //Creates a list with the following locations 
    //[0.Bhawana, 1.Kotuwa, 2.Pita Kotuwa, 3.Base, 4.X of the player, 5.Approach of the player]
    int randomLocations[6] = {7,25,44,0,0,0};
    int bhawanaSteps = 0;
    int kotuwaSteps =0;
    int pitaKotuwaSteps =0;

    landingCount ++;


    //Setting base, X  and approach cell locations based on the player
    if (strcmp(color, "Yellow") == 0) {
        randomLocations[3] = 0; //Setting base location
        randomLocations[4] = 0; //Setting X location
        randomLocations[5] = 50; //Setting Approach cell location
        bhawanaSteps = 8;
        kotuwaSteps = 26;
        pitaKotuwaSteps = 45;

    } else if (strcmp(color, "Blue") == 0) {
        randomLocations[3] = 13;
        randomLocations[4] = 13;
        randomLocations[5] = 11;

        bhawanaSteps = 47;
        kotuwaSteps = 13;
        pitaKotuwaSteps = 32;

    } else if (strcmp(color, "Red") == 0) {
        randomLocations[3] = 26;
        randomLocations[4] = 26;
        randomLocations[5] = 24;

        bhawanaSteps = 34;
        kotuwaSteps = 0;
        pitaKotuwaSteps = 19;

    } else {
        randomLocations[3] = 39;
        randomLocations[4] = 39;
        randomLocations[5] = 37;

        bhawanaSteps = 21;
        kotuwaSteps = 35;
        pitaKotuwaSteps = 6;
    }

    int randomIndex = rand() % 6;
    int randomLocation = randomLocations[randomIndex];
    piece->location = randomLocation;

    printf("%s player lands on a mystery cell and is teleported to L%d\n\n", color, randomLocation);

    if(randomIndex ==0){ //Bhawana

        float aura = (rand() % 2 == 0) ? 0.5f : 2.0f;
        piece->aura = aura;
        piece->steps = piece->direction > 0? bhawanaSteps: 54-bhawanaSteps;

        printf("%s piece %s teleported to Bhawana\n", color, piece->pieceId);

        aura > 0? printf("%s piece %s feels energized and movement piece doubles.\n",color, piece->pieceId) : printf("%s piece %s feels sick and movement piece halves.\n",color, piece->pieceId);

         
    }else if(randomIndex ==1){ //Kotuwa
        printf("%s piece %s teleported to Kotuwa\n", color, piece->pieceId);

        if(false){
            piece->captures = 0;
            piece->direction =0;
            piece->isAtBase = true;
            piece->isInBlock= false;
            piece->steps=0;

            printf("%s piece %s is movement restricted and has rolled three consecutively. Teleporting piece %s to base\n", color, piece->pieceId, piece->pieceId);
        }
        else {
            piece->steps = piece->direction > 0 ? kotuwaSteps : 54 - kotuwaSteps;
            printf("%s piece %s attends briefing and cannot move for four rounds\n", color, piece->pieceId);
        }

    }else if(randomIndex ==2){ //Pitakotuwa
        printf("%s piece %s teleported to Pita-Kotuwa\n", color, piece->pieceId);

        if(piece->direction > 0){
            piece->direction = -1; //Changing piece's directiom
            piece->steps = pitaKotuwaSteps;
            printf("The %s piece %s which was moving clockwise, has changed to moving counterclockwise\n", color, piece->pieceId);

        } else{
            piece->location =randomLocations[1]; //Teleporting to Kotuwa
            piece->steps = 54 - kotuwaSteps;
            printf("The %s piece %s is moving in a counter clockwise direction. The Teleporting to Kotuwa from Pita-Kotuwa\n", color, piece->pieceId);
        }

    }else if(randomIndex ==3){ //Base
        piece->captures = 0;
        piece->direction =0;
        piece->isAtBase = true;
        piece->isInBlock= false;
        piece->steps=0;

        printf("%s piece %s teleported to Base\n", color, piece->pieceId);

    }else if(randomIndex ==4){ // X 
        piece->steps=0;
        printf("%s piece %s teleported to X\n", color, piece->pieceId);

    }else{ //Approach cell
        piece->steps=0;
        printf("%s piece %s teleported to Approach\n", color, piece->pieceId);

    }
}

//Checks if piece can move(Due to blocks or not sufficient blocks in home straight)

bool canMove(struct Piece *piece, int steps){
    //Setting steps based on the piece's aura
    steps = (int)(piece->aura * steps);

    if(steps <=1){
        return false;
    }

    //Piece with clockwise direction moving in the home straight
    if(piece->direction > 0 && piece->steps >=51 && (piece->steps + steps) <=57 && !piece->isAtHome && piece->captures >0){
        return true;
    }

    //Piece with anti-clockwise direction moving in the home straight
    if(piece->direction < 0 && piece->steps >=53 && (piece->steps + steps) <=59 && !piece->isAtHome && piece->captures >0){
        return true;
    }

    //Piece that can enter the home straight but havent reached the O-block
    if(piece->captures > 0 && piece->direction > 0 && piece->steps < 51){
        return true;
    }

    if(piece->captures > 0 && piece->direction < 0 && piece->steps < 53){
        return true;
    }

    //Piece that cant enter the home straight
    if(!piece->isAtBase && !piece->isAtHome){
        return true;
    }

    return false;
}


//Checks if the opponents have created a block and whether the piece cqn move forward or not
bool isPathBlocked(){
    return false;
}

//Move  player piece based on the value rolled and the direction of movement
void movePiece(int steps, struct Piece *piece, char color[10]){

    steps = (int)(piece->aura * steps);

    int prevLocation = piece->location;
    piece->location = (52 + piece->location + piece->direction * steps) % 52;
    piece->steps = piece->steps + steps;

    if(piece->location == mysteryCellLocation){
        teleportPieces(piece, color);
        return;
    }


    //Moving pieces with captures 1 into home straight
    if((piece->direction < 0 && piece->steps >53 && piece->captures > 0 && !piece->isAtHome) || (piece->direction > 0 && piece->steps >51 && piece->captures > 0 && !piece->isAtHome)){
        
        piece->location = 500; //Takes pieces in the home straight out of the standard cells

        printf("%s has moved by %d\n", piece->pieceId, piece->steps); //Remove

        if((piece->direction < 0 && piece->steps == 59 && !piece->isAtHome) || (piece->direction > 0 && piece->steps == 57 && !piece->isAtHome)){

            piece->isAtHome = true;
            printf("Piece %s of %s player has reached home\n\n",piece->pieceId, color);

        }else{

            printf("Piece %s of %s player has moved %d squares up the home straight\n", piece->pieceId, color, steps);
        }

        return;
    }

    //Resetting steps of pieces with zero captures
    if((piece->direction < 0 && piece->steps > 53) && piece->captures < 1 || (piece->direction > 0 && piece->steps >51 &&piece->captures < 1)){
        if(piece->direction < 0){
            piece->steps = piece->steps - 50;
        }else{
            piece->steps = piece->steps - 52;
        }
    }

    printf("%s moves piece %s from location L%d to L%d by %d units in ", color,piece->pieceId, prevLocation, piece->location, steps);
    piece->direction == 1? printf("Clockwise Direction\n\n"):printf("AntiClockwise Direction \n\n");
}

//Updates the direction, steps, and stepDivider of the block after a new piece is added to the block
void updateBlockData(struct Block *block){
    struct Piece* pieces[4] = {block->pieces[0], block->pieces[1], block->pieces[2], block->pieces[3]};

    int newSteps;
    int newDirection;
    int newDieValueDivider = 4;

    for(int i=0; i<4; i++){
        if(pieces[i] == NULL){
            newDieValueDivider--; 
            continue;
        }

        if(pieces[i]->steps < block->steps){
            newSteps = pieces[i]->steps;
            newDirection = pieces[i]->direction;
        }
    }
    block->direction = newDirection;
    block->steps = newSteps;
    block->dieValueDivider = newDieValueDivider;
    return;
}

bool canCreateBlock(struct Player *player, int steps){

    struct Piece pieces[4] = {player->piece1, player->piece2, player->piece3, player->piece4};
    struct Block blocks[2] = {player->block1, player->block2};


    //When the player has no blocks, checks if a block can be made by moving a piece;
    if(player->block1.isNull && player->block2.isNull){
        for(int i = 0; i<3; i++){ 
            for(int j=i+1; j<4; j++){
                
                int newLocation = (TOTAL_STEPS + pieces[i].location + pieces[i].direction * steps) % TOTAL_STEPS;
                if( newLocation == pieces[j].location){
                    printf("A block can be been created\n");
                    return true;
                }
            }
        }
    }

    //When a player has two blocks checks if moving one block can create a larger block with the other
    if(!player->block1.isNull && !player->block2.isNull){

        int newBlock1Location = (TOTAL_STEPS + player->block1.location + player->block1.direction * (steps / player->block1.dieValueDivider));
        int newBlock2Location = (TOTAL_STEPS + player->block2.location + player->block2.direction * (steps / player->block2.dieValueDivider));

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

                    int newBlockLocation = (TOTAL_STEPS + blocks[i].location + blocks->direction * (steps / blocks[i].dieValueDivider)) %TOTAL_STEPS; //New location of the block if you decide to move it
                    int newPieceLocation = (TOTAL_STEPS + pieces[j]. location + pieces[j].direction * steps) %TOTAL_STEPS; //New location of the piece if you decide you move it

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

        int newLocation = (TOTAL_STEPS + pieces[i]->location + (pieces[i]->direction * steps)) % TOTAL_STEPS;
        int playerSteps = pieces[i]->direction < 0? 53 : 51;

        for(int j=i+1; j<4; j++){
            
            if((newLocation == pieces[j]->location) && ((pieces[i]->steps + steps) < playerSteps) && pieces[j]->location > 0){
                pieces[i]->isInBlock = true;
                pieces[i]->location = newLocation;
                pieces[i]->steps = pieces[i]->steps + steps;

                //If both blocks of the player are empty, add the values to block1
                if(player->block1.isNull && player->block2.isNull){

                    player->block1.pieces[0] = pieces[i];
                    player->block1.pieces[1] = pieces[j];
                    player->block1.isNull = false;
                    player->block1.location = pieces[i]->location;
                    updateBlockData(&player->block1);

                    printf("%s Player has created a block at L%d with pieces %s and %s\n", player->color, player->block1.location, pieces[i]->pieceId, pieces[j]->pieceId);
                    return;
                }

                //If the first block is occupied and the new location doesnt match the location of the first block
                if(player->block2.isNull && !player->block1.isNull && player->block1.location != newLocation){
                    player->block2.pieces[0] = pieces[i];
                    player->block2.pieces[1] = pieces[j];
                    player->block2.isNull = false;
                    player->block2.location = pieces[i]->location;

                    updateBlockData(&player->block2);
                    printf("%s Player has created a block at L%d with pieces %s and %s\n", player->color, player->block2.location, pieces[i]->pieceId, pieces[j]->pieceId);

                    return;

                }

                //When there is only one block with two pieces and a more pieces joins that block
                for(int k=0; k<2; k++){
                    if(!blocks[k]->isNull && blocks[k]->location == newLocation){

                        for (int l = 0; l < 4; l++) {
                            if (blocks[k]->pieces[l] == NULL) {
                                blocks[k]->pieces[l] = pieces[i];
                                break;
                            }
                        }
                        printf("Player has created a block, this block is a beta\n");

                        updateBlockData(blocks[k]);
                        //Add Print statement here
                        return;
                    }
                }
            }
        }
    }

    //If there exists no piece that isnt in a block, then all pieces are in a block
    //Since createBlock() is only implemented when canCreateBlock() returns, it means, the players have two blocks each with two pieces
    int block1NewLocation = (TOTAL_STEPS + player->block1.location + player->block1.direction * steps /player->block1.dieValueDivider) % TOTAL_STEPS;
    int block2NewLocation = (TOTAL_STEPS + player->block2.location + player->block2.direction * steps /player->block2.dieValueDivider) % TOTAL_STEPS;

    //When block1 moves to block2 and creates a new block
    if(block1NewLocation == player->block2.location){
        for(int i=0; i < 4; i++){
            for(int j=0; j<4; j++){
                
                if(player->block2.pieces[j] == NULL && player->block1.pieces[i] != NULL){
                    player->block2.pieces[j] = player->block1.pieces[i];
                    player->block1.pieces[i] = NULL;

                }
            }
        }
        //Resetting block1
        player->block1.dieValueDivider = 1;
        player->block1.direction = 1;
        player->block1.isNull = true;
        player->block1.location = 0;
        player->block1.steps = 0;

        //Updating block2
        updateBlockData(&player->block2);
        printf("%s Player has created a block at L%d\n", 
            player->color, 
            player->block2.location
        );

        return;
    }
    //When block2 moves to block1 and creates a new block
    if(block2NewLocation == player->block1.location){
        for(int i=0; i < 4; i++){
            for(int j=0; j<4; j++){

                printf("Debug: i=%d, j=%d, block2.pieces[%d]=%p, block1.pieces[%d]=%p\n", i, j, i, player->block2.pieces[i], j, player->block1.pieces[j]);
                if(player->block1.pieces[j] == NULL && player->block2.pieces[i] != NULL){
                    printf("Test 10\n");
                    
                    player->block1.pieces[j] = player->block2.pieces[i];
                    player->block2.pieces[i] = NULL;
                }
            }
        }

        //Resetting block 2;
        player->block2.dieValueDivider = 1;
        player->block2.direction = 1;
        player->block2.isNull = true;
        player->block2.location = 0;
        player->block2.steps = 0;

        updateBlockData(&player->block1);

        printf("%s Player has created a block at L%d\n", 
            player->color, 
            player->block1.location
        );
        return;
    }
}

//Checks if the block can move to the home straight
bool canBlockMoveToHomeStraight(struct Block *block, int steps){

    int blockSteps = block->direction > 0? 51 : 53;
    int moveablePieceCount = 0;

    //Returns true if the block can move into home straight; All pieces in block need to have captures greater than 0
    if(block->steps + (steps/ block->dieValueDivider) > blockSteps && block->location < 500){
        for(int i=0; i<4; i++){
            if(block->pieces[i] != NULL && block->pieces[i]->captures > 0 && (steps/block->dieValueDivider >0)){
                moveablePieceCount++;
            }
        }

        //Checks if  the moveable piece count is equal to the total number of pieces in block
        if(moveablePieceCount == block->dieValueDivider){ 
            return true;
        }
    }

    return false;
}

//Checks if the block can move forward in the home straight
bool canBlockMoveInHomeStraight(struct Block* block, int steps){

    int maxBlockSteps = block->direction > 0? 57 : 59;
    int blockSteps = block->direction > 0? 51 : 53;
    int blockNewSteps = block->steps + (steps/block->dieValueDivider);

    if(block->steps > blockSteps && blockNewSteps <= maxBlockSteps && (steps/block->dieValueDivider >0)){
        return true;
    }
    return false;
}

void moveBlock(struct Block *block1, struct Block *block2, int steps, char color[10]) {

    int block1NewLocation = (TOTAL_STEPS + (block1->location + (block1->direction * (steps / block1->dieValueDivider))) % TOTAL_STEPS) % TOTAL_STEPS;

    int block2NewLocation = (TOTAL_STEPS + block2->location + (block2->direction * (steps / block2->dieValueDivider))) % TOTAL_STEPS;

    int block1TotalSteps = block1->direction > 0 ? 57 : 59;
    int block2TotalSteps = block2->direction > 0 ? 57 : 59;

    int block1Steps = block1->direction > 0 ? 51 : 53;
    int block2Steps = block2->direction > 0 ? 51 : 53;

    int newBlock1steps = block1->steps + steps/block1->dieValueDivider;
    int newBlock2steps = block2->steps + steps/block2->dieValueDivider;

    //When both block1 and block2 exists
    if(!block1->isNull && !block2->isNull){

        //When block1 is closer to home than block 2
        if(block1->steps > block2->steps && steps/block1->dieValueDivider > 0){
             
            //Checks if the block can move in the home straight
            if(canBlockMoveInHomeStraight(block1, steps)){
                //If the block can reach home;
                if(newBlock1steps == block1TotalSteps){
                    for(int i=0; i<4; i++){
                        if(block1->pieces[i] !=NULL){
                            block1->pieces[i]->isAtHome = true;
                            block1->pieces[i]->isInBlock = false;
                            printf("Piece %s of %s player has reached home\n\n",block1->pieces[i]->pieceId, color);
                        }
                    }
                //If the block can't reach home but can still move in the home straight
                }else{
                    block1->steps = newBlock1steps;
                    printf("A block of %s player has moved %dth cell of the home straight",color, newBlock1steps - block1Steps);
                }

            //Moves the block to the home striahgt
            }else if(canBlockMoveToHomeStraight(block1, steps)){
                block1->location = 500;
                block1->steps = newBlock1steps;
                printf("A block of %s player has entered and moved %d cells up the home straight\n",color, newBlock1steps - block1Steps);
            
            //Moves the block normally
            }else{
                printf("%s player moves block at L%d to L%d by 1\n", color, block1->location, block1NewLocation);

                block1->location = block1NewLocation;
                if(block1->direction > 0){
                    block1->steps = newBlock1steps > block1Steps? newBlock1steps - 52 : newBlock1steps;
                }else{
                    block1->steps = newBlock1steps > block1Steps? newBlock1steps - 50 : newBlock1steps;
                }
            
            }


        //When block2 is closer to home than block 1
        }else if (block2->steps > block1->steps && steps/block2->dieValueDivider > 0){
            if(canBlockMoveInHomeStraight(block2, steps)){
                //If the block can reach home;
                if(newBlock2steps == block2TotalSteps){
                    for(int i=0; i<4; i++){
                        if(block2->pieces[i] !=NULL){
                            block2->pieces[i]->isAtHome = true;
                            block2->pieces[i]->isInBlock = false;
                            printf("Piece %s of %s player has reached home\n\n",block2->pieces[i]->pieceId, color);
                        }
                    }
                //If the block can't reach home but can still move in the home straight
                }else{
                    block2->steps = newBlock2steps;
                    printf("A block of %s player has moved %dth cell of the home straight\n",color, newBlock2steps - block2Steps);
                }

            //Moves the block to the home straight
            }else if(canBlockMoveToHomeStraight(block2, steps)){
                block2->location = 500;
                block2->steps = newBlock1steps;
                printf("A block of %s player has entered and moved %d cells up the home straight\n",color, newBlock2steps - block2Steps);

            //Moves the block normally
            }else{
                printf("%s player moves block at L%d to L%d by 2\n", color, block2->location, block2NewLocation);
                block2->location = block2NewLocation;
                if(block2->direction > 0){
                    block2->steps = newBlock2steps > block2Steps? newBlock2steps - 52 : newBlock2steps;
                }else{
                    block2->steps = newBlock2steps > block2Steps? newBlock2steps - 50 : newBlock2steps;
                }
            }
            
        }else{
            printf("Cant move any pieces\n");
        }
    
    //When only block1 exists
    }else if(!block1->isNull && block2->isNull){

        //Block 1 can be moved
        if((steps/ block1->dieValueDivider) > 0){
            //Checks if the block can move in the home straight
            if(canBlockMoveInHomeStraight(block1, steps)){
                //If the block can reach home;
                if(newBlock1steps == block1TotalSteps){
                    for(int i=0; i<4; i++){
                        if(block1->pieces[i] !=NULL){
                            block1->pieces[i]->isAtHome = true;
                            block1->pieces[i]->isInBlock = false;
                            printf("Piece %s of %s player has reached home\n\n",block1->pieces[i]->pieceId, color);
                        }
                    }
                //If the block can't reach home but can still move in the home straight
                }else{
                    block1->steps = newBlock1steps;
                    printf("A block of %s player has moved %dth cell of the home straight",color, newBlock1steps - block1Steps);
                }

            //Moves the block to the home stright
            }else if(canBlockMoveToHomeStraight(block1, steps)){
                block1->location = 500;
                block1->steps = newBlock1steps;
                printf("A block of %s player has entered and moved %d cells up the home straight\n",color, newBlock1steps - block1Steps);
            
            //Moves the block normally
            }else{
                printf("%s player moves block at L%d to L%d by 3\n", color, block1->location, block1NewLocation);

                block1->location = block1NewLocation;
                if(block1->direction > 0){
                    block1->steps = newBlock1steps > block1Steps? newBlock1steps - 52 : newBlock1steps;
                }else{
                    block1->steps = newBlock1steps > block1Steps? newBlock1steps - 50 : newBlock1steps;
                } 
            }

        }

    //When only block2 exists
    }else if(block1->isNull && !block2->isNull){

        if((steps / block2->dieValueDivider) > 0){

            if(canBlockMoveInHomeStraight(block2, steps)){
                //If the block can reach home;
                if(newBlock2steps == block2TotalSteps){
                    for(int i=0; i<4; i++){
                        if(block2->pieces[i] !=NULL){
                            block2->pieces[i]->isAtHome = true;
                            block2->pieces[i]->isInBlock = false;
                            printf("Piece %s of %s player has reached home\n\n",block2->pieces[i]->pieceId, color);
                        }
                    }
                //If the block can't reach home but can still move in the home straight
                }else{
                    block2->steps = newBlock2steps;
                    printf("A block of %s player has moved %dth cell of the home straight\n",color, newBlock2steps - block2Steps);
                }

            //Moves the block to the home straight
            }else if(canBlockMoveToHomeStraight(block2, steps)){
                block2->location = 500;
                block2->steps = newBlock1steps;
                printf("A block of %s player has entered and moved %d cells up the home straight\n",color, newBlock2steps - block2Steps);

            //Moves the block normally
            }else{
                printf("%s player moves block at L%d to L%d by 4\n", color, block2->location, block2NewLocation);

                block2->location = block2NewLocation;
                if(block2->direction > 0){
                    block2->steps = newBlock2steps > block2Steps? newBlock2steps - 52 : newBlock2steps;
                }else{
                    block2->steps = newBlock2steps > block2Steps? newBlock2steps - 50 : newBlock2steps;
                }
            }

        }
    }

    return;
}

//Breaks all the blocks of a player
void breakBlock(struct Player *player){
    struct Block* blocks[4] = {&player->block1, &player->block2};

    for(int j=0; j<2; j++){
        for(int i=0; i<4; i++){
            if(!blocks[j]->isNull && blocks[j]->pieces[i] != NULL){

                //Updating steps for pieces
                if(blocks[j]->pieces[i]->direction != blocks[j]->direction){
                    if(blocks[j]->steps == 1){
                        blocks[j]->pieces[i]->steps  = 1;
                    }else if(blocks[j]->steps == 0){
                        blocks[j]->pieces[i]->steps  = 2;
                    }else{
                        blocks[j]->pieces[i]->steps  = abs(54 - blocks[j]->steps);
                    }
                }else{
                    blocks[j]->pieces[i]->steps = blocks[j]->steps;
                }
                //Updating other info
                blocks[j]->pieces[i]->location = blocks[j]->location;
                blocks[j]->pieces[i]->isInBlock = false;
                blocks[j]->pieces[i] = NULL;

                
                
            }
        }
        if(!blocks[j]->isNull){
            blocks[j]->steps = 0;
            blocks[j]->location = 0;
            blocks[j]->dieValueDivider = 1;
            blocks[j]->direction = 1;
            blocks[j]->isNull = true;
        }
        
    }

    printf("All blocks of %s player has been broken\n", player->color);

}

//Checks if a piece of another player can be captured
bool canCapture(struct Piece *currentPiece, struct Player *opponent, int steps) {

    steps = (int)(currentPiece->aura * steps);

    struct Piece* opponentPieces[4] = {&opponent->piece1, &opponent->piece2, &opponent->piece3, &opponent->piece4};

    for(int i =0; i<4; i++){

        int newLocation = (TOTAL_STEPS+ currentPiece->location + (steps * currentPiece->direction)) %TOTAL_STEPS;
        int playerSteps = currentPiece->direction > 0? 531 : 53;

        if((newLocation == opponentPieces[i]->location) && (opponentPieces[i]->steps > 1) && !((currentPiece->steps + steps > playerSteps) && (currentPiece->captures > 0 ) && !opponentPieces[i]->isAtHome)){
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
                steps = (int)(currentPlayerPieces[j]->aura * steps);

                int prevLocation = currentPlayerPieces[j]->location;
                int newLocation =  (TOTAL_STEPS + currentPlayerPieces[j]->location + (steps * currentPlayerPieces[j]->direction)) % TOTAL_STEPS;
                int playerSteps = currentPlayerPieces[j]->direction > 0? 51 : 53;

                //Looping thorugh all the pieces of the opponent 
                for(int k=0; k<4; k++){
                    if((newLocation == opponentPieces[k]->location) && (!currentPlayerPieces[j]->isAtBase) && (opponentPieces[k]->steps > 1) && !opponentPieces[i]->isAtHome && !((currentPlayerPieces[j]->steps + steps > playerSteps) && (currentPlayerPieces[j]->captures > 0))){

                        //Moving the current player's piece to capture the opponent's piece and increments captures by 1

                        currentPlayerPieces[j]->location = newLocation;
                        currentPlayerPieces[j]->steps += steps;

                        currentPlayerPieces[j]->captures += 1;

                        printf("%s piece %s lands on square L%d, captures %s piece %s and returns it to base.\n", currentPlayer->color, currentPlayerPieces[j]->pieceId, newLocation, players[i]->color, opponentPieces[k]->pieceId);

                        //Resetting the opponent's piece that was captured
                        opponentPieces[k]->isAtBase = true;
                        opponentPieces[k]->location = 0;
                        opponentPieces[k]->steps = 0;
                        opponentPieces[k]->captures = 0;
                        opponentPieces[k]->isInBlock = false;
                        opponentPieces[k]->direction = 1;

                        testCaptures +=1; //Remove this

                        return;
                    }
                }
            }
        }
    }
}

//Moves piece (X) by the based on the die roll and any captures opponents piece if it is on the square X lands on 
void moveOrCapture(int steps, struct Piece *playerPiece, char color[10], struct Player *allPlayers[4]){

    steps = (int)(playerPiece->aura * steps);

    movePiece(steps, playerPiece, color); //Change to normal

    for(int i=0; i<4; i++){

        if (strcmp(color, allPlayers[i]->color) != 0) {

            struct Piece *opponentPieces[4] = {&allPlayers[i]->piece1, &allPlayers[i]->piece2, &allPlayers[i]->piece3, &allPlayers[i]->piece4};

            int playerSteps = playerPiece->direction > 0? 51 : 53;
        
            for(int j=0; j<4; j++){
                
                if((playerPiece->location == opponentPieces[j]->location) && (opponentPieces[j]->steps > 1) && !(playerSteps >= playerPiece->steps && playerPiece->captures > 0) && !opponentPieces[j]->isAtHome ){
                    opponentPieces[j]->captures = 0;
                    opponentPieces[j]->direction = 1;
                    opponentPieces[j]->isAtBase = true;
                    opponentPieces[j]->isInBlock = false;
                    opponentPieces[j]->location = 0;
                    opponentPieces[j]->steps = 0;

                    printf("%s piece %s lands on square L%d, captures %s piece %s and returns it to base\n",color, playerPiece->pieceId, playerPiece->location, allPlayers[i]->color, opponentPieces[j]->pieceId);
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
            breakBlock(yellow);
            return;
        }

        if (arePiecesAtBase(yellow)) {
            drawPiece(yellow);

        } else if (capturePossible) {
            capturePiece(players, yellow, dieValue);

        } else {

            // Find the closest piece to home that is not at the base
            for(int i = 0; i < 4; i++){
                if(!playerPieces[i]->isAtBase && (max == NULL || playerPieces[i]->steps >  max->steps) && canMove(playerPieces[i], dieValue) ){
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
                if(!playerPieces[i]->isAtBase && (max == NULL || playerPieces[i]->steps > max->steps) && canMove(playerPieces[i], dieValue)){
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

void bluePlayerBehaviour(struct Player *blue, int consecutiveSixes, struct Player* players[4]){
    int dieValue = rollDie();
    printf("Blue has rolled %d\n", dieValue);

    struct Piece *playerPieces[4] = {&blue->piece1, &blue->piece2, &blue->piece3, &blue->piece4};
    struct Piece *max = NULL; // Initialize to NULL

    if(dieValue ==6){
        consecutiveSixes++;

        if (consecutiveSixes == 3) {
            printf("Blue rolled three consecutive 6s! Turn is passed to the next player.\n");
            breakBlock(blue);
            return;
        }

        //Checks of the player can move to the mystery cell
        bool canMoveToMysteryCell = (52 + playerPieces[currentBluePiece]->location + playerPieces[currentBluePiece]->direction * (int)(dieValue * playerPieces[currentBluePiece]->aura)) % 52 == mysteryCellLocation;

        if(playerPieces[currentBluePiece]->direction > 0 && !playerPieces[currentBluePiece]->isAtBase || playerPieces[currentBluePiece]->direction < 0 && !playerPieces[currentBluePiece]->isAtBase && !canMoveToMysteryCell){

            moveOrCapture(dieValue, playerPieces[currentBluePiece], blue->color, players);
            currentBluePiece = (currentBluePiece +1) % 4;
            
        }else if(arePiecesAtBase(blue)){
            drawPiece(blue);
        }else{
            printf("No pices can be moved\n");
        }

        bluePlayerBehaviour(blue, consecutiveSixes, players);

    }else{
        bool canMoveToMysteryCell = (52 + playerPieces[currentBluePiece]->location + playerPieces[currentBluePiece]->direction * (int)(dieValue * playerPieces[currentBluePiece]->aura)) % 52 == mysteryCellLocation;

        if(playerPieces[currentBluePiece]->direction > 0 && !playerPieces[currentBluePiece]->isAtBase || playerPieces[currentBluePiece]->direction < 0 && !playerPieces[currentBluePiece]->isAtBase && !canMoveToMysteryCell){

            moveOrCapture(dieValue, playerPieces[currentBluePiece], blue->color, players);
            currentBluePiece = (currentBluePiece +1) % 4;
            
        }
    }
}

void redPlayerBehaviour(struct Player *red, int consecutiveSixes, struct Player* players[4]){

    int dieValue = rollDie();
    printf("Red has rolled %d\n", dieValue);

    struct Piece *playerPieces[4] = {&red->piece1, &red->piece2, &red->piece3, &red->piece4};
    struct Piece *max = NULL; // Initialize to NULL

    bool capturePossible = false;
    for (int i = 0; i < 4; i++) {
        for(int j=0; j<4; j++){
            if (players[i] != red && canCapture(playerPieces[i], players[j], dieValue) ) {
                capturePossible = true;
                break;
            }
        }     
    }

    if(dieValue ==6){
        consecutiveSixes ++;

        if (consecutiveSixes == 3) {
            printf("Red rolled three consecutive 6s! Turn is passed to the next player.\n");
            breakBlock(red);
            return;
        }

        if(capturePossible){
            capturePiece(players, red, dieValue);

        }else if(arePiecesAtBase(red)) {
            drawPiece(red);

        } else {

            // Find the closest piece to home that is not at the base
            for(int i = 0; i < 4; i++){
                if(!playerPieces[i]->isAtBase && (max == NULL || playerPieces[i]->steps >  max->steps) && canMove(playerPieces[i], dieValue) ){
                    max = playerPieces[i];
                }
            }

            // Move the piece if found
            if(max != NULL){
                movePiece(dieValue, max, red->color);
            }else{
                printf("No pieces can be moved\n");
            }
        }
        redPlayerBehaviour(red, consecutiveSixes, players);
    }else {

        if (capturePossible) {
            capturePiece(players, red, dieValue);
            redPlayerBehaviour(red, consecutiveSixes, players);
        } 
        else {
            // Find the closest piece to home that is not at the base
            for(int i = 0; i < 4; i++){
                if(!playerPieces[i]->isAtBase && (max == NULL || playerPieces[i]->steps > max->steps) && canMove(playerPieces[i], dieValue) ){
                    max = playerPieces[i];
                }
            }
            // Move the piece if found
            if(max != NULL){
                movePiece(dieValue, max, red->color);
            }else{
                printf("No pieces can be moved\n");
            }
        }
    }
}

void greenPlayerBehaviour(struct Player *green, int consecutiveSixes, struct Player* players[4]){

    struct Piece *playerPieces[4] = {&green->piece1, &green->piece2, &green->piece3, &green->piece4};

    int dieValue = rollDie();

    printf("Green has rolled %d\n", dieValue);

    if(dieValue == 6){ //Logic when player rolls a six
        consecutiveSixes++;

        if (consecutiveSixes == 3) {
            printf("Green rolled three consecutive 6s! Turn is passed to the next player.\n");
            //Insert logic to break blocks if any
            breakBlock(green);
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

        //If a block exists, move it
        if((!green->block1.isNull && dieValue/green->block1.dieValueDivider > 0) || !green->block2.isNull && dieValue/green->block2.dieValueDivider > 0){
            moveBlock(&green->block1, &green->block2, dieValue, green->color);
            return;
        }

        //Moves the piece closest to home; when a block cant be created, an existing block cant be moved and no pieces are at base
        //Sets the peices that is; drawn from base, is closest to home, can move and is not in a block to max
        struct Piece *max = NULL;
        
    
        for(int i = 0; i < 4; i++){
            if(max == NULL || ((playerPieces[i]->steps > max->steps) && !playerPieces[i]->isInBlock && canMove(playerPieces[i], dieValue))){
                max = playerPieces[i];
            }
        }

        if(max != NULL){
            // Move the max piece if found
            moveOrCapture(dieValue, max, green->color, players);
            
        }else{
            //breaks a block as no other piece can be moved  (max is not found initially)
            for(int i = 0; i < 4; i++){
                if(max == NULL || ((playerPieces[i]->steps > max->steps) && canMove(playerPieces[i], dieValue))){
                    max = playerPieces[i];
            }

            max->isInBlock = false;    
            }
            //Updating the location, steps to the current that of the block and setting isInBlock to false 
            for(int i=0; i<4; i++){
                if(green->block1.pieces[i] == max){
                    max->location= green->block1.location;
                    max->steps= green->block1.steps;

                    break;
                }

                if(green->block2.pieces[i] == max){
                    max->isInBlock = false;
                    max->location= green->block2.location;
                    max->steps= green->block2.steps;

                    break;
                }
            }

            moveOrCapture(dieValue, max, green->color, players);  
        }

        greenPlayerBehaviour(green, consecutiveSixes, players);

    }else{ //Logic when player rolls a valie that isn't a six

        //If a block exists, move it
        if((!green->block1.isNull && dieValue/green->block1.dieValueDivider > 0) || !green->block2.isNull && dieValue/green->block2.dieValueDivider > 0){
            moveBlock(&green->block1, &green->block2, dieValue, green->color);
            return;
        }

        //If a block is not moveable, move a piece; 
        struct Piece *max = NULL;
        
        for(int i = 0; i < 4; i++){
            if(max == NULL || (playerPieces[i]->steps > max->steps && !playerPieces[i]->isAtBase && !playerPieces[i]->isInBlock && canMove(playerPieces[i], dieValue))){
                max = playerPieces[i];
            }
        }

        if(max != NULL){
            // Move the max piece if found
            moveOrCapture(dieValue, max, green->color, players);
            
        }else{
            //breaks a block as no other piece can be moved  (max is not found initially)
            for(int i = 0; i < 4; i++){
                if((max == NULL || playerPieces[i]->steps > max->steps) && (!playerPieces[i]->isAtBase) && canMove(playerPieces[i], dieValue)){
                    max = playerPieces[i];
                }   
            }  
            
            if(max == NULL){ //If max is still, then no pieces have have been drawn from base
                printf("No pieces can be moved\n");
                return; 
            } 
            //Updating the location, steps to the current that of the block and setting isInBlock to false 
            
            for(int i=0; i<4; i++){
                if(green->block1.pieces[i] == max){
                    max->isInBlock = false;
                    max->location= green->block1.location;
                    max->steps= green->block1.steps;

                    break;
                }

                if(green->block2.pieces[i] == max){
                    max->isInBlock = false;
                    max->location= green->block2.location;
                    max->steps= green->block2.steps;

                    break;
                }
            }
            
            moveOrCapture(dieValue, max, green->color, players);  
        }
    }
}

//Call player behviour based on the piece's color
void getBehaviour(char color[10], struct Player *player, struct Player* players[4]) {
    int consecutiveSixes = 0;

    if (strcmp(color, "Yellow") == 0) {
        yellowPlayerBehaviour(player, consecutiveSixes, players);

    } else if (strcmp(color, "Blue") == 0) {
        //bluePlayerBehaviour();
        bluePlayerBehaviour(player, consecutiveSixes, players);

    } else if (strcmp(color, "Red") == 0) {
        redPlayerBehaviour(player, consecutiveSixes, players);

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

    bool isGameOver = false;
    int roundCounter = 0;
    int turnCounter =0;

    while (!isGameOver){
        
        for(int i = 0; i<4; i++){
            if(players[i]->playOrder == currentTurn)
            {
                getBehaviour(players[i]->color, players[i], players);
                printf("\n");

                currentTurn = (currentTurn % 4) + 1;
                turnCounter +=1;
                break; 
            }    
        }

        //Check if a player has brought all players to home and end game;
        for(int i=0; i<4; i++){
            if(players[i]->piece1.isAtHome && players[i]->piece2.isAtHome && players[i]->piece3.isAtHome && players[i]->piece4.isAtHome){
                    
                    isGameOver = true;
                    printf("%s player has won!!!\n", players[i]->color);
                    break;
            }
        }
        
        //Incrementing the round counter when all the 4 players can taken their turns
        roundCounter = turnCounter%4 ==0? roundCounter+1 : roundCounter;

        updateMysterCell(roundCounter, players);

        
    }
}

void playLudo() {

    srand(time(NULL)); // Seed the random number generator

    // Initialize players
    struct Player yellow = {
        //Piece - location,direction,steps,captures,aura,isAtBase,isAtHome,isInBlock,pieceID
        {0, 1, 0, 0, 1, true, false, false, "Y1"}, 
        {0, 1, 0, 0, 1, true, false, false, "Y2"}, 
        {0, 1, 0, 0, 1, true, false, false, "Y3"}, 
        {0, 1, 0, 0, 1, true, false, false, "Y4"},

        { {NULL, NULL, NULL, NULL}, 1, 0, 1, 0, true},  //Block - pieces, direction, location, isNUll
        { {NULL, NULL, NULL, NULL}, 1, 0, 1, 0, true},
        0, "Yellow" //Playorder, Player Color
    };

    struct Player blue = {
        {13, 1, 0, 0, 1, true, false, false, "B1"}, 
        {13, 1, 0, 0, 1, true, false, false, "B2"}, 
        {13, 1, 0, 0, 1, true, false, false, "B3"}, 
        {13, 1, 0, 0, 1, true, false, false, "B4"},

        { {NULL, NULL, NULL, NULL}, 1, 0, 1, 0, true},
        { {NULL, NULL, NULL, NULL}, 1, 0, 1, 0, true},

        0, "Blue"
    };

    struct Player red = {
        {26, 1, 0, 0, 1, true, false, false, "R1"}, 
        {26, 1, 0, 0, 1, true, false, false, "R2"}, 
        {26, 1, 0, 0, 1, true, false, false, "R3"}, 
        {26, 1, 0, 0, 1, true, false, false, "R4"},
        
        { {NULL, NULL, NULL, NULL}, 1, 0, 1, 0, true},
        { {NULL, NULL, NULL, NULL}, 1, 0, 1, 0, true},

        0, "Red"
    };

    struct Player green = {
        {39, 1, 0, 0, 1, true, false, false, "G1"},
        {39, 1, 0, 0, 1, true, false, false, "G2"}, 
        {39, 1, 0, 0, 1, true, false, false, "G3"}, 
        {39, 1, 0, 0, 1, true, false, false, "G4"},
        
        { {NULL, NULL, NULL, NULL}, 1, 0, 1, 0, true},
        { {NULL, NULL, NULL, NULL}, 1, 0, 1, 0, true},

        0, "Green"
    };

    printIntro();
    playerOrder(&yellow, &blue, &red, &green);
    startGame(&yellow, &blue, &red, &green);

    printf("\n");
}

//-----------Main Function------------------//
int main() {
    playLudo();
    return 0;
}