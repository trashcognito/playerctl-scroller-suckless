#include "config.hpp"
#include <cstring>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cwchar>

#include "cscroll.hpp"

char* full;
int offset = 0;
int maxLength = 1000;

int len = LENGTH;

void rotateText(int status) {

    // 0 paused + long
    // print from offset
    // printableChars - shortenLength

    // 1 playing + dont rotate
    // 1 paused + short
    // just print string
    // printableChars - wideCharOffset

    // 2 playing + rotate
    // print from offset
    // increment offset
    // printableChar - shortenLength

    // skip wide character parts 
    // (bytes after first one)
    // and go to next actual character
    int wideCharOffset = 0;

    // increase offset by first
    // character width for next
    // rotation to start at the
    // actual next character
    int firstCharWidth = 1;

    // count any wide characters
    // as double wide to prevent
    // a wide text output
    int shortenLength = 0;

    // Don't print "len" characters
    // if "full" is shorter than
    // the value of "len"
    int printableChars = (strlen(full) < len ) ? strlen(full) : len;
    
    for (int i = 0; i < printableChars - (status == 1 ? wideCharOffset : shortenLength); i++){
        char* ptr = full+((offset+i+wideCharOffset)%strlen(full));
        char c = *ptr;

        if (c >= -62 && c <= -33){
            // 2 byte wide character
            wchar_t wideChar[2];
            char* unicodeString = (char *)malloc((2+1) * sizeof(char));
            unicodeString[0] = c;
            unicodeString[1] = *(ptr+1);
            unicodeString[2] = L'\0';

            mbstowcs(wideChar, unicodeString, 2);
            printf("%ls", wideChar);
            
            wideCharOffset += 1;
            if (i == 0)
                firstCharWidth = 2;
            
            int width = wcwidth(*wideChar);
            if (width > 1)
                shortenLength += width - 1;
            free(unicodeString);
        }
        else if (c >= -32 && c <= -17){
            // 3 byte wide character
            wchar_t wideChar[2];
            char* unicodeString = (char *)malloc((3+1) * sizeof(char));
            unicodeString[0] = c;
            unicodeString[1] = *(ptr+1);
            unicodeString[2] = *(ptr+2);
            unicodeString[3] = L'\0';

            mbstowcs(wideChar, unicodeString, 2);
            printf("%ls", wideChar);
            
            wideCharOffset += 2;
            if (i == 0)
                firstCharWidth = 3;
            
            int width = wcwidth(*wideChar);
            if (width > 1)
                shortenLength += width - 1;
            free(unicodeString);
        }
        else if (c >= -16 && c <= -12){
            // 4 byte wide character
            wchar_t wideChar[2];
            char* unicodeString = (char *)malloc((4+1) * sizeof(char));
            unicodeString[0] = c;
            unicodeString[1] = *(ptr+1);
            unicodeString[2] = *(ptr+2);
            unicodeString[3] = *(ptr+3);
            unicodeString[4] = L'\0';

            mbstowcs(wideChar, unicodeString, 2);
            printf("%ls", wideChar);
            
            wideCharOffset += 3;
            if (i == 0)
                firstCharWidth = 4;
            
            int width = wcwidth(*wideChar);
            if (width > 1)
                shortenLength += width - 1;
            free(unicodeString);
        }
        else // default (1 byte wide)
            printf("%c", c);
    }
    if (status == 2)
        offset += (firstCharWidth - 1);
}