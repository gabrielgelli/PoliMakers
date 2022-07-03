/* *****************************************
  PURPOSE:	 Timer assíncrono para permitir multitasking
  Created by Gabriel Gelli Checchinato
  DATE:		   long ago
****************************************** */

#ifndef TheTimer_h
#define TheTimer_h

#include "Arduino.h"

enum Units // Unidades de tempo à serem escolhidas
{
  MILLIS,
  MICROS
};
static bool errorStarted = false;
static int num_de_timers = 0;

class TheTimer
{

public:
  TheTimer(String timer_name, Units unit = MILLIS); // Escolher entre MILLIS e MICROS
  void setUnit(Units unit);                         // Escolher entre MILLIS e MICROS

  void setReference();                        // Salva um instante de referência e reinicia contagem de ambas à seguir
  bool completed(unsigned long interval);     // Retorna true se completou o intervalo e em todas as próximas
  bool justCompleted(unsigned long duration); // Retorna true uma vez se completou o intervalo e false nas próximas

  // Retorna o tempo decorrido desde setReference()
  unsigned long getElapsedTime();

  //-----------------------------------------
  // Prints
  static void enableLogPrints();  // Habilita prints para todas as instâncias
  static void disableLogPrints(); // Desabilita prints para todas as instâncias

  //-----------------------------------------
  // Erro
  bool emErro(); // Responde se algum timer entrou em erro
                 // Tornar global depois de alguma forma (da classe, não de um dos objetos)
  String errorMsg();

private:
  String null_str = "";
  String _timer_name;
  Units _unit;   // Unidade de tempo escolhida
  int _timer_id; // Identificação do objeto, ao instanciar o timer seu id se torna o número de timers instanciados antes dele (começa em 0)
  unsigned long _startTime;
  unsigned long _duration; // Salva a duração passada pelo justCompleted logo após o setReference, para impedir que seja alterada

  bool _ReferenceHasBeenSet = false;         // Para saber se em algum momento foi feita uma referência. Do contrário, impede o funcionamento.
  bool _ReferenceHasJustBeenSet = false;     // Para saber quando a _duration pode ser atualizada
  bool _informed_that_justCompleted = false; // Se o justCompleted() já retornou true uma vez após completar o intervalo

  unsigned long Now(); // Retorna o instante atual de acordo com a unidade de tempo escolhida

  //-----------------------------------------
  // Prints
  static bool _logPrintsEnabled;

  //-----------------------------------------
  // Erro
  String _error_message = "";
  void startError(String error_message); // Inicia o erro em todos os Timers
};

#endif
