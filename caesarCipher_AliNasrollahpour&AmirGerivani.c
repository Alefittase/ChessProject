#include <stdio.h>
#include <stdlib.h>

void caesarCipher(FILE *inFile, FILE *outFile, int shift){
    char ch;
    while ((ch = fgetc(inFile)) != EOF){
        if (ch >= 'a' && ch <= 'z')
            ch = ((ch - 'a' + shift) % 26) + 'a';
        else if (ch >= 'A' && ch <= 'Z')
            ch = ((ch - 'A' + shift) % 26) + 'A';
        fputc(ch, outFile);
    }
}

int main(){
    FILE *inFile = fopen("input.txt", "r");
    FILE *outFile = fopen("output.txt", "w");
    int shift; scanf("%d", &shift);

    caesarCipher(inFile, outFile, shift);

    fclose(inFile);
    fclose(outFile);

    printf("File encrypted successfully\n");
    return 0;
}