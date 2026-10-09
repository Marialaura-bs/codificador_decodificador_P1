#include <iostream>
#include <ctime>
#include <string>

using namespace std;


// ======================================================
// FUNÇÕES BÁSICAS
// ======================================================

// Verifica se o código de barras possui 44 dígitos numéricos.
bool codigoBarrasValido(string codigo)
{
    // O código de barras deve ter 44 dígitos.
    if (codigo.length() != 44)
    {
        return false;
    }

    // Verifica se todos os caracteres são números.
    for (int i = 0; i < codigo.length(); i++)
    {
        if (codigo[i] < '0' || codigo[i] > '9')
        {
            return false;
        }
    }

    return true;
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


// ======================================================
// LINHA DIGITÁVEL
// ======================================================

// Converte o código de barras em linha digitável.
string linhaDigitavel(string codigo)
{
    // Campo 1: posições 1 a 4 + posições 20 a 24.
    string campo1 = codigo.substr(0, 4);
    campo1 += codigo.substr(19, 5);

    int dv1 = modulo10Dv(campo1);
    campo1 += char('0' + dv1);

    // Campo 2: posições 25 a 34.
    string campo2 = codigo.substr(24, 10);

    int dv2 = modulo10Dv(campo2);
    campo2 += char('0' + dv2);

    // Campo 3: posições 35 a 44.
    string campo3 = codigo.substr(34, 10);

    int dv3 = modulo10Dv(campo3);
    campo3 += char('0' + dv3);

    // Campo 4: dígito verificador geral.
    string campo4 = codigo.substr(4, 1);

    // Campo 5: fator de vencimento + valor.
    string campo5 = codigo.substr(5, 14);

    // Formata os campos com pontos e espaços.
    string linha = campo1.substr(0, 5) + "." +
                   campo1.substr(5, 5) + " " +
                   campo2.substr(0, 5) + "." +
                   campo2.substr(5, 6) + " " +
                   campo3.substr(0, 5) + "." +
                   campo3.substr(5, 6) + " " +
                   campo4 + " " +
                   campo5;

    return linha;
}


// ======================================================
// FATOR DE VENCIMENTO E VALOR
// ======================================================

// Separa o fator de vencimento e o valor.
void separarFatorValor(string campoFatorValor,
                       string &fator, string &valor)
{
    if (campoFatorValor[0] == '0')
    {
        fator = "0000";
        valor = campoFatorValor;
    }
    else
    {
        fator = campoFatorValor.substr(0, 4);
        valor = campoFatorValor.substr(4, 10);
    }
}


// Converte o fator de vencimento em dia, mês e ano.
void dataVencimento(int fator, int &dia, int &mes, int &ano)
{
    tm dataBase = {};

    dataBase.tm_year = 2000 - 1900;
    dataBase.tm_mon = 6;
    dataBase.tm_mday = 3;

    time_t base = mktime(&dataBase);

    int dias = fator - 1000;

    time_t data = base + (dias * 24 * 60 * 60);

    tm *vencimento = localtime(&data);

    dia = vencimento->tm_mday;
    mes = vencimento->tm_mon + 1;
    ano = vencimento->tm_year + 1900;
}


// ======================================================
// IDENTIFICAÇÃO DO CONVÊNIO
// ======================================================

// Identifica o tipo de convênio pelo campo livre.
int identificarTipoConvenio(string campoLivre)
{
    
    if (campoLivre.substr(23, 2) == "21")
    {
        return 17;
    }
    else if (campoLivre.substr(0, 6) == "000000")
    {
        return 7;
    }
    else
    {
        return 0;
    }
}


// Extrai os dados específicos conforme o tipo identificado.
string dadosConvenio(string campoLivre, int tipoConvenio)
{
    if (tipoConvenio == 7)
    {
        return campoLivre.substr(6, 19);
    }
    else if (tipoConvenio == 17)
    {
        return campoLivre.substr(0, 23);
    }
    else if (tipoConvenio == 4 || tipoConvenio == 6)
    {
        return campoLivre;
    }

    return "";
}


// ======================================================
// MAIN
// ======================================================

int main()
{
    string codigo;

    cout << "Digite o codigo de barras: ";
    cin >> codigo;

    // Verifica se o código de barras é válido.
    if (!codigoBarrasValido(codigo))
    {
        cout << "alerta vermelho: codigo de barras invalido."
             << endl;
        return 0;
    }

    // Extrai os campos do código de barras.
    string banco = codigo.substr(0, 3);
    string moeda = codigo.substr(3, 1);
    char dvGeral = codigo[4];
    string campoLivre = codigo.substr(19, 25);
    string campoFatorValor = codigo.substr(5, 14);

    // Separa o fator de vencimento e o valor.
    string fator;
    string valor;

    separarFatorValor(campoFatorValor, fator, valor);

    // Converte o valor de centavos para reais.
    int valorCentavos = stoi(valor);
    double valorReais = valorCentavos / 100.0;

    // Calcula a linha digitável e identifica o convênio.
    string linha = linhaDigitavel(codigo);
    int tipoConvenio = identificarTipoConvenio(campoLivre);

    // Exibe os dados extraídos.
    cout << "Linha digitavel: " << linha << endl;
    cout << "Banco: " << banco << endl;
    cout << "Moeda: " << moeda << endl;
    cout << "Digito verificador geral: " << dvGeral << endl;
    cout << "Campo livre: " << campoLivre << endl;
    cout << "Fator: " << fator << endl;
    cout << "Valor: R$ " << valorReais << endl;

    // Exibe a data de vencimento.
    if (fator != "0000")
    {
        int dia, mes, ano;

        dataVencimento(stoi(fator), dia, mes, ano);

        cout << "Data de vencimento: ";

        if (dia < 10)
            cout << "0";

        cout << dia << "/";

        if (mes < 10)
            cout << "0";

        cout << mes << "/" << ano << endl;
    }
    else
    {
        cout << "Data de vencimento: nao informada no codigo."
             << endl;
    }

    // Exibe o tipo e os dados específicos do convênio.
    if (tipoConvenio != 0)
    {
        cout << "Tipo de convenio identificado: "
             << tipoConvenio << endl;

        cout << "Dados especificos do convenio: "
             << dadosConvenio(campoLivre, tipoConvenio) << endl;
    }
    else
    {
        cout << "Tipo de convenio: nao identificado automaticamente."
             << endl;
    }

    return 0;
}
