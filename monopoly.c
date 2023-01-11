/*
Description: <This program essentially is a  text based software version of a game called Property ... Property. This is a turn-based two-player
board game similar to that of Monopoly.Players compete to acquire wealth by buying or renting properties. The game ends when a player goes bankrupt. This occurs when
the player is not able to pay rent becuase they have no money or they do not have enough cash in hand to pay the fine
>
Programmed by: <Naman> <S-15A>
Last modified: <07/02/2022>
Version: <2>
[Acknowledgements: < https://www.techonthenet.com/c_language/standard_library_functions/math_h/floor.php> 
                   <https://www.javatpoint.com/count-the-number-of-digits-in-c>
                   <https://www.javatpoint.com/count-the-number-of-digits-in-c>
*/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

void Menu();
void Start();
int setPosition(int);
int getPropertyStatus(int, long int);
void displayPropertyName(int);
char wantToRenovate(int);
int CostToBuyProperty(int);
int costOfHouseProperty(int);
int calculateRent(int, int, int);
int resaleCostOfProperty(int);
int is_prime(int);
long int updateBoard(int, int, long int);
int countdigits(int);
int checkBoard(int, long int);
int main()
{
    int choice;
	int nFlag = 0;
    do
    {
        //displaying a menu option
        Menu();
        printf("Enter your choice: ");
        scanf("%d", &choice);           //taking user's choice
        switch(choice)
        {
            case 1: Start();
			         break;	//if choice is 1, invoking a function to play the game
			case 2: nFlag = 1;
					break;
			default: printf("Invalid option");
					break;
        }
    }while(nFlag != 1);                //as long as choice is 1, program will play the game

    return 0;
}

/*
This function will display the menu on screen
*/
void Menu()
{
    printf("\n--------------------------------------------------------------------------------------");
    printf("\n1. Start a new game");
    printf("\n2. Exit the program");
    printf("\n--------------------------------------------------------------------------------------");
    printf("\n");
}

/*
this function will start playing the game
*/
void Start()
{
    long int currentStatus = 000000000;
    int p1CashInHand = 200, p2CashInHand = 200;
    int pla1Pos = 0, pla2Pos = 0;
    int diceValue, turn = 1;
    int jailFlag1 = 0, jailFlag2 = 0;
    int costOfRenovation = 50;
    
    srand(time(0));             //feed the time to null, so rand function will generate new values
    do
    {
        int currentPosition;
        diceValue = rand() %6 + 1;           //selecting a random number between 1 to 6
        printf("Turn = %d\t dice rolled to = %d\n", turn, diceValue);
        if(turn == 1)
        {
            pla1Pos += diceValue;           //setting the position on the board based on dice rolled value
            if( pla1Pos > 9)                // if it gone over 9,
            {
                //that means it will land on 0 or passes by, add 200 bucks to player1 account
                p1CashInHand += 200 ;
                pla1Pos = setPosition(pla1Pos);   //then start it from 0 again
            }
            currentPosition = pla1Pos;            //set it to the current position
        }
        
        
        else if(turn == 2)
        {
            pla2Pos += diceValue;
            if( pla2Pos > 9)
            {
                //that means it will land on 0 or passes by, add 200 bucks to player2 account
                p2CashInHand += 200 ;
                pla2Pos = setPosition(pla2Pos);     //then start it from 0 again
            }
            currentPosition = pla2Pos;              //set it to the current position
        }

        printf("Currentposition On Board = %d\n", currentPosition);

        //if player lands on a number other than 0, 4, 6
        if(currentPosition != 4 && currentPosition != 6 && currentPosition != 0)
            {
                //get the status of that particular property
                int propertyStatus = getPropertyStatus(currentPosition, currentStatus);
                char choice;

                //passing that status to switch to figure out our next task
                switch(propertyStatus)
                {
                    //if status is 0, that means property is owned by bank
                    case 0 : printf("Do you want to buy ");
                             displayPropertyName(currentPosition);      //displaying the property name
                             printf("[B]Buy / [N]Do Nothing? : ");
                             do
                             {
                                scanf(" %c", &choice);
                                if(choice != 'B' && choice != 'N')
                                    printf("Invalid input! Please try again\n");
                             }while(choice != 'B' && choice != 'N');

                             if(choice == 'B')
                             {
                                 //finds oout the buying cost of that particular property
                                 int cost = CostToBuyProperty(currentPosition);
                                 //check if player has enough cash
                                 if(turn == 1 && p1CashInHand > cost)
                                 {
                                    //if so, deduct the cost of the property from player's cash in hand
                                    p1CashInHand -= cost;
                                    //and update the 9 digit number
                                    currentStatus = updateBoard(currentPosition, 1, currentStatus);
                                 }
                                 else if(turn == 2 && p2CashInHand > cost)
                                 {
                                    //if so, deduct the cost of the property from player's cash in hand
                                    p2CashInHand -= cost;
                                    //and update the 9 digit number
                                    currentStatus = updateBoard(currentPosition, 2, currentStatus);
                               }
                                 else if (turn == 1 && p1CashInHand < cost)
                                  {
                                   printf("\n Can't buy the property because of Insufficient Cash! \n");
                                  }
                                  else if (turn == 2 && p2CashInHand < cost)   
                                   {
                                   printf("\n Can't buy the property because of Insufficient Cash! \n");
                                   }
                             }
                             break;

                    //if the status is 1, that means property is owned by player 1
                    //so, if it's player1's turn, then he can renovate or do nothing
                    //or if its player 2's turn, then he has to pay the rent to player1
                    case 1 : if(turn == 1)
                                {
                                    //checking if it's not a electric company or rail road, because player
                                    //can't renovate them.
                                    if(currentPosition != 2 && currentPosition != 7)
                                    {
                                         choice = wantToRenovate(currentPosition);
                                         if(choice == 'R')
                                         {
                                             //check if player has enough cash to renovate the property
                                             if(p1CashInHand > costOfRenovation)
                                             //if so, deduct the renovation cost from player's cash in hand
                                             {
                                                 p1CashInHand -= costOfRenovation;
                                                 //and update the 9 digit number
                                                 currentStatus = updateBoard(currentPosition, 3, currentStatus);
                                             }
                                              else{ 
											  printf(" Insuffecient Cash for the Rennovation!\n");
								           }
                                         }
                                    }
                                }
                                else //if turn == 2
                                {
                                    int rent = calculateRent(currentPosition, diceValue, propertyStatus);
                                    //player2 has to pay the rent to player 1
                                    //if player2 doesn't have enough cash to pay the rent, then
                                    if(p2CashInHand < rent)
                                    {
                                        //player2 has to sell one of it's property
                                         int flag = 0;
                                        do
                                        {
                                            printf("Player2 has to sell one of it's property\n");
                                            //getting the property position owned by player2
                                            int propertyPosToSell = checkBoard(2, currentStatus);
                                            //if function returns 0, means player 2 owns no property,
                                            //so, he has nothing to sell and goes bankrupt
                                            if(propertyPosToSell == 0)
                                            {
                                                flag = 1;
                                                p2CashInHand = 0;
                                            }
                                            else
                                            {
                                                printf("Property %d is going to be sold\n", propertyPosToSell);
                                                //finding out the resale value of the property
                                                int resaleValue = resaleCostOfProperty(propertyPosToSell);
                                                //add resale value to player's account
                                                p2CashInHand += resaleValue;
                                                //now the property owned by the bank, so update the board
                                                currentStatus = updateBoard(propertyPosToSell, 0, currentStatus);
                                                if(p2CashInHand > rent)
                                                {
                                                    //if he has enough cash to pay the rent, change the flag
                                                    flag = 1;
                                                }
                                            }
                                        }while(flag == 0);
                                    }
                                    //deduct the rent from player2 cash in hand
                                    p2CashInHand -= rent;
                                    //then player1 cash in hand gets added by the rent paid
                                    p1CashInHand += rent;
                                    printf("Player2 paid %d rent to Player1\n", rent);
                                }
                                break;

                    //if the status is 2, that means property is owned by player 2
                    //so, if its player 1's turn, then he has to pay the rent to player1
                    //or, if it's player2's turn, then he can renovate or do nothing
                    case 2:     if(turn == 1)
                                {
                                    int rent = calculateRent(currentPosition, diceValue, propertyStatus);
                                    //player1 has to pay the rent to player 2
                                    //if player1 doesn't have enough cash to pay the rent, then
                                    if(p1CashInHand < rent)
                                    {
                                        //player1 has to sell one of it's property
                                        int flag = 0;
                                        do
                                        {
                                            printf("Player1 has to sell one of it's property\n");
                                            //getting the property position owned by player1
                                            int propertyPosToSell = checkBoard(1, currentStatus);
                                            //if function returns 0, means player 1 owns no property,
                                            //so, he has nothing to sell and goes bankrupt
                                            if(propertyPosToSell == 0)
                                            {
                                                flag = 1;
                                                p1CashInHand = 0;
                                          
											}
                                            else
                                            {
                                                printf("Property %d is going to be sold\n", propertyPosToSell);
                                                int resaleValue = resaleCostOfProperty(propertyPosToSell);
                                                //add resale value to player's account
                                                p1CashInHand += resaleValue;
                                                //now the property owned by the bank, so update the board
                                                currentStatus = updateBoard(propertyPosToSell, 0, currentStatus);
                                                if(p1CashInHand > rent)
                                                {
                                                    flag = 1;
                                                }
                                            }
                                        }while(flag == 0);
                                    }
                                    //deduct the rent from player2 cash in hand
                                    p1CashInHand -= rent;
                                    //then player1 cash in hand gets added by the rent paid
                                    p2CashInHand += rent;
                                    printf("Player1 paid %d rent to Player2\n", rent);
                                }
                                else //if turn == 2
                                {
                                    //checking if it's not a electric company or rail road, because player
                                    //can't renovate them.
                                    if(currentPosition != 2 && currentPosition != 7)
                                    {
                                        choice = wantToRenovate(currentPosition);
                                         if(choice == 'R')
                                         {
                                             //check if player has enough cash to renovate the property
                                             if(p2CashInHand > costOfRenovation)
                                             //if so, deduct the renovation cost from player's cash in hand
                                             {
                                                 p2CashInHand -= costOfRenovation;
                                                 //and update the 9 digit number
                                                 currentStatus = updateBoard(currentPosition, 4, currentStatus);
                                             }
                                              else
                                            {
                                            	 printf(" Insuffecient Cash for the Rennovation!\n");
											}
                                         }
                                    }
                                }
                                break;
                    case 3 :    //if turn == 1, do nothing
                                if(turn == 2)
                                {
                                    //calculate the rent of a renovated property
                                    int rent = calculateRent(currentPosition, diceValue, propertyStatus);
                                    //player2 has to pay the rent to player 1
                                    //if player2 doesn't have enough cash to pay the rent, then
                                    if(p2CashInHand < rent)
                                    {
                                        //player2 has to sell one of it's property

                                        int flag = 0;
                                        do
                                        {
                                            printf("Player2 has to sell one of it's property\n");
                                             //getting the property position owned by player2
                                            int propertyPosToSell = checkBoard(2, currentStatus);
                                            //if function returns 0, means player 2 owns no property,
                                            //so, he has nothing to sell and goes bankrupt
                                            if(propertyPosToSell == 0)
                                            {
                                                flag = 1;
                                                p2CashInHand = 0;
                                            }
                                            else
                                            {
                                                printf("Property %d is going to be sold\n", propertyPosToSell);
                                                int resaleValue = resaleCostOfProperty(propertyPosToSell);
                                                //add resale value to player's account
                                                p2CashInHand += resaleValue;
                                                //now the property owned by the bank, so update the board
                                                currentStatus = updateBoard(propertyPosToSell, 0, currentStatus);
                                                if(p2CashInHand > rent)
                                                {
                                                    flag = 1;
                                                }
                                            }
                                        }while(flag == 0);
                                    }
                                    //deduct the rent from player2 cash in hand
                                    p2CashInHand -= rent;
                                    //then player1 cash in hand gets added by the rent paid
                                    p1CashInHand += rent;
                                    printf("Player2 paid %d renovated rent to Player1\n", rent);
                                }
                                break;
                    case 4 :    if(turn == 1)
                                {
                                    //calculate the rent of a renovated property
                                    int rent = calculateRent(currentPosition, diceValue, propertyStatus);
                                    //player1 has to pay the rent for a renovated property to player 2
                                    if(p1CashInHand < rent)
                                    {
                                        //player1 has to sell one of it's property
                                        //if player1 doesn't have enough cash to pay the rent, then
                                        int flag = 0;
                                        do
                                        {
                                            printf("Player1 has to sell one of it's property\n");
                                            //getting the property position owned by player1
                                            int propertyPosToSell = checkBoard(1, currentStatus);
                                            //if function returns 0, means player 1 owns no property,
                                            //so, he has nothing to sell and goes bankrupt
                                            if(propertyPosToSell == 0)
                                            {
                                                flag = 1;
                                                p1CashInHand = 0;
                                            }
                                            else
                                            {
                                                printf("Property %d is going to be sold\n", propertyPosToSell);
                                                int resaleValue = resaleCostOfProperty(propertyPosToSell);
                                                //add resale value to player's account
                                                p1CashInHand += resaleValue;
                                                //now the property owned by the bank, so update the board
                                                currentStatus = updateBoard(propertyPosToSell, 0, currentStatus);
                                                if(p1CashInHand > rent)
                                                {
                                                    flag = 1;
                                                }
                                            }
                                        }while(flag == 0);
                                    }
                                    //deduct the rent from player2 cash in hand
                                    p1CashInHand -= rent;
                                    //then player1 cash in hand gets added by the rent paid
                                    p2CashInHand += rent;
                                    printf("Player1 paid %d renovated rent to Player2\n", rent);
                                }
                                //if turn == 2, do nothing
                                break;
                }
            }
            //if it lands on feeling lucky scenario
            else if (currentPosition == 6)
            {
                //rolling the dice again, randomly generate one more value between 1 to 6
                int dice = rand()%6 + 1;
                printf("Feeling lucky dice value = %d\n", dice);
                //if it is 1, then jail theme for the respected player
                if(dice == 1)
                {
                    if(turn == 1)
                        jailFlag1 = 1;
                    else
                        jailFlag2 = 1;
                    printf("Jail time for Player %d\n", turn);
                }
                //if not 1,
                else
                {
                    //then check if it's prime or not
                    int isPrime = is_prime(dice);
                    //if it's prime, then
                    if( isPrime == 1)
                    {
                        //randomly generates a bonus value between 100 to 200
                        int bonus = rand() % 100 + 100;
                        //add it to respected player's account
                        if(turn == 1)
                            p1CashInHand += bonus;
                        else
                            p2CashInHand += bonus;
                        printf("Player %d received bonus of %d\n", turn, bonus);
                    }
                    //if it's not prime then
                    else
                    {
                        //randomly generates a value (to pay back) between 50 to 150
                        int payBack = rand() % 100 + 50;
                        //deduct it from respected player's account
                        if(turn == 1)
                            p1CashInHand -= payBack;
                        else
                            p2CashInHand -= payBack;
                        printf("Player %d has paid fine of %d\n", turn, payBack);
                    }
                }
            }
            //if it lands on position 4, i.e. jail theme
            else if(currentPosition == 4)
            {
                //jail theme scenario
                if(turn == 1)
                    jailFlag1 = 1;
                else
                    jailFlag2 = 1;
                printf("Jail time for Player %d\n", turn);
            }
      
        //if player1 is in jail, skip his next turn
        if(turn == 1 && jailFlag1 == 1)
        {
            turn = 2;
            jailFlag1 = 0;
        }

        //if player2 is in jail, skip his next turn
        else if(turn == 2 && jailFlag2 == 1)
        {
            turn = 1;
            jailFlag2 = 0;
        }


  //change the player's turn, if 1 then make it 2, if 2 then make it 1
        if(turn == 1)
            turn = 2;
        else
            turn = 1;

        printf("Board status = %d\n", currentStatus);
        printf("Player1 cash = %d\t Player2 cash = %d\n\n\n", p1CashInHand, p2CashInHand);
    }while(p1CashInHand > 0 &&  p2CashInHand > 0);

    if(p1CashInHand <= 0){
    	  printf("Player 1 goes bankrupt.\n");
    	  printf(" Player 2 Wins !!!!! ");
	}
    else if(p2CashInHand <= 0)
    {
    	printf("Player 2 goes bankrupt.\n");
    	printf(" Player 1 Wins !!!!! ");
	}
}

/*
This function will set the player position in 0-9
@param pos: a value which is the addition of current position of the player on the board and dice rolled value
@return : returning the position in a number between 0 to 9
*/
int setPosition(int pos)
{
    //if it is over 9, start it from 0 again
    if(pos > 9)
    {
        pos = pos - 10; //subtracting 10 because, total positions on board are 10
    }
    return pos;
}

/*
This function will return the current status of property
@param pos: position on the board
@param currentStatus: 9-digit number which represents the current status of all properties
@return: current status of the particular property
*/
int getPropertyStatus(int pos, long int currentStatus)
{
    int rem;
    int cnt = 0;
    //getting the remainder of the position on the board
    while(cnt < pos)
    {
        rem = currentStatus % 10;   //get the remainder
        currentStatus /= 10;        //change the priginal number
        cnt++;
    }
    return rem;                     //return the remainder value of that position
}

/*
This function will display the property name on screen
@param pos: current position on the board
*/
void displayPropertyName(int pos)
{
    switch(pos)
    {
        case 1 : printf(" Tree house?\n"); 
		               break;
        case 2 : printf(" Electric Company?\n"); 
		                break;
        case 3 : printf(" Beach house?\n"); 
		               break;
        case 5 : printf(" Castle?\n");
		                 break;
        case 7 : printf(" Rail Road?\n"); 
		                break;
        case 8 : printf(" Igloo?\n");  
		                 break;
        case 9 : printf(" Farm house?\n");   
		                break;
    }
}

/*
This function will get the user's choice of whether he/she wants to renovate the property
@param currentPosition: current position on the board
@return : user's choice in Yes or No
*/
char wantToRenovate(int currentPosition)
{
    char choice;
    printf("Do you want to renovate the property ");
    displayPropertyName(currentPosition);
    printf("[R]Renovate / [N]Do Nothing ? : ");
    do
    {
        scanf(" %c", &choice);
        if(choice != 'R' && choice != 'N')
            printf("Invalid input! Please try again\n");
    }while(choice != 'R' && choice != 'N');
    return choice;
}

/*
This function will calculate the cost needed to buy a particular property
@param pos: current position on the board
@return : the cost of the property
*/
int CostToBuyProperty(int pos)
{
    int cost = 0;
    switch(pos)
    {
        case 1 : cost = costOfHouseProperty(pos);
		 break;
        case 2 : cost = 150; 
		                 break;                     //cost of the electric company
        case 3 : cost = costOfHouseProperty(pos); 
		                 break;
        case 5 : cost = costOfHouseProperty(pos);
		                 break;
        case 7 : cost = 100;
		                break;                     //cost of the rail road
        case 8 : cost = costOfHouseProperty(pos); 
		                 break;
        case 9 : cost = costOfHouseProperty(pos); 
		                 break;
    }
    return cost;
}
/*
This function will calculate the cost needed to buy a particular house property
@param pos: current position on the board
@return : the cost of the house property
*/
int costOfHouseProperty(int pos)
{
    return 20 * (pos % 7);          //cost is 20 times the remainder of the position when divided by 7
}

/*
This function will calculate the rent of a particular property
@param pos: current position on the board
@param diceValue: current rolled dice value
@param status: status of the property weather it is 1 or 2 or 3 or 4
@return : the rent of the property
*/
int calculateRent(int pos, int diceValue, int status)
{
    int rent = 0;
    switch(pos)
    {
        case 2 : rent = 8 * diceValue; 
		                   break;       //rent of electric company, 8 times the dice rolled value
        case 7 : rent = 35;
		            break;                  //rent of rail road is fixed 35bucks
        case 1 :
        case 3 :
        case 5 :
        case 8 :
        case 9 : if(status == 1 || status == 2)         //rent of a basic house property
                    {
                        rent = (int) (costOfHouseProperty(pos) / 5);
                    }
                 else if ( status == 3 || status == 4)  //rent of a renovated property
                    {
                        rent = 1 + ( 2 * (int)( ( costOfHouseProperty(pos) / 5)));
                    }
                break;
    }
    printf("Rent = %d\n", rent);
    return rent;
}

/*
This function will calculate the resale cost of a particular property
@param pos: current position on the board
@return : the resale value of the property
*/
int resaleCostOfProperty(int pos)
{
    int cost = 0;
    cost = CostToBuyProperty(pos);
    return cost / 2;
}

/*
This function will find out whether the input number is prime or not
@param no: a number which is dice rolled value in feeling lucky scenario
@return : 1 if prime, 0 if not prime
*/
int is_prime(int no)
{
    int rem, div = 2;
    int flag;
    //start dividing the number by 2 till number-1 to check if it divisible
    if(no == 1)     // if no is 1, it is not prime, so setting flag to 0
        flag = 0;
    else
        flag = 1;

    while(div < no)
    {
        if(no % div == 0)
        {
            flag = 0;       //if so, change the flag value
        }
        div++;              //increment the divisor
    }
    return flag;
}

/*This function will update the 9-digit number which represents the status of all properties
@param pos: current position on the board
@param value: new status of a particular property
@param board: 9-digit number which represents the status of all properties
@return : updated 9-digit number in form of variable 
*/

long int updateBoard (int postion, int value, long int board)
{
    long int boardNumber = board;
    int numAtGivenPosition = 0, finalPosition;
    int length = countdigits(boardNumber); // extracts the number of digits from the board
    if (length >= postion)
    {
        for(int i =1; i <= postion; i++) // for loop till the  position of the proeprty 
        {
                 numAtGivenPosition = boardNumber % 10;
                 boardNumber = boardNumber / 10;
        }
        int valueToRemove = board - (floor(numAtGivenPosition) * pow(10, (postion-1))); // using pow function and the floor function this removes the initial number from the respective position
        finalPosition = valueToRemove + (value * pow(10,(postion-1))); //  adds the respective number
    }
    else // if length is less than the posotion
    {
         finalPosition = boardNumber + (value * pow(10,(postion-1)));
    }
    return finalPosition;
}

/*This function will find out the number of digits for a varaible in the updateBoard function  in order for the update 9 digit 
@param num: can be any number 
@return : number of digits of the number in the form of variable (counter) 
*/
int countdigits(int n)
{
    int counter=0; // variable declaration
    while(n!=0)
    {
        n=n/10;
        counter++;
    }
    return counter;
}
/*
This function will find out the property position which is owned by a particular player in order for the user to sell if the rent of a particular property is
 greater  than the hands in cash. 
@param player: a player 1 or a player 2
@param board: 9-digit number
@return : position of the property which is owned by the inputted player number 
*/
int checkBoard(int player, long int board)
{
{
    int pos = 0, rem, cnt = 1;
    printf("Which property you want to sell, Please enter the position of the property\n");
    while(board > 0)
    {
        rem = board % 10;
        board = board / 10;
        if(rem == player || rem == player + 2)
        //if player is 1, then 1 and 3 are owned by player1
       //if player is 2, then 2 and 4 are owned by player2
        {
            pos = cnt;
           printf("%d ", pos);
            displayPropertyName(pos);
        }
        cnt++;
    }
    scanf("%d", &pos);
    return pos;
}
}
