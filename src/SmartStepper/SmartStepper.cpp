#include "SmartStepper.h"

SmartStepper::SmartStepper(bool direcao_principal, int pino_direcao, int pino_step, int pino_sleep, String ptr_Nome_do_Motor)
{
	_direcao_principal = direcao_principal;
	_pino_direcao = pino_direcao;
	_pino_step = pino_step;
	_pino_sleep = pino_sleep;
	_Nome_do_Motor = ptr_Nome_do_Motor;
	digitalWrite(_pino_step, _valor_pino_step);
	_timerStepperMicros.setUnit(MICROS);
	// restart(); // Talvez não seja bom, pois pode parecer que não precisa usar o restart
}

// Reinicia a contagem de passos
void SmartStepper::restart()
{
	_ja_foi_reiniciado = true;
	_acabou_de_reiniciar = true;
	_direcao = _direcao_principal;
	_cumpriu_o_goTo_solicitado = false;
}

//-------------------------------------------------------------------------------------------------------------------
// Movimentação Incremental

bool SmartStepper::move(int passos, int metade_do_intervalo)
{
	if (passos >= 0)
	{
		_direcao = _direcao_principal;
	}
	else
	{
		_direcao = !_direcao_principal;
	}

	// Inicializa, se é primeira vez que está sendo chamada após o restart()
	if (_acabou_de_reiniciar)
	{
		_passos = abs(passos);
		_metade_do_intervalo = metade_do_intervalo;

		digitalWrite(_pino_direcao, _direcao);
		_iteracao = abs(passos) * 2;
		_cumpriu_os_passos_solicitados = false;
		_acabou_de_reiniciar = false;

		_timerStepperMicros.setReference();
		if (_autoSleepLigado)
			stopSleep();
	}

	// Dá os passos, se ainda faltarem
	if (_iteracao > 0)
	{
		if (_timerStepperMicros.justCompleted(_metade_do_intervalo))
		{
			_valor_pino_step = !_valor_pino_step;
			digitalWrite(_pino_step, _valor_pino_step);
			_iteracao--;
			_timerStepperMicros.setReference();
		}
	}
	// Finaliza, se já tiver dado todos os passos solicitados (função retorna true, que finalizou)
	else
	{
		if (_autoSleepLigado)
			startSleep();
		_cumpriu_os_passos_solicitados = true;
	}
	return (_cumpriu_os_passos_solicitados);
}

bool SmartStepper::vai(int passos, int metade_do_intervalo)
{
	return (move(passos, metade_do_intervalo));
}

bool SmartStepper::volta(int passos, int metade_do_intervalo)
{
	return (move(-1 * passos, metade_do_intervalo));
}
bool SmartStepper::sobe(int passos, int metade_do_intervalo)
{
	return (move(passos, metade_do_intervalo));
}
bool SmartStepper::desce(int passos, int metade_do_intervalo)
{
	return (move(-1 * passos, metade_do_intervalo));
}

//-------------------------------------------------------------------------------------------------------------------
// Movimentação Absoluta

void SmartStepper::setZero()
{
	step_count = 0;
}

bool SmartStepper::goTo(int step_position, int metade_do_intervalo)
{

	if (move(step_position - step_count, metade_do_intervalo)) // Passos finalizados
	{
		if (_cumpriu_o_goTo_solicitado == false) // Primeira chada após finalizar
		{
			step_count = step_position; // Atualiza a posição
			_cumpriu_o_goTo_solicitado = true;
		}
	}

	return _cumpriu_o_goTo_solicitado;
}

//-------------------------------------------------------------------------------------------------------------------
// Sleep Mode

void SmartStepper::startSleep()
{
	digitalWrite(_pino_sleep, LOW); // Habilita o Sleep Mode do driver (desliga o motor)
}

void SmartStepper::stopSleep()
{
	digitalWrite(_pino_sleep, HIGH); // Desabilita o Sleep Mode do driver (liga o motor)
}

void SmartStepper::startAutoSleep()
{
	_autoSleepLigado = true;
	startSleep(); // Para já começar com o motor desligado, já que só deve ligar ao estar sendo usado
}

void SmartStepper::stopAutoSleep()
{
	_autoSleepLigado = false;
}
