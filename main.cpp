#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <string>
using namespace std;

int Error(int code)
{
    if (code<0 || code>10) return 10;
    string a[]={"Некорректное значение","cin have been failed", "cout have been failed", "File have not been opened", "Problem with getting file stat",
    "File have not been deleted", "Can't overwrite the file", "Captcha failed, operation cancelled", "Badbit or Failbit error using urandom", "File have not been closed",
    };
    cerr<<a[code-1];
    return code;
}
int Capcha()
{
    int a,b,c;
    cout<<"Для подтверждения, пожалуйста, введите результат операции:\n";
    if (cout.fail()) return 3;
    srand(time(NULL));
    a=rand()%10; b=rand()%10;
    cout<<a<<'+'<<b<<'\n';
    if (cout.fail()) return 3;
    cin>>c;
    if (cin.fail()) return 2;
    if (c==a+b) return 0; else return 8;
}

int Gost()
{
    int res;
    ofstream UData;
    string FileWay;
    cout << "Введите путь до файла c информацией\n";
    if (cout.fail()) return Error(3);
    cin >> FileWay;
    if (cin.fail()) return Error(2);
    res=Capcha();
    if (res!=0) return Error(res);
    struct stat sb{};
    if(stat(FileWay.c_str(),&sb)!=0) return Error(5);
    long long Fsize=sb.st_size;
    string str, helper;
    ifstream urandom("/dev/urandom", ios::in|ios::binary);
    if (urandom.fail()) return Error(9);
    while(str.length()<Fsize) {
        urandom >> helper;
        if (urandom.fail()) {
            urandom.close();
            return Error(9);
        }
        str += helper;
    }
    str.erase(str.begin()+Fsize, str.end());
    urandom.close();
    if (urandom.fail()) return Error(9);
    UData.open(FileWay.c_str(), ios::binary);
    if (!UData.is_open()) return Error(4);
    UData<<str;
    if (UData.fail()){
        UData.close();
        return Error(7);
    }
    UData.close();
    if (UData.fail()){
        return Error(10);
    }
    if(remove(FileWay.c_str())) return Error(6);
    return 0;
}
int UserMode()
{
    int res,number; long long str;
    ofstream UData;
    string FileWay;
    cout << "Введите путь до файла c информацией\n";
    if (cout.fail()) return Error(3);
    cin >> FileWay;
    if (cin.fail()) return Error(2);
    res=Capcha();
    if (res!=0) return Error(res);
    struct stat sb{};
    if(stat(FileWay.c_str(),&sb)!=0) return Error(5);
    long long Fsize=sb.st_size;
    cout<<"Сколькими различными значениями вы собираетесь затереть файл (не более 50)?\n";
    if (cout.fail()) return Error(3);
    cin>>number;
    if (cin.fail()) return Error(2);
    if (number<=0 || number>50) return Error(1);
    for (size_t i=0;i<number;i++) {
        cout << "Введите значение байта, которым хотите перезаписать файл\n";
        if (cout.fail()) return Error(3);
        cin >> str;
        if (cin.fail()) return Error(2);
        UData.open(FileWay.c_str(), ios::binary);
        if (!UData.is_open()) return Error(4);
        for (long long i = 0; i < Fsize; i++) {
            UData.write(reinterpret_cast<const char *>(&str), 1);
            if (UData.fail()) {
                UData.close();
                return Error(7);
            }
        }
        UData.close();
        if (UData.fail()) return Error(10);
    }
  if(remove(FileWay.c_str())) return Error(6);
    return 0;
}
int main() {
    cout << "Программа для гарантированного удаления пользовательских данных" <<'\n';
    if (cout.fail()) return Error(3);
    cout<< "Выберите режим работы:\n 1 - удаление по ГОСТ Р 50739-95\n 2 - перезапись файла пользовательским значением\n";
    if (cout.fail()) return Error(3);
    short int mode;
    cin>>mode;
    if (cin.fail()) return Error(2);
    switch (mode)
    {
        case 1: return Gost();
        case 2: return UserMode();
        default: return Error(1);
    }
    return 0;
}