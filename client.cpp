#include <iostream>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#include <unistd.h>
#include "header.h"

using namespace std;


int main(int argc, char* argv[])
{
	komunikat kom;
    long ident = getpid();
    int errnum;
	int kolejka1, kolejka2;

	kolejka1 = msgget(klucz1, 0777);
	kolejka2 = msgget(klucz2, 0777);

    if(!argv[1])
    {
        cout << "Wywołano niepoprawnie.\nPrzykładowe użycie: ./client pies" << endl;
        exit(1);
    }

	strcpy(kom.dane, argv[1]);
	kom.typ = ident;

    if(msgsnd(kolejka1, &kom, sizeof(char)*256, 0) == -1)
    {
        errnum = errno;
        fprintf(stderr, "Value of errno: %d\n", errno);
        perror("Error printed by perror");
        fprintf(stderr, "Error opening file[%ld]: %s\n", kom.typ, strerror( errnum ));
        cout << "Nie udało się wysłać danych do serwera" << endl;
        exit(1);
    }


    if(msgrcv(kolejka2, &kom, sizeof(char)*256, ident, 0) == -1)
    {
        errnum = errno;
        fprintf(stderr, "Value of errno: %d\n", errno);
        perror("Error printed by perror");
        fprintf(stderr, "Error opening file[%ld]: %s\n", kom.typ, strerror( errnum ));
        cout << "Nie udało się odebrać danych od serwera" << endl;
        exit(1);
    }

    string napis = "Polski\t-\tHiszpański";
    cout << napis << endl;
    for (int i=0; i < napis.length(); i++){
        cout << "_";
    }
    cout << "\n" << argv[1] << "\t-\t" << kom.dane << endl;


    return 0;
}
