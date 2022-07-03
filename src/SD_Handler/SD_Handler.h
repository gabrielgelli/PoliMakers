
#ifndef SD_Handler_h
#define SD_Handler_h

#include <SPI.h>
#include <SD.h>

class SD_Handler
{
public:
    SD_Handler(int chip_select = 5);
    bool begin();

    void leProximoLote();
    String getCor();
    int getQuantidade();
    float getComprimento();
    bool lotesFinalizados();
    void retornaAoInicio();

    // Testes
    void write(String texto); // Caso queira gravar dados no SD para testes
    void read();              // Caso queira ler dados do SD para testes
    void testePrintaLotes();

private:
    int chipSelect;
    File file;

    String cor;
    int quantidade;
    float comprimento;
};

#endif