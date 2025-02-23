# Dosímetro de Ruído NR 15

Projeto desenvolvido como Eetapa de Capacitação Profissional - Embarcatech.

Dosímetro de Ruído NR 15 - para a medição e monitoramento de níveis de ruído, visando atender aos requisitos da Norma Regulamentadora NR 15.

## Apresentação do Projeto

Um dosímetro de ruído é um dispositivo usado para medir a exposição ao ruído ao longo do tempo, geralmente em ambientes de trabalho. Ele é essencial para a segurança ocupacional, pois ajuda a determinar se um trabalhador está sendo exposto a níveis de som que podem causar danos auditivos.

Este projeto visa desenvolver um dosímetro de ruído portátil que atenda às normas da NR-15, que regulamenta a exposição dos trabalhadores a níveis de ruído ocupacional. O dispositivo será capaz de medir, em tempo real, os níveis de pressão sonora e calcular a dose de ruído diária, garantindo a segurança e saúde dos trabalhadores em ambientes industriais.

## Objetivos do Projeto

Desenvolver um dispositivo portátil e de baixo custo para medição de ruído ocupacional.
Garantir que o dispositivo atenda aos requisitos da NR-15, fornecendo dados precisos sobre a exposição ao ruído.
Facilitar a monitoração contínua e automática dos níveis de ruído em ambientes de trabalho.
Fornecer alertas sonoros e visuais quando os limites de exposição forem ultrapassados.

## Principais Requisitos
Precisão: O dispositivo deve medir níveis de ruído com precisão de ±1 dB.
Portabilidade: O dosímetro deve ser leve e compacto, permitindo que o trabalhador o carregue consigo durante a jornada de trabalho.
Autonomia: Bateria com duração mínima de 8 horas.
Interface Amigável: Display LCD ou LED para mostrar os níveis de ruído em tempo real e a dose acumulada.
Conformidade: O dispositivo deve ser capaz de calcular a dose de ruído de acordo com os limites estabelecidos pela NR-15.
Durabilidade: Resistente a poeira e água, atendendo ao padrão IP54.

## Descrição do Funcionamento
O dosímetro de ruído portátil funcionará da seguinte maneira:
Captação do Som: Um microfone de alta sensibilidade captará os sons ambientais.
Processamento do Sinal: O sinal será processado por um microcontrolador que calculará o nível de pressão sonora em dB(A).
Cálculo da Dose de Ruído: O microcontrolador calculará a dose de ruído acumulada ao longo do tempo, comparando-a com os limites da NR-15.
Exibição dos Dados: Os dados serão exibidos em um display LCD, mostrando o nível de ruído atual, a dose acumulada e o tempo restante de exposição segura.
Alertas: Se os limites de exposição forem ultrapassados, o dispositivo emitirá alertas sonoros e visuais.
Armazenamento de Dados: O dispositivo poderá armazenar os dados coletados para posterior análise.


## Justificativa
A exposição excessiva ao ruído em ambientes industriais é uma das principais causas de perda auditiva ocupacional. A NR-15 regulamenta limites de tolerância ao ruído para garantir a segurança dos trabalhadores. No entanto, muitas empresas não possuem meios eficientes para monitorar e gerenciar a exposição ao som. O desenvolvimento deste dosímetro possibilita a prevenção de doenças ocupacionais, contribuindo para um ambiente de trabalho mais seguro.


## Hardware Utilizado

- **Microcontrolador:** RP2040
- **Microfone:** [Modelo do sensor utilizado]
- **Conversor ADC:** RP2040
- **Display:  SSD1306, 128x64px, I2C** 
- **LED RGB:** 3 cores
- **Buzzer:** Buzzer piezoelétrico
- **Componentes Adicionais:** [capacitores, resistores, conectores 2 pin e micro-usb]

## Software e Bibliotecas

- **Ambiente de Desenvolvimento:** Visual Code e Pico SDK
- **Linguagem de Programação:** Linguagem C
- **Bibliotecas Utilizadas:** 
  - [<stdio.h>, <math.h>, <string.h, "pico/stdlib.h", "hardware/adc.h", "hardware/dma.h", "hardware/gpio.h", "hardware/pwm.h", "hardware/pio.h" e "inc/ssd1306.h"]




1. **Clone o Repositório:**
   ```bash
   git clone https://github.com/gitdoalberto/dosimetro-ruido-nr15.git
