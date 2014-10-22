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
This section of code contains the required functionality.  In the beginning of the code it sets up variables and pulls functions from external sources. After the first block is drawn to the screen, the code goes through an infinite while loop to see if buttons are pressed.  If any of the directional buttons are pressed, the nokia display will draw a new box in the direction chosen without erasing the previous box.  If the auxillary button is pressed, the drawBlock function will invert and draw a white box to the nokia display.  In this manner, the program creates an etch-a-sketch type game.

```
;-------------------------------------------------------------------------------
;Subroutine Name: decryptMessage
;Author: Jeremy Gruszka
;Function: Decrypts a string of bytes and stores the result in memory.  Accepts
;           the address of the encrypted message, address of the key, and address
;           of the decrypted message (pass-by-reference).  Accepts the length of
;           the message by value.  Uses the decryptCharacter subroutine to decrypt
;           each byte of the message.  Stores theresults to the decrypted message
;           location.
;Inputs:	R4 (message), R11 (message length counter), R6 (key), R7 (key length)
;Outputs:	Decrypted Message in RAM starting at location 0x0200
;Registers destroyed:	None
;-------------------------------------------------------------------------------

decryptMessage:

			mov.w	#mesLength, R11		;counter for length of message
			mov.b	@R4+, R5		;moves next part of message into register
			mov.b	@R6+, R8		;moves next part of key into register
			call	#decryptCharacter
			mov.b	R5, 0(R10)		;stores decrypted message in memory
			inc.w	R10
			dec.w	R7
			jz	keyTracker		;resets the key if it reaches the end of the key
subReset
			dec.w	R11
			jnz	decryptMessage		;re-runs decrypt message as long as there is part of the message left

            		ret

keyTracker
			mov.w	#key, R6
			mov.w	#keyLength, R7
			jmp	subReset
```
This section of contains the decryptMessage subroutine. I had to manually count the length of the encrypted message, and then made a for loop that decrypted the message until the message was fully decrypted.  This part of the code also contains the B functionality.  It essentially uses the key until the key is fully used, and then resets the key.  This allows any length of key to be used for decrption.

```
;-------------------------------------------------------------------------------
;Subroutine Name: decryptCharacter
;Author: Jeremy Gruszka
;Function: Decrypts a byte of data by XORing it with a key byte.  Returns the
;           decrypted byte in the same register the encrypted byte was passed in.
;           Expects both the encrypted data and key to be passed by value.
;Inputs:	R5 (character), R8 (key)
;Outputs:	R5 (decrypted character)
;Registers destroyed:	R5
;-------------------------------------------------------------------------------

decryptCharacter:

			xor	R8, R5		;decrypts character

            		ret
```
This final section of code contains the decrypt character subroutine.  It takes in an individual byte and XORs it with the key which decrypts the byte.  It then sends it back to the decryptMessage subroutine for storage in RAM.


####Debugging/Testing

I started the program by using the basic outline given in the Lab 2 instructions on the sharepoint.  That gave me the basic idea of how I would write the program.  I had the basic outline and then had to fill in each portion to get the program functionality working.  

I wrote the program as I thought it should work and tried compiling.  I had a weird error at the XOR portion of the decrpytCharacter subroutine where it was saying something along the lines of the key register was not allowed in the subroutine.  After pulling out my hair for ahwile, I realized that I needed to change my key from a .equ to .byte in order to place it in ROM.  That was the only problem that I had with the required funcitonality.

My biggest problem with B functionality was finding an easy yet simple way to reset the key once the end of the key is reached.  I solved this by using jumps inside the decryptMessage subroutine to temporarily leave the subroutine and reset the key, then return to the same spot in the subroutine.

Unfortunately, due to time constraints, other homework, and my lack of creativity with this particular type of programming, I was not able to get the A functionality of the code working.

####Conclusions/Results

I ran the test cases given by the lab handout for the basic program and for the B functionality.  Each test produced the correct result, showing that my basic program worked and that my B functionality worked.

Here is a picture of my nokia display after running my program, showing that my required functionality works:
![alt text](https://raw.githubusercontent.com/JeremyGruszka/ECE382_Lab4/master/required_func.jpg "Required Functionality")

Documentation:  Class notes and handouts, Lab 4 handout, My previous work and labs
