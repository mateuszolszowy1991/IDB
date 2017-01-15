#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>
#include <time.h>
#define DTTMFMT "%H:%M:%S "
#define DTTMSZ 15

class Logger
{
    public:
       static void saveToFile(std::string data);
       static void clearLogs();
    protected:

    private:
        static char* getDtTm ();

};

#endif // DUMPDBTOFILE_H

//std::fstream plik( "nazwa_pliku.txt", std::ios::out ); //zakładamy, że nie wystąpił błąd (plik otwarto/utworzono)
//std::string napis;
//getline( std::cin, napis );
//plik.write( & napis[ 0 ], napis.length() ); //zapisuje dane poczynając od 0 indeksu
