//Writeup
//https://drive.google.com/file/d/0B3TpGffw4gaITDc2ZkxVRmI4cmM/view?usp=sharing


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

typedef struct LINK *CHAIN;
struct LINK {
	char word[6];
	CHAIN next;
};

//I will be including a separate file with more details on the algorithms/any additional info worth mentioning



//I found this code at
//http://stackoverflow.com/questions/361/generate-list-of-all-possible-permutations-of-a-string
//this function is recursive and also calls Swap
//the way this is done is called backtracking which honestly I don't understand completely
//it works something like recursively calling the function on smaller subsets of the letters in the word to be permuted

void Swap(char *a, char *b) {
    char temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

void Permute(char *a, int i, int n,CHAIN checkList) {
    int j;
    int p;
    if(i == n) {
        while(checkList->next!=NULL){
            if(strcmp(a,checkList->word)==0){
                goto skipInsert;
            }
        checkList=checkList->next;
        }

        if(checkList->next==NULL){
            for(p=0;p<6;p++){
                checkList->word[p]=a[p];
            }
        checkList->next=(CHAIN)malloc(sizeof(struct LINK));
        checkList=checkList->next;
        checkList->next=NULL;
        }
        else{
            while(checkList->next!=NULL){
                checkList=checkList->next;
            }
            for(p=0;p<6;p++){
                checkList->word[p]=a[p];
            }
            checkList->next=(CHAIN)malloc(sizeof(struct LINK));
            checkList=checkList->next;
            checkList->next=NULL;

        }

        //printf("%s\n", a);
    skipInsert:;
    }
    else {
        for(j = i; j <= n; j++) {
            Swap(a + i, a + j);
            Permute(a, i + 1, n,checkList);
            Swap(a + i, a + j);
        }
    }
}


//taken from http://www.geeksforgeeks.org/power-set/
//it finds the power set of a string by iterating through a binary counter the same length of the string
//and grabbing the letters from the bits that are "on"

void PowerSet(char *set, int set_size,CHAIN checkList)
{
    int counter, j,i=0;
    char tempArray[10];
    for(i=0;i<10;i++){
            tempArray[i]=0;
        }
    i=0;
    /*set_size of power set of a set with set_size
      n is (2**n -1)*/
    unsigned int pow_set_size = pow(2, set_size);


    /*Run from counter 000..0 to 111..1*/
    for(counter = 0; counter < pow_set_size; counter++){
      for(j = 0; j < set_size; j++){
          /* Check if jth bit in the counter is set
             If set then pront jth element from set */
          if(counter & (1<<j)){
            //this is where it could be copied into an array one char at a time
            //printf("%c", set[j]);
            tempArray[i]=set[j];
            i++;
          }
       }
       //printf("\n");
       if ((strlen(tempArray)>2)){
            //printf("%s\n",tempArray);

       Permute(tempArray, 0, strlen(tempArray) - 1,checkList);
       //printf("----------------------------------------------\n");
       }
       for(i=0;i<10;i++){
            tempArray[i]=0;
        }
    i=0;
    }
}

//takes the word, the length of the word and where it belong in the hash table as arguments
void InsertWord(int index, char* fromFile, CHAIN hashArrayAddress){
    int i=0;
    if (hashArrayAddress->next==NULL){
        //this happens when there is an empty struct at the index in the hash table, want to insert the word in that element
        for (i=0;i<=index;i++){
            //copying over the relevant characters into the struct
            hashArrayAddress->word[i]=fromFile[i];
        }
            //this is an extra precaution to make sure there isn't any garbage in the array
        for(i=index+1;i<=5;i++){
            hashArrayAddress->word[i]=0;
        }
        hashArrayAddress->next=(CHAIN)malloc(sizeof(struct LINK));
        hashArrayAddress=hashArrayAddress->next;
        hashArrayAddress->next=NULL;
    }

    else{
        //printf("in insertWord else\n");
        //if it is not null, need to add to the end of the chain
        while(hashArrayAddress->next!=NULL){           //finding the end of the link
            hashArrayAddress=hashArrayAddress->next;
        }
        //now have the address of the empty struct at the end of the chain
        for (i=0;i<=index;i++){
            //copying over the relevant characters into the struct
            hashArrayAddress->word[i]=fromFile[i];
        }
            //this is an extra precaution to make sure there isn't any garbage in the array
        for(i=index+1;i<=5;i++){
            hashArrayAddress->word[i]=0;
        }
        //creating the new node, adding it to the chain, setting next to NULL at the last node
        hashArrayAddress->next=(CHAIN)malloc(sizeof(struct LINK));
        hashArrayAddress=hashArrayAddress->next;
        hashArrayAddress->next=NULL;
    }
}

//searches the table for a match
//this function is fed the linked list full of the words that can be made with the letters input by the user

void SearchWord(int index, char* fromFile, CHAIN hashArrayAddress,int* score){
    int i=0;
    int count=0;
    while(hashArrayAddress->next!=NULL){
        for(i=0;i<=index;i++){
            //printf("%c ",fromFile[i]);
            //printf("%c ",hashArrayAddress->word[i]);
            if(fromFile[i]==hashArrayAddress->word[i]){
                 //printf("%c %c\n",fromFile[i],hashArrayAddress->word[i]);
                count++;
            }
        }
        //printf("--------\n");
        if (count==(index+1)){
            printf("Match found: %s\t",fromFile);
            if(count==3){
                printf("10 points\n");
                *score=*score+10;
            }
            if(count==4){
                printf("25 points\n");
                *score=*score+25;
            }
            if(count==5){
                printf("50 points\n");
                *score=*score+50;
            }
            if(count==6){
                printf("100 points\n");
                *score=*score+100;
            }

            break;
        }
    count=0;
    hashArrayAddress=hashArrayAddress->next;
    }
return;
}





//this is the function that calculates the hash and processes the word to see if it is valid
//it takes the array that holds the word being scanned in and a double pointer to the hash array
//the insert parameter tells the function if it is inserting or searching
void Hash(char* inputWord,CHAIN *hashPtr,int insert,int*score){
    //want to do some processing that excludes the inputWords shorter than 2 and longer than 6
    int total=0;
    int index=0;
    int count=0;
    int hashValue=0;
    CHAIN hashArrayAddress=0;
    int i=0;
    int hashNums[]={9676,5293,3436,1151,966,488};

    //this is what processes the string for length and valid characters
    while(1){
        if (((inputWord[i]<65)||((inputWord[i]>90)&&(inputWord[i]<97))||inputWord[i]>122)){
            return;
        }
        total=(inputWord[index])*(hashNums[index])+total;
        index++;
        if(inputWord[index]==0){
        break;
        }
    }
    count=index;
    index=count-1;
    if(count<3){
        if(insert==0){
            printf("Invalid word, less than 3 letters\n");
        }
        return;
    }
    if(count>6){
        if(insert==0){
            printf("Invalid word, more than 6 letters\n");
        }
        return;
    }

    hashValue=total%9999;
    hashArrayAddress=hashPtr[hashValue];

    if(insert==1){
        InsertWord(index,inputWord,hashArrayAddress);
    }
    else{
        SearchWord(index,inputWord,hashArrayAddress,score);
        return;
    }
    return;
}

//for clearing the search array linked list when the user wants to put in another word
DeleteList(CHAIN toBeDeleted){
    CHAIN tempPtr;
    while(toBeDeleted!=NULL){
        tempPtr=toBeDeleted;
        toBeDeleted=toBeDeleted->next;
        free(tempPtr);
    }
}

int main()
{
    FILE *inFile=NULL;
    int i=0,j=0;
    int* score=(int)malloc(sizeof(int));
    *(score)=0;
    inFile = fopen("words","r");

    if (inFile == NULL)
	{
		printf("Error, invalid filename\n");
		return -1;
	}

	//I initialize structs in every position. Its wasteful but the only way I could get it to work
	//I also set all of the values to zero and next's to NULL
    CHAIN *hashPtr=NULL;
    CHAIN hashArray[9999];
    for(i=0;i<9999;i++){
        hashArray[i]=(CHAIN)malloc(sizeof(struct LINK));
        for(j=0;j<6;j++){
            hashArray[i]->word[j]=0;
        }
        hashArray[i]->next=NULL;
    }

    hashPtr=(&hashArray[0]);

    char tempArray[255];
    //this is the loop that fills up the dictionary hash table
    while(!feof(inFile)){
        fscanf(inFile, "%s",tempArray );
        Hash(tempArray,hashPtr,1,score);
    }
    //printf("Done scanning file\n\n\n");

    //this is for safety because of how I process the strings
    for(i=0;i<255;i++){
        tempArray[i]=0;
    }
    printf("\n\n");

    int wordCount=0;
    CHAIN checkList;
    CHAIN tempCheckPtr=NULL;
    checkList=(CHAIN)malloc(sizeof(struct LINK));
    checkList->next=NULL;
    char copyArray[7];
    char anotherWord[200];
    anotherWord[0]='y';

    while(anotherWord[0]=='y'){
        tryagain:;
            for(i=0;i<255;i++){
                tempArray[i]=0;
            }
        printf("Please input a word to be searched for:\n");
        printf("Valid words are between 3 and 6 letters,\ncontain no unusual characters:\n\n");
        scanf("%s",tempArray);
        i=0;

        //this is sloppy and unsightly. Should be in a function but I am in a rush to get this done
        while(1){
            if(tempArray[i]==0){
                break;
            }

            if (((tempArray[i]<65)||((tempArray[i]>90)&&(tempArray[i]<97))||tempArray[i]>122)){
                printf("Invalid character, please enter a valid word\n");
                goto tryagain;
            }
        i++;

        }
        if(i<3){
            printf("Word too short, please enter a valid word\n");
            goto tryagain;
        }
        if(i>6){
            printf("Word too long, please enter a valid word\n");
            goto tryagain;
        }

    PowerSet(tempArray, strlen(tempArray),checkList);


    //much of this is making sure that there isn't any garbage in any arrays
    //if you haven't noticed I'd rather be safe than sorry
    for(i=0;i<7;i++){
        copyArray[i]=0;
    }
    tempCheckPtr=checkList;
        while(tempCheckPtr->next!=NULL){
            for(i=0;i<6;i++){
                copyArray[i]=tempCheckPtr->word[i];
            }
        Hash(copyArray,hashPtr,0,score);
            for(i=0;i<7;i++){
                copyArray[i]=0;
            }
        tempCheckPtr=tempCheckPtr->next;
        }

    printf("FINAL SCORE:%d\n\n",*score);
    printf("Would you like to enter another word? y/n\n");
    DeleteList(checkList);
    *(score)=0;
    scanf("%2s",anotherWord);
    }
    printf("See ya later\n");


















/*
    tempCheckPtr=checkList;
    //code for running through the list of words to be checked

    while(tempCheckPtr->next!=NULL){

            for(i=0;i<12;i++){
                printf("%c",tempCheckPtr->word[i]);
            }
            printf("\n");

        tempCheckPtr=tempCheckPtr->next;
        wordCount++;
    }
    printf("the number of words is the list is %d\n",wordCount);
*/

















    //-----------------------testing-----------------------------
/*
    int g=9998;
    CHAIN tempPtr=NULL;
    tempPtr=hashArray[g];
    for(j=0;j<6;j++){
            printf ("%c", tempPtr->word[j]);
        }
        printf("\n");
    while(tempPtr->next!=NULL){
    tempPtr=tempPtr->next;
    for(j=0;j<6;j++){
            printf ("%c", tempPtr->word[j]);
        }
    printf("\n");
    }


    int longest=0,linkcount=0,count=0;
    for (i=0;i<9999;i++){
        if(hashArray[i]->next==NULL){
            count++;
        }
    tempPtr=hashArray[i];
        while(tempPtr->next!=NULL){
            tempPtr=tempPtr->next;
            linkcount++;
        }
        if (linkcount>longest){
            longest=linkcount;
        }
    linkcount=0;
    }
    printf("count %d\n",count);
    printf("longest %d\n",longest);
*/
    return 0;
}





/*
(hashArray[0])->word[0]='b';
        printf("structtest2 %c\n",(hashArray[0])->word[0]);
        (hashArray[2])->word[2]='c';
        printf("structtest2 %c\n",(hashArray[2])->word[2]);
    (hashArray[0])->word[0]='a';
    printf("structtestmain %c\n",(hashArray[0])->word[0]);
    printf("in main &(hashArray[0])=%x\n",&(hashArray[0]));
    printf("hashArray %x\n",hashArray);
        printf("hashArray[0] %x\n",hashArray[0]);
        printf("*(hashArray[0]) %x\n",*(hashArray[0]));
        printf("&(hashArray[0]) %x\n",&(hashArray[0]));

printf("hashArray %d\n",hashArray);
	printf("&hashArray %d\n",&hashArray);

	printf("hashPtr+1 %d\n",hashPtr);
	hashPtr->word[0]='a';
    printf("main %c\n",hashPtr->word[0]);
    printf("hashPtr->next %x\n",hashPtr->next);

    */
