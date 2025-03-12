#include "game.h"

/* void	LoadMusic(GameContext  *ctx, const char *filename) */
/* { */
/**/
/* 	ctx->music.wave			= LoadWave(filename); */
/* 	ctx->music.samples		= LoadWaveSamples(ctx->music.wave); */
/* 	ctx->music.sound		= LoadSoundFromWave(ctx->music.wave); */
/*     ctx->music.sampleIndex	= 0; */
/* 	ctx->music.isPlaying 	= 1; */
/*     PlaySound(ctx->music.sound); */
/* } */






void LoadMusic(GameContext *ctx, const char *filename)
{
	printf("Loading new music: %s\n", filename);

	// Stop and Unload previous music data properly
	StopMusicStream(ctx->music.music);
	UnloadMusicStream(ctx->music.music);
	UnloadWave(ctx->music.wave);

	if (ctx->music.samples) {
		UnloadWaveSamples(ctx->music.samples);
		ctx->music.samples = NULL;
	}

	DestroyFFTProcessor(&ctx->music.fft);  // Ensure FFT is reset

	// Load new music and wave data
	ctx->music.music = LoadMusicStream(filename);
	ctx->music.wave = LoadWave(filename);

	// Ensure wave data is valid
	if (!IsWaveValid(ctx->music.wave)) {
		printf("Error: Wave data is not valid!\n");
		return;
	}

	// Convert wave to float samples
	ctx->music.samples = LoadWaveSamples(ctx->music.wave);
	if (!ctx->music.samples) {
		printf("Error: Could not load wave samples!\n");
		return;
	}

	// Reinitialize FFT Processor for new song
	InitFFTProcessor(&ctx->music.fft);

	// Reset sample index and playback position
	ctx->music.sampleIndex = 0;
	SeekMusicStream(ctx->music.music, 0);
	ctx->music.isPlaying = 1;
	PlayMusicStream(ctx->music.music);

	// Reset the last sample offset to avoid invalid reads
	printf("New wave loaded, frame count: %d\n", ctx->music.wave.frameCount);
}

