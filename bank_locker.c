#include<reg51.h> //Header file inclusion for 8051
#include<stdio.h>

sfr LCDdata = 0x80; //Connections of LCD data lines// port 0
sbit LCDrs = P1^0; //The Register select Pin
sbit LCDen = P1^1; //The Enable Pin
sbit r1=P2^0;
sbit r2=P2^1;
sbit r3=P2^2;
sbit r4=P2^3;
sbit c1=P2^4;
sbit c2=P2^5;
sbit c3=P2^6;
sbit c4=P2^7;
sbit led=P1^7;
sbit relay=P3^3;
sbit scan=P3^4;
sbit buzzer=P3^5;
sbit m1=P3^6;
sbit m2=P3^7;

unsigned char rec1[12],rec2[12],rec3[12],rec4[17];
unsigned char abc[16];
unsigned int i;
unsigned char Search[12] = {0XEF, 0X01, 0XFF, 0XFF, 0XFF, 0XFF, 0X01, 0X00, 0X03, 0X01, 0X00, 0X05 };
char p,q,r,s;
unsigned int j;
void delay(unsigned int rtime)
{
unsigned int r,s;
for(r=0;r<rtime;r++)
for(s=0;s<1275;s++);
}

//This function makes one column to GND one by one and check the rows for key press
char keypress()
{
while(1)
{
c1=0;c2=1;c3=1;c4=1;
if(r1==0){return '1';}
if(r2==0){return '4';}
if(r3==0){return '7';}
if(r4==0){return '*';}
c1=1;c2=0;c3=1;c4=1;
if(r1==0){return '2';}
if(r2==0){return '5';}
if(r3==0){return '8';}
if(r4==0){return '0';}
c1=1;c2=1;c3=0;c4=1;
if(r1==0){return '3';}
if(r2==0){return '6';}
if(r3==0){return '9';}
if(r4==0){return '#';}
c1=1;c2=1;c3=1;c4=0;
if(r1==0){return 'A';}
32
if(r2==0){return 'B';}
if(r3==0){return 'C';}
if(r4==0){return 'D';}
}
}

void lcdcmd (unsigned char DATA)
{
LCDrs=0;
LCDen=1;
LCDdata=DATA;
LCDrs=0;
LCDen=0;
}
void initialize (void)
{
lcdcmd (0x30);
delay(30);
lcdcmd (0x38);
delay(30);
lcdcmd (0x0C);
delay(30);
lcdcmd (0x01);
delay(30);
lcdcmd (0x06);
delay(30);
}
void lcddat (unsigned int DATA)
{
LCDrs=1;
LCDen=1;
LCDdata=DATA;
LCDrs=1;
LCDen=0;
}

void display_lcd (unsigned char location, unsigned char *d)
{
lcdcmd(0x80 | location);
delay(1);
while(*d)
    {
lcddat(*d++);
//delay(1);
}
}

void ser_send(char dat)
    {
SBUF=dat;
while(TI==0);
TI=0;
}

char ser_rec(void)
    {
//char val;
while(!RI);
RI=0;
//val=SBUF;
return SBUF;
}

void main(void)
    {
scan=1,led=1;
relay=0;
initialize();
SCON = 0x52; // 8-bit UART mode
TMOD = 0x20; // timer 1 mode 2 auto reload
TH1= 0xfd; // 9600 8-n-1
TR1 = 1; // run timer1
TI=1;
printf("AT\n");
delay(20);
printf("AT+CMGF=1\n");
delay(20);
printf("AT+CMGD=1\n");
delay(20);
printf("AT+CMGS=\"8319877823\"\n");
delay(10);
printf("two step security system \n");
delay(10);
putchar(26);
delay(1000);
//sprintf(abc,"%s","super");
//sprintf(abc,"%c",OpenDevice[i]);
//display_lcd(0xc0,abc);
while(1)
{
start2:
start4:
relay=1;// relay off postion
SCON = 0x52; // 8-bit UART mode
TMOD = 0x20; // timer 1 mode 2 auto reload
TH1= 0xfd; // 9600 8-n-1
TR1 = 1; // run timer1
TI=1;
led=1;
display_lcd(0xc0,"Start ");

if(scan==0 )
{
display_lcd(0xc0,"Scan ");
ser_send(0xFF);
ser_send(0x05);
ser_send(0xFD);
ser_send(0xFF);
ser_send(0x4B);
ser_send(0xFF);
ser_send(0xEE);
ser_send(0x82);
ser_send(0x25);
ser_send(0xF3);
ser_send(0xF6);
ser_send(0x04);
ser_send(0x88);
ser_send(0x08);
ser_send(0x08);
ser_send(0xFF);
ser_send(0xEF);
ser_send(0x01);
ser_send(0xFF);
ser_send(0xFF);
ser_send(0xFF);
ser_send(0xFF);
ser_send(0x01);
ser_send(0x00);
ser_send(0x07);
ser_send(0x13);
ser_send(0x00);
ser_send(0x00);
ser_send(0x00);
ser_send(0x00);
ser_send(0x00);
ser_send(0x1B);
for (i = 0; i < 12; i++)
{
rec1[i] = ser_rec();
}
for(i=0;i<12;i++) //Identify
{
ser_send(Search[i]); // scane need to send 0x05 last value
}
for (i = 0; i < 12; i++)
{
rec2[i] = ser_rec();
}
if (rec2[11] == 0X0A)
{
ser_send(0XEF);// 13 value sent
ser_send(0X01);
ser_send(0XFF);
ser_send(0XFF);
ser_send(0XFF);
ser_send(0XFE);
ser_send(0X01);
ser_send(0X02);
ser_send(0X04);
ser_send(0X02);
ser_send(0X01);
ser_send(0X09);
ser_send(0X08);
for (i = 0; i <12; i++)
{
rec3[i] = ser_rec();
}
}
if (rec3[11] == 0X0A)
{
ser_send(0XEF);// 17 value sent
ser_send(0X01);
ser_send(0XFF);
ser_send(0XFF);
ser_send(0XFF);
ser_send(0XFF);
ser_send(0X01);
ser_send(0X00);
ser_send(0X08);
ser_send(0X1b);
ser_send(0XC1);
ser_send(0X00);
ser_send(0XH0);
ser_send(0X03);
ser_send(0XE9);
ser_send(0X01);
ser_send(0X11);
rec4[0] = ser_rec();
rec4[1] = ser_rec();
rec4[2] = ser_rec();
rec4[3] = ser_rec();
rec4[4] = ser_rec();
rec4[5] = ser_rec();
rec4[6] = ser_rec();
rec4[7] = ser_rec();
rec4[8] = ser_rec();
rec4[9] = ser_rec();
rec4[10] = ser_rec();
rec4[11] = ser_rec();
}
if(rec2[11]==0x0A )
{
display_lcd(0xc0," print found ");
delay(300);
}
if(rec4[11]==0x01)
{
led=0;
display_lcd(0xc0,"ID: 1 ");
delay(200);
start1:
display_lcd(0x40,"Password: ");
p = keypress();
sprintf(abc,"%c",p);
display_lcd(0x49,abc);
delay(50);
q = keypress();
sprintf(abc,"%c",q);
display_lcd(0x4A,abc);
delay(50);
r = keypress();
sprintf(abc,"%c",r);
display_lcd(0x4B,abc);
delay(50);
s = keypress();
sprintf(abc,"%c",s);
display_lcd(0x4C,abc);
delay(50);
if(p=='2'&& q=='5'&& r=='6'&& s=='3')
{
display_lcd(0x40,"Password valid");
delay(200);
m1=1;
m2=0;
delay(725);
m1=0;
m2=0;
delay(800);
m1=0;
m2=1;
delay(725);
m1=0;
m2=0;
delay(100);
}
else
{
buzzer=0;
display_lcd(0x40,"Password invalid");
delay(50);
j++;
delay(50);
buzzer=1;
if(j<2)
{
display_lcd(0x40,"Try Again ");
delay(100);
}
if(j==2)
{
j=0;
display_lcd(0x40,"Max attemp finsh");
delay(500);
SCON = 0x52; // 8-bit UART mode
TMOD = 0x20; // timer 1 mode 2 auto reload
TH1= 0xfd; // 9600 8-n-1
TR1 = 1; // run timer1
TI=1;
relay=0;
delay(500);
printf("AT\n");
delay(20);
printf("AT+CMGF=1\n");
delay(20);
printf("AT+CMGD=1\n");
delay(20);
printf("AT+CMGS=\"8319877823\"\n");
delay(10);
printf(" wrong access \n");
delay(10);
putchar(26);
delay(1000);
goto start2;
}
goto start1;
}//password else
}// FP if close
else if(rec4[11]==0x02)
{
led=0;
display_lcd(0xc0,"ID: 2 ");
delay(200);
start3:
display_lcd(0x40,"Password: ");
p = keypress();
sprintf(abc,"%c",p);
display_lcd(0x49,abc);
delay(50);
q = keypress();
sprintf(abc,"%c",q);
display_lcd(0x4A,abc);
delay(50);
r = keypress();
sprintf(abc,"%c",r);
display_lcd(0x4B,abc);
delay(50);
s = keypress();
sprintf(abc,"%c",s);
display_lcd(0x4C,abc);
delay(50);
if(p=='2'&& q=='5'&& r=='6'&& s=='3')
{
display_lcd(0x40,"Password valid");
delay(200);
m1=1;
m2=0;
delay(725);
m1=0;
m2=0;
delay(800);
m1=0;
m2=1;
delay(725);
m1=0;
m2=0;
delay(100);
}
else
{
buzzer=0;
display_lcd(0x40,"Password invalid");
delay(50);
j++;
delay(10);
buzzer=1;
delay(50);
if(j<2)
{
display_lcd(0x40,"Try Again ");
delay(100);
}
if(j==2)
{
j=0;
display_lcd(0x40,"Max attemp finsh");
SCON = 0x52; // 8-bit UART mode
TMOD = 0x20; // timer 1 mode 2 auto reload
TH1= 0xfd; // 9600 8-n-1
TR1 = 1; // run timer1
TI=1;
relay=0;
delay(500);
printf("AT\n");
delay(20);
printf("AT+CMGF=1\n");
delay(20);
printf("AT+CMGD=1\n");
delay(20);
printf("AT+CMGS=\"8319877823\"\n");
delay(10);
printf(" wrong access \n");
delay(10);
putchar(26);
delay(1000);
goto start4;
}
goto start3;
}
}
else
{
display_lcd(0xc0,"Access Denied! ");
delay(200);
}
rec1[12]=0;rec2[12]=0;rec3[12]=0;rec4[17]=0;
////////////////////////
}//if scane
}// while
}//void