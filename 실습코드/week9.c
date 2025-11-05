#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>


int main(void) {
  	unsigned char value = 128;
  	DDRA = 0xff;
  	bool isRight = true; 
    for (;;) {
  		PORTA = value;
  		_delay_ms(200);
  		
  		if (isRight) {
  			value >>= 1;
  		} else {
  			value <<= 1;
  		}
  		
  		if (value == 0 && isRight) {
  			value = 2;
  			isRight = false;
  		}
  		else if (value == 0 && !isRight) {
  			value = 64;
  			isRight = true;
  		}
  	}
}