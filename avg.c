#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
    char str[20];
    int count = 0;
    float sum = 0;
    // Declare file from cmd args
    FILE *fp = fopen(argv[1], "r");
    // File does not exist
    if(fp == NULL) {
        printf("cannot open file\n");
        exit(1);
    }
    // Loop through file
    while(fgets(str, 20, fp) != 0) {
        sum += atof(str);
        count++;
    }
    // Print output
    printf("%f\n", (sum/count));
    fclose(fp);
    return 0;
}
