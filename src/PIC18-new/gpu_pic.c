#include "gpu_pic.h"

//Loads the source address (registers 0, 1, and 2) into the GPU 
void load_s_addr(uint32 address)
{    
    PORTC = 0x02;                                //Load least significant byte
    PORTB = (uint8) (0x000000FF & address);
    PORTA = 0x01;                                //enable load
    PORTA = 0x00;                                //load complete
    address >>= 8;
    PORTC = 0x01;                                 //Load middle byte
    PORTB = (uint8) (0x000000FF & address);
    PORTA = 0x01;                                //enable load
    PORTA = 0x00;                                //load complete
    address >>= 8;
    PORTC = 0x00;                                 //Load most significant byte
    PORTB = (uint8) (0x000000FF & address);
    PORTA = 0x01;                                //enable load
    PORTA = 0x00;                                //load complete
}

void load_t_addr(uint32 address)
{    
    PORTC = 0x05;                                 
    PORTB = (uint8) (0x000000FF & address);
    PORTA = 0x01;                                
    PORTA = 0x00;                                
    address >>= 8;
    PORTC = 0x04;                                 
    PORTB = (uint8) (0x000000FF & address);
    PORTA = 0x01;                                
    PORTA = 0x00;                            
    address >>= 8;
    PORTC = 0x03;                                 
    PORTB = (uint8) (0x000000FF & address);
    PORTA = 0x01;                            
    PORTA = 0x00;
}

void load_s_lines(uint8 lines)
{    
    PORTC = 0x06;
    PORTB = lines;
    PORTA = 0x01;
    PORTA = 0x00;
}

void load_l_size(uint8 size)
{    
    PORTC = 0x07;
    PORTB = size;
    PORTA = 0x01;
    PORTA = 0x00;
}

void load_alphaOp( bool alphaOp)
{
    PORTC = 0x08;
    if (alphaOp)
        PORTB = 0x01;
    else
        PORTB = 0x00;
    PORTA = 0x01;
    PORTA = 0x00;
}

void setDoubleBuffer(bool doubleBuffer)
{
    PORTC = 0x09;
    if (doubleBuffer)
        PORTB = 0x01;
    else
        PORTB = 0x00;

    PORTA = 0x01;
    PORTA = 0x00;
}

void setFieldColor(uint8 color)
{
    PORTC = 0x0B;
    PORTB = color;
    PORTA = 0x01;
    PORTA = 0x00;
}


void flipBuffer(uint8* buf)
{
    PORTC = 0x0A;
    if (*buf)
    {
        *buf = 0;
        PORTB = 0x01;
    }
    else
    {
        *buf = 1;
        PORTB = 0x00;
    }
    PORTA = 0x01;
    PORTA = 0x00;
}

void drawData(void)
{
    PORTA = 0b00000010;                    //initialize draw
    PORTA = 0b00000000;                    //draw command recieved, so turn off draw bit
    while (!(PORTD & 0b10000000))        //hang here till we recieve idle from GPU
    {
    }
}

int32 max(int32 p, int32 q)
{
    if (p > q)
        return p;
    else
        return q;
}

void drawFullscreen(uint32 address)
{
    load_t_addr(0x00000000);
    load_s_addr(address);
    load_s_lines(SCREEN_HEIGHT);
    load_l_size(SCREEN_WIDTH);
    load_alphaOp(false);
    drawData();
}

void draw(uint32 address, uint8 width, uint8 height, int16 x, int16 y, bool isTransparent)
{
    //
    // we are assuming a screen of 160 addresses x 240 lines
    //
    uint32 destAddress;
    uint32 readAddress = address;
    int16 drawWidth = (int16)width;
    int16 drawHeight = (int16)height;

    int16 extentX;
    int16 extentY;

    extentX = x + (int16)width;
    if (extentX > SCREEN_WIDTH)
    {
        //
        // right clipping
        //
        drawWidth -= (extentX - SCREEN_WIDTH);
        if (drawWidth <= 0)
            return;
    }
    else if (x < 0)
    {
        //
        // left clipping
        //
        readAddress += -x;

        drawWidth -= -x;
        if (drawWidth <= 0)
            return;
    }

    extentY = y + (int16)height;
    if (extentY > SCREEN_HEIGHT)
    {
        //
        // bottom clipping
        //
        drawHeight -= (extentY - SCREEN_HEIGHT);
        if (drawHeight <= 0)
            return;
    }
    else if (y < 0)
    {
        //
        // top clipping
        //
        readAddress += (-y * SCREEN_WIDTH);

        drawHeight -= -y;
        if (drawHeight <= 0)
            return;
    }

    destAddress = max(y, 0) * SCREEN_WIDTH + max(x, 0);

    load_t_addr(destAddress);
    load_s_addr(readAddress);
    load_s_lines(drawHeight);
    load_l_size(drawWidth);

    load_alphaOp(isTransparent);

    drawData();
}

void gpuInit()
{
    //Configure port A
    ADCON1 = 0x07;                                //disable analog inputs
    TRISA = 0x00;
    //Configure port B
    TRISB = 0x00;
    //Configure port C
    TRISC = 0x00;
    //Configure port D
    TRISD = 0b11111111;                            //port D is input from peripherals

    //Initialize port A
    PORTA = 0x00;
    //Initialize port B
    PORTB = 0x00;
    //Initialize port C
    PORTC = 0x00;
    //Initialize port D
    PORTD = 0x00;
}
