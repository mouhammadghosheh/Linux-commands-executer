//name:Mouhammed Ghosheh
//id Number: 322310509

#include <stdio.h>

#include <string.h>

#include <sys/prctl.h>

#include <unistd.h>

#include <wait.h>

#include <stdlib.h>


#include <fcntl.h>

//Function list
void commands(char *input, int, int, char *, char *, int);

void SplitString(char *, int, char *);

int Nohup_Identifier(char *);

int second_Pipe(char *);

void SplitString2(char *, int, char *, char *);

int And_Sign(char *);

int *CountFunc(char *);

char *trimwhitespace(char *);

void DoCommand(char **, int, int);

char *Space_Remover(char *);

void execpipe(char **argv1, char **argv2);

int pipeIndex(char *);

void print();

void History_writer(FILE *fp, char *output);

char *Find_Command(char *, FILE *, int);

void execTwoPiped(char **, char **, char **);

void handler(int);


int main() {
    signal(SIGCHLD, handler);
    print();
    return 0;
}

void print() {
    //useful variables init
    int counter = 0;
    char temp[512];
    char temp2[512];
    char dest[512];
    char right_dest[512];
    char Far_right_dest[512];
    char in[512];
    char *path = getcwd(temp, 512); //path extraction stored in path
    static int *result;
    int Word_Count = 0;

    FILE *fp;

    while (1) {
        //File creation to append
        fp = fopen("File.txt", "a+");
        if (fp == NULL) {
            printf("Error opening file");
            return;
        }
        counter++; //number of commands counter
        printf("%s>", path);
        fgets(in, 512, stdin); //input getter
        fflush(stdin);
        result = CountFunc(in); //input string information stored in result array
        if (in[0] == ' ' || in[0] ==
                            '\n') { //if a space or enter were entered it isn't counted as a word and if it was a space we tell the user
            fprintf(fp, "%s", in); //enter the input to the file
            fflush(fp);
            counter--;
            if (in[0] == ' ')
                fprintf(stderr, "%s", "SPACES DETECTED\n");
            continue;
        } else if (strcmp(in, "history\n") == 0) { //if the word history was detected:
            History_writer(fp, temp2);
        } else if (strcmp(in, "done\n") ==
                   0) { //close the program if the word done without spaces was detected and print the information needed
            printf("number of commands: %d\n", counter);
            printf("Total number of words in commands is : %d\n", Word_Count);
            fclose(fp);
            return;

        } else if (in[0] == 'c' && in[1] == 'd') //if cd was detected enter this message
            printf("This Command is not supported (YET)\n");
        else if (Nohup_Identifier(in) == 1) {
            commands(in, 0, And_Sign(in), " ", " ", 1);
            signal(SIGHUP, SIG_IGN);
        } else if (second_Pipe(in) >
                   0) { //if double pipes were detected we do operations to the strings to make it easy to operate
            int second = second_Pipe(in);
            char right[second - pipeIndex(in)];
            char left[pipeIndex(in)];
            char Far_right[strlen(in) - second];
            SplitString(in, second + 1, Far_right);
            SplitString2(in, pipeIndex(in), right, left);
            char new_right[strlen(right) - pipeIndex(right)];
            SplitString2(right, pipeIndex(right), Far_right, new_right);
            strcpy(right, new_right);
            trimwhitespace(left);
            trimwhitespace(Far_right);
            trimwhitespace(right);
            strcpy(right, Space_Remover(right));
            strcpy(Far_right, Space_Remover(Far_right));
            if (left[0] == '!' && right[0] != '!' && Far_right[0] !=
                                                     '!') { // the next set of is statements each one is for a different situation according to the input
                SplitString(left, 1, dest);
                int left_num = atoi(dest);
                strcpy(dest, Find_Command(dest, fp, left_num));

                fprintf(fp, "%s|%s|%s\n", dest, right, Far_right);
                strcat(left, "\n");
                //dest[strlen(dest) - 1] = '\0';
                strcat(right, "\n");
                strcat(Far_right, "\n");
                if (And_Sign(right) >= 0 || And_Sign(dest) >= 0)
                    commands(dest, 2, 1, right, Far_right, 0);
                else
                    commands(dest, 2, 0, right, Far_right, 0);


            } else if (left[0] != '!' && right[0] == '!' && Far_right[0] != '!') {
                SplitString(right, 1, right_dest);
                int right_num = atoi(right_dest);
                strcpy(right_dest, Find_Command(right_dest, fp, right_num));
                strcat(Far_right, "\n");

                if (And_Sign(right_dest) >= 0 || And_Sign(left) >= 0)
                    commands(left, 2, 1, right_dest, Far_right, 0);
                else
                    commands(left, 2, 0, right_dest, Far_right, 0);
                fprintf(fp, "%s|%s|%s", dest, right, Far_right);

            } else if (left[0] != '!' && right[0] != '!' && Far_right[0] == '!') {
                SplitString(Far_right, 1, Far_right_dest);
                int Far_Right_Num = atoi(Far_right_dest);
                strcpy(Far_right_dest, Find_Command(Far_right_dest, fp, Far_Right_Num));
                strcat(right, "\n");
                strcat(dest, "\n");
                if (And_Sign(right_dest) >= 0 || And_Sign(Far_right_dest) >= 0)
                    commands(left, 2, 1, right, Far_right_dest, 0);
                else
                    commands(left, 2, 0, right, Far_right_dest, 0);
                fprintf(fp, "%s|%s|%s", left, right, Far_right_dest);
            } else if (left[0] != '!' && right[0] == '!' && Far_right[0] == '!') {
                SplitString(right, 1, right_dest);
                int right_num = atoi(right_dest);
                SplitString(Far_right, 1, Far_right_dest);
                int Far_Right_Num = atoi(Far_right_dest);
                strcpy(right_dest, Find_Command(right_dest, fp, right_num));
                strcpy(Far_right_dest, Find_Command(Far_right_dest, fp, Far_Right_Num));
                strcat(right, "\n");
                strcat(dest, "\n");
                if (And_Sign(right_dest) >= 0 || And_Sign(Far_right_dest) >= 0)
                    commands(left, 2, 1, right_dest, Far_right_dest, 0);
                else
                    commands(left, 2, 0, right_dest, Far_right_dest, 0);
                fprintf(fp, "%s|%s|%s", left, right_dest, Far_right_dest);
            } else if (left[0] == '!' && right[0] != '!' && Far_right[0] == '!') {
                SplitString(left, 1, dest);
                int left_num = atoi(dest);
                SplitString(Far_right, 1, Far_right_dest);
                int Far_Right_Num = atoi(Far_right_dest);
                strcpy(dest, Find_Command(dest, fp, left_num));
                strcpy(Far_right_dest, Find_Command(Far_right_dest, fp, Far_Right_Num));
                strcat(right, "\n");
                if (And_Sign(right_dest) >= 0 || And_Sign(Far_right_dest) >= 0)
                    commands(dest, 2, 1, right, Far_right_dest, 0);
                else
                    commands(dest, 2, 0, right, Far_right_dest, 0);
                dest[strlen(dest) - 1] = '\0';
                fprintf(fp, "%s|%s|%s", left, right_dest, Far_right_dest);
            } else if (left[0] == '!' && right[0] == '!' && Far_right[0] != '!') {
                SplitString(left, 1, dest);
                int left_num = atoi(dest);
                SplitString(right, 1, right_dest);
                int right_num = atoi(right_dest);
                strcpy(dest, Find_Command(dest, fp, left_num));
                strcpy(right_dest, Find_Command(right_dest, fp, right_num));
                strcat(Far_right, "\n");
                if (And_Sign(right_dest) >= 0 || And_Sign(dest) >= 0)
                    commands(dest, 2, 1, right_dest, Far_right, 0);
                else
                    commands(dest, 2, 0, right_dest, Far_right, 0);
                dest[strlen(dest) - 1] = '\0';
                fprintf(fp, "%s|%s|%s", dest, right_dest, Far_right);
            } else if (left[0] == '!' && right[0] == '!' && Far_right[0] == '!') {
                SplitString(left, 1, dest);
                int left_num = atoi(dest);
                SplitString(right, 1, right_dest);
                int right_num = atoi(right_dest);
                SplitString(Far_right, 1, Far_right_dest);
                int Far_Right_Num = atoi(Far_right_dest);
                strcpy(dest, Find_Command(dest, fp, left_num));
                strcpy(right_dest, Find_Command(right_dest, fp, right_num));
                strcpy(Far_right_dest, Find_Command(Far_right_dest, fp, Far_Right_Num));

                fprintf(fp, "%s|%s|%s\n", dest, right_dest, Far_right_dest);
                strcat(dest, "\n");
                if (And_Sign(right) >= 0 || And_Sign(dest) >= 0)
                    commands(dest, 2, 1, right_dest, Far_right_dest, 0);
                else
                    commands(dest, 2, 0, right_dest, Far_right_dest, 0);

                right_dest[strlen(right_dest) - 1] = '\0';
                fprintf(fp, "%s|%s|%s\n", dest, right_dest, Far_right_dest);
            } else {
                fprintf(fp, "%s|%s|%s", left, right, Far_right);
                strcat(left, "\n");
                strcat(right, "\n");
                strcat(Far_right, "\n");
                strcpy(right, Space_Remover(right));
                strcpy(Far_right, Space_Remover(Far_right));
                commands(left, 2, 0, right, Far_right, 0);
            }
        } else if (pipeIndex(in) >= 0) {

            char right[strlen(in) - pipeIndex(in)];
            char left[pipeIndex(in)];
            SplitString2(in, pipeIndex(in), right, left);
            trimwhitespace(left);
            trimwhitespace(right);
            strcpy(right, Space_Remover(right));
            if (left[0] == '!' && right[0] != '!') {
                SplitString(left, 1, dest);
                int left_num = atoi(dest);
                strcpy(dest, Find_Command(dest, fp, left_num));
                strcat(right, "\n");
                if (And_Sign(right) >= 0 || And_Sign(dest) >= 0)
                    commands(dest, 1, 1, right, " ", 0);
                else
                    commands(dest, 1, 0, right, " ", 0);
                dest[strlen(dest) - 1] = '\0';
                fprintf(fp, "%s|%s", dest, right);
            } else if (left[0] == '!' && right[0] == '!') {
                SplitString(left, 1, dest);
                int left_num = atoi(dest);
                SplitString(right, 1, right_dest);
                int right_num = atoi(right_dest);
                strcpy(dest, Find_Command(dest, fp, left_num));
                strcpy(right_dest, Find_Command(right_dest, fp, right_num));
                if (And_Sign(right_dest) >= 0 || And_Sign(dest) >= 0)
                    commands(dest, 1, 1, right_dest, " ", 0);
                else
                    commands(dest, 1, 0, right_dest, " ", 0);
                dest[strlen(dest) - 1] = '\0';
                fprintf(fp, "%s|%s", dest, right_dest);
            } else if (left[0] != '!' && right[0] == '!') {
                SplitString(right, 1, right_dest);
                int right_num = atoi(right_dest);
                strcpy(right_dest, Find_Command(right_dest, fp, right_num));
                if (And_Sign(right_dest) >= 0 || And_Sign(left) >= 0)
                    commands(left, 1, 1, right_dest, " ", 0);
                else
                    commands(left, 1, 0, right_dest, " ", 0);
                dest[strlen(dest) - 1] = '\0';
                fprintf(fp, "%s|%s", dest, right_dest);
            } else {
                fprintf(fp, "%s", in);
                strcat(left, "\n");
                strcat(right, "\n");
                strcpy(right, Space_Remover(right));
                commands(left, 1, 0, right, " ", 0);
            }
        } else {
            if (in[0] != '!') { //if the input was a normal command sent it to the file and then send it to the execvp
                fprintf(fp, "%s", in);
                fflush(fp);
                if (And_Sign(in) >= 0) {
                    in[And_Sign(in) - 1] = '\n';
                    in[And_Sign(in)] = '\0';
                    commands(in, 0, 1, " ", " ", 0);

                } else
                    commands(in, 0, 0, " ", " ", 0);
            } else { //else if the input was a ! with a number then we extract the number using atoi
                SplitString(in, 1, dest);
                int num = atoi(dest);
                char *str = Find_Command(in, fp,
                                         num); //this function finds the command from the file and sends it to the command function
                if (strcmp(str, " ") !=
                    0) { //the function returns an empty string if number of lines entered exceeds total number of lines in file
                    if (strcmp(str, "history\n") ==
                        0) //if the command extracted was history send it to the history function
                        History_writer(fp, temp2);
                    else {
                        printf("%s", str); //print the extracted command
                        fprintf(fp, "%s", str); //enter it to the file
                        fflush(fp);

                        commands(str, 0, 0, " ", " ", 0); //send the command to execution
                    }
                }

                result[0] = CountFunc(str)[0]; //number of words for the extracted command
            }
        }
        Word_Count += result[0]; //total command words

    }
}

//returns the index of the first pipe
int pipeIndex(char *input) {
    for (int i = 0; i < strlen(input); ++i) {
        if (input[i] == '|')
            return i;

    }
    return -1;
}

//returns the indext of the second pipe if available
int second_Pipe(char *input) {
    int flag = -1;
    int i = 0;
    for (i = 0; input[i] != '|'; ++i);

    for (int j = i + 1; j < strlen(input); ++j) {
        if (input[j] == '|') {
            return j;
        }
    }
    return flag;
}

//return the index of the and sign
int And_Sign(char *input) {
    for (int i = 0; i < strlen(input); ++i) {
        if (input[i] == '&')
            return i;

    }
    return -1;
}

//this function prints the file contents when wrtiting the word history in the input along with their count
void History_writer(FILE *fp, char *output) {
    int count = 0;
    fclose(fp);
    fp = fopen("File.txt", "r"); //these two lines reset the pointer in the file, so we can read from the beginning
    while (fgets(output, 512,
                 fp)) { //output is the word we are currently printing and count is for every word in the file until we finish
        output[strlen(output)] = '\0';
        count++;
        printf("%d: %s", count, output);
    }
    printf("%d: %s\n", count + 1,
           "history"); //display the word history that was just entered along with the others so it is updated in real time
    fclose(fp); //reset the pointer
    fp = fopen("File.txt", "a+"); //append again
    fprintf(fp, "%s\n", "history"); //enter history to the file
    fflush(fp);
}

//this function counts how many lines there currently is in the file and then checks if the line number that was ordered is in the range of the total
//line number in the file and if this is true it iterates through the lines until it reaches the needed line number
// the function return the new command that was extracted and it returns an empty string if the line was not in range
char *Find_Command(char *in, FILE *pFile, int num) {
    pFile = fopen("File.txt", "r");
    char line[512];
    int Line_Number = 0;
    while (fgets(line, 512, pFile)) {
        Line_Number++;
    }
    pFile = fopen("File.txt", "r");
    if (Line_Number < num)
        perror("Not In History\n");
    else {
        for (int i = 0; i < num; ++i) {
            fgets(line, 512, pFile);
        }
        strcpy(in, line);
        fclose(pFile);
        return in;
    }
    return " ";

}

//this function stores all the string information that we passed as an argument and returns an array that has all that information, first slot in the array
//is how many words are there in that string, second slot for how many chars,third is whether the word is history (1 if true, 0 if false) and the last one is
//for the exit keyword (1 if true, 0 if false)
//I made it so that it returns an array with all the results so that I pass through the string only one time and store all the information in one run
int *CountFunc(char *word) {
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
void commands(char *input, int swish, int And_Swish, char *right, char *FarRight, int nohup) {
    int word_count = CountFunc(input)[0];
    int right_count = CountFunc(right)[0];
    int Far_rightCount = CountFunc(FarRight)[0];
    char *comms[word_count + 1];
    int m = 0;
    char *rightComms[right_count + 1];
    char *Far_rightComms[right_count + 1];
    comms[word_count] = NULL;


    int End_Index = 0, Start_Index = 0, i = 0;
    while (Start_Index < (strlen(input)) - 1) { //strlen-1 because we don't want the \n in the string's end
        if (input[Start_Index] == ' ' ||
            input[Start_Index + 1] == '\n') { //if we reach a new word or the string's ending:
            if (input[Start_Index + 1] == '\n')
                Start_Index++;
            comms[i] = (char *) calloc(Start_Index - End_Index + 1,
                                       sizeof(char)); //memory allocation using calloc(same as malloc
            //but requires two arguments)
            if (comms[i] == NULL) { //check if allocation fails
                perror("Dynamic Allocation Failed");
                break;

            }
            strncpy(comms[i], &input[End_Index],
                    Start_Index - End_Index); //we fill the array place with the words from the command
            End_Index = Start_Index + 1; // jump to the next word
            i++;

        }
        Start_Index++; //loop iteration continue

    }

    int k = 0;
    if (swish >= 1) { //if single pipe allocate another memory to the second command
        rightComms[right_count] = NULL;
        End_Index = 0, Start_Index = 0;

        while (Start_Index < (strlen(right)) - 1) { //strlen-1 because we don't want the \n in the string's end
            if (right[Start_Index] == ' ' ||
                right[Start_Index + 1] == '\n') { //if we reach a new word or the string's ending:
                if (right[Start_Index + 1] == '\n')
                    Start_Index++;
                rightComms[k] = (char *) calloc(Start_Index - End_Index + 1,
                                                sizeof(char)); //memory allocation using calloc(same as malloc
                //but requires two arguments)
                if (rightComms[k] == NULL) { //check if allocation fails
                    perror("Dynamic Allocation Failed");
                    break;

                }

                strncpy(rightComms[k], &right[End_Index],
                        Start_Index - End_Index);//we fill the array place with the words from the command

                End_Index = Start_Index + 1; // jump to the next word
                k++;

            }

            Start_Index++; //loop iteration continue

        }

    }
    if (swish >= 2) { // if there is two pipes allocate memory for the third command
        Far_rightComms[Far_rightCount] = NULL;
        m = 0;

        End_Index = 0, Start_Index = 0;

        while (Start_Index < (strlen(right)) - 1) { //strlen-1 because we don't want the \n in the string's end
            if (FarRight[Start_Index] == ' ' ||
                FarRight[Start_Index + 1] == '\n') { //if we reach a new word or the string's ending:
                if (FarRight[Start_Index + 1] == '\n')
                    Start_Index++;
                Far_rightComms[m] = (char *) calloc(Start_Index - End_Index + 1,
                                                    sizeof(char)); //memory allocation using calloc(same as malloc
                //but requires two arguments)
                if (Far_rightComms[m] == NULL) { //check if allocation fails
                    perror("Dynamic Allocation Failed");
                    break;

                }

                strncpy(Far_rightComms[m], &FarRight[End_Index],
                        Start_Index - End_Index);//we fill the array place with the words from the command

                End_Index = Start_Index + 1; // jump to the next word
                m++;

            }

            Start_Index++; //loop iteration continue

        }

    }

    if (swish == 0) {
        if (nohup == 1) {
            DoCommand(&comms[1], And_Swish, nohup);
        } else
            DoCommand(comms, And_Swish, nohup); //send the command to the execvp

    } else if (swish == 1) {

        execpipe(comms, rightComms); //the command for a pipe
    } else {
        execTwoPiped(comms, rightComms, Far_rightComms); //function for two pipes
    }
    for (int j = 0; j < i; ++j) {
        free(comms[j]); //free the allocated memory one by one
    }

        for (int j = 0; j < k; ++j) {
            free(rightComms[j]); //free the allocated memory one by one
        }

        for (int j = 0; j < m; ++j) {
            free(Far_rightComms[j]); //free the allocated memory one by one
        }
}

//this function creates a child process through fork and in the child process we execute the command
void DoCommand(char **comms, int And_swish, int nohup) {
    pid_t child;
    child = fork();
    int file;
    if (nohup == 1) {
        file = open("nohup.txt", O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if (file < 0)
            perror("NOHUP Failed");
    }
    if (child == -1)
        perror("Fork Failed"); //fork failing check
    else if (child == 0) {
        prctl(PR_SET_PDEATHSIG, SIGHUP);

        if (nohup == 1) {

            dup2(file, STDOUT_FILENO);
        }

        if (execvp(comms[0], comms) == -1)
            perror("No Such Command\n"); //if execvp fails

    }
    if (nohup == 1)
        close(file);
    if (And_swish == 0) {
        wait(NULL);
        wait(NULL);
        wait(NULL);
    }


}

//this function identifies a process and kills it when used with the main signal so we don't have a zombie process
void handler(int sig) {

    int waitN = 1;
    while (waitN != -1 && waitN != 0) {
        waitN = waitpid(-1, &sig, WNOHANG);
    }
}


//this function splits the string according to the index we need to split it from,it is used so we can remove the ! from
//the command number string when we want to execute it from the file,it recieves a string and according to the length of it and
//from the index we want to get it stores the characters we want in new string
void SplitString(char *Str, int index, char *dest) {
    int length = strlen(Str);

    if (index < length) {
        for (int i = index; i < length + 1; i++)
            dest[i - index] = Str[i];
    }

}

//splits the string to two strings according to the index
void SplitString2(char *Str, int index, char *right, char *left) {
    int length = strlen(Str);
    int j = 0;
    int i = 0;
    if (index < length) {
        for (i = 0; i < index; ++i) {
            left[i] = Str[i];
        }
        left[i] = '\n';
        left[index + 1] = '\0';
        for (int i = index + 1; i <= length - 1; ++i, j++) {
            right[j] = Str[i];
        }
        right[j - 1] = '\n';
        right[j] = '\0';
    }
}

// a function that removes leading white spaces
char *Space_Remover(char *str) {
    static char str1[99];
    int count = 0, l, k;

    // Iterate String until last
    // leading space character
    while (str[count] == ' ') {
        count++;
    }

    // Putting string into another
    // string variable after
    // removing leading white spaces
    for (l = count, k = 0;
         str[l] != '\0'; l++, k++) {
        str1[k] = str[l];
    }
    str1[k] = '\0';

    return str1;
}

//a function that trims trailing white spaces
char *trimwhitespace(char *str) {
    int index, i;

    /* Set default index */
    index = -1;

    /* Find last index of non-white space character */
    i = 0;
    while (str[i] != '\0') {
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n') {
            index = i;
        }

        i++;
    }

    /* Mark next character to last non-white space character as NULL */
    str[index + 1] = '\0';
    return str;
}

//a function thatexecutes a pipe command
void execpipe(char **argv1, char **argv2) {

    int fds[2];
    pipe(fds);
    pid_t parent;
    pid_t pid = fork();
    if (pid == -1) {

        printf("error fork!!\n");

    }
    if (pid == 0) { // child process

        dup2(fds[0], 0);//stream the fds[0] from the output
        close(fds[0]);
        close(fds[1]);
        if (execvp(argv2[0], argv2) == -1)
            perror("No Such Command\n");
        exit(0);
    } else {
        parent = fork();
        if (parent == -1) {
            perror("Parent fork failed");
        }
        if (parent == 0) {

            dup2(fds[1], 1); // to the input
            close(fds[0]);
            close(fds[1]);

            if (execvp(argv1[0], argv1) == -1)
                perror("pipe not working\n");


        }
    }
    close(fds[0]);
    close(fds[1]);
    wait(NULL);
    wait(NULL);
}

//a function that executes three command pipe
void execTwoPiped(char **argv1, char **argv2, char **argv3) {
    int fds1[2];
    int fds2[2];
    pipe(fds1);
    pipe(fds2);
    pid_t pid1;
    pid_t pid2; //three children
    pid_t pid3;
    pid1 = fork();
    if (pid1 == -1) {
        printf("error fork!!\n");
    }
    if (pid1 == 0) { // child process
        dup2(fds1[1], STDOUT_FILENO);  //stream the first to the output
        close(fds1[1]);
        close(fds1[0]);
        close(fds2[0]);
        close(fds2[1]);
        if (execvp(argv1[0], argv1) == -1)
            perror("No Such Command\n");   // execute it
        exit(0);
    } else {
        pid2 = fork();
        if (pid2 == 0) {
            dup2(fds1[0], STDIN_FILENO);//stream the first back to input
            dup2(fds2[1], STDOUT_FILENO);//stream the second to output
            close(fds1[0]);
            close(fds1[1]);
            close(fds2[0]);
            close(fds2[1]);
            if (execvp(argv2[0], argv2) == -1) //execute the second
                perror("pipe not working\n");
            exit(0);
        } else if (pid2 == -1) {
            perror("pid2 failed");
        } else {//safety tests
            pid3 = fork();
            if (pid3 == -1) {
                perror("PID3 FAILED");
            }
            if (pid3 == 0) {
                dup2(fds2[0], STDIN_FILENO); //stream the second to input finally
                close(fds1[0]);
                close(fds1[1]);
                close(fds2[0]);
                close(fds2[1]);
                if (execvp(argv3[0], argv3) == -1) // execute the third and last command
                    perror("pipe not working\n");
                exit(0);
            } else {
                close(fds1[0]);
                close(fds1[1]);
                close(fds2[0]);
                close(fds2[1]);
                wait(NULL);
                wait(NULL);
                wait(NULL);
            }

        }
    }
}

int Nohup_Identifier(char *in) {
    if (in[0] == 'n' && in[1] == 'o' && in[2] == 'h' && in[3] == 'u' && in[4] == 'p') {
        return 1;
    }
    return 0;
}
