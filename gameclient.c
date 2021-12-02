/*
client:
	form client fifo 
	open public server fifo for writing
	send client fifo string to server 
	close public server fifo 
	
	open client fifo for reading (clientfp) 
	read the first line - server fifo 
	open server fifo for writing (serverfp)
	
	while (read a string from server (clientfp))
		display it 
		if string has "The word is"
			we are done!
		get user response 
		send the user response to server (serverfp)

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAXLEN 1000

int main(int argc, char *argv[]) {	
	if (argc !=3) {
		puts("arg issue");
		exit(1);
	}

    char clientfifo[MAXLEN];
	sprintf(clientfifo, "/tmp/raghavpillai-client");
	mkfifo(clientfifo, 0600);
	chmod(clientfifo, 0622);
	
	
	FILE *fp = fopen("/tmp/raghavpillai-server", "w");
	char input;
	scanf("%d", &input);  
	fprintf(fp, "%s\n", input);
	fclose(fp);

	
	FILE *wordfp = fopen("/tmp/raghavpillai-client", "r");
	char line[MAXLEN];
	
	while (fgets(line, MAXLEN, wordfp))
		puts(line);
	unlink(clientfifo);
}