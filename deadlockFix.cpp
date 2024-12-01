#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <semaphore> 
#include <condition_variable>
#include <mutex>

std::counting_semaphore<1> resourceA(1); // Semáforo para el recurso A
std::counting_semaphore<1> resourceB(1); // Semáforo para el recurso B

std::atomic<bool> deadlockDetected(false);
std::atomic<bool> process1Finished(false);
std::atomic<bool> firstExecution(true); // Indica si es la primera ejecución
std::atomic<bool> process1Completed(false); // Variable para controlar si el Proceso 1 ha terminado

std::mutex cvMutex;
std::condition_variable cv; // Para sincronizar el proceso 2 con el proceso 1

// Proceso 1
void process1() {
    std::cout << "Proceso 1: Intentando adquirir Recurso A...\n";
    resourceA.acquire();
    std::cout << "Proceso 1: Recurso A adquirido.\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Proceso 1: Intentando adquirir Recurso B...\n";
    resourceB.acquire(); // Aquí ocurre el deadlock si otro hilo tiene resourceB bloqueado
    std::cout << "Proceso 1: Recurso B adquirido.\n";

    resourceB.release();
    resourceA.release();
    std::cout << "Proceso 1: Recursos A y B liberados.\n";

    process1Finished = true;
    process1Completed = true; // Marcar el Proceso 1 como completamente terminado

    // Notificar al Proceso 2 que el Proceso 1 ha terminado
    cv.notify_one();
}

// Proceso 2
void process2() {
    if (!firstExecution) { // Solo esperar si no es la primera ejecución
        std::cout << "Proceso 2: Esperando a que el Proceso 1 termine completamente.\n";
        std::unique_lock<std::mutex> lock(cvMutex);
        cv.wait(lock, [] { return process1Completed.load(); });
    }

    std::cout << "Proceso 2: Intentando adquirir Recurso B...\n";
    resourceB.acquire();
    std::cout << "Proceso 2: Recurso B adquirido.\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Proceso 2: Intentando adquirir Recurso A...\n";
    resourceA.acquire(); // Aquí ocurre el deadlock si otro hilo tiene resourceA bloqueado
    std::cout << "Proceso 2: Recurso A adquirido.\n";

    resourceA.release();
    resourceB.release();
    std::cout << "Proceso 2: Recursos B y A liberados.\n";
}

// Detector de deadlock
void deadlockDetector() {
    std::this_thread::sleep_for(std::chrono::seconds(3)); // Tiempo para que ocurra el deadlock

    if (!process1Finished.load()) { // Si el proceso 1 no ha terminado, asumimos deadlock
        std::cout << "Deadlock detectado. Liberando recursos forzadamente...\n";

        // Liberar los recursos forzadamente
        resourceA.release();
        std::cout << "Recurso A liberado forzadamente.\n";

        resourceB.release();
        std::cout << "Recurso B liberado forzadamente.\n";

        deadlockDetected = true; // Indicar que se detectó y resolvió el deadlock
        std::cout << "Recursos liberados. Reiniciando procesos...\n";

        // Reiniciar los procesos después de resolver el deadlock
        process1Finished = false;
        process1Completed = false; // Resetear el estado de finalización del Proceso 1
        firstExecution = false; // Indicar que ya no es la primera ejecución

        // Crear hilos nuevamente
        std::thread t1(process1);
        std::thread t2(process2);

        t1.join();
        t2.join();

        deadlockDetected = false; // Resetear el estado tras la ejecución correcta
        std::cout << "Procesos completados exitosamente tras resolver el deadlock.\n";
    }
}

int main() {
    std::thread detector(deadlockDetector);

    // Iniciar procesos iniciales
    std::thread t1(process1);
    std::thread t2(process2);

    t1.join();
    t2.join();
    detector.join();

    std::cout << "Todos los procesos han finalizado correctamente.\n";
    return 0;
}
