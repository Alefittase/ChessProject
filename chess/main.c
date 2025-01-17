/*
chess/main.c
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BlackOdd(string) "\x1b[40m\x1b[92m" string "\x1b[0m"
#define WhiteOdd(string) "\x1b[40m\x1b[31m" string "\x1b[0m"
#define BlackEven(string) "\x1b[47m\x1b[92m" string "\x1b[0m"
#define WhiteEven(string) "\x1b[47m\x1b[31m" string "\x1b[0m"
#define RestOfTheBoard(string) "\x1b[44m" string "\x1b[0m"


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

//Check function
int isCheck(int color);

//piece color for movment validation function
int isPieceOfTheRightColor(int x, int y);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//validation functions (creates a list of valid moves for each piece)
int validMoveListPawn();
int validMoveListRook();
int validMoveListKnight();
int validMoveListBishop();
int validMoveListQueen();
int validMoveListKing();

void checkPawnFirstMove();
int doesNotPutKingInCheck();
int isNotOutOfFrame();

void createMoveList();
////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Move functions
int findValidMove(char move[5]);
void movePiece(int x1, int y1, int x2, int y2);

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

int isGameOver(){
    /*return the typeOfEnding/gameNotOver:*/
    //checkmate
    if(isCheckmate()) return 1;
    //draw(stalemate or threefold repetition or insufficient material)
    else if(isDraw()) return isDraw();
    //not over
    else return 0;
}

//reset function
void gameReset();

//replay function
int doesWantToPlayAgain();

//moveList flusher function
void flushMoveList();

//Global variables
char boardHistory[12500][20][10];
int moveNumber=0, wnatsToPlayAgain=0;
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
        //Increment the move number
        moveNumber++;
        //While the move is not validated get the player's input
        int moveValidated=0;
        while(!moveValidated){
            int x1, x2, y1, y2, valid=0;
            char move[5];
            //receive the player's input
            printf("To exit the game, enter \"End\"\n");
            printf("Enter the move: ");
            scanf("%s", move);
            //check for manual exit
            if(move[0]=='E' && move[1]=='n' && move[2]=='d') break;
            //convert input to the board coordinates
            x1=move[1]-'a'+1; y1=12-(move[2]-'0');
            x2=move[3]-'a'+1; y2=12-(move[4]-'0');
            //create the valid move list
                //flush the moveList
            flushMoveList();
                //createMoveList();
            scanf("%s", moveList[0]);
            //Check if the move is valid (by searching throw the moveList)
            if(findValidMove(move)){
                //move the piece, and validate the move
                movePiece(x1, y1, x2, y2);
                moveValidated=1;
                checkPawnFirstMove(move);
            }
            printf("1\n");
        }
        //align the captured piece in the top/bottom of the board
        printf("2\n");
        alignCapturedPieces(0,0);
        //Check if the game is over
        //if the game is over
            //break the main loop
            printf("3\n");
        if(isGameOver()){
            printf("4\n");
            switch (isGameOver()){
            case 1:
                printf("the game ended in a Checkmate the winner is:\n");
                if(turn) printf("Black\n");
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
            if(!doesWantToPlayAgain()) return 0;
        }
        //Switch the turn
        printf("5\n");
        turn=!turn;
        //if the apponent's king is in check
            //callout the check
            printf("6\n");
        if(isCheck(turn)) printf("Check\n");
    }
    // //Display the final board
    // system("clear");
    // displayBoard();
    // //Display the game over message
    // printf("Game is Over\n");
    // printf("press any button to exit");
    // //wait for the player to exit
    // char tmpchr;
    // scanf(" %c", &tmpchr);
    // return 0;
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
    // system("clear");
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

//captured pieces alignment function implementation
void alignCapturedPieces(int i, int j){
    int side=pieceColor(board[16][0]);
    printf("%d %c %c\n", side, board[14*side+0][0], board[16][0]);
    //find the location to move the new piece
    for( ; value(board[14*side+j][i]) > value(board[16][0]);i++){
        if(i==10){ i=0; j++; }
        printf("%d\n", i);
    }
    fullDisplay();
    //put the piece in the desired location and move the next captured piece to the top of the captured pieces buffer
    board[16][1]=board[14*side+j][i];
    board[14*side+j][i]=board[16][0];
    board[16][0]=board[16][1];
    board[16][1]='.';

    fullDisplay();
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

//----------------------------------------------------------------------------------------------------------
//Check function implementation
int isCheck(int color){
    return 0;
    /*check if the king of the given color is in check*/
    //return 1 if in check
    //return 0 if not in check
}

//piece movement validation functions implementation
int isPieceOfTheRightColor(int x, int y){
    //check if the piece is of the right color
    //return 1 if right color
    if(turn==0) if(64<board[x][y] && board[x][y]<91) return 1;
    else if(96<board[y][x] && board[x][y]<123) return 1;
    //return 0 if not
    else return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

//validation functions implementation
    //make global valid movement list, check list
int validMoveListPawn(int moveListIndex){
    for(int i=4; i<12; i++){
        for(int j=1; j<9; j++){
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
                    if(board[i-1+2*turn][j]=='.' && (board[i-2+4*turn][j]=='.' || (board[i-2+4*turn][j]>='a'-32*turn && board[i-2+4*turn][j]<='z'-32*turn))){
                        moveList[moveListIndex][0]='P'+32*turn;
                        moveList[moveListIndex][1]=j-1+'a';
                        moveList[moveListIndex][2]='8'-(i-4);
                        moveList[moveListIndex][3]=j-1+'a';
                        moveList[moveListIndex][4]='8'-((i-2+4*turn)-4);
                        moveListIndex++;
                    }
                }
                //adding the previous square if the pawn has moved before
                else{
                    if(board[i+1-2*turn][j]=='.' || (board[i+1-2*turn][j]>='a'-32*turn && board[i+1-2*turn][j]<='z'-32*turn)){
                        moveList[moveListIndex][0]='P'+32*turn;
                        moveList[moveListIndex][1]=j-1+'a';
                        moveList[moveListIndex][2]='8'-(i-4);
                        moveList[moveListIndex][3]=j-1+'a';
                        moveList[moveListIndex][4]='8'-((i+1-2*turn)-4);
                        moveListIndex++;
                    }
                }
            }
        }
    }
    return moveListIndex;
}

int validMoveListRook(moveListIndex){
    for(int i=4; i<12; i++){
        for(int j=1; j<9; j++){
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
        }
    }
    return moveListIndex;
}

int validMoveListKnight(int moveListIndex){
    for(int i=4; i<12; i++){
        for(int j=1; j<9; j++){
            //find knights of the right turn
            if(board[i][j]=='K'+32*turn){
                if(i+3<12 && j+2<9 && (board[i+3][j+2]=='.' || (board[i+3][j+2]>='a'-32*turn && board[i+3][j+2]<='z'-32*turn))){
                    moveList[moveListIndex][0]='R'+32*turn;
                    moveList[moveListIndex][1]=j-1+'a';
                    moveList[moveListIndex][2]='8'-(i-4);
                    moveList[moveListIndex][3]=(j+2)-1+'a';
                    moveList[moveListIndex][4]='8'-((i+3)-4);
                    moveListIndex++;
                }
                if(i+3<12 && j-2>0 && (board[i+3][j-2]=='.' || (board[i+3][j-2]>='a'-32*turn && board[i+3][j-2]<='z'-32*turn))){
                    moveList[moveListIndex][0]='R'+32*turn;
                    moveList[moveListIndex][1]=j-1+'a';
                    moveList[moveListIndex][2]='8'-(i-4);
                    moveList[moveListIndex][3]=(j-2)-1+'a';
                    moveList[moveListIndex][4]='8'-((i+3)-4);
                    moveListIndex++;
                }
                if(i-3>3 && j+2<9 && (board[i-3][j+2]=='.' || (board[i-3][j+2]>='a'-32*turn && board[i-3][j+2]<='z'-32*turn))){
                    moveList[moveListIndex][0]='R'+32*turn;
                    moveList[moveListIndex][1]=j-1+'a';
                    moveList[moveListIndex][2]='8'-(i-4);
                    moveList[moveListIndex][3]=(j+2)-1+'a';
                    moveList[moveListIndex][4]='8'-((i-3)-4);
                    moveListIndex++;
                }
                if(i-3>3 && j-2>0 && (board[i-3][j-2]=='.' || (board[i-3][j-2]>='a'-32*turn && board[i-3][j-2]<='z'-32*turn))){
                    moveList[moveListIndex][0]='R'+32*turn;
                    moveList[moveListIndex][1]=j-1+'a';
                    moveList[moveListIndex][2]='8'-(i-4);
                    moveList[moveListIndex][3]=(j-2)-1+'a';
                    moveList[moveListIndex][4]='8'-((i-3)-4);
                    moveListIndex++;
                }
            }
        }
    }
    return moveListIndex;
}

int validMoveListBishop(int moveListIndex){
    for(int i=4; i<12; i++){
        for(int j=1; j<9; j++){
            //find bishops of the right turn
            if(board[i][j]=='B'+32*turn){
                //if bishop is in the left side of the board
                if(j>=1 && j<=4){
                    //go North-East and check for empty or enemy squares and add them
                    for(int r=i-1; r>3; r--){
                        for(int s=j+1; s<9; s++){
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
                        }
                    }
                    //go South-East
                    for(int r=i+1; r<12; r++){
                        for(int s=j+1; s<9; s++){
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
                        }
                    }
                }

                //if bishop is in the right side of the board
                if(j>=5 && j<=8){
                    //go North-West
                    for(int r=i-1; r>3; r--){
                        for(int s=j-1; s>0; s--){
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
                        }
                    }
                    //go South-West
                    for(int r=i+1; r<12; r++){
                        for(int s=j-1; s>0; s--){
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
                        }
                    }
                } 
            }
        }
    }
    return moveListIndex;
}

int validMoveListQueen(){

}

int validMoveListKing(){

}

int doesNotPutKingInCheck(){

}

void checkPawnFirstMove(char move[]){
    if(move[0]=='P' || move[0]=='p'){
        if(movedPawnFlag[turn][move[1]-'a'+1]!=1) movedPawnFlag[turn][move[1]-'a'+1]=1; 
    }
}

//
void createMoveList(){
    int moveListIndex=0;
    validMoveListPawn(moveListIndex);
    validMoveListRook(moveListIndex);
    validMoveListKnight(moveListIndex);
    validMoveListBishop(moveListIndex);
    validMoveListQueen(moveListIndex);
    validMoveListKing(moveListIndex);
    doesNotPutKingInCheck();

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////


//move function implementation
int findValidMove(char move[5]){
    int i=0;
    int x1=move[1]-'a'+1, y1=12-(move[2]-'0');
    //Check if the move is valid
    //If the move is valid
        //return the index of the valid move in the moveList
    while(moveList[i][0]){
        int flag=1;
        for(int j=0; j<5; j++)
            if(move[j]!=moveList[i][j]) flag=0;
        if(flag) return i-1;
        i++;
    }
    //Else
        //Display an error message
    printf("Invalid move\n");
    return 0;
}

void movePiece(int x1, int y1, int x2, int y2){
    board[16][0]=board[y2][x2];
    board[y2][x2]=board[y1][x1];
    board[y1][x1]='.';
}

//board copy function implementation
void copyToHistory(){
    char (*pointer)[20][10]=boardHistory;
    for(int i=0; i<20; i++)
        for(int j=0; j<10; j++)
            boardHistory[moveNumber][i][j]=board[i][j];
        //    *(*(*(pointer+moveNumber)+i)+j)=board[i][j];
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
    if(moveList[0][0]) return 0;
    return 1;
}

int isThreefoldRepetition(){
    int numberOfRepetition=0;
    for(int i=moveNumber; i>0; i--){
        int isEqual=1;
        for(int j=4; j<12; j++){
            for(int k=1; k<9; k++){
                if(boardHistory[i][j][k]!=board[j][k]) isEqual=0;
                if(!isEqual) break;
            }
            if(!isEqual) break;
        }
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
    /*check if the game is a draw*/
    //stalemate
    if(isStalemate()) return 2;
    //threefold repetition
    else if(isThreefoldRepetition()) return 3;
    //insufficient material
    // else if(isInsufficientMaterial()) return 4;
    //not a draw
    else return 0;
}

int isCheckmate(){
    if((isCheck(0) && isStalemate(0)) || (isCheck(1) && isStalemate(1))) return 1;
    else return 0;
}

//reset function implementation
void gameReset(){
    //reset turn
    turn=1;
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
    moveNumber=0;
}

//replay function implementation
int doesWantToPlayAgain(){
    char tmp;
    printf("Do you want to play again? (Y/n)");
    scanf(" %c", &tmp);
    if(tmp=='Y' || tmp=='y'){
        gameReset();
        return 1;
    }
    return 0;
}

//moveList flusher function implementation
void flushMoveList(){
    for(int i=0; i<550; i++){
        if(!moveList[i][0]) break;  //breaks out when reaches an empty spot
        for(int j=0; j<6; j++) moveList[i][j]=0;
    }
}
