#include "header.h"




//rotina da interrupção
static void gpio_irq_handler(uint gpio, uint32_t events){
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    if (current_time - last_time >= 300000){
        last_time = current_time;
        ssd1306_t ssd;
        ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
        ssd1306_config(&ssd); // Configura o display
        ssd1306_fill(&ssd, false);
        if (gpio == BUT_PIN_A){
            gpio_put(LED_PIN_GREEN, !gpio_get(LED_PIN_GREEN));
            printf("O estado do LED verde mudou\r\n");
            if (gpio_get(LED_PIN_GREEN))
                sprintf(message, "LED Verde Alto ");
            else
                sprintf(message, "LED Verde Baixo");
            
            ssd1306_draw_string(&ssd, message, 0, 30);
            ssd1306_send_data(&ssd);
        }
        if (gpio == BUT_PIN_B){
            gpio_put(LED_PIN_BLUE, !gpio_get(LED_PIN_BLUE));
            printf("O estado do LED azul mudou\r\n");
            if (gpio_get(LED_PIN_BLUE))
                sprintf(message, "LED Azul Alto ");
            else
                sprintf(message, "LED Azul Baixo");
            ssd1306_draw_string(&ssd, message, 0, 30);
            ssd1306_send_data(&ssd);
        }
            
    }
    
}


//função principal
int main()
{
    PIO pio = pio0; 
    bool ok;
    uint16_t i;
    uint32_t valor_led;
    double r = 0.0, b = 0.0 , g = 0.0;
    
    //coloca a frequência de clock para 128 MHz, facilitando a divisão pelo clock
    ok = set_sys_clock_khz(128000, false);

    // Inicializa todos os códigos stdio padrão que estão ligados ao binário.
    stdio_init_all();
    

    uart_init(UART_ID, BAUD_RATE);

    

    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);


    
    //configurações da PIO
    uint offset = pio_add_program(pio, &matriz_program);
    uint sm = pio_claim_unused_sm(pio, true);
    matriz_program_init(pio, sm, offset, OUT_PIN);

    gpio_init(LED_PIN_RED);
    gpio_set_dir(LED_PIN_RED, GPIO_OUT);
    gpio_put(LED_PIN_RED, 0);
    
    gpio_init(LED_PIN_GREEN);
    gpio_set_dir(LED_PIN_GREEN, GPIO_OUT);
    gpio_put(LED_PIN_GREEN, 0);
    
    gpio_init(LED_PIN_BLUE);
    gpio_set_dir(LED_PIN_BLUE, GPIO_OUT);
    gpio_put(LED_PIN_BLUE, 0);
    
    gpio_set_irq_enabled_with_callback(BUT_PIN_A, GPIO_IRQ_EDGE_FALL, 1, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BUT_PIN_B, GPIO_IRQ_EDGE_FALL, 1, &gpio_irq_handler);
    
    //inicializar o botão de interrupção - GPIO5
    gpio_init(BUT_PIN_A);
    gpio_set_dir(BUT_PIN_A, GPIO_IN);
    gpio_pull_up(BUT_PIN_A);
    
    //inicializar o botão de interrupção - GPIO5
    gpio_init(BUT_PIN_B);
    gpio_set_dir(BUT_PIN_B, GPIO_IN);
    gpio_pull_up(BUT_PIN_B);
    
    //interrupção da gpio habilitada
    gpio_set_irq_enabled_with_callback(BUT_PIN_A, GPIO_IRQ_EDGE_FALL, 1, & gpio_irq_handler);


    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_pull_up(I2C_SDA); // Pull up the data line
    gpio_pull_up(I2C_SCL); // Pull up the clock line
    ssd1306_t ssd; // Inicializa a estrutura do display
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd); // Configura o display
    ssd1306_send_data(&ssd); // Envia os dados para o display

    // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    bool cor = false;


    while (true) {
        if (stdio_usb_connected()){
            char c;
            if (scanf("%c", &c) == 1){
                if (c >= 48 && c < 57){
                    matrix_number = c - 48;
                    numero_pio(numbers[matrix_number], valor_led, pio, sm, r, g, b);
            }
            
            printf("O caractere lido foi: %c\r\n", c);

            ssd1306_t ssd;
            ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
            ssd1306_config(&ssd); // Configura o display
            ssd1306_fill(&ssd, false);
            ssd1306_draw_char(&ssd, c, 59, 27);
            ssd1306_send_data(&ssd);
        }
    }
        sleep_ms(50);
    }
    
}
    uint32_t matrix_rgb(double b, double r, double g)
    {
      unsigned char R, G, B;
      R = r * 255;
      G = g * 255;
      B = b * 255;
      return (G << 24) | (R << 16) | (B << 8);
    }
    
    
    void numero_pio(double *numero, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b){
    
        for (int16_t i = 0; i < NUM_PIXELS; i++) {
            if (i%2==0)
            {
                valor_led = matrix_rgb(numero[24-i], r=0.0, g=0.0);
                pio_sm_put_blocking(pio, sm, valor_led);
    
            }else{
                valor_led = matrix_rgb(b=0.0, numero[24-i], g=0.0);
                pio_sm_put_blocking(pio, sm, valor_led);
            }
        }
    }