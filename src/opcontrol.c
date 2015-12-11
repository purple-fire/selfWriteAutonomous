/** @file opcontrol.c
 * @brief File for operator control code
 *
 * This file should contain the user operatorControl() function and any functions related to it.
 *
 * Copyright (c) 2011-2014, Purdue University ACM SIG BOTS.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Purdue University ACM SIG BOTS nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL PURDUE UNIVERSITY ACM SIG BOTS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Purdue Robotics OS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"
#include "motorControl.c"

#define size 6
#define speed 50

bool state[size]; //Bools for the individual channels
int val[size];  //Values for each channel
bool change[size];  //If a channel has changed

bool oldState[size]; //Bools for the individual channels
int oldVal[size];  //Values for each channel

double initialTime;

int Btn7U, Btn7D, Btn7L, Btn7R;
int Btn8U, BtnD, Btn8L, Btn8R;

void init()
{
	for (int i = 0; i < size; i++)
		state[i] = false;
	for (int i = 0; i < size; i++)
		val[i] = 0;
	for (int i = 0; i < size; i++)
		change[i] = false;
	for (int i = 0; i < size; i++)
		oldState[i] = false;
	for (int i = 0; i < size; i++)
		oldVal[i] = 0;
}
void dump()
{
	for (int i = 0; i < size; i++)
	{
		oldState[i] = state[i];
		oldVal[i] = val[i];
	}
}

void forBack()
{
	while(true)
	{
		if (Btn7U && !state[0])
		{
			dump();
			delay(5);
			change[0] = true;
			state[0] = true;
			val[0] = speed;
		}
		else if (Btn7D && !state[1])
		{
			dump();
			delay(5);
			change[1] = true;
			state[1] = true;
			val[1] = -speed;
		}

		if (!Btn7U && state[0])
		{
			dump();
			delay(5);
			change[0] = true;
			val[0] = 0;
			state[0] = false;
		}
		if (!Btn7D && state[1])
		{
			dump();
			delay(5);
			change[1] = true;
			val[1] = 0;
			state[1] = false;
		}
	}
}

void rot()
{
	while(true)
	{
		if (Btn7L && !state[2])
		{
			dump();
			delay(5);
			change[2] = true;
			state[2] = true;
			val[2] = -speed;
		}
		else if (Btn7R && !state[3])
		{
			dump();
			delay(5);
			change[3] = true;
			state[3] = true;
			val[3] = speed;
		}

		if (!Btn7L && state[2])
		{
			dump();
			delay(5);
			change[2] = true;
			val[2] = 0;
			state[2] = false;
		}
		if (!Btn7R && state[3])
		{
			dump();
			delay(5);
			change[3] = true;
			val[3] = 0;
			state[3] = false;
		}
	}
}

bool isRobotMoving()
{
	for(int i = 0; i < size; i++)
	{
		if (oldState[i])
			return true;
	}
	return false;
}

bool suddenChange()
{
	for (int i = 0; i < size; i++)
	{
		if (change[i])
			return true;
	}
	return false;
}

void resetTimer(){
	initialTime = millis();
}

int getTimePassed(){
	return millis() - initialTime;
}
void autoWrite()
{
	resetTimer();
	while(true)
	{
		if (suddenChange())
		{
			if(!isRobotMoving())
				printf("reset();");
			else
			{
				if(oldState[0])
					printf("move(%d);", oldVal[0]);
				else if(oldState[1])
					printf("move(%d);", oldVal[1]);
				else if (oldState[2])
					printf("rotate(%d);", oldVal[2]);
				else if (oldState[3])
					printf("rotate(%d);", oldVal[3]);
				else if (oldState[4])
					printf("target = %d", oldVal[4]);
				else if (oldState[5])
					printf("target = %d;", oldVal[5]);
			}
			printf("delay(%d);", getTimePassed());


			for (int i = 0; i < size; i++)
			{
				if (change[i])
				{
					change[i] = false;
					break;
				}
			}
			resetTimer();
		}
	}
}

void reset()
{
	for (int i = 0; i < 12; i++)
		setMotorSpeed(i, 0);
}

void drive(int s){

}

void rotate(int s)
{

}

void getInputs(){

}

void operatorControl() {

	/*StartTask(forBack);
	StartTask(rot);
	StartTask(autoWrite);*/
	while(1)
	{
		if (Btn7U)
			drive(speed);
		else if (Btn7D)
			drive(-speed);
		else if (Btn7L)
			rotate(speed);
		else if (Btn7R)
			rotate(-speed);
		else
			drive(0);
	}
}
