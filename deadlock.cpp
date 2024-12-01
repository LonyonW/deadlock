#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex resourceA;
std::mutex resourceB;

// Proceso 1
void process1() {
    std::cout << "Proceso 1: Intentando adquirir Recurso A...\n";
    resourceA.lock();
    std::cout << "Proceso 1: Recurso A adquirido.\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Proceso 1: Intentando adquirir Recurso B...\n";
    resourceB.lock(); // Aquí ocurre el deadlock si otro hilo tiene resourceB bloqueado
    std::cout << "Proceso 1: Recurso B adquirido.\n";

    resourceB.unlock();
    resourceA.unlock();
    std::cout << "Proceso 1: Recursos A y B liberados.\n";
}

// Proceso 2
void process2() {
    std::cout << "Proceso 2: Intentando adquirir Recurso B...\n";
    resourceB.lock();
    std::cout << "Proceso 2: Recurso B adquirido.\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Proceso 2: Intentando adquirir Recurso A...\n";
    resourceA.lock(); // Aquí ocurre el deadlock si otro hilo tiene resourceA bloqueado
    std::cout << "Proceso 2: Recurso A adquirido.\n";

    resourceA.unlock();
    resourceB.unlock();
    std::cout << "Proceso 2: Recursos B y A liberados.\n";
}

int main() {
    std::thread t1(process1);
    std::thread t2(process2);

    t1.join();
    t2.join();

    std::cout << "Ambos procesos han terminado (Esto no ocurrirá debido al deadlock).\n";

    return 0;
}
