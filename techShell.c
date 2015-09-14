/*
 * techshell.c
 * made by James Ehlmann
 * 
 * not finished
 * probably never will be
 * current goal: fix the ||||||||| and fix grep 'c' giving error code 256 which is not an error code...
 * current succcess rate: 99%
 * could be better
 * realistic success rate:
 * 1500%
 * 
 * unpredictable things are mostly managed.
 * 
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void PrintHelp()
{
	printf("\n/*******help page of mystical cow powers******************/\n\n");
	printf("works like bash, except still under development\n");
	printf("still breaks |*7 or something like that\n");
	printf("$ to retrieve set variables, unfortunatly intrerpets until\n");
	printf("white space or $ is hit again.\n");
	printf("cd, ls, pwd, links, sudo, apt-get links, man, man, man, man\n");
	printf("honestly you should probably install bash\n\n");
	printf("/*********************************************************/\n\n");
}

// this fucks with it even if we didn't pass by reference because the syntax for that was impossible to find. :P
void Cover(char * list[], int pos)
{
	while(list[pos + 1] != NULL)
	{
		list[pos] = realloc(list[pos], sizeof(strlen(list[pos+1])));
		strcpy(list[pos], list[pos + 1]);
		pos++;
	}
	list[pos] = NULL;
}

// I DIDN"T EVEN USE THIS XD
void addEntry(char** tempTotal, char* input, int i, int last)
{
	tempTotal[0][0]++;
	//printf("%d", (int)tempTotal[0][0]);
	tempTotal[(int)tempTotal[0][0]] = malloc(sizeof(char) * ( i - last));
	strcpy(tempTotal[(int)tempTotal[0][0]], (input + sizeof(char) * last)); 
	tempTotal[(int)tempTotal[0][0]][i-last] = '\0';
	//printf("0,0: %d; thing: %s\n", (unsigned int)tempTotal[0][0], tempTotal[(int)tempTotal[0][0]]);
}

char * pwd;
char * distro;
char * username;

void printHang(void)
{
	printf("%s@%s:%s%% ", username, distro, pwd);
}

// create this for our hash functions....
// have yet to build the hash funcion this is just the linked list we need for it to work properly

struct varNode {
	char * item;
	char * name;
	struct varNode * next;
	
};
struct varNode * HashTable[64];

// super fucking useful why is this not in normal c?
char compareString(char * string1, char * string2)
{
	int abcCounter = 1;
	char trueForThis = 1;
	while(string1[abcCounter] != '\0' && string2[abcCounter] != '\0' && trueForThis)
	{
		if(string1[abcCounter] != string2[abcCounter])
		{
			trueForThis = 0;
		}
		abcCounter++;
	}
	if(trueForThis)
	{
		if(string1[abcCounter] != string2[abcCounter])
		{
			trueForThis = 0;
		}
	}
	return trueForThis;
}

char * insertString(char * string1, int pos, int chunkOut, char * insertee)
{
	// meh fuck it
	char * tempStr = (char *)malloc(strlen(string1) + strlen(insertee) + 5);
	// copy up to pos to start replacement
	strncpy(tempStr, string1, pos);
	//printf("%d, %d\n", pos, chunkOut);
	//printf("%s\n", tempStr);
	// replace teh final character w/ a \0
	//tempStr[pos] = '\0';
	// then add to the end our insertee
	strcat(tempStr, insertee);
	//printf("%s\n", tempStr);
	// and finally add the rest of the string we had on.
	strcat(tempStr, string1 + pos + chunkOut);
	//printf("%s\n", tempStr);
	return tempStr;
	// credit mingos :: http://stackoverflow.com/questions/2015901/inserting-char-string-into-another-char-string ::
}

void addNode(char * name, char * item, struct varNode * current)
{
	
	if(current->name == NULL)
	{
		//printf("%s, %s\n", name, item);
		//printf("current == NULL");
		//current->name = (char*)malloc(sizeof(char *));
		current->name = name;
		//current->item = (char*)malloc(sizeof(char *));
		current->item = item;
		//printf("\n%s, %s\n", current->name, current->item);
		current->next = NULL;
	}
	else
	{
		if(compareString(current->name, name))
		{
			printf("overwriting...\n");
			current->item = item;
			return;
		}
		while(current->next != NULL)
		{
			current = current->next;
			if(compareString(current->name, name))
			{
				printf("overwriting....\n");
				current->item = item;
				return;
			}
		}
		current->next = (struct varNode *)malloc(sizeof(struct varNode)) ;
		current = current->next;
		current->name = name;
		current->item = item;
	}
}

int hashThing(char * thingToHash)
{
	return (int)thingToHash[0] % 64;
}

void list()
{
	int i;
	for(i = 0; i < 64; i++)
	{
		struct varNode * current = HashTable[i];
		if(current->name == NULL)
		{
			// do nothing
		}
		else
		{
			current = HashTable[i];
			while(current != NULL)
			{
				printf("%s:%s\n", current->name, current->item);
				current = current->next;
			}
		}
	}
}

char * find(char * string)
{
	struct varNode * current = HashTable[hashThing(string)];
	if(current->name == NULL)
	{
		
	}
	else
	{
		//printf("in the else");
		fflush(stdout);
		while(current != NULL)
		{
			if(compareString(string, current->name))
			{
				return current->item;
			}
			current = current->next;
		}
	}
	return "\0";
}

int main(int argc, char **argv)
{
	distro = malloc(65);
	getdomainname(distro, 65);
	//free(temp);
	username = getlogin();
	// have to generate our table of user stored values
	int neverUsedAgain;
	for(neverUsedAgain = 0; neverUsedAgain < 64; neverUsedAgain ++)
	{
		HashTable[neverUsedAgain] = (struct varNode *)malloc(sizeof(struct varNode));
		HashTable[neverUsedAgain]->next = NULL;
		HashTable[neverUsedAgain]->name = NULL;
		HashTable[neverUsedAgain]->item = NULL;
	}
	
	char * input;
	input = NULL;
	int n; // ni is just for getline pointer
	int i; // is the iterator last char retrieved
	for(i=0; i<argc; i++)
	{
		if(strncmp(argv[i], "-h", 3) == 0 || strncmp(argv[i], "--help", 5) == 0)
		{
			PrintHelp();
			exit(0);
		}	
	}
	int last; // is the last chop placeholder for commands / tokens
	pwd = "asdfasdfa";
	pwd = get_current_dir_name();
	printHang();
	while(getline(&input, &n, stdin))
	{
		int size;
		last = 0;
		i = 0;
		char * * tempTotal;
		//unsigned int ** pointersToUtility;
		//pointersToUtility = ( int **)malloc(sizeof(int[3]) * strlen(input)/2 + 1);
		//pointersToUtility[0] = malloc(sizeof(int));
		//pointersToUtility[0][0] = 0;
		// the 0th character is a number to where we are as long as !>256 we should be fine
		// this should be resolved by next version...
		// we also size to + 1 because of the fact that we want to put a ; at the very end of things. 
		size = strlen(input) + 2;
		//
		tempTotal = (unsigned char **)malloc(sizeof(char *) * size);
		tempTotal[0] = malloc(sizeof(char));
		tempTotal[0][0] = '\0';
		#ifndef addEntry
			#define addEntry() tempTotal[0][0]++; tempTotal[(int)tempTotal[0][0]] = malloc(sizeof(char) * ( i - last)); strcpy(tempTotal[(int)tempTotal[0][0]], (input + sizeof(char) * last)); tempTotal[(int)tempTotal[0][0]][i-last] = '\0'; // printf("0,0: %d; thing: %s\n", (unsigned int)tempTotal[0][0], tempTotal[(int)tempTotal[0][0]]);
		#endif
		// this handles every single character, 1 by 1.
		int waitingOnQuote = 0;
		while(input[i] != '\0' && (int)tempTotal[0][0] < size - 1)
		{
			
			char *tempStr;
			
			// it is whitespace
			if(input[i] == '$')
			{
				char * replacementString;
				int x = i;
				int x2 = i;
				x++;
				int beginingCharacter = 1;
				while(input[x] != '\0'&& input[x] != ' ' && input[x] != '\t' && input[x] != '\n' && input[x] != '$')
				{
					//printf("%d\n", x);
					fflush(stdout);
					x++;
				}
				replacementString = (char *)malloc(sizeof(char) * (x - (i) + 1));
				//printf("malloc worked %d\n", (x - i));
				fflush(stdout);
				strcpy(replacementString, (input + sizeof(char) * i + 1));
				replacementString[(x-(i) - 1)] = '\0';
				fflush(stdout);
				//printf("%s, REPLACMENT", replacementString);
				fflush(stdout);
				char * found = find(replacementString);
				if(found[0] != '\0')
				{
					found[(int)(strlen(found))] = '\0';
					//printf("\n%s, FOUND\n", found);
					input = insertString(input, i, x-i, found);
					//printf("input: %s|\n", input);
					i--;
				}
				
			}
			if(input[i] == '\\' && input[i] != '\0')
			{
				i++;
			}
			if(input[i] == '"')
			{
				// we're waiting.
				if(waitingOnQuote == 1)
				{
					waitingOnQuote = 0;
				}
				else
				{
					waitingOnQuote = 1;
					//printf("waiting on quote now\n");
				}
			}
			if((input[i] == ' ' || input[i] == '\t' || input[i] == '\n') && waitingOnQuote != 1)
			{
				// destroy the heathens
				if(i == last)
				{
					//input[i] = '\0';
				}
				else
				{	
					
					addEntry();
					// create a new thing with malloc for our stuff
					//tempStr = malloc((i - last) * sizeof(char));
					//// then copy that to tmpStr 
					//strcpy(tempStr, input + sizeof(char) * last);
					//// the empty character we add for printf removes the final white space
					//tempStr[i-last] = '\0';
					//// print out everything b4 the stuff followed by a new line
					//printf("i = %d; last = %d : %s\n", i, last, tempStr);
					// last has to equal i + 1 to remove the character we just hit, sort of counter logic
				}
				// add one anyways because that's the next viable character and it makes destroying the heathens easier
				last = i + 1;
			}
			else if(input[i] == '|' || input[i] == ';' || input[i] == '>' || input[i] == '<') // this line is used to decide what special characters we break tokens around
			{
				
				// test if the last character was whitespace
				//usually we would use add entry @ whitespace to break things "hi hi"
				// call @ the whitespace
				// but here we have to find if we have | or ;
				// so we have to increment 1 to succesfully break
				// last = i + 1 to remove the character from being double counted
				// it's only a little bit confusing
				// the only issue with this is if they stack || or ;; or something to that sort, really ;; shouldn't crash us though
				if(i > 1)
				{
					// these are things which would trigger a ADDENTRY so we have to use a special case for them. added > to this so that it'll work generally
					if(input[i - 1] == ' ' || input[i-1] == '\t' || input[i-1] == '\n' || input[i-1] == '>' || input[i-1] == ';' || input[i-1] == '|')
					{
						i = i +1;
						addEntry();
						i = i - 1;
					}
					else
					{
						// if it wasn't we have to chunk that bit off 
						addEntry();
						last = i;
						i = i + 1;
						addEntry();
						i = i - 1;
					}
					last = i + 1;
				}
			}
			i++;
		}
		// if get too many commands unreasonable... // we're adding one anyways with the ; to force run so we have to count at size - 1?
		if((int)tempTotal[0][0] == size - 1)
		{
			printf("you should write commands that actually exist");
		}
		else
		{
			
			// we have succesfully tokenized, all things assumed
			// using <= here because 0th is the counter
			// this is now working somehow? ? ? 
			//printf("numTokens: %d\n", tempTotal[0][0]);
			// adding a ; at the end of the line so that things auto run.......
			
			tempTotal[0][0]++;
			tempTotal[(int)tempTotal[0][0]] = malloc(2);
			tempTotal[(int)tempTotal[0][0]][0] = ';';
			tempTotal[(int)tempTotal[0][0]][1] = '\0';
			//printf("\ntempTotal[last] = %s\n", tempTotal[(int)tempTotal[0][0]]);
			// create 2 counters
			
			unsigned char iterator = '\0';
			iterator++;
			
			unsigned char iteratorLast = '\0';
			iteratorLast++;
			// by creating this here it should be freed automatically at the end of the loop?
			
			// this counter is used in debugging
			unsigned char commands = '\0';
			commands++;
			
			//printf("\n");
			//printf("command: %d\n", (int)commands);
			
			commands++;
			// use this one to increment when we hit an end pipe
			unsigned char pipeCount = '\0';
			// this one is to check and see if we have gotten another pipe, if we haven't follow normal redirection rules.
			unsigned char pipeCountLast = '\0';
			unsigned char fixPipe = '\0';
			int fds[256][2];
			
			for(;iterator <= tempTotal[0][0]; iterator++)
			{
				if(tempTotal[(int)iterator][0] == '|')
				{
					// can use a char piping to count how many pipes we have?
					pipeCount++;
					//printf("piping %d\n", pipeCount);
				}
				else if(tempTotal[(int)iterator][0] == ';')
				{
					fixPipe++;
				}
				// this means we are on a new command :D
				if(tempTotal[(int)iterator][0] == ';' || tempTotal[(int)iterator][0] == '|')
				{
					//printf("\n");
					//printf("command: %d\n", (int)commands );
					commands++;
					
					
					// this bit is mostly working, try not to break it!!!
					char * * command = malloc(sizeof(char*) * ((int)(iterator - iteratorLast) + 1));
					
					// if we have detected any < or > in this
					// it's a boolean
					char splitting = '\0';
					int x = 0;
					// split it into execvp() able chunks. 
					while(iteratorLast < iterator)
					{
						
						command[x] = tempTotal[iteratorLast];
						//printf("%s ", command[x]);
						// useful interpreter spew-outer.
						//printf("%s, command\n", command[x]);
						fflush(stdout);
						if(command[x][0] == '>' || command[x][0] == '<')
						{
							splitting++;
						}
						iteratorLast++;
						x++;
					}
					//printf("\n");
					// have to use this to get around capturing ';' and '|'
					iteratorLast++;
					// for use w/ execvp
					command[x] = NULL;
					fflush(stdout);
					// strncmp returns 0 if TRUE
					// which is slightly retarded because 0 is false
					// why this hasn't been changed by version 11?
					// who knows.?
					if(command[0] == NULL)
					{
						//printf("caught null command, crisis averted");
					}
					else if(strncmp(command[0], "cd", 3) == 0)
					{
						if(command[1] == NULL || strncmp(command[1], "~", 2) == 0)
						{
							chdir(getenv("HOME"));
						}
						else
						{
							chdir(command[1]);
						}
						pwd = get_current_dir_name();
					}
					else if(strncmp(command[0], "exit", 5) == 0)
					{
						
						exit(0);
					}
					else if(strncmp(command[0], "pwd", 4) == 0)
					{
						printf("%s\n", pwd);
					}
					else if(strncmp(command[0], "help", 5) == 0)
					{
						PrintHelp();
					}
					else if(strncmp(command[0], "set", 4) == 0)
					{
						// i swear i wasn't high when i wrote set, i just forgot how to use string.h momentarily.
						//printf("hit set\n");
						fflush(stdout);
						if(command[1] != NULL)
						{
							int tempInt = hashThing(command[1]);
							if(command[2] != NULL)
							{
								char hit2ndQuotation = 0;
								int tempInt2 = 1;
								//("setting a thing to %s : %d\n", command[2], tempInt);
								if(command[2][0] != '"')
								{
									printf("error excpected a \"");
								}
								else
								{
									
									
									while(command[2][tempInt2] != '\0')
									{
										if(command[2][tempInt2] == '\\' && command[2][tempInt2 + 1] != '\0')
										{
											int tempTempInt2 = tempInt2;
											// covering up the escape character
											while(command[2][tempInt2] != '\0')
											{
												command[2][tempInt2] = command[2][tempInt2 + 1];
												tempInt2++;
											}
											tempInt2 = tempTempInt2;
										}
										else if(command[2][tempInt2] == '"')
										{
											command[2][tempInt2] = '\0';
											command[2][tempInt2 + 1] = '\0';
											hit2ndQuotation = 1;
										}
										tempInt2++;
									}
									
								}
								if(hit2ndQuotation)
								{
									tempInt2 = 1;
									int numberForMalloc = 0;
									// momentarilly forgot about strlen();
									while(command[2][tempInt2] != '\0')
									{
										numberForMalloc++;
										tempInt2++;
									}
									char * tempCommand2 = (char *)malloc(numberForMalloc);
									numberForMalloc = 0;
									tempInt2 = 1;
									while(command[2][tempInt2] != '\0')
									{
										tempCommand2[numberForMalloc] = command[2][tempInt2];
										tempInt2++;
										numberForMalloc++;
									}
									addNode(command[1], tempCommand2, HashTable[tempInt]);
								}
								else
								{
									printf("error didn't hit 2nd Quotation\n");
								}
							}
							else
							{
								printf("no arg found setting %s to \\0\n", command[1]);
								addNode(command[1], "\0", HashTable[tempInt]);
							}
						}
						else
						{
							list();
						}
					}
					else if(strncmp(command[0], "list", 5) == 0)
					{
						list();
					}
					else
					{
						// standard else for single commands
						x = 0;
						// could maybe be added to the upper pipeCountFinder bounds or, you know, whatever.
						if(pipeCount > pipeCountLast && pipeCount != 255) // hard code the stopping point
						{
							pipe(fds[(int)pipeCount]); // create a pipe for each of our little processes.
						}
						x = fork();
						if(x == 0)
						{
							// this is the child
							// if we are fucking with shit...
							if(pipeCount > pipeCountLast)
							{
								// we have to hook up our butt to the pipe
								//printf("pipeCount > pipeCountLast\n");
								// dup our stdout over
								dup2(fds[(int)pipeCount][1], 1);
								// release this too.
								close(fds[(int)pipeCount][1]);
								
								// and we have to hook up our front to the pipe, but not if we're 1st motherfucker.
								if(pipeCountLast != '\0')
								{
									//printf("pipeCountLast != '\0'");
									dup2(fds[(int)pipeCountLast][0], 0);
									// have to also close the pipe b4 this, if we forget this we HANGggggg
									close(fds[(int)pipeCountLast][0]);
									close(fds[(int)pipeCountLast][1]);
								}
								close(fds[(int)pipeCount][0]);
							}
							else if(pipeCount == pipeCountLast && (int)pipeCount > 0)
							{
								//printf("pipeCount == pipeCountLast && pipeCount > 0\n");
								//printf("pipeCountLast: %d\n", (int)pipeCountLast);
								// set our front to eating & nothing else.
								dup2(fds[(int)pipeCountLast][0], 0);
								close(fds[(int)pipeCount][0]);
								close(fds[(int)pipeCount][1]);
								// this would mean that we are at the very end of the pipe chain, and have too hook our front into the pipes.
							}
							if(splitting)
							{
								// this line was redundant
								//x = 0;
								while(command[x] != NULL)
								{
									
									// we only run x++ if we haven't cover'd anything in this loop
									//printf("command: %s\t", command[x]);
									if(command[x][0] == '>')
									{
										if(command[x + 1][0] == '>')
										{
											Cover(command, x);
											freopen(command[x+1], "a+", stdout);
										}
										else
										{
											freopen(command[x+1], "w", stdout);
										}
										
										Cover(command, x);
										// increment and set our thing equal to null as well
										Cover(command, x);
									}
									else if(command[x][0] == '<')
									{
										freopen(command[x+1], "r", stdin);
										// hide the < and the file we're taking in.
										Cover(command, x);
										Cover(command, x);
									}
									else if(command[x][0] == '|')
									{
										x++;
										// do nothing
									}
									else
									{
										x++;
										// do nothing
									}
									
								}
							}
							// the actual execution of our command
							
							execvp(command[0], command);
							
							fprintf(stderr, "command failed execvp: %s\n", command[0]);
							// adding this line somehow removed an error, I REALLY don't get it 
							// error desc: stdin << the \r the \r the \r the \r the \r exit
							// would not exit
							exit(-1);
						}
						else
						{
							// this is the parent
							int status;
							int id;
							if(pipeCountLast == pipeCount && pipeCount != '\0')
							{
								//printf("pipeCount last == pipeCount && pipeCount != '', parent\n");
								// we can close up our FINAL PIPE BOYS
								close(fds[(int)pipeCountLast][0]);
								close(fds[(int)pipeCountLast][1]);
								while(x != wait(&status));
								if(status != 0)
								{
									printf("%d exited with status: %d\n", x, status);
								}
								pipeCount = '\0';
								pipeCountLast = '\0';

							}
							else if(pipeCount > pipeCountLast)
							{
								//printf("pipeCount > pipeCountLast, parent\n");
								// don't wait if we are still expecting pipes.
								// release the one that was hopefully just handed off
								if(pipeCountLast != '\0')
								{
									//printf("pipeCountLast != '', parent\n:%d\n", (int)pipeCountLast);
									close(fds[(int)pipeCountLast][0]);
									close(fds[(int)pipeCountLast][1]);
								}
								pipeCountLast++;
							}
							else
							{
								//printf("else, parent\n");
								while(x != wait(&status));
								if(status != 0)
								{
									printf("%d exited with status: %d\n", id, status);
								}
								pipeCount = '\0';
								pipeCountLast = '\0';
							}
							
							//if(splitting)
							//{
								//// this is the bit where we need to change back to writing the correct way...
								//fflush(stdout);
								//// fsetpos(stdout, NULL);
							//}
							
						}
					}
				}
				else
				{
					//printf("%s ", tempTotal[(int)iterator]);
				}
			}
		}
		//printf("%d : %s", n, input);
		// let us know we're in the techshell
		printHang();
	}
	
	return 0;
}
