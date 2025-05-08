#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>

#define BUFFER_SIZE 10
#define MAX_CONSUMERS 5
#define MAX_QUALITIES 3

typedef struct {
    char name[128];
    char* data;
    size_t size;
    int priority; // lower number = higher priority //just ur quality
} VideoChunk;

VideoChunk* buffer[BUFFER_SIZE];
int in = 0, out = 0;
int total_chunks_produced = 0;
int total_chunks_consumed = 0;
int producer_done = 0;

sem_t empty, full;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    int consumer_id;
    int chunks_processed;
} ConsumerLeaderboard;

ConsumerLeaderboard leaderboard[MAX_CONSUMERS];

void log_event(const char *event) {
    FILE *log_file = fopen("server_log.txt", "a");
    if (!log_file) return;

    struct timeval tv;
    gettimeofday(&tv, NULL);
    time_t nowtime = tv.tv_sec;
    struct tm *tm_info = localtime(&nowtime);
    
    char time_str[26];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm_info);
    fprintf(log_file, "[%s.%06ld] %s\n", time_str, tv.tv_usec, event);
    fclose(log_file);
}

int cmp_int(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

int* get_chunk_ids(const char* dirpath, int* out_count) {
    DIR* dir = opendir(dirpath);
    if (!dir) {
        *out_count = 0;
        return NULL;
    }

    int* ids = NULL, count = 0, cap = 0;
    struct dirent* ent;
    while ((ent = readdir(dir)) != NULL) {
        int id;
        if (sscanf(ent->d_name, "chunk_%03d.ts", &id) == 1) {
            if (count == cap) {
                cap = cap ? cap * 2 : 8;
                ids = realloc(ids, cap * sizeof(int));
            }
            ids[count++] = id;
        }
    }
    closedir(dir);

    qsort(ids, count, sizeof(int), cmp_int);
    *out_count = count;
    return ids;
}

void load_chunk(const char* folder, int chunk_id, VideoChunk* chunk) {
    char path[128];
    sprintf(path, "video_chunks/%s/chunk_%03d.ts", folder, chunk_id);
    FILE* file = fopen(path, "rb");
    if (!file) {
        chunk->size = 0;
        return;
    }

    fseek(file, 0, SEEK_END);
    chunk->size = ftell(file);
    fseek(file, 0, SEEK_SET);

    chunk->data = malloc(chunk->size);
    fread(chunk->data, 1, chunk->size, file);
    fclose(file);
}

void* producer(void* arg) {
    const char* qualities[] = {"240", "480", "720"};

    for (int v = 1; v <= 5; v++) {
        char base[64];
        sprintf(base, "video_chunks/v%d", v);

        for (int q = 0; q < MAX_QUALITIES; q++) {
            char dirpath[128];
            sprintf(dirpath, "%s/%s", base, qualities[q]);

            int n_chunks;
            int* ids = get_chunk_ids(dirpath, &n_chunks);
            for (int i = 0; i < n_chunks; i++) {
                int chunk_id = ids[i];
                VideoChunk* chunk = malloc(sizeof(VideoChunk));
                char folderbuf[64];
                sprintf(folderbuf, "v%d/%s", v, qualities[q]);
                load_chunk(folderbuf, chunk_id, chunk);
                if (chunk->size == 0) {
                    free(chunk);
                    continue;
                }

                // Assign priority: 720 -> 0, 480 -> 1, 240 -> 2
                chunk->priority = MAX_QUALITIES - 1 - q;

                struct timeval tv;
                gettimeofday(&tv, NULL);
                char log_msg[256];
                sprintf(log_msg, "Producer loaded chunk %d at %ld.%06ld", chunk_id, tv.tv_sec, tv.tv_usec);
                log_event(log_msg);

                sem_wait(&empty);
                pthread_mutex_lock(&mutex);

                // Priority insertion
                int temp = in;
                int idx = in;
                while (temp != out) {
                    int prev = (temp - 1 + BUFFER_SIZE) % BUFFER_SIZE;
                    if (buffer[prev] == NULL || buffer[prev]->priority <= chunk->priority)
                        break;
                    buffer[temp] = buffer[prev];
                    temp = prev;
                }
                buffer[temp] = chunk;
                in = (in + 1) % BUFFER_SIZE;
                total_chunks_produced++;

                pthread_mutex_unlock(&mutex);
                sem_post(&full);

                usleep(50000);
            }
            free(ids);
        }
    }

    pthread_mutex_lock(&mutex);
    producer_done = 1;
    pthread_mutex_unlock(&mutex);

    for (int i = 0; i < MAX_CONSUMERS; i++)
        sem_post(&full);

    pthread_exit(NULL);
}

void* consumer(void* arg) {
    int id = *(int*)arg;
    char fifo_name[128];
    sprintf(fifo_name, "fifo_consumer_%d", id);

    struct timeval tv;
    gettimeofday(&tv, NULL);
    char log_msg[256];
    sprintf(log_msg, "Consumer %d opened FIFO at %ld.%06ld", id, tv.tv_sec, tv.tv_usec);
    log_event(log_msg);

    int fifo_fd;
    do {
        fifo_fd = open(fifo_name, O_WRONLY);
        if (fifo_fd == -1) {
            sleep(1);
        }
    } while (fifo_fd == -1);

    while (1) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        if (producer_done && total_chunks_consumed >= total_chunks_produced) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        VideoChunk* chunk = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        total_chunks_consumed++;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);

        if (chunk->size > 0) {
            write(fifo_fd, chunk->data, chunk->size);
            printf("Consumer %d processed chunk\n", id);

            gettimeofday(&tv, NULL);
            sprintf(log_msg, "Consumer %d processed chunk at %ld.%06ld", id, tv.tv_sec, tv.tv_usec);
            log_event(log_msg);

            leaderboard[id].chunks_processed++;
            sprintf(log_msg, "Consumer %d processed chunk %d", id, leaderboard[id].chunks_processed);
            log_event(log_msg);
        }

        free(chunk->data);
        free(chunk);
    }

    close(fifo_fd);
    pthread_exit(NULL);
}

void display_fancy_leaderboard() {
    FILE* file = fopen("leaderboard.txt", "w");
    if (!file) {
        perror("Failed to open leaderboard file");
        return;
    }

    printf("\n==================== LEADERBOARD ====================\n");
    fprintf(file, "\n==================== LEADERBOARD ====================\n");
    printf("| %-10s | %-15s |\n", "Consumer", "Chunks Processed");
    fprintf(file, "| %-10s | %-15s |\n", "Consumer", "Chunks Processed");
    printf("------------------------------------------------------\n");
    fprintf(file, "------------------------------------------------------\n");

    for (int i = 0; i < MAX_CONSUMERS; i++) {
        printf("| %-10d | %-15d |\n", leaderboard[i].consumer_id, leaderboard[i].chunks_processed);
        fprintf(file, "| %-10d | %-15d |\n", leaderboard[i].consumer_id, leaderboard[i].chunks_processed);
    }

    printf("======================================================\n");
    fprintf(file, "======================================================\n");

    fclose(file);
}

int main() {
    pthread_t producer_thread;
    pthread_t consumers[MAX_CONSUMERS];
    int consumer_ids[MAX_CONSUMERS] = {0, 1, 2, 3, 4};

    for (int i = 0; i < MAX_CONSUMERS; i++) {
        leaderboard[i].consumer_id = i;
        leaderboard[i].chunks_processed = 0;
    }

    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    for (int i = 0; i < MAX_CONSUMERS; i++) {
        char fifo_name[128];
        sprintf(fifo_name, "fifo_consumer_%d", i);
        mkfifo(fifo_name, 0666);
    }

    for (int i = 0; i < MAX_CONSUMERS; i++)
        pthread_create(&consumers[i], NULL, consumer, &consumer_ids[i]);

    pthread_create(&producer_thread, NULL, producer, NULL);

    pthread_join(producer_thread, NULL);
    for (int i = 0; i < MAX_CONSUMERS; i++)
        pthread_join(consumers[i], NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    display_fancy_leaderboard();

    for (int i = 0; i < MAX_CONSUMERS; i++) {
        char fifo_name[128];
        sprintf(fifo_name, "fifo_consumer_%d", i);
        unlink(fifo_name);
    }

    return 0;
}

