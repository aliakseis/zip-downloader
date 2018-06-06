// zip-downloader.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "unzip/unzip.h"

#include <windows.h>
#include <string>
#include <fstream>
#include <iostream>
#include <direct.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: zip=downloader url\n";
        return 1;
    }

    CoInitialize(NULL);
    atexit([] {CoUninitialize(); });

    char* zipfile = argv[1];

    unzFile uf = unzOpen(zipfile);
    if (!uf)
    {
        return 1;
    }
    unzGoToFirstFile(uf);
    do {
        char filename[MAX_PATH];
        unzGetCurrentFileInfo(uf, 0, filename, sizeof(filename), 0, 0, 0, 0);

        auto filenamelen = strlen(filename);
        if (filenamelen > 0 && filename[filenamelen - 1] == '/')
        {
            _mkdir(filename);
        }
        else
        {
            unzOpenCurrentFile(uf);

            std::ofstream f(filename, std::ofstream::binary);

            char buf[1024 * 64];
            int r;
            do
            {
                r = unzReadCurrentFile(uf, buf, sizeof(buf));
                if (r > 0)
                {
                    f.write(buf, r);
                }
            } while (r > 0);
            unzCloseCurrentFile(uf);
        }
    } while (unzGoToNextFile(uf) == UNZ_OK);

    unzClose(uf);

    return 0;
}

