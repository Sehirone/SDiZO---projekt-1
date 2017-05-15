// menu.cpp : Defines the entry point for the console application.
//


#include<conio.h>
#include<string>
#include<iostream>
#include "Array.h"
#include "List.h"
#include "Heap.h"   // wersja uzyta przy sprawdzaniu poprawnosci implementacji operacji
#include "HeapNoRealloc.h" // wersja uzyta przy przeprowadzaniu testow zlozonosci obliczeniowej
#include <chrono>
#include <random>
using namespace std;

// do testow, maxMem/keySize w bajtach
long long maxMem = 1000000000;
int keySize = sizeof(int);
int maxD = maxMem/keySize;
random_device rd;  // seed
mt19937 gen(rd()); // rng engine
void generateDataFile(int ile, int from, int to, string filename);


void displayMenu(string info)
{
	cout << endl;
	cout << info << endl;
	cout << "1.Wczytaj z pliku" << endl;
	cout << "2.Usun" << endl;
	cout << "3.Dodaj" << endl;
	cout << "4.Znajdz" << endl;
	cout << "5.Utworz losowo" << endl;
	cout << "6.Wyswietl" << endl;
	cout << "7.Test (pomiary)" << endl;
	cout << "0.Powrot do menu" << endl;
	cout << "Podaj opcje:";
}

void menu_table()
{
	char opt;
	int key, pos;
	int result;
	Array<int> testArr;
	do{
		displayMenu("--- TABLICA ---");
		opt = getche();
		cout << endl << endl;
		switch (opt){
		case '1': {//tutaj wczytytwanie pliku z tablicy
		    cout << "Podaj nazwe pliku: ";
		    string filename;
		    cin >> filename;
		    if(testArr.fillFromFile(filename) == -1) {
                cout << "Wczytywanie nie powiodlo sie!" << endl;
		    } else {
                cout << "Wczytano dane pomyslnie!" << endl;
                cout << testArr.printStruct() << endl;
		    }
			break;
		}
		case '2': //tutaj usuwanie elemenu z tablicy
		    cout << "Pozycja[-1-puste]: ";
		    cin >> pos;
		    cout << "Klucz[0-puste]: ";
		    cin >> key;
		    if(pos == -1) {
                result = testArr.deleteElementByKey(key);
		    }else if(key == 0) {
                result = testArr.deleteElementByPos(pos);
		    } else {
                result = testArr.deleteElementByPos(pos, key);
		    }
		    if(result == -1) {
                cout << "Usuwanie nie powiodlo sie!" << endl;
		    } else {
                cout << "Usunieto element!" << endl;
                cout << testArr.printStruct() << endl;
		    }
			break;
		case '3': //tutaj dodawanie elemetu do tablicy
		    cout << "Element: ";
		    cin >> key;
		    cout << "Pozycja: ";
		    cin >> pos;
		    testArr.addElement(pos, key);
		    cout << testArr.printStruct() << endl;
			break;
		case '4': //tutaj znajdowanie elemetu w tablicy
		    cout << "Szukany klucz: ";
		    cin >> key;
		    pos = testArr.findElement(key);
		    if(pos >= 0) {
                cout << "Znaleziono " << key << " na pozycji " << pos << "!" << endl;
		    } else {
                cout << "Nie znaleziono " << key << "!" << endl;
		    }
			break;
		case '5':  //tutaj generowanie  tablicy
		    cout << "Rozmiar: ";
		    cin >> pos;
		    testArr.fillRandom(pos);
		    cout << testArr.printStruct() << endl;
			break;
		case '6':  //tutaj wyœwietlanie tablicy
		    cout << testArr.printStruct() << endl;
			break;
		case '7': //tutaj nasza funkcja do eksperymentów (pomiary czasów i generowanie daneych) - nie bêdzie testowana przez prowadz¹cego
		    {
                const int dataRanges[8] = {10, 100, INT_MAX/2 - 100000000, INT_MAX/2 + 100000000, INT_MAX - 200000000, INT_MAX, 1, INT_MAX};
                const int dataRangesCount = 4;
                const int minElements = 16;
                const int minTestPrecision = 1;
                ofstream testOutput("Array.txt");
                if(!testOutput) {
                    cout << "BLAD zapisu testow!";
                }
                testOutput << "CZASY  OPERACJI W NANOSEKUNDACH\n";
                cout << "Wstawianie na koniec...";
                testOutput << "Wstawianie na koniec:\n";
                {
                    testOutput << "wielkosc struktury\t[1,100]\t[~INT_MAX/2]\t[~INT_MAX]\t[1,INT_MAX]\n";
                    for(int i = minElements ; i <= maxD; i *= 2) {
                        testOutput << i << "\t";
                        int testPrecision = 2500000/i;
                        if(testPrecision < minTestPrecision) {
                            testPrecision = minTestPrecision;
                        }
                        for(int k = 0 ; k < dataRangesCount ; k++) {
                            uniform_int_distribution<> dis(dataRanges[k*2], dataRanges[k*2+1]);
                            testArr.fillRandom(i, dataRanges[k*2], dataRanges[k*2+1]);
                            auto fillStartTime = chrono::high_resolution_clock::now();
                            for(int j = 0 ; j < testPrecision ; j++) {
                                testArr.fillRandom(i, dataRanges[k*2], dataRanges[k*2+1]);
                            }
                            auto fillEndTime = chrono::high_resolution_clock::now();
                            auto fillTime = chrono::duration_cast<chrono::nanoseconds>(fillEndTime - fillStartTime).count();
                            auto startTime = chrono::high_resolution_clock::now();
                            for(int j = 0 ; j < testPrecision ; j++) {
                                testArr.fillRandom(i, dataRanges[k*2], dataRanges[k*2+1]);
                                testArr.addElement(i, dis(gen));
                            }
                            auto endTime = chrono::high_resolution_clock::now();
                            auto sumaCzasow = (chrono::duration_cast<chrono::nanoseconds>(endTime - startTime).count() - fillTime)/testPrecision;
                            testOutput << "\t" << sumaCzasow;
                        }
                        testOutput << endl;
                    }
                }
                cout << "DONE!\nWstawianie na środek...";
                testOutput << "Wstawianie na środek:\n";
                {
                    testOutput << "wielkosc struktury\t[1,100]\t[~INT_MAX/2]\t[~INT_MAX]\t[1,INT_MAX]\n";
                    for(int i = minElements ; i <= maxD; i *= 2) {
                        testOutput << i << "\t";
                        int testPrecision = 2500000/i;
                        if(testPrecision < minTestPrecision) {
                            testPrecision = minTestPrecision;
                        }
                        for(int k = 0 ; k < dataRangesCount ; k++) {
                            uniform_int_distribution<> dis(dataRanges[k*2], dataRanges[k*2+1]);
                            testArr.fillRandom(i, dataRanges[k*2], dataRanges[k*2+1]);
                            auto fillStartTime = chrono::high_resolution_clock::now();
                            for(int j = 0 ; j < testPrecision ; j++) {
                                testArr.fillRandom(i, dataRanges[k*2], dataRanges[k*2+1]);
                            }
                            auto fillEndTime = chrono::high_resolution_clock::now();
                            auto fillTime = chrono::duration_cast<chrono::nanoseconds>(fillEndTime - fillStartTime).count();
                            auto startTime = chrono::high_resolution_clock::now();
                            for(int j = 0 ; j < testPrecision ; j++) {
                                testArr.fillRandom(i, dataRanges[k*2], dataRanges[k*2+1]);
                                testArr.addElement(i/2, dis(gen));
                            }
                            auto endTime = chrono::high_resolution_clock::now();
                            auto sumaCzasow = (chrono::duration_cast<chrono::nanoseconds>(endTime - startTime).count() - fillTime)/testPrecision;
                            testOutput << "\t" << sumaCzasow;
                        }
                        testOutput << endl;
                    }
                }
                cout << "DONE!\nWstawianie na początek...";
                testOutput << "Wstawianie na początek:\n";
                {
                    testOutput << "wielkosc struktury\t[1,100]\t[~INT_MAX/2]\t[~INT_MAX]\t[1,INT_MAX]\n";
                    for(int i = minElements ; i <= maxD; i *= 2) {
                        testOutput << i << "\t";
                        int testPrecision = 2500000/i;
                        if(testPrecision < minTestPrecision) {
                            testPrecision = minTestPrecision;
                        }
                        for(int k = 0 ; k < dataRangesCount ; k++) {
                            uniform_int_distribution<> dis(dataRanges[k*2], dataRanges[k*2+1]);
                            testArr.fillRandom(i, dataRanges[k*2], dataRanges[k*2+1]);
                            auto fillStartTime = chrono::high_resolution_clock::now();
                            for(int j = 0 ; j < testPrecision ; j++) {
                                testArr.fillRandom(i, dataRanges[k*2], dataRanges[k*2+1]);
                            }
                            auto fillEndTime = chrono::high_resolution_clock::now();
                            auto fillTime = chrono::duration_cast<chrono::nanoseconds>(fillEndTime - fillStartTime).count();
                            auto startTime = chrono::high_resolution_clock::now();
                            for(int j = 0 ; j < testPrecision ; j++) {
                                testArr.fillRandom(i, dataRanges[k*2], dataRanges[k*2+1]);
                                testArr.addElement(0, dis(gen));
                            }
                            auto endTime = chrono::high_resolution_clock::now();
                            auto sumaCzasow = (chrono::duration_cast<chrono::nanoseconds>(endTime - startTime).count() - fillTime)/testPrecision;
                            testOutput << "\t" << sumaCzasow;
                        }
                        testOutput << endl;
                    }
                }
                cout << "DONE!\nWyszukiwanie - klucz poza przedziałem...";
                testOutput << "Wyszukiwanie - klucz poza przedziałem:\n";
                {
                    testOutput << "wielkosc struktury\t[1,100]\t[~INT_MAX/2]\t[~INT_MAX]\t[1,INT_MAX]\n";
                    for(int i = minElements ; i <= maxD; i *= 2) {
                        testOutput << i << "\t";
                        int testPrecision = 2500000/i;
                        if(testPrecision < minTestPrecision) {
                            testPrecision = minTestPrecision;
                        }
                        for(int k = 0 ; k < dataRangesCount ; k++) {
                            uniform_int_distribution<> dis(dataRanges[k*2], dataRanges[k*2+1]);
                            testArr.fillRandom(i, dataRanges[k*2], dataRanges[k*2+1]);
                            auto fillStartTime = chrono::high_resolution_clock::now();
                            for(int j = 0 ; j < testPrecision ; j++) {
                                testArr.fillRandom(i, dataRanges[k*2], dataRanges[k*2+1]);
                            }
                            auto fillEndTime = chrono::high_resolution_clock::now();
                            auto fillTime = chrono::duration_cast<chrono::nanoseconds>(fillEndTime - fillStartTime).count();
                            auto startTime = chrono::high_resolution_clock::now();
                            for(int j = 0 ; j < testPrecision ; j++) {
                                testArr.fillRandom(i, dataRanges[k*2], dataRanges[k*2+1]);
                                testArr.findElement(-1);
                            }
                            auto endTime = chrono::high_resolution_clock::now();
                            auto sumaCzasow = (chrono::duration_cast<chrono::nanoseconds>(endTime - startTime).count() - fillTime)/testPrecision;
                            testOutput << "\t" << sumaCzasow;
                        }
                        testOutput << endl;
                    }
                }
                cout << "DONE!\nUsuwanie z końca...";
                testOutput << "Usuwanie z końca:\n";
                {
                    testOutput << "wielkosc struktury\t[1,100]\t[~INT_MAX/2]\t[~INT_MAX]\t[1,INT_MAX]\n";
                    for(int i = minElements ; i <= maxD; i *= 2) {
                        testOutput << i << "\t";
                        int testPrecision = 2500000/i;
                        if(testPrecision < minTestPrecision) {
                            testPrecision = minTestPrecision;
                        }
                        for(int k = 0 ; k < dataRangesCount ; k++) {
                            uniform_int_distribution<> dis(dataRanges[k*2], dataRanges[k*2+1]);
                            testArr.fillRandom(i, dataRanges[k*2], dataRanges[k*2+1]);
                            auto fillStartTime = chrono::high_resolution_clock::now();
                            for(int j = 0 ; j < testPrecision ; j++) {
                                testArr.fillRandom(i, dataRanges[k*2], dataRanges[k*2+1]);
                            }
                            auto fillEndTime = chrono::high_resolution_clock::now();
                            auto fillTime = chrono::duration_cast<chrono::nanoseconds>(fillEndTime - fillStartTime).count();
                            auto startTime = chrono::high_resolution_clock::now();
                            for(int j = 0 ; j < testPrecision ; j++) {
                                testArr.fillRandom(i, dataRanges[k*2], dataRanges[k*2+1]);
                                testArr.deleteElementByPos(i-1);
                            }
                            auto endTime = chrono::high_resolution_clock::now();
                            auto sumaCzasow = (chrono::duration_cast<chrono::nanoseconds>(endTime - startTime).count() - fillTime)/testPrecision;
                            testOutput << "\t" << sumaCzasow;
                        }
                        testOutput << endl;
                    }
                }
                cout << "DONE!\nUsuwanie z środka...";
                testOutput << "Usuwanie z środka:\n";
                {
                    testOutput << "wielkosc struktury\t[1,100]\t[~INT_MAX/2]\t[~INT_MAX]\t[1,INT_MAX]\n";
                    for(int i = minElements ; i <= maxD; i *= 2) {
                        testOutput << i << "\t";
                        int testPrecision = 2500000/i;
                        if(testPrecision < minTestPrecision) {
                            testPrecision = minTestPrecision;
                        }
                        for(int k = 0 ; k < dataRangesCount ; k++) {
                            uniform_int_distribution<> dis(dataRanges[k*2], dataRanges[k*2+1]);
                            testArr.fillRandom(i, dataRanges[k*2], dataRanges[k*2+1]);
                            auto fillStartTime = chrono::high_resolution_clock::now();
                            for(int j = 0 ; j < testPrecision ; j++) {
                                testArr.fillRandom(i, dataRanges[k*2], dataRanges[k*2+1]);
                            }
                            auto fillEndTime = chrono::high_resolution_clock::now();
                            auto fillTime = chrono::duration_cast<chrono::nanoseconds>(fillEndTime - fillStartTime).count();
                            auto startTime = chrono::high_resolution_clock::now();
                            for(int j = 0 ; j < testPrecision ; j++) {
                                testArr.fillRandom(i, dataRanges[k*2], dataRanges[k*2+1]);
                                testArr.deleteElementByPos(i/2);
                            }
                            auto endTime = chrono::high_resolution_clock::now();
                            auto sumaCzasow = (chrono::duration_cast<chrono::nanoseconds>(endTime - startTime).count() - fillTime)/testPrecision;
                            testOutput << "\t" << sumaCzasow;
                        }
                        testOutput << endl;
                    }
                }
                cout << "DONE!\nUsuwanie z początku...";
                testOutput << "Usuwanie z początku:\n";
                {
                    testOutput << "wielkosc struktury\t[1,100]\t[~INT_MAX/2]\t[~INT_MAX]\t[1,INT_MAX]\n";
                    for(int i = minElements ; i <= maxD; i *= 2) {
                        testOutput << i << "\t";
                        int testPrecision = 2500000/i;
                        if(testPrecision < minTestPrecision) {
                            testPrecision = minTestPrecision;
                        }
                        for(int k = 0 ; k < dataRangesCount ; k++) {
                            uniform_int_distribution<> dis(dataRanges[k*2], dataRanges[k*2+1]);
                            testArr.fillRandom(i, dataRanges[k*2], dataRanges[k*2+1]);
                            auto fillStartTime = chrono::high_resolution_clock::now();
                            for(int j = 0 ; j < testPrecision ; j++) {
                                testArr.fillRandom(i, dataRanges[k*2], dataRanges[k*2+1]);
                            }
                            auto fillEndTime = chrono::high_resolution_clock::now();
                            auto fillTime = chrono::duration_cast<chrono::nanoseconds>(fillEndTime - fillStartTime).count();
                            auto startTime = chrono::high_resolution_clock::now();
                            for(int j = 0 ; j < testPrecision ; j++) {
                                testArr.fillRandom(i, dataRanges[k*2], dataRanges[k*2+1]);
                                testArr.deleteElementByPos(0);
                            }
                            auto endTime = chrono::high_resolution_clock::now();
                            auto sumaCzasow = (chrono::duration_cast<chrono::nanoseconds>(endTime - startTime).count() - fillTime)/testPrecision;
                            testOutput << "\t" << sumaCzasow;
                        }
                        testOutput << endl;
                    }
                }
                cout << "DONE!\n";
                testOutput.close();
                break;
		    }
		}
	} while (opt != '0');
}

void menu_list()
{
    //analogicznie jak menu_table()
    char opt;
	int key, pos;
	int result;
	List<int> testList;
	do{
		displayMenu("--- LISTA ---");
		opt = getche();
		cout << endl << endl;
		switch (opt){
		case '1': {//tutaj wczytytwanie pliku
		    cout << "Podaj nazwe pliku: ";
		    string filename;
		    cin >> filename;
		    if(testList.fillFromFile(filename) == -1) {
                cout << "Wczytywanie nie powiodlo sie!" << endl;
		    } else {
                cout << "Wczytano dane pomyslnie!" << endl;
                cout << testList.printStruct() << endl;
		    }
            }
            break;
		case '2': //tutaj usuwanie elemenu
            cout << "Pozycja[-1-puste]: ";
		    cin >> pos;
		    cout << "Klucz[0-puste]: ";
		    cin >> key;
		    if(pos == -1) {
                result = testList.deleteElementByKey(key);
		    }else if(key == 0) {
                result = testList.deleteElementByPos(pos);
		    } else {
                result = testList.deleteElementByPos(pos, key);
		    }
		    if(result == -1) {
                cout << "Usuwanie nie powiodlo sie!" << endl;
		    } else {
                cout << "Usunieto element!" << endl;
                cout << testList.printStruct() << endl;
		    }
			break;
		case '3': //tutaj dodawanie elemetu
            cout << "Element: ";
		    cin >> key;
		    cout << "Pozycja: ";
		    cin >> pos;
		    testList.addElement(pos, key);
		    cout << testList.printStruct() << endl;
			break;
		case '4': //tutaj znajdowanie elemetu
		    cout << "Szukany klucz: ";
		    cin >> key;
		    pos = testList.findElement(key);
		    if(pos >= 0) {
                cout << "Znaleziono " << key << " na pozycji " << pos << "!" << endl;
		    } else {
                cout << "Nie znaleziono " << key << "!" << endl;
		    }
		    break;
		case '5':  //tutaj generowanie
		    cout << "Rozmiar: ";
		    cin >> pos;
		    testList.fillRandom(pos);
		    cout << testList.printStruct() << endl;
			break;
		case '6':  //tutaj wyœwietlanie
		    cout << testList.printStruct() << endl;
			break;
		case '7': //tutaj nasza funkcja do eksperymentów (pomiary czasów i generowanie daneych) - nie bêdzie testowana przez prowadz¹cego
		    {
                const int dataRanges[8] = {10, 100, INT_MAX/2 - 100000000, INT_MAX/2 + 100000000, INT_MAX - 200000000, INT_MAX, 1, INT_MAX};
                const int dataRangesCount = 4;
                const int minElements = 16;
                const int minTestPrecision = 2;
                const int baseTestPrecision = 3000000;
                ofstream testOutput("List.txt");
                if(!testOutput) {
                    cout << "BLAD zapisu testow!";
                }
                testOutput << "CZASY  OPERACJI W NANOSEKUNDACH\n";
                cout << "Wstawianie na koniec...";
                testOutput << "Wstawianie na koniec:\n";
                {
                    testOutput << "wielkosc struktury\t[10,100]\t[~INT_MAX/2]\t[~INT_MAX]\t[1,INT_MAX]\n";
                    for(int i = minElements ; i <= maxD; i *= 2) {
                        testOutput << i << "\t";
                        int testPrecision = baseTestPrecision/i;
                        if(testPrecision < minTestPrecision) {
                            testPrecision = minTestPrecision;
                        }
                        for(int k = 0 ; k < dataRangesCount ; k++) {
                                uniform_int_distribution<> dis(dataRanges[k*2], dataRanges[k*2+1]);
                                testList.fillRandom(i, dataRanges[k*2], dataRanges[k*2+1]);
                                auto startTime = chrono::high_resolution_clock::now();
                                for(int j = 0 ; j < testPrecision ; j++) {
                                    testList.addElement(i, dis(gen));
                                    testList.deleteElementByPos(0);
                                }
                                auto endTime = chrono::high_resolution_clock::now();
                                auto sumaCzasow = ((chrono::duration_cast<chrono::nanoseconds>(endTime - startTime).count())/testPrecision)-60;
                                testOutput << "\t" << sumaCzasow;
                        }

                        testOutput << endl;
                    }
                }
                cout << "DONE!\nWstawianie na środek...";
                testOutput << "Wstawianie na środek:\n";
                {
                    testOutput << "wielkosc struktury\t[10,100]\t[~INT_MAX/2]\t[~INT_MAX]\t[1,INT_MAX]\n";
                    for(int i = minElements ; i <= maxD; i *= 2) {
                        testOutput << i << "\t";
                        int testPrecision = baseTestPrecision/i;
                        if(testPrecision < minTestPrecision) {
                            testPrecision = minTestPrecision;
                        }
                        for(int k = 0 ; k < dataRangesCount ; k++) {
                                uniform_int_distribution<> dis(dataRanges[k*2], dataRanges[k*2+1]);
                                testList.fillRandom(i, dataRanges[k*2], dataRanges[k*2+1]);
                                auto startTime = chrono::high_resolution_clock::now();
                                for(int j = 0 ; j < testPrecision ; j++) {
                                    testList.addElement(i/2, dis(gen));
                                    testList.deleteElementByPos(0);
                                }
                                auto endTime = chrono::high_resolution_clock::now();
                                auto sumaCzasow = ((chrono::duration_cast<chrono::nanoseconds>(endTime - startTime).count())/testPrecision)-60;
                                testOutput << "\t" << sumaCzasow;
                        }

                        testOutput << endl;
                    }
                }
                cout << "DONE!\nWstawianie na początek...";
                testOutput << "Wstawianie na początek:\n";
                {
                    testOutput << "wielkosc struktury\t[10,100]\t[~INT_MAX/2]\t[~INT_MAX]\t[1,INT_MAX]\n";
                    for(int i = minElements ; i <= maxD; i *= 2) {
                        testOutput << i << "\t";
                        int testPrecision = baseTestPrecision;
                        if(testPrecision < minTestPrecision) {
                            testPrecision = minTestPrecision;
                        }
                        for(int k = 0 ; k < dataRangesCount ; k++) {
                            uniform_int_distribution<> dis(dataRanges[k*2], dataRanges[k*2+1]);
                            testList.fillRandom(i, dataRanges[k*2], dataRanges[k*2+1]);
                            auto startTime = chrono::high_resolution_clock::now();
                            for(int j = 0 ; j < testPrecision ; j++) {
                                testList.addElement(0, dis(gen));
                            }
                            auto endTime = chrono::high_resolution_clock::now();
                            auto sumaCzasow = (chrono::duration_cast<chrono::nanoseconds>(endTime - startTime).count())/testPrecision;
                            testOutput << "\t" << sumaCzasow;
                        }
                        testOutput << endl;
                    }
                }
                cout << "DONE!\nWyszukiwanie - klucz poza przedziałem...";
                testOutput << "Wyszukiwanie - klucz poza przedziałem:\n";
                {
                    testOutput << "wielkosc struktury\t[1,100]\t[~INT_MAX/2]\t[~INT_MAX]\t[1,INT_MAX]\n";
                    for(int i = minElements ; i <= maxD; i *= 2) {
                        testOutput << i << "\t";
                        int testPrecision = baseTestPrecision/i;
                        if(testPrecision < minTestPrecision) {
                            testPrecision = minTestPrecision;
                        }
                        for(int k = 0 ; k < dataRangesCount ; k++) {
                            uniform_int_distribution<> dis(dataRanges[k*2], dataRanges[k*2+1]);
                            testList.fillRandom(i, dataRanges[k*2], dataRanges[k*2+1]);
                            auto startTime = chrono::high_resolution_clock::now();
                            for(int j = 0 ; j < testPrecision ; j++) {
                                testList.findElement(-1);
                            }
                            auto endTime = chrono::high_resolution_clock::now();
                            auto sumaCzasow = (chrono::duration_cast<chrono::nanoseconds>(endTime - startTime).count())/testPrecision;
                            testOutput << "\t" << sumaCzasow;
                        }
                        testOutput << endl;
                    }
                }
                cout << "DONE!\nUsuwanie z końca...";
                testOutput << "Usuwanie z końca:\n";
                {
                    testOutput << "wielkosc struktury\t[1,100]\t[~INT_MAX/2]\t[~INT_MAX]\t[1,INT_MAX]\n";
                    for(int i = minElements ; i <= maxD; i *= 2) {
                        testOutput << i << "\t";
                        int testPrecision = baseTestPrecision/i;
                        if(testPrecision < minTestPrecision) {
                            testPrecision = minTestPrecision;
                        }
                        for(int k = 0 ; k < dataRangesCount ; k++) {
                            uniform_int_distribution<> dis(dataRanges[k*2], dataRanges[k*2+1]);
                            testList.fillRandom(i, dataRanges[k*2], dataRanges[k*2+1]);
                            auto startTime = chrono::high_resolution_clock::now();
                            for(int j = 0 ; j < testPrecision ; j++) {
                                testList.deleteElementByPos(i-1);
                                testList.addElement(0,dis(gen));
                            }
                            auto endTime = chrono::high_resolution_clock::now();
                            auto sumaCzasow = ((chrono::duration_cast<chrono::nanoseconds>(endTime - startTime).count())/testPrecision)-140;
                            testOutput << "\t" << sumaCzasow;
                        }
                        testOutput << endl;
                    }
                }
                cout << "DONE!\nUsuwanie z środka...";
                testOutput << "Usuwanie z środka:\n";
                {
                    testOutput << "wielkosc struktury\t[1,100]\t[~INT_MAX/2]\t[~INT_MAX]\t[1,INT_MAX]\n";
                    for(int i = minElements ; i <= maxD; i *= 2) {
                        testOutput << i << "\t";
                        int testPrecision = baseTestPrecision/i;
                        if(testPrecision < minTestPrecision) {
                            testPrecision = minTestPrecision;
                        }
                        for(int k = 0 ; k < dataRangesCount ; k++) {
                            uniform_int_distribution<> dis(dataRanges[k*2], dataRanges[k*2+1]);
                            testList.fillRandom(i, dataRanges[k*2], dataRanges[k*2+1]);
                            auto startTime = chrono::high_resolution_clock::now();
                            for(int j = 0 ; j < testPrecision ; j++) {
                                testList.deleteElementByPos(i/2);
                                testList.addElement(0,dis(gen));
                            }
                            auto endTime = chrono::high_resolution_clock::now();
                            auto sumaCzasow = ((chrono::duration_cast<chrono::nanoseconds>(endTime - startTime).count())/testPrecision)-140;
                            testOutput << "\t" << sumaCzasow;
                        }
                        testOutput << endl;
                    }
                }
                cout << "DONE!\nUsuwanie z początku...";
                testOutput << "Usuwanie z początku:\n";
                {
                    testOutput << "wielkosc struktury\t[1,100]\t[~INT_MAX/2]\t[~INT_MAX]\t[1,INT_MAX]\n";
                    for(int i = minElements ; i <= maxD; i *= 2) {
                        testOutput << i << "\t";
                        int testPrecision = baseTestPrecision;
                        if(testPrecision < minTestPrecision) {
                            testPrecision = minTestPrecision;
                        }
                        for(int k = 0 ; k < dataRangesCount ; k++) {
                            uniform_int_distribution<> dis(dataRanges[k*2], dataRanges[k*2+1]);
                            testList.fillRandom(i, dataRanges[k*2], dataRanges[k*2+1]);
                            auto startTime = chrono::high_resolution_clock::now();
                            for(int j = 0 ; j < testPrecision ; j++) {
                                testList.deleteElementByPos(0);
                                testList.addElement(0,dis(gen));
                            }
                            auto endTime = chrono::high_resolution_clock::now();
                            auto sumaCzasow = ((chrono::duration_cast<chrono::nanoseconds>(endTime - startTime).count())/testPrecision)-140;
                            testOutput << "\t" << sumaCzasow;
                        }
                        testOutput << endl;
                    }
                }
                cout << "DONE!\n";
                testOutput.close();
                break;
		    }
		}

	} while (opt != '0');
}

void menu_heap()
{
	//analogicznie jak menu_table()
	char opt;
	int key, pos;
	int result;
	Heap<int> testHeap;
	do{
		displayMenu("--- KOPIEC ---");
		opt = getche();
		cout << endl << endl;
		switch (opt){
		case '1': {//tutaj wczytytwanie pliku
		    cout << "Podaj nazwe pliku: ";
		    string filename;
		    cin >> filename;
		    if(testHeap.fillFromFile(filename) == -1) {
                cout << "Wczytywanie nie powiodlo sie!" << endl;
		    } else {
                cout << "Wczytano dane pomyslnie!" << endl;
                testHeap.printStruct();
		    }
            }
            break;
		case '2': //tutaj usuwanie elemenu
            cout << "Pozycja[-1-puste]: ";
		    cin >> pos;
		    cout << "Klucz[0-puste]: ";
		    cin >> key;
		    if(pos == -1) {
                result = testHeap.deleteElementByKey(key);
		    }else if(key == 0) {
                result = testHeap.deleteElementByPos(pos);
		    } else {
                result = testHeap.deleteElementByPos(pos, key);
		    }
		    if(result == -1) {
                cout << "Usuwanie nie powiodlo sie!" << endl;
		    } else {
                cout << "Usunieto element!" << endl;
                testHeap.printStruct();
		    }
			break;
		case '3': //tutaj dodawanie elemetu
            cout << "Element: ";
		    cin >> key;
		    cout << "Pozycja: ";
		    cin >> pos;
            testHeap.addElement(key, pos);
		    testHeap.printStruct();
			break;
		case '4': //tutaj znajdowanie elemetu
		    cout << "Szukany klucz: ";
		    cin >> key;
		    pos = testHeap.findElement(key);
		    if(pos >= 0) {
                cout << "Znaleziono " << key << " na pozycji " << pos << "!" << endl;
		    } else {
                cout << "Nie znaleziono " << key << "!" << endl;
		    }
		    break;
		case '5':  //tutaj generowanie
		    cout << "Rozmiar: ";
		    cin >> pos;
		    testHeap.fillRandom(pos);
		    testHeap.printStruct();
			break;
		case '6':  //tutaj wyœwietlanie
		    testHeap.printStruct();
			break;
		case '7': //tutaj nasza funkcja do eksperymentów (pomiary czasów i generowanie daneych) - nie bêdzie testowana przez prowadz¹cego
		    {
		        HeapNoRealloc<int> testHeap;
                const int dataRanges[8] = {10, 100, INT_MAX/2 - 100000000, INT_MAX/2 + 100000000, INT_MAX - 200000000, INT_MAX, 1, INT_MAX};
                const int dataRangesCount = 4;
                const int minElements = 16;
                const int minTestPrecision = 10;
                const int baseTestPrecision = 2500000;
                ofstream testOutput("Heap.txt");
                if(!testOutput) {
                    cout << "BLAD zapisu testow!";
                }
                testOutput << "CZASY  OPERACJI W NANOSEKUNDACH - BEZ REALOKACJI\n";
                cout << "Wstawianie...";
                testOutput << "Wstawianie:\n";
                {
                    testOutput << "wielkosc struktury\t[10,100]\t[~INT_MAX/2]\t[~INT_MAX]\t[1,INT_MAX]\n";
                    for(int i = minElements ; i <= maxD; i *= 2) {
                        testOutput << i << "\t";
                        int testPrecision = baseTestPrecision;
                        if(testPrecision < minTestPrecision) {
                            testPrecision = minTestPrecision;
                        }
                        for(int k = 0 ; k < dataRangesCount ; k++) {
                            uniform_int_distribution<> dis(dataRanges[k*2], dataRanges[k*2+1]);
                            testHeap.fillRandom(i, dataRanges[k*2], dataRanges[k*2+1]);
                            auto startTime = chrono::high_resolution_clock::now();
                            for(int j = 0 ; j < testPrecision ; j++) {
                                testHeap.addElement(dis(gen), i);
                                testHeap.deleteElementByPos(i);
                            }
                            auto endTime = chrono::high_resolution_clock::now();
                            auto sumaCzasow = ((chrono::duration_cast<chrono::nanoseconds>(endTime - startTime).count())/testPrecision)-50;
                            testOutput << "\t" << sumaCzasow;
                        }
                        testOutput << endl;
                    }
                }
                cout << "DONE!\nWyszukiwanie - klucz poza przedziałem..."; // OK
                testOutput << "Wyszukiwanie - klucz poza przedziałem:\n";
                {
                    testOutput << "wielkosc struktury\t[10,100]\t[~INT_MAX/2]\t[~INT_MAX]\t[1,INT_MAX]\n";
                    for(int i = minElements ; i <= maxD; i *= 2) {
                        testOutput << i << "\t";
                        int testPrecision = (baseTestPrecision*20)/i;
                        if(testPrecision < minTestPrecision) {
                            testPrecision = minTestPrecision;
                        }
                        for(int k = 0 ; k < dataRangesCount ; k++) {
                            uniform_int_distribution<> dis(dataRanges[k*2], dataRanges[k*2+1]);
                            testHeap.fillRandom(i, dataRanges[k*2], dataRanges[k*2+1]);
                            auto startTime = chrono::high_resolution_clock::now();
                            for(int j = 0 ; j < testPrecision ; j++) {
                                testHeap.findElement(-1);
                            }
                            auto endTime = chrono::high_resolution_clock::now();
                            auto sumaCzasow = (chrono::duration_cast<chrono::nanoseconds>(endTime - startTime).count())/testPrecision;
                            testOutput << "\t" << sumaCzasow;
                        }
                        testOutput << endl;
                    }
                }/*
                cout << "DONE!\nUsuwanie z końca...";
                testOutput << "Usuwanie z końca:\n";
                {
                    testOutput << "wielkosc struktury\t[10,100]\t[~INT_MAX/2]\t[~INT_MAX]\t[1,INT_MAX]\n";
                    for(int i = minElements ; i <= maxD; i *= 2) {
                        testOutput << i << "\t";
                        int testPrecision = baseTestPrecision;
                        if(testPrecision < minTestPrecision) {
                            testPrecision = minTestPrecision;
                        }
                        for(int k = 0 ; k < dataRangesCount ; k++) {
                            uniform_int_distribution<> dis(dataRanges[k*2], dataRanges[k*2+1]);
                            testHeap.fillRandom(i, dataRanges[k*2], dataRanges[k*2+1]);
                            auto startTime = chrono::high_resolution_clock::now();
                            for(int j = 0 ; j < testPrecision ; j++) {
                                testHeap.deleteElementByPos(i-1);
                                testHeap.addElement(1, i-1);
                            }
                            auto endTime = chrono::high_resolution_clock::now();
                            auto sumaCzasow = (chrono::duration_cast<chrono::nanoseconds>(endTime - startTime).count())/testPrecision;
                            testOutput << "\t" << sumaCzasow;
                        }
                        testOutput << endl;
                    }
                }*/
                cout << "DONE!\nUsuwanie z początku...";
                testOutput << "Usuwanie z początku:\n";
                {
                    testOutput << "wielkosc struktury\t[10,100]\t[~INT_MAX/2]\t[~INT_MAX]\t[1,INT_MAX]\n";
                    for(int i = minElements ; i <= maxD; i *= 2) {
                        testOutput << i << "\t";
                        int testPrecision = baseTestPrecision;
                        if(testPrecision < minTestPrecision) {
                            testPrecision = minTestPrecision;
                        }
                        for(int k = 0 ; k < dataRangesCount ; k++) {
                            uniform_int_distribution<> dis(dataRanges[k*2], dataRanges[k*2+1]);
                            testHeap.fillRandom(i, dataRanges[k*2], dataRanges[k*2+1]);
                            auto startTime = chrono::high_resolution_clock::now();
                            for(int j = 0 ; j < testPrecision ; j++) {
                                testHeap.deleteElementByPos(0);
                                testHeap.addElement(dis(gen), i);
                            }
                            auto endTime = chrono::high_resolution_clock::now();
                            auto sumaCzasow = ((chrono::duration_cast<chrono::nanoseconds>(endTime - startTime).count())/testPrecision)-90;
                            testOutput << "\t" << sumaCzasow;
                        }
                        testOutput << endl;
                    }
                }
                cout << "DONE!\n";
                testOutput.close();
                break;
		}
		}
	} while (opt != '0');

}

int main(int argc, char* argv[])
{
    srand(time(NULL));

	char option;
	do{
		cout << endl;
		cout << "==== MENU GLOWNE ===" << endl;
		cout << "1.Tablica" << endl;
		cout << "2.Lista" << endl;
		cout << "3.Kopiec" << endl;
		cout << "4.Przygotuj dane testowe" << endl;
		cout << "0.Wyjscie" << endl;
		cout << "Podaj opcje:";
		option = getche();
		cout << endl;

		switch (option){
		case '1':
			menu_table();
			break;

		case '2':
			menu_list();
			break;

		case '3':
			menu_heap();
			break;
        case '4':
            generateDataFile(50, 10, 100, "Data1-1.txt");
            generateDataFile(50, INT_MAX/2 - 100000000, INT_MAX/2 + 100000000, "Data1-2.txt");
            generateDataFile(50, INT_MAX - 200000000, INT_MAX, "Data1-3.txt");
            generateDataFile(maxD/2, 10, 100, "Data2-1.txt");
            generateDataFile(maxD/2, INT_MAX/2 - 100000000, INT_MAX/2 + 100000000, "Data2-2.txt");
            generateDataFile(maxD/2, INT_MAX - 200000000, INT_MAX, "Data2-3.txt");
            generateDataFile(maxD, 10, 100, "Data3-1.txt");
            generateDataFile(maxD, INT_MAX/2 - 100000000, INT_MAX/2 + 100000000, "Data3-2.txt");
            generateDataFile(maxD, INT_MAX - 200000000, INT_MAX, "Data3-3.txt");
            break;
		}

	} while (option != '0');


	return 0;
}

void generateDataFile(int ile, int from, int to, string filename) {
    ofstream output(filename.c_str());
    if(!output) {
        cout << filename << " BLAD!!!!" << endl;
    } else {
        output << ile << endl;
        uniform_int_distribution<> dis(from, to);
        for(int i = 0 ; i < ile ; i++) {
            output << dis(gen) << endl;
        }
    }
    cout << "Generowanie " << filename << " ZAKONCZONE!" << endl;
}
