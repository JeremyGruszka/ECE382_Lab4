#include <msp430g2553.h>
#include "lab4.h"

extern void init();
extern void initNokia();
extern void clearDisplay();
extern void drawBlock(unsigned char row, unsigned char col, unsigned char color);

#define		TRUE			1
#define		FALSE			0

//defines the createBall method
ball_t createBall(int xPos, int yPos, int xVel, int yVel)
{
	ball_t	ballToMove;
	ballToMove.xPos = xPos;
	ballToMove.yPos = yPos;
	ballToMove.xVel = xVel;
	ballToMove.yVel = yVel;
	return ballToMove;
}

void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	unsigned char	x, y, z;

	init();
	initNokia();
	clearDisplay();

	ball_t myBall = createBall(2,2,1,1); //create a ball object

	while(1)
	{
		//These 4 if statements check to see if the ball hits a wall
		if((myBall.xPos * 8) >= 88) //right wall
		{
			myBall.xVel *= -1;
		}
		if((myBall.xPos * 8) <= 0) //left wall
		{
			myBall.xVel *= -1;
		}
		if((myBall.yPos * 8) >= 56) //bottom wall
		{
			myBall.yVel *= -1;
		}
		if((myBall.yPos * 8) <= 0) //top wall
		{
			myBall.yVel *= -1;
		}
		//the next two statments move the ball according to the ball velocity
		myBall.xPos += myBall.xVel;
		myBall.yPos += myBall.yVel;
		x = myBall.xPos;
		y = myBall.yPos;
		z = 1;
		clearDisplay();
		drawBlock(y,x,z);
		int j,k;
		//wait function to slow down ball movement
		for(j = 0; j < 8500; j++)
		{
			for(k = 0; k < 100; k++);
		}
	}
}
