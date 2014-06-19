#include <stdlib.h>
#include <stdio.h>
#include "picomms.h"
#include<math.h>

/*
This uses a wallfollower algorithm 
*/

//----------------------------------------------------------------------------------------

void turn(float degree);
void getCoordinates();
void follow_wall();
//----------------------------------------------------------------------------------------



const int SPEED = 50;//50


float previousL = 0, previousR = 0;
float angle0 = 0;
float coordx = 0, coordy = 0;		// path correcting error (in cm)
float Xa = 0, Ya = 0, Xb = 0, Yb = 0;

const int TURNSPEED = 25;//25

int main()
{
	connect_to_robot();
	initialize_robot();
	follow_wall();
}

//from midterm
void getCoordinates() {
	int motorEncoders = 0, leftEncoder = 0, rightEncoder = 0;
	float rr = 0;
	float dy = 0, dx = 0;
	float x1 = 0, x2 = 0, y1= 0, y2 = 0;

	get_motor_encoders(&leftEncoder, &rightEncoder);
	motorEncoders = leftEncoder;

	float l = (leftEncoder * 31) / 360;
	float dl = l - previousL;
	previousL = l;

	float r = (rightEncoder * 31) / 360;
	float dr = r - previousR;
     	previousR = r;

        float d0 = (dl-dr) / 23;

	if (dl != dr) {
		angle0 = angle0 + d0;

		rr = dl / d0;
		x1 = (rr - (23.5 / 2)) * cos(angle0);
		x2 = (rr - (23.5 / 2)) * cos(angle0+d0);

		y1 = (rr - (23.5 / 2)) * sin(angle0);
		y2 = (rr - (23.5 / 2)) * sin(angle0+d0);

		dx = ( l > r ) ? (x1-x2) : -(x1-x2);      //if moving right add dx or if moving left, minus dx
		dy = y2 - y1;
	}

	else {
		dx = dl * sin(angle0);
		dy = dr * cos(angle0);
	}

	coordx += dx;
	coordy += dy;
       	printf("coordx is %f         coordy is %f\n", coordx, coordy);
}



//Turn function
void turn(float degree) {
    int leftEncoder = 0, rightEncoder = 0;
get_motor_encoders(&leftEncoder, &rightEncoder);
int initialEncoder = leftEncoder;
float degEncoder = (23.5 * degree)/ 10.5;
if (degree > 0) //turn clockwise 
{
    do
	{
       get_motor_encoders(&leftEncoder, &rightEncoder);
       set_motors(20,-20);
    } while((leftEncoder - initialEncoder) < degEncoder);
        
	} else {  //turn anticlockwise
	
	do
	{
       get_motor_encoders(&leftEncoder, &rightEncoder);
       set_motors(-20, 20);
    } while((leftEncoder - initialEncoder) > degEncoder);
}
}



void follow_wall()
{
    int irValue_Left, irValue_Right;
	int isValue_Left, isValue_Right;
	int leftEncoder,rightEncoder;
    
	int usValue;

    
	set_ir_angle(LEFT,-35);
	set_ir_angle(RIGHT,35);
    
	
	while(1)
	{

	//follow left wall

		get_front_ir_dists(&irValue_Left,&irValue_Right);
		get_side_ir_dists(&isValue_Left,&isValue_Right);
		usValue = get_us_dist();
		
		//if dead end
		if(usValue < 20 && irValue_Right < 30 && irValue_Left < 30) 
		{
			turn(195.9);
		}		
		else if(usValue<25)
		{
			if(irValue_Left>irValue_Right)		
			{
				turn(-90);
			}
			else
			{
				turn(90);
			}
		}
		
		else if(irValue_Left>30)
		{
			set_motors(SPEED-TURNSPEED+10,SPEED+TURNSPEED);
		}
		
		else if(irValue_Left<15)//15
		{
			set_motors(SPEED+10,SPEED-10);//SPEED+TURNSPEED,SPEED-TURNSPEED
		}
		
		else
		{
			set_motors(SPEED+5,SPEED+5);
		}
        
		
        
        
		getCoordinates();
		
			
		if (coordy < -7) {
			break;
		}	


	
	
	}

}




