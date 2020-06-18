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



void Sleep_Wait(int num)
{
    for (int i = 0; i < num*10000; i++)
        for (int i = 0; i < num*10000; i++) {}   
}



sem_t Waiting_Room;
sem_t Dealing[3];
sem_t Shared_Resource[3];
sem_t Printing;


char Deposit_String[] = {"Deposit"};
char Withdraw_String[] = {"Withdraw"};
char Billing_String[] = {"Billing"};


struct Customer_Info
{
    int ID;
    char Purpose;
    int type;
};




int Deposit_Customers , Withdraw_Customers , Bill_Customers;
int mode , tr_customer_number;

int flag = 1;

int count1_x = 0;
int count2_x = 0;
int count3_x = 0;



// void gotoxy(int x,int y)
// {
//     printf("%c[%d;%df",0x1B,y,x);
// }

void box_outline ()
{
	//gotoxy(0,50);
	printf("%c[%d;%df",0x1B, 50 , 0 );
    
	printf("**************************An Overview of the customers that are dealth with.********************************\n");
	printf("|COUNTERS.         |     CUSTOMER NUMBER\n");
	printf("|__________________|_______________________________________________________________________________________|\n");
	printf("|Counter Number 1  |                                                                                       |\n");
	printf("|Counter Number 2  |                                                                                       |\n");
	printf("|Counter Number 3  |                                                                                       |\n");
	printf("************************************************************************************************************\n");
	
}


void clearing (int x ,int y)
{
    sem_wait(&Shared_Resource[2]);
    int i ,j;

    for(i = 0; i < 7 ;i++)
    {
        for(j = 0; j < 70; j++)
        {
            // gotoxy(x + j ,y + i);
            printf("%c[%d;%df ",0x1B, y+i , x+j );
        }
    }
        sem_post(&Shared_Resource[2]);
}


void Photocopy(int counter_number ,int check, int x , int y)
{
    
    sem_wait(&Shared_Resource[0]);
    if (check)
    {
        if(mode == 2)
        {
            sem_wait(&Printing);
            // gotoxy(x ,y);
            printf("%c[%d;%dfAccessing Photocopy machine \n",0x1B,y,x);    
            sem_post(&Printing);
        }
       
        else
        {
            printf("Photocopy machine is being accessed by counter number: %d \n", counter_number);   
        }
        
    }
    Sleep_Wait(2);
    sem_post(&Shared_Resource[0]);

}


void MCM(int counter_number ,int check ,int x ,int y) //money counting machine
{
    
    sem_wait(&Shared_Resource[1]);
    if (check)
    {
        if(mode == 2)
        {   
            sem_wait(&Printing);
            // gotoxy(x ,y);
           printf("%c[%d;%dfAccessing Money counting machine. \n" , 0x1B,y,x);
           sem_post(&Printing);
        }
        else
        {
            printf("Money machine is being accessed by counter number: %d \n", counter_number);
        }
    }
    Sleep_Wait(2);
    sem_post(&Shared_Resource[1]);

}


void Deposit_Withdraw_1(struct Customer_Info *Customer_ID)
{
    
    
    int check ,x ,y ,x1 ,y1;
    char *p;
    sem_wait(&Dealing[0]);
    check = 0;
    x = 0;
    y = 10;
    x1 = x;
    y1 = y;
    if (Customer_ID->Purpose == 'd')
    {
        p = &Deposit_String[0];
    }
    else
    {
        p = &Withdraw_String[0];
    }

    if((mode == 1 && Customer_ID->ID == tr_customer_number) || mode == 3 )
    {

        printf("Customer with ID %d have reached Deposit/Withdraw Counter 1 for the purpose of %s\n" , Customer_ID->ID , p);
        printf("Customer with ID %d %sing the money\n", Customer_ID->ID , p);
        check = 1;   
    }
    else if(mode == 2)
    {
        sem_wait(&Printing);

        if (flag)
        {
            system("clear");
            box_outline ();
            flag = 0;
        }

        //system("clear");
        // gotoxy(x ,y++);
        printf("%c[%d;%df**************COUNTER NUMBER 1 (Deposit/Withdraw)************\n" , 0x1B, y++ , x);
        // gotoxy(x ,y++);
        printf("%c[%d;%dfCustomer with ID %d have reached Counter 1 for the purpose of %s\n" ,0x1B,y++, x, Customer_ID->ID , p);
        // gotoxy(x ,y++);
        printf("%c[%d;%dfCustomer with ID %d %sing the money\n",0x1B, y++ , x, Customer_ID->ID , p);
        check = 1;
        sem_post(&Printing);
    }
    if(Customer_ID->type == 0 || Customer_ID->type == 2)
    {
        if((mode == 1 && Customer_ID->ID == tr_customer_number) || mode == 3 )
        {
            printf("Customer with ID %d doesnt have a copy of CNIC.\n", Customer_ID->ID);
        }
        else if(mode == 2)   
        {
            sem_wait(&Printing);
            // gotoxy(x ,y++);
            printf("%c[%d;%dfCustomer with ID %d doesnt have a copy of CNIC.\n", 0x1B, y++ , x, Customer_ID->ID);
            sem_post(&Printing);
        }
        Photocopy(1 ,check ,x ,y);
        y++;
    }
    if(Customer_ID->type == 1 || Customer_ID->type == 2)
    {
        MCM(1 ,check ,x ,y);
        y++;
    }
    Sleep_Wait(5);
    if((mode == 1 && Customer_ID->ID == tr_customer_number) || mode == 3 )
    {
         printf("Customer with ID %d %sed the money\n", Customer_ID->ID , p);
    }   
    else if(mode == 2)
    {   
        sem_wait(&Printing);
        // gotoxy(x ,y++);
        printf("%c[%d;%dfCustomer with ID %d %sed the money\n",0x1B, y++ , x, Customer_ID->ID , p);
        // gotoxy(x ,y++);
        printf("%c[%d;%dfCustomer with ID %d leaving the bank\n", 0x1B, y++, x, Customer_ID->ID );
        sleep(2);
       // gotoxy(21 + i,53);
		if(Customer_ID->ID < 10)
		{
			printf("%c[%d;%df0%d",0x1B, 53, 21 + count1_x, Customer_ID->ID);
		}
		else
		{
			printf("%c[%d;%df%d",0x1B, 53, 21 + count1_x, Customer_ID->ID);
		}
		count1_x += 3;
        clearing(x1 ,y1 +1);
        // gotoxy(0 ,30);
        printf("%c[%d;%df " , 0x1B, 30, 0);
        sem_post(&Printing);
    }
    
    sem_post(&Dealing[0]);

}


void Deposit_Withdraw_2(struct Customer_Info *Customer_ID)
{
    int check ,x ,y ,x1 ,y1;
    char *p;
    sem_wait(&Dealing[1]);
    check = 0;
    x = 100;
    y = 10;
    x1 = x;
    y1 = y;
    if (Customer_ID->Purpose == 'd')
    {
        p = &Deposit_String[0];
    }
    else
    {
        p = &Withdraw_String[0];
    }

    if((mode == 1 && Customer_ID->ID == tr_customer_number) || mode == 3 )
    {
        printf("Customer with ID %d have reached Deposit/Withdraw Counter 2 for the purpose of %s\n" , Customer_ID->ID , p);
        printf("Customer with ID %d %sing the money\n", Customer_ID->ID , p);
        check = 1;
    }
    else if(mode == 2)
    {

        sem_wait(&Printing);
       // system("clear");
        // gotoxy(x ,y++);
        printf("%c[%d;%df**************COUNTER NUMBER 2 (Deposit/Withdraw)************\n", 0x1B, y++, x);
        // gotoxy(x ,y++);
        printf("%c[%d;%dfCustomer with ID %d have reached Counter 2 for the purpose of %s\n" , 0x1B, y++, x , Customer_ID->ID , p);
        // gotoxy(x ,y++);
        printf("%c[%d;%dfCustomer with ID %d %sing the money\n", 0x1B, y++, x , Customer_ID->ID , p);
        check = 1;
        sem_post(&Printing);
        

    }
     if(Customer_ID->type == 0 || Customer_ID->type == 2)
    {
        if((mode == 1 && Customer_ID->ID == tr_customer_number) || mode == 3)
        {
            printf("Customer with ID %d doesnt have a copy of CNIC.\n", Customer_ID->ID);
        }
        else if(mode == 2)
        {
            sem_wait(&Printing); 
            // gotoxy(x ,y++);
            printf("%c[%d;%dfCustomer with ID %d doesnt have a copy of CNIC.\n", 0x1B, y++, x, Customer_ID->ID);
            sem_post(&Printing);

        }
        Photocopy(2 ,check ,x ,y);
        y++;
    }
    if(Customer_ID->type == 1 || Customer_ID->type == 2)
    {
        MCM(2 ,check ,x ,y);
        y++;
    }
    Sleep_Wait(5);
    if((mode == 1 && Customer_ID->ID == tr_customer_number) || mode == 3 )
    {
        printf("Customer with ID %d %sed the money\n", Customer_ID->ID , p);
    }
    else if(mode == 2)
    {
        sem_wait(&Printing);
        // gotoxy(x ,y++);
        printf("%c[%d;%dfCustomer with ID %d %sed the money\n" , 0x1B, y++, x, Customer_ID->ID , p);
        // gotoxy(x ,y++);
        printf("%c[%d;%dfCustomer with ID %d leaving the bank\n" , 0x1B, y++, x, Customer_ID->ID );
        sleep(2);
        if(Customer_ID->ID < 10)
		{
			printf("%c[%d;%df0%d",0x1B, 54, 21 + count2_x, Customer_ID->ID);
		}
		else
		{
		printf("%c[%d;%df%d",0x1B , 54, 21 + count2_x, Customer_ID->ID);
		}
		count2_x += 3;
        clearing(x1 ,y1 + 1);
        // gotoxy(0 ,30);
        printf("%c[%d;%df", 0x1B, 30, 0);
        sem_post(&Printing);
    }
    
    sem_post(&Dealing[1]);

}



void Billing(struct Customer_Info *Customer_ID)
{
    int check ,x ,y ,x1 ,y1;
    sem_wait(&Dealing[2]);
    check = 0;
    x = 60;
    y = 20;
    x1 = x;
    y1 = y;
    if((mode == 1 && Customer_ID->ID == tr_customer_number) || mode == 3 )
    {
        check = 1;
        printf("Customer with ID %d have reached Billing Counter to pay Bills\n" , Customer_ID->ID);
        printf("Customer with ID %d Paying Bill\n", Customer_ID->ID );
    }
    else if(mode == 2)
    {

        sem_wait(&Printing);
       // system("clear");
        // gotoxy(x ,y++);
        printf("%c[%d;%df**************COUNTER NUMBER 3 (Billing)************\n", 0x1B, y++, x);
        // gotoxy(x ,y++);
        printf("%c[%d;%dfCustomer with ID %d have reached Counter 3 to pay Bills.\n" , 0x1B, y++, x , Customer_ID->ID);
        // gotoxy(x ,y++);
        printf("%c[%d;%dfCustomer with ID %d Paying the Bill\n" , 0x1B, y++, x , Customer_ID->ID );
        check = 1;
        sem_post(&Printing);

    }
     if(Customer_ID->type == 0 || Customer_ID->type == 2)
    {
        if((mode == 1 && Customer_ID->ID == tr_customer_number) || mode == 3)
        {
            printf("Customer with ID %d doesnt have a copy of CNIC.\n", Customer_ID->ID);
        }
        else if(mode == 2)
        {
            sem_wait(&Printing);
            // gotoxy(x ,y++);
            printf("%c[%d;%dfCustomer with ID %d doesnt have a copy of CNIC.\n" , 0x1B, y++, x, Customer_ID->ID);
            sem_post(&Printing);
        }

        Photocopy(3 ,check ,x ,y);
        y++;
    }
    if(Customer_ID->type == 1 || Customer_ID->type == 2)
    {
        MCM(3 ,check ,x ,y);
        y++;
    }
    Sleep_Wait(5);
    if((mode == 1 && Customer_ID->ID == tr_customer_number) || mode == 3 )
    {
        printf("Customer with ID %d Payed the Bill\n", Customer_ID->ID);
    }
    else if(mode == 2)
    {
        sem_wait(&Printing);
        // gotoxy(x ,y++);
        printf("%c[%d;%dfCustomer with ID %d Payed the money\n" , 0x1B, y++, x , Customer_ID->ID );
        // gotoxy(x ,y++);
        printf("%c[%d;%dfCustomer with ID %d leaving the bank\n" , 0x1B, y++, x , Customer_ID->ID );
        sleep(3);
        if(Customer_ID->ID < 10)
		{
			printf("%c[%d;%df0%d", 0x1B, 55, 21 + count3_x, Customer_ID->ID);
		}
		else
		{
			printf("%c[%d;%df%d", 0x1B, 55, 21 + count3_x, Customer_ID->ID);
		}
		count3_x += 3;
        clearing(x1 ,y1 +1);
        // gotoxy(0 ,30);
        printf("%c[%d;%df", 0x1B, 30, 0);
        sem_post(&Printing);
    }
    sem_post(&Dealing[2]);

}



void* Customers(void *Customer_ID)
{
    struct Customer_Info *I = (struct Customer_Info *)Customer_ID;
    Sleep_Wait(rand() % 3);
    if((mode == 1 && I->ID == tr_customer_number) || mode == 3)
        printf("Customer with ID %d Reached the Bank\n" , I->ID);
    int value; 
    sem_getvalue(&Waiting_Room, &value);
    Sleep_Wait(1);
    
    if (value <= 0)
    {
        if((mode == 1 && I->ID == tr_customer_number) || mode == 3)
            printf("Waiting Room is full , Customer with ID %d is waiting outside\n", I->ID);
    }
    
    sem_wait(&Waiting_Room);
    if((mode == 1 && I->ID == tr_customer_number) || mode == 3)
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
    

    
    if((mode == 1 && I->ID == tr_customer_number) || mode == 3)
    { 
        printf("Customer with ID %d leaving the Bank\n", I->ID);
        sleep(2);
        if (mode == 1)
        exit(0);
    }
    sem_post(&Waiting_Room);

}


int main()
{
    srand(time(0)); 
    
    sem_init(&Waiting_Room, 0, Waiting_Chairs);
    
    sem_init(&Dealing[0], 0, 1);
    sem_init(&Dealing[1], 0, 1);
    sem_init(&Dealing[2], 0, 1);

    sem_init(&Shared_Resource[0], 0 ,1);
    sem_init(&Shared_Resource[1], 0 ,1);
    sem_init(&Shared_Resource[2], 0 ,1);

    sem_init(&Printing, 0 ,1);
    

    printf("How many customers for Deposit ? \n");
    scanf("%d" , &Deposit_Customers);

    printf("How many customers for Withdraw ? \n");
    scanf("%d" , &Withdraw_Customers);
    
    printf("How many customers for Billing ? \n");
    scanf("%d" , &Bill_Customers);

    printf("Chose a mode for viewing: \n 1) Track movement of a particular customer. \n 2) Counter view. \n 3)Overview of the whole process.\n");
    scanf("%d" ,&mode);     //value check missing.
    if(mode == 1)
    {
        printf("Enter the customer number you want to track.\n");
        scanf("%d", &tr_customer_number);   //value check missing.
    }


    pthread_t Customers_Threads[Deposit_Customers + Withdraw_Customers + Bill_Customers];
    struct Customer_Info C[Deposit_Customers + Withdraw_Customers + Bill_Customers];

    for (int i = 0; i < Deposit_Customers; i++)
    {
        C[i].ID = i+1;
        C[i].Purpose = 'd';
        C[i].type = rand() % 4;
        //printf("Customer number: %d, customer type: %d \n", C[i].ID, C[i].type);
        pthread_create(&Customers_Threads[i], NULL , Customers , &C[i]);

    }

    for (int i = Deposit_Customers; i < Deposit_Customers + Withdraw_Customers; i++)
    {
        C[i].ID = i+1;
        C[i].Purpose = 'w';
        C[i].type = rand() % 4;
       // printf("Customer number: %d, customer type: %d \n", C[i].ID, C[i].type);
        pthread_create(&Customers_Threads[i], NULL , Customers , &C[i]);
            
    }

    for (int i = Deposit_Customers + Withdraw_Customers; i < Deposit_Customers + Withdraw_Customers + Bill_Customers; i++)
    {
        C[i].ID = i+1;
        C[i].Purpose = 'b';
        C[i].type = rand() % 4;
        //printf("Customer number: %d, customer type: %d \n", C[i].ID, C[i].type);
        pthread_create(&Customers_Threads[i], NULL , Customers , &C[i]);
            
    }


    for (int i = 0; i < Deposit_Customers + Withdraw_Customers + Bill_Customers; i++)
    {
        pthread_join(Customers_Threads[i], NULL);
    }
}
