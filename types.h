// types.h
#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>

#define TOTAL_STEPS 52

//--------Defining Structures-----------//

struct Piece {
    int location;
    int direction;
    int steps;
    int captures;
    float aura;
    bool isAtBase;
    bool isAtHome;
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
    struct Block block1; 
    struct Block block2;
    int playOrder;
    char color[10];
};

//--------Function Prototypes-----------//

void printIntro();
int rollDie();
int shouldMoveClockwise();
void playerOrder(struct Player *player1, struct Player *player2, struct Player *player3, struct Player *player4);
bool arePiecesAtBase(struct Player *player);
void getPlayerCount(struct Player *player);
void displayPlayerStats(struct Player *players[4], int roundCounter);
void drawPiece(struct Player *player);
bool arePiecesOnCell(int mysteryCellLocation, struct Player *players[4]);
void updateMysterCell(int roundCounter, struct Player *players[4]);
void teleportPieces(struct Piece *piece, char color[10]);
bool canMove(struct Piece *piece, int steps);
bool isPathBlocked(struct Piece *piece);
void movePiece(int steps, struct Piece *piece, char color[10]);
void updateBlockData(struct Block *block);
bool canCreateBlock(struct Player *player, int steps);
void createBlock(struct Player *player, int steps);
bool canBlockMoveToHomeStraight(struct Block *block, int steps);
bool canBlockMoveInHomeStraight(struct Block* block, int steps);
void breakBlock(struct Player *player);
bool canCapture(struct Piece *currentPiece, struct Player *opponent, int steps);
void capturePiece(struct Player *players[4], struct Player *currentPlayer, int steps);
void moveOrCapture(int steps, struct Piece *playerPiece, char color[10], struct Player *allPlayers[4]);
void yellowPlayerBehaviour(struct Player *yellow, int consecutiveSixes, struct Player* players[4]);
void bluePlayerBehaviour(struct Player *blue, int consecutiveSixes, struct Player* players[4]);
void redPlayerBehaviour(struct Player *red, int consecutiveSixes, struct Player* players[4]);
void greenPlayerBehaviour(struct Player *green, int consecutiveSixes, struct Player* players[4]);
void getBehaviour(char color[10], struct Player *player, struct Player* players[4]);
void startGame(struct Player *player1, struct Player *player2, struct Player *player3, struct Player *player4);
void playLudo();



#endif // TYPES_H
