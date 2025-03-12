#include "game.h"
#include "raylib.h"


void AddBlock(GameContext* ctx, int column)
{
    Block* newBlock = (Block*)malloc(sizeof(Block));
    newBlock->column = column;
    newBlock->x = column * (ctx->windowWidth/ctx->columnCount);
    newBlock->y = 0;
    newBlock->next = ctx->head;
	newBlock->height = (ctx->windowHeight * 0.10);
    ctx->head = newBlock;
}

void RemoveFirstBlock(GameContext* ctx)
{ Block *newHead = ctx->head->next; free(ctx->head);
	ctx->head = newHead;
}

void UpdateBlocks(GameContext *ctx)
{
    Block *current = ctx->head;
    Block *prev = NULL;

    while (current != NULL) {
        current->y += ctx->fallSpeed;

        // Remove block if it goes off the screen
        if (current->y > ctx->windowHeight)
		{
            printf("Removing block from column %d\n", current->column);
            RemoveFirstBlock(ctx);
            current = (prev) ? prev->next : ctx->head;  // Reset loop pointer
        }
		else
		{
            prev = current;
            current = current->next;
        }
    }
}
void UpdateBlocksPosition(GameContext *ctx)
{
	Block *current = ctx->head;
	while (current != NULL)
	{
		current->y += ctx->fallSpeed;
		current = current->next;
	}
}












#define BEAT_THRESHOLD 0.5f  // Sensitivity threshold for detecting beats
#define MIN_BEAT_INTERVAL 0.2f  // Minimum time between beats (in seconds)

void DetectBeats(GameContext *ctx)
{
    static float lastBeatTime = 0.0f;
    float currentTime = GetMusicTimePlayed(ctx->music.music);
    
    int sampleOffset = (int)(currentTime * ctx->music.wave.sampleRate) * ctx->music.wave.channels;
    if (sampleOffset >= ctx->music.wave.frameCount * ctx->music.wave.channels) return;

    float amplitude = 0.0f;
    int numSamples = 1024; // Analyze 1024 samples at a time

    for (int i = 0; i < numSamples; i++) {
        int index = sampleOffset + i;
        if (index >= ctx->music.wave.frameCount * ctx->music.wave.channels) break;
        amplitude += fabs(ctx->music.samples[index]); // Get absolute amplitude
    }

    amplitude /= numSamples; // Normalize amplitude

    // If amplitude exceeds threshold and enough time has passed, spawn a block
    if (amplitude > BEAT_THRESHOLD && (currentTime - lastBeatTime) > MIN_BEAT_INTERVAL) {
        lastBeatTime = currentTime;
        int column = DetermineLaneFromFFT(ctx);  // Choose lane based on FFT
        printf("Beat detected at %.2f sec -> Spawning block in column %d\n", currentTime, column);
        AddBlock(ctx, column);  // Spawn a block
    }
}

int DetermineLaneFromFFT(GameContext *ctx)
{
    int column = 0;
    float maxMagnitude = 0.0f;
    int numBands = FFT_SIZE / 2 / ctx->columnCount; // Split FFT into lane regions

    for (int i = 0; i < ctx->columnCount; i++) {
        float bandMagnitude = 0.0f;

        for (int j = i * numBands; j < (i + 1) * numBands; j++) {
            bandMagnitude += ctx->music.fft.magnitude[j];
        }

        if (bandMagnitude > maxMagnitude) {
            maxMagnitude = bandMagnitude;
            column = i;
        }
    }

    return column;
}
