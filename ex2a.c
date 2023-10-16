//name:Mouhammed Ghosheh
//id Number: 322310509

#include <stdio.h>

#include <string.h>

#include <unistd.h>

#include <wait.h>

#include <stdlib.h>
//Function list
void commands(char * input);
int * CountFunc(char * );
void DoCommand(char ** );
void print();
void History_writer(FILE * fp, char * output);

int main() {
    print();
    return 0;
}
void print() {
    //usefull variables init
    int counter = 0;
    char temp[512];
    char temp2[512];
    char in [512];
    char * path = getcwd(temp, 512); //path extraction stored in path
    static int * result;
    int Word_Count = 0;

    FILE * fp;

    while (1) {
        //File creation to append
        fp = fopen("File.txt", "a+");
        if (fp == NULL) {
            printf("Error opening file");
            return;
        }
        counter++; //number of commands counter
        printf("%s>", path);
        fgets( in , 512, stdin); //input getter
        fflush(stdin);
        result = CountFunc( in ); //input string information stored in result array
        if ( in [0] == ' ' || in [0] == '\n') { //if a space or enter were entered it isn't counted as a word and if it was a space we tell the user
            fprintf(fp, "%s", in ); //enter the input to the file
            fflush(fp);
            counter--;
            if ( in [0] == ' ')
                fprintf(stderr, "%s", "SPACES DETECTED\n");
            continue;
        } else if (strcmp( in , "history\n") == 0) { //if the word history was detected:
            History_writer(fp, temp2);
        } else if (strcmp( in , "done\n") == 0) { //close the program if the word done without spaces was detected and print the information needed
            printf("number of commands: %d\n", counter);
            printf("Total number of words in commands is : %d\n", Word_Count);
            fclose(fp);
            return;

        } else if ( in [0] == 'c' && in [1] == 'd') //if cd was detected enter this message
            printf("This Command is not supported (YET)\n");

        else {
            
                fprintf(fp, "%s", in );
                fflush(fp);
                commands( in );
            }
        Word_Count += result[0];
        }
         //total command words

    }

//this function prints the file contents when wrtiting the word history in the input along with their count
void History_writer(FILE * fp, char * output) {
    int count = 0;
    fclose(fp);
    fp = fopen("File.txt", "r"); //these two lines reset the pointer in the file, so we can read from the beginning
    while (fgets(output, 512, fp)) { //output is the word we are currently printing and count is for every word in the file until we finish
        output[strlen(output)] = '\0';
        count++;
        printf("%d: %s", count, output);
    }
    printf("%d: %s\n", count + 1, "history"); //display the word history that was just entered along with the others so it is updated in real time
    fclose(fp); //reset the pointer
    fp = fopen("File.txt", "a+"); //append again
    fprintf(fp, "%s\n", "history"); //enter history to the file
    fflush(fp);
}


//this function stores all the string information that we passed as an argument and returns an array that has all that information, first slot in the array
//is how many words are there in that string, second slot for how many chars,third is whether the word is history (1 if true, 0 if false) and the last one is
//for the exit keyword (1 if true, 0 if false)
//I made it so that it returns an array with all the results so that I pass through the string only one time and store all the information in one run
int * CountFunc(char * word) {
    int i = 0;
    while (word[i] == ' ') i++;
    static int result[4];
    int wordCount = 1;
    int charCount = 1;
    int ifHistory = 0;
    int ifExit = 0;
    for (; i < strlen(word); ++i) {

        if ((word[i] == 'h' && word[i + 1] == 'i' && word[i + 2] == 's' && word[i + 3] == 't' &&
             word[i + 4] == 'o' && word[i + 5] == 'r' && word[i + 6] == 'y') &&
            (word[i + 7] == ' ' || word[i + 7] == '\0')) { //check if history is detected and break if true
            ifHistory = 1; //set the flag to one(detected!!)
            break;
        } else if ((word[i] == 'e' && word[i + 1] == 'x' && word[i + 2] == 'i' && word[i + 3] == 't') &&
                   (word[i + 4] == ' ' || word[i + 4] == '\0')) { //check if history is detected and break if true
            ifExit = 1; //set the flag to one(detected!!)
            break;
        }

        if (word[i] == ' ' && word[i + 1] != ' ' && word[i + 1] != '\0') { //count words
            wordCount++;

        }
        if (word[i + 1] != ' ' && word[i + 1] != '\0') { //count chars
            charCount++;
        }
    }
    //store all the information in the results array and return it
    result[0] = wordCount;
    result[1] = charCount;
    result[2] = ifHistory;
    result[3] = ifExit;
    return result;

}

//this function is responsible of allocating memory for an array that will contain the command words, each word in a place in the array
//with the last place being null
void commands(char * input) {
    int word_count = CountFunc(input)[0];
    char * comms[word_count + 1];
    comms[word_count] = NULL;
    int End_Index = 0, Start_Index = 0, i = 0;
    while (Start_Index < (strlen(input)) - 1) { //strlen-1 because we don't want the \n in the string's end
        if (input[Start_Index] == ' ' || input[Start_Index + 1] == '\n') { //if we reach a new word or the string's ending:
            if (input[Start_Index + 1] == '\n')
                Start_Index++;
            comms[i] = (char * ) calloc(Start_Index - End_Index + 1, sizeof(char)); //memory allocation using calloc(same as malloc
            //but requires two arguments)
            if (comms[i] == NULL) { //check if allocation fails
                perror("Dynamic Allocation Failed");
                break;

            }
            strncpy(comms[i], & input[End_Index], Start_Index - End_Index); //we fill the array place with the words from the command
            End_Index = Start_Index + 1; // jump to the next word
            i++;

        }
        Start_Index++; //loop iteration continue

    }
    DoCommand(comms); //send the command to the execvp
    for (int j = 0; j < i; ++j) {
        free(comms[j]); //free the allocated memory one by one
    }
}
//this function creates a child process through fork and in the child process we execute the command
void DoCommand(char ** comms) {
    pid_t child;
    child = fork();
    if (child == -1)
        perror("Fork Failed"); //fork failing check
    else if (child == 0) {

        if (execvp(comms[0], comms) == -1)
            perror("No Such Command\n"); //if execvp fails

        exit(0);
    }
    wait(NULL);
}


