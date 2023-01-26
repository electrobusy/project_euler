#include <stdio.h>
#include <stdlib.h> // atoi() 
#include <string.h> // strtok()

#define NUM_VERTICES 40 // number of nodes in the network (same as number of lines in the file) 
#define BUFFER_SIZE 255 

#define INF 10000 // minimum span algorithm

/*
* Function description: Custom made file reader read file (which follows the .csv convention) 
* & store network's weights in matrix (using pointer arithmetic)
*/
int ReadCSVFile(int fileNumLines, int weightMatrix[fileNumLines][fileNumLines], const char* fileName, int* totalSum){
 
    // dealing with the file
    char lineString[BUFFER_SIZE]; 
    char *token;
    const char del[] = ",";
    
    // for looping through weight matrix
    int c = 0;
    int l = 0;

    // open file
    FILE* filePtr = fopen(fileName,"r");
    if(filePtr == NULL){
        printf("Unable to open file!\n");
        return 0;
    }

    // read line from file and store weights
    while(fgets(lineString, BUFFER_SIZE, filePtr) != NULL){
        
        /* divide string in substring */
        token = strtok(lineString, del); // get first subString
        while(token != NULL){
            if(*token > '0'){
                // printf(" %s\n", token);
                *(*(weightMatrix + c) + l) = atoi(token);
                *totalSum += *(*(weightMatrix + c) + l);
            }
            if(*token == '-'){
                // printf(" %s\n", token );
                *(*(weightMatrix + c) + l) = 0; 
            }

            // get next subString
            token = strtok(NULL, del);
            c++;
        }

        l++; // next line
        c = 0; // reset columns
    }

    // Symmetric matrix - hence the total sum is divided by half
    *totalSum /= 2;

    // close file
    fclose(filePtr);

    /*
    // print matrix
    for(l=0; l<NUM_VERTICES; l++){
        for(c=0; c<NUM_VERTICES; c++){
            printf("%d ", *(*(weightMatrix + c) + l));
        }
        printf("\n");
    }
    */
     
    /* -- done in previous loop to save time
    // Calculate total cost (sum all weights in the network)
    for(int i=0; i<NUM_VERTICES; i++){
        for(int j=i; j<NUM_VERTICES;j++){
            *totalSum += *(*(weightMatrix + i) + j); 
        }
    }
    */
    
    printf("Total cost: %d \n", *totalSum);
}

int main(void){

    printf("-- Project Euler - Problem 107 -- \n");

    // Network/graph represented as a cost matrix (2D array) - symmetric matrix
    int cost[NUM_VERTICES][NUM_VERTICES]; 
    int initCost = 0; 

    // 1 - read file and store weights
    ReadCSVFile(NUM_VERTICES, cost, "p107_network.txt", &initCost);
    
    // 2 - quick/dirty solution -> minimum span algorithm (from Optimization course/book)
    
    // "bool" array that acts as a mask to check visited node
    int visited[NUM_VERTICES]; 
    memset(visited, 0, sizeof(visited)); // set all elements to false

    // start w/ first node by default
    int nodeCounter = 0; 
    visited[0] = 1;

    // dummy variables to store matrix indices that point found edge w/ minimum cost
    int x; 
    int y;

    // variables to compute minimum network cost 
    int mask = 0;
    int minCost = 0;

    while(nodeCounter < NUM_VERTICES){
        // set minimum cost to very high value
        int min = INF;
        // go through visited nodes
        for(int i=0; i<NUM_VERTICES; i++){
            if(visited[i]){
                // printf("\n -- VISIT NODE %d \n", i); // Debugging 
                // go through unvisited nodes
                for(int j=0; j<NUM_VERTICES; j++){
                    // printf("\t Unvisited node %d \n", j);
                    if((cost[i][j]) && (visited[j] == 0) && (min > cost[i][j])){
                        min = cost[i][j];
                        x = i;
                        y = j;
                        // printf("\t \t Minimum edge: Node %d - Node %d : Min cost %d \n", x, y, min); // print chosen edge
                        mask = 1; // to computer minimum cost
                    }
                }
            }
        }
        if (mask){
            // printf("CHOSEN EDGE: %d - %d : %d \n", x, y, cost[x][y]); // print chosen edge
            visited[y] = 1; // mark visited node (in the second loop)
            minCost += cost[x][y];
            mask = 0;
        }
        nodeCounter++; // next edge 
    }

    // 3 - Computer gain
    printf("\nTotal Cost: %d\nMinimum Cost: %d\nGain: %d\n", initCost, minCost, initCost - minCost);

    return 0;
}