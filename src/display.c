#include "logger.h"
#include "raylib.h"
#include "game.h"
#include <stdio.h>


void	DisplayScore(GameContext *ctx)
{
	char score[20];
	sprintf(score, "SCORE : %d", ctx->score);
	DrawText(score, 0, 0, 10, YELLOW);
}

void	DisplayGameColumns(GameContext *ctx)
{
	if (ctx->visualisationOnly == 1)
		return;
	Vector2 columnStart	= {.x = (float)ctx->gameWindowWidth/ctx->columnCount, .y = 0 };
	Vector2 columnEnd	= {.x = columnStart.x, .y = ctx->gameWindowHeight };
	for (int i = 0; i < ctx->columnCount; i++)
	{
		DrawLineEx(columnStart, columnEnd, 2, GRAY);
		columnStart.x	= columnStart.x + ((float)ctx->gameWindowWidth / ctx->columnCount);
		columnEnd.x		= columnStart.x;
	}
	Vector2	markStart 	= {.x = 0, .y =  ctx->validMark};
	Vector2	markEnd 	= {.x = ctx->gameWindowWidth, .y = ctx->validMark};
	DrawLineEx(markStart, markEnd, 2, GRAY);
}

void	DisplayGameBlocks(GameContext *ctx)
{
	if (ctx->visualisationOnly == 1)
		return ;
	Block	*current	= ctx->head;
	while (current != NULL)
	{
		Rectangle rec = {
			.x		=	current->column * ((float)ctx->gameWindowWidth/ctx->columnCount),
			.y		=	current->y,
			.width	= ((float)ctx->gameWindowWidth/ctx->columnCount),
			.height = current->height
		};
		DrawRectangleRec(rec, GREEN);
		current = current->next;
	}
}



void DisplayRawWave(GameContext *ctx)
{
	if (!ctx->music.isPlaying)
		return;

	int posX	=	0;
	int posY	=	0;
	int width	=	ctx->windowWidth;
	int height	=	ctx->windowHeight;

	if (ctx->visualisationOnly == -1)
	{
		posX	=	ctx->gameWindowWidth;
		posY	=	10;
		width	=	ctx->windowWidth - posX;
		height	=	ctx->windowHeight / 2;
		DrawRectangle(posX, posY, width, height, BLACK);
	}
	// Draw black background for the waveform display

	static int lastSampleOffset = 0;
	float elapsedTime = GetMusicTimePlayed(ctx->music.music);
	int sampleOffset = (int)(elapsedTime * ctx->music.wave.sampleRate) * ctx->music.wave.channels;

	if (sampleOffset < lastSampleOffset)	// WARN: If it breaks it must be about this.
		lastSampleOffset = 0;
	if (sampleOffset <= lastSampleOffset) return; // Avoid unnecessary updates
	lastSampleOffset = sampleOffset;

	int channels = ctx->music.wave.channels;
	float scaleFactor = height / 2.5f;

	int prevX = posX;
	int prevY = posY + height / 2;
	int step = 4; // Downsample to avoid overlapping lines

	for (int i = 0; i < SAMPLE_SIZE - step; i += step) {
		int index = sampleOffset + (i * channels);
		int nextIndex = sampleOffset + ((i + step) * channels);

		if ((unsigned int)index >= ctx->music.wave.frameCount * channels ||
			(unsigned int)nextIndex >= ctx->music.wave.frameCount * channels)
			break;

		float sampleValue = 0.0f;
		float nextSampleValue = 0.0f;

		for (int ch = 0; ch < channels; ch++) {
			sampleValue += ctx->music.samples[index + ch];
			nextSampleValue += ctx->music.samples[nextIndex + ch];
		}
		sampleValue /= channels;
		nextSampleValue /= channels;

		sampleValue = fmaxf(fminf(sampleValue, 1.0f), -1.0f);
		nextSampleValue = fmaxf(fminf(nextSampleValue, 1.0f), -1.0f);

		int x1 = posX + (i * width / SAMPLE_SIZE);
		int y1 = posY + height / 2 - sampleValue * scaleFactor;
		int x2 = posX + ((i + step) * width / SAMPLE_SIZE);
		int y2 = posY + height / 2 - nextSampleValue * scaleFactor;

		Vector2 v1 = {.x = x1, .y = y1};
		Vector2 v2 = {.x = x2, .y = y2};
		DrawLineEx(v1, v2, 2 ,GREEN);
	}
}



#define NUM_BARS 200  // Number of bars to display



void DisplayFFT(GameContext *ctx)
{
    if (!ctx->music.isPlaying)
        return;

    int posX   = 0;
    int posY   = 0;
    int width  = ctx->windowWidth;
    int height = ctx->gameWindowHeight;

    if (ctx->visualisationOnly == -1)
    {
        posX   = ctx->gameWindowWidth;
        posY   = ctx->windowHeight / 2;
        width  = ctx->windowWidth - posX;
        height = ctx->gameWindowHeight - 10;
        DrawRectangle(posX, posY, width, height, BLACK);
    }

    static float lastProcessedTime = -1.0f; // Force first update
    static float smoothedMagnitudes[FFT_SIZE / 2] = {0};

    float currentTime = GetMusicTimePlayed(ctx->music.music);

    // Ensure lastProcessedTime resets after a new song is loaded
    if (lastProcessedTime < 0 || currentTime < lastProcessedTime) {
        lastProcessedTime = 0;  // Reset processing time
        memset(smoothedMagnitudes, 0, sizeof(smoothedMagnitudes));  // Clear old data
    }

    // Force FFT update immediately after a new song loads
    if (currentTime - lastProcessedTime < FFT_MIN_UPDATE && lastProcessedTime > 0) return;
    lastProcessedTime = currentTime;

    int sampleOffset = (int)(currentTime * ctx->music.wave.sampleRate) * ctx->music.wave.channels;
    if (sampleOffset >= ctx->music.wave.frameCount * ctx->music.wave.channels) return;

    float fftSamples[FFT_SIZE] = {0};
    int channels = ctx->music.wave.channels;

    for (int i = 0; i < FFT_SIZE; i++) {
        int index = sampleOffset + (i * channels);
        if (index >= ctx->music.wave.frameCount * channels) break;

        fftSamples[i] = 0.0f;
        for (int ch = 0; ch < channels; ch++) {
            fftSamples[i] += ctx->music.samples[index + ch];
        }
        fftSamples[i] /= channels;
    }

    ComputeFFT(&ctx->music.fft, fftSamples);

    float maxMagnitude = 0.0001f;
    for (int i = 0; i < FFT_SIZE / 2; i++) {
        smoothedMagnitudes[i] = (smoothedMagnitudes[i] * 0.75f) + 
                                (ctx->music.fft.magnitude[i] * 0.25f);
        if (smoothedMagnitudes[i] > maxMagnitude) {
            maxMagnitude = smoothedMagnitudes[i];
        }
    }

    maxMagnitude *= 0.9f;

    int barWidth = width / NUM_BARS;

    for (int i = 0; i < NUM_BARS; i++) {
        int index = (int)(powf(10.0f, ((float)i / NUM_BARS) * log10f(FFT_SIZE / 2)));
        if (index >= FFT_SIZE / 2) index = FFT_SIZE / 2 - 1;

        float magnitude = smoothedMagnitudes[index];

        float normalizedMagnitude = magnitude / maxMagnitude;
        normalizedMagnitude = fminf(fmaxf(normalizedMagnitude, 0.05f), 1.0f);

        float value = logf(1.0f + normalizedMagnitude * 30.0f) * (height / 3.5f);
        if (value < 2.0f) value = 2.0f;

        int x = posX + (i * barWidth);
        int y = posY + height - (int)value;

        DrawRectangle(x, y, barWidth - 1, (int)value, RED);
    }
}










void	UpdateFrame(GameContext *ctx)
{
	RefreshGameCtx(ctx);
	ClearBackground(BLACK);
	/* UpdateBlocksPosition(ctx); */
	UpdateMusicStream(ctx->music.music);
	HandleKeys(ctx);
	/* DetectBeats(ctx);  // Detect beats and spawn blocks */
    UpdateBlocks(ctx);
	CheckBlocks(ctx);
	CheckValid(ctx);

	DisplayScore(ctx);
	DisplayGameBlocks(ctx);
	DisplayGameColumns(ctx);
	DisplayFFT(ctx);
	DisplayRawWave(ctx);
}


