/*

server:
	open dictionary file and read all the words
	open main pipe
	while (fgets()) { //read new client fifo
		fork() to create child to process this client 
		create new server fifo 
		open client fifo for writing  (clientfp)
		send new server fifo to client 
		open new server fifo for reading (serverfp)
		
		select a random word from dictionary array
		form the starred word
		while (1) {
			//fprintf(clientfp, "Enter a letter in %s\n", starredWord);
			send the display line to client (write to clientfp)  
			read response from client (read from serverfp) 
			process!
			//if all chars are OPEN, fprintf(clientfp, "The word is %s", word);
		}	
	}


*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAXWORDS 10000
#define MAXLEN 1000

char *words[MAXWORDS];
int numWords=0;

int main() {
	char line[MAXLEN];

	FILE *fp = fopen("dictionary.txt", "r");
	if (!fp) {
		puts("dictionary.txt cannot be opened for reading.");
		exit(1);
	}
	int i=0;

	while (fgets(line, MAXLEN, fp)) {
		words[i] = (char *) malloc (strlen(line)+1);
		strcpy(words[i], line);
		i++;
	}
	numWords = i;

	srand(getpid() + time(NULL) + getuid());

	char fifo[MAXLEN];
	sprintf(fifo, "/tmp/%s-%d", getenv("USER"), getpid());
	mkfifo(fifo, 0600);
	chmod(fifo, 0622);
	printf("reqs to %s\n", filename);
	
	if (fork()) {
		FILE *fp = fopen(fifo, "w");
		for(int i=1; i<=100; i++) {
			fprintf(fp, "Line %d\n", i);
			fflush(fp);
			sleep(1);
		}
		fclose(fp);
		puts("Parent finished.");
	} else {
		FILE *fp;
		while (fp = fopen(fifo, "r")) {
			puts("Pipe opened.");
			char buffer[1000];
			while (fgets(buffer, 1000, fp))
				puts(buffer);
			fclose(fp);
			puts("Pipe closed.");
		}
		puts("Child finished.");
	}

	unlink(fifo);


	while (1) {
		FILE *fp = fopen(filename, "r");
		if (!fp) {
			printf("FIFO %s cannot be opened for reading.\n", filename);
			exit(2);
		}
		printf("Opened %s to read...\n", filename);
		while (fgets(line, MAXLEN, fp)) {
			char *cptr = strchr(line, '\n');
			if (cptr) 
				*cptr = '\0';

			FILE *clientfp = fopen("/tmp/raghavpillai-client", "w");
			printf("%s: ", line);
			if (clientfp) {
				int wordNumber = rand() % numWords;
				printf("%s", words[wordNumber]);
				fputs(words[wordNumber], clientfp);
				fclose(clientfp);
			}
			putchar('\n');
		}
		fclose(fp);
	}
}
