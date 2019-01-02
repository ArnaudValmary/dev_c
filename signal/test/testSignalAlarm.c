
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

// mon handler
void monhandler(int signal) {
  printf("Début de mon handler\n");
  switch(signal)
  {
    case SIGALRM :
      printf("-> Signal SIGALRM recu %d\n",signal);
      break;
    default :
      printf("-> Signal %d non géré\n",signal);
      break;
  }
  printf("Fin de mon handler\n");
}

int setSignal() {
  struct sigaction action;
  memset(&action, 0, sizeof(struct sigaction));
  action.sa_handler = monhandler; 
  if (sigemptyset(&action.sa_mask) != 0) {
    fprintf(stderr, "Erreur sur 'sigemptyset'\n");
    return 1;
  }
  if (sigaddset(&action.sa_mask, SIGALRM) != 0) {
    fprintf(stderr, "Erreur sur 'sigaddset' pour SIGALRM\n");
    return 1;
  }
  if (sigaction(SIGALRM, &action, NULL) != 0) {
    sigemptyset(&action.sa_mask);
    sigdelset(&action.sa_mask, SIGALRM);
    fprintf(stderr, "Erreur d'affectation du handler du signal SIGALRM par 'sigaction'\n");
    return 1;
  }
  return 0;
}

void setAlarm(int seconds) {
  int nbReste = 0;
  if (seconds > 0) {
    printf("Alarme posée à %d secondes\n", seconds);
    nbReste = alarm(seconds);
  } else {
    printf("Alarme désactivée\n");
    nbReste = alarm(0);
  }
  printf("Il restait %d seconde(s)\n", nbReste);
}

int resetSignal() {
  struct sigaction action;
  memset(&action, 0, sizeof(struct sigaction));
  action.sa_handler = monhandler;
  if (sigemptyset(&action.sa_mask) != 0) {
    fprintf(stderr, "Erreur sur 'sigemptyset'\n");
    return 1;
  }
  if (sigaddset(&action.sa_mask, SIGALRM) != 0) {
    fprintf(stderr, "Erreur sur 'sigaddset' pour SIGALRM\n");
    return 1;
  }
  if (sigaction(SIGALRM, &action, NULL) != 0) {
    fprintf(stderr, "Erreur d'annulation d'interception du signal SIGALRM par 'sigaction'\n");
    return 1;
  }
  return 0;
}

int main(void) {

  if (setSignal()) {
    fprintf(stderr, "Sortie anormale du programme\n");
    return 1;
  }
  setAlarm(3);

  printf("Je dors 3600 secondes...\n");

  sleep(1);
  setAlarm(5); // Réarmement
  sleep(3599);

  printf("Après le repos\n");

  resetSignal();
  setAlarm(0);

  //setAlarm(4);
  //sleep(5);

  printf("Fin normale du programme.\n");

  return 0;
}
