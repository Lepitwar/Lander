#ifndef LANDER_H
#define LANDER_H


#define LINE_LEN 257
#define PROG_LEN 129
#define IMG_LEN 20

struct land
{
  int lineCNT;
  long  coordX[IMG_LEN+1];
  long  coordY[IMG_LEN+1];
  long 	valid[IMG_LEN+1][4];
  int  validCNT;	
  double  slope[IMG_LEN+1][7];
  int  slopeCNT;
};
struct basicShip
{
  double midX;
  double midY;
  double shipx[4];
  double shipy[4];
  double thrustx[4];
  double thrusty[4];
};



/*Records the LandScape*/
extern void landscaper(FILE * fpointer,FILE*sketchPad, struct land  *scape);
/* Draws the Landscape*/
extern void printLand(FILE*sketchPad, struct land  *scape);
/*intiates the ship*/
extern int intShip(FILE*sketchPad,struct basicShip *rocket);
/*Draws the ship*/
extern void drawShip(FILE*sketchPad,struct basicShip *rocket);
/*Starts up the ncurse*/
extern void ncurses();
/*Kills the ncurses*/
extern void shutdown_ncurses();
/*Draws the thruster*/
extern void drawthrust(FILE*sketchPad, struct basicShip *rocket);
/*Erases the thruster*/
extern void erasethrust(FILE*sketchPad, struct basicShip *rocket);
/*Erases the ship*/
extern void eraseship(FILE*sketchPad, struct basicShip *rocket);
/*Gravity and movement*/
extern float shipfalling(FILE*sketchPad,struct basicShip *rocket, float gravity, float oldAcc, float velx);
/*Thruster power*/
extern float thrusting(float thrust,int gravity,float vel, float velx, int masterangle );
/*Checks if the ship is with in the sketchpad bounds*/
extern int checkship(struct basicShip *rocket, float acc, float gravity);
/*Runs this function everytime the time interval is achieved*/
extern void handle_timeout(int signal);
/*Rotation*/
extern int translating(struct basicShip * rocket, int angle, int masterAngle);	
/*Horz acc.*/
extern float  horThrusting(float thrust,int gravity,float vel, float velx, int masterangle );
/*Finds the flat land and the sloped land*/
extern void veldt(struct land  *scape);
/*Line collision*/
extern char observer(struct basicShip * rocket,struct land  *scape, int masterangle );
/*prints fuel bar*/
extern void intfuelbar(int fuel, FILE* sketchPad);
/*Removes fuel Cells*/
extern void removebar(int fuel, FILE* sketchPad);
#endif