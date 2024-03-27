#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#define FILEPATH "/dev/fb0"
#define NUM_WORDS 64
#define FILESIZE (NUM_WORDS * sizeof(uint16_t))

#define RGB565_RED 0xF800

#define RED 0xF900
#define GREEN 0x00FF00
#define BLUE 0x0000FF

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 8

#include <fcntl.h>
#include <linux/fb.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

extern int i;
extern int fbfd;
extern uint16_t *map;
extern uint16_t *p;
extern struct fb_fix_screeninfo fix_info;

void delay(int);

int initLED();
void unMapLED();

void displayLetter(uint8_t (*text)[CHAR_WIDTH], u_int16_t color);
void blinkLetter(uint8_t (*text)[CHAR_WIDTH], u_int16_t color);

#endif
