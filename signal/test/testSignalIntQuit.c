
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

// variables globales

int nbINT;
struct sigaction action;


// mon handler

void monhandler(int signal)
{
	switch(signal)
	{
		case SIGINT :
			printf("Signal SIGINT recu %d\n",signal);
			nbINT++;
			if(nbINT == 3)
			{
				action.sa_handler = SIG_DFL;
				sigaction(SIGINT,&action,NULL);
			}
			break;
		case SIGQUIT :
			printf("Signal SIGQUIT recu %d\n",signal);
	}
	printf("Fin de mon handler\n");
}


// fonction principale

int main()
{
	nbINT = 0;

	action.sa_handler = monhandler;		// preparation de la struct sigaction

	sigemptyset(&action.sa_mask);		// masque vide pour SIGQUIT
	sigaction(SIGQUIT,&action,NULL);	// association de mon handler au signal SIGQUIT

	sigaddset(&action.sa_mask,SIGQUIT);	// SIGQUIT masque pour SIGINT
	sigaction(SIGINT,&action,NULL);		// association de mon handler au signal SIGINT

	while(1) sleep(1);			// ne rien faire !!!!!

        return 0;
}