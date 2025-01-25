#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//definition for colors
#define BlackOdd(string) "\x1b[40m\x1b[92m" string "\x1b[0m"
#define WhiteOdd(string) "\x1b[40m\x1b[31m" string "\x1b[0m"
#define BlackEven(string) "\x1b[47m\x1b[92m" string "\x1b[0m"
#define WhiteEven(string) "\x1b[47m\x1b[31m" string "\x1b[0m"
#define RestOfTheBoard(string) "\x1b[44m" string "\x1b[0m"

#define printMoveList for(int i=0; moveList[i][0]; i++) printf("%s\n", moveList[i]);

//Display function and its dependencies
void displayBoard();
void displayTurn();
void display();

void fullDisplay();

//piece evaluetion function
int value(char piece);

//piece color function
int pieceColor(char piece);

//captured pieces alignment function
void alignCapturedPieces(int i, int j);

//validation functions (creates a list of valid moves for each piece)
int validMoveListPawn();
int validMoveListRook();
int validMoveListKnight();
int validMoveListBishop();
int validMoveListQueen();
int validMoveListKing();

void checkPawnFirstMove();
int isNotOutOfFrame();

void createMoveList();

//Check functions
int isCheck();
int doesNotPutKingInCheck();

//Move functions
int findValidMove(char move[]);
void movePiece(int x1, int y1, int x2, int y2);

//pawn promotion function
void pawnPromotion();

//board copy function
void copyToHistory();

//number of the pieces still on the board function
int numberOfPiecesOnBoard();

//game over function and its dependencies
int isStalemate();
int isThreefoldRepetition();
int isInsufficientMaterial();
int isDraw();
int isCheckmate();

int isGameOver();

//reset function
void gameReset();

//replay function
int doesNotWantToPlayAgain();

//moveList flusher function
void flushMoveList();

//Global variables
char boardHistory[12500][20][10];
int moveNumber;
    //Initialize the board
char board[20][10]={
    {'.','.','.','.','.','.','.','.','.','.'},
    {'.','.','.','.','.','.','.','.','0','0'},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ','a','b','c','d','e','f','g','h',' '},
    {'8','r','n','b','q','k','b','n','r','8'},
    {'7','p','p','p','p','p','p','p','p','7'},
    {'6','.','.','.','.','.','.','.','.','6'},
    {'5','.','.','.','.','.','.','.','.','5'},
    {'4','.','.','.','.','.','.','.','.','4'},
    {'3','.','.','.','.','.','.','.','.','3'},
    {'2','P','P','P','P','P','P','P','P','2'},
    {'1','R','N','B','Q','K','B','N','R','1'},
    {' ','a','b','c','d','e','f','g','h',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {'.','.','.','.','.','.','.','.','.','.'},
    {'.','.','.','.','.','.','.','.','0','0'},
    {'.','.', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }
};
int turn; //0 for white, 1 for black
char moveList[550][6];
int movedPawnFlag[2][10];

//Main function
int main(){
    //While the game is not over
    while(1){
        //Display the board
        display();
        //Copy the board to the history
        copyToHistory();
        //create the valid move list
            //flush the moveList
        flushMoveList();
        createMoveList();
        doesNotPutKingInCheck();

        // printMoveList

        //if the king is in check
            //callout the check
        if(isCheck()) printf("Check\n");
        //Check if the game is over
        //if the game is over
            //break the main loop
        if(moveNumber && isGameOver()){
            display();
            switch (isGameOver()){
            case 1:
                printf("the game ended in a Checkmate the winner is:\n");
                if(!turn) printf("Black\n");
                else printf("white\n");
                break;
            case 2:
                printf("the game ended in a draw due to stalemate\n");
                break;
            case 3:
                printf("the game ended in a draw due to threefold repetition\n");
                break;
            case 4:
                printf("the game ended in a draw due to insufficient material\n");
                break;
            default:
                printf("Error: function \"isGameOver\" returend an invalid value\n");
                break;
            };
            if(!doesNotWantToPlayAgain()) return 0;
        }
        //Increment the move number
        moveNumber++;
        //While the move is not validated get the player's input
        int moveValidated=0, End=0;
        while(!moveValidated){
            int x1, x2, y1, y2, valid=0;
            char move[5];
            //receive the player's input
            printf("To exit the game, enter \"End\"\n");
            printf("Enter the move: ");
            scanf("%s", move);
            //check for manual exit
            if(move[0]=='E' && move[1]=='n' && move[2]=='d'){ End=1; break; }
            //convert input to the board coordinates
            x1=move[1]-'a'+1; y1=12-(move[2]-'0');
            x2=move[3]-'a'+1; y2=12-(move[4]-'0');
            //Check if the move is valid (by searching through the moveList)
            if(findValidMove(move)){
                //move the piece, and validate the move
                movePiece(x1, y1, x2, y2);
                moveValidated=1;
                //update the pawns if it's their first move
                if((move[0]=='P' || move[0]=='p') && movedPawnFlag[turn][move[1]-'a'+1]!=1) movedPawnFlag[turn][move[1]-'a'+1]=1; 
            }
        }
        //align the captured piece in the top/bottom of the board
        alignCapturedPieces(0,0);
        //Switch the turn
        turn=!turn;
        
        //if the game has ended manually, check if the players wants to play again
        if(End) if(doesNotWantToPlayAgain()) return 0;
    }
    return 1;
}

//Display functions implementation
void displayBoard(){
    //print the board
    for(int i=0; i<16; i++){
        printf(WhiteOdd("  "));
        for(int j=0; j<10; j++){
            if(i<12 && i>3 && j<9 && j>0)
                if((i+j)%2)
                    if(board[i][j]=='.') printf(WhiteOdd("   "), board[i][j]);
                    else if(board[i][j]<96) printf(WhiteOdd(" %c "), board[i][j]);
                    else printf(BlackOdd(" %c "), board[i][j]);
                else
                    if(board[i][j]=='.') printf(WhiteEven("   "), board[i][j]);
                    else if(board[i][j]<96) printf(WhiteEven(" %c "), board[i][j]);
                    else printf(BlackEven(" %c "), board[i][j]);
            else if(i<13 && i>2 && j<10 && j>-1) printf(RestOfTheBoard(" %c "), board[i][j]);
            else if(i<=2) printf(WhiteOdd(" %c "), board[i][j]);
            else if(i>=13) printf(BlackOdd(" %c "), board[i][j]);
            else printf(" %c ", board[i][j]);
        }
        printf(WhiteOdd("  "));
        printf("\n");
    }
    printf("\n");
}

void displayTurn(){
    //print the turn
    if(turn==0) printf("White's turn");
    else printf("Black's turn");
    printf(" (%d)\n", moveNumber);
}

void display(){
    //clear the screen and show the new srceen
    system("cls");
    system("clear");
    displayBoard();
    displayTurn();
}

void fullDisplay(){
    for(int i=0; i<10; i++)
        printf("%c", board[16][i]);
    printf("\n");
    display();
}

//piece evaluetion function implementation
int value(char piece){
    if(piece=='Q' || piece=='q') return 9;
    if(piece=='R' || piece=='r') return 5;
    if(piece=='B' || piece=='b') return 3;
    if(piece=='N' || piece=='n') return 3;
    if(piece=='P' || piece=='p') return 1;
    return 0;
}

//piece color function impelementation
int pieceColor(char piece){
    if(piece>96) return 1;
    return 0;
}
//-------------------------------------------------------------------------------------------
//captured pieces alignment function implementation
void alignCapturedPieces(int i, int j){
    int side=pieceColor(board[16][0]);
    //find the location to move the new piece
    for( ; value(board[14*side+j][i]) > value(board[16][0]);i++)
        if(i==10){ i=0; j++; }
    
    // fullDisplay();
    //put the piece in the desired location and move the next captured piece to the top of the captured pieces buffer
    board[16][1]=board[14*side+j][i];
    board[14*side+j][i]=board[16][0];
    board[16][0]=board[16][1];
    board[16][1]='.';

    // fullDisplay();
    //go to the next line if the current line is full
    if(board[14*side+j][i]=='.'){
        board[14*side+1][9]=48;
        board[14*side+1][8]=48;
        for(int k=0, p=0; p<2; k++){
            if(k==10){ k=0; p++; }
            board[14*side+1][9]+=value(board[14*side+p][k]);
            if(board[14*side+1][9]==58){
                board[14*side+1][9]=48;
                board[14*side+1][8]++;
            }
        }
        return;
    }
    //recursive call to align the next captured piece in case the piece is not the last one
    alignCapturedPieces(i, j);
    return;
}

//validation functions implementation
    //make global valid movement list, check list
int validMoveListPawn(int moveListIndex){
    for(int i=4; i<12; i++)
        for(int j=1; j<9; j++)
            if(board[i][j]=='P'+32*turn){
                //adding the next square
                moveList[moveListIndex][0]='P'+32*turn;
                moveList[moveListIndex][1]=j-1+'a';
                moveList[moveListIndex][2]='8'-(i-4);
                moveList[moveListIndex][3]=j-1+'a';
                moveList[moveListIndex][4]='8'-((i-1+2*turn)-4);
                moveListIndex++;
                //adding the two squares forward if the pawn hasn't moved before
                if(!movedPawnFlag[turn][j]){
                    if(board[i-1+2*turn][j]=='.' && board[i-2+4*turn][j]=='.'){
                        moveList[moveListIndex][0]='P'+32*turn;
                        moveList[moveListIndex][1]=j-1+'a';
                        moveList[moveListIndex][2]='8'-(i-4);
                        moveList[moveListIndex][3]=j-1+'a';
                        moveList[moveListIndex][4]='8'-((i-2+4*turn)-4);
                        moveListIndex++;
                    }
                }
                //adding the previous square if the pawn has moved before
                else
                    if(board[i+1-2*turn][j]=='.' || (board[i+1-2*turn][j]>='a'-32*turn && board[i+1-2*turn][j]<='z'-32*turn)){
                        moveList[moveListIndex][0]='P'+32*turn;
                        moveList[moveListIndex][1]=j-1+'a';
                        moveList[moveListIndex][2]='8'-(i-4);
                        moveList[moveListIndex][3]=j-1+'a';
                        moveList[moveListIndex][4]='8'-((i+1-2*turn)-4);
                        moveListIndex++;
                    }
            }
    return moveListIndex;
}

int validMoveListRook(int moveListIndex){
    for(int i=4; i<12; i++)
        for(int j=1; j<9; j++)
            //find rooks of the right turn
            if(board[i][j]=='R'+32*turn){
                //go forwards or backwards and check for empty or enemy squares and add them
                for(int r=i+1; r<12; r++){
                    if(board[r][j]=='.' || (board[r][j]>='a'-32*turn && board[r][j]<='z'-32*turn)){
                        moveList[moveListIndex][0]='R'+32*turn;
                        moveList[moveListIndex][1]=j-1+'a';
                        moveList[moveListIndex][2]='8'-(i-4);
                        moveList[moveListIndex][3]=j-1+'a';
                        moveList[moveListIndex][4]='8'-(r-4);
                        moveListIndex++;
                    }
                    //if found an unempty square, stop moving in that direction
                    if(board[r][j]!='.') break;
                }
                for(int r=i-1; r>3; r--){
                    if(board[r][j]=='.' || (board[r][j]>='a'-32*turn && board[r][j]<='z'-32*turn)){
                        moveList[moveListIndex][0]='R'+32*turn;
                        moveList[moveListIndex][1]=j-1+'a';
                        moveList[moveListIndex][2]='8'-(i-4);
                        moveList[moveListIndex][3]=j-1+'a';
                        moveList[moveListIndex][4]='8'-(r-4);
                        moveListIndex++;
                    }
                    if(board[r][j]!='.') break;
                }
                //go to the right
                for(int s=j+1; s<9; s++){
                    if(board[i][s]=='.' || (board[i][s]>='a'-32*turn && board[i][s]<='z'-32*turn)){
                        moveList[moveListIndex][0]='R'+32*turn;
                        moveList[moveListIndex][1]=j-1+'a';
                        moveList[moveListIndex][2]='8'-(i-4);
                        moveList[moveListIndex][3]=s-1+'a';
                        moveList[moveListIndex][4]='8'-(i-4);
                        moveListIndex++;
                    }
                    if(board[i][s]!='.') break;
                }
                //go to the left
                for(int s=j-1; s>0; s--){
                    if(board[i][s]=='.' || (board[i][s]>='a'-32*turn && board[i][s]<='z'-32*turn)){
                        moveList[moveListIndex][0]='R'+32*turn;
                        moveList[moveListIndex][1]=j-1+'a';
                        moveList[moveListIndex][2]='8'-(i-4);
                        moveList[moveListIndex][3]=s-1+'a';
                        moveList[moveListIndex][4]='8'-(i-4);
                        moveListIndex++;
                    }
                    if(board[i][s]!='.') break;
                }
            }
    return moveListIndex;
}

int validMoveListKnight(int moveListIndex){
    for(int i=4; i<12; i++)
        for(int j=1; j<9; j++)
            //find knights of the right turn
            if(board[i][j]=='N'+32*turn){
                if(j+2<9 && (board[i+3][j+2]=='.' || (board[i+3][j+2]>='a'-32*turn && board[i+3][j+2]<='z'-32*turn))){
                    moveList[moveListIndex][0]='N'+32*turn;
                    moveList[moveListIndex][1]=j-1+'a';
                    moveList[moveListIndex][2]='8'-(i-4);
                    moveList[moveListIndex][3]=(j+2)-1+'a';
                    moveList[moveListIndex][4]='8'-((i+3)-4);
                    moveListIndex++;
                }
                if(j-2>0 && (board[i+3][j-2]=='.' || (board[i+3][j-2]>='a'-32*turn && board[i+3][j-2]<='z'-32*turn))){
                    moveList[moveListIndex][0]='N'+32*turn;
                    moveList[moveListIndex][1]=j-1+'a';
                    moveList[moveListIndex][2]='8'-(i-4);
                    moveList[moveListIndex][3]=(j-2)-1+'a';
                    moveList[moveListIndex][4]='8'-((i+3)-4);
                    moveListIndex++;
                }
                if(j+2<9 && (board[i-3][j+2]=='.' || (board[i-3][j+2]>='a'-32*turn && board[i-3][j+2]<='z'-32*turn))){
                    moveList[moveListIndex][0]='N'+32*turn;
                    moveList[moveListIndex][1]=j-1+'a';
                    moveList[moveListIndex][2]='8'-(i-4);
                    moveList[moveListIndex][3]=(j+2)-1+'a';
                    moveList[moveListIndex][4]='8'-((i-3)-4);
                    moveListIndex++;
                }
                if(j-2>0 && (board[i-3][j-2]=='.' || (board[i-3][j-2]>='a'-32*turn && board[i-3][j-2]<='z'-32*turn))){
                    moveList[moveListIndex][0]='N'+32*turn;
                    moveList[moveListIndex][1]=j-1+'a';
                    moveList[moveListIndex][2]='8'-(i-4);
                    moveList[moveListIndex][3]=(j-2)-1+'a';
                    moveList[moveListIndex][4]='8'-((i-3)-4);
                    moveListIndex++;
                }
                if(j+3<9 && (board[i+2][j+3]=='.' || (board[i+2][j+3]>='a'-32*turn && board[i+2][j+3]<='z'-32*turn))){
                    moveList[moveListIndex][0]='N'+32*turn;
                    moveList[moveListIndex][1]=j-1+'a';
                    moveList[moveListIndex][2]='8'-(i-4);
                    moveList[moveListIndex][3]=(j+2)-1+'a';
                    moveList[moveListIndex][4]='8'-((i+3)-4);
                    moveListIndex++;
                }
                if(j-3>0 && (board[i+2][j-3]=='.' || (board[i+2][j-3]>='a'-32*turn && board[i+2][j-3]<='z'-32*turn))){
                    moveList[moveListIndex][0]='N'+32*turn;
                    moveList[moveListIndex][1]=j-1+'a';
                    moveList[moveListIndex][2]='8'-(i-4);
                    moveList[moveListIndex][3]=(j-2)-1+'a';
                    moveList[moveListIndex][4]='8'-((i+3)-4);
                    moveListIndex++;
                }
                if(j+3<9 && (board[i-2][j+3]=='.' || (board[i-2][j+3]>='a'-32*turn && board[i-2][j+3]<='z'-32*turn))){
                    moveList[moveListIndex][0]='N'+32*turn;
                    moveList[moveListIndex][1]=j-1+'a';
                    moveList[moveListIndex][2]='8'-(i-4);
                    moveList[moveListIndex][3]=(j+2)-1+'a';
                    moveList[moveListIndex][4]='8'-((i-3)-4);
                    moveListIndex++;
                }
                if(j-3>0 && (board[i-2][j-3]=='.' || (board[i-2][j-3]>='a'-32*turn && board[i-2][j-3]<='z'-32*turn))){
                    moveList[moveListIndex][0]='N'+32*turn;
                    moveList[moveListIndex][1]=j-1+'a';
                    moveList[moveListIndex][2]='8'-(i-4);
                    moveList[moveListIndex][3]=(j-2)-1+'a';
                    moveList[moveListIndex][4]='8'-((i-3)-4);
                    moveListIndex++;
                }
            }
    return moveListIndex;
}

int validMoveListBishop(int moveListIndex){
    for(int i=4; i<12; i++)
        for(int j=1; j<9; j++)
            //find bishops of the right turn
            if(board[i][j]=='B'+32*turn){
                //if bishop is in the left side of the board
                if(j>=1 && j<=4){
                    //go North-East and check for empty or enemy squares and add them
                    for(int r=i-1, s=j+1; r>3 && s<9;){
                        if(board[r][s]=='.' || (board[r][s]>='a'-32*turn && board[r][s]<='z'-32*turn)){
                            moveList[moveListIndex][0]='B'+32*turn;
                            moveList[moveListIndex][1]=j-1+'a';
                            moveList[moveListIndex][2]='8'-(i-4);
                            moveList[moveListIndex][3]=s-1+'a';
                            moveList[moveListIndex][4]='8'-(r-4);
                            moveListIndex++;
                        }
                        //if found an unempty square, stop moving in that direction
                        if(board[r][s]!='.') break;
                        r--; s++;
                    }
                    //go South-East
                    for(int r=i+1, s=j+1; r<12 && s<9;){
                        if(board[r][s]=='.' || (board[r][s]>='a'-32*turn && board[r][s]<='z'-32*turn)){
                            moveList[moveListIndex][0]='B'+32*turn;
                            moveList[moveListIndex][1]=j-1+'a';
                            moveList[moveListIndex][2]='8'-(i-4);
                            moveList[moveListIndex][3]=s-1+'a';
                            moveList[moveListIndex][4]='8'-(r-4);
                            moveListIndex++;
                        }
                        //if found an unempty square, stop moving in that direction
                        if(board[r][s]!='.') break;
                        r++; s++;
                    }
                }
                //if bishop is in the right side of the board
                if(j>=5 && j<=8){
                    //go North-West
                    for(int r=i-1, s=j-1; r>3 && s>0;){
                        if(board[r][s]=='.' || (board[r][s]>='a'-32*turn && board[r][s]<='z'-32*turn)){
                            moveList[moveListIndex][0]='B'+32*turn;
                            moveList[moveListIndex][1]=j-1+'a';
                            moveList[moveListIndex][2]='8'-(i-4);
                            moveList[moveListIndex][3]=s-1+'a';
                            moveList[moveListIndex][4]='8'-(r-4);
                            moveListIndex++;
                        }
                        //if found an unempty square, stop moving in that direction
                        if(board[r][s]!='.') break;
                        r--; s--;
                    }
                    //go South-West
                    for(int r=i+1, s=j-1; r<12 && s>0;){
                        if(board[r][s]=='.' || (board[r][s]>='a'-32*turn && board[r][s]<='z'-32*turn)){
                            moveList[moveListIndex][0]='B'+32*turn;
                            moveList[moveListIndex][1]=j-1+'a';
                            moveList[moveListIndex][2]='8'-(i-4);
                            moveList[moveListIndex][3]=s-1+'a';
                            moveList[moveListIndex][4]='8'-(r-4);
                            moveListIndex++;
                        }
                        //if found an unempty square, stop moving in that direction
                        if(board[r][s]!='.') break;
                        r++; s--;
                    }
                }
            }
    return moveListIndex;
}

int validMoveListQueen(int moveListIndex){
    for(int i=4; i<12; i++)
        for(int j=1; j<9; j++)
            //find the queen
            if(board[i][j]=='Q'+32*turn){
                //add the Rook moves
                //go forwards or backwards and check for empty or enemy squares and add them
                for(int r=i+1; r<12; r++){
                    if(board[r][j]=='.' || (board[r][j]>='a'-32*turn && board[r][j]<='z'-32*turn)){
                        moveList[moveListIndex][0]='Q'+32*turn;
                        moveList[moveListIndex][1]=j-1+'a';
                        moveList[moveListIndex][2]='8'-(i-4);
                        moveList[moveListIndex][3]=j-1+'a';
                        moveList[moveListIndex][4]='8'-(r-4);
                        moveListIndex++;
                    }
                    //if found an unempty square, stop moving in that direction
                    if(board[r][j]!='.') break;
                }
                for(int r=i-1; r>3; r--){
                    if(board[r][j]=='.' || (board[r][j]>='a'-32*turn && board[r][j]<='z'-32*turn)){
                        moveList[moveListIndex][0]='Q'+32*turn;
                        moveList[moveListIndex][1]=j-1+'a';
                        moveList[moveListIndex][2]='8'-(i-4);
                        moveList[moveListIndex][3]=j-1+'a';
                        moveList[moveListIndex][4]='8'-(r-4);
                        moveListIndex++;
                    }
                    if(board[r][j]!='.') break;
                }
                //go to the right
                for(int s=j+1; s<9; s++){
                    if(board[i][s]=='.' || (board[i][s]>='a'-32*turn && board[i][s]<='z'-32*turn)){
                        moveList[moveListIndex][0]='Q'+32*turn;
                        moveList[moveListIndex][1]=j-1+'a';
                        moveList[moveListIndex][2]='8'-(i-4);
                        moveList[moveListIndex][3]=s-1+'a';
                        moveList[moveListIndex][4]='8'-(i-4);
                        moveListIndex++;
                    }
                    if(board[i][s]!='.') break;
                }
                //go to the left
                for(int s=j-1; s>0; s--){
                    if(board[i][s]=='.' || (board[i][s]>='a'-32*turn && board[i][s]<='z'-32*turn)){
                        moveList[moveListIndex][0]='Q'+32*turn;
                        moveList[moveListIndex][1]=j-1+'a';
                        moveList[moveListIndex][2]='8'-(i-4);
                        moveList[moveListIndex][3]=s-1+'a';
                        moveList[moveListIndex][4]='8'-(i-4);
                        moveListIndex++;
                    }
                    if(board[i][s]!='.') break;
                }
                //add the bishop moves
                //if queen is in the left side of the board
                if(j>=1 && j<=4){
                    //go North-East and check for empty or enemy squares and add them

                    for(int r=i-1, s=j+1; r>3 && s<9;){
                        if(board[r][s]=='.' || (board[r][s]>='a'-32*turn && board[r][s]<='z'-32*turn)){
                            moveList[moveListIndex][0]='Q'+32*turn;
                            moveList[moveListIndex][1]=j-1+'a';
                            moveList[moveListIndex][2]='8'-(i-4);
                            moveList[moveListIndex][3]=s-1+'a';
                            moveList[moveListIndex][4]='8'-(r-4);
                            moveListIndex++;
                        }
                        //if found an unempty square, stop moving in that direction
                        if(board[r][s]!='.') break;
                        r--; s++;
                    }
                    //go South-East
                    for(int r=i+1, s=j+1; r<12 && s<9;){
                        if(board[r][s]=='.' || (board[r][s]>='a'-32*turn && board[r][s]<='z'-32*turn)){
                            moveList[moveListIndex][0]='Q'+32*turn;
                            moveList[moveListIndex][1]=j-1+'a';
                            moveList[moveListIndex][2]='8'-(i-4);
                            moveList[moveListIndex][3]=s-1+'a';
                            moveList[moveListIndex][4]='8'-(r-4);
                            moveListIndex++;
                        }
                        //if found an unempty square, stop moving in that direction
                        if(board[r][s]!='.') break;
                        r++; s++;
                    }
                }
                //if queen is in the right side of the board
                if(j>=5 && j<=8){
                    //go North-West
                    for(int r=i-1, s=j-1; r>3 && s>0;){
                        if(board[r][s]=='.' || (board[r][s]>='a'-32*turn && board[r][s]<='z'-32*turn)){
                            moveList[moveListIndex][0]='Q'+32*turn;
                            moveList[moveListIndex][1]=j-1+'a';
                            moveList[moveListIndex][2]='8'-(i-4);
                            moveList[moveListIndex][3]=s-1+'a';
                            moveList[moveListIndex][4]='8'-(r-4);
                            moveListIndex++;
                        }
                        //if found an unempty square, stop moving in that direction
                        if(board[r][s]!='.') break;
                        r--; s--;
                    }
                    //go South-West
                    for(int r=i+1, s=j-1; r<12 && s>0;){
                        if(board[r][s]=='.' || (board[r][s]>='a'-32*turn && board[r][s]<='z'-32*turn)){
                            moveList[moveListIndex][0]='Q'+32*turn;
                            moveList[moveListIndex][1]=j-1+'a';
                            moveList[moveListIndex][2]='8'-(i-4);
                            moveList[moveListIndex][3]=s-1+'a';
                            moveList[moveListIndex][4]='8'-(r-4);
                            moveListIndex++;
                        }
                        //if found an unempty square, stop moving in that direction
                        if(board[r][s]!='.') break;
                        r++; s--;
                    }
                }
                //add the standard knight moves
                if(board[i+2][j+1]=='.' || (board[i+2][j+1]>='a'-32*turn && board[i+2][j+1]<='z'-32*turn)){
                    moveList[moveListIndex][0]='Q'+32*turn;
                    moveList[moveListIndex][1]=j-1+'a';
                    moveList[moveListIndex][2]='8'-(i-4);
                    moveList[moveListIndex][3]=(j+1)-1+'a';
                    moveList[moveListIndex][4]='8'-((i+2)-4);
                    moveListIndex++;
                }
                if(board[i+2][j-1]=='.' || (board[i+2][j-1]>='a'-32*turn && board[i+2][j-1]<='z'-32*turn)){
                    moveList[moveListIndex][0]='Q'+32*turn;
                    moveList[moveListIndex][1]=j-1+'a';
                    moveList[moveListIndex][2]='8'-(i-4);
                    moveList[moveListIndex][3]=(j-1)-1+'a';
                    moveList[moveListIndex][4]='8'-((i+2)-4);
                    moveListIndex++;
                }
                if(board[i-2][j+1]=='.' || (board[i-2][j+1]>='a'-32*turn && board[i-2][j+1]<='z'-32*turn)){
                    moveList[moveListIndex][0]='Q'+32*turn;
                    moveList[moveListIndex][1]=j-1+'a';
                    moveList[moveListIndex][2]='8'-(i-4);
                    moveList[moveListIndex][3]=(j+1)-1+'a';
                    moveList[moveListIndex][4]='8'-((i-2)-4);
                    moveListIndex++;
                }
                if(board[i-2][j-1]=='.' || (board[i-2][j-1]>='a'-32*turn && board[i-2][j-1]<='z'-32*turn)){
                    moveList[moveListIndex][0]='Q'+32*turn;
                    moveList[moveListIndex][1]=j-1+'a';
                    moveList[moveListIndex][2]='8'-(i-4);
                    moveList[moveListIndex][3]=(j-1)-1+'a';
                    moveList[moveListIndex][4]='8'-((i-2)-4);
                    moveListIndex++;
                }
                if(j+2<9 && (board[i+1][j+2]=='.' || (board[i+1][j+2]>='a'-32*turn && board[i+1][j+2]<='z'-32*turn))){
                    moveList[moveListIndex][0]='Q'+32*turn;
                    moveList[moveListIndex][1]=j-1+'a';
                    moveList[moveListIndex][2]='8'-(i-4);
                    moveList[moveListIndex][3]=(j+2)-1+'a';
                    moveList[moveListIndex][4]='8'-((i+1)-4);
                    moveListIndex++;
                }
                if(j-2<9 && (board[i+1][j-2]=='.' || (board[i+1][j-2]>='a'-32*turn && board[i+1][j-2]<='z'-32*turn))){
                    moveList[moveListIndex][0]='Q'+32*turn;
                    moveList[moveListIndex][1]=j-1+'a';
                    moveList[moveListIndex][2]='8'-(i-4);
                    moveList[moveListIndex][3]=(j-2)-1+'a';
                    moveList[moveListIndex][4]='8'-((i+1)-4);
                    moveListIndex++;
                }
                if(j+2<9 && (board[i-1][j+2]=='.' || (board[i-1][j+2]>='a'-32*turn && board[i-1][j+2]<='z'-32*turn))){
                    moveList[moveListIndex][0]='Q'+32*turn;
                    moveList[moveListIndex][1]=j-1+'a';
                    moveList[moveListIndex][2]='8'-(i-4);
                    moveList[moveListIndex][3]=(j+2)-1+'a';
                    moveList[moveListIndex][4]='8'-((i-1)-4);
                    moveListIndex++;
                }
                if(j-2<9 && (board[i-1][j-2]=='.' || (board[i-1][j-2]>='a'-32*turn && board[i-1][j-2]<='z'-32*turn))){
                    moveList[moveListIndex][0]='Q'+32*turn;
                    moveList[moveListIndex][1]=j-1+'a';
                    moveList[moveListIndex][2]='8'-(i-4);
                    moveList[moveListIndex][3]=(j-2)-1+'a';
                    moveList[moveListIndex][4]='8'-((i-1)-4);
                    moveListIndex++;
                }
            }
    return moveListIndex;
}

int validMoveListKing(int moveListIndex){
    for(int i=4; i<12; i++)
        for(int j=1; j<9; j++)
            if(board[i][j]=='K'+32*turn){
                if(board[i-1][j]=='.' || (board[i-1][j]>='a'-32*turn && board[i-1][j]<='z'-32*turn)){
                    moveList[moveListIndex][0]='K'+32*turn;
                    moveList[moveListIndex][1]=j-1+'a';
                    moveList[moveListIndex][2]='8'-(i-4);
                    moveList[moveListIndex][3]=j-1+'a';
                    moveList[moveListIndex][4]='8'-((i-1)-4);
                    moveListIndex++;
                }
                if(board[i-1][j+1]=='.' || (board[i-1][j+1]>='a'-32*turn && board[i-1][j+1]<='z'-32*turn)){
                    moveList[moveListIndex][0]='K'+32*turn;
                    moveList[moveListIndex][1]=j-1+'a';
                    moveList[moveListIndex][2]='8'-(i-4);
                    moveList[moveListIndex][3]=(j+1)-1+'a';
                    moveList[moveListIndex][4]='8'-((i-1)-4);
                    moveListIndex++;
                }
                if(board[i][j+1]=='.' || (board[i][j+1]>='a'-32*turn && board[i][j+1]<='z'-32*turn)){
                    moveList[moveListIndex][0]='K'+32*turn;
                    moveList[moveListIndex][1]=j-1+'a';
                    moveList[moveListIndex][2]='8'-(i-4);
                    moveList[moveListIndex][3]=(j+1)-1+'a';
                    moveList[moveListIndex][4]='8'-(i-4);
                    moveListIndex++;
                }
                if(board[i+1][j+1]=='.' || (board[i+1][j+1]>='a'-32*turn && board[i+1][j+1]<='z'-32*turn)){
                    moveList[moveListIndex][0]='K'+32*turn;
                    moveList[moveListIndex][1]=j-1+'a';
                    moveList[moveListIndex][2]='8'-(i-4);
                    moveList[moveListIndex][3]=(j+1)-1+'a';
                    moveList[moveListIndex][4]='8'-((i+1)-4);
                    moveListIndex++;
                }
                if(board[i+1][j]=='.' || (board[i+1][j]>='a'-32*turn && board[i+1][j]<='z'-32*turn)){
                    moveList[moveListIndex][0]='K'+32*turn;
                    moveList[moveListIndex][1]=j-1+'a';
                    moveList[moveListIndex][2]='8'-(i-4);
                    moveList[moveListIndex][3]=j-1+'a';
                    moveList[moveListIndex][4]='8'-((i+1)-4);
                    moveListIndex++;
                }
                if(board[i+1][j-1]=='.' || (board[i+1][j-1]>='a'-32*turn && board[i+1][j-1]<='z'-32*turn)){
                    moveList[moveListIndex][0]='K'+32*turn;
                    moveList[moveListIndex][1]=j-1+'a';
                    moveList[moveListIndex][2]='8'-(i-4);
                    moveList[moveListIndex][3]=(j-1)-1+'a';
                    moveList[moveListIndex][4]='8'-((i+1)-4);
                    moveListIndex++;
                }
                if(board[i][j-1]=='.' || (board[i][j-1]>='a'-32*turn && board[i][j-1]<='z'-32*turn)){
                    moveList[moveListIndex][0]='K'+32*turn;
                    moveList[moveListIndex][1]=j-1+'a';
                    moveList[moveListIndex][2]='8'-(i-4);
                    moveList[moveListIndex][3]=(j-1)-1+'a';
                    moveList[moveListIndex][4]='8'-(i-4);
                    moveListIndex++;
                }
                if(board[i-1][j-1]=='.' || (board[i-1][j-1]>='a'-32*turn && board[i-1][j-1]<='z'-32*turn)){
                    moveList[moveListIndex][0]='K'+32*turn;
                    moveList[moveListIndex][1]=j-1+'a';
                    moveList[moveListIndex][2]='8'-(i-4);
                    moveList[moveListIndex][3]=(j-1)-1+'a';
                    moveList[moveListIndex][4]='8'-((i-1)-4);
                    moveListIndex++;
                }
            }
    return moveListIndex;
}

//Check function implementation
int isCheck(){
    //check if the king of the current turn is in check, 1 if in check, 0 if not
    char k1, k2, flag=0, tmpMoveList[550][6]={0};
    //find the current turn's king, save the coordinates in k1 and k2
    for(int i=4; i<12; i++)
        for(int j=1; j<9; j++)
            if(board[i][j]=='K'+32*turn){
                k1=j-1+'a';
                k2='8'-(i-4);
            }
    //copy the current moveList
    for(int i=0; i<550; i++)
        for(int j=0; j<6; j++)
            tmpMoveList[i][j]=moveList[i][j];
    //switch the turn
    turn=!turn;
    //create a new move list
    createMoveList();
    //check if the opponent would have the opportunity to capture the king
    for(int i=0; moveList[i][0]; i++)
        if(moveList[i][3]==k1 && moveList[i][4]==k2){ flag=1; break; }
    //revert the turn
    turn=!turn;
    //revert the move list
    for(int i=0; i<550; i++)
        for(int j=0; j<6; j++)
            moveList[i][j]=tmpMoveList[i][j];
    return flag;
}

int doesNotPutKingInCheck(){
    char tmpBoard[20][10];
    int tmpX1, tmpX2, tmpY1, tmpY2;
    //for every move listed in moveList,
    for(int k=0; moveList[k][0]; k++){
        //copy the current board into tmbBoard
        for(int i=0; i<20; i++)
            for(int j=0; j<10; j++)
                tmpBoard[i][j]=board[i][j];
        //move the piece
        tmpX1=moveList[k][1]-'a'+1; tmpY1=12-(moveList[k][2]-'0');
        tmpX2=moveList[k][3]-'a'+1; tmpY2=12-(moveList[k][4]-'0');
        movePiece(tmpX1, tmpY1, tmpX2, tmpY2);
        //if it was check, then the move isn't valid and therefore is deleted* from the moveList
        if(isCheck()){
            moveList[k][0]=' ';
            moveList[k][1]=' ';
            moveList[k][2]='!';
            moveList[k][3]=' ';
            moveList[k][4]=' ';
        }
        //return the board into the previous state
        for(int i=0; i<20; i++)
            for(int j=0; j<10; j++)
                board[i][j]=tmpBoard[i][j];
    }
}

void checkPawnFirstMove(char move[]){
    if(move[0]=='P' || move[0]=='p'){
        if(movedPawnFlag[turn][move[1]-'a'+1]!=1) movedPawnFlag[turn][move[1]-'a'+1]=1; 
    }
}

int isNotOutOfFrame(){
    //look for any moves that would go out of frame, delete* them
    for(int i=0; moveList[i][0]; i++)
        if(moveList[i][3]<'a' || moveList[i][3]>'h' || moveList[i][4]<'1' || moveList[i][4]>'8'){
            moveList[i][0]=' ';
            moveList[i][1]=' ';
            moveList[i][2]=' ';
            moveList[i][3]=' ';
            moveList[i][4]=' ';
        }
}

//
void createMoveList(){
    int moveListIndex=0;
    moveListIndex = validMoveListPawn(moveListIndex);
    moveListIndex = validMoveListRook(moveListIndex);
    moveListIndex = validMoveListKnight(moveListIndex);
    moveListIndex = validMoveListBishop(moveListIndex);
    moveListIndex = validMoveListQueen(moveListIndex);
    moveListIndex = validMoveListKing(moveListIndex);
    isNotOutOfFrame();
}

//move function implementation
int findValidMove(char move[]){
    int x1=move[1]-'a'+1, y1=12-(move[2]-'0');
    //Check if the move is valid, if it is return 1
    for(int i=0; moveList[i][0]; i++){
        int flag=1;
        for(int j=0; j<5; j++) if(move[j]!=moveList[i][j]) flag=0;
        if(flag) return 1;
    }
    //Else, Display an error message
    printf("Invalid move\n");
    return 0;
}

void movePiece(int x1, int y1, int x2, int y2){
    board[16][0]=board[y2][x2];
    board[y2][x2]=board[y1][x1];
    board[y1][x1]='.';
}

//pawn promotion function implementation
void pawnPromotion(){
    char promotionTarget=1;
    for(int i=4; i<12; i+=7)
        for(int j=1; j<9; j++)
            if(board[i][j]=='P'+(i/11)*32)
                while(promotionTarget){
                    printf("Choose the piece to promote to: (%c %c %c %c)", 'B'+(i/11)*32, 'N'+(i/11)*32, 'R'+(i/11)*32, 'Q'+(i/11)*32);
                    scanf(" %c", &promotionTarget);
                    if(promotionTarget=='B'+(i/11)*32 || promotionTarget=='N'+(i/11)*32 || promotionTarget=='R'+(i/11)*32 || promotionTarget=='Q'+(i/11)*32){
                        board[i][j]=promotionTarget;
                        promotionTarget=0;
                    }
                    else printf("Invalid piece");
                }
}

//board copy function implementation
void copyToHistory(){
    for(int i=0; i<20; i++)
        for(int j=0; j<10; j++)
            boardHistory[moveNumber][i][j]=board[i][j];
}

//number of the pieces still on the board function implementation
int numberOfPiecesOnBoard(){
    int numberOfPieces=0;
    for(int i=4; i<12; i++)
        for(int j=1; i<9; j++)
            if(board[i][j]!='.') numberOfPieces++;
    return numberOfPieces;
}

//game over function dependencies implementation
int isStalemate(){
    //checks if there are any valid moves
    for(int i=0; i<12500; i++)
        if(moveList[i][0]!=' ' && moveList[i][0]!=0)
            return 0;
    return 1;
}

int isThreefoldRepetition(){
    int numberOfRepetition=0;
    for(int k=moveNumber; k>0; k--){
        int isEqual=1;
        for(int i=4; i<12; i++)
            for(int j=1; j<9; j++)
                if(boardHistory[k][i][j]!=board[i][j]) isEqual=0;
        numberOfRepetition+=isEqual;
        if(numberOfRepetition==3) return 1;
    }
    return 0;
}
//-------------------------------------------------------------------------------------------------------------------------
int isInsufficientMaterial(){
    int state=0;
    if(numberOfPiecesOnBoard()==2) return 1;
    if(numberOfPiecesOnBoard()<7){
        for(int i=4; i<12; i++){
            for(int j=1; j<9; j++){
                //returns '0' after finding the first queen, rook, or pawn
                if(board[i][j]=='Q' || board[i][j]=='q' || board[i][j]=='R' || board[i][j]=='r' || board[i][j]=='P' || board[i][j]=='p') return 0;

            }
        }
    }
    return state;
}

int isDraw(){
    //check if the game is a draw
    if(isStalemate()) return 2;
    else if(isThreefoldRepetition()) return 3;
    // else if(isInsufficientMaterial()) return 4;
    //not a draw
    else return 0;
}

int isCheckmate(){
    if(isCheck() && isStalemate()) return 1;
    else return 0;
}

int isGameOver(){
    //checkmate
    if(isCheckmate()) return 1;
    //draw(stalemate or threefold repetition or insufficient material)
    else if(isDraw()) return isDraw();
    //not over
    else return 0;
}

//reset function implementation
void gameReset(){
    //reset move number
    moveNumber=0;
    //reset turn
    turn=0;
    //reset board
    for(int i=0; i<20; i++)
        for(int j=0; j<10; j++)
            board[i][j]=boardHistory[0][i][j];
    //reset boardHistory
    for(int k=0; k<12500; k++){
        if(!boardHistory[k][0][0]) break;
        for(int i=0; i<20; i++)
            for(int j=0; j<10; j++)
                boardHistory[k][i][j]=0;
    }
}

//replay function implementation
int doesNotWantToPlayAgain(){
    char tmp;
    printf("Do you want to play again? (Y/n)");
    scanf(" %c", &tmp);
    if(tmp=='Y' || tmp=='y'){
        gameReset();
        return 0;
    }
    return 1;
}

//moveList flusher function implementation
void flushMoveList(){
    for(int i=0; i<550; i++){
        if(!moveList[i][0]) break;  //breaks out when reaches an empty spot
        for(int j=0; j<6; j++) moveList[i][j]=0;
    }
}