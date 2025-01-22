#include <stdbool.h>
#include <stm8s.h>
#include <stdio.h>
#include "main.h"
#include "milis.h"
//#include "delay.h"
#include "uart1.h"

#define PWM_PORT GPIOD
#define PWM_PIN GPIO_PIN_4

#define MAX_CCR 2500
#define MIN_CCR 500


void init(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);      // taktovani MCU na 16MHz

    GPIO_Init(PWM_PORT,PWM_PIN,GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(BTN_PORT, BTN_PIN, GPIO_MODE_IN_FL_NO_IT);

    init_milis();
    init_uart1();
}

void pwm_init(void){
    TIM2_DeInit();
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2,ENABLE);

    TIM2_TimeBaseInit(TIM2_PRESCALER_16,19999);
    TIM2_OC1Init(TIM2_OCMODE_PWM1,
     TIM2_OUTPUTSTATE_ENABLE,
      1500,
      TIM2_OCPOLARITY_HIGH);
      TIM2_OC1PreloadConfig(ENABLE);

      //TIM2_ITConfig(TIM2_IT_CC1,ENABLE);

      TIM2_Cmd(ENABLE);
      //enableInterrupts();
}
   
void nastaveni_uhlu(uint16_t uhel){ 
    uint16_t pulz = MIN_CCR +(uhel*((MAX_CCR-MIN_CCR)/180));
    TIM2_SetCompare1(pulz);
    //printf("Šířka pulzu: %u\n", pulz);
    //printf("úhel: %u\n", uhel);
}

int main(void)
{   
    uint8_t BTNstate = 1;
    uint8_t lastBTNstate = 1;
    uint8_t pozice = 1;

    init();
    pwm_init();

    while (1) {
        BTNstate = GPIO_ReadInputPin(BTN_PORT, BTN_PIN);
        
        if (lastBTNstate == 0 && BTNstate == 1) {
            pozice = pozice + 1;
        }
        if (pozice == 1){
            nastaveni_uhlu(0);
        }
        else if (pozice == 2){
            nastaveni_uhlu(45);
        }
        else if(pozice == 3){
            nastaveni_uhlu(90);
        }
        else if(pozice == 4){
            nastaveni_uhlu(135);
        }
        else if(pozice == 5){
            nastaveni_uhlu(180);
        }

        if(pozice >5){
            pozice = 1;
        }
    printf("Pozice: %u\n", pozice);
    
  lastBTNstate = BTNstate;
 }
}

/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"
