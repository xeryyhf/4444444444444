#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Мьютекс для синхронизации вывода
pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;

// Структура для передачи параметров в поток
typedef struct {
    int thread_num;     // Номер потока
    int sleep_time;     // Время работы потока (в секундах)
} ThreadParams;

// Функция, выполняемая каждым потоком
void* thread_function(void* arg) {
    ThreadParams* params = (ThreadParams*)arg;

    // Блокировка мьютекса для вывода
    pthread_mutex_lock(&print_mutex);
    printf("%d-й поток начал работу\n", params->thread_num);
    pthread_mutex_unlock(&print_mutex);

    // Имитация работы потока (задержка)
    sleep(params->sleep_time);

    // Вывод сообщения о завершении
    pthread_mutex_lock(&print_mutex);
    printf("%d-й поток выполнил задачу\n", params->thread_num);
    pthread_mutex_unlock(&print_mutex);

    free(params); // Освобождение памяти
    return NULL;
}

int main() {
    char choice; // Переменная для выбора пользователя

    do {
        int sleep_times[5]; // Массив для времени работы потоков

        // Запрос времени работы для каждого потока
        printf("Введите время работы для каждого из 5 потоков (в секундах):\n");
        for (int i = 0; i < 5; i++) {
            printf("Поток %d: ", i + 1);
            scanf("%d", &sleep_times[i]);
        }

        pthread_t threads[5]; // Идентификаторы потоков

        // Создание пяти потоков
        for (int i = 0; i < 5; i++) {
            ThreadParams* params = malloc(sizeof(ThreadParams));
            params->thread_num = i + 1;
            params->sleep_time = sleep_times[i];

            if (pthread_create(&threads[i], NULL, thread_function, params) != 0) {
                perror("Ошибка создания потока");
                exit(EXIT_FAILURE);
            }
        }

        // Ожидание завершения всех потоков
        for (int i = 0; i < 5; i++) {
            pthread_join(threads[i], NULL);
        }

        // Запрос действия у пользователя
        printf("\nНажмите 'x' для выхода или 'r' для перезапуска: ");
        scanf(" %c", &choice); // Пробел перед %c игнорирует предыдущие символы перевода строки

    } while (choice == 'r' || choice == 'R');

    return 0;
}
