ECE382_Lab4
===========

##Lab4

####Objective

Learn to interface C code, assembly code, and the MSP430 to create images and even games on the nokia display

####Prelab

Handed in in class

####Code

```
extern void init();
extern void initNokia();
extern void clearDisplay();
extern void drawBlock(unsigned char row, unsigned char col, unsigned char color);

#define		TRUE			1
#define		FALSE			0
#define		UP_BUTTON		(P2IN & BIT5)
#define		DOWN_BUTTON		(P2IN & BIT4)
#define		AUX_BUTTON		(P2IN & BIT3)
#define		LEFT_BUTTON		(P2IN & BIT2)
#define		RIGHT_BUTTON	(P2IN & BIT1)


void main()
{

	unsigned char	x, y, z, button_press;

	// === Initialize system ================================================
	IFG1=0; /* clear interrupt flag1 */
	WDTCTL=WDTPW+WDTHOLD; /* stop WD */
	button_press = FALSE;


	init();
	initNokia();
	clearDisplay();
	x=4;
	y=4;
	z = 1;
	drawBlock(y,x,z);

	while(1)
	{

			if (UP_BUTTON == 0)
			{
				while(UP_BUTTON == 0);
				if (y>=1) y=y-1;
				button_press = TRUE;
			}
			else if (DOWN_BUTTON == 0)
			{
				while(DOWN_BUTTON == 0);
				if (y<=6) y=y+1;
				button_press = TRUE;
			}
			else if (LEFT_BUTTON == 0)
			{
				while(LEFT_BUTTON == 0);
				if (x>=1) x=x-1;
				button_press = TRUE;
			}
			else if (RIGHT_BUTTON == 0)
			{
				while(RIGHT_BUTTON == 0);
				if (x<=10) x=x+1;
				button_press = TRUE;
			}

			if(AUX_BUTTON == 0)
			{
				while(AUX_BUTTON == 0);
				if(z == 1)
				{
					z = 0;
				}
				else
				{
					z = 1;
				}
			}

			if (button_press)
			{
				button_press = FALSE;
				//clearDisplay();
				drawBlock(y,x,z);
			}
		}
}
```
This section of code contains the required functionality.  In the beginning of the code it sets up variables and pulls functions from external sources. After the first block is drawn to the screen, the code goes through an infinite while loop to see if buttons are pressed.  If any of the directional buttons are pressed, the nokia display will draw a new box in the direction chosen without erasing the previous box.  If the auxillary button is pressed, the drawBlock function will invert and draw a white box to the nokia display.  In this manner, the program creates an etch-a-sketch type game. I changed the drawBlock function in main.c to have 3 input variables, with the third one being the color variable. I also commented out the clearDisplay() call in the final if statement of the main.c program. I then added R14 to the drawBlock function to account for the color variable.

```
#include <msp430g2553.h>
#include "lab4.h"
extern void init();
extern void initNokia();
extern void clearDisplay();
extern void invertDisplay();
extern void drawBlock(unsigned char row, unsigned char col, unsigned char color);
#define TRUE 1
#define FALSE 0
#define AUX_BUTTON (P2IN & BIT3)
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
// === Initialize system ================================================
IFG1=0; /* clear interrupt flag1 */
WDTCTL=WDTPW+WDTHOLD; /* stop WD */
unsigned char	x, y, z, button_press;
button_press = FALSE;
z = 1;
init();
initNokia();
clearDisplay();
ball_t myBall = createBall(2,2,1,1); //create a ball object
while(1)
{
//the first two if statements are to watch for a SW3 button press to invert the screen
if(AUX_BUTTON == 0)
{
button_press = TRUE;
while(AUX_BUTTON == 0);
if(z == 1)
{
z = 0;
}
else
{
z = 1;
}
}
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
if (z == 1)
{
clearDisplay();
}
else
{
invertDisplay();
}
drawBlock(y,x,z);
int j,k;
//wait function to slow down ball movement
for(j = 0; j < 8500; j++)
{
for(k = 0; k < 100; k++);
}
}
}
```
This section of contains B functionality.  It creates a ball object, and moves the ball around the screen according to the velocity vectors defined in the creation of the ball.  It accounts for the ball hitting the walls of the nokia display and changes the direction of the ball movement when the ball hits a wall.  This code also contains code for one of the optional functionalities, but more on that later.  I apologize for the uglyness of this code, but I explain my reasoning in the debugging/testing portion of the readme.  I also had a header file, which can be viewed in the collection of files in my lab 4 profile.

```
drawBlock:
push	R5
push	R12
push	R13
push R14
rla.w	R13	; the column address needs multiplied
rla.w	R13	; by 8in order to convert it into a
rla.w	R13	; pixel address.
call	#setAddress	; move cursor to upper left corner of block
mov	#1, R12
cmp	#0x01, R14
jnz	noColor
mov	#0x18, R13
jmp	next
noColor:
mov	#0xE7, R13
call	#writeNokiaByte
mov	#0xC3, R13
call	#writeNokiaByte
mov	#0x81, R13
call	#writeNokiaByte
mov	#0x00, R13
call	#writeNokiaByte
mov	#0x00, R13
call	#writeNokiaByte
mov	#0x81, R13
call	#writeNokiaByte
mov	#0xC3, R13
call	#writeNokiaByte
mov	#0xE7, R13
call	#writeNokiaByte
jmp	done
next:
call	#writeNokiaByte
mov	#0x3C, R13
call	#writeNokiaByte
mov	#0x7E, R13
call	#writeNokiaByte
mov	#0xFF, R13
call	#writeNokiaByte
mov	#0xFF, R13
call	#writeNokiaByte
mov	#0x7E, R13
call	#writeNokiaByte
mov	#0x3C, R13
call	#writeNokiaByte
mov	#0x18, R13
call	#writeNokiaByte
```
This section of code contains the changes I made to nokia.asm to get the ball and inverting extra functionalities.  The program goes through an eight step procedure to make a diamond-like figure that represents a ball on the nokia display.  The ball function works the exact same as B functionality in that it is just a ball bouncing around the screen.  The inverting functionality draws a reverse diamond to the nokia display, which creates a white ball bouncing around on the screen when the screen color is inverted.  I had to create a new method in nokia.asm to invert the screen background color.  It is pretty much just clearDisplay() but clears the display with black pixels instead of white.  That method I created is shown below:

```
;-------------------------------------------------------------------------------
; Name: invertDisplay
; Inputs: none
; Outputs: none
; Purpose: Writes 0x360 blank 8-bit columns to the Nokia display
;-------------------------------------------------------------------------------
invertDisplay:
push	R11
push	R12
push	R13
mov.w	#0x00, R12	; set display address to 0,0
mov.w	#0x00, R13
call	#setAddress
mov.w	#0x01, R12	; write a "clear" set of pixels
mov.w	#0xFF, R13	; to every byt on the display
mov.w	#0x360, R11	; loop counter
clearTheLoop:
call	#writeNokiaByte
dec.w	R11
jnz	clearTheLoop
mov.w	#0x00, R12	; set display address to 0,0
mov.w	#0x00, R13
call	#setAddress
pop	R13
pop	R12
pop	R11
ret
```


####Debugging/Testing

The first thing I had to debug and test was the required functionality.  The first time that I tried running the code I wrote, nothing showed up on the screen.  With help from Dr. Coulston, I realized that I need to pop R14 after calling drawBlock, I was forgetting to do that.  After that, the program worked, and I provided picture proof in the conclusion section.  You can tell it works because there are three different sets of black forms, separated by white space.  I would not be able to do that unless I had required functionality.

The next thing I had to debug and test was the B functionality.  This was a real pain.  I tried just using my code from homework 6, the pong game, but I quickly found out that the program did not work as I thought it did.  After trying to make the program work for a couple hours using a header file, implementation file, and actual program file, I gave up.  I had to resort to caveman-esque coding practices.  I took out the moveBall function completely and just ran it through the while statement in the main program file.  I stopped using an implementation file and defined my createBall method from my header in the main program file.  Despite the ugly coding, the code finally worked and all was well with the world.

The next thing I had to debug and test was the ball extra functionality.  I new that I had to make a diamond-like object to make it look like a ball on the nokia display.  The first time I tried to adjust drawBlock to make this work, I created a reverse diamond, or an hourglass.  As cool as that was, it was not what I needed (although it turned out that it was what I needed for the invert display extra functionality).  All I had to do to fix this problem was to switch the hex code going into R13 to be drawn by the code.  When I did this, the functionality worked.

The final thing I had to test and debug was the inverting functionality.  I used the inverted diamond I accidentally made in the testing of the ball functionality to get this functionality to work.  The only thing left I had to do was to invert the background of the nokia display.  To get this to work, I had to create a new method, one that did the opposite of clearDisplay().  This functionality worked the first time I tested it and thus did not require much effort.

####Conclusions/Results

Here is a picture of my nokia display after running my program, showing that my required functionality works:
![alt text](https://raw.githubusercontent.com/JeremyGruszka/ECE382_Lab4/master/required_func.jpg "Required Functionality")

The rest of my functionalities were checked off by Dr. Coulston.  My lab worked, and it was fun making stuff dance around the screen of the nokia display.

Documentation:  Class notes and handouts, Lab 4 handout, My previous work and labs
