#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <pthread.h> 
#include <semaphore.h>

#define Waiting_Chairs 10
#define Total_Customers 50

#define true 1

sem_t Waiting_Room;
// sem_t DW_1;
// sem_t DW_2;
// sem_t Bill;
sem_t Dealing[3];
 
char Deposit_String[] = {"Deposit"};
char Withdraw_String[] = {"Withdraw"};
char Billing_String[] = {"Billing"};

struct Customer_Info
{
    int ID;
    char Purpose;
};




int Deposit_Customers , Withdraw_Customers , Bill_Customers;



void Deposit_Withdraw_1(struct Customer_Info *Customer_ID)
{
    
    char *p;
    sem_wait(&Dealing[0]);
    if (Customer_ID->Purpose == 'd')
    {
        p = &Deposit_String[0];
        // Deposit_Customers--;
    }
    else
    {
        p = &Withdraw_String[0];
        // Withdraw_Customers--;
    }

    printf("Customer with ID %d have reached Deposit/Withdraw Counter 1 for the purpose of %s\n" , Customer_ID->ID , p);
    printf("Customer with ID %d %sing the money\n", Customer_ID->ID , p);
    sleep(5);
    printf("Customer with ID %d %sed the money\n", Customer_ID->ID , p);
    sem_post(&Dealing[0]);

}


void Deposit_Withdraw_2(struct Customer_Info *Customer_ID)
{
    
    char *p;
    sem_wait(&Dealing[1]);
    if (Customer_ID->Purpose == 'd')
    {
        p = &Deposit_String[0];
        // Deposit_Customers--;
    }
    else
    {
        p = &Withdraw_String[0];
        // Withdraw_Customers--;
    }

    printf("Customer with ID %d have reached Deposit/Withdraw Counter 2 for the purpose of %s\n" , Customer_ID->ID , p);
    printf("Customer with ID %d %sing the money\n", Customer_ID->ID , p);
    sleep(5);
    printf("Customer with ID %d %sed the money\n", Customer_ID->ID , p);
    sem_post(&Dealing[1]);

}


void Billing(struct Customer_Info *Customer_ID)
{
    
    sem_wait(&Dealing[2]);
    printf("Customer with ID %d have reached Billing Counter 2 to pay Bills\n" , Customer_ID->ID);
    printf("Customer with ID %d Paying Bill\n", Customer_ID->ID );
    sleep(5);
    printf("Customer with ID %d Payed the Bill\n", Customer_ID->ID);
    sem_post(&Dealing[2]);

}



void* Customers(void *Customer_ID)
{
    struct Customer_Info *I = (struct Customer_Info *)Customer_ID;
    sleep(rand() % 3);
    printf("Customer with ID %d Reached the Bank\n" , I->ID);
    int value; 
    sem_getvalue(&Waiting_Room, &value);
    sleep(1);
    
    if (value <= 0)
    {
        printf("Waiting Room is full , Customer with ID %d is waiting outside\n", I->ID);
    }
    
    sem_wait(&Waiting_Room);
    printf("Customer with ID %d entered in the Waiting Room\n", I->ID);
    
    if (I->Purpose == 'd' || I->Purpose == 'w')
    {
        
        while (true)
        {   
            int a , b;
            sem_getvalue(&Dealing[0], &a);
            sem_getvalue(&Dealing[1], &b);
            if (a)
            {
              
                Deposit_Withdraw_1(I);
                break;
            }
            else if (b)
            {
                Deposit_Withdraw_2(I);
                break;
            }
        }
        
    }
    else
    {
        while(true)
        {
            int a;
            sem_getvalue(&Dealing[2], &a);
            if (a)
            {
                Billing(I);
                break;
            }
        }
    }
    

    
    
    printf("Customer with ID %d leaving the Waiting Room\n", I->ID);
    sem_post(&Waiting_Room);




}


int main()
{
    srand(time(0)); 
    
    sem_init(&Waiting_Room, 0, Waiting_Chairs);
    
    // sem_init(&DW_1, 0, 0);
    // sem_init(&DW_2, 0, 0);
    // sem_init(&Bill, 0, 0);
    
    sem_init(&Dealing[0], 0, 1);
    sem_init(&Dealing[1], 0, 1);
    sem_init(&Dealing[2], 0, 1);
    
    printf("How many customers for Deposit ? \n");
    scanf("%d" , &Deposit_Customers);

    printf("How many customers for Withdraw ? \n");
    scanf("%d" , &Withdraw_Customers);
    
    printf("How many customers for Deposit or Withdraw ? \n");
    scanf("%d" , &Bill_Customers);


    pthread_t Customers_Threads[Deposit_Customers + Withdraw_Customers + Bill_Customers];
    struct Customer_Info C[Deposit_Customers + Withdraw_Customers + Bill_Customers];

    for (int i = 0; i < Deposit_Customers; i++)
    {
        C[i].ID = i+1;
        C[i].Purpose = 'd';
        pthread_create(&Customers_Threads[i], NULL , Customers , &C[i]);

    }

    for (int i = Deposit_Customers; i < Deposit_Customers + Withdraw_Customers; i++)
    {
        C[i].ID = i+1;
        C[i].Purpose = 'w';
        pthread_create(&Customers_Threads[i], NULL , Customers , &C[i]);
            
    }

    for (int i = Deposit_Customers + Withdraw_Customers; i < Deposit_Customers + Withdraw_Customers + Bill_Customers; i++)
    {
        C[i].ID = i+1;
        C[i].Purpose = 'b';
        pthread_create(&Customers_Threads[i], NULL , Customers , &C[i]);
            
    }


    for (int i = 0; i < Deposit_Customers + Withdraw_Customers + Bill_Customers; i++)
    {
        pthread_join(Customers_Threads[i], NULL);
    }
       



}