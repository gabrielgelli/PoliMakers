
#ifndef Maquinas_de_Estados_h
#define Maquinas_de_Estados_h

#include "PoliMakers.h"
#include "Config.h"

//======================================================================================
// Variáveis/Constantes

enum Estados_Fabricacao_Completa
{
    Nenhum_Fabricacao_Completa = 0,
    Aguarda_Confirmacao_do_Ususario,
    Prepara_Fabricacao,
    Gerencia_Fabricacao_dos_Lotes,
    Fabrica_Lote,
    Troca_de_Fio
};

String Nome_Estados_Fabricacao_Completa[] = {
    "Nenhum_Fabricacao_Completa",
    "Aguarda_Confirmacao_do_Ususario",
    "Prepara_Fabricacao",
    "Gerencia_Fabricacao_dos_Lotes",
    "Fabrica_Lote",
    "Troca_de_Fio"};

enum Estados_Fabricacao_Completa Estado_Fabricacao_Completa = Aguarda_Confirmacao_do_Ususario;
enum Estados_Fabricacao_Completa Estado_Anterior_Fabricacao_Completa = Nenhum_Fabricacao_Completa;

// Parâmetros dos lotes
String cor_do_fio = "";
String cor_do_fio_anterior = "";
int quantidade_de_unidades = 0;
float comprimento_do_fio_em_mm = 0; // Parte interna, sem considerar as pontas desencapadas
int comprimento_do_fio = 0;         // Em passos

int contador_de_lotes = 0;
int contador_de_unidades = 0;

//======================================================================================
// Funções

// Declarações
void Rotina_Fabricacao_Completa();
bool SubRotina_Prepara_Fio(bool reiniciar = false);
bool SubRotina_Fabrica_Unidade(bool reiniciar = false);
void Le_Proximo_Lote_no_SD();
void exibir_status_no_LCD();

// Rotina_Fabricacao_Completa
void Rotina_Fabricacao_Completa()
{
    static bool Entrou_no_Estado = false;

    if (Estado_Anterior_Fabricacao_Completa != Estado_Fabricacao_Completa)
    {
        // Serve para executar algo só na vez em que entra no estado,
        // e não nas milhares de vezes seguintes em que se continua nele.
        Entrou_no_Estado = true;
        Serial.println(Nome_Estados_Fabricacao_Completa[Estado_Fabricacao_Completa]);
        Estado_Anterior_Fabricacao_Completa = Estado_Fabricacao_Completa;

        Stepper_Esteira.restart();
    }
    else
    {
        Entrou_no_Estado = false;
    }

    switch (Estado_Fabricacao_Completa)
    {

    case Aguarda_Confirmacao_do_Ususario:
        if (Entrou_no_Estado)
        {
            sd.retornaAoInicio();
            Le_Proximo_Lote_no_SD();

            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Coloque o fio:      ");
            lcd.setCursor(0, 1);
            lcd.print(cor_do_fio);
            lcd.setCursor(0, 2);
            lcd.print("e aperte Select para");
            lcd.setCursor(0, 3);
            lcd.print("iniciar a fabricacao");
            //       ("|| ||| ||  || ||| ||");
        }
        if (digitalRead(BOTAO_AZUL) == LOW) // Botão pressionado
        {
            Estado_Fabricacao_Completa = Prepara_Fabricacao;
        }
        break;

    case Prepara_Fabricacao:
        if (Entrou_no_Estado)
        {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("                    ");
            lcd.setCursor(0, 1);
            lcd.print("Preparando o fio... ");
            lcd.setCursor(0, 2);
            lcd.print("                    ");
            lcd.setCursor(0, 3);
            lcd.print("                    ");
            //       ("|| ||| ||  || ||| ||");
            SubRotina_Prepara_Fio(true); // reinicia
        }
        if (SubRotina_Prepara_Fio())
        {
            Estado_Fabricacao_Completa = Fabrica_Lote;
        }
        break;

    case Fabrica_Lote:
        if (Entrou_no_Estado)
        {
            contador_de_unidades = 0;
            exibir_status_no_LCD();
        }
        if (SubRotina_Fabrica_Unidade()) // Ao terminar
        {
            SubRotina_Fabrica_Unidade(true); // reinicia
            contador_de_unidades++;
            if (contador_de_unidades < quantidade_de_unidades)
            {
                exibir_status_no_LCD();
            }
            else
            {
                cor_do_fio_anterior = cor_do_fio;
                contador_de_lotes++;
                Estado_Fabricacao_Completa = Gerencia_Fabricacao_dos_Lotes;
            }
        }
        break;

    case Gerencia_Fabricacao_dos_Lotes:
        if (Entrou_no_Estado)
        {
            exibir_status_no_LCD();
            Le_Proximo_Lote_no_SD();
        }
        if (Stepper_Esteira.vai(passos_entre_lotes, intervalo_Stepper_Esteira))
        {
            if (sd.lotesFinalizados())
            {
                Blynk.logEvent("lote_finalizado", "Lotes finalizados com sucesso! :)");
                Blynk.syncAll();
                contador_de_lotes = 0;
                Estado_Fabricacao_Completa = Aguarda_Confirmacao_do_Ususario;
            }
            else
            {
                if (cor_do_fio == cor_do_fio_anterior)
                    Estado_Fabricacao_Completa = Fabrica_Lote;
                else
                    Estado_Fabricacao_Completa = Troca_de_Fio; // Se a cor do lote novo for diferente
            }
        }
        break;

    case Troca_de_Fio:
        if (Entrou_no_Estado)
        {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Troque o fio de cor:");
            lcd.setCursor(0, 1);
            lcd.print(cor_do_fio_anterior);
            lcd.setCursor(0, 2);
            lcd.print("Por: " + cor_do_fio);
            lcd.setCursor(0, 3);
            lcd.print("Depois aperte Select");
            //       ("|| ||| ||  || ||| ||");

            Blynk.logEvent("lote_finalizado", "Troque o fio " + cor_do_fio_anterior + " por " + cor_do_fio);
            Blynk.syncAll();
        }
        if (digitalRead(BOTAO_AZUL) == LOW) // Botão pressionado
        {
            Estado_Fabricacao_Completa = Fabrica_Lote;
        }
        break;
    }
}

bool SubRotina_Prepara_Fio(bool reiniciar)
{
    static bool Entrou_no_Estado = true;
    static int Estado_Prepara_Fio = 1;
    static int Estado_Anterior_Prepara_Fio = 0;

    if (reiniciar)
    {
        Estado_Prepara_Fio = 1;
        Estado_Anterior_Prepara_Fio = 0;
    }

    if (Estado_Anterior_Prepara_Fio != Estado_Prepara_Fio)
    {
        // Serve para executar algo só na vez em que entra no estado,
        // e não nas milhares de vezes seguintes em que se continua nele.
        Entrou_no_Estado = true;
        Estado_Anterior_Prepara_Fio = Estado_Prepara_Fio;

        // Reinicia os Steppers
        Stepper_Corte.restart();
        Stepper_Extrusora.restart();
    }
    else
    {
        Entrou_no_Estado = false;
    }

    switch (Estado_Prepara_Fio)
    {
        // ==============================================================
        // Alinha Alicate
        // Seta o Zero do Corte

    case 1:
        if (digitalRead(END_STOP_CORTE) == LOW) // Se alinhou (pressionou o end-stop)
        {
            Stepper_Corte.setZero();
            Estado_Prepara_Fio++;
        }
        else if (Stepper_Corte.volta(100 * microstepping, 2 * intervalo_Stepper_Corte))
        {
            // To do: Entrar em Erro
        }
        break;

        // ==============================================================
        // Corta Ponta do fio
        // Elimina pontas ruins e alinha a extremidade do fio

    case 2: // Descanso
        if (Stepper_Corte.goTo(posicao_de_descanso, intervalo_Stepper_Corte))
            Estado_Prepara_Fio++;
        break;
    case 3: // Movimenta ponta a ser cortada
        if (Stepper_Extrusora.vai(2 * comprimento_de_fio_desencapado, intervalo_Stepper_Extrusora))
            Estado_Prepara_Fio++;
        break;
    case 4: // Corte
        if (Stepper_Corte.goTo(posicao_de_corte, intervalo_Stepper_Corte))
            Estado_Prepara_Fio++;
        break;
    case 5: // Descanso
        if (Stepper_Corte.goTo(posicao_de_descanso, intervalo_Stepper_Corte))
            Estado_Prepara_Fio++;
        break;

        // ==============================================================
        // Desencapa a Ponta
        // Cria a Margem de Corte

    case 6: // Move o fio para desencapar a margem de corte
        if (Stepper_Extrusora.vai(margem_de_corte, intervalo_Stepper_Extrusora))
            Estado_Prepara_Fio++;
        break;
    case 7: // Desencapar
        if (Stepper_Corte.goTo(posicao_de_desencapar, intervalo_Stepper_Corte))
            Estado_Prepara_Fio++;
        break;
    case 8: // Empurrar Capa
        if (Stepper_Corte.goTo(posicao_de_empurrar_capa, intervalo_Stepper_Corte))
            Estado_Prepara_Fio++;
        break;
    case 9: // Empurra a capa
        if (Stepper_Extrusora.volta(margem_de_corte + margem_de_desencapar, intervalo_Stepper_Extrusora))
            Estado_Prepara_Fio++;
        break;
    case 10: // Descanso
        if (Stepper_Corte.goTo(posicao_de_descanso, intervalo_Stepper_Corte))
            Estado_Prepara_Fio++;
        break;
    case 11: // Move o fio para desencapar a ponta interna
        if (Stepper_Extrusora.vai(margem_de_desencapar, intervalo_Stepper_Extrusora))
            Estado_Prepara_Fio++;
        break;

    case 12:
        // Finalizado
        break;
    }

    return (Estado_Prepara_Fio == 12);
}

bool SubRotina_Fabrica_Unidade(bool reiniciar)
{
    static bool Entrou_no_Estado = true;
    static int Estado_Fabrica_Unidade = 1;
    static int Estado_Anterior_Fabrica_Unidade = 0;

    if (reiniciar)
    {
        Estado_Fabrica_Unidade = 1;
        Estado_Anterior_Fabrica_Unidade = 0;
    }

    if (Estado_Anterior_Fabrica_Unidade != Estado_Fabrica_Unidade)
    {
        // Serve para executar algo só na vez em que entra no estado,
        // e não nas milhares de vezes seguintes em que se continua nele.
        Entrou_no_Estado = true;
        Estado_Anterior_Fabrica_Unidade = Estado_Fabrica_Unidade;

        // Reinicia os Steppers
        Stepper_Corte.restart();
        Stepper_Extrusora.restart();
    }
    else
    {
        Entrou_no_Estado = false;
    }

    switch (Estado_Fabrica_Unidade)
    {
    case 1: // Descanso
        if (Stepper_Corte.goTo(posicao_de_descanso, intervalo_Stepper_Corte))
            Estado_Fabrica_Unidade++;
        break;
    case 2: // Move o fio para desencapar a ponta externa
        if (Stepper_Extrusora.vai(2 * comprimento_de_fio_desencapado + margem_de_corte, intervalo_Stepper_Extrusora))
            Estado_Fabrica_Unidade++;
        break;
    case 3: // Desencapar
        if (Stepper_Corte.goTo(posicao_de_desencapar, intervalo_Stepper_Corte))
            Estado_Fabrica_Unidade++;
        break;
    case 4: // Empurrar Capa
        if (Stepper_Corte.goTo(posicao_de_empurrar_capa, intervalo_Stepper_Corte))
            Estado_Fabrica_Unidade++;
        break;
    case 5: // Empurra a capa
        if (Stepper_Extrusora.volta(2 * comprimento_de_fio_desencapado + margem_de_corte + margem_de_desencapar, intervalo_Stepper_Extrusora))
            Estado_Fabrica_Unidade++;
        break;
    case 6: // Descanso
        if (Stepper_Corte.goTo(posicao_de_descanso, intervalo_Stepper_Corte))
            Estado_Fabrica_Unidade++;
        break;
    case 7: // Move o fio para desencapar a ponta interna
        if (Stepper_Extrusora.vai(2 * comprimento_de_fio_desencapado + comprimento_do_fio + margem_de_corte + margem_de_desencapar, intervalo_Stepper_Extrusora))
            Estado_Fabrica_Unidade++;
        break;
    case 8: // Desencapar
        if (Stepper_Corte.goTo(posicao_de_desencapar, intervalo_Stepper_Corte))
            Estado_Fabrica_Unidade++;
        break;
    case 9: // Empurrar Capa
        if (Stepper_Corte.goTo(posicao_de_empurrar_capa, intervalo_Stepper_Corte))
            Estado_Fabrica_Unidade++;
        break;
    case 10: // Empurra a capa, desencapa a ponta interna
        if (Stepper_Extrusora.volta(comprimento_de_fio_desencapado + margem_de_corte, intervalo_Stepper_Extrusora))
            Estado_Fabrica_Unidade++;
        break;
    case 11: // Descanso
        if (Stepper_Corte.goTo(posicao_de_descanso, intervalo_Stepper_Corte))
            Estado_Fabrica_Unidade++;
        break;
    case 12: // Move o fio para a posição de corte
        if (Stepper_Extrusora.vai(comprimento_de_fio_desencapado, intervalo_Stepper_Extrusora))
            Estado_Fabrica_Unidade++;
        break;
    case 13: // Angulo_de_Corte
        if (Stepper_Corte.goTo(posicao_de_corte, intervalo_Stepper_Corte))
            Estado_Fabrica_Unidade++;
        break;
    case 14: // Angulo_de_Descanso
        if (Stepper_Corte.goTo(posicao_de_descanso, intervalo_Stepper_Corte))
            Estado_Fabrica_Unidade++;
        break;
    case 15: // Angulo_de_Descanso
        // Finalizado
        break;
    }

    return (Estado_Fabrica_Unidade == 15);
}

void Le_Proximo_Lote_no_SD()
{
    // Obtém do SD parâmetros do Lote
    sd.leProximoLote();
    cor_do_fio = sd.getCor();
    quantidade_de_unidades = sd.getQuantidade();
    comprimento_do_fio_em_mm = sd.getComprimento();
    comprimento_do_fio = mm2steps(comprimento_do_fio_em_mm);
    exibir_status_no_LCD();
}

void exibir_status_no_LCD()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Pecas Fabricadas    ");
    lcd.setCursor(0, 1);
    lcd.print("Lotes:    " + String(contador_de_lotes) + "        ");
    lcd.setCursor(0, 2);
    lcd.print("Unidades: " + String(contador_de_unidades) + "        ");
    lcd.setCursor(0, 3);
    lcd.print("                    ");
    //       ("|| ||| ||  || ||| ||");
}

#endif