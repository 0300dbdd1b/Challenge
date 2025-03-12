#include "game.h"

int main()
{

	GameContext ctx = {0};
	InitGame(&ctx);
	SetTargetFPS(30);
	/* LoadMusic(&ctx, "src/songs/THEODORT-Wayeh.mp3"); */
	AddBlock(&ctx, 3);

	while (!WindowShouldClose())
	{
		if (IsFileDropped())
		{
			FilePathList droppedFiles = LoadDroppedFiles();
			LoadMusic(&ctx, droppedFiles.paths[droppedFiles.count - 1]);
			UnloadDroppedFiles(droppedFiles);
		}
		BeginDrawing();
		ClearBackground(BLACK);
		UpdateFrame(&ctx);
		EndDrawing();
	}

	DestroyFFTProcessor(&ctx.music.fft);
	UnloadWaveSamples(ctx.music.samples);
	UnloadWave(ctx.music.wave);
	UnloadMusicStream(ctx.music.music);
	CloseAudioDevice();
	CloseWindow();

	return 0;
}

