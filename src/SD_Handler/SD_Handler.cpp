#include "SD_Handler.h"

SD_Handler::SD_Handler(int chip_select)
{
    chipSelect = chip_select;
}

bool SD_Handler::begin()
{
    Serial.println("---------------------------");
    Serial.println("Inicializando cartão SD ...");
    bool resultado = SD.begin(chipSelect);
    if (resultado == false)
    {
        Serial.println("Inicialização falhou!");
    }
    else
    {
        Serial.println("Incialização concluída.");
        file = SD.open("/pedido.txt", FILE_READ);
        Serial.print("Tamanho do pedido.txt: ");
        Serial.println(file.size());
    }
    Serial.println("---------------------------");
    Serial.println("");
    return (resultado);
}

void SD_Handler::leProximoLote()
{
    String str_cor = "";
    String str_qtd = "";
    String str_comp = "";

    bool finalizou_leitura_da_linha = false;
    int elemento_atual = 0;
    char char_atual = 0;
    char char_anterior = 0;

    // Se abriu o arquivo corretamente
    if (file)
    {
        // Realiza leitura da linha de instruções
        // Se ainda há bytes para ler no arquivo e na linha
        while (file.available() > 0 && finalizou_leitura_da_linha == false)
        {

            // Lê o caracter do endereço atual
            char_atual = file.read();

            // Verifica se é uma transição
            if (char_atual == ' ')
            {
                if (char_anterior != ' ')
                {
                    elemento_atual++;
                }
            }
            else if (char_atual == '\r')
            {
                // Ignora, haverá um \n em seguida se a
                // formatação usar quebra de linha do tipo \r\n
            }
            else if (char_atual == '\n')
            {
                finalizou_leitura_da_linha = true;
            }
            // Salva valores
            else
            {
                switch (elemento_atual)
                {
                case 0:
                    str_cor = str_cor + String(char_atual); // Acrescenta novo caracter ao concatená-lo
                    break;
                case 1:
                    str_qtd = str_qtd + String(char_atual);
                    break;
                case 2:
                    str_comp = str_comp + String(char_atual);
                    break;
                }
            }
            char_anterior = char_atual;
        }

        // Atualiza variáveis
        cor = str_cor;
        quantidade = str_qtd.toInt();
        comprimento = str_comp.toFloat();
    }
    else
    {
        Serial.println(">> Erro no SD ao tentar abrir: /pedido.txt <<");
    }
}

String SD_Handler::getCor()
{
    return (cor);
}
int SD_Handler::getQuantidade()
{
    return (quantidade);
}
float SD_Handler::getComprimento()
{
    return (comprimento);
}

bool SD_Handler::lotesFinalizados()
{
    // available() retorna o tamanho - posição atual no arquivo
    // ou seja, quantos bytes restam para serem lidos
    return (file.available() <= 0); // Se terminou o arquivo (leu a última linha)
}

void SD_Handler::retornaAoInicio()
{
    file.close();
    file = SD.open("/pedido.txt", FILE_READ); // Volta à posição inicial
}

// ======================================================================
// Testes

void SD_Handler::write(String texto)
{
    file.close();
    file = SD.open("/pedido.txt", FILE_WRITE);
    file.print(texto);
    file.close();
    file = SD.open("/pedido.txt", FILE_READ);
}

void SD_Handler::read()
{
    file.close();
    file = SD.open("/pedido.txt", FILE_READ);
    // Printa tudo que houver escrito no arquivo
    while (file.available())
    {
        Serial.write(file.read());
    }
    Serial.println("");
    file.close();
    file = SD.open("/pedido.txt", FILE_READ);
}

void SD_Handler::testePrintaLotes()
{
    if (lotesFinalizados() == false)
    {
        Serial.print("Posicao:     ");
        Serial.println(file.position());
        leProximoLote();
        Serial.print("cor:         ");
        Serial.println(getCor());
        Serial.print("quantidade:  ");
        Serial.println(getQuantidade());
        Serial.print("comprimento: ");
        Serial.println(getComprimento());
        Serial.print("Posicao:     ");
        Serial.println(file.position());
        Serial.println("");
        if (lotesFinalizados()) // Se acbou de finalizar
        {
            Serial.println("Finalizou leitura do arquivo!");
        }
    }
}