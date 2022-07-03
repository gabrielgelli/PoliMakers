
#ifndef Testes_de_Hardware_h
#define Testes_de_Hardware_h

#include "PoliMakers.h"

void Teste_Sensores()
{
    delay(1000);
    Serial.print("BOTAO_AZUL: ");
    Serial.print(digitalRead(BOTAO_AZUL));
    Serial.print(" BOTAO_VERMELHO: ");
    Serial.print(digitalRead(BOTAO_VERMELHO));
    Serial.print(" END_STOP_CORTE: ");
    Serial.print(digitalRead(END_STOP_CORTE));
    Serial.print(" SENSOR_DE_FIO: ");
    Serial.println(digitalRead(SENSOR_DE_FIO));
}

#endif