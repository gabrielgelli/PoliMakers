#include "TheTimer.h"

TheTimer::TheTimer(String timer_name, Units unit)
{
	_timer_id = num_de_timers; // Identificação do obejeto instanciado
	num_de_timers++;

	_timer_name = timer_name;
	_unit = unit;
}

void TheTimer::setUnit(Units unit)
{
	_unit = unit;
}

//===============================================================================================================================================
// SetReference

// Os métodos à seguir iniciam suas contagens de tempo no momento em que o setReference() é chamado

void TheTimer::setReference()
{
	if (_logPrintsEnabled)
	{
		Serial.print("[");
		Serial.print(_timer_name);
		Serial.print(", ");
		Serial.print(_timer_id);
		Serial.print("]: ");
		Serial.println("reference set");
	}
	_startTime = Now();
	_ReferenceHasBeenSet = true;
	_ReferenceHasJustBeenSet = true;
	_informed_that_justCompleted = false;
}

// Retorna true sempre após atingir o intervalo, isso até o _startTime ser resetado
bool TheTimer::completed(unsigned long interval)
{
	if (_ReferenceHasBeenSet)
	{
		return ((Now() - _startTime) >= interval);
	}
	else
	{
		//                 |---------------------- 50 ----------------------||---------------------- 50 ----------------------|
		String mensagem = null_str +
						  _timer_name + " teve o completed() chamado antes de chamar ao menos uma vez o setReference()" + "\n" +
						  "Não é possível checar se completou o intervalo antes de definir o momento de referência";
		startError(mensagem);
	}
}

// Retorna true apenas uma vez após atingir o intervalo
bool TheTimer::justCompleted(unsigned long duration)
{
	if (_ReferenceHasJustBeenSet)
	{
		_ReferenceHasJustBeenSet = false;
		_duration = duration;
	}
	if (duration != _duration)
	{ // Isso é para garantir que não sejam passados intervalos diferentes
		//                 |---------------------- 50 ----------------------||---------------------- 50 ----------------------|
		String mensagem = null_str +
						  _timer_name + " apresentou duration != _duration" + "\n" +
						  "Foi passado um intervalo diferente para a justCompleted() sem definir um novo ponto de referência";
		startError(mensagem);
	}
	if (completed(duration) && _informed_that_justCompleted == false)
	{
		_informed_that_justCompleted = true;
		return (true);
	}
	else
	{
		return (false);
	}
}

//===============================================================================================================================================
// Outros

// Retorna o instante atual de acordo com a unidade de tempo escolhida
unsigned long TheTimer::Now()
{
	switch (_unit)
	{
	case MILLIS:
		return (millis());
		break;
	case MICROS:
		return (micros());
		break;
	}
}

unsigned long TheTimer::getElapsedTime()
{
	return (Now() - _startTime);
}

// Initialize static member of class TheTimer
// Isto implica que a variável será compartilhada por todas as instâncias
bool TheTimer::_logPrintsEnabled = false;

// Static Function Member
// Habilita prints para todas as instâncias
void TheTimer::enableLogPrints()
{
	_logPrintsEnabled = true;
};

// Static Function Member
// Desabilita prints para todas as instâncias
void TheTimer::disableLogPrints()
{
	_logPrintsEnabled = false;
};

//===============================================================================================================================================
// Erro

bool TheTimer::emErro()
{
	return (errorStarted);
}

String TheTimer::errorMsg()
{
	return (_error_message);
}

void TheTimer::startError(String error_message)
{
	if (errorStarted == false)
	{
		errorStarted = true;
		Serial.println("\n\n");
		Serial.println("===> Erro na TheTimer <===");
		Serial.print("Timer ID: ");
		Serial.println(_timer_id);
		Serial.println(error_message);
		Serial.println("\n\n");
	}
}
