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
#include <signal.h>
#include <sys/time.h>
#include <assert.h>
int fuelCells;
int masterAngle=90;
float velx=0;
static int g_finished = 0;
static int running= 1;
void handle_timeout( int signal );
FILE* fpointer;
FILE* sketchPad;
struct basicShip rocket[1];
struct land scape[1];
float gravity;
float oldAcc;
const char exec_name[] = "java -jar Sketchpad.jar";
static int hardmode=0;
int  fuelCells=100;
int wind=0;
void error(FILE*sketchPad){
  fprintf(stderr,"Lander line:0 Error\n");
  fprintf(sketchPad,"end");
  pclose(sketchPad);
  exit(1);
}

int main(int argc, char* argv[]){
  int iter;
  int input;
  gravity=-1;
  float thruster;
  thruster=1;

  //FILE* fpointer;
  //FILE* sketchPad;
  sketchPad =popen(exec_name,"w");
  printf("%d\n",argc);
  if (sketchPad == NULL)
    fprintf(stderr, "Could not open pipe %s\n", exec_name);
  for(iter=1; iter<argc; iter+=2 ){
    if (strncmp(argv[iter],"-g",2)==0 ) {
	printf("%s : %s \n",argv[iter],argv[iter+1]);
        gravity=atof(argv[iter+1]);
      }
    else if (strncmp(argv[iter],"-t",2)==0 ) {
	printf("%s : %s \n",argv[iter],argv[iter+1]);
	thruster=atof(argv[iter+1]);
      }
    else if (strncmp(argv[iter],"-f",2)==0 ) {
	printf("%s: %s \n",argv[iter],argv[iter+1]);
	fpointer = fopen(argv[iter+1],"r");
      }
    else if (strncmp(argv[iter],"-i",2)==0 ) {
	printf("Improvements enabled!\n");
	iter-=1;
	hardmode=1;
    }

    else{
      fprintf(stderr,"Lander line:0 Error\n");
      error(sketchPad);
    }
  }
  if( (fpointer==NULL) ||(gravity==-1) ||
      (thruster==1) ) {
    error(sketchPad);
  }
  if( (gravity>20) || (gravity<0) ){
    printf("Gravity must between 0:20\n");
    error(sketchPad);
  }

  if( (thruster>0) || (thruster<-20) ){
    printf("Thrusters must between 0:-20\n");
    error(sketchPad);
  }    
if (hardmode){
      printf("Please enter a windspeed -10:10 (0 is default)\n");
      scanf("%d", &wind);
      velx=wind;
  }
  printf("%s started on ",argv[0]);
  fflush(stdout);
  system("date");
  ncurses();
  printw(" How to Play \n Left Arrow key for Thrust Left \n Right Arrow key for Thrust Right \n Space Bar for vertical thrust \n");
  printw(" Press q to quit\n");
  printw(" Press Any Key\n");
  getch();
  fflush(stdout);

  struct sigaction sigAct;
  landscaper(fpointer, sketchPad,scape);
  intShip(sketchPad, rocket);
  observer(rocket, scape, masterAngle);
  if (hardmode){
      intfuelbar(fuelCells,sketchPad);
  }
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~`
  sigAct.sa_handler = handle_timeout;
  sigemptyset ( &sigAct.sa_mask );
  sigAct.sa_flags=0;
  if( sigaction( SIGALRM, &sigAct, NULL ) < 0 ) {
    exit( EXIT_FAILURE );
  }
  struct itimerval timer;
  timer.it_value.tv_sec = 0;  
  timer.it_value.tv_usec = 50000; 
  // interval between subsequent alarms
  timer.it_interval.tv_sec = 0;  
  timer.it_interval.tv_usec = 50000; 

  if( setitimer( ITIMER_REAL, &timer, NULL ) < 0 ) {
    exit( EXIT_FAILURE );
  }
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  while(!g_finished ){
    while (running) {

      input = getch();

      if(input==ERR){
	//printf("error %p\n",input);
	continue;
      }

      //Simple fuel cell concept
      if (hardmode){
	if (fuelCells==0){
	  printw("\n Warning Out of Fuel\n");
	}
	else if (fuelCells==75)
	  printw("\n75%% fuel remaining\n");
	else if (fuelCells==50)
	  printw("\n50%% fuel remaining\n");
	else if (fuelCells==25)
	  printw("\n25%% fuel remaining\n");
      }

      switch( input ) {
      case KEY_LEFT:
	;//Signal Blockers
	sigset_t block_mask ;
	sigset_t old_mask ;
	sigemptyset( &block_mask );
	sigaddset( &block_mask, SIGALRM );
	if( sigprocmask( SIG_BLOCK, &block_mask, &old_mask ) < 0 ) {
	  exit( EXIT_FAILURE );
	}//signal blocker

	//Reducing Fuel
	if (hardmode){
	  if (fuelCells>0){
	    fuelCells-=1;
	  }
	  else{
	    if( sigprocmask( SIG_SETMASK, &old_mask, NULL ) < 0 ) {
	      exit( EXIT_FAILURE );
	    }//End of Signal Blockers
	    break;
	  }
	}

	erasethrust(sketchPad,rocket);
	eraseship(sketchPad,rocket);
	masterAngle=translating(rocket,1,masterAngle );
	
	if( sigprocmask( SIG_SETMASK, &old_mask, NULL ) < 0 ) {
	  exit( EXIT_FAILURE );
	}//End of Signal Blockers
	break;
      case KEY_RIGHT:
	;//Signal Blockers
	sigemptyset( &block_mask );
	sigaddset( &block_mask, SIGALRM );
	if( sigprocmask( SIG_BLOCK, &block_mask, &old_mask ) < 0 ) {
	  exit( EXIT_FAILURE );
	}
	//Reducing Fuel
	if (hardmode){
	  if (fuelCells>0){
	    fuelCells-=1;
	  }
	  else{
	    if( sigprocmask( SIG_SETMASK, &old_mask, NULL ) < 0 ) {
	      exit( EXIT_FAILURE );
	    }//End of Signal Blockers
	    break;
	  }
	}
	//end of hardmode
	erasethrust(sketchPad,rocket);
	eraseship(sketchPad,rocket);
	masterAngle=translating(rocket,0,masterAngle );

	if( sigprocmask( SIG_SETMASK, &old_mask, NULL ) < 0 ) {
	  exit( EXIT_FAILURE );
	}//End of Signal Blockers
	break;

      case ' ':
	;//Signal Blockers
	sigemptyset( &block_mask );
	sigaddset( &block_mask, SIGALRM );
	if( sigprocmask( SIG_BLOCK, &block_mask, &old_mask ) < 0 ) {
	  exit( EXIT_FAILURE );
	}
	//Reducing Fuel
	if (hardmode){
	  if (fuelCells>0){
	    fuelCells-=1;
	  }
	  else{
	    if( sigprocmask( SIG_SETMASK, &old_mask, NULL ) < 0 ) {
	      exit( EXIT_FAILURE );
	    }//End of Signal Blockers
	    break;
	  }
	}
	//end of hardmode

	drawthrust(sketchPad,rocket);
	oldAcc=thrusting(thruster,gravity,oldAcc,velx,masterAngle);
	velx=horThrusting(thruster,gravity,oldAcc,velx,masterAngle);

	if( sigprocmask( SIG_SETMASK, &old_mask, NULL ) < 0 ) {
	  exit( EXIT_FAILURE );
	}//End of Signal Blockers
	break;

      case 'q':
	running=0;
	g_finished=1;
	//printw("Ending Sim.\n");
	break;

      default:
	break;
    }
    }
    break;
  }

  printw("Press Any Key exit program\n");
  getch();

  shutdown_ncurses();
  fprintf(sketchPad,"end");
  pclose(sketchPad);
  fclose(fpointer);

}

void ncurses(){
  int r;
  // start up the ncurses environment
  //setlocale(LC_ALL, "");
  initscr(); // nothing to check, initscr exits on error
  // don't wait for enter for keyboard input
  r = cbreak(); assert( r != ERR );
  // don't echo keypresses to screen
  r = noecho(); assert( r != ERR );
  r = nonl(); assert( r != ERR );
  // turn cursor off
  r = curs_set( 0 ); assert( r != ERR );
  // don't worry about cursor position
  r = leaveok( stdscr, TRUE ); assert( r != ERR );
  // slower handling of ^C but saner output
  r = intrflush( stdscr, FALSE ); assert( r != ERR );
  // translate movement espace codes into single keys
  r = keypad( stdscr, TRUE ); assert( r != ERR );
}

void shutdown_ncurses()
{
  endwin();
}

void handle_timeout(int signal){ 
  static int called = 0;
  char WinCon;
  // check that we were called because of SIGALRM
  if( signal == SIGALRM ) {
    fflush(sketchPad);
    fflush(stdout);
    called++;
    oldAcc=shipfalling(sketchPad, rocket, gravity, oldAcc, velx);
    drawShip(sketchPad, rocket); 
    removebar(fuelCells,sketchPad);
    WinCon=observer(rocket, scape, masterAngle);
    fflush(sketchPad);
    fflush(stdout);
    if (WinCon=='w'){
      called=5001;
    }
    else if (WinCon=='c'){
      called=5002;
    }

    if( called > 5000 ) {
      struct itimerval timer;
      // turn the timer off by setting the values to 0/0
      timer.it_interval.tv_sec = 0;
      timer.it_interval.tv_usec = 0;
      timer.it_value.tv_sec = 0;
      timer.it_value.tv_usec = 0;
      if( setitimer( ITIMER_REAL, &timer, NULL ) < 0 ) {
	exit( EXIT_FAILURE );
      }

      if (called==5000){
	printw("You ran out of time, Sorry\n");
      }

      else if( called == 5001 ){
	if( (oldAcc<20) && (masterAngle==90) ){
	  printw("You are winnar!\n");
	}
	else{
	  printw("Lander Crashed!\n");
	}
      }
      else if( called == 5002 ){
	printw("Lander Crashed!\n");
      }
      getch();
      printw("Press Any Key exit program\n");
      getch();
      running=0;
      g_finished = 1;
    }
  }
}
