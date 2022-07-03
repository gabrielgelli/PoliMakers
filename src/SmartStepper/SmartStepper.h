#ifndef SmartStepper_h
#define SmartStepper_h

#include "Arduino.h"
#include "../TheTimer/TheTimer.h"

class SmartStepper
{
public:
	SmartStepper(bool direcao_principal, int pino_direcao, int pino_step, int pino_sleep, String ptr_Nome_do_Motor);

	void restart(); // Reinicia a contagem de passos e o alinhamento

	// Movimentação Incremental
	bool move(int passos, int metade_do_intervalo);	 // Dá passos na direção positiva ou negativa e retorna true após concluí-los
	bool vai(int passos, int metade_do_intervalo);	 // Dá passos e retorna true após concluí-los
	bool volta(int passos, int metade_do_intervalo); // Dá passos e retorna true após concluí-los
	bool sobe(int passos, int metade_do_intervalo);	 // Dá passos e retorna true após concluí-los
	bool desce(int passos, int metade_do_intervalo); // Dá passos e retorna true após concluí-los

	// Movimentação Absoluta
	void setZero();										   // Seta o step de posição zero
	bool goTo(int step_position, int metade_do_intervalo); // Vai até um step de determinado número

	void startSleep();	   // Liga o sleep mode do driver
	void stopSleep();	   // Desliga o sleep mode do driver
	void startAutoSleep(); // Desliga o sleep mode do driver automaticamente ao iniciar os passos/alinhamento e desliga ao acabá-los
	void stopAutoSleep();

private:
	TheTimer _timerStepperMicros{"_timerStepperMicros"}; // Configurada como MICROS no construtor
	bool _direcao_principal;
	int _pino_direcao;
	int _pino_step;
	int _pino_sleep;
	String _Nome_do_Motor;

	bool _direcao;
	bool _acabou_de_reiniciar = false;
	bool _ja_foi_reiniciado = false; // Para saber se em algum momento foi reiniciado. Do contrário, impede o funcionamento.

	// Movimentação Incremental
	int _passos;
	int _metade_do_intervalo;
	bool _cumpriu_os_passos_solicitados = false;
	bool _valor_pino_step = HIGH;
	int _iteracao = 0;

	// Movimentação Absoluta
	long int step_count = 0;
	bool _cumpriu_o_goTo_solicitado = false;

	// Sleep
	bool _autoSleepLigado = false;
};

#endif
