#ifndef BATCH_H
#define BATCH_H

typedef struct {
    int batchId;
    int courseId;
    float fee;
    int capacity;
    char mode[20];
    char startDate[15];   // NEW
} Batch;

extern Batch batches[100];
extern int batchCount;

void saveBatches();
void loadBatches();

void addBatch();
void viewBatches();

#endif