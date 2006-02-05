#include <system.h>
#include <rand.h>
#include "intro.h"
#include "gpu_pic.h"
#include "spu_pic.h"

#pragma CLOCK_FREQ 50000000						//required for accurate delay functionality

char input;
char newinput;
char currentstate;
char fb = 0;
char playerKills = 0;
char playerGotHit = 0;

enum Visibility
{
	VISIBLE_Dead = 0,
	VISIBLE_Player = 2,
	VISIBLE_PlayerZoom = 3,
	VISIBLE_F16DownSlow = 10,
	VISIBLE_F16Seeking = 11,
	VISIBLE_F16DownFast = 12,
	VISIBLE_F16Right = 13,
	VISIBLE_F16Left = 14,
	VISIBLE_Chopper = 15,
	VISIBLE_F14 = 16,
	VISIBLE_PlayerBullet = 255,
	VISIBLE_LargeExplosion = 211,
	VISIBLE_MediumExplosion = 200,
	VISIBLE_SmallExplosion = 209
};

//initialize serial port for continuous receive
void serialinit()
{
	set_bit(trisc, 7);
	set_bit(trisc, 6);
	spbrg = 80;
	clear_bit(txsta, BRGH); 
	clear_bit(baudcon, BRG16);
	clear_bit(txsta, SYNC);
	set_bit(rcsta, SPEN);
	set_bit(txsta, TXEN);
	set_bit(rcsta, CREN);
	set_bit(pie1, RCIE);
	set_bit(intcon, PEIE);
	set_bit(intcon, GIE);
}

void putchar(char d)
{
	txreg = d;
}

char screenrand()
{
	char r = (char) rand();
	if (r > 140)
	{
		r -= 140;
	}
	return r;
}
	
//generic ISR
void interrupt()
{
	//serial receive interrupt handler?
	if (pir1 & 0b00100000)
	{
		input = rcreg;
		newinput = 1;			
	}
}

void introMain()
{
	//plays intro, any player input should stop intro
	if (newinput)
	{
		newinput = 0;
		currentstate = 1;
		return;		
	}
	delay_s(2);
	scene1();
	if (newinput)
	{
		newinput = 0;
		currentstate = 1;
		return;		
	}
	scene2();
	if (newinput)
	{
		newinput = 0;
		currentstate = 1;
		return;		
	}
	scene3();
	if (newinput)
	{
		newinput = 0;
		currentstate = 1;
		return;		
	}
	scene4();
	if (newinput)
	{
		newinput = 0;
		currentstate = 1;
		return;		
	}
	scene5();
	if (newinput)
	{
		newinput = 0;
		currentstate = 1;
		return;		
	}
	//always go to the title after last scene
	scene6();
	currentstate = 1;
	return;

}

void drawTitle()
{
	Sprite me;
		me.image.address = 0x02BB000;
		me.image.lines = 240;
		me.image.width = 160;
		me.position.x = 0;
		me.position.y = 0;
		me.alpha = 0;
		me.visible = 1;

	char i;
	set_fieldcolor (0b00000000);
	for ( i = 0; i < 4; i++)
	{
		cls (i);
		delay_ms(50);
	}
	
	delay_ms(250);
	delay_ms(250);
	
	for ( i = 0; i < 4; i++)
	{
		cls (3-i);
		delay_ms(50);
	}
	
	for (i = 0; i < 19; i++)
	{
		drawsprite(me);
		me.image.address += 0x9600;
		delay_ms(100);
	}
	for ( i = 0; i < 255; i++)
	{
		if (newinput)
		{
			if (input & 00010000b)
			{
				//seed random generator based on time player takes to hit start
				srand(i);
				currentstate = 2;
				playmusic(255);
				delay_ms(200);
				playmusic(2);
				return;
			}
		}
		delay_ms(100);
	}
	currentstate = 0;
	playmusic(255);		
	return;
}

void drawarray (Sprite sprite[])
{
	char i;
	for (i = 0; i < 12; i++)
	{
		if (sprite[i].visible)
		{		
			unsigned long destination_address = sprite[i].position.y * 160;
			destination_address += sprite[i].position.x;
			load_t_addr(destination_address);
			load_s_addr(sprite[i].image.address);
			load_s_lines(sprite[i].image.lines);
			load_l_size(sprite[i].image.width);
			if (sprite[i].alpha)
				load_alphaOp(true);
			else
				load_alphaOp(false);
			draw();
		}
	}
}

void inputHandler(Sprite & player, Sprite missile[])
{
	char i;
	
	//if we have a joypad or joystick
	if (portd & 0b00000001)
	{
		if (player.visible == 2)
		{
			//Player input gets handled here
			newinput = 0;
			if (input & 0b00001000)
			{
				//
				// moving up
				//
				player.position.y -= 3;
				player.y -= 3;

				if (player.y < 25)
				{
					player.position.y = 25;
					player.y = 25;
				}
			}
			if (input & 0b00000010)
			{
				//
				// moving left
				//
				if (player.a > 2)
				{
					player.a -= 2;
				}
				if( player.position.x > 0) 
				{
					player.position.x -= 2;
					player.x -= 2;
				}
			}
			if (input & 0b00000100)
			{
				//
				// moving down
				//
				if (player.position.y < 220)
				{
					player.position.y += 3;
					player.y += 3;
				}
			}
			if (input & 0b00000001)
			{
				//
				// moving right
				//
				if (player.a < 28)
				{
					player.a += 2;
				}
				if (player.position.x < 144)
				{
					player.position.x += 2;
					player.x += 2;
				}
			}
		}
		if ((input & 0b01000000))
		{
			//if we're not overheating, fire and increase heat
			if (player.b < 20)
			{	
				for (i = 0; i < 12; i++)
				{
					if (!missile[i].visible)
					{
						//found a new missile slot, so we can instantiate it here
						missile[i].image.address = 0x000D97B2;
						missile[i].image.lines = 6;
						missile[i].image.width = 2;
						missile[i].position.x = (player.position.x + 7);
						missile[i].position.y = player.position.y;
						missile[i].alpha = 1;
						missile[i].visible = 255;
						missile[i].x = (player.x + 7);
						missile[i].y = player.y;
						player.b +=4;	
						return;
					}
				}
			}
			//if we are overheating, don't fire, but increase heat
			else if (player.b < 50)
			{
				player.b += 1;
				return;
			}
			else
			{
				return;
			}
		}
	}
	else
	//if we have a keyboard (somehow)
	{
		if (player.visible == 2)
		{
			//Player input gets handled here
			newinput = 0;
			if (input == '8')
			{
				if (player.position.y > 1)
				{
					player.position.y -= 4;
					player.y -= 4;
				}
			}
			if (input == '4')
			{
				if (player.a > 2)
				{
					player.a -= 2;
				}
				if( player.position.x > 0) 
				{
					player.position.x -= 2;
					player.x -= 2;
				}
			}
			if (input == '2')
			{
				if (player.position.y < 212)
				{
					player.position.y += 4;
					player.y += 4;
				}
			}
			if (input == '6')
			{
				if (player.a < 28)
				{
					player.a += 2;
				}
				if (player.position.x < 144)
				{
					player.position.x += 2;
					player.x += 2;
				}
			}
		}
		if (input == ' ')
		{
			for (i = 0; i < 12; i++)
			{
				if (!missile[i].visible)
				{
					missile[i].image.address = 0x000D97B2;
					missile[i].image.lines = 6;
					missile[i].image.width = 2;
					missile[i].position.x = (player.position.x + 7);
					missile[i].position.y = player.position.y;
					missile[i].alpha = 1;
					missile[i].visible = 255;
					missile[i].x = (player.x + 7);
					missile[i].y = player.y;
					player.b += 10;
					return;
				}
			}
		}
		
	}
}
void movementHandler(Sprite & unit)
{
	///////////////////////////////////////////////////////////
	//START OF GENERAL PURPOSE BEHAVIOR DEFINITIONS
	//
	//ID:1 -> ID: 10
	//
	
	//ID = 1, just a regular image - do nothing
	
	//handler for player and playerlike units
	//ID = 2
	if (unit.visible == 2)
	{
		//Player movement and collision detection gets handled here.
		if (unit.a < 10)
		{
			unit.a++;
			unit.image.address = 0x000D9D20;
		}
		else if (unit.a > 20)
		{
			unit.a--;
			unit.image.address = 0x000D8BA0;
		}
		else 
		{
			if (unit.a < 15)
			{
				unit.a++;
			}
			else if (unit.a > 15)
			{
				unit.a--;
			}
			unit.image.address = 0x000D7A20;
		}
		return;
	}
	
	//unit zooming off the screen (going up)
	//ID = 3
	else if (unit.visible == 3)
	{
		if (unit.position.y > 4)
			unit.position.y -= 4;
		else
		{
			unit.position.y -= 4;
			unit.visible = 0;
		}
		return;
	}
	
	///////////////////////////////////////////////////////////
	//START OF EFFECT DEFINITIONS (Bullets, explosions)
	//
	//ID:200 -> ID: 255
	//
	
	//Explosion #1
	else if ( (unit.visible >= 211) && (unit.visible <= 222))
	{
		//beginning of explosion
		if (unit.visible == 211)
		{
			unit.a = 0;
			unit.image.address = 0x00107B60;
			unit.image.width = 40;
			unit.image.lines = 80;
			unit.visible++;
		} 
		else if (unit.visible == 222)
		{
			unit.image.address += unit.image.width;
			unit.visible = VISIBLE_Dead;
		}
		else
		{
			static char updateRate = 5; // higher number = slower update

			unit.a++;
			if (unit.a % updateRate == 0)
			{
				if (unit.a % (updateRate * 4) == 0)
				{
					unit.image.address += 160 * unit.image.lines - 3 * unit.image.width;
				}
				else
				{
					unit.image.address += unit.image.width;
				}

				unit.visible++;
			}
		}
		return;
	}
}

char isBullet(Sprite* thing)
{
	return (thing->visible >= 150) && (thing->visible <= 157);
}

char isExplosion(Sprite* thing)
{
	return (thing->visible >= 200) && (thing->visible <= 230);
}

void collideArrayPlayer (Sprite enemy[], Sprite & player)
{
	//
	// "fudge" factors, give the player a little room
	// before actually being hit
	//
	char px = player.x + 3;
	char py = player.y + 5;
	char pw = player.image.width - 3;
	char ph = player.image.lines - 5;

	char i;
	for (i = 0; i < 12; i++)
	{
		if (enemy[i].visible && !isExplosion(&enemy[i]))
		{
			if(((enemy[i].image.width + enemy[i].x) > px) &&
			    (enemy[i].x < (pw + px)) &&
			    (enemy[i].y < (ph + py)) &&
			   ((enemy[i].image.lines + enemy[i].y) > py) )
			{
				//if player is not already an explosion or other effect (e.g. invincible)
				if ((player.visible < 200) && (player.visible) )
				{
					playerGotHit = 10;
					if (isBullet(&enemy[i]))
					{
						// bullets do 1 point of damage
						player.alpha--;
						enemy[i].visible = VISIBLE_SmallExplosion;
					}
					else
					{
						// running into enemies does 2 points of damage
						player.alpha -= 2;
						playerKills++;
						enemy[i].visible = VISIBLE_MediumExplosion;
					}

					if (player.alpha < 1)
						player.alpha = 1;

					if (player.alpha == 1)
					{
						//
						// HACK change player position
						// so explosion is centered on ship
						//
						player.x -= 12;
						player.position.x -= 12;
						player.y -= 26;
						player.position.y -= 26;

			   			player.visible = VISIBLE_LargeExplosion;
					}
			   	}
			   	return;
			}
		}
	}
}

void collideArrayArray (Sprite enemy[], Sprite missile[])
{
	char i, j;
	
	//for each player bullet
	for (j = 0; j < 12; j++)
	{
		for (i = 0; i < 12; i++)
		{
			//here only care about collidable objects (ID < 150) and objects which exist
			if ((enemy[i].visible < 150) && 
				(enemy[i].visible != VISIBLE_Dead) && 
				!isExplosion(&missile[j]) &&
				(missile[j].visible != VISIBLE_Dead))
			{
				if( ((enemy[i].image.width + enemy[i].x) > missile[j].x) &&
				   (enemy[i].x < missile[j].x) &&
				   (enemy[i].y < missile[j].y) &&
				   ((enemy[i].image.lines + enemy[i].y) > missile[j].y) )
				{
					//reduce enemy's health
					enemy[i].a--;
					missile[j].visible = VISIBLE_SmallExplosion;

					//if player is not already an explosion or other effect (e.g. invincible)
					//if ((enemy[i].visible < 200) && (enemy[i].visible) && enemy[i].a < 1)
					if (!isExplosion(&enemy[i]) && 
						(enemy[i].visible != VISIBLE_Dead) &&
						(enemy[i].a < 1))
					{
						playerKills++;
					  	enemy[i].visible = VISIBLE_MediumExplosion;
					}
				}
			}
		}
	}
}
void blankEnemies (Sprite enemy[])
{
	char i, j;
	for (i = 0; i < 12; i++)
	{
		enemy[i].image.address = 0x000DA7D0;
		enemy[i].image.lines = 49;
		enemy[i].image.width = 14;
		enemy[i].position.x = 30;
		enemy[i].position.y = 0;
		enemy[i].alpha = 1;
		enemy[i].visible = 0;
		enemy[i].a = 0;
		//we're done as soon as we find an empty slot in which to initialize the enemy
	}
}

void initEnemy (Sprite enemy[], Sprite & player, char type, char from, char xcoord)
{
	char i, j;
	for (i = 0; i < 12; i++)
	{
		//find a blank spot to put new spawn
		if ((enemy[i].visible == 0))
		{
			//various variants of the F16 enemy fighter
			if ((type <= 14) && (type >= 10))
			{
				enemy[i].image.address = 0x000DA7D0;
				enemy[i].image.lines = 49;
				enemy[i].image.width = 15;
				enemy[i].position.x = xcoord;
				enemy[i].position.y = 0;
				enemy[i].alpha = 1;
				enemy[i].visible = type;
				enemy[i].a = 5;//health
				enemy[i].b = 0;	//timer for changing sprites
				enemy[i].x = xcoord;
				enemy[i].y = 0;
				//we're done as soon as we find an empty slot in which to initialize the enemy
				return;
			}
			//Type 15, attack chopper, static on screen, random start
			if (type == 15)
			{
				enemy[i].image.address = 0x000D809F;
				enemy[i].image.lines = 47;
				enemy[i].image.width = 19;
				enemy[i].position.x = xcoord;
				enemy[i].position.y = 0;
				enemy[i].alpha = 1;	//extra data can be stored here
				enemy[i].visible = type;
				enemy[i].a = 12; // health
				enemy[i].b = (screenrand() / 2) + 50;
				enemy[i].x = xcoord;
				enemy[i].y = 0;
				//we're done as soon as we find an empty slot in which to initialize the enemy
				return;
			}
			
			//Type 16, F14 fighter, fires missiles
			if (type == 16)
			{
				enemy[i].image.address = 0x000D7A30;
				enemy[i].image.lines = 73;
				enemy[i].image.width = 34;
				enemy[i].position.x = xcoord;
				enemy[i].position.y = 0;
				enemy[i].alpha = 1;	//extra data can be stored here
				enemy[i].visible = type;
				enemy[i].a = 7;//health
				enemy[i].b = 0;	//timer for changing sprites
				enemy[i].x = xcoord;
				enemy[i].y = 0;
				return;
			}
			
			//Type 20-30, AA missile launcher
			if (type >= 20 && type <= 30)
			{
				enemy[i].image.address = 0x000DF5FC;
				enemy[i].image.lines = 22;
				enemy[i].image.width = 12;
				enemy[i].position.x = xcoord;
				enemy[i].position.y = 0;
				enemy[i].alpha = 1;	//extra data can be stored here
				enemy[i].visible = type;
				enemy[i].a = 5;//health
				enemy[i].b = 0;	//timer for changing sprites
				enemy[i].x = xcoord;
				enemy[i].y = 0;
				return;
			}
			
			//Type 150-157, bullet regular shot in fixed angles
			else if ( (type >= 150) && (type <= 157))
			{
				char xt = (enemy[from].image.width / 2) - 2;
				char yt = enemy[from].image.lines;
				enemy[i].image.address = 0x000D97BB;
				enemy[i].image.lines = 7;
				enemy[i].image.width = 4;
				enemy[i].position.x = (enemy[from].position.x + xt);
				enemy[i].position.y = (enemy[from].position.y + yt);
				enemy[i].alpha = 1;
				enemy[i].visible = type;
				enemy[i].x = enemy[from].x + xt;
				enemy[i].y = enemy[from].y + yt;
				//we're done as soon as we find an empty slot in which to initialize the enemy
				return;
			}
			//Type 160, enemy missile
			else if (type == 160)
			{
				char xt = (enemy[from].image.width / 2) - 2;
				char yt = enemy[from].image.lines;
				enemy[i].image.address = 0x000D9E19;
				enemy[i].image.lines = 21;
				enemy[i].image.width = 5;
				enemy[i].position.x = (enemy[from].position.x + xt);
				enemy[i].position.y = (enemy[from].position.y + yt);
				enemy[i].alpha = 1;
				enemy[i].visible = type;
				enemy[i].x = enemy[from].x + xt;
				enemy[i].y = enemy[from].y + yt;
				//we're done as soon as we find an empty slot in which to initialize the enemy
				return;
			}
			
		}
	}
}
void enemyBullet (Sprite enemy[], Sprite & player, char i)
{
	char deltax;
	if (player.x > enemy[i].x)
		deltax = player.x - enemy[i].x;
	else
		deltax = enemy[i].x - player.x;
	if (deltax == 0)
	{
		deltax = 1;
	}
	char deltay = player.y - enemy[i].y;
	char slope = deltay/deltax;
	//if player is approximately in front
	if ( slope > 8)
	{
		initEnemy(enemy, player, 150, i, 0);
	}
	//if player is approximately 45 degrees right
	else if ( (slope < 4)  && (player.x > enemy[i].x))
	{
		initEnemy(enemy, player, 151, i, 0);
	}
	//if player is approximately 45 degrees left
	else if ( (slope < 4)  && (player.x < enemy[i].x))
	{
		initEnemy(enemy, player, 152, i, 0);
	}
	//if player is approximately 22.5 degress right		
	else if ( (slope < 8) && (slope > 4) && (player.x > enemy[i].x))
	{
		initEnemy(enemy, player, 153, i, 0);
	}
	//if player is approximately 22.5 degress left		
	else if ( (slope < 8) && (slope > 4) && (player.x < enemy[i].x))
	{
		initEnemy(enemy, player, 154, i, 0);
	}
}			

	
void enemyHandler (Sprite enemy[], Sprite & player)
{
	char i;
	for (i = 0; i<12; i++)
	{ 
		///////////////////////////////////////////////////////////
		//START OF ENEMY BEHAVIOR DEFINITIONS
		//
		//ID:10 -> ID:199
		//

		//behavior for F16 going straight down
		if (enemy[i].visible == 10)
		{
			//decide if we want to shoot
			if ((screenrand() < 3) && (enemy[i].y < player.y))
			{
				//F16 shoots regular old bullets
				enemyBullet(enemy, player, i);
			}
			if (enemy[i].y < 240)
			{		
			 	enemy[i].y += 2;
			 	enemy[i].position.y += 2;
			 	//engine animation
			 	if (enemy[i].b)
			 	{ 
			 		enemy[i].image.address = 0x000DA7D0;
			 		enemy[i].b = 0;
			 	}
			 	else
			 	{
			 		enemy[i].image.address = 0x000DAEA1;
			 		enemy[i].b = 1;
			 	}	
			 }
			else
				enemy[i].visible = 0;
		}
		
		//F16 which seeks the player
		if (enemy[i].visible == 11)
		{
			//decide if we want to shoot
			if ((screenrand() < 3) && (enemy[i].y < player.y))
			{
				//F16 shoots regular old bullets
				enemyBullet(enemy, player, i);
			}
			if (enemy[i].y < 212)
			{		
			 	enemy[i].y += 2;
			 	enemy[i].position.y += 2;
			 
			 	if (enemy[i].b == 1)
				{
					if (enemy[i].x < player.x)
					{		
					 	enemy[i].x++;
					 	enemy[i].position.x++;
					}
					else if (enemy[i].x > player.x)
					{
						enemy[i].x--;
					 	enemy[i].position.x--;
					}
				}
			 	//engine animation
			 	if (enemy[i].b)
			 	{ 
			 		enemy[i].image.address = 0x000DA7D0;
			 		enemy[i].b = 0;
			 	}
			 	else
			 	{
			 		enemy[i].image.address = 0x000DAEA1;
			 		enemy[i].b = 1;
			 	}	
			 }
			else
				enemy[i].visible = 0;
		}
		
		//F16 going straight down fast
		if (enemy[i].visible == 12)
		{
			//decide if we want to shoot
			if ((screenrand() < 4) && (enemy[i].y < player.y))
			{
				//F16 shoots regular old bullets
				enemyBullet(enemy, player, i);
			}
			if (enemy[i].y < 212)
			{		
			 	enemy[i].y += 4;
			 	enemy[i].position.y += 4;
			 	//engine animation
			 	if (enemy[i].b)
			 	{ 
			 		enemy[i].image.address = 0x000DA7D0;
			 		enemy[i].b = 0;
			 	}
			 	else
			 	{
			 		enemy[i].image.address = 0x000DAEA1;
			 		enemy[i].b = 1;
			 	}	
			 }
			else
				enemy[i].visible = 0;
		}
		
		//F16 going to the right
		if (enemy[i].visible == 13)
		{
			//decide if we want to shoot
			if ((screenrand() < 3) && (enemy[i].y < player.y))
			{
				//F16 shoots regular old bullets
				enemyBullet(enemy, player, i);
			}

			if (enemy[i].y > 50)
			{
				//go to the right if low enough
				enemy[i].x++;
				enemy[i].position.x++;
			}

			if (enemy[i].y < 212)
			{		
			 	enemy[i].y += 2;
			 	enemy[i].position.y += 2;
			 	//engine animation
			 	if (enemy[i].b)
			 	{ 
			 		enemy[i].image.address = 0x000DA7D0;
			 		enemy[i].b = 0;
			 	}
			 	else
			 	{
			 		enemy[i].image.address = 0x000DAEA1;
			 		enemy[i].b = 1;
			 	}	
			 }
			else
				enemy[i].visible = 0;
		}		

				
		//F16 going to the left
		if (enemy[i].visible == 14)
		{
			//decide if we want to shoot
			if ((screenrand() < 3) && (enemy[i].y < player.y))
			{
				//F16 shoots regular old bullets
				enemyBullet(enemy, player, i);
			}

			if (enemy[i].y > 50)
			{
				//go to the right if low enough
				enemy[i].x--;
				enemy[i].position.x--;
			}

			if (enemy[i].y < 212)
			{		
			 	enemy[i].y += 2;
			 	enemy[i].position.y += 2;
			 	//engine animation
			 	if (enemy[i].b)
			 	{ 
			 		enemy[i].image.address = 0x000DA7D0;
			 		enemy[i].b = 0;
			 	}
			 	else
			 	{
			 		enemy[i].image.address = 0x000DAEA1;
			 		enemy[i].b = 1;
			 	}	
			 }
			else
				enemy[i].visible = 0;
		}		

		//behavior for chopper
		if (enemy[i].visible == 15)
		{
			//decide if we want to shoot
			if ((screenrand() < 4) && (enemy[i].y < player.y))
			{
				//chopper can shoot regular bullets
				enemyBullet(enemy, player, i);
			}
			//if health gets low, retreat
			if (enemy[i]. a < 3)
			{
				enemy[i].y -=3 ;
				enemy[i].position.y -= 3; 
			}
			//else move towards target x, y
			else if (enemy[i]. y < enemy[i].b)
			{
				if (enemy[i].alpha == 1)
				{
					if (enemy[i].x < player.x)
					{		
					 	enemy[i].x++;
					 	enemy[i].position.x++;
					}
					else if (enemy[i].x > player.x)
					{
						enemy[i].x--;
					 	enemy[i].position.x--;
					}
				}
				enemy[i].y += 2;
				enemy[i].position.y += 2;
			}
			//engine animation
			 if (enemy[i].alpha == 1)
			 { 
			 	enemy[i].image.address = 0x000D809F;
			 	enemy[i].alpha = 2;
			 }
			 else if (enemy[i].b == 1)
			 {
			 	enemy[i].image.address = 0x000D80B2;
			 	enemy[i].b++;
			 }
			 else if (enemy[i].alpha == 2)
			 {
			 	enemy[i].image.address = 0x000D80C5;
			 	enemy[i].alpha=1;
			 }
			 else if (enemy[i].b == 3)
			 {
			 	enemy[i].image.address = 0x000D80B2;
			 	enemy[i].b = 0;
			 }
			 if (enemy[i].y > 240)
		 	 {
				enemy[i].visible = 0;
			 }
		}
	
		//behavior for F14 going straight down
		if (enemy[i].visible == 16)
		{
			//decide if we want to shoot
			if (enemy[i].y == 15)
			{
				//F14 shoots missiles
				initEnemy(enemy, player, 160, i, 0);
			}
			if (enemy[i].y < 212)
			{		
			 	enemy[i].y += 3;
			 	enemy[i].position.y += 3;
			 	//engine animation
			 	if (enemy[i].b)
			 	{ 
			 		enemy[i].image.address = 0x000D7A30;
			 		enemy[i].b = 0;
			 	}
			 	else
			 	{
			 		enemy[i].image.address = 0x000D9DF7;
			 		enemy[i].b = 1;
			 	}	
			 }
			else
				enemy[i].visible = 0;
		}
		
		//behavior for AA gun straight down
		if ((enemy[i].visible >= 20) && (enemy[i].visible <= 30))
		{
			//decide if we want to shoot
			if (enemy[i].y == 50 || enemy[i].y == 100)
			{
				//AA gun shoots missiles
				initEnemy(enemy, player, 160, i, 0);
			}
			if (enemy[i].y < 212)
			{		
			 	enemy[i].y += 1;
			 	enemy[i].position.y += 1;

			 }
			else
				enemy[i].visible = 0;
		}
		
		//bullet ID 150 (fire straight down)
		else if (enemy[i].visible == 150)
		{
			if (enemy[i].y < 235)
			{
			 	enemy[i].y += 4;
			 	enemy[i].position.y += 4;
			}
			else
				enemy[i].visible = 0;
		}
		//bullet ID 151 (45 deg right)
		else if (enemy[i].visible == 151)
		{
			if ((enemy[i].y < 235) && (enemy[i].x < 154))
			{
				enemy[i].x += 2;
			 	enemy[i].y += 4;
			 	enemy[i].position.x += 2;
			 	enemy[i].position.y += 4;
			}
			else
				enemy[i].visible = 0;
		}
		//bullet ID 152 (45 deg left)
		else if (enemy[i].visible == 152)
		{
			if ((enemy[i].y < 235) && (enemy[i].x > 2))
			{
				enemy[i].x -= 2;
			 	enemy[i].y += 4;
			 	enemy[i].position.x -= 2;
			 	enemy[i].position.y += 4;
			}
			else
				enemy[i].visible = 0;
		}
		//bullet ID 153 (22.5 deg right)
		else if (enemy[i].visible == 153)
		{
			if ((enemy[i].y < 235) && (enemy[i].x < 154))
			{
				enemy[i].x += 1;
			 	enemy[i].y += 4;
			 	enemy[i].position.x += 1;
			 	enemy[i].position.y += 4;
			}
			else
				enemy[i].visible = 0;
		}
		//bullet ID 154 (22.5 deg right)
		else if (enemy[i].visible == 154)
		{
			if ((enemy[i].y < 235) && (enemy[i].x < 154))
			{
				enemy[i].x -= 1;
			 	enemy[i].y += 4;
			 	enemy[i].position.x -= 1;
			 	enemy[i].position.y += 4;
			}
			else
				enemy[i].visible = 0;
		}
	
		//missile ID 160 (straight down)
		else if (enemy[i].visible == 160)
		{
			if (enemy[i].x < (player.x - 10))
			{		
				enemy[i].image.address = 0x000D9E23;
			 	enemy[i].x +=2;
			 	enemy[i].position.x +=2;
			}
			else if (enemy[i].x > (player.x + 10))
			{
				enemy[i].image.address = 0x000D9E1E;
				enemy[i].x -=2;
			 	enemy[i].position.x -=2;
			}
			else 
			{
				enemy[i].image.address = 0x000D9E19;
				enemy[i].y += 2;
				enemy[i].position.y += 2;
			 }
			 
			if (enemy[i].y < 235)
			{
				enemy[i].y +=4;
				enemy[i].position.y += 4;
			}
			else 
				enemy[i].visible = 0;
		}
	
		///////////////////////////////////////////////////////////
		//START OF EFFECT DEFINITIONS (Bullets, explosions)
		//
		//ID:200 -> ID: 255
		//
		
		//Explosion #1
		//ID = [200, 208]
		else if ( (enemy[i].visible >= 200) && (enemy[i].visible <= 208))
		{
			//beginning of explosion
			if (enemy[i].visible == 200)
			{
				enemy[i].a = 0; // animation counter
				enemy[i].image.address = 0x000DD7C0;
				enemy[i].image.width = 20;
				enemy[i].image.lines = 40;
				enemy[i].visible++;
			} 
			else if (enemy[i].visible == 208)
			{
				enemy[i].image.address += 20;
				enemy[i].visible = 0;//make unit 'destroyed'
			}
			else
			{
				enemy[i].a++;
				if (enemy[i].a == 3)
				{
					enemy[i].a = 0;
					enemy[i].image.address += 20;
					enemy[i].visible++;
				}
			}
		}
		else if ( (enemy[i].visible >= 209) && (enemy[i].visible <= 210))
		{
			//beginning of explosion
			if (enemy[i].visible == 209)
			{
				enemy[i].a = 0; // animation counter
				enemy[i].image.address = 0x00111160;
				enemy[i].image.width = 5;
				enemy[i].image.lines = 10;
				enemy[i].visible++;
			}
			else if (enemy[i].visible == 210)
			{
				enemy[i].image.address += 5;
				enemy[i].visible = VISIBLE_Dead;
			}
			else
			{
				enemy[i].a++;
				if (enemy[i].a == 3)
				{
					enemy[i].a = 0;
					enemy[i].image.address += 5;
					enemy[i].visible++;
				}
			}
		}
		//Also handle all projectiles here
		else if (enemy[i].visible == VISIBLE_PlayerBullet)
		{
			if (enemy[i].y > 10)
			{
			 	enemy[i].y -= 10;
			 	enemy[i].position.y -= 10;
			 }
			else
				enemy[i].visible = 0;
		}
	}		
}

signed char min(signed char x, signed char y)
{
	if (x < y)
		return x;
	else
		return y;
}

void drawkills(char numKills)
{
	static Sprite kill;
	static char init = 0;

	if (!init)
	{
		kill.image.address = 0x00111C00;
		kill.image.lines = 4;
		kill.image.width = 5;
		kill.position.y = 12;
		kill.alpha = 1;
		kill.visible = 1;

		init = 1;
	}

	signed char kills = (signed char)numKills;
	char groups = 0;

	kill.position.x = 154;
	while (kills > 0)
	{
		kill.image.address = 0x00111C00 + kill.image.width * (min(5, kills) - 1);
		kills -= 5;

		drawsprite(kill);

		++groups;
		if (groups == 3 || groups == 7)
		{
			//
			// skip over ridges in ship
			//
			kill.position.x -= 5;
		}

		kill.position.x -= 6;
	}

}

#pragma OPTIMIZE "0"
void gameMain()
{	
	unsigned int line_counter = 0;
	
	//keeps track of the player's roll value
	char playerx;
	char playery;
	char i, j;
	
	//declare the array of spawn events
	rom char *types =  "\
10\10\10\10\10\10\10\10\10\10\
13\13\13\13\13\14\14\14\14\14\
15\15\11\11\12\10\10\11\15\15\
10\10\10\10\10\10\10\10\10\10\
10\10\10\10\10\10\10\10\10\10\
10\10\10\10\10\10\10\10\10\10\
10\10\10\10\10\10\10\10\10\10\
10\10\10\10\10\10\10\10\10\10\
10\10\10\10\10\10\10\10\10\10\
13\13\13\13\13\14\14\14\14\14\
\255";
					    
	//and the corresponding relative times in which they happen
	rom char *times =  "\
100\20\255\20\20\100\15\15\15\15\
200\50\50\50\50\50\50\50\50\50\
100\100\50\60\30\50\60\30\100\
255\255\255\255\255\255\255\255\255\
255\255\255\255\255\255\255\255\255\
255\255\255\255\255\255\255\255\255\
255\255\255\255\255\255\255\255\255\
255\255\255\255\255\255\255\255\255\
255\255\255\255\255\255\255\255\255\
255\255\255\255\255\255\255\255\255\
40\40\40\40\40\40\40\40\40\40\
\100";

	//and where to put the unit
	rom char *xcoords = "\
10\30\50\70\30\70\90\120\140\130\
20\20\20\20\20\140\140\140\140\140\
50\86\80\50\30\100\120\100\80\90\
10\10\10\10\10\10\10\10\10\10\
10\10\10\10\10\10\10\10\10\10\
10\10\10\10\10\10\10\10\10\10\
10\10\10\10\10\10\10\10\10\10\
10\10\10\10\10\10\10\10\10\10\
10\10\10\10\10\10\10\10\10\10\
10\10\10\10\10\10\10\10\10\10\
10\10\10\10\10\140\140\140\140\140\
\50";
					   
	char currenttype = types[0];
	char nextevent = times[0];
	char currentxcoord = xcoords[0];
	unsigned int eventindex = 0;
	char nextbank = 0;
	
	//initalize game stuff here
	Bitmap world;
			world.address = 0x00024D200;
			world.lines = 0xF0;
			world.width = 0xA0;

	Sprite player;
			player.image.address = 0x000D7A20;
			player.image.lines = 28;
			player.image.width = 16;
			player.position.x = 80;
			player.position.y = 240;
			player.x = 80;
			player.y = 240;
			player.alpha = 1 + 5;	// stores alpha-ness and health (5 here)
			player.visible = VISIBLE_Player;
			player.a = 15;
			player.b = 0;
	playerKills = 0;
			
	Sprite heatmeter;
			heatmeter.image.address = 0x000DF0C0;
			heatmeter.image.lines = 8;
			heatmeter.image.width = 30;
			heatmeter.position.x = 0;
			heatmeter.position.y = 230;
			heatmeter.alpha = 1;
			heatmeter.visible = 1;
			heatmeter.a = 2;			//stores the current animation frame
			heatmeter.b = 0;
			
	Sprite healthmeter;
			healthmeter.image.address = 0x0011116A;
			healthmeter.image.lines = 8;
			healthmeter.image.width = 30;
			healthmeter.position.x = 130;
			healthmeter.position.y = 230;
			healthmeter.alpha = 1;
			healthmeter.visible = 1;
			healthmeter.a = 0;			//stores the current animation frame
			healthmeter.b = 0;
						
	//Topgraphic is the border graphic
	//clear screen and also check for exiting
	Sprite border;
			border.image.lines = 1;
			border.image.address = 0x00111F20;
			border.image.width = 160;
			border.position.x = 0;
			border.position.y = 0;
			border.alpha = 0;
			border.visible = 1;

	//each bank of enemies handles 12 objects (PIC 18 limitation)	
	Sprite enemy_a[12];
	Sprite enemy_b[12];
	Sprite missile[12];
	
	//initialize data structures for fighter wave
	blankEnemies(enemy_a);
	blankEnemies(enemy_b);

	//move player 1 ship onto the screen (intro sort of)
	for (i = 0; i < 2; i++)
	{
		for (line_counter = 0; line_counter < 100; line_counter++)
		{	
			drawtobackground(world);
			drawsprite(player);
			flip_buffer(fb);
			
			if (i == 1)
			{
				player.y --;
				player.position.y --;
			}	
			
			delay_ms(20 + (i*10));
			world.address -= 0xA0;
		}	
		
		//reset scrolling background back to beginning
		world.address = 0x00024D200;
	}
	
	blankEnemies(missile);
		
	//handle regular scroll section
	for (line_counter = 0; line_counter < 8750; line_counter++)
	{
		if (line_counter == 8650)
		{
			player.visible = 3;
		}
		
		//////////////////////////////////
		//collision and movement go here
				
		//check collision between enemy and player
		collideArrayPlayer(enemy_a, player);
		collideArrayPlayer(enemy_b, player);
		collideArrayArray(enemy_a, missile);
		collideArrayArray(enemy_b, missile);
		
		if (newinput)
		{
			newinput = 0;
			inputHandler(player, missile);	
		}
		movementHandler(player);
		
		//handle enemy array
		enemyHandler(missile, player);
		enemyHandler(enemy_a, player);
		enemyHandler(enemy_b, player);
				
		///////////////////////////////////
		//Line events go here!
		
		//when it's time for the next event
		if (nextevent == 0)
		{
			//event 255 means make player exit through top of screen
			if (currenttype == 255)
			{
				player.visible = 3;
				break;
			}
			//find the LRU bank and spawn enemy in that bank
			if (nextbank == 0)
			{
				initEnemy(enemy_a, player, currenttype, 0, currentxcoord);
                nextbank = 1;
			}
			if (nextbank == 1)
			{
				initEnemy(enemy_b, player, currenttype, 0, currentxcoord); 
				nextbank = 0;
			}
			//grab the next event;
			eventindex ++;
			currenttype = types[eventindex];
			nextevent = times[eventindex];
			currentxcoord = xcoords[eventindex];
		}
		nextevent --;
		
		///////////////////
		//handle specific code for the heatmeter
		
		//if overheating, just strobe the overheating indicators
		if (player.b > 16)
		{
			if (heatmeter.b)
			{
				heatmeter.image.address = 0x000DF5C0;
				heatmeter.b = 0;
			}
			else 
			{
				heatmeter.image.address = 0x000DF5DE;
				heatmeter.b = 1;
			}
		}
		//otherwise increment indicator based on heat value
		else
		{
			heatmeter.image.address = 0x000DF0C0 + (unsigned long)((player.b>>2) * 30);
			//use heatmeter.a as a counter to determine when we should decrease the player's heat value
		}

		char health = player.alpha - 1;
		if (health > 1)
		{
			char offset = (5 - health) * 30;
			healthmeter.image.address = 0x0011116A + (unsigned long)offset;
		}
		else if (health == 1)
		{
			//
			// 1 health left, blink it
			//
			healthmeter.b++;
			if (healthmeter.b % 10 < 5)
			{
				healthmeter.image.address = 0x0011166A;
			}
			else
			{
				healthmeter.image.address = 0x0011166A + 30;
			}

			if (healthmeter.b == 10)
				healthmeter.b = 0;
		}
		else
		{
			//
			// player is dead
			//
			healthmeter.image.address = 0x0011166A + 30;
		}

		//handle player's roll decay
		if (player.b > 0)
		{
			player.b--;
		}		

		//handle flashing of screen
		if (playerGotHit)
		{
			playerGotHit --;
			if (playerGotHit.0)
			{
				set_fieldcolor (0b11000000);
			}
			else
			{
				set_fieldcolor (0b00000000);
			}
		}

		//HANDLE BORDER CRAP HERE
		if (border.image.lines < 24)
		{
			border.image.lines++;
		}
		
		//////////////////////////////////
		//DRAW GOES HERE
		drawtobackground(world);
		drawsprite(player);
		drawarray(enemy_a);
		drawarray(enemy_b);
		drawarray(missile);
		drawsprite(border);
		drawkills(playerKills);
		drawsprite(heatmeter);
		drawsprite(healthmeter);

		//postdraw stuff
		flip_buffer(fb);
		world.address -= 0xA0;	
		delay_ms(20);
		
		if (!player.visible)
		{
			playmusic(255);
			delay_s(1);
			currentstate = 1;
			return;
		}
	}	
	currentstate = 0;
	return;
}
	
void main()
{
	//various variables related only to main
	char i = 0;
		
	//initalize GPU ports
	gpuinit();
	serialinit();
	newinput = 0;	
	set_drawmode(0);
	delay_s(2);						//wait for system to boot
	bootup();						//draw bootup logo
	currentstate = 0;
	
	//main event endless loop
	while( true )
	{		
		//clear input flag
		
		//determines if we should be in the game or the intro
		
		if (newinput)
		{
			newinput = 0;
			//advance to title
			currentstate = 1;
		}	
		if (currentstate == 0) 
		{
			set_drawmode(0);
			introMain();
		}
		if (currentstate == 1)
		{
			set_drawmode(0);
			drawTitle();
		}
	
		if (currentstate == 2)
		{
			set_drawmode(1);
			gameMain();
		}
	}
}
