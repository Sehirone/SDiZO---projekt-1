#ifndef HEAPNOREALLOC_H
#define HEAPNOREALLOC_H
#include <string>
#include <fstream>
#include <sstream>
#include <random>
#include "Array.h"
#include "math.h"
using namespace std;

#ifndef TO_STRING
#define TO_STRING
template<class s>
string to_string(s data) {
    stringstream ss;
    ss << data;
    return ss.str();
}
#endif // TO_STRING

// Kopiec Binarny Maksymalny - Wersja testowa, bez realokacji pamieci
/*
    ******************************
    ***********UWAGA**************
    *Niezdefiniowane dzialanie****
    *operacji innych niz**********
    *testowane w badaniu**********
    ******************************
*/
template <class T>
class HeapNoRealloc {
    private:
        Array<T> arr;
        mt19937 gen; // rng engine
        int maxLength;
        int length;

    public:
        HeapNoRealloc() : gen((random_device())()){
            maxLength = 150217728;
            length = 0;
            arr.setSize(maxLength);
        }

        ~HeapNoRealloc() {

        }

        void addElement(T key, int pos) {
            if(pos > length) {
                pos = length;
            }
            arr.changeElement(pos, key);
            length++;
            if(pos == length - 1) {
               fixUp(pos);
            } else {
                for(int i = pos ; i < length ; i++) {
                    fixUp(i);
                }
            }
        }

        void fixUp(int pos) {
            while (pos != 0) {
                int newElem = arr.getElement(pos);
                int parentPos = ((pos - 1) / 2);
                int parent = arr.getElement(parentPos);

                if (parent < newElem) {
                    arr.swapElements(pos, parentPos);
                    pos = parentPos;
                } else {
                    break;
                }
            }
        }

        void fixDown(int pos) {
            while((2*pos+1) < length) {
                int elem = arr.getElement(pos);
                int leftPos = 2*pos + 1;
                int rightPos = 2*pos + 2;

                if(rightPos >= length) {
                    int left = arr.getElement(leftPos);
                    if(left > elem) {
                        arr.swapElements(leftPos, pos);
                        pos = leftPos;
                    } else {
                        break;
                    }
                } else {
                    int left = arr.getElement(leftPos);
                    int right = arr.getElement(rightPos);
                    if(left > elem || right > elem) {
                        if(left >= right) {
                            arr.swapElements(leftPos, pos);
                            pos = leftPos;
                        } else {
                            arr.swapElements(rightPos, pos);
                            pos = rightPos;
                        }
                    } else {
                        break;
                    }
                }
            }
        }

        int deleteElementByPos(int pos, T key = 0) { // with option to check key
            if(pos >= 0 && pos < length) {
                if(arr.getElement(pos) == key || key == 0) {
                    if(pos == length-1) {
                       arr.changeElement(pos,0);
                       length--;
                       return 1;
                    }
                    T lastElem = arr.getElement(length-1);
                    arr.swapElements(pos, length-1);
                    arr.changeElement(length-1, 0);
                    length--;

                    if(pos == 0) {
                        fixDown(pos);
                    } else {
                        int parentPos = (pos-1)/2;
                        if(arr.getElement(parentPos) <= lastElem) {
                            fixUp(pos);
                        } else {
                            fixDown(pos);
                        }
                    }
                    return 1;
                } else {
                    return -1; // Key given != key at given pos && key != 0
                }
            } else {
                return -1; // pos < 0 || pos >= arrSize
            }
        }

        int deleteElementByKey(int key) {
            return deleteElementByPos(arr.findElement(key), key);
        }

        int fillFromFile(string filename) {
            ifstream input(filename.c_str());
            if(!input) {
                return -1;
            }
            int length;
            T value;
            input >> length;
            arr.setSize(0);
            for(int i = 0 ; i < length ; i++) {
                input >> value;
                addElement(value, i);
            }
            input.close();
            return 0;
        }

        void fillRandom(int newSize, int minVal = 10, int maxVal = 100) {
            maxLength == newSize+1;
            arr.setSize(0);
            arr.setSize(maxLength);
            uniform_int_distribution<> dis(minVal, maxVal);
            for(int i = 0 ; i < newSize ; i++) {
                    length = i;
                    addElement(dis(gen), i);

            }
        }

        int findElement(T key) {
            for(int i = 0 ; i < length ; i++) {
                if(arr.getElement(i) == key)
                    return i;
            }
            return -1; // element not found
        }

        void printStruct() {
            int height = getHeapHeight();
            int spaces, dash, parent, lefty, righty;
            int len = length;

            for (int i = 0; i < height; i++) {
                // printing spaces and numbers
                for (int j = 0; j < pow(2, i); j++) {
                    spaces = pow(2, (height - i));

                    for (int k = 0; k < (spaces - 1); k++) {
                        cout << " ";
                    }

                    if (pow(2, i) + j <= len) {
                        printf("%2d", arr.getElement(pow(2, i) + j - 1));
                    }

                    for (int k = 0; k < spaces - 1; k++) {
                        cout << " ";
                    }
                }
                cout << endl;
                spaces = pow(2, (height - i));
                dash   = pow(2, (abs(i - height))) / 2;

                // printing lines and dashes
                for (int j = 0; j < pow(2, i); j++) {
                    for (int k = 0; k < (spaces - dash); k++) {
                        cout << " ";
                    }
                    parent = pow(2, i) + j;
                    lefty  = 2 * parent;
                    righty = 2 * parent + 1;

                    if (parent <= len) {
                        if (lefty <= len) {
                            for (int l = 0; l < dash; l++) {
                                cout << "_";
                            }
                            cout << "|";

                            if (righty <= len) {
                                for (int l = 0; l < dash; l++) {
                                    cout << "_";
                                }
                            }
                        }
                    }

                    for (int k = 0 + dash; k < spaces - 1; k++) {
                        cout << " ";
                    }
                }
                cout << endl;
            }
        }

        int getHeapHeight() {
            return log2(length) + 1;
        }
};
#endif // HEAPNOREALLOC_H
