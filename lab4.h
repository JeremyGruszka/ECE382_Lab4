/*
 * lab4.h
 * Author: Jeremy Gruszka
 * Date: 20 Oct 2014
 * Description: B functionality
 */

#ifndef _LAB4_H
#define _LAB4_H

//constants
#define SCREEN_WIDTH	96
#define SCREEN_HEIGHT	68

typedef struct {
    int xPos, yPos, xVel, yVel;
} ball_t;

/*
 * This section of the code is the constructor for the ball.  It takes in 4 values and
 * outputs a single ball object
 */
ball_t createBall(int xPos, int yPos, int xVel, int yVel);

#endif
