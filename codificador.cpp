#include <iostream>
#include <ctime>
using namespace std;

int main(){
    int Cod_banco, moeda, dia, mes, ano, valor, convenio, venc, dv;
    cin >> Cod_banco >> moeda >> dia >> mes >> ano >> valor;
    venc = fatorVencimento(dia, mes, ano);
    dv = Dv(Cod_banco, moeda, venc, valor, convenio);
}
int Dv(Cod_banco, moeda, venc, valor, convenio){

}

int fatorVencimento(dia, mes, ano) {
    tm dataBase = {};
    dataBase.tm_year = 2000 - 1900;
    dataBase.tm_mon = 6;  // julho
    dataBase.tm_mday = 3;

    tm vencimento = {};
    vencimento.tm_year = ano - 1900;
    vencimento.tm_mon = mes - 1;
    vencimento.tm_mday = dia;

    time_t base = mktime(&dataBase);
    time_t data = mktime(&vencimento);

    int dias = (data - base) / (60 * 60 * 24);

    int fator = 1000 + dias;

    // Após 21/02/2025, o fator volta para 1000
    if (fator > 9999)
        fator = 1000 + (dias % 9000);

    return fator;
}