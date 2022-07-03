/*
+-------------------------+
|       PoliMakers        |
+-------------------------+

Uso:     Máquina Cortador de Jumpers
Autores: Gabriel Gelli Checchinato
         &&
         Pedro Luís Bacelar dos Santos
Data:    06/2022

Código destinado ao controle de uma Máquina Cortadora de Jumpers, desenvolvida
como trabalho para disciplina de Sistemas Embarcados na PoliUSP.
A máquina tem como finalidade fabricar jumpers rígidos de alta qualidade,
para uso próprio e possivelmente comercial sob a marca PoliMakers.

O código utiliza Máquinas de Estados não bloqueantes para acionar de forma
rápida os motores de passo. Essa é uma das formas possíveis de se realizar
passos com pequenos intervalos, como de 50 us.
*/

#include "Maquinas_de_Estados.h"
#include "Testes_de_Hardware.h"

void setup()
{
    // -----------------------------------
    // Pinos

    // Inputs
    pinMode(BOTAO_AZUL, INPUT_PULLUP);
    pinMode(BOTAO_VERMELHO, INPUT_PULLUP);
    pinMode(END_STOP_CORTE, INPUT_PULLUP);
    pinMode(SENSOR_DE_FIO, INPUT);
    // Outputs
    pinMode(STEPPERS_ENABLE, OUTPUT);
    pinMode(STEPPER_CORTE_STEP, OUTPUT);
    pinMode(STEPPER_CORTE_DIR, OUTPUT);
    pinMode(STEPPER_EXTRUSORA_STEP, OUTPUT);
    pinMode(STEPPER_EXTRUSORA_DIR, OUTPUT);
    pinMode(STEPPER_ESTEIRA_STEP, OUTPUT);
    pinMode(STEPPER_ESTEIRA_DIR, OUTPUT);

    digitalWrite(STEPPERS_ENABLE, LOW); // Habilita Steppers

    // -----------------------------------
    // Servo
    servo.attach(SERVO_MOTOR);
    servo.write(0);

    // -----------------------------------
    // Serial
    Serial.begin(115200);
    Serial.println("================");
    Serial.println(">> PoliMakers <<");
    Serial.println("================");
    Serial.println("");

    // -----------------------------------
    // LCD
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("+------------------+");
    lcd.setCursor(0, 1);
    lcd.print("|    PoliMakers    |");
    lcd.setCursor(0, 2);
    lcd.print("|                  |");
    lcd.setCursor(0, 3);
    lcd.print("+------------------+");
    //       ("|| ||| ||  || ||| ||");
    delay(1000);

    //----------------------------------
    // Cartão SD
    if (!sd.begin())
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("WARNING:");
        lcd.setCursor(0, 1);
        lcd.print("Falha ao tentar ler");
        lcd.setCursor(0, 2);
        lcd.print("o cartão SD!");
        //       ("|| ||| ||  || ||| ||");
    }

    // Descomentar para rodar Demonstração
    sd.write(pedido_demo);
    sd.read();

    // -----------------------------------
    // WiFi
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("+------------------+");
    lcd.setCursor(0, 1);
    lcd.print("|    PoliMakers    |");
    lcd.setCursor(0, 2);
    lcd.print("|  Conectando ao   |");
    lcd.setCursor(0, 3);
    lcd.print("|  WiFi...         |");
    //       ("|| ||| ||  || ||| ||");

    Blynk.begin(auth, ssid, pass);
}

void loop()
{
    Rotina_Fabricacao_Completa();
    Blynk.run();

    // -----------------------
    // Testes
    // sd.testePrintaLotes();
    // Teste_Sensores();
}
