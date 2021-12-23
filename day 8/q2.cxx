// solution grabbed from u/m3n0tu on aoc reddit

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

using namespace std;

void sortLetters(string &entryString);
bool checkLetters(string entryString, char letter);

int main()
{
    int place = 0, endPlace = 0, letterCount = 0, total = 0;
    string cryptoNumber[10], number[10], cryptoDigit[4];
    char segment[7];
    string dataEntry;

    ifstream inputFile;
    inputFile.open("input.txt");

    while (getline(inputFile, dataEntry)) 
    {
        place = 0;
        for (int i = 0; i < 10; i++)
        {
            endPlace = dataEntry.find(' ', place);
            cryptoNumber[i] = dataEntry.substr(place, endPlace - place);
            place = endPlace + 1;
            sortLetters(cryptoNumber[i]);
            if (cryptoNumber[i].length() == 2)
            {
                number[1] = cryptoNumber[i];
            }

            if (cryptoNumber[i].length() == 3)
            {
                number[7] = cryptoNumber[i];
            }

            if (cryptoNumber[i].length() == 4) {
                number[4] = cryptoNumber[i];
            }

            if (cryptoNumber[i].length() == 7) {
                number[8] = cryptoNumber[i];
            }
        }

        for (int i = 'a'; i <= 'g'; i++) {
            letterCount = 0;
            for (int j = 0; j < dataEntry.find('|'); j++)
            {
                if (dataEntry[j] == i) {
                    letterCount++;
                }
            }
            if (letterCount == 9) {
                segment[2] = i;
            }
            if (letterCount == 4) {
                segment[4] = i;
            }
            if (letterCount == 6) {
                segment[5] = i;
            }

            if (letterCount == 7) {
                if (checkLetters(number[4], i)) {

                    segment[6] = i;
                }
                else {

                    segment[3] = i;
                }
            }

            if (letterCount == 8) {
                if (checkLetters(number[4], i)) {
                    segment[1] = i;
                }
                else {
                    segment[0] = i;
                }
            }


        }

        for (int i = 0; i < 10; i++) {
            if (cryptoNumber[i].length() == 6) {
                if (!checkLetters(cryptoNumber[i], segment[1])) {
                    number[6] = cryptoNumber[i];
                }
                else if (!checkLetters(cryptoNumber[i], segment[6])) {
                    number[0] = cryptoNumber[i];
                }
                else {
                    number[9] = cryptoNumber[i];
                }
            }

            if (cryptoNumber[i].length() == 5) {
                if (!checkLetters(cryptoNumber[i], segment[1])) {
                    number[5] = cryptoNumber[i];
                }
                else if (!checkLetters(cryptoNumber[i], segment[2])) {
                    number[2] = cryptoNumber[i];
                }
                else {
                    number[3] = cryptoNumber[i];
                }
            }
        }

        place = dataEntry.find('|', 0) + 2;
        for (int i = 3; i >= 0; i--)
        {

            endPlace = dataEntry.find(' ', place);
            cryptoDigit[i] = dataEntry.substr(place, endPlace - place);
            place = endPlace + 1;
            sortLetters(cryptoDigit[i]);

        }

        for (int i = 0; i < 4; i++) {

            for (int j = 0; j < 10; j++) {
                if (cryptoDigit[i] == number[j]) {
                    total += pow(10.0, i) * j;
                }
            }
           
        }
    }
            
    inputFile.close();
    cout<<total<<endl;
    return 0;
}

void sortLetters(string &entryString) {
    
        char key, j;
        for (int i = 0; i < entryString.length(); i++) {
            key = entryString[i];
            j = i;
            while (j > 0 && entryString[j - 1] > key) {
                entryString[j] = entryString[j - 1];
                j--;
            }
            entryString[j] = key;  
    }
        
}

bool checkLetters(string entryString, char letter) {
    for (int i = 0; i < entryString.length(); i++)
    {
        if (entryString[i] == letter) {
            return true;
        }
    }
    return false;
}