/*
 * VS1003B.c
 *
 *  Created on: Nov 13, 2024
 *      Author: andrey
 */

#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "VS1003B.h"
#include "fatfs.h"
#include <stdio.h> // Include for printf
#include <string.h>
#include "MP3_FileHandaling.h"

#define READ 0x3
#define WRITE 0x2


const uint8_t SCI_MODE = 0x0;
const uint8_t SCI_STATUS = 0x1;
const uint8_t SCI_BASS = 0x2;
const uint8_t SCI_CLOCKF = 0x3;
const uint8_t SCI_DECODE_TIME = 0x4;
const uint8_t SCI_AUDATA = 0x5;
const uint8_t SCI_WRAM = 0x6;
const uint8_t SCI_WRAMADDR = 0x7;
const uint8_t SCI_HDAT0 = 0x8;
const uint8_t SCI_HDAT1 = 0x9;
const uint8_t SCI_AIADDR = 0xa;
const uint8_t SCI_VOL = 0xb;
const uint8_t SCI_AICTRL0 = 0xc;
const uint8_t SCI_AICTRL1 = 0xd;
const uint8_t SCI_AICTRL2 = 0xe;
const uint8_t SCI_AICTRL3 = 0xf;
const uint8_t SCI_num_registers = 0xf;


void VS1003B_Reset(void)
{
    HAL_GPIO_WritePin(XRST_GPIO_Port, XRST_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(XCS_GPIO_Port, XCS_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(XDCS_GPIO_Port, XDCS_Pin, GPIO_PIN_SET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(XRST_GPIO_Port, XRST_Pin, GPIO_PIN_SET);
}

void VS1003B_Test(void)
{
	VS1003B_Write(SCI_VOL, 0x0001);
	HAL_Delay(100);
    printf("status register value is: %u\n\r", VS1003B_Read(SCI_VOL));

    VS1003B_Write(SCI_VOL, 0xFFFF);
    HAL_Delay(100);
    printf("status register value is: %u\n\r", VS1003B_Read(SCI_VOL));
}

void VS1003B_Init(void)
{
    VS1003B_Reset();
    VS1003B_Write(SCI_VOL, 0xFFFF); // turn the volume off to avoid noise
    VS1003B_Write(SCI_AUDATA,10); // slow the sample rate for slow startup to avoid noise
    HAL_Delay(100);
    VS1003B_Write(SCI_VOL,0xfefe); // gradual volume increase for smooth transition
    VS1003B_Write(SCI_AUDATA,44101); // set to sample rate to 44.1khz
    VS1003B_Write(SCI_VOL,0x2020); // final volume level
    //maybe add software reset
    VS1003B_Write(SCI_CLOCKF,0xB800); //CLKI = 12.288 MHz * 3.0 * 1.5 = 55.296 MHz.
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16; //increase the spi clock to 3MHZ
    printf("Init Completed\n\r");
}

uint16_t VS1003B_Read(uint8_t address)
{
    uint8_t data[2] = {READ, address};
    uint8_t RxData[2];

    HAL_GPIO_WritePin(XCS_GPIO_Port, XCS_Pin, GPIO_PIN_RESET);

    if (HAL_SPI_Transmit(&hspi1, data, 2, 200) != HAL_OK)
    {
        HAL_GPIO_WritePin(XCS_GPIO_Port, XCS_Pin, GPIO_PIN_SET);
        return 0;
    }

    if (HAL_SPI_Receive(&hspi1, RxData, 2, 200) != HAL_OK)
    {
        HAL_GPIO_WritePin(XCS_GPIO_Port, XCS_Pin, GPIO_PIN_SET);
        return 0;
    }

    HAL_GPIO_WritePin(XCS_GPIO_Port, XCS_Pin, GPIO_PIN_SET);

    return ((uint16_t)RxData[0] << 8) | RxData[1];
}


void VS1003B_SetVolume(uint8_t vol)
{
	  uint16_t value = vol;
	  value <<= 8;
	  value |= vol;
	  VS1003B_Write(SCI_VOL, value);
}


void VS1003B_SendData(const uint8_t *data, size_t length)
{
    const size_t vs1003_chunk_size = 32; // Adjust this based on the VS1003's requirements

    DCS_Enable(); // Start data mode
    while (length)
    {
        DREQ_Wait(); // Wait until the VS1003 is ready to receive data
        delay_us(3); // Small delay to ensure stability

        // Calculate chunk length (min of remaining length and max chunk size)
        size_t chunk_length = (length < vs1003_chunk_size) ? length : vs1003_chunk_size;
        length -= chunk_length;

        // Send the chunk
        if (HAL_SPI_Transmit(&hspi1, (uint8_t *)data, chunk_length, 200) != HAL_OK)
        {
            // Handle SPI transmission error (optional)
            break;
        }

        // Move data pointer forward
        data += chunk_length;
    }
    DCS_Disable(); // End data mode
}


void VS1003B_Write(uint8_t address, uint16_t value)
{
    uint8_t data[4] = {WRITE, address, (uint8_t)(value >> 8), (uint8_t)value};

    CS_Enable();
    HAL_SPI_Transmit(&hspi1, data, 4, 200);
    DREQ_Wait();
    CS_Disable();
}

void CS_Enable(void)
{
    HAL_GPIO_WritePin(XCS_GPIO_Port, XCS_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(XDCS_GPIO_Port, XDCS_Pin, GPIO_PIN_SET);
}

void CS_Disable(void)
{
    HAL_GPIO_WritePin(XCS_GPIO_Port, XCS_Pin, GPIO_PIN_SET);
}

void DCS_Enable(void)
{
    HAL_GPIO_WritePin(XCS_GPIO_Port, XCS_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(XDCS_GPIO_Port, XDCS_Pin, GPIO_PIN_RESET);
}

void DCS_Disable(void)
{
    HAL_GPIO_WritePin(XDCS_GPIO_Port, XDCS_Pin, GPIO_PIN_SET);
}

void DREQ_Wait(void)
{
    while (!HAL_GPIO_ReadPin(DREQ_GPIO_Port, DREQ_Pin)) {}
}


void delay_us(uint32_t us) {
    uint32_t start = SysTick->VAL;
    uint32_t ticks = us * (SystemCoreClock / 1000000);

    while ((SysTick->VAL - start) < ticks);
}



void PlayMP3File(const char *filename, uint8_t *PlayState)
{
    FIL file;            // File object
    UINT bytesRead;      // Number of bytes read
    uint8_t buffer[512]; // Buffer to hold file data
    FRESULT res;

    // Open the MP3 file
    res = f_open(&file, filename, FA_READ);
    if (res != FR_OK)
    {
        printf("Error opening file: %s\n", filename);
        return;
    }
    printf("Playing file: %s\n", filename);
    VS1003B_SetVolume(0x00);
    // Read and send data in chunks
    do
    {
        if (*PlayState)
        {
            // Read a chunk from the file
            res = f_read(&file, buffer, sizeof(buffer), &bytesRead);
            if (res != FR_OK)
            {
                printf("Error reading file: %d.\n", res);
                break;
            }

            // Send the chunk to the VS1003 for playback
            VS1003B_SendData(buffer, bytesRead);
        }
        else
        {
            // Playback is paused; wait until PlayState becomes true
            osDelay(10); // Avoid busy-waiting; delay for 10 ms
            continue;
        }

        // Check if the song has changed
        if (strcmp(mp3FileListShufled[SongIndex], filename) != 0)
        {
            printf("Song changed to: %s\n", mp3FileListShufled[SongIndex]);
            break;
        }

    } while (bytesRead > 0); // Continue until the end of the file

    // Close the file
    f_close(&file);
    printf("Playback finished.\n");
}

