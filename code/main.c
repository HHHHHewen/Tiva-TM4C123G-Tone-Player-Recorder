#define RCGCGPIO (*((volatile unsigned long *) 0x400FE608)) //A,B,C,D,F
#define RCGCPWM (*((volatile unsigned long *) 0x400FE640))
#define RCGCI2C (*((volatile unsigned long *) 0x400FE620))

//GPIODEN + GPIODIR
# define GPIODEN_A (*((volatile unsigned long *) 0x4000451C))
# define GPIODEN_B (*((volatile unsigned long *) 0x4000551C))
# define GPIODEN_C (*((volatile unsigned long *) 0x4000651C))
# define GPIODEN_D (*((volatile unsigned long *) 0x4000751C))
# define GPIODEN_F (*((volatile unsigned long *) 0x4002551C))
# define GPIODIR_A (*((volatile unsigned long *) 0x40004400))
# define GPIODIR_B (*((volatile unsigned long *) 0x40005400))
# define GPIODIR_C (*((volatile unsigned long *) 0x40006400))
# define GPIODIR_D (*((volatile unsigned long *) 0x40007400))
# define GPIODIR_F (*((volatile unsigned long *) 0x40025400))
//GPIODATA
//# define GPIODATA_A (*((volatile unsigned long *) 0x40004010)) //PA2 no(PA0 PA1)
# define GPIODATA_B (*((volatile unsigned long *) 0x400052E0)) //PB3,PB4,PB5,PB7
# define GPIODATA_C (*((volatile unsigned long *) 0x400063C0)) //PC4 PC5 PC6 PC7
# define GPIODATA_D (*((volatile unsigned long *) 0x40007100)) //PD6
# define GPIODATA_F (*((volatile unsigned long *) 0x40025030)) //PF2 PF3
//GPIO pull up/down
# define GPIOPUR_C (*((volatile unsigned long *) 0x40006510)) //PC5 PC6 PC7
# define GPIOPUR_D (*((volatile unsigned long *) 0x40007510)) //PD6
# define GPIOPDR_C (*((volatile unsigned long *) 0x40006514)) //PC5 PC6 PC7
# define GPIOPDR_D (*((volatile unsigned long *) 0x40007514)) //PD6
# define GPIOPUR_A (*((volatile unsigned long *) 0x40004510)) //PA7

#define RCGC_0 (*((volatile unsigned long *) 0x400FE100))
#define RCGC_2 (*((volatile unsigned long *) 0x400FE108))
#define GPIODEN_B (*((volatile unsigned long *) 0x4000551C))
#define GPIODIR_B (*((volatile unsigned long *) 0x40005400))
#define GPIOAFSEL_B (*((volatile unsigned long *) 0x40005420))
#define GPIOPCTL_B (*((volatile unsigned long *) 0x4000552C))
#define RCC (*((volatile unsigned long *) 0x400FE060))
#define PWM0CTL (*((volatile unsigned long *) 0x40028040))
#define PWM0GENA (*((volatile unsigned long *) 0x40028060))
#define PWM0LOAD (*((volatile unsigned long *) 0x40028050))
#define PWM0CMPA (*((volatile unsigned long *) 0x40028058))
#define PWM0CMPB (*((volatile unsigned long *) 0x4002805C))
#define PWMENABLE_0 (*((volatile unsigned long *) 0x40028008))

#define GPIOAFSEL_A (*((volatile unsigned long *) 0x40004420))
#define GPIOPCTL_A (*((volatile unsigned long *) 0x4000452C))
#define GPIOODR_A (*((volatile unsigned long *) 0x4000450C)) //PA7
#define I2CMCR1 (*((volatile unsigned long *) 0x40021020))
#define I2CMTPR1 (*((volatile unsigned long *) 0x4002100C))
#define I2CMSA1 (*((volatile unsigned long *) 0x40021000))
#define I2CMDR1 (*((volatile unsigned long *) 0x40021008))
#define I2CMCS1 (*((volatile unsigned long *) 0x40021004))

int loads[9]={0xFFFF,0xFF,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88};
char data_addr1=0;
char data_addr2=0;
//int loads[9]={0xFFFF,0x99,0xEE,0xDD,0xCC,0xBB,0xAA,0xFF,0x88};


void delay(){
    volatile unsigned long i;
    for(i=0;i<1000;i++);
}

void delay_i2c(){
    volatile unsigned long i;
    for(i=0;i<100000;i++);
}

void delay_note(){
    volatile unsigned long i;
    for(i=0;i<400000;i++);
}

char detect_data(void){
    while(1){
        GPIODATA_F=0xC;
        GPIODATA_B&=~(1<<3);
        GPIODATA_C=0x10;
        delay();
        if(((GPIODATA_C&(1<<5))>>5)==0){
            return '7';
        }
        else if(((GPIODATA_C&(1<<6))>>6)==0){
            return '8';
        }
        else if(((GPIODATA_C&(1<<7))>>7)==0){//9
            continue;
        }
        else if(((GPIODATA_D&(1<<6))>>6)==0){
            return 'C';
        }
        else{
            GPIODATA_F=0x4;
            GPIODATA_B|=0x8;
            GPIODATA_C=0x10;
            delay();
            if(((GPIODATA_C&(1<<5))>>5)==0){
                return '4';
            }
            else if(((GPIODATA_C&(1<<6))>>6)==0){
                return '5';
            }
            else if(((GPIODATA_C&(1<<7))>>7)==0){
                return '6';
            }
            else if(((GPIODATA_D&(1<<6))>>6)==0){
                return 'B';
            }
            else{
                GPIODATA_F=0x8;
                GPIODATA_B|=0x8;
                GPIODATA_C=0x10;
                delay();
                if(((GPIODATA_C&(1<<5))>>5)==0){
                    return '1';
                }
                else if(((GPIODATA_C&(1<<6))>>6)==0){
                    return '2';
                }
                else if(((GPIODATA_C&(1<<7))>>7)==0){
                    return '3';
                }
                else if(((GPIODATA_D&(1<<6))>>6)==0){
                    return 'A';
                }
                else{
                    GPIODATA_F=0xC;
                    GPIODATA_B|=0x8;
                    GPIODATA_C=0x0;
                    delay();
                    if(((GPIODATA_C&(1<<5))>>5)==0){//*
                        continue;
                    }
                    else if(((GPIODATA_C&(1<<6))>>6)==0){//0
                        continue;
                    }
                    else if(((GPIODATA_C&(1<<7))>>7)==0){//#
                        continue;
                    }
                    else if(((GPIODATA_D&(1<<6))>>6)==0){
                        return 'D';
                    }
                }
            }
        }
    }
}

int button_press(){
    int i=0;
    int res;
    for(;i<1000;i++){
        res = detect_data();
    }
    return res;
}

void i2cTx(char data){
//    I2CMCS1=0x0B;
    I2CMSA1=0xA0;
    I2CMDR1=0x0;
//    while(((I2CMCS1&(1<<6))>>6)!=0){;} //bus busy=0
    I2CMCS1=0x03;//3,B
    while(((I2CMCS1&(1<<0))>>0)!=0){;} //i2c busy=0
    while(((I2CMCS1&(1<<1))>>1)!=0){;} //error bit=0
    delay_i2c();
    I2CMDR1=data_addr1;
    data_addr1++;
    I2CMCS1=0x01;//1,B
    while(((I2CMCS1&(1<<1))>>1)!=0){;} //error bit=0
    delay_i2c();
    while(((I2CMCS1&(1<<0))>>0)!=0){;} //i2c busy=0
    I2CMDR1=data;
    I2CMCS1=0x05;//5,7
    while(((I2CMCS1&(1<<1))>>1)!=0){;} //error bit=0
    delay_i2c();
    while(((I2CMCS1&(1<<0))>>0)!=0){;} //i2c busy=0
//    check error bit?
}

char i2cRx(void){
//    I2CMCS1=0x0B;
    I2CMSA1=0xA0;
    I2CMDR1=0x0;
    I2CMCS1=0x03;
    while(((I2CMCS1&(1<<0))>>0)!=0){;} //i2c busy=0
    delay_i2c();
    I2CMDR1=data_addr2;
    data_addr2++;
    I2CMCS1=0x01;
    while(((I2CMCS1&(1<<0))>>0)!=0){;} //i2c busy=0
    delay_i2c();
    I2CMSA1=0xA1;
    I2CMCS1=0x07;
    while(((I2CMCS1&(1<<0))>>0)!=0){;} //i2c busy=0
    delay_i2c();
    return I2CMDR1;
}

int main(void)
{
    int data_int=0;
    RCGCGPIO=0x2F; //A,B,C,D,F
    GPIODEN_A=0xC0; //PA6,PA7 1100 0000
    GPIODEN_B=0xF8; //PB3,PB4,PB5,PB6,PB7 1111 1000
    GPIODEN_C=0xF0; //PC4,PC5,PC6,PC7 1111 0000
    GPIODEN_D=0x40; //PD6 0100 0000
    GPIODEN_F=0xC; //PF2,PF3 1100
//    GPIODIR_A=0xC0;
    GPIODIR_B=0xF8; //PB3,PB4,PB5,PB6,PB7
    GPIODIR_C=0x10; //PC4
    GPIODIR_F=0xC; //PF2,PF3
    GPIOPUR_C=0xE0;
    GPIOPUR_D=0x40;

//    GPIODATA_B=0x80;

    //PWM initialization
    RCGCPWM=0x1; //module 0
    //RCGC_0=0x00100000;
    //RCGC_2|=0x2;.

//    GPIODIR_B=0x40;
    GPIOAFSEL_B=0x40;
    GPIOPCTL_B=0x4000000; //0000 1000 0000 0000 0000 0000 0000 0000 PB6 function4
    RCC|=(1<<20); //bit20:USESYSDIV; bits19:17:PWMDIV
//    RCC|=0xE0000;
    PWM0CTL=0x0; //count down mode
    PWM0GENA=0x4; //load: toggle
    PWM0LOAD=0xFFFF;
    PWM0CMPA=0x0;
    PWM0CMPB=0x0;
//    PWM0CTL=0x1; //start the timer
//    PWMENABLE_0=0x1; //enable output

    RCGCI2C=0x2;
    GPIOAFSEL_A=0xC0;
    GPIOODR_A=0x80;
    GPIOPUR_A=0xC0;
    GPIOPCTL_A=0x33000000; //PA6,PA7,I2C1
    I2CMCR1=0x10;
    I2CMTPR1=0x7;

    unsigned char mode='A';
    unsigned char data=0;
    while(1){
        if(mode=='A'){
            GPIODATA_B&=~(1<<5);
            GPIODATA_B&=~(1<<7);
            GPIODATA_B|=0x10;
            data=button_press();
            if(data=='A'||data=='B'||data=='C'||data=='D'){
                mode=data;
                if(mode=='C'||mode=='D'){
                    i2cTx('9');
                }
            }
            else{
                data_int=data-48;
                PWM0LOAD=loads[data_int];
                PWM0CTL=0x1; //start the timer
                PWMENABLE_0=0x1; //enable output
                delay_note();
                PWM0CTL=0x0;
                PWMENABLE_0=0x0;
            }
            continue;
        }
        else if(mode=='B'){
            GPIODATA_B&=~(1<<4);
            GPIODATA_B&=~(1<<7);
            GPIODATA_B|=0x20;
            data=button_press();
            if(data=='A'||data=='B'||data=='C'||data=='D'){
                mode=data;
                if(mode=='C'||mode=='D'){
                    i2cTx('9');
                }
                if(mode!='A'){data_addr1=0;data_addr2=0;}
            }
            else{
                data_int=data-48;
                PWM0LOAD=loads[data_int];
//                char data_char=data;
                i2cTx(data);
//                record[idx]=data;
//                idx++;
                PWM0CTL=0x1; //start the timer
                PWMENABLE_0=0x1; //enable output
                delay_note();
                PWM0CTL=0x0;
                PWMENABLE_0=0x0;
            }
            continue;
        }
        else if(mode=='C'||mode=='D'){
            GPIODATA_B&=~(1<<4);
            GPIODATA_B&=~(1<<5);
            GPIODATA_B|=0x80;
            while(1){
//                int a = data_addr2;
                data=i2cRx();
                data_int=data-48;
//                a = data_add['''''''''''r2;
                if(data=='9'){break;}
                PWM0LOAD=loads[data_int];
                PWM0CTL=0x1; //start the timer
                PWMENABLE_0=0x1; //enable output
                delay_note();
                PWM0CTL=0x0;
                PWMENABLE_0=0x0;
            }
            data=button_press();
            if(data=='A'||data=='B'||data=='C'||data=='D'){
                mode=data;
                if(mode!='A'){data_addr1=0;data_addr2=0;}
            }
            continue;
        }
    }
}








