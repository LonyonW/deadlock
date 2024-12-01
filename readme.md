Descripción del Programa
    Objetivo Principal:
        Simular y resolver un deadlock que ocurre cuando dos procesos intentan adquirir los mismos recursos en un orden diferente.
    Componentes del Programa:
        Recursos Compartidos:
            Dos recursos representados por semáforos: resourceA y resourceB.
        Procesos Concurrentes:
            Proceso 1: Intenta adquirir resourceA y luego resourceB.
            Proceso 2: Intenta adquirir resourceB y luego resourceA.
        Detector de Deadlock:
            Monitorea los procesos y, si detecta que el deadlock persiste, libera los recursos forzadamente y reinicia los procesos.
    Funcionamiento:
        Los procesos intentan adquirir los recursos según su lógica.
        Si ocurre un deadlock (los recursos quedan bloqueados indefinidamente), el detector de deadlock libera los recursos y reinicia los procesos.
        El programa asegura que todos los procesos se ejecuten correctamente incluso después de resolver el deadlock.
    Salida:
        Mensajes en la consola que indican:
            Cuándo los procesos adquieren y liberan los recursos.
            Si se detecta un deadlock y cómo se resuelve.
            El estado final de los procesos.

Ejecución
    En Linux

        Compilación:
            g++ -std=c++20 -o deadlockFix deadlockFix.cpp -pthread
        Ejecución:
            ./deadlockFix

    En Windows

        Compilación (usando MinGW):
            g++ -std=c++20 -o deadlockFix deadlockFix.cpp -pthread
        Ejecución:
            deadlockFix.exe
