//--------------------------//
//       Declarações        //
//--------------------------//

/*
Arquivo destinado à declarar/definir/inicializar variáveis e instanciar objetos
*/

#ifndef PoliMakers_h
#define PoliMakers_h

#include "Pinos_ESP32.h"
// #include "Config.h"
// #include "Maquinas_de_Estados.h"

//------------------------------------------------------
// Pinos

// Ordem do lado direito do ESP32
// Se está conectado ao CNC Shield, possui o nome do pino comentado ao lado

// Inputs
const int BOTAO_AZUL = IO_39;     // Pressionado: LOW
const int BOTAO_VERMELHO = IO_36; // Pressionado: LOW

const int END_STOP_CORTE = IO_34; // y end-stop, Pressionado: LOW
const int SENSOR_DE_FIO = IO_35;  // z end-stop, Possui fio: HIGH

// Outputs
const int STEPPER_CORTE_STEP = IO_32;     // x dir
const int STEPPER_EXTRUSORA_STEP = IO_33; // y dir
const int STEPPER_ESTEIRA_STEP = IO_25;   // z dir
const int STEPPER_CORTE_DIR = IO_26;      // x step
const int STEPPER_EXTRUSORA_DIR = IO_27;  // y step
const int STEPPER_ESTEIRA_DIR = IO_14;    // z step
const int STEPPERS_ENABLE = IO_13;        // With LOW, enables all CNC Shield steppers (possui pullup)

const int SERVO_MOTOR = IO_12; // x lim (GPIO_12 boot fail if pulled high)

//----------------------------------
// LCD

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// Default address é 0x27 (A0-A2 not jumpered)
LiquidCrystal_I2C lcd(0x27, 20, 4);

//----------------------------------
// Motores de Passo

#include "src/SmartStepper/SmartStepper.h"

SmartStepper Stepper_Corte(HIGH, STEPPER_CORTE_DIR, STEPPER_CORTE_STEP, STEPPERS_ENABLE, "Stepper_Corte");
SmartStepper Stepper_Extrusora(HIGH, STEPPER_EXTRUSORA_DIR, STEPPER_EXTRUSORA_STEP, STEPPERS_ENABLE, "Stepper_Extrusora");
SmartStepper Stepper_Esteira(HIGH, STEPPER_ESTEIRA_DIR, STEPPER_ESTEIRA_STEP, STEPPERS_ENABLE, "Stepper_Esteira");

//----------------------------------
// Servomotor

#include <Servo.h>
Servo servo;

//----------------------------------
// Cartão SD

#include "src/SD_Handler/SD_Handler.h"
SD_Handler sd(IO_5);

//----------------------------------
// Timer

#include "src/TheTimer/TheTimer.h"
TheTimer timer_1("timer_1");

//----------------------------------
// WI-Fi

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL_W3Ey9uQ"
#define BLYNK_DEVICE_NAME "PoliMakers"
#define BLYNK_AUTH_TOKEN "5i4bsKwPFx42i5ySN6fCHHrDdQjfx_xC"

char auth[] = BLYNK_AUTH_TOKEN;

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Starlink";
char pass[] = "SpaceX60";

#endif