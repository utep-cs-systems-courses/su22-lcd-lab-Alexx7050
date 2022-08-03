#include <msp430.h>
#include <libTimer.h>
#include "switches.h"
#include "lcdutils.h"
#include "lcddraw.h"
#include "buzzer.h"
#include "demo.h"

unsigned char step = 0;

void draw_ball(int col, int row, unsigned short color)
{
  fillRectangle(col-1, row-1, 3, 3, color);
}

short ballDrawPos[2] = {screenWidth/2,screenHeight/2};         // Axis 0 for col, axis 1 for row
short ballControlPos[2] = {1+screenWidth/2, 1+screenHeight};   // Axis 0 for col, axis 1 for row
short ballColVelocity = 1, ballColLim[2] = {1, screenWidth-3}; // -3 because of ball size
short ballRowVelocity = 5, ballRowLim[2] = {1, screenHeight-3};// -3 because of ball size

void screen_update_ball()
{
  for (char axis = 0; axis < 2; axis++) 
    if (ballDrawPos[axis] != ballControlPos[axis]) // position changed?
      goto redraw;
  return;			// nothing to do
 redraw:
  draw_ball(ballDrawPos[0], ballDrawPos[1], COLOR_BLACK);  // erase
  for (char axis = 0; axis < 2; axis++) 
    ballDrawPos[axis] = ballControlPos[axis];
  draw_ball(ballDrawPos[0], ballDrawPos[1], COLOR_WHITE); // draw
}

void bounce_ball(char at_pos)
{
  if (ballControlPos[0]+1 < screenHeight && ballColVelocity > -4)
    ballColVelocity -= 2;
  else if (ballControlPos[0]+1 > ballControlPos[at_pos] && ballColVelocity < 4)
    ballColVelocity += 2;
  ballRowVelocity  = -ballRowVelocity;
}

void position_update_ball()
{
  {   // move ball horizontally
    short oldCol = ballControlPos[0];
    short newCol = oldCol + ballColVelocity;
    if (newCol <= ballColLim[0] || newCol >= ballColLim[1])
      ballColVelocity = -ballColVelocity;
    else
      ballControlPos[0] = newCol;
  }

  {		// move ball vertically
    short oldRow = ballControlPos[1];
    short newRow = oldRow + ballRowVelocity;
    if (newRow <= ballRowLim[0])     // ball in upper row screen edge
      bounce_ball(0);
    else if (newRow >= ballRowLim[1])  // ball in lower row screen edge
      bounce_ball(1);
    else
      ballControlPos[1] = newRow;
  }
}

void draw_ptrn()
{
  buzzer_set_period(0);
  for (unsigned char col = 0; col < screenHeight; col++)
    {
      for(unsigned char row = 0; row < screenWidth; row++)
	      
	      fillRectangle(row, col, 5, 5, COLOR_AQUAMARINE);
// 	{ 
// 	  if(col % 2 == 0)
// 	    {
// 	      fillRectangle(row, col, 5, 5, COLOR_AQUAMARINE);
// 	    }
// 	  else
// 	    fillRectangle(col, row, 5, 5, COLOR_FIREBRICK);
// 	}
    }
}

short redrawScreen = 1;

void wdt_c_handler()
{
  static int secCount = 0;

  secCount ++;
  if (secCount >= 25) {		/* 10/sec */
	  
    {
      if (switches & SW1) position_update_ball();
      if (switches & SW2) buzzer_set_period(1000);
      if (switches & SW3) draw_ptrn();
      if (step <= 30)
	step ++;
      else
	step = 0;
      secCount = 0;
    }
    if (switches & SW4)
    {
       buzzer_set_period(0);
       clearScreen(COLOR_BLACK);
       return;
    }
    redrawScreen = 1;
  }
}

void update_shape()
{
  screen_update_ball();
}
