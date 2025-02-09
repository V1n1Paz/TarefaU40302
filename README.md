# Projeto: Controle de Matriz de LEDs, LED RGB e Display SSD1306 com Raspberry Pi Pico  

Este projeto utiliza o **Raspberry Pi Pico** e o **Pico SDK** para controlar uma matriz de LEDs WS2812, um LED RGB, um display OLED SSD1306 e botões em **pull-up**.

Link com a demonstração do projeto:  https://youtu.be/Ah0V5p5szFM

## 📌 Funcionalidades  

- Controle de uma matriz de LEDs WS2812 para exibir padrões e animações.  
- Manipulação de um LED RGB para indicar estados do sistema.  
- Exibição de informações em um display **SSD1306** via **I2C**.  
- Uso de botões físicos com resistores de **pull-up** para interações.  
- Implementação de **interrupções** para resposta eficiente aos botões.  

## 🛠️ Tecnologias e Ferramentas  

- **Linguagem**: C  
- **Plataforma**: Raspberry Pi Pico  
- **SDK**: Pico SDK  
- **Bibliotecas utilizadas**:  
  - `hardware/gpio.h` (para manipulação dos botões e LED RGB)  
  - `hardware/i2c.h` (para comunicação com o display SSD1306)  
  - `pico/stdlib.h` (para funcionalidades gerais do Pico SDK)  

## 🔧 Como Compilar e Executar  

1. **Clone o repositório**  
   ```bash
   git clone <URL_DO_REPOSITORIO>
   cd <NOME_DO_REPOSITORIO>
   ```
2. **Configure o ambiente do Pico SDK** (caso ainda não tenha feito)  
3. **Compile o código**  
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```
4. **Carregue o binário no Raspberry Pi Pico**  
5. **Observe e teste a execução com a placa conectada vis USB ao computador.**
