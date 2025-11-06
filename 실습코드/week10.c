#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h> // Standard library (for rand() function)

//int main() {
//	// Array for 7-segment patterns (0-9)
//	unsigned char digit[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x27, 0x7f, 0x6f};
//	
//	// Array for FND digit selection (PG3 to PG0)
//	// Order: 4th, 3rd, 2nd, 1st digit
//	unsigned char fndSel[4] = {0x08, 0x04, 0x02, 0x01};
//	
//	// Array for LED selection (PA7 to PA0)
//	unsigned char ledSel[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
//
//	int randomNum = 0; // Stores the random number (0-7)
//	
//	// Set Port C, G (lower 4 bits), and A as outputs
//	DDRC = 0xff; // PORTC for FND data segments
//	DDRG = 0x0f; // PORTG for FND digit selection
//	DDRA = 0xff; // PORTA for LEDs
//
//	int i = 0; // Loop counter for FND digits
//	int j = 0; // Loop counter for overall display time
//	
//	while(1) {
//		// Generate a random number between 0 and 7
//		randomNum = rand() % 8;
//		
//		// Light up the corresponding LED
//		PORTA = ledSel[randomNum];
//
//		// Outer loop: Repeats the full FND scan to display the number
//		// for a noticeable duration. (Total time = 100 * (4 * 5ms) = 2 sec)
//		for (j = 0; j < 100; j++) {
//			// Inner loop: Scan through the 4 FND digits (dynamic scanning)
//			for (i = 0; i < 4; i++) {
//				// Send the number pattern to the FND segments
//				PORTC = digit[randomNum];
//				
//				// Select one of the four digits to light up
//				PORTG = fndSel[i];
//				
//				// Brief delay to keep the digit visible
//				_delay_ms(5);
//			}
//		}
//	}
//}

void printNum(unsigned char num[]) {
	// Array for FND digit selection (PG3 to PG0)
	// Order: 4th(Left), 3rd, 2nd, 1st(Right)
	unsigned char fndSel[4] = { 0x08, 0x04, 0x02, 0x01 };
	int i = 0;

	// Iterate through all 4 digits
	for (i = 0; i < 4; i++) {
		PORTC = num[i];     // Send segment data (the number's shape)
		PORTG = fndSel[i];  // Select the specific digit to light up
		_delay_ms(2);
	}
}

int main() {
	// Array for 7-segment patterns (0-9)
	unsigned char digit[10] = { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x27, 0x7f, 0x6f };
	// Bit pattern for the decimal point (DP)
	unsigned char dot = 0x80;

	// Set Port A (LEDs) as output
	DDRA = 0xff;
	// Set Port E (Switches) as input
	DDRE = 0x00;
	// Set Port C (FND data) as output
	DDRC = 0xff;
	// Set Port G (lower 4 bits, FND select) as output
	DDRG = 0x0f;

	int i = 0, j = 0, k = 0, l = 0; // Loop counters for digits
	unsigned char num[4] = {};     // Array to hold patterns for printNum

	// Infinite loop to run the counter
	for (i = 0; ; i++) {
		if (i == 10) { // Reset digit
			i = 0;
		}
		// Set pattern for the 4th digit
		num[0] = digit[i];

		// 'j' represents the value for the 3rd digit
		for (j = 0; j < 10; j++) {
			// Set pattern for the 3rd digit, adding the decimal point
			num[1] = digit[j] + dot;

			// 'k' represents the value for the 2nd digit
			for (k = 0; k < 10; k++) {
				// Set pattern for the 2nd digit
				num[2] = digit[k];

				// 'l' represents the value for the 1st digit
				for (l = 0; l < 10; l++) {
					// Set pattern for the 1st digit
					num[3] = digit[l];

					// Display the 4 digits (Order: [num[0], num[1], num[2], num[3]])
					printNum(num);

					// Polling: Check if SW1 (PE4) is pressed (returns 0 if pressed)
					if ((PINE & 0x10) == 0x00) {
						PORTA = 0xff; // Turn all LEDs ON
					}
					else {
						PORTA = 0x00; // Turn all LEDs OFF
					}
				}
			}
		}
	}
}