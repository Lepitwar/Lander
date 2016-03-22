/*
# name:               Dylan Waters
# ONE Card number:    1343144
# Unix id:            Dwaters
# lecture section:    A2
# lab section:        D02
# TA's name:          Juehui Fan
*/          

#define _XOPEN_SOURCE
#define _BSD_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "lander.h"
#include <ctype.h>
#include <curses.h>
#include <ncurses.h>
/*Draws the landscape*/
void landscaper(FILE * fpointer,FILE*sketchPad, struct land  *scape){
  char singleline[LINE_LEN+1];
  long  X;
  long  Y;
  int lineCNT;
  lineCNT=0;
  while(fgets(singleline,LINE_LEN,fpointer)!=NULL) {
    char * pointer = singleline;
    while (isblank(*pointer)){
      pointer++;//to ignore blank areas
    }
    if (sscanf(pointer,"%ld %ld", &X, &Y)==2){
      scape[0].coordX[lineCNT]=X;
      scape[0].coordY[lineCNT]=Y;
      scape[0].lineCNT=lineCNT;
      lineCNT++;

	}
  }
  printLand(sketchPad, scape);
  veldt(scape);
  return;
}

void printLand(FILE*sketchPad, struct land  *scape){
  int i;
  for( i=0; i<scape[0].lineCNT;i++){
    fprintf(sketchPad,"drawSegment %ld %ld %ld %ld \n",scape[0].coordX[i],
	   scape[0].coordY[i],
	   scape[0].coordX[i+1],
	   scape[0].coordY[i+1] );

  }
  return;
}

void veldt(struct land  *scape){
  int count= 0;
  int scount=0;
  int i;
  double ror;
  double rise;
  double run;
  for( i=0; i<scape[0].lineCNT;i++){
    if  (scape[0].coordY[i]== scape[0].coordY[i+1]){
      //printw("Valid:");
      scape[0].valid[count][0]=scape[0].coordX[i];
      scape[0].valid[count][1]=scape[0].coordY[i];
      scape[0].valid[count][2]=scape[0].coordX[i+1];
      scape[0].valid[count][3]=scape[0].coordY[i+1];
      count+=1;
    }
    else{
      //Max Y, it's X, Min Y, it's X, slope, 
      if (scape[0].coordY[i]>scape[0].coordY[i+1]){
	scape[0].slope[scount][0]=scape[0].coordY[i];
	scape[0].slope[scount][1]=scape[0].coordX[i];
	scape[0].slope[scount][2]=scape[0].coordY[i+1];
	scape[0].slope[scount][3]=scape[0].coordX[i+1];
	//slope of the edge
	rise=scape[0].coordY[i]-scape[0].coordY[i+1];
	run= scape[0].coordX[i]-scape[0].coordX[i+1];
	ror= rise/run;
	scape[0].slope[scount][4]=ror;
      }
      else{
	scape[0].slope[scount][0]=scape[0].coordY[i+1];
	scape[0].slope[scount][1]=scape[0].coordX[i+1];
	scape[0].slope[scount][2]=scape[0].coordY[i];
	scape[0].slope[scount][3]=scape[0].coordX[i];
	//slope of the edge
	rise= scape[0].coordY[i+1]-scape[0].coordY[i];
	run=scape[0].coordX[i+1]-scape[0].coordX[i];
	ror= rise/run;
	scape[0].slope[scount][4]=ror;
      }//End of if statement
      scape[0].slope[scount][5]=scape[0].coordX[i];
      scape[0].slope[scount][6]=scape[0].coordX[i+1];
      scount+=1;
    }//End of Else statement
  }//End of For loop
 scape[0].validCNT=count;
 scape[0].slopeCNT=scount;
  return;
}
