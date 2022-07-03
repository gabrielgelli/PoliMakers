/*
Inputs e Outputs

Devido à algumas restrições do ESP32, apenas parte de seus pinos podem ser utilizados.
Visando tornar a escolha segura e prática, apenas os pinos diponíveis poderão ser escolhidos.

Dentre os 25 pinos que poderiam ser utilizados como GPIOs, foram selecionados para 23,
resguardando os pinos para comunicação Serial, por ser considerada essencial para um
projeto de desenvolvimento.

Totalidade de pinos utilizáveis da placa
- Numero máximo de Inputs: 23
- Numero máximo de Outputs: 19 (Temos 4 pinos input only)

Referência: https://randomnerdtutorials.com/esp32-pinout-reference-gpios/

*/

#ifndef Pinos_ESP32_h
#define Pinos_ESP32_h

enum Pinos_ESP32 : int
{
    //  GPIO	      Input	    Output	Notes

    // IO_0 = 0,//    pulled up	OK		Boot Button (outputs PWM signal at boot)
    // IO_1 = 1,//    TX pin	OK		debug output at boot

    IO_2 = 2, //      OK	    OK		connected to on-board LED

    // IO_3 = 3,//    OK	    RX pin	HIGH at boot

    IO_4 = 4, //      OK	    OK
    IO_5 = 5, //      OK	    OK		outputs PWM signal at boot

    // IO_6 = 6,//    x			x		connected to the integrated SPI flash
    // IO_7 = 7,//    x			x		connected to the integrated SPI flash
    // IO_8 = 8,//    x			x		connected to the integrated SPI flash
    // IO_9 = 9,//    x			x		connected to the integrated SPI flash
    // IO_10 = 10,//  x			x		connected to the integrated SPI flash
    // IO_11 = 11,//  x			x		connected to the integrated SPI flash

    IO_12 = 12, //    OK		OK		boot fail if pulled high
    IO_13 = 13, //	  OK		OK
    IO_14 = 14, //	  OK		OK		outputs PWM signal at boot
    IO_15 = 15, //	  OK		OK		outputs PWM signal at boot
    IO_16 = 16, //	  OK		OK
    IO_17 = 17, //	  OK		OK
    IO_18 = 18, //	  OK		OK
    IO_19 = 19, //	  OK		OK

    IO_21 = 21, //	  OK		OK
    IO_22 = 22, //	  OK		OK
    IO_23 = 23, //	  OK		OK

    IO_25 = 25, //	  OK		OK
    IO_26 = 26, //	  OK		OK
    IO_27 = 27, //	  OK		OK

    IO_32 = 32, //	  OK		OK
    IO_33 = 33, //	  OK		OK
    IO_34 = 34, //	  OK		input only
    IO_35 = 35, //	  OK		input only
    IO_36 = 36, //	  OK		input only
    IO_39 = 39, //	  OK		input only
};

#endif