#include <stdio.h>


//#define NCHANNEL        0
#define  TI_RI_MASK        0x3    //Transmit/Receive interrupt enable
#define  SP_MODE_1         0x9    //Mode0

#define dXTAL 20000000UL //������� �������� ���������� ��
#define BitRate 9600UL // �������� ������/��������
#define BAUD_LO_VAL ((unsigned char) (dXTAL/(BitRate*2*8)-1))
#define BAUD_HI_VAL  0x80 //Clock UART from External clock of XTAL1

#pragma interrupt 0  timer_irq
#pragma interrupt 01 adc_irq
#pragma interrupt 8 IRQ_uart_rx_tx

/*Declaration variable*/
unsigned char ByteReceived = 0;
unsigned char status_uart = 0;
volatile unsigned char adc_mode;
volatile unsigned int adc_res;

//����������  ��� ������� �� ����
unsigned char rab=0;

// ���������� - ����
short unsigned  clon=0;
//���������� ��� ������ ������
short unsigned int SDV_Low =0xFF;

//������� �������
volatile unsigned int i;

//����������-������� �������
volatile unsigned char k;

//���������� - ������� ����������
volatile unsigned char adc_convertion;
volatile unsigned char timer_convertion;

// ����������, ����������� ��� ������
short signed int ADCRES=0;
 float alfa=15.0/16.0;
 float mas[3]={0};

void USART0_Init(unsigned char spcon);
void IRQ_uart_rx_tx();
/***************/


//��������� ���
void adc_init(unsigned char achannel)
{
  ADC_EN = 0x05;
  ADC_CON = 0x01 | adc_mode | (achannel << 3);
}

//���������� ���
void adc_off()
{
  ADC_EN = 0x00;
}


//��������� ����
void USART1_Init(unsigned char spcon)
{
  unsigned char reg;
  WSR=15;
  reg=IOC1;
  reg|=0x20;
  WSR=0;
  IOC1=reg;
  SP_CON1=0x09;
  BAUD_RATE1 = BAUD_LO_VAL;  //write Baud rate value
  BAUD_RATE1 = BAUD_HI_VAL;
  __NOP();
  __NOP();
}


//��������� �������
void init_Timer(void){

  unsigned char tmp, tmp1;
  tmp = WSR;
  WSR = (tmp & 0x80);
  IOC1 = 0x25;
  tmp1 = INT_MASK;
  tmp1 |= 0x01;
  INT_MASK = tmp1;
  WSR = (tmp & 0x80) | 0xF;
  TIMER1 = 0x0001;
  WSR = (tmp & 0x80);
                     }


//��������� �������� ��� �������
 void first_start ()
 {
        //adc_convertion = 0;
        for (k=0;k<3;k++)
                {
                adc_init(k);
                __NOP ();
                __NOP ();
                __NOP ();
                if ( ADC_RESULT&0x8000==0)
                        {
                        mas[k]=inverse(ADC_RESULT);
                        }
                }

 }

void main ()
{
 unsigned char tmp;
//INT_MASK |= 0x02;
__EI();

  //adc_convertion = 0;
  adc_mode=(0x00 << 1);
  i=3;
  first_start();
  k=0;

  USART1_Init(SP_MODE_1);
  timer_convertion=0;
  init_Timer();
  adc_init(k);
//IOC1 = 0x20;
  while (1)
{
    if (timer_convertion==1)
    {
                 timer_convertion=0;
                 status_uart = SP_STAT1;    //���������� ������� �������
                        if (status_uart&0x40)
                                {
                                ByteReceived = SBUF_RX1;
                                }
                //���� ������ ������� ������� �������� �� ����
                if  (ByteReceived==228)
                        {
                        __NOP ();
                        __NOP ();
                        }


                if (ByteReceived==13)    //���������� �������� ������ �� ����
                        {
                        if (ADC_RESULT&0x8000==0)
                                {
                                if (k<2)
                                        {
                                        // adc_convertion = 0;
                                        i++;
                                        ADCRES=inverse(ADC_RESULT);
                                        mas[k]=mas[k]+(1-alfa)*(ADCRES-mas[k]);
                                        clon=mas[k];
                                        //SP_CON0=0x0;
                                        SBUF_TX1 =k;
                                        __NOP ();
                                        __NOP ();

                                        while (clon!=0)
                                                {
                                                tmp = WSR;
                                                WSR=0;
                                                status_uart = SP_STAT1;
                                                WSR = tmp;
                                                if ((status_uart&0x8)!=0)
                                                        {
                                                        rab=clon&SDV_Low;
                                                        SBUF_TX1 = rab;
                                                        clon=clon>>8;
                                                        __NOP ();
                                                        }
                                                }   //while clon!=0
                                                        //inverse(ADCRES);

                                        __NOP ();
                                        __NOP ();
                                        //SP_CON0=0x8;
                                        k++;
                                        adc_init(k);
                                        } //if k<2
                                 else
                                        {
                                        ADCRES=inverse(ADC_RESULT);
                                        mas[k]=mas[k]+(1-alfa)*(ADCRES-mas[k]);
                                        clon=mas[k];
                                        // SP_CON0=0x0;

                                        SBUF_TX1 =k;
                                        __NOP ();
                                        __NOP ();
                                        while (clon!=0)
                                                {
                                                tmp = WSR;
                                                WSR=0;
                                                status_uart = SP_STAT1;
                                                WSR = tmp;
                                                if ((status_uart&0x8)!=0)
                                                        {
                                                        rab=clon&SDV_Low;
                                                        SBUF_TX1 = rab;
                                                        clon=clon>>8;
                                                        __NOP ();
                                                        }
                                                }  //while clon!=0
                                        __NOP ();
                                        __NOP ();
                                        //SP_CON0=0x8;

                                        k=0 ;
                                        i++;
                                        adc_convertion = 0;
                                        adc_init(k);
                                        }  //else (k>=2)
                                }//ADC_RESULT&0x8000==0

                        }//if (ByteReceived==13)
    } //if (timer_convertion==1)
}// while (1)
}//main


    void    timer_irq()
        {
        timer_convertion=1;
        TIMER1 = 0x0001;
        }


 //������ ����������   ADC
/*void adc_irq(){
  adc_convertion = 1;
} */

