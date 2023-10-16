#Linux Commands executor
azrieli college

==Description==
This program stores input-given commands in an external file,
the program has two unique keywords,"history" which shows every input the user entered from the first run of the program and "done" which ends the program and the shows some information for the user tha twill be specified after, the program does:
1-Counts how many words were entered in each run.

2-Counts how many commands were entered each run. 

(these calculation are written to the console after the user enters "done" which ends the program).

3-the program executes the commands that were given in the input by the user through execvp if possible and shows the result of these commands' execution on the console, an error will be displayed accordingly if the command is not relevant.

4-the program enters all of the user input into a file called "File.txt" the file stores everything the user input bar the word "done", the file's contents can be shown on the console using the keyword "history", everything the user inputs is stored in this file.

5- the program scans if the user input a string from the format "!number" if this happens the program will execute the command that is found in the line "number" and print it to the console if of course number is not bigger than the total number of lines in the file "File.txt".



functions:

1-print(): a void function that is responsible for recording the input and showing the output and check for the special keywords and write the string into the file.txt.

2-CountFunc(char*): this function returns an array that stores all of the information related to the string ,it recieves the input as a parameter, that includes How many words,how many characters,whether if history keyword was detected.

3-SplitString(char*,int,char*):this function recieves an input as a parameter and splits the string starting from the given index in the parameter, it then stores the split string in the third parameter.

4-commands(char*):this function recieves the input as a parameter and splits the 
input into words and puts them in an allocated array using calloc, the input's words
are stored one by one, each word in a place in the array and the last place in the
array is null so that execvp works as expected, the function checks if the allocation works as expected and prints an error if needed, then it frees all the memory that was allocated through the array, the function is void but it activates the function that has the execvp(DoCommad()).

5-DoCommand(char**):this function creates a child process through fork() and check if it fails sand in the child process it executes the command if it is a good command and prints an error if it doesn't work.

6-History_writer(FILE*,char*):this function prints the file contents when writing the word history in the input along with their count this function is called whenever history needs to be activated and this happens in two circumstances,the first one is when the user inputs history directly and the second one is when !"number" hits the line number that has history in it and activates also this function .

7-Nohup_Identifier(char*) : this function checks if there is a nohup in the input

8-second_Pipe(char*): this function returns the index of the second pipe if available 

9-And_Sign(char*): this function returns the index of the & sign if available 

10- char* trimwhitespace(char*): this function deletes leading spaces and returns the edited string

11-Space_Remover : this function removes all leading spaces in a string and returns the edited string

12- execpipe(char**,char**) this function executes a command that has two pipes

13- pipeIndex(char*): this function returns the index of the first pipe if available

14-execTwoPiped(char**,char**,char**): this function executes a command that has two pipes

15- handler: returns handle for the signal in the main to handle the process

16- void SplitString2(char *, int, char *, char *): this function splits the string into two strings according to the given index

==Program Files==
ex3.c : the main file
file.txt: history file
nohup.txt: nohup input

Makefile-to compile the program.
==How to compile?==
compile: gcc -o ex3.c ex3
run: ./ex3

==Input:==
the user inputs terminal line commands

==Output:==
executes the given command if it is supported, if not it prints a suitable error, "history" prints the file's contents and "done" ends the program
