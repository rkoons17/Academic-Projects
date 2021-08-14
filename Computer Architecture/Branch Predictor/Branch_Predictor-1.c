//Gshare Branch Predictor--RKK
//Computer Architecture Project 2--2020
//Description: 

//NOTE: COMPILATION INSTRUCTIONS
//"gcc Branch_Predictor.c -o gShare -lm"
//NOTE: -lm IS NECESSARY SINCE math.h IS USED
//I/P to command line: ./gShare <GPB> <RB> <TRACEFILE>
//I/p Parameters: 
    //(M) GPB: # of bits used to index the gshare table
    //(N) RB: Global History Register Bits
    //File for grabbing address, and simulating accesses

#include <stdio.h>
#include <stdlib.h> //necessary for atoi function
#include <string.h> //necessary for separating/parsing string (each line of tracefile)
#include <math.h>   //necessary for log_2 function, need -lm in gcc compile
#include <malloc.h> //necessary for creating string of padded zeroes
#include <stdbool.h> //bool for dirty bit array

double log_2( double x, double base ); //function that returns log base 2 of input
char* pad_zeroes( char* address);      //functon that pads "hex" string to have appropriate 64-bit size


//main function--can read parameters from command line
int main( int argc, char *argv[])
{
    //test case for input parameters, should have argc=6 (including program name)
    if (argc != 4){
    printf( "Error: Invalid number of parameters\n\n");
    exit(1); //safely exit program
    }

//--------------Variable Initialization----------------------------//
    //Capture all input parameters from the command line for configuring BP (Branch Predictor)
     int M = atoi(argv[1]);  //number of bits to index gShare Table
     int N = atoi(argv[2]);  //number of bits for GHR
    long long int tableSize= pow(2, M);  //size of gShare Array/Table
    int gShare[tableSize];
    long long int mispredict=0;
    long long  int totalpredict=0;
    int shift_result=0; //shift a 1 to the right of the GHB, since it was taken
    long long int counter=0;
//init all indices of array to '2'--weakly taken
    long long int i;
for (i = 0; i < tableSize; i++)
{
  gShare[i] = 2;
 // printf("%d\n", gShare[i]);
}
 
    //printf("init: %d\n" ,gShare[tableSize-1]);
     char line[1000]; //buffer to read in each line of the file
     int GHB =0; //initialize GHB to 0
     char space= ' '; //for parsing out operation
     char * outcome;  //outcome for instruction
    
    printf("%i %d ", M, N);
           

//--------------Begin Reading TraceFile----------------------------//
    FILE *tracefile = fopen(argv[3], "r"); // read tracefile from command line
    if(tracefile == 0) //error case
    {
        printf("Could Not Open File\n");
        exit(1); //safely exit program
    }

    //executes for each line of code
    while( fgets (line, sizeof(line), tracefile))
    {
        if(line[0] == '\n') continue;  //skip empty lines

        //grab address
        //grab operation

        //char op=line[0];//grab operation from first character of each line
        char * end; //ptr for strtoull function (stdlib.h)
        
        
            outcome = strrchr(line, space );
            outcome++;  //incrmement pointer to grab outcome char
            //printf( "outcome: %s\n", outcome);
            //printf("LINE #: %llu\n", counter+1);
            counter++;
             //printf( "outcome: %c\n", outcome[0]);  //outcome[0] is the outcome of the instruction
            

            char *temp;
            temp = strchr(line,' ');   //Get the pointer to char token
            *temp = '\0';             //Replace token with null char

           // printf("Current Address: %s\n", line);

        //convert 64 bit hex string to long long int
        long long int address= strtoull( line, &end , 16 );
        //printf("address (lli): %lli\n", address);

       

//--------------Remove Bute Offset from Index----------------------------//
        
        //shift address 2 bits to the right
        address = address >> 2; //get rid of byte offset 
       // printf(" \n\nNUMBER %lld\n", n);
         // printf("shifted address (lli): %lli\n", address);


          char* oneStr = malloc(M + 1);//allocate for string of 1s (binary) equivalent to number of set bits, plus NULL
        memset( oneStr, '1', M); //fill new string with 1s for padding
        oneStr[M] = '\0'; //string of ones, terminated with null character
          //  printf("oneStr: %s\n", oneStr);

//convert binary string to integer--use AND bitwise operation
char* ptr; //ptr necessary for strtol function
long int binaryString = strtol(oneStr, &ptr, 2); //create binary string
       // printf("Binary string: %li\n", binaryString);

unsigned long long int M_index= address & binaryString; //take rightmost bits equivalent to number of set bits
//printf("Current line #: %lli\n", totalpredict+1);
       // printf("M_index: %llu\n", M_index);

   // printf("Current GHB: %d\n", GHB);
    int tempGHB= GHB<<(M-N);
   // printf("first temp GHB shift: %d\n", tempGHB);

    long long int entry_Index = tempGHB ^ M_index;
    //printf("Entry Index: %llu\n",entry_Index);

  // printf("Initial Entry value: %i\n", gShare[entry_Index]);


//--------------Prediction Cases---------------------//
    //prediction time
    //predict taken, acutally taken
    if(outcome[0] =='t' && gShare[entry_Index] >= 2){
       //printf("Predict taken, actually taken\n");
        totalpredict++;
        if(gShare[entry_Index] < 3 )
         gShare[entry_Index]++;
       

    shift_result=1;
    }

    //predict taken, actually not taken, decrement index
     else if(outcome[0]=='n' && gShare[entry_Index] >=2){
        //printf("Predict taken, actually not taken\n");
        totalpredict++;
        //printf("here!\n");
        mispredict++;  //increment counter
        if(gShare[entry_Index] > 0 )
        gShare[entry_Index]--;
        

    shift_result=0;
    }

    //predict not taken, actually taken, increment index
     else if(outcome[0]=='t' && gShare[entry_Index] <2){
        totalpredict++;
        mispredict++;  //increment counter
       // printf("TEST: outcome: %c Index value %i")
        //printf("Predict not taken, actually taken\n");
       if(gShare[entry_Index] < 3 )
         gShare[entry_Index]++;

    shift_result=1;
    }

    //predict not taken, actually not  taken, decrement index
     else {    //(outcome[0]='n' && gShare[entry_Index] <2){
        totalpredict++;
      // printf("Predict not taken, actually not taken\n");
          if(gShare[entry_Index] > 0 )
        gShare[entry_Index]--;

    shift_result=0;
    }

//
    // printf("Current Index: %d\n", gShare[entry_Index]);
    // printf("shift result: %i\n", shift_result);


     //update GHB, shift it right one, and with string of 1s
    // printf("GHB test: %d\n", GHB);
     
     //printf("GHB Test 2: %d", GHB);

      if( N!=0)
      {

      
        if(shift_result==0)
        {
             GHB=GHB>>1;
        }
       else if(shift_result==1)
        {
            
     char* zeroStr2 = malloc(N+1 );//allocate for string of 0s (binary) equivalent to number of GHB bits, plus NULL
        memset( zeroStr2, '0', N); //fill new string with 1s for padding
            zeroStr2[0]='1';
           // printf("modified GHB string: %s\n", zeroStr2);
             char* ptr2; //ptr necessary for strtol function
long int binaryGHB = strtol(zeroStr2, &ptr2, 2); //create binary string

////printf("Binary GHB: %li\n", binaryGHB);
//printf("GHB: %i\n", GHB);
    GHB=GHB>>1;
    GHB= GHB | binaryGHB;
        }
      }
        
//create string of 1s to get correct GHB size
   // oneStr2 = strncat( result, oneStr2, 100 ); //64 is max number of characters to be appended

   



   // printf("Final GHB: %i\n", GHB);
   // printf("Mispredict Count: %lli\n\n", mispredict);
    //  float ratio=(float)mispredict/totalpredict;
    //printf("Mispredict ratio: %.2f\n\n", ratio);





   







    }


    fclose(tracefile);  //close file
    float ratio=(float)mispredict/totalpredict;
    
//printf("MISS: %llu\n", mispredict);
//printf("TOTAL: %llu\n", totalpredict);
    printf(" %.4f\n", ratio);


//--------------Print Statistics----------------------------//







return 0; //end main function
}



//need string to be 16 characters long (for 64 bit address and bit shift operations)
char* pad_zeroes( char* address )
{   //strlen doessn't count null character
    //assume its passed address= 7fff6c5b7b80 12 characters, need 16 characters, excluding null
    int strLength = 16-strlen(address);
    char* padStr = malloc(strLength+1);//allocate for 16 hex characters present, + NULL
    memset( padStr, '0', strLength); //fill new string with 0s for padding
    padStr[strLength] = '\0'; //string of zeroes, terminated with null character

    //append string of 0s in front of address
    address = strncat( padStr, address, 64 ); //64 is max number of characters to be appended
    return padStr; //return padded string
}


//function necessary for computing number of bits to index set
double log_2( double x, double base )
{
return log(x)/log(base); //compute log base 2 of input
}