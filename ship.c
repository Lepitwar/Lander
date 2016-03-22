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
#include <ncurses.h>
int booling =1;

int intShip(FILE*sketchPad,struct basicShip * rocket){
  // struct basicShip rocket;
  rocket[0].midX=300;
  rocket[0].midY=10;


  rocket[0].shipx[0]=295;
  rocket[0].shipx[1]=305;
  rocket[0].shipx[2]=310;
  rocket[0].shipx[3]=290;

  rocket[0].shipy[0]=1;
  rocket[0].shipy[1]=1;
  rocket[0].shipy[2]=16;
  rocket[0].shipy[3]=16;

  rocket[0].thrustx[0]=290;
  rocket[0].thrustx[1]=300;
  rocket[0].thrustx[2]=310;
  rocket[0].thrustx[3]=310;

  rocket[0].thrusty[0]=16;
  rocket[0].thrusty[1]=20;
  rocket[0].thrusty[2]=16;
  rocket[0].thrusty[3]=310;

  // drawShip(sketchPad,rocket);
  fflush(sketchPad);
  return(EXIT_SUCCESS);
  }

int checkship(struct basicShip *rocket, float acc,float gravity){
  int i;
  acc=( (acc*.05)+(gravity*0.05*0.05*0.05) );
  for(i=0;i<4;i++){
    if( ( (rocket[0].shipx[i]+acc)<=0)   ||
	( (rocket[0].shipy[i]+acc)<=0)   ||
	( (rocket[0].thrustx[i]+acc)<=0) ||
	( (rocket[0].thrusty[i]+acc)<=0) ||
	( (rocket[0].shipx[i]+acc)>=750)   ||
	( (rocket[0].shipy[i]+acc)>=750)   ||
	( (rocket[0].thrustx[i]+acc)>=750) ||
	( (rocket[0].thrusty[i]+acc)>=750)  ){
      return 0;
  }
  }
  return 1;
}

void drawShip(FILE*sketchPad,struct basicShip *rocket){
  int i;

  for(i=0;i<3;i++){
    fprintf(sketchPad,"drawSegment %ld %ld %ld %ld\n",
	    lround( rocket[0].shipx[i]),
	    lround( rocket[0].shipy[i]),
	    lround( rocket[0].shipx[i+1]),
	    lround( rocket[0].shipy[i+1]) );
  }
  fprintf(sketchPad,"drawSegment %ld %ld %ld %ld\n",
	  lround( rocket[0].shipx[3]),
	  lround( rocket[0].shipy[3]),
	  lround( rocket[0].shipx[0]),
	  lround( rocket[0].shipy[0]) );
  fflush(sketchPad);

  return;
}

void drawthrust(FILE*sketchPad, struct basicShip  * rocket){
  int i;

  for (i=0;i<2;i++){
    fprintf(sketchPad,"drawSegment %ld %ld %ld %ld\n",
	    lround(rocket[0].thrustx[i]),
	    lround(rocket[0].thrusty[i]),
	    lround(rocket[0].thrustx[i+1]),
	    lround(rocket[0].thrusty[i+1]) );
      }   
 fprintf(sketchPad,"drawSegment %ld %ld %ld %ld\n",
	 lround(rocket[0].thrustx[0]),
	 lround(rocket[0].thrusty[0]),
	 lround(rocket[0].thrustx[2]),
	 lround(rocket[0].thrusty[2]) );
 fflush(sketchPad);
 return;
}

void erasethrust(FILE*sketchPad, struct basicShip *rocket){
  int i;


  for (i=0;i<2;i++){
    fprintf(sketchPad,"eraseSegment %ld %ld %ld %ld\n",
	    lround(rocket[0].thrustx[i]),
	    lround(rocket[0].thrusty[i]),
	    lround(rocket[0].thrustx[i+1]),
	    lround(rocket[0].thrusty[i+1]) );
      }   
 fprintf(sketchPad,"eraseSegment %ld %ld %ld %ld\n",
	 lround(rocket[0].thrustx[0]),
	 lround(rocket[0].thrusty[0]),
	 lround(rocket[0].thrustx[2]),
	 lround(rocket[0].thrusty[2]) );

 fflush(sketchPad);
 return;
}

void eraseship(FILE*sketchPad, struct basicShip *rocket){
  int i;

  for(i=0;i<3;i++){
    fprintf(sketchPad,"eraseSegment %ld %ld %ld %ld\n",
	    lround( rocket[0].shipx[i]),
	    lround( rocket[0].shipy[i]),
	    lround( rocket[0].shipx[i+1]),
	    lround( rocket[0].shipy[i+1]) );
  }
  fprintf(sketchPad,"eraseSegment %ld %ld %ld %ld\n",
	  lround( rocket[0].shipx[3]),
	  lround( rocket[0].shipy[3]),
	  lround( rocket[0].shipx[0]),
	  lround( rocket[0].shipy[0]) );
  fflush(sketchPad);
  return;

}


float shipfalling(FILE*sketchPad,struct basicShip * rocket, 
		  float gravity, float oldAcc, float velx){
  if (booling){
    oldAcc=gravity;
    booling=0;
    }
  int i;
  float vel;
  erasethrust(sketchPad,rocket);
  eraseship(sketchPad,rocket);
  vel=(gravity*.05)+oldAcc;

  rocket[0].midY+= (vel*.05)+(gravity*0.05*0.05*0.05) ;
  rocket[0].midX+=  (velx*.05)  ;
  for( i=0;i<4;i++){
    //vertical
    rocket[0].thrusty[i]+= (vel*.05)+(gravity*0.05*0.05*0.05) ;
    rocket[0].shipy[i]+= (vel*.05)+(gravity*0.05*0.05*0.05) ;
    //addition 
    rocket[0].thrustx[i]+= (velx*.05) ;
    rocket[0].shipx[i]+= (velx*.05) ;
    }
  return vel;
}



float thrusting(float thrust,int gravity,
		float vel, float velx, int masterangle ){
  int m;
  if (masterangle!=90){
    m= masterangle * M_PI/180.0;
    vel= (vel + (thrust*sin(m))*0.05);
    //velx+= (thrust*cos(m)*0.05);
  }
  else{
    vel=(thrust*.05)+vel;
  }

  return vel;
}

float horThrusting(float thrust,int gravity,
		   float vel, float velx, int masterangle ){
  int m;
  if( (masterangle==90) || (masterangle==270) ||(masterangle==-90) ){
    //printw("~~%d\n",masterangle);
  }
  else{
    m= masterangle * M_PI/180.0;
    velx+= (thrust*cos(m)*0.05);
    //printw("%d\n",masterangle);
  }

  return velx;

}


void rotating(struct basicShip * rocket, int angle, int i ){
  double angle_radians;
  double X;
  double Y;
  double deltaX;
  double deltaY;
  angle_radians = angle * M_PI / 180.0;

  X=rocket[0].shipx[i];
  Y=rocket[0].shipy[i];
  deltaX=X * cos(angle_radians) - Y * sin(angle_radians);
  deltaY=X * sin(angle_radians) + Y * cos(angle_radians);
  rocket[0].shipx[i]=deltaX;
  rocket[0].shipy[i]=deltaY;

  X=rocket[0].thrustx[i];
  Y=rocket[0].thrusty[i];
  deltaX=X * cos(angle_radians) - Y * sin(angle_radians);
  deltaY=X * sin(angle_radians) + Y * cos(angle_radians);
  rocket[0].thrustx[i]=deltaX;
  rocket[0].thrusty[i]=deltaY;
  

}

int translating(struct basicShip * rocket,int angle, int masterAngle){
  int i;
  if (angle==1){
    angle=-10;
    masterAngle-=10;
  }
  if (angle==0){
    angle=10;
    masterAngle+=10;
  }
  if( (masterAngle==450) || (masterAngle==-270) ){
      masterAngle=90;
    }
  
  for(i=0;i<4;i++){
    //Shifts to 0,0 mid
    rocket[0].shipx[i]-=rocket[0].midX; 
    rocket[0].shipy[i]-=rocket[0].midY;  
    rocket[0].thrustx[i]-=rocket[0].midX; 
    rocket[0].thrusty[i]-=rocket[0].midY; 

    rotating(rocket,angle,i);

    rocket[0].shipx[i]+=rocket[0].midX; 
    rocket[0].shipy[i]+=rocket[0].midY;  
    rocket[0].thrustx[i]+=rocket[0].midX; 
    rocket[0].thrusty[i]+=rocket[0].midY; 

  }
  return masterAngle;
  fflush(stdout);
}

char observer(struct basicShip * rocket,struct land  *scape,int masterangle){
  int i;
  int j;
  double rise;
  double run;
  double ror;
  double maxY;
  //printw("Observer online\n");
  for(i=0;i<scape[0].validCNT;i++){
    if (masterangle==90){
      if( (rocket[0].shipx[2]>scape[0].valid[i][0])&&
	  (rocket[0].shipx[2]<scape[0].valid[i][2]) ){
	if( (rocket[0].shipy[2]>scape[0].valid[i][1]-5)&&
	    (rocket[0].shipy[2]<scape[0].valid[i][1]+5) ){
	      return 'w';
	}//Checking Y 
      }//Checking X 
    }//MasterAngle if
    else{
      if( (rocket[0].shipx[2]>scape[0].valid[i][0])&&
	  (rocket[0].shipx[2]<scape[0].valid[i][2]) ){
	if( (rocket[0].shipy[2]<scape[0].valid[i][1]+10)&&
	    (rocket[0].shipy[2]>scape[0].valid[i][1]) ){
	  return 'w';
	}
	else if( (rocket[0].shipy[1]<scape[0].valid[i][1]+10)&&
		 (rocket[0].shipy[1]>scape[0].valid[i][1]) ){
	  return 'w';
	}
	else if( (rocket[0].shipy[0]<scape[0].valid[i][1]+10)&&
		 (rocket[0].shipy[0]>scape[0].valid[i][1]) ){
	  return 'w';
	}
      }
    }//end of else
  }//end of for loop
  
  //printw("Tac-com online\n");
  for(i=0;i<scape[0].slopeCNT;i++){


    if( (rocket[0].shipx[2]>scape[0].slope[i][5])&&
	(rocket[0].shipx[2]<scape[0].slope[i][6])) {
      maxY=scape[0].slope[i][2];
      if( (rocket[0].shipy[2]>maxY) ||
	  (rocket[0].shipy[0]>maxY)
	  ){

	for(j=0;j<4;j++){
	  rise=rocket[0].shipy[j]-scape[0].slope[i][2];
	  run=rocket[0].shipx[j]-scape[0].slope[i][3];
	  ror=rise/run;
	  if (scape[0].slope[i][4]<0){
	    if(ror<=scape[0].slope[i][4]){
	      return 'c';
	    }
	  }
	  else {
	    if(ror>=scape[0].slope[i][4]){
	      return 'c';

	    }
	  }
	}//End for ship for 
      }//end of Y if
    }//end of X if
  }//end of Scape for loop
      /*
    printw("MaxY:%lf X:%lf MinY:%lf X:%lf Slope:%lf\n",
	   scape[0].slope[i][0],
	   scape[0].slope[i][1],
	   scape[0].slope[i][2],
	   scape[0].slope[i][3],
	   scape[0].slope[i][4]); */
  return 'n';
}


void intfuelbar(int fuel, FILE* sketchPad){
  int i;
  for(i=0;i<=fuel;i++){
    fprintf(sketchPad,"drawSegment %d 0 %d 5 \n",i,i);
  }
  return;
}

void removebar(int fuel, FILE* sketchPad){
  int i;
  for(i=100;i>=fuel;i--){
    fprintf(sketchPad,"eraseSegment %d 0 %d 5 \n",i,i);
  }
  return;
}