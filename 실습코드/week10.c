#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

  //int main() {
  //	unsigned char digit[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x27, 0x7f, 0x6f};
  //	unsigned char fndSel[4] = {0x01, 0x02, 0x04, 0x08};
  //	unsigned char ledSel[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
  //	
  //	int randomNum = 0;
  //	DDRC = 0xff;
  //	DDRG = 0x0f;
  //	DDRA = 0xff;
  //	int i = 0;
  //	volatile int j = 0;
  //	
  //	while(1) {
  //		randomNum = rand() % 8;
  //		
  //		PORTA = ledSel[randomNum];
  //		
  //		for (j = 0; j < 32000; j++) {
  //			for (i = 0; i < 4; i++) {
  //				PORTC = digit[randomNum];
  //				PORTG = fndSel[i];
  //			}	
  //		}
  //	}
  //}

void printNum(unsigned char num[]) {
	unsigned char fndSel[4] = { 0x08, 0x04, 0x02, 0x01 };
	int i = 0;

	for (i = 0; i < 4; i++) {
		PORTC = num[i];
		PORTG = fndSel[i];
		_delay_ms(2);
	}
}

int main() {
	unsigned char digit[10] = { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x27, 0x7f, 0x6f };
	unsigned char dot = 0x80;

	DDRA = 0xff;
	DDRE = 0x00;
	DDRC = 0xff;
	DDRG = 0x0f;
	int i = 0;
	int j = 0;
	int k = 0;
	int l = 0;
	unsigned char num[4] = {};

	for (i = 0;; i++) {
		if (i == 10) {
			i = 0;
		}

		num[0] = digit[i];

		for (j = 0; j < 10; j++) {
			num[1] = digit[j] + dot;

			for (k = 0; k < 10; k++) {
				num[2] = digit[k];

				for (l = 0; l < 10; l++) {
					num[3] = digit[l];
					printNum(num);

					if ((PINE & 0x10) == 0x00) {
						PORTA = 0xff;
					}
					else {
						PORTA = 0x00;
					}
				}
			}
		}
	}
}