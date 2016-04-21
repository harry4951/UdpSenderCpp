#include "GenerateRandomId.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#define N_smallpacket 4

int RandomId()
 {
    static int count_times=0;
    static int myArray[N_smallpacket] = {0};
    int randomIndex,randomValue;
    //Initial the ID number    
    if(count_times==0)
    {
      for(int i=0; i<N_smallpacket; i++)
      {
        myArray[i]=i;
      }
    }
    //Generate random ID from the given array
    //srand(time(NULL));
    randomIndex = rand() % (N_smallpacket-count_times);
    randomValue = myArray[randomIndex];

    //Reform the array except the one generated previously
    for(int i=randomIndex; i<N_smallpacket-count_times-1; i++)
    {
      myArray[i]=myArray[i+1];
    }

    count_times++;
    if(count_times==N_smallpacket)
    {
        count_times=0;

    }
    return randomValue;
 }