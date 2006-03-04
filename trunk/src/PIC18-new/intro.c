#include "intro.h"
#include "input.h"
#include "spu.h"
#include "delay.h"
#include "figure.h"

void cls(uint8 color)
{
    //clear screen and also check for exiting
    char i;
    Figure me;

    me.image.height = 24;
    me.image.width = 160;
    me.position.x = 0;
    me.position.y = 0;
    me.alpha = 0;
    me.visible = 1;
    
    if (color == 0)
        me.image.address = 0x000B6F80;
    else if (color == 1)
        me.image.address = 0x000B7E80;
    else if (color == 2)
        me.image.address = 0x000B8D80;
    else if (color == 3)
        me.image.address = 0x000B9C80;
    for ( i = 0; i < 10; i++)
    {
        drawFigure(&me);
        me.position.y += 24;
    }
    return;
}

void fadeInOut(void)
{
    char i;
    //fade in and out at the end of bootup
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
    return;
}

bool showScene1(void)
{
    char i = 0;
    char j = 0;

    Figure near_future;
    Figure eye_anim;
    Figure eye_mask;
    Figure compass;
    Figure lines;
    static Figure bars;
    static Figure outline;
        
    //initalize graphics to original settings
    
    near_future.image.address = 0x00025800;
    near_future.image.height = 0x0B;
    near_future.image.width = 0xA0;
    near_future.position.x = 0;
    near_future.position.y = 63;
    near_future.alpha = 0;
    near_future.visible = 1;

    eye_anim.image.address = 0x00026CA0;
    eye_anim.image.height = 0x1B;
    eye_anim.image.width = 0x1F;
    eye_anim.position.x = 14;
    eye_anim.position.y = 135;
    eye_anim.alpha = 0;
    eye_anim.visible = 1;

    eye_mask.image.address = 0x0001C200;
    eye_mask.image.height = 0x1B;
    eye_mask.image.width = 0x1F;
    eye_mask.position.x = 14;
    eye_mask.position.y = 135;
    eye_mask.alpha = 0;
    eye_mask.visible = 1;
        
    compass.image.address = 0x00029A40;
    compass.image.height = 0x06;
    compass.image.width = 0x2D;
    compass.position.x = 51;
    compass.position.y = 134;
    compass.alpha = 0;
    compass.visible = 1;

    lines.image.address = 0x00029E00;
    lines.image.height = 0x1A;
    lines.image.width = 0x4F;
    lines.position.x = 73;
    lines.position.y = 76;
    lines.alpha = 0;
    lines.visible = 1;

    bars.image.address = 0x0002AE94;
    bars.image.height = 15;
    bars.image.width = 0x2F;
    bars.position.x = 10;
    bars.position.y = 81;
    bars.alpha = 0;
    bars.visible = 1;

    outline.image.address = 0x00027D80;
    outline.image.height = 46;
    outline.image.width = 43;
    outline.position.x = 27;
    outline.position.y = 130;
    outline.alpha = 1;
    outline.visible = 1;

    //fade in introduction text
    cls(0);
    if (delayS(1)) return true;
    playMusic (1);
    if (delayMs(500)) return true;
    drawFigure(&near_future);
    if (delayMs(250)) return true;
    near_future.image.address = 0X00025EE0;
    cls(0);
    drawFigure(&near_future);
    if (delayMs(250)) return true;
    near_future.image.address = 0X000265C0;
    cls(0);
    drawFigure(&near_future);
    if (delayS(2)) return true;
    //black screen - dramatic pause
    cls(0);
    if (delayS(2)) return true;
    //draw eye open animation
    for (i = 0; i < 5; i++)
    {
        drawFigure(&eye_mask);
        drawFigure(&eye_anim);
        eye_anim.image.address += 0x00000020;
        if (delayMs(100)) return true;
    }
    //draw and animate compass
    j = 30;
    for ( i = 0; i < 5; i++)//slow down
    {
        drawFigure(&compass);//1
        compass.image.address += 0x0000002E;
        if (delayMs(j)) return true;
        drawFigure(&compass);//2
        compass.image.address += 0x0000002E;
        if(delayMs(j)) return true;
        drawFigure(&compass);//3
        compass.image.address -= 0x0000005C;    
        if (delayMs(j)) return true;
        j+= 15;
    }
    j=20;
    for ( i = 0; i < 5; i++)//reverse
    {
        compass.image.address += 0x0000005C;
        drawFigure(&compass);//3
        compass.image.address -= 0x0000002E;
        if (delayMs(j)) return true;
        drawFigure(&compass);//2
        compass.image.address -= 0x0000002E;
        if (delayMs(j)) return true;
        drawFigure(&compass);//1
        if (delayMs(j)) return true;
        j+= 15;
    }
    j=150;
    for ( i = 0; i < 3; i++)//speed up
    {
        drawFigure(&compass);//1
        compass.image.address += 0x0000002E;
        if (delayMs(j)) return true;
        drawFigure(&compass);//2
        compass.image.address += 0x0000002E;
        if (delayMs(j)) return true;
        drawFigure(&compass);//3
        compass.image.address -= 0x0000005C;    
        if (delayMs(j)) return true;
        j-= 60;
    }
    drawFigure(&lines);
    drawFigure(&bars);

    j=10;
    for (i = 0; i < 2; i++)//slow down
    {
        drawFigure(&compass);
        compass.image.address += 0x0000002E;
        if (delayMs(j)) return true;
        drawFigure(&compass);//2
        compass.image.address += 0x0000002E;
        if (delayMs(j)) return true;
        drawFigure(&compass);//3
        compass.image.address -= 0x0000005C;    
        if (delayMs(j)) return true;
        j+= 40;
    }

    lines.image.address = 0x00029E50;
    lines.image.width = 0x4B;
    bars.image.address += 0x00000960;
    bars.image.width = 0x35;
    drawFigure(&lines);
    drawFigure(&bars);
    
    drawFigure(&compass);
    compass.image.address += 0x0000002E;
    if (delayMs(j)) return true;
    drawFigure(&compass);//2
    compass.image.address += 0x0000002E;
    if (delayMs(j)) return true;
    drawFigure(&compass);//3
    compass.image.address -= 0x0000005C;    
    if (delayMs(j)) return true;
    j+= 40;

    lines.image.address = 0x0002AE40;
    lines.image.width = 0x53;
    bars.image.address += 0x0000960;
    drawFigure(&lines);
    drawFigure(&bars);
    
    drawFigure(&compass);
    compass.image.address += 0x0000002E;
    if (delayMs(j)) return true;
    drawFigure(&compass);//2
    compass.image.address += 0x0000002E;
    if (delayMs(j)) return true;
    drawFigure(&compass);//3
    compass.image.address -= 0x0000005C;    
    if (delayMs(j)) return true;
    drawFigure(&compass);//1
    
    lines.image.address = 0x0002BE80;
    bars.image.address += 0x00000961;
    bars.image.width = 0x36;
    drawFigure(&lines);
    drawFigure(&bars);
    if (delayMs(200)) return true;
        
    lines.image.address = 0x0002CEC0;
    lines.image.height = 32;
    lines.image.width = 84;
    lines.position.x = 72;
    lines.position.y = 75;
    bars.image.address += 0x00000960;
    drawFigure(&bars);    
    drawFigure(&lines);
    if (delayMs(50)) return true;
    bars.image.address = 0x000713E0;
    bars.image.height = 21;
    bars.image.width = 62;
    bars.position.x = 4;
    bars.position.y = 80;
    drawFigure(&bars);
    drawFigure(&lines);
    
    //glitch stuff up
    for (i = 0; i < 10; i++)
    {
        bars.image.address = 0x000713E0;
        lines.image.address = 0x000821E0;
        drawFigure(&bars);
        drawFigure(&lines);
        if (delayMs(25)) return true;
        bars.image.address = 0x000706C0;
        lines.image.address = 0x0002CEC0;
        drawFigure(&bars);
        drawFigure(&lines);
        if (delayMs(25)) return true;            
    }    

    //draw overlay
    
    drawFigure(&outline);        

    for (i = 0; i < 30; i++)
    {
        bars.image.address = 0x000713E0;
        lines.image.address = 0x000821E0;
        drawFigure(&bars);
        drawFigure(&lines);
        if (delayMs(5)) return true;
        bars.image.address = 0x000706C0;
        lines.image.address = 0x0002CEC0;
        drawFigure(&bars);
        drawFigure(&lines);
        if (delayMs(5)) return true;            
    }        

    //draw eye close animation with overlay
    eye_anim.image.address = 0x00026D20;
    for (i = 0; i < 5; i++)
    {
        drawFigure(&eye_mask);            
        drawFigure(&eye_anim);
        drawFigure(&outline);
        eye_anim.image.address -= 0x00000020;
        if (delayMs(50)) return true;
    }
    //draw eye open with overlay
    eye_anim.image.address = 0x00026CA0;
    for (i = 0; i < 5; i++)
    {
        drawFigure(&eye_mask);
        drawFigure(&eye_anim);
        drawFigure(&outline);
        eye_anim.image.address += 0x00000020;
        if (delayMs(50)) return true;
    }
    if (delayMs(250)) return true;
    //draw rest of overlay
    drawFigure(&eye_mask);
    outline.position.x = 24;
    outline.image.address = 0x00027DAB;
    outline.image.width = 46;
    drawFigure(&outline);
    drawFigure(&compass);
    if (delayMs(200)) return true;
    outline.image.address = 0x00027DD9;
    outline.image.width = 60;
    drawFigure(&outline);
    drawFigure(&compass);
    if (delayS(2)) return true;
    //if no input, return falses
    return false;
}

bool showScene2(void)
{
    char i = 0;
    char j = 0;
    
    //initalize graphics to original settings
    
    Figure pilot;
    Figure screen0;
    Figure screen1;
    Figure screen2;
    Figure screen3;
    static Figure screen4;
    static Figure screen5;
    static Figure screen6;
    static Figure screen7;
    static Figure boot0;
    static Figure boot1;
    static Figure boot2;
    static Figure boot3;
    static Figure boot4;
    static Figure boot5;

    pilot.image.address = 0x000AB180;//0x000A4740;
    pilot.image.height = 170;
    pilot.image.width = 77;
    pilot.position.x = 20;
    pilot.position.y = 39;
    pilot.alpha = 1;
    pilot.visible = 1;

    screen0.image.address = 0x000A86AD;
    screen0.image.height = 69;
    screen0.image.width = 40;
    screen0.position.x = 72;
    screen0.position.y = 71;
    screen0.alpha = 1;
    screen0.visible = 1;
    
    screen1.image.address = 0x000A4F0D;
    screen1.image.height = 89;
    screen1.image.width = 59;
    screen1.position.x = 72;
    screen1.position.y = 59;
    screen1.alpha = 1;
    screen1.visible = 1;

    screen2.image.address = 0x0009E5C0;
    screen2.image.height = 156;
    screen2.image.width = 57;
    screen2.position.x = 49;
    screen2.position.y = 25;
    screen2.alpha = 1;
    screen2.visible = 1;
    
    screen3.image.address = 0x0009E5F9;
    screen3.image.height = 156;
    screen3.image.width = 38;
    screen3.position.x = 106;
    screen3.position.y = 25;
    screen3.alpha = 1;
    screen3.visible = 1;
    
    screen4.image.address = 0x0009A060;
    screen4.image.height = 111;
    screen4.image.width = 40;
    screen4.position.x = 13;
    screen4.position.y = 3;
    screen4.alpha = 1;
    screen4.visible = 1;

    screen5.image.address = 0x0009A095;
    screen5.image.height = 111;
    screen5.image.width = 21;
    screen5.position.x = 67;
    screen5.position.y = 3;
    screen5.alpha = 1;
    screen5.visible = 1;

    screen6.image.address = 0x00099EEB;
    screen6.image.height = 17;
    screen6.image.width = 17;
    screen6.position.x = 120;
    screen6.position.y = 2;
    screen6.alpha = 1;
    screen6.visible = 1;
    
    screen7.image.address = 0x000AA575;
    screen7.image.height = 20;
    screen7.image.width = 16;
    screen7.position.x = 143;
    screen7.position.y = 188;
    screen7.alpha = 1;
    screen7.visible = 1;
    
    boot0.image.address = 0x000A478D;
    boot0.image.height = 12;
    boot0.image.width = 59;
    boot0.position.x = 6;
    boot0.position.y = 15;
    boot0.alpha = 1;
    boot0.visible = 1;
    
    boot1.image.address = 0x000A0EBF;
    boot1.image.height = 9;
    boot1.image.width = 65;
    boot1.position.x = 6;
    boot1.position.y = 29;
    boot1.alpha = 1;
    boot1.visible = 1;
    
    boot2.image.address = 0x000A145F;
    boot2.image.height = 9;
    boot2.image.width = 65;
    boot2.position.x = 6;
    boot2.position.y = 44;
    boot2.alpha = 1;
    boot2.visible = 1;
    
    boot3.image.address = 0x000A19FF;
    boot3.image.height = 9;
    boot3.image.width = 26;
    boot3.position.x = 6;
    boot3.position.y = 58;
    boot3.alpha = 1;
    boot3.visible = 1;

    boot4.image.address = 0x00099AC0;
    boot4.image.height = 9;
    boot4.image.width = 60;
    boot4.position.x = 6;
    boot4.position.y = 73;
    boot4.alpha = 1;
    boot4.visible = 1;

    boot5.image.address = 0x00099AFC;
    boot5.image.height = 9;
    boot5.image.width = 60;
    boot5.position.x = 66;
    boot5.position.y = 73;
    boot5.alpha = 1;
    boot5.visible = 1;
        
    cls(0);
    if (delayS(1)) return true;
    drawFigure(&pilot);
    if (delayMs(200)) return true;
    drawFigure(&screen0);
    //flash newest screen
    for (i = 0; i < 10; i++)
    {
        cls(0);
        drawFigure(&pilot);
        if (delayMs(25)) return true;
        drawFigure(&screen0);
        if (delayMs(25)) return true;
    }    
    drawFigure(&screen1);    
    for (i = 0; i < 10; i++)
    {
        cls(0);
        drawFigure(&pilot);
        drawFigure(&screen0);
        if (delayMs(25)) return true;
        drawFigure(&screen1);
        if (delayMs(25)) return true;
    }    
    
    drawFigure(&screen2);
    drawFigure(&screen3);        
    for (i = 0; i < 10; i++)
    {
        cls(0);
        drawFigure(&pilot);
        drawFigure(&screen0);
        drawFigure(&screen1);
        if (delayMs(25)) return true;
        drawFigure(&screen2);
        drawFigure(&screen3);
        if (delayMs(25)) return true;
        if (i == 3)
            pilot.image.address = 0x000AB1CD;
    }    


    if (delayMs(255)) return true;
    drawFigure(&screen4);
    drawFigure(&screen5);
    drawFigure(&screen6);
    drawFigure(&screen7);
    for (i = 0; i < 10; i++)
    {
        cls(0);
        drawFigure(&pilot);
        drawFigure(&screen0);
        drawFigure(&screen1);
        drawFigure(&screen2);
        drawFigure(&screen3);
        if (delayMs(25)) return true;
        drawFigure(&screen4);
        drawFigure(&screen5);
        drawFigure(&screen6);
        drawFigure(&screen7);
        if (delayMs(25)) return true;
        if (i == 7)
            pilot.image.address = 0x000A4740;
        
    }    

    if (delayS(1)) return true;
    drawFigure(&boot0);
    if (delayMs(255)) return true;
    if (delayMs(255)) return true;
    drawFigure(&boot1);
    if (delayMs(255)) return true;
    if (delayMs(255)) return true;
    if (delayMs(255)) return true;
    drawFigure(&boot2);
    if (delayS(2)) return true;
    drawFigure(&boot3);
    if (delayMs(255)) return true;
    drawFigure(&boot4);
    drawFigure(&boot5);
    if (delayS(3)) return true;
    //if no input, return falses
    return false;
}

bool showScene3(void)
{
    char i = 0;
    char j = 0;
    bool pressed = false;

    Figure* cannon1 = new(Figure);
    Figure* cannon2 = new(Figure);
    Figure* cannon3 = new(Figure);
    Figure* credit1 = new(Figure);
    Figure* credit2 = new(Figure);
    Figure* credit3 = new(Figure);
    Figure* credit4 = new(Figure);
    Figure* credit5 = new(Figure);
    Figure* credit6 = new(Figure);
    Figure* grid = new(Figure);
    Figure* plane_1 = new(Figure);
    Figure* plane_2 = new(Figure);
    Figure* tech_1 = new(Figure);
    Figure* tech_2 = new(Figure);

    cannon1->image.address = 0x0005308A;
    cannon1->image.height = 186;
    cannon1->image.width = 44;
    cannon1->position.x = 10;
    cannon1->position.y = 40;
    cannon1->alpha = 0;
    cannon1->visible = 1;
    
    cannon2->image.address = 0x0005A4CA;
    cannon2->image.height = 186;
    cannon2->image.width = 44;
    cannon2->position.x = 10;
    cannon2->position.y = 40;
    cannon2->alpha = 0;
    cannon2->visible = 1;
    
    cannon3->image.address = 0x00064280;
    cannon3->image.height = 186;
    cannon3->image.width = 44;
    cannon3->position.x = 10;
    cannon3->position.y = 40;
    cannon3->alpha = 0;
    cannon3->visible = 1;

    credit1->image.address = 0x000B1BC0;
    credit1->image.height = 45;
    credit1->image.width = 60;
    credit1->position.x = 65;
    credit1->position.y = 80;
    credit1->alpha = 1;
    credit1->visible = 1;
    
    credit2->image.address = 0x000B1BFC;
    credit2->image.height = 45;
    credit2->image.width = 60;
    credit2->position.x = 65;
    credit2->position.y = 140;
    credit2->alpha = 1;
    credit2->visible = 1;
            
    credit3->image.address = 0x000B37E0;
    credit3->image.height = 45;
    credit3->image.width = 60;
    credit3->position.x = 10;
    credit3->position.y = 120;
    credit3->alpha = 1;
    credit3->visible = 1;

    credit4->image.address = 0x000B381C;
    credit4->image.height = 45;
    credit4->image.width = 60;
    credit4->position.x = 10;
    credit4->position.y = 180;
    credit4->alpha = 1;
    credit4->visible = 1;
    
    credit5->image.address = 0x000B5400;
    credit5->image.height = 45;
    credit5->image.width = 60;
    credit5->position.x = 99;
    credit5->position.y = 120;
    credit5->alpha = 1;
    credit5->visible = 1;

    credit6->image.address = 0x000B543C;
    credit6->image.height = 45;
    credit6->image.width = 60;
    credit6->position.x = 99;
    credit6->position.y = 180;
    credit6->alpha = 1;
    credit6->visible = 1;
    
    grid->image.address = 0x00052F80;
    grid->image.height = 150;
    grid->image.width = 105;
    grid->position.x = 40;
    grid->position.y = 0;
    grid->alpha = 0;
    grid->visible = 1;

    plane_1->image.address = 0x00064221;
    plane_1->image.height = 150;
    plane_1->image.width = 49;
    plane_1->position.x = 50;
    plane_1->position.y = 0;
    plane_1->alpha = 1;
    plane_1->visible = 1;
    
    plane_2->image.address = 0x00064252;
    plane_2->image.height = 150;
    plane_2->image.width = 46;
    plane_2->position.x = 99;
    plane_2->position.y = 0;
    plane_2->alpha = 1;
    plane_2->visible = 1;

    tech_1->image.address = 0x000717E1;
    tech_1->image.height = 150;
    tech_1->image.width = 49;
    tech_1->position.x = 45;
    tech_1->position.y = 00;
    tech_1->alpha = 1;
    tech_1->visible = 1;
    
    tech_2->image.address = 0x00071812;
    tech_2->image.height = 150;
    tech_2->image.width = 46;
    tech_2->position.x = 94;
    tech_2->position.y = 00;
    tech_2->alpha = 1;
    tech_2->visible = 1;

    cls(0);

    //draw cannon and some credits
    //leadin animations
    drawFigure(cannon1);
    if (delayMs(200)) { pressed = true; goto exit; }
    
    drawFigure(credit1);
    drawFigure(credit2);
    //speed up
    for (i = 0; i < 25; i++)
    {
        drawFigure(cannon2);
        if (delayMs(125 - (i * 4))) { pressed = true; goto exit; }
        drawFigure(cannon3);
        if (delayMs(125 - (i * 4))) { pressed = true; goto exit; }
    }
    //slow down
    for (i = 0; i < 25; i++)
    {
        drawFigure(cannon2);
        if (delayMs(25 + (i * 4))) { pressed = true; goto exit; }
        drawFigure(cannon3);
        if (delayMs(25 + (i * 4))) { pressed = true; goto exit; }
    }
    //leadout animations
    if (delayMs(125)) { pressed = true; goto exit; }
    drawFigure(cannon2);
    if (delayMs(200)) { pressed = true; goto exit; }
    drawFigure(cannon1);
    if (delayMs(200)) { pressed = true; goto exit; }
    cls(0);
    if (delayMs(255)) { pressed = true; goto exit; }
    
    //scroll plane up
    for (i = 0; i < 70; i++)
    {
        drawFigure(grid);
        drawFigure(plane_1);
        drawFigure(plane_2);
        drawFigure(credit3);
        drawFigure(credit4);
        grid->image.address += 160;
        plane_1->image.address += 160;
        plane_2->image.address += 160;
        tech_1->image.address += 160;
        tech_2->image.address += 160;
        if (delayMs(130)) { pressed = true; goto exit; }
    }
    if (delayMs(255)) { pressed = true; goto exit; }


    //move screen left    
    while(grid->position.x > 0)
    {
        grid->position.x -= 1;
        tech_1->position.x -= 1;
        tech_2->position.x -= 1;
        plane_1->position.x -= 1;
        plane_2->position.x -= 1;

        cls(0);    
        drawFigure(grid);
        drawFigure(plane_1);
        drawFigure(plane_2);
        if (delayMs(20)) { pressed = true; goto exit; }
    }
    
    //scroll plane up with overlay
    for (i = 0; i < 70; i++)
    {
        drawFigure(grid);
        drawFigure(plane_1);
        drawFigure(plane_2);
        drawFigure(tech_1);
        drawFigure(tech_2);
        drawFigure(credit5);
        drawFigure(credit6);
        
        grid->image.address += 160;
        plane_1->image.address += 160;
        plane_2->image.address += 160;
        tech_1->image.address += 160;
        tech_2->image.address += 160;
        if (delayMs(130)) { pressed = true; goto exit; }
    }

    cls(0);
    if (delayS(1)) { pressed = true; goto exit; }
    //if no input, return falses

exit:
    free(cannon1);
    free(cannon2);
    free(cannon3);
    free(credit1);
    free(credit2);
    free(credit3);
    free(credit4);
    free(credit5);
    free(credit6);
    free(grid);
    free(plane_1);
    free(plane_2);
    free(tech_1);
    free(tech_2);
    
    return pressed;
}


bool showScene4(void)
{
    char i = 0;
    char j = 0;
    char front_buffer = 0;

    Figure skyline;
    Figure engine;
    Figure plane_1;
    Figure plane_2;
    Figure plane_3;
    static Figure plane_4;

    skyline.image.address = 0x00090510;
    skyline.image.height = 240;
    skyline.image.width = 0;
    skyline.position.x = 80;
    skyline.position.y = 0;
    skyline.alpha = 0;
    skyline.visible = 1;

    engine.image.address = 0x0008AD14;
    engine.image.height = 18;
    engine.image.width = 29;
    engine.position.x = 66;
    engine.position.y = 150;
    engine.alpha = 1;
    engine.visible = 1;
    
    plane_1.image.address = 0x0008B7E0;
    plane_1.image.height = 40;
    plane_1.image.width = 40;
    plane_1.position.x = 0;
    plane_1.position.y = 200;
    plane_1.alpha = 1;
    plane_1.visible = 1;
    
    plane_2.image.address = 0x0008B808;
    plane_2.image.height = 40; 
    plane_2.image.width = 40;
    plane_2.position.x = 40;
    plane_2.position.y = 200;
    plane_2.alpha = 1;
    plane_2.visible = 1;

    plane_3.image.address = 0x0008B830;
    plane_3.image.height = 40;
    plane_3.image.width = 40;
    plane_3.position.x = 80;
    plane_3.position.y = 200;
    plane_3.alpha = 1;
    plane_3.visible = 1;

    plane_4.image.address = 0x0008B858;
    plane_4.image.height = 40; //123;
    plane_4.image.width = 40;
    plane_4.position.x = 120;
    plane_4.position.y = 200;
    plane_4.alpha = 1;
    plane_4.visible = 1;
        
    cls(0);
    
    //this part is done in double buffer mode
    setDoubleBuffer(true);
    //flush video buffers
    cls(0);
    flipBuffer(&front_buffer);
    for (i = 0; i <81 ; i++)
    {
        cls(0);
        drawFigure(&skyline);
        drawFigure(&plane_1);
        drawFigure(&plane_2);
        drawFigure(&plane_3);
        drawFigure(&plane_4);
        flipBuffer(&front_buffer);
        //do some slide fx here
        skyline.image.width +=2;
        skyline.image.address --;
        skyline.position.x --;
    
        plane_1.position.y -= 1;
        plane_2.position.y -= 1;
        plane_3.position.y -= 1;
        plane_4.position.y -= 1;
        plane_1.image.height ++;
        plane_2.image.height ++;    
        plane_3.image.height ++;
        plane_4.image.height ++;
        
        if (delayMs(20 + i)) return true;
    }
    setDoubleBuffer(false);
    if (delayS(2)) return true;
    plane_2.position.y ++;
    plane_3.position.y ++;
    for ( i = 0; i < 5; i++)
    {
        drawFigure(&plane_2);
        drawFigure(&plane_3);
        drawFigure(&engine);
        if (delayMs(255 - (i * 16))) return true;
        engine.image.address -= 29;
    }
    if (delayMs(255)) return true;
    //if no input, return falses
    return false;
}

bool showScene5(void)
{
    char i = 0;
    char j = 0;

    unsigned long takeoff_addr[8] = 
    {
        0x0002E720,
        0x000332C0,
        0x00037E60,
        0x0003CA00,
        0x00040BA0,
        0x00044D40,
        0x00049840,
        0x0004E3E0
    };

    char takeoff_lines[8] =
    {
        121,
        121,
        121,
        105,
        105,
        120,
        120,
        121
    };

    Figure takeoff;

    takeoff.image.address = takeoff_addr[0];
    takeoff.image.height = takeoff_lines[0];
    takeoff.image.width = 160;
    takeoff.position.x = 0;
    takeoff.position.y = 49;
    takeoff.alpha = 0;
    takeoff.visible = 1;
        
    cls(0);
    
    //accelerate plane
    for (i = 0; i < 5 ; i++)
    {
        for ( j = 0; j < 8; j++)
        {
            drawFigure(&takeoff);
            takeoff.image.address = takeoff_addr[j];
            takeoff.image.height = takeoff_lines[j];
            delay_ms (60 - (i * 4));
        }
    }

    //max speed
    for (i = 0; i < 5 ; i++)
    {
        for ( j = 0; j < 8; j++)
        {
            drawFigure(&takeoff);
            takeoff.image.address = takeoff_addr[j];
            takeoff.image.height = takeoff_lines[j];
            if (delayMs(40)) return true;
        }
    }
    cls(0);
    //if no input, return falses
    return false;
}

bool showScene6(void)
{
    char i = 0;
    char j = 0;
    Figure reflect;

    reflect.image.address = 0x0E1140;
    reflect.image.height = 90;
    reflect.image.width = 160;
    reflect.position.x = 0;
    reflect.position.y = 80;
    reflect.alpha = 0;
    reflect.visible = 1;

    cls(0);
    for (i = 0; i < 5 ; i++)
    {
        for ( j = 0; j < 10; j++)
        {
            drawFigure(&reflect);
            reflect.image.address += 0x3840;
            if (delayMs(80)) return true;

        }
        //reset thingy to beginning
        reflect.image.address = 0x0E1140;
    }
    cls(0);
    //if no input, return falses
    return false;
}

void showBootSplash(void)
{
    char i = 0;
    char j = 0;
    Figure frame;

    setDoubleBuffer(false);

    frame.image.address = 0x003767C0;
    frame.image.height = 160;
    frame.image.width = 79;
    frame.position.x = 40;
    frame.position.y = 40;
    frame.alpha = 0;
    frame.visible = 1;
    
    cls(0);
        
    delay_ms(200);

    //play boot movie
    for (i = 0; i <  21; i++)
    {
        //show 'left' frame
        drawFigure(&frame);
        if (i == 0)
        {
            delay_ms(250);
            delay_ms(250);
        }
        
        //show 'right' frame
        if (i > 18)
            delay_ms(100);
        else
            delay_ms(60);
        frame.image.address += 80;
        drawFigure(&frame);
        if (i > 18)
            delay_ms(100);
        else
            delay_ms(60);
        //skip down to next two frames
        frame.image.address += 25520;    
    }
    fadeInOut();
}

/**
 * returns true if we timeout (no input)
 * returns false otherwise
 */
bool showTitle(void)
{
    char i;
    
    Input* event;
    Figure me;

    setDoubleBuffer(false);

    me.image.address = 0x02C43E0;
    me.image.height = 240;
    me.image.width = 160;
    me.position.x = 0;
    me.position.y = 0;
    me.alpha = 0;
    me.visible = 1;

    setFieldColor(0b00000000);
    for ( i = 0; i < 4; i++)
    {
        cls (i);
        delay_ms(50);
    }
    
    delay_ms(250);
    delay_ms(250);
    
    for (i = 0; i < 4; i++)
    {
        cls (3-i);
        delay_ms(50);
    }
    
    for (i = 0; i < 19; i++)
    {
        drawFigure(&me);
        me.image.address += 0x9600;
        delay_ms(100);
    }

    for (i = 0; i < 255; i++)
    {
        event = getInputEvent();
        if (event->startPressed)
        {
            //seed random generator based on time player takes to hit start
            srand(i);
            playMusic(255);
            delay_ms(200);
            playMusic(2);
            return false;
        }
        delay_ms(100);
    }

    playMusic(255);

    return true;
}

void showIntro(void)
{
    // single-buffer mode
    setDoubleBuffer(false);

    delay_s(2);

    if (showScene1() || showScene2() || showScene3() ||
        showScene4() || showScene5() || showScene6())
        return;
}

