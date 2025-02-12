#ifndef MP3_FILE_HANDALING_H
#define MP3_FILE_HANDALING_H

#include "fatfs.h"
#include <stdint.h>

// Global variables
extern char **mp3FileList;          // Pointer to the original list (dynamically allocated)
extern char **mp3FileListShufled;   // Pointer to the shuffled list (dynamically allocated)
extern uint32_t fileCount;
extern int SongIndex;// Number of MP3 files found

// Function declarations
FRESULT Scan_SD_MP3(const char *path);
void FreeFileList();
void PrintMp3List();
void PrintShufledMp3List();
void ShuffleList();
void swap(char *str1, char *str2);
int RandWithLimit(int upper, int lower);

#endif // MP3_FILE_HANDALING_H
