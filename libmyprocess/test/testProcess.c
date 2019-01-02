
// -*- coding: utf-8 -*-

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_NB_FILS  5
#define TAB_PID_SIZE MAX_NB_FILS
#define NB_PROCESS   20

int main(int argc, char* argv[]) {

  int idxProcessFils = 0;
  int status         = 0;
  int pidFils        = 0;
  int countPidFils   = 0;
  int idxPidTmp      = 0;

  int tabPidsFils[TAB_PID_SIZE];

  memset(tabPidsFils, 0, sizeof(tabPidsFils));

  for(idxProcessFils = 0; idxProcessFils < NB_PROCESS; idxProcessFils++) {

    if (countPidFils < MAX_NB_FILS) {
      
      //fprintf(stderr, "countPidFils %d / MAX_NB_FILS %d\n", countPidFils, MAX_NB_FILS);

      pidFils = fork();
      switch(pidFils) {
      case 0: // Fils
	fprintf(stderr, "\tProcessus fils (count = %d, PID = %d)...\n", idxProcessFils, getpid());
	sleep(idxProcessFils * 2);
	fprintf(stderr, "\tFin de processus fils (count = %d, PID = %d)\n", idxProcessFils, getpid());
	return 0;
      case -1: // Erreur
	fprintf(stderr, "Fork impossible (count = %d)\n", idxProcessFils);
	return 1;
      default: // Père
	for(idxPidTmp = 0; idxPidTmp < TAB_PID_SIZE; idxPidTmp++) {
	  if(tabPidsFils[idxPidTmp] == 0) {
	    tabPidsFils[idxPidTmp] = pidFils;
	    fprintf(stderr, "Processus père (count = %d, PID fils = %d, idxPidTmp = %d)\n", idxProcessFils, pidFils, idxPidTmp);
	    countPidFils++;
	    break;
	  }
	}
	if(idxPidTmp >= TAB_PID_SIZE) {
	  fprintf(stderr, "Pas de PID stockable (count = %d) !\n", idxProcessFils);
	  return 0;
	}
      }
    } else {
      fprintf(stderr, "Attente d'une fin de fils (count = %d)...\n", idxProcessFils);
      pidFils = waitpid(-1, &status, 0);
      switch(pidFils) {
      case 0:
	fprintf(stderr, "Sortie de waitpid à z�ro (WNOHANG sans processus fils)\n");
	break;
      case -1:
	fprintf(stderr, "Sortie de waitpid à -1 (erreur)\n");
	break;
      default:
	fprintf(stderr, "Fin du processus fils %d détectée par le père\n", pidFils);
	for(idxPidTmp = 0; idxPidTmp < TAB_PID_SIZE; idxPidTmp++) {
	  if(tabPidsFils[idxPidTmp] == pidFils) {
	    tabPidsFils[idxPidTmp] = 0;
	    fprintf(stderr, "Fin de processus attendue (count = %d, PID fils = %d, idxPidTmp = %d, OK? = %d)\n", idxProcessFils, pidFils, idxPidTmp, WIFEXITED(status));
	    countPidFils--;
	    break;
	  }
	}
	break;
      }
    }
  }

  fprintf(stderr, "===== Attente de la fin des derniers processus fils...\n");
  while((pidFils = waitpid(-1, &status, 0)) > 0) {
    fprintf(stderr, "Fin du processus fils %d détectée par le père\n", pidFils);
    for(idxPidTmp = 0; idxPidTmp < TAB_PID_SIZE; idxPidTmp++) {
      if(tabPidsFils[idxPidTmp] == pidFils) {
	tabPidsFils[idxPidTmp] = 0;
	fprintf(stderr, "Fin de processus attendue (count = %d, PID fils = %d, idxPidTmp = %d, OK? = %d)\n", idxProcessFils, pidFils, idxPidTmp, WIFEXITED(status));
	countPidFils--;
	break;
      }
    }
  }

  return 0;
}

