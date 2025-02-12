# STM32 Freertos MP3-player

do advanced my stm32 skills i wanted to implemnt a whole working embeded system from zero to full working prodcut that will involed a lot of diffrent parts workign together an mp3 player sound like a smothing i could do by my self the hardware the software the pcb design and custom 3d printed parts.
the project uses the following hardware
# System Overview  
 

## Current Operation  
The board plays audio from a microSD card and allows easy file transfers via USB. When connected to a PC, the microSD card appears as a mass storage device. Audio output is handled by an MP3 decoder module with a 3.5mm headphone jack. On power-up, the board starts playing a song, and pressing the blue user button skips to the next track.  


[![STM32 MP3 Player](https://github.com/AndreyPodlesni/STM32_MP3_PLAYER/blob/main/Thumbnail.jpg?raw=true)](https://www.youtube.com/watch?v=TIzKGpkJbzY)

## Hardware Specifications  
- **Microcontroller**: STM32F429 DISCO1 Evaluation Board  
- **Operating System**: FreeRTOS  
- **Storage & Connectivity**:  
  - **SD Card Interface**: STM32 SDIO  
  - **File System**: FATFS  
  - **USB Device Interface**: STM32 USB Device  
  - **USB Class**: Mass Storage  

## GUI and Display (In Development)  
- **Display Interface**: LTDC  
- **Display Driver**: ILI9341  
- **Hardware Acceleration**: ChromART (DMA2D)  
- **Frame Buffer**:  
  - **Storage**: Flash  
  - **Peripheral**: FMC (Flexible Memory Controller)  
- **Graphics Library**: LVGL  
- **UI Design Tool**: SquareLine Studio

- 
![setup](https://github.com/AndreyPodlesni/STM32_MP3_PLAYER/blob/main/board.jpg?raw=true)

## STM32F429 DISCO1 Evaluation Board
The DISCO1 was chosen because it includes all the necessary peripherals for prototyping the project. It features an SDIO interface for SD card connectivity, a USB peripheral for straightforward USB device development, an integrated TFT display based on the ILI9341, onboard flash memory connected via the FMC peripheral, and an LTDC peripheral capable of providing the required data lines for the display driver And  designed to manage display output efficiently.


## FreeRTOS: The Perfect Fit for Real-Time Multitasking

FreeRTOS is a powerful real-time operating system designed for running multiple tasks simultaneously with efficient context switching. In complex embedded systems—where tasks like streaming MP3 data to a decoder, transferring files between USB and an SD card, and managing a GUI must operate in parallel—FreeRTOS stands out as the ideal solution.

It offers essential features like task notifications for seamless inter-task communication, priority-based scheduling to ensure smooth operation without interrupting critical processes like MP3 playback and GUI updates, and mutexes for safe resource sharing without conflicts. With these capabilities and more, FreeRTOS provides the perfect foundation for building responsive and efficient real-time applications.

## SDIO
SDIO is a peripheral on the STM32 that communicates with SD cards using the SD card protocol. It moves data in blocks, making reading and writing to the SD card faster and more efficient than SPI.

In this project, SDIO is configured in **1-bit mode** due to issues encountered with 4-bit mode.
 It supports **DMA (Direct Memory Access)** to offload data transfers from the CPU This allows smooth MP3 playback while managing other tasks like USB file transfers and GUI rendering.

## FATFS
FATFS is a lightweight file system designed for embedded systems, providing a way to manage files and directories on storage devices like SD cards. It supports **FAT12, FAT16, and FAT32**, making it compatible with various storage formats commonly used in consumer electronics.
It allows the system to browse the SD card, open MP3 files, and stream audio data seamlessly. FATFS also enables **USB Mass Storage support**, allowing the SD card to appear as a removable drive when connected to a PC, making it easy to transfer music file


## LTDC (LCD-TFT Display Controller)

The  **LTDC**  is a STM32 peripheral for driving TFT displays. It’s used here to handle the GUI on the ILI9341 display. Key features:

1.  **Dual-Layer Support**:
    -   Two layers for blending (e.g., text over images).
    -   Each layer has its own frame buffer (SRAM, SDRAM, or flash).
2.  **Color Formats**:
    -   Supports  **RGB565**  (16-bit) for the ILI9341 display.
    -   Also supports  **ARGB8888**  (32-bit) for advanced effects.
3.  **DMA2D (ChromART)**:  
    -   Accelerates graphics (filling, copying, blending).
    -   Reduces CPU load for smooth GUI updates.
4.  **Timing Control**:
    -   Configurable sync signals (HSYNC, VSYNC).
    -   Adjustable display area and porch settings.
The LTDC, combined with DMA2D, ensures efficient and smooth GUI rendering while freeing up the CPU for other tasks like MP3 playback.

## Hardware Acceleration: ChromART (DMA2D)
The  **DMA2D (ChromART)**  accelerates graphics operations like filling, copying, and blending, offloading the CPU for smoother GUI performance.

## Frame Buffer
-   **Storage**: Flash stores the frame buffer for the display. 
-   **Peripheral**:  **FMC (Flexible Memory Controller)**  manages external memory access, ensuring fast data transfer to the display.
- 
## Graphics Library: LVGL
**LVGL**  is a lightweight, open-source graphics library for creating dynamic GUIs. It works seamlessly with the LTDC and DMA2D for efficient rendering.

## UI Design Tool: SquareLine Studio
**SquareLine Studio**  is a drag-and-drop tool for designing LVGL-based UIs, making it easy to create and customize the GUI without deep coding.

Together, these components enable a responsive and visually appealing user interface.






> Written with [StackEdit](https://stackedit.io/).
