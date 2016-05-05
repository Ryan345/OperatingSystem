#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

// Agent for when stuff on the table
sem_t rAgent;

// Make sure the pusher is only one on table
sem_t lPush;

// Make sure a smoker has all items needed
sem_t smokers[3];

// Items pusher can give out
sem_t pushers[3];

// Types of items a smoker can start with
char* types[3] = { "tobacco","paper","matches" };

// Matches with types to show if the items are on the table
int onTable[3] = { 0,0,0 };

void* smoker(void* x)
{
   int sId = *(int*)x;
   int i = 0;
   int type = sId % 3;
   printf("Smoker of id %d created, has %s\n",sId,types[type]);

   for(i = 0; i < 3; ++i)
   {
      sem_wait(&smokers[type]);
      printf("Smoker %d now making cigarette\n",sId);
      sem_post(&rAgent);
      printf("Smoker %d now smoking\n",sId);
      if(i==2)
         printf("Smoker %d finished all smoking\n",sId);
   }
}

void* pusher(void* x)
{
   int pId = *(int*)x;
   int i = 0;
   for(i = 0; i < 12; ++i)
   {
      // Wait until we are needed
      sem_wait(&pushers[pId]);
      // Wait until we have lock
      sem_wait(&lPush);

      // Check what items are on table, and take them off, then tell the corresponding smoker he is ready
      if(onTable[(pId+1)%3] == 1)
      {
         onTable[(pId+1)%3] = 0;
         sem_post(&smokers[(pId+2)%3]);
      }
      else if(onTable[(pId+2)%3] == 1)
      {
         onTable[(pId+2)%3] = 0;
         sem_post(&smokers[(pId+1)%3]);
      }
      else
      {
         onTable[pId] = 1;
      }
      sem_post(&lPush);
   }
}

void* agent(void* x)
{
   int aId = *(int*)x;
   int i = 0;

   for(i = 0; i < 6; ++i)
   {
      // Wait for up to 200 ms
      usleep(rand()%201);
      // Wait until we are needed
      sem_wait(&rAgent);

      // Put items needed on the tables
      sem_post(&pushers[aId]);
      sem_post(&pushers[(aId+1)%3]);
   }

}

int main()
{
   int i = 0;
   int retVal;

   // Seed the random generator with the time
   srand(time(NULL));
   sem_init(&rAgent,0,1);
   sem_init(&lPush,0,1);

   // Create 3 smokers and pushers
   for(i = 0; i < 3; ++i)
   {
      sem_init(&smokers[i],0,0);
      sem_init(&pushers[i],0,0);
   }

   int sId[6];
   pthread_t tSmokers[6];

   // Create 6 smokers, store IDs in the sId array
   for (i = 0; i < 6; ++i)
   {
      sId[i] = i;
      pthread_create(&tSmokers[i], NULL, smoker, &sId[i]);
   }

   int pId[3];
   pthread_t tPushers[3];

   // Create 3 pushers, store IDs in pId
   for (i = 0; i < 3; ++i)
   {
      pId[i] = i;
      pthread_create(&tPushers[i],NULL,pusher,&pId[i]);
   }

   int aId[3];
   pthread_t tAgents[3];

   // Create 3 agents, store IDs in aId
   for (i = 0; i < 3; ++i)
   {
      aId[i] = i;
      pthread_create(&tAgents[i],NULL,agent,&aId[i]);
   }

   // Wait for the smokers to finish smoking
   for (i = 0; i < 6; ++i)
   {
      pthread_join(tSmokers[i],NULL);
   }

}

