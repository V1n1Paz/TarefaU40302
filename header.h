#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"
#include "ssd1306.h"
//arquivo .pio
#include "matriz.pio.h"

//número de LEDs
#define NUM_PIXELS 25

//pino de saída
#define OUT_PIN 7

#define LED_PIN_RED 13
#define LED_PIN_GREEN 11
#define LED_PIN_BLUE 12

#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1

//botoes de interupção
#define BUT_PIN_A 5
#define BUT_PIN_B 6

//i2c
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

//contador dos botoes
static volatile uint matrix_number;

//variavel para controle do tempo
static volatile uint32_t last_time = 0;

static char message[15];

// Vetores para criar imagens na matriz de LED 
double number0[25] = {0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0, 
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0};

double number1[25] = {0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0, 
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0};

double number2[25] = {0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 0.0, 0.0, 
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0};

double number3[25] = {0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 0.0, 0.0, 
    0.0, 0.1, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0};

double number4[25] = {0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0, 
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 1.0, 0.0};

double number5[25] = {0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0, 0.0, 
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0};

double number6[25] = {0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0, 0.0, 
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0};

double number7[25] = {0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 0.0, 0.0, 
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0};

double number8[25] = {0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0, 
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0};

double number9[25] = {0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0, 
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0};

double off[25] = {0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0};

//matriz com os vetores definidos
double* numbers[10] = {number0, number1, number2, number3, number4, number5, number6, number7, number8, number9};


//
uint32_t matrix_rgb(double b, double r, double g);
void numero_pio(double *numero, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b);