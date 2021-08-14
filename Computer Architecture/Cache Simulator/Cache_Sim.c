//Cache Simulator--RKK
//Computer Architecture Project 1--2020
//Desription: Level One Cache Simulator using C-programming Language. A tracefile is  parsed through line by line and each R/W instruction is simulated by the cache simulator.
//Each Hex address undergoes string manipulation and is converted to a long long int in order to be able to easily write the rest of the code. Statistics are tracked
//throughout the program-- most importantly o/p: Miss Ratio, # Writes to Memory, # Reads from Memory. The simulator is capable of Write-Back/Trough policy as well as LRU/FIFO policy.

//NOTE: COMPILATION INSTRUCTIONS
//"gcc cache_sim.c -0 SIM -lm"
//"./SIM <Cache_Size> <Associativity> < Replacement Policy> <Writing Policy> <TraceFile>"
//NOTE: -lm IS NECESSARY SINCE math.h IS USED
//I/P to command line: ./SIM <CACHE_SIZE> <ASSOC> <REPLACEMENT> <WB> <TRACE_FILE>
//I/p Parameters: 
    //0---LRU, 1--FIFO
    //0-----Write through, 1-----Write Back

//{---------Tag Bits----------}{-----------Set Number---------}{----------Block Offset---------}

//Note: 64B Block Size is selected for all configurations

//For testing with example from class:
    //block size=16
    //strlen--6 characters (no padding)
    //block offset--4 bits

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
    if (argc != 6){
    printf( "Error: Invalid number of parameters\n\n");
    exit(1); //safely exit program
    }

//--------------Variable Initialization----------------------------//
    //Capture all input parameters from the command line for configuring cache
    long long int cache_size = atoi(argv[1]);
    int assoc = atoi(argv[2]);
    int replace = atoi(argv[3]); //0---LRU, 1--FIFO
    bool wb = atoi(argv[4]); //0-----Write through, 1-----Write Back
    char line[100]; //buffer to read in each line of the file
    char x = 'x';// used to search for x character before address in each line
    int block_size=64;    //cache size is usually 64B (fixed) for examples

         //printf("Cache Size: %lli\n", cache_size);
         //printf("Associativity: %i\n",assoc);
         //printf("Replacement Policy: %i\n", replace);
         //printf("Writing Policy: %i\n")
    
    
    double set_num = cache_size/(block_size*assoc); //define number of sets--eqn. from class
    int set_bits = (int)log_2( set_num, 2); //find number of bits to index set
         //printf("set num %lf", set_num);
         //printf("set_bitst: %d\n", set_bits);

    long long int tag_array[(int)set_num][assoc]; //tag array
    long long int replace_position[(int)set_num][assoc]; //replacement policy condition array, holds counter values for LRU/FIFO comparison, same array is used for both policies
    bool validity[(int)set_num][assoc]; //check to see if block has been used; 0---invalid (unused), 1--valid (used)
    bool dirty[(int)set_num][assoc];  //dirty bit array, 1---Dirty bit, must write back (WB Policy only)
    
    //initialize all arrays to 0's
    memset( tag_array, 0, sizeof(tag_array));
     memset( replace_position, 0, sizeof(replace_position));
      memset( validity, 0, sizeof(validity));
       memset( dirty, 0, sizeof(dirty));

    long long int read_hit_count=0;
    long long int read_miss_count=0;
    long long int replace_count=0; //counter for keeping track of LRU/FIFO POlicy Evictions (same counter is used for both eviction policies)
    long long int write_hit_count=0;
    long long int write_miss_count=0;
    long long int mem_write_count=0; //counter for writes to memory (changes depending on writing policy)
    long  long int mem_read_count=0; //counter for reads from memory
    int unused_set_flag=0; //flag for a filled up set; 1----set is not filled, 0---set is filled, must evict a block
    int hit_flag=0;  //flag for whether or not R/W instruction is a hit; 1---hit, 0---miss
        
        //long long int read_count=0; //counter for total reads (instructions) of program
        //long long int write_count=0; //counter for total writes (instructions) of program
        //long long int evict_count=0;     //counter for evictions

//--------------Begin Reading TraceFile----------------------------//
    FILE *tracefile = fopen(argv[5], "r"); // read tracefile from command line
    if(tracefile == 0) //error case
    {
        printf("Could Not Open File\n");
        exit(1); //safely exit program
    }

    //executes for each line of code
    while( fgets (line, sizeof(line), tracefile))
    {
        if(line[0] == '\n') continue;

        char op=line[0];//grab operation from first character of each line
        char* strAddress, *end; //ptr for strtoull function (stdlib.h)
        //remove '0x' from string
            strAddress = strrchr(line, x );
            strAddress++; //increments the pointer, to remove 'x' char

        if( strlen(strAddress) < 16  )//if not 64-bit address, pad with zeroes   
        {
            strAddress= pad_zeroes(strAddress); //pad string with zeroes
        }

                //printf("String Address: %s\n", strAddress);

        //convert 64 bit hex string to long long int
        long long int address= strtoull( strAddress, &end , 16 );
        //printf("address (lli): %lli\n", address);

       

//--------------Remove Block Offset----------------------------//
        //assume 64 byte  block size for all configurations
        // block offset = log_2 (64) = 6 bits;
        //shift address 6 bits to the right
        address = address >> 6; //get rid of block offset 


//-------------Extract Set Number------------------------------//
char* oneStr = malloc(set_bits + 1);//allocate for string of 1s (binary) equivalent to number of set bits, plus NULL
    memset( oneStr, '1', set_bits); //fill new string with 1s for padding
    oneStr[set_bits] = '\0'; //string of ones, terminated with null character
            //printf("oneStr: %s\n", oneStr);

//convert binary string to integer--use AND bitwise operation
char* ptr; //ptr necessary for strtol function
long int binaryString = strtol(oneStr, &ptr, 2); //create binary string
        //printf("Binary string: %li\n", binaryString);

unsigned long long int set = address & binaryString; //take rightmost bits equivalent to number of set bits
        //printf("Set: %llu\n", set);



//--------------Extract Tag------------------------------------//
long long int tag = address >> set_bits; //shift number of set bits to obtain tag (get tag as an integer)
        //printf("Tag: %llu\n\n\n", tag);    




//-------------begin Simulating Access----------------------------//


//--------------Cache Read----------------------------//
 //Rough Draft: Pseudocode
    /*
    //increase read counter
    //choose specific row--specific index  [row][columns]
    //scan all tag register blocks in row of speciifc index
    //see if tag_array == Tag, Hit
        //hit: increase hit count
        //update LRU counter (pass function index and Tag)
        //return 1 HIT
    //else: Miss
        //increase miss counter
        //look for block to evict
        //select unused block first (no actual eviction here)
        //if blockindex[j] is not used yet (validity  matrix) this is our guy, assign tag to this block
        //update LRU (R/W whether or hit or miss)
        //return 0; MISS
    //if all blocks are used, choose candidate to evict, choose which block from one row (identify with column)
            //evicted_block = choose block to evict(based on policy);
            //check for write back policy, check dirty bit if 1 updated, so yo
            //WB counter++
            tag array is assigned tG OF EVICTED BLOCK?[index][evicted block]
            //update  LRU*/


if( op == 'R' || op == 'r') //if read instruction
{
    //read_count++;//increment read counter

    //search through row of specific set index
    for(int j=0; j<assoc; j++)
    {   
//--------------Cache Read Hit----------------------------//
        if(tag_array[set][j] == tag)
        {
;           read_hit_count++; //increment read hit counter
            hit_flag=1;       //trigger hit flag
           
                if(replace == 0)//LRU Policy
                {
                    replace_position[set][j]=replace_count; // assign current index the counter value
                        replace_count++; //increment counter since access has taken place
                }             

                else if(replace == 1 )//FIFO Policy
                {
                    replace_count++; //increment counter
                }
                
           break; //go to next line of file
        }
    }

//--------------Cache Read Miss---------------------------//
        if(hit_flag==0){  
            mem_read_count++;  //must read from memory
            read_miss_count++; //read miss has occurred
           
            //choose block within row/set 
            for( int j=0; j<assoc; j++)
            {
                //if block isnt used yet, choose block
                if( validity[set][j] == 0)
                {
                    tag_array[set][j] = tag;  //update tag for block chosen
                    validity[set][j] = 1;     //update validity, it is now valid/used
                   
                    replace_position[set][j] = replace_count; //assign current counter value to current index for replacement policy
                    replace_count++;  //increment replacement counter                    
                    unused_set_flag=1; //trigger filled set flag,
                    break;
                }
            }

//--------------Cache Read Evict----------------------------//
        if(unused_set_flag==0)
        {
            //choose block to evict
            long long int lru_min=0; //current minimum counter value within set
            long long int evicted_block=0;  //corresponding column for block eviction
            
            for( int j=0; j<assoc; j++)//search row
            {
                if(j==0)//first execution, assign first block->min
                { lru_min=replace_position[set][j];
                evicted_block=j;
                }
                else if(replace_position[set][j] < lru_min)//find minimum counter, and assign as block to evict
                {
                    lru_min=replace_position[set][j];
                    evicted_block=j;
                }

                }

            //if Write BACK policy is selected
            if( wb == 1 && dirty[set][evicted_block] == 1)
            mem_write_count++; //write back to memory, if the block about to be evicted is dirty

            tag_array[set][evicted_block] = tag; //update tag  of evicted block (eviction)
            validity[set][evicted_block] = 1; //update validity
                //evict_count++; //increment eviction counter
            dirty[set][evicted_block]=0; //make it clean again since replaced with read, no longer dirty

           //update eviction policy, same for LRU and FIFO
                replace_position[set][evicted_block] = replace_count; //assign new block the current counter value
                replace_count++; //increment replacement counter             
            }
        }

            unused_set_flag=0; //reset flag
            hit_flag=0;        //reset flag
        } //end of write instruction, head to next line of file
        



//--------------Cache Write----------------------------//

//Rough draft: Pseudocode
//writeCounter++
//scan all blocks in that row
//check for hit
    //if hit: update write hit counter
    //update LRU or FIFO?
    //if WB dirty bit =1
    //else WT: increase WB counter??
//else miss:
    //increase write miss counter
    //check all blocks in the set for unused
    //if unused, use this guy to write
        //assign tag
        //make dirty flag 1
        //update policy
        //return 0
//if all blocks are occupied??
    //choose guy to evict according to replacement policy
        //if WB Policy: see if evicted block has been updated
         //if updated: WB++
    //else Tag array of current block =tag
        //if WB policy, mark it as dirty??
        //update Policy


//current instruction is a write
if(op == 'W' || op == 'w')
{   
        //write_count++; //increase write counter
    if(wb==0) mem_write_count++; //simulate a write to memory

    //search all blocks within set
    for(int j=0; j<assoc; j++)
    {
 //--------------Cache Write Hit----------------------------//
        if(tag_array[set][j] == tag)
        {
            write_hit_count++; //increase write hit count
            hit_flag=1;  //raise write_hit flag

            //update eviction policy
           if(replace == 0)//LRU Policy
                {
                     replace_position[set][j] = replace_count; //assign it the value of the counter
                        replace_count++; //increment counter since access
                }

                else if(replace == 1 )//FIFO Policy
                {
                    replace_count++; //increment counter since access
                }
                if(wb==1) dirty[set][j]= 1; //set dirty bit

                break;
        }
    }

//--------------Cache Write Miss---------------------------//
    if(hit_flag == 0 )
    {
        mem_read_count++;  //increment both counters
        write_miss_count++;
        
        //look for unoccupied block
            for (int j=0; j<assoc; j++)
            {
                 //if block isnt used yet, choose block
                if( validity[set][j] == 0)
                {
                    //update tag for block chosen
                    tag_array[set][j] = tag;
                    if(wb == 1 ) dirty[set][j] =1; //set dirty bit
                    
                    validity[set][j]=1; //make index valid

                    //update eviction policy
                     replace_position[set][j] = replace_count; //assign it the value of the counter
                     replace_count++; //increment counter since access
                    unused_set_flag=1;  //raise flag
                    break;
                   
                }
            }
        
 //--------------Cache Write Evict----------------------------//
        //need to find block to evict
        if(unused_set_flag==0)
        {
            //choose block to evict
            long long int lru_min=0; //current minimum value counter within set
            long long int evicted_block=0;  //current column for block to evict from set
            for( int j=0; j<assoc; j++) //scan set
            {
                if(j==0)//assign first block as block to evict
                { lru_min=replace_position[set][j];
                evicted_block=j;
                }
                else if(replace_position[set][j] < lru_min)//compare and select lowest counter to evict
                {
                    lru_min=replace_position[set][j];
                    evicted_block=j;
                }

                }
        
        if(wb==1 && dirty[set][evicted_block] == 1) mem_write_count++; //write evicted block to memory

        tag_array[set][evicted_block] = tag; //update block, new tag
        if(wb==1) dirty[set][evicted_block] =1; //set dirty bit since update, and write instruction

            //evict_count++; //increment eviction counter
        validity[set][evicted_block]=1; //amke valid

        //update eviction policy, same for LRU and FIFO
        
                replace_position[set][evicted_block] = replace_count;
                replace_count++;
        }
    }
    //reset flags for next line of file
    hit_flag=0;
    unused_set_flag=0;


}
    }


    fclose(tracefile);  //close file


//--------------Print Statistics----------------------------//
long long int total_misses= write_miss_count+read_miss_count; //calculate total misses and hits
long long int total_hits= write_hit_count+read_hit_count;
    //printf("Total Misses: %lli\n", total_misses); //print  total misses
    //printf("Total Hits: %lli\n", total_hits); //print  total hits

float ratio = (float) total_misses/(total_hits+total_misses); //calculate miss ratio
printf("%f\n", ratio);//display miss ratio
printf("%Lf\n", (long double) mem_write_count); //display total writes to memory
printf("%Lf\n", (long double) mem_read_count); //display total reads from memory

    //printf("Evict Count: %lli\n\n", evict_count);



//--------------OPTIONAL/DEBUG: DISPLAY CACHE----------------------------//
/*
printf("Current Cache:\n");
for(int i=0; i<set_num; i++)
{
    for(int j=0; j<assoc; j++){
    printf("%lli", tag_array[i][j]);
    if(wb==1 & dirty[i][j]== 1)
    printf("(D)");

    if(validity[i][j] == 0)
    printf("(I)");

    printf("    ");
    }
    puts("");
}*/

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