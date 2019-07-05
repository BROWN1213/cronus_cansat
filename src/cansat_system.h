
/**
* @file 
* @author SeungMin Shin, Haneulbit Kim, Chan Lee
* @license This project is released under the MIT License (MIT)
* @copyright Copyright (c) 2019 Asgardia
* @date June 2019
* @brief ...
*/


void CansatSystemInit()
{
  // 초기화 부분
  MCUCR   = 0x00 ;

  DDRA  = 0xFF ; // for DTLM CMD : output
  PORTA = 0x00 ;

  DDRB  = 0xFF;  // for OUTPUT LED
  PORTB = 0xFF;

  DDRC  =0xFF; // for selection output
  DDRF  =0x00; // for adc input
  //PORTF =0x00;

  DDRE  =0xFF; // 1110 0010 : OUT for 1 Read for 0

  // Timer 설정 부분

  TIMSK = (1<<TOIE0) ;
  //TCNT0 = 255-108 ;
  TCNT0=0;
  TCCR0 = 4 ; // 1 , 2=/8, 3 =/64 , 4 =/256 5 =/1024
//  TCCR0 = 3 ; // 1 , 2=/8, 3 =/64 , 4 =/256 5 =/1024


  DDRD = 0xFF ; // 양방향

  sei();

}