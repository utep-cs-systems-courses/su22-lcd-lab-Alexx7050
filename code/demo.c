#include <msp430.h>
#include <libTimer.h>
#include "switches.h"
#include "screen.h"
#include "lcdutils.h"
#include "lcddraw.h"

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

void bounce_ball(char at_bar)
{
  //If ball doesn't hit the center of the bar, make the ball move two pixels faster to that direction where the ball hit the bar
  if (ballControlPos[0]+1 < barControlPos[at_bar] && ballColVelocity > -4)
    ballColVelocity -= 2;
  else if (ballControlPos[0]+1 > barControlPos[at_bar] && ballColVelocity < 4)
    ballColVelocity += 2;
  ballRowVelocity  = -ballRowVelocity;
}

char score[2] = {'0','0'};
char prev_score[2] = {'1','1'};

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
    if (newRow <= ballRowLim[0]) {      // ball in upper row screen edge
      if (ballControlPos[0]+2 < barControlPos[0]-5 || ballControlPos[0] > barControlPos[0]+8)
        score[1]++;
      else
        bounce_ball(0);
    }
    else if (newRow >= ballRowLim[1]) {  // ball in lower row screen edge
      if (ballControlPos[0]+2 < barControlPos[1]-5 || ballControlPos[0] > barControlPos[1]+8)
        score[0]++;
      else
        bounce_ball(1);
    }
    else
      ballControlPos[1] = newRow;
  }
}