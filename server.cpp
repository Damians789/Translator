#include <iostream>
#include <bits/stdc++.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#include "header.h"

using namespace std;


string zamiana(map<string, string> slownik, string slowo)
{
    if (!slownik.count(slowo))
        return "Nie znam takiego słowa";
    else
        return slownik[slowo];
}


int main(int argc, char *argv[])
{
    map<string, string> tlumacz{
        {"pies", "perro"},
        {"kot", "gato"},
        {"woda", "agua"},
        {"biegać", "correr"},
        {"piwo", "cerveza"},
        {"jabłko", "manzana"},
        {"tańczyć", "bailar"},
        {"głowa", "cabeza"},
        {"jedzenie", "comida"},
    };

	komunikat kom;
	int kolejka1, kolejka2;
    int errnum;

	kolejka1 = msgget(klucz1, 0777 | IPC_CREAT);
	kolejka2 = msgget(klucz2, 0777 | IPC_CREAT);

	cout << "Serwer rozpoczął działanie" << endl;

    while(1)
    {
        if(msgrcv(kolejka1, &kom, sizeof(char)*256, 0L, 0) == -1)
        {
            errnum = errno;
            fprintf(stderr, "Value of errno: %d\n", errno);
            perror("Error printed by perror");
            fprintf(stderr, "Error opening file[%ld]: %s\n", kom.typ, strerror( errnum ));
            cout << "Nie udało się odebrać danych od klienta" << endl;
            exit(1);
        }else
            cout << endl << kom.typ << " przesyła do przetłumaczenia: " << kom.dane << endl;

        
        string wynik = zamiana(tlumacz, kom.dane);
        strcpy(kom.dane, wynik.c_str());
        cout << "Wynik: " << kom.dane << endl;
        // sleep(4);

        if(msgsnd(kolejka2, &kom, sizeof(char)*256, 0) == -1)
        {
            errnum = errno;
            fprintf(stderr, "Value of errno: %d\n", errno);
            perror("Error printed by perror");
            fprintf(stderr, "Error opening file[%ld]: %s\n", kom.typ, strerror( errnum ));
            cout << "Nie udało się wysłać danych do klienta" << endl;
            exit(1);
        }
        msgctl(klucz1, IPC_RMID, 0);
        msgctl(klucz2, IPC_RMID, 0);
    }
    return 0;
}
