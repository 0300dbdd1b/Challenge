#include "game.h"
#include "raylib.h"


void	HandleKeys(GameContext *ctx)
{
	if (IsKeyPressed(KEY_V))
		ctx->visualisationOnly *= (-1);
}
