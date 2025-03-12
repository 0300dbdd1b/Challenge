#ifndef GAME_H
#define GAME_H #include "raylib.h"
#include "platform.h"
#include "logger.h"
#include "fftw3.h"
#include "raylib.h"
#include "math.h"


#define SAMPLE_SIZE 1024
#define FFT_SIZE 1024

typedef struct {
    fftw_complex *in;
    fftw_complex *out;
    fftw_plan plan;
    float magnitude[FFT_SIZE / 2];
} FFTProcessor;


typedef	struct Block
{
	int x;
	int y;
	int column;
	int	isActive;
	int	height;
	struct	Block *next;
}	Block;

typedef struct Keybind
{
	KeyboardKey	key;
	int			column;
}	Keybind;

typedef struct GameMusic
{
	Music			music;
	Wave			wave;
	Sound			sound;
	float			*samples;
	int				sampleIndex;
	FFTProcessor	fft;
	int				isPlaying;
}	GameMusic;


typedef struct GameContext
{
	int				windowHeight;
	int				windowWidth;
	int				gameWindowHeight;
	int				gameWindowWidth;

	int				columnCount;
	char			*windowName;

	int				fallSpeed;
	int				visualisationOnly;

	int				validMark;
	int				score;
	float			difficulty;
	GameMusic		music;
	KeyboardKey		keybinds[7];
	Block			*head;
}	GameContext;


void	DisplayGameColumns(GameContext *ctx);
void	DisplayScore(GameContext *ctx);
void	DisplayRawWave(GameContext *ctx);
void	DisplayFFT(GameContext *ctx);

void	DisplayGameBlocks(GameContext *ctx);
void	UpdateFrame(GameContext *ctx);

void	InitGame(GameContext *ctx);
void	RefreshGameCtx(GameContext *ctx);

void	AddBlock(GameContext* ctx, int column);
void	RemoveFirstBlock(GameContext* ctx);
void	UpdateBlocksPosition(GameContext *ctx);
void	UpdateBlocks(GameContext *ctx);

void	CheckBlocks(GameContext *ctx);
void	CheckValid(GameContext *ctx);

void DetectBeats(GameContext *ctx);
int DetermineLaneFromFFT(GameContext *ctx);


void InitFFTProcessor(FFTProcessor *fft);
void ComputeFFT(FFTProcessor *fft, float *samples);
void DestroyFFTProcessor(FFTProcessor *fft);


void	LoadMusic(GameContext  *ctx, const char *filename);

#define FFT_SMOOTHING 0.8f			//NOTE: Higher = smoother, lower = more responsive
#define FFT_MIN_UPDATE 0.001f		//NOTE: Minimum time before FFT updates (avoid blinking)

void	HandleKeys(GameContext *ctx);


#endif
