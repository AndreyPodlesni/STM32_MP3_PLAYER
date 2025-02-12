#include "MP3_FileHandaling.h"
#include "string.h"
#include "stdio.h"
#include <stdlib.h>

// Global variables
extern char **mp3FileList = NULL;
 extern char **mp3FileListShufled = NULL;
uint32_t fileCount = 0;
int SongIndex =  0;

FRESULT Scan_SD_MP3(const char *path) {
    DIR dir;
    FILINFO fno;
    FRESULT res;

    printf("Creating MP3 list...\n");

    res = f_opendir(&dir, path);
    if (res != FR_OK) return res;

    while (1) {
        res = f_readdir(&dir, &fno);
        if (res != FR_OK || fno.fname[0] == 0) break;

        if (fno.fattrib & AM_DIR) {
            if (!(strcmp(fno.fname, ".") == 0 || strcmp(fno.fname, "..") == 0)) {
                char subDir[128];
                snprintf(subDir, sizeof(subDir), "%s/%s", path, fno.fname);
                Scan_SD_MP3(subDir); // Recursive call to scan subdirectory
            }
        } else {
            if (strstr(fno.fname, ".mp3") || strstr(fno.fname, ".MP3")) {
                // Allocate memory for the file list dynamically
                mp3FileList = realloc(mp3FileList, (fileCount + 1) * sizeof(char *));
                if (mp3FileList == NULL) {
                    perror("Memory allocation for mp3FileList failed");
                    return FR_INT_ERR;
                }
                mp3FileList[fileCount] = strdup(fno.fname);
                if (mp3FileList[fileCount] == NULL) {
                    perror("Memory allocation for file name failed");
                    return FR_INT_ERR;
                }
                fileCount++;
            }
        }
    }

    printf("File count: %d\n\r", fileCount);
    f_closedir(&dir);
    return FR_OK;
}

void FreeFileList() {
    for (int i = 0; i < fileCount; i++) {
        free(mp3FileList[i]);
    }
    free(mp3FileList);
    free(mp3FileListShufled);
}

void PrintMp3List() {
    printf("Printing list:\n");
    for (int i = 0; i < fileCount; i++) {
        if (mp3FileList[i] != NULL) {
            printf("File %d: %s\n", i + 1, mp3FileList[i]);
        }
    }
}

void PrintShufledMp3List() {
    printf("Printing shuffled list:\n");
    for (int i = 0; i < fileCount; i++) {
        if (mp3FileListShufled[i] != NULL) {
            printf("File %d: %s\n", i + 1, mp3FileListShufled[i]);
        }
    }
}

int* createUniqueRandomArray(int fileCount) {
    int* array = malloc(fileCount * sizeof(int));
    if (array == NULL) {
        perror("Failed to allocate memory for random array");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < fileCount; i++) {
        array[i] = i;
    }

    srand(HAL_GetTick());

    for (int i = fileCount - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }

    return array;
}

void ShuffleList() {
    int* uniqueRandomArray = createUniqueRandomArray(fileCount);

    // Allocate memory for the shuffled list
    mp3FileListShufled = malloc(fileCount * sizeof(char *));
    if (mp3FileListShufled == NULL) {
        perror("Memory allocation for mp3FileListShufled failed");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < fileCount; i++) {
        mp3FileListShufled[i] = mp3FileList[uniqueRandomArray[i]];
    }

    free(uniqueRandomArray);
}


