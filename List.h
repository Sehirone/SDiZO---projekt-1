#ifndef LIST_H
#define LIST_H
#include <string>
#include <fstream>
#include <sstream>
#include "time.h"
#include "stdlib.h"
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

template<class e>
struct Element {
    Element *prev;
    Element *next;
    e value;
};

template<class T>
class List{
    private:
        Element<T> *head;
        Element<T> *tail;
        mt19937 gen; // rng engine

    public:
        List() : gen((random_device())()) {
            head = nullptr;
            tail = nullptr;
        }

        ~List() {
            emptyList();
        }

        void addOnBeg(T key) {
            Element<T> *newElement = new Element<T>;
            newElement->value = key;

            if(head == nullptr) {
                head = newElement;
                tail = head;
                head->next = nullptr;
            } else {
                newElement->next = head;
                head->prev = newElement;
                head = newElement;
            }
            head->prev = nullptr;
        }

        void addOnEnd(T key) {
            if(head == nullptr) {
                addOnBeg(key);
            } else {
                Element<T> *newElement = new Element<T>;
                newElement->value = key;
                if(tail != head) {
                    newElement->prev = tail;
                    newElement->next = nullptr;
                    tail->next = newElement;
                    tail = newElement;
                } else {
                    head->next = newElement;
                    newElement->prev = head;
                    tail = newElement;
                    tail->next = nullptr;
                }
            }
        }

        void addElement(int pos, T key) {
            if(pos < 0) {  // POPRAWA: rekurencyjne wywo³anie dodania elementu w celu rozszerzenia tablicy z lewej stronoy
                addElement(pos+1,0);
                pos = 0;
            }
            if(head == nullptr && pos != 0) {
                addOnBeg(0);
            }
            if(head == nullptr || pos == 0) {
                addOnBeg(key);
                return;
            }

            Element<T> *ptr = head;

            for(int i = 0 ; i < pos - 1 ; i++) {
                if(ptr->next == nullptr) {
                    addOnEnd(0);
                }
                ptr = ptr->next;
            }

            if(ptr->next == nullptr) {
                    addOnEnd(key);
                    return;
            }

            Element<T> *newElement = new Element<T>;
            newElement->value = key;
            Element<T> *elementBefore = ptr;
            Element<T> *elementNext = ptr->next;
            elementBefore->next = newElement;
            elementNext->prev = newElement;
            newElement->next = elementNext;
            newElement->prev = elementBefore;
        }

        void emptyList() {
            if(head != nullptr) {
                Element<T> *ptr = head;
                while(ptr->next != nullptr) {
                    ptr = ptr->next;
                    delete ptr->prev;
                }
                delete ptr;
                head = nullptr;
                tail = nullptr;
            }
        }

        int fillFromFile(string filename) {
            ifstream input(filename.c_str());
            if(!input) {
                return -1;
            }
            int length;
            T value;
            input >> length;
            emptyList();
            for(int i = 0 ; i < length ; i++) {
                input >> value;
                addElement(i, value);
            }
            input.close();
            return 0;
        }

        void fillRandom(int newSize, int minVal = 1, int maxVal = 1000) {
            emptyList();
            uniform_int_distribution<> dis(minVal, maxVal);
            for(int i = 0 ; i < newSize ; i++) {
                    addElement(0, dis(gen));
            }
        }

        int findElement(T key) {
            Element<T> *ptr = head;
            int i = 0;
            while(ptr != nullptr) {
                if(ptr->value == key) {
                    return i;
                }
                ptr = ptr->next;
                i++;
            }
            return -1; // element not found
        }

        int deleteElementByPos(int pos, T key = 0) { // with option to check key
            if(pos < 0){
                return -1;
            }
            Element<T> *ptr = head;

            if(pos == 0) {
                if(head == nullptr) {
                    return -1;
                } else if (head->next == nullptr) {
                    if(key == 0 || head->value == key) {
                        emptyList();
                        return 1;
                    } else {
                        return -1;
                    }
                }
                if(key == 0 || head->value == key) {
                    ptr = ptr->next;
                    ptr->prev = nullptr;
                    delete head;
                    head = ptr;
                    return 1;
                } else {
                    return -1;
                }
            }

            int i = 0;
            while(i < pos && ptr->next != nullptr) {
                ptr = ptr->next;
                i++;
            }
            if(pos > i) {
                return -1;
            }
            if(ptr->next == nullptr) {
                if(key == 0 || ptr->value == key) {
                    tail = ptr->prev;
                    tail->next = nullptr;
                    delete ptr;
                    return 1;
                } else {
                    return -1;
                }
            } else {
                if(key == 0 || ptr->value == key) {
                    ptr->prev->next = ptr->next;
                    ptr->next->prev = ptr->prev;
                    delete ptr;
                    return 1;
                } else {
                    return -1;
                }
            }
        }

        int deleteElementByKey(T key) {
            return deleteElementByPos(findElement(key), key);
        }

        string printStruct() {
            string toPrint = "List: ";
            if(head == nullptr) {
                toPrint += "Empty!";
            } else {
                Element<T> *ptr = head;
                do {
                    toPrint += to_string(ptr->value) + ' ';
                    ptr = ptr->next;
                } while (ptr != nullptr);
            }
            return toPrint;
        }
};

#endif // ARRAY_H
