#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <map>
#include <vector>
#include <functional>
#include <iomanip>
#include <thread>
#include <mutex>
#include <algorithm>
#include <fcntl.h>
#include <sys/types.h>
#include <syscall.h>
#include <string.h>
#include <termios.h>

using namespace std;

int lastProcess = -1;
mutex global_mutex;
vector<thread::id> threads;

char _getch()
{
   struct termios oldAttr, newAttr;
   tcgetattr(STDIN_FILENO, &oldAttr);
   newAttr = oldAttr;
   newAttr.c_lflag &= ~(ICANON | ECHO);
   tcsetattr(STDIN_FILENO, TCSANOW, &newAttr);
   char symbol;
   symbol = getchar();
   tcsetattr(STDIN_FILENO, TCSANOW, &oldAttr);
   return symbol;
}

void worker(int current_id) {

    while (true) {
        global_mutex.lock(); // захватываем мьютексустановка мьютекса в положение «занято».Если мьютекс уже занят, функция ждет его освобождения и после этого зани-мает его;
        if(threads.end() == find(threads.begin(), threads.end(), this_thread::get_id())) {
            lastProcess = -1;
           global_mutex.unlock(); // освобождаем мьютекс..
           return;
        }
        if (lastProcess != current_id - 1) {
            global_mutex.unlock(); // // освобождаем мьютекс
            this_thread::sleep_for(chrono::milliseconds(10));
            continue;
        }

        if (current_id == threads.size() - 1) {
            lastProcess= -1;
        }
        else lastProcess = current_id;

        if (current_id == 0) {
            this_thread::sleep_for(chrono::milliseconds(1000));
            cout << setfill('-') << setw(40) << '-' << endl;
        }

        cout << "Thread " << current_id + 1 << endl;
        if (threads.size() == 0) {
            cout << setfill('-') << setw(40) << '-' << endl;// в качестве символа заполн процесса
        }
        global_mutex.unlock();
    }
}



int main() {

    cout << "+ --- Create new thread.\n- --- Delete last thread.\nq --- Delete all threads.\n";
    while(true){
         switch(_getch()){
            case '+': {
                thread* tr = new thread(worker, threads.size());//+1
                threads.push_back(tr->get_id());
                tr->detach();// сразу же освобождаем объект, связанный с ним
            }
                break;
            case '-': {
                threads.pop_back();//Удаляет последний элемент в векторе, эффективно уменьшая размер контейнера на единицу.
                if(threads.size() == 0) {
                    cout << "Threads treads is clear.\n";
                }
            }
                break;
            case 'q': {
                threads.clear();
                exit(0);
            }
        }
    }
    return 0;
}


