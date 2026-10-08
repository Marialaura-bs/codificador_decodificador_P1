#include <iostream>
#include <ctime>
#include <string>
#include <cmath>

using namespace std;


// Completa um campo com zeros à esquerda até atingir o tamanho desejado.
string completarZeros(string campo, int tamanho)
{
    while (campo.length() < tamanho)
    {
        campo = "0" + campo;
    }

    return campo;
}


// Calcula o DV dos campos da linha digitável usando Módulo 10.
int modulo10Dv(string campo)
{
    int soma = 0;
    int multiplicador = 2;

    for (int i = campo.length() - 1; i >= 0; i--)
    {
        int numero = campo[i] - '0';

        int resultado = numero * multiplicador;

        if (resultado > 9)
        {
            resultado = (resultado / 10) + (resultado % 10);
        }

        soma += resultado;

        if (multiplicador == 2)
            multiplicador = 1;
        else
            multiplicador = 2;
    }

    int resto = soma % 10;

    if (resto == 0)
    {
        return 0;
    }

    return 10 - resto;
}


// Calcula o fator de vencimento a partir da data.
int fatorVencimento(int dia, int mes, int ano)
{
    tm dataBase = {};

    dataBase.tm_year = 2000 - 1900;
    dataBase.tm_mon = 6;
    dataBase.tm_mday = 3;

    tm vencimento = {};

    vencimento.tm_year = ano - 1900;
    vencimento.tm_mon = mes - 1;
    vencimento.tm_mday = dia;

    time_t base = mktime(&dataBase);
    time_t data = mktime(&vencimento);

    int dias = (data - base) / (60 * 60 * 24);

    int fator = 1000 + dias;

    if (fator > 9999)
    {
        fator = 1000 + (dias % 9000);
    }

    return fator;
}


// Converte o valor para centavos e cria o campo de 10 posições.
string valorBoleto(double valor)
{
    int centavos = round(valor * 100);

    string valorString = to_string(centavos);

    valorString = completarZeros(valorString, 10);

    return valorString;
}


// Calcula o DV geral do código de barras usando Módulo 11.
int modulo11Dv(string codigo)
{
    int soma = 0;
    int multiplicador = 2;

    // Percorre da direita para a esquerda.
    for (int i = codigo.length() - 1; i >= 0; i--)
    {
        int numero = codigo[i] - '0';

        soma += numero * multiplicador;

        multiplicador++;

        if (multiplicador > 9)
        {
            multiplicador = 2;
        }
    }

    int resto = soma % 11;
    int dv = 11 - resto;

    if (dv == 0 || dv == 10 || dv == 11)
    {
        dv = 1;
    }

    return dv;
}


// Monta o código de barras de acordo com o tipo de convênio.
string codigoBarras(string banco, string moeda, int venc, double valor,
                    int tipoConvenio, string convenio,
                    string complemento, string agencia,
                    string conta, string carteira)
{
    string codigo;

    // Banco: 3 posições
    banco = completarZeros(banco, 3);

    // Fator de vencimento: 4 posições
    string fator = completarZeros(to_string(venc), 4);

    // Valor: 10 posições
    string valorCampo = valorBoleto(valor);

    // Campos específicos do convênio.
    if (tipoConvenio == 4)
    {
        convenio = completarZeros(convenio, 4);
        complemento = completarZeros(complemento, 7);
        agencia = completarZeros(agencia, 4);
        conta = completarZeros(conta, 8);
        carteira = completarZeros(carteira, 2);

        codigo =
            banco +
            moeda +
            "0" +
            fator +
            valorCampo +
            convenio +
            complemento +
            agencia +
            conta +
            carteira;
    }
    else if (tipoConvenio == 6)
    {
        convenio = completarZeros(convenio, 6);
        complemento = completarZeros(complemento, 5);
        agencia = completarZeros(agencia, 4);
        conta = completarZeros(conta, 8);
        carteira = completarZeros(carteira, 2);

        codigo =
            banco +
            moeda +
            "0" +
            fator +
            valorCampo +
            convenio +
            complemento +
            agencia +
            conta +
            carteira;
    }
    else if (tipoConvenio == 7)
    {
        convenio = completarZeros(convenio, 7);
        complemento = completarZeros(complemento, 10);
        carteira = completarZeros(carteira, 2);

        codigo =
            banco +
            moeda +
            "0" +
            fator +
            valorCampo +
            "000000" +
            convenio +
            complemento +
            carteira;
    }
    else if (tipoConvenio == 17)
    {
        convenio = completarZeros(convenio, 6);
        complemento = completarZeros(complemento, 17);

        // Para o Nosso Número livre de 17 posições,
        // o documento determina obrigatoriamente "21".
        codigo =
            banco +
            moeda +
            "0" +
            fator +
            valorCampo +
            convenio +
            complemento +
            "21";
    }

    // Verifica se o código possui as 44 posições.
    if (codigo.length() != 44)
    {
        return "";
    }

    // Calcula o DV usando as 43 posições,
    // ignorando temporariamente a posição 5.
    string codigoSemDv =
        codigo.substr(0, 4) +
        codigo.substr(5, 39);

    int dv = modulo11Dv(codigoSemDv);

    // Coloca o DV na posição 5.
    codigo[4] = char('0' + dv);

    return codigo;
}


int main()
{
    string banco;
    string moeda;

    int dia, mes, ano;
    int tipoConvenio;

    double valor;

    cout << "Banco: ";
    cin >> banco;

    cout << "Moeda: ";
    cin >> moeda;

    cout << "Dia: ";
    cin >> dia;

    cout << "Mes: ";
    cin >> mes;

    cout << "Ano: ";
    cin >> ano;

    cout << "Valor: ";
    cin >> valor;

    cout << "Tipo de convenio (4, 6, 7 ou 17): ";
    cin >> tipoConvenio;

    string convenio;
    string complemento;
    string agencia;
    string conta;
    string carteira;

    cout << "Numero do convenio: ";
    cin >> convenio;

    cout << "Complemento do Nosso Numero: ";
    cin >> complemento;

    if (tipoConvenio == 4 || tipoConvenio == 6)
    {
        cout << "Agencia: ";
        cin >> agencia;

        cout << "Conta: ";
        cin >> conta;

        cout << "Carteira: ";
        cin >> carteira;
    }
    else if (tipoConvenio == 7)
    {
        cout << "Carteira: ";
        cin >> carteira;
    }

    int venc = fatorVencimento(dia, mes, ano);

    string codigo = codigoBarras(
        banco,
        moeda,
        venc,
        valor,
        tipoConvenio,
        convenio,
        complemento,
        agencia,
        conta,
        carteira
    );

    if (codigo == "")
    {
        cout << "alerta: dados invalidos para montar o codigo de barras." << endl;
    }
    else
    {
        cout << endl;
        cout << "Codigo de barras: " << codigo << endl;
    }

    return 0;
}