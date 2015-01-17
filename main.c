#include "signalmap.h"

#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

char buffer[1025];

inline int min(int a, int b) {
	if (a < b) return a;
	return b;
}

int main(int argc, char** argv) {
	if (argc<2) return 2;
	int in = open(argv[1],O_RDONLY);
	if (in<0) {
        if (errno == ENOENT) 
            return 0; // OK if the file doesn't exist.
        perror("huh?");
        return 3;
    }
	ssize_t amount = read(in,buffer,1024);
	close(in);
	buffer[amount] = '\0';
	char* error = NULL;
	int process = strtol(buffer,&error,10);
	if(error == buffer)
	  return 4;
	int i = 0;
    int signal = signalstr(getenv("signal"));
    if(signal<1)
        signal = SIGTERM;

    int signals[] = {signal,signal,SIGINT,SIGQUIT,SIGKILL};
    
    kill(process,SIGCONT);
	for(i=0;i<sizeof(signals)/sizeof(int);++i) {
	  kill(process,signals[i]);
      if(0 != kill(process,0))
          break;
	  sleep(1);
	  if(0!=kill(process,0))
	    break;
	  else if(++i > 0x10) 
	    return 5;
      printf("Failed to kill %d trying again\n",process);
	}

	unlink(argv[1]);
	return 0;
}
