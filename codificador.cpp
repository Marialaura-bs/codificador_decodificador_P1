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


// Converte o valor para centavos.
string valorBoleto(double valor)
{
    long long centavos = llround(valor * 100);

    string valorString = to_string(centavos);

    return valorString;
}


// Calcula o DV geral do código de barras usando Módulo 11.
int modulo11Dv(string codigo)
{
    int soma = 0;
    int multiplicador = 2;

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


// Monta o código de barras.
string codigoBarras(string banco, string moeda, int venc,
                    double valor, int tipoConvenio, string campoLivre)
{
    string codigo;

    // Completa o banco para 3 posições.
    banco = completarZeros(banco, 3);

    // Converte o valor para centavos.
    string valorCampo = valorBoleto(valor);


    // Verifica se o valor ultrapassou as 10 posições.
    bool valorMaiorQue10 = valorCampo.length() > 10;


    // Se passar de 14 posições, não cabe no espaço
    // ocupado pelo fator + valor.
    if (valorCampo.length() > 14)
    {
        return "";
    }


    // Valor maior que 10 posições:
    // ocupa as 14 posições que seriam fator + valor.
    if (valorMaiorQue10)
    {
        valorCampo = completarZeros(valorCampo, 14);
    }
    else
    {
        // Valor normal ocupa 10 posições.
        valorCampo = completarZeros(valorCampo, 10);
    }


    // Começa com banco + moeda + DV temporário.
    codigo = banco + moeda + "0";


    // Se o valor for maior que 10 posições,
    // o fator de vencimento é eliminado.
    if (valorMaiorQue10)
    {
        codigo += valorCampo;
    }
    else
    {
        codigo += to_string(venc);
        codigo += valorCampo;
    }


    // Formatos de 4 e 6 posições.
    if (tipoConvenio == 4 || tipoConvenio == 6)
    {
        if (campoLivre.length() > 25)
        {
            return "";
        }

        campoLivre = completarZeros(campoLivre, 25);

        codigo += campoLivre;
    }


    // Formato de 7 posições.
    else if (tipoConvenio == 7)
    {
        if (campoLivre.length() > 19)
        {
            return "";
        }

        campoLivre = completarZeros(campoLivre, 19);

        codigo += "000000";
        codigo += campoLivre;
    }


    // Formato de 17 posições.
    else // assume que qualquer valor do tipo de convenio que seja diferente aos anteriores será livre ou seja 17 posições.
    {
        if (campoLivre.length() > 23)
        {
            return "";
        }

        campoLivre = completarZeros(campoLivre, 23);

        codigo += campoLivre;
        codigo += "21";
    }


    // O código de barras precisa ter exatamente 44 posições.
    if (codigo.length() != 44)
    {
        return "";
    }


    // Retira temporariamente o DV da posição 5.
    string codigoSemDv =
        codigo.substr(0, 4) +
        codigo.substr(5, 39);


    // Calcula o DV do código de barras.
    int dv = modulo11Dv(codigoSemDv);


    // Coloca o DV calculado na posição 5.
    codigo[4] = char('0' + dv);


    return codigo;
}


int main()
{
    string banco;
    string moeda;
    string campoLivre;

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


    if (tipoConvenio == 4 || tipoConvenio == 6)
    {
        cout << "Campo livre (ate 25 caracteres): ";
        cin >> campoLivre;
    }
    else if (tipoConvenio == 7)
    {
        cout << "Campo livre (ate 19 caracteres): ";
        cin >> campoLivre;
    }
    else
    {
        cout << "Campo livre (ate 23 caracteres): ";
        cin >> campoLivre;
    }

    // Calcula o fator de vencimento.
    int venc = fatorVencimento(dia, mes, ano);


    // Monta o código de barras.
    string codigo = codigoBarras(
        banco,
        moeda,
        venc,
        valor,
        tipoConvenio,
        campoLivre
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