#ifndef INC_VS1003B_H_
#define INC_VS1003B_H_

#include <stdint.h> // Include for standard integer types

// Function prototypes
void VS1003B_Reset(void);                // Reset the VS1003B chip
void VS1003B_Init(void);                 // Initialize the VS1003B chip
void VS1003B_Test(void);                 // Test functionality
uint16_t VS1003B_Read(uint8_t address);  // Read from a VS1003B register
void VS1003B_Write(uint8_t address, uint16_t value); // Write to a VS1003B register
void CS_Enable(void);                    // Enable chip select for control
void CS_Disable(void);                   // Disable chip select for control
void DCS_Enable(void);                   // Enable chip select for data
void DCS_Disable(void);                  // Disable chip select for data
void DREQ_Wait(void);                    // Wait for DREQ pin to signal ready
void VS1003B_SetVolume(uint8_t vol); 			//set volume level
void delay_us(uint32_t us);
void PlayMP3File(const char *filename, uint8_t *PlayState);//basic microsecond delay function

#endif /* INC_VS1003B_H_ */
