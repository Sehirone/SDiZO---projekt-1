#ifndef ARRAY_H
#define ARRAY_H
#include <string>
#include <fstream>
#include <sstream>
#include <random>
using namespace std;

#ifndef TO_STRING
#define TO_STRING
template<class s>
string to_string(s data) {
    stringstream ss;
    ss << data;
    return ss.str();
}
#endif



template<class T>
class Array{
    private:
        T *arrPtr;
        int arrSize;
        mt19937 gen; // rng engine


        void fillZeros(int startPos){ // initial fill with zeros
            for(int i = startPos ; i < arrSize ; i++){
                arrPtr[i] = 0;
            }
        }

    public:
        Array(int n = 0) : arrSize(n), gen((random_device())()){ // Constructor with initial size
            arrPtr = new T[arrSize];
            fillZeros(0);
        }

        ~Array(){ // Destructor
            delete[] arrPtr;
        }

        int getSize() {
            return arrSize;
        }

        T getElement(int pos) {
            if(pos >= 0 && pos < arrSize)
                return arrPtr[pos];
            else
                return 0;
        }

        int deleteElementByPos(int pos, T key = 0) { // with option to check key
            if(pos >= 0 && pos < arrSize) {
                if(arrPtr[pos] == key || key == 0) {
                    for(int i = pos ; i < arrSize - 1 ; i++) {
                        arrPtr[i] = arrPtr[i+1];
                    }
                    setSize(arrSize-1);
                    return 1;
                } else {
                    return -1; // Key given != key at given pos && key != 0
                }
            } else {
                return -1; // pos < 0 || pos >= arrSize
            }
        }

        int deleteElementByKey(T key) {
            return deleteElementByPos(findElement(key), key);
        }

        int addElement(int pos, T key) {
            if(pos < 0) { // POPRAWA: rekurencyjne wywo³anie dodania elementu w celu rozszerzenia tablicy z lewej stronoy
                addElement(pos+1, 0);
                pos = 0;
            }
            if(pos < arrSize) {
                setSize(arrSize+1);
                for(int i = arrSize-1 ; i > pos ; i--) {
                    arrPtr[i] = arrPtr[i-1];
                }
            } else {
                setSize(pos+1);
            }
            arrPtr[pos] = key;
            return 1;
        }

        // Funkcja uzywana wylacznie do testow kopca bez realokacji pamieci
        void changeElement(int pos, T key) {
            arrPtr[pos] = key;
        }

        int findElement(T key) {
            for(int i = 0 ; i < arrSize ; i++) {
                if(arrPtr[i] == key)
                    return i;
            }
            return -1; // element not found
        }

        void setSize(int newSize) { // resizing
            T *newArrPtr = new T[newSize];
            if(newSize > arrSize) {
                for(int i = 0 ; i < arrSize ; i++) {
                    newArrPtr[i] = arrPtr[i];
                }
            } else {
                for(int i = 0 ; i < newSize ; i++) {
                    newArrPtr[i] = arrPtr[i];
                }
            }
            int oldArrSize = arrSize;
            arrSize = newSize;
            delete[] arrPtr;
            arrPtr = newArrPtr;
            fillZeros(oldArrSize);
        }

        int fillFromFile(string filename) {
            ifstream input(filename.c_str());
            if(!input) {
                return -1;
            }
            int length;
            T value;
            input >> length;
            setSize(length);
            for(int i = 0 ; i < length ; i++) {
                input >> value;
                arrPtr[i] = value;
            }
            input.close();
            return 0;
        }

        void fillRandom(int newSize, int minVal = 1, int maxVal = 1000) {
            setSize(newSize);
            uniform_int_distribution<> dis(minVal, maxVal);
            for(int i = 0 ; i < arrSize ; i++) {
                    arrPtr[i] = dis(gen);
            }
        }

        void swapElements(int one, int two) {
            if(one >= arrSize || two >= arrSize) {
                return;
            } else {
                T temp = arrPtr[one];
                arrPtr[one] = arrPtr[two];
                arrPtr[two] = temp;
            }
        }

        string printStruct() { // Printing whole structure
            string toPrint = "Array: ";
            for(int i = 0 ; i < arrSize ; i++) {
                toPrint += to_string(arrPtr[i]) + ' ';
            }
            return toPrint;
        }
};

#endif // ARRAY_H
