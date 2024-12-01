#include <iostream>
#include <thread>
#include <semaphore> // Biblioteca para semáforos en C++20
#include <chrono>

// Semáforos para los recursos
std::binary_semaphore resourceA(1); // Semáforo binario para Recurso A (inicialmente disponible)
std::binary_semaphore resourceB(1); // Semáforo binario para Recurso B (inicialmente disponible)

// Proceso 1
void process1() {
    std::cout << "Proceso 1: Intentando adquirir Recurso A...\n";
    resourceA.acquire(); // Bloquea el acceso a Recurso A
    std::cout << "Proceso 1: Recurso A adquirido.\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Proceso 1: Intentando adquirir Recurso B...\n";
    resourceB.acquire(); // Bloquea el acceso a Recurso B
    std::cout << "Proceso 1: Recurso B adquirido.\n";

    // Liberar recursos
    resourceB.release();
    resourceA.release();
    std::cout << "Proceso 1: Recursos A y B liberados.\n";
}

// Proceso 2
void process2() {
    std::cout << "Proceso 2: Intentando adquirir Recurso A...\n";
    resourceA.acquire(); // Bloquea el acceso a Recurso A
    std::cout << "Proceso 2: Recurso A adquirido.\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Proceso 2: Intentando adquirir Recurso B...\n";
    resourceB.acquire(); // Bloquea el acceso a Recurso B
    std::cout << "Proceso 2: Recurso B adquirido.\n";

    // Liberar recursos
    resourceB.release();
    resourceA.release();
    std::cout << "Proceso 2: Recursos A y B liberados.\n";
}

int main() {
    std::thread t1(process1);
    std::thread t2(process2);

    t1.join();
    t2.join();

    std::cout << "Ambos procesos han terminado correctamente sin deadlock.\n";

    return 0;
}
