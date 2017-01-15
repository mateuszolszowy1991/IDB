#include "DumpDBToFile.hpp"

void DumpDBToFile::saveToFile(std::string napis)
{
    std::fstream plik( "/home/mato3/OSCAR/SYS/TEMP/dump.txt", std::ios::app );
    //std::string date = static_cast<std::string>(getDtTm());
    std::string log = " \t " + napis + "\n";
    plik.write( & log[ 0 ], log.length() );
    plik.close();
}

void DumpDBToFile::clearLogs()
{
    std::fstream plik( "/home/mato3/OSCAR/SYS/TEMP/dump.txt", std::ios::out );
    plik.write( "", 0 );
    plik.close();
}

char* DumpDBToFile::getDtTm()
{   char buff[DTTMSZ];
    time_t t = time (0);
    strftime (buff, DTTMSZ, DTTMFMT, localtime (&t));
    return buff;
}
