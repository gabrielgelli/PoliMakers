#ifndef Config_h
#define Config_h

// Parâmetros Demo para o SD

String pedido_demo = "preto 2 10.16\npreto 2 20.32\npreto 2 40.64\nvermelho 3 30\nazul 2 50";

//------------------------------------------------------
// Parâmetros dos Steppers

/* Calibração dos steppers

Teste: 1000 full steps -> 115,80 mm
Logo: 1 step = 0.1158 mm

Erro máximo ao utilizar full step: +- 0,0579 mm
Completamente desprezível considerenado as grandezas:
- Espaçamento dos furos na Protoboard: 2,54 mm
- Comprimento de fio desencapado: 7,5 mm
*/

// Valores do Hardware
const int microstepping = 16; // Para todos
const float extruder_full_step_in_mm = 0.1158;

// Corte
const int posicao_de_corte = 0;
const int posicao_de_desencapar = 16 * microstepping;
const int posicao_de_empurrar_capa = 20 * microstepping;
const int posicao_de_descanso = 70 * microstepping;

// Extrusora
// Valores em mm
const float comprimento_de_fio_desencapado_em_mm = 7.5;
const float margem_de_corte_em_mm = 4;      // Evita que o corte acabe pegando parte o plástico do próximo jumper.
const float margem_de_desencapar_em_mm = 4; // Na hora de desencapar, movimenta esse valor extra, para que não fique presa
// Valores em passos
const int comprimento_de_fio_desencapado = int((comprimento_de_fio_desencapado_em_mm / extruder_full_step_in_mm) + 0.5) * microstepping;
const int margem_de_corte = int((margem_de_corte_em_mm / extruder_full_step_in_mm) + 0.5) * microstepping;
const int margem_de_desencapar = int((margem_de_desencapar_em_mm / extruder_full_step_in_mm) + 0.5) * microstepping;

// Esteira
const int passos_entre_lotes = 150 * microstepping;

// Velocidades em Microssegundos
const int intervalo_Stepper_Extrusora = 800 / microstepping;
const int intervalo_Stepper_Corte = 4000 / microstepping;
const int intervalo_Stepper_Esteira = 8000 / microstepping;

int mm2steps(float comprimento_em_mm)
{
    return (int((comprimento_em_mm / extruder_full_step_in_mm) + 0.5) * microstepping);
}
#endif