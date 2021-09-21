#include <time.h>
#include <stdio.h>
#include <unistd.h>

int main () {
    clock_t start_t, end_t;
    int i;
    double total_t, tiempo1, tiempo2;
    start_t = clock();
    tiempo1 = (double) start_t / CLOCKS_PER_SEC;
    printf("Starting of the program, start_t = %f\n", tiempo1);
    
    printf("Going to scan a big loop, start_t = %f\n", tiempo1);
    for(i=0; i< 100000000; i++) {
    }
    end_t = clock();
    tiempo2 = (double) end_t / CLOCKS_PER_SEC;
    printf("End of the big loop, end_t = %f\n", tiempo2);
    
    for(i=0; i< 100000000; i++) {
    }
    end_t = clock();
    for(i=0; i< 100000000; i++) {
    }
    start_t = end_t;
    tiempo2 = (double) end_t / CLOCKS_PER_SEC;
    printf("End of the big loop, end_t = %f\n", tiempo2);
    total_t = tiempo2-tiempo1;
    printf("Total time taken by CPU: %f\n", total_t  );
    end_t = clock();
    tiempo1 = (double) start_t / CLOCKS_PER_SEC;
    printf("End of the big loop, end_t = %f\n", tiempo1);
    printf("Exiting of the program...\n");

    return(0);
}