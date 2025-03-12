#include "game.h"
#include "raylib.h"

void	InitGame(GameContext *ctx)
{
	ctx->windowName		= "Game";
	ctx->windowWidth	= 800;
	ctx->windowHeight	= 800;
	ctx->gameWindowWidth 	= 0.5 * ctx->windowWidth;
	ctx->gameWindowHeight	= ctx->windowHeight;
	ctx->columnCount	= 4;
	ctx->fallSpeed		= 1;
	ctx->keybinds[0]	= KEY_D;
	ctx->keybinds[1]	= KEY_F;
	ctx->keybinds[2]	= KEY_J;
	ctx->keybinds[3]	= KEY_K;
	ctx->head			= NULL;
	ctx->visualisationOnly	= -1;
	ctx->difficulty		= 1;
	ctx->validMark		= ctx->gameWindowHeight - (ctx->gameWindowHeight * 0.2);
	InitAudioDevice();
	InitWindow(ctx->windowWidth, ctx->windowHeight, ctx->windowName); /* SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED); */
}

void	RefreshGameCtx(GameContext *ctx)
{
	ctx->windowWidth	= GetScreenWidth();
	ctx->windowHeight	= GetScreenHeight();
	/* LOG_DEBUG("--Refreshing Context-- windowHeight : %d", ctx->windowHeight); */
	/* LOG_DEBUG("--Refreshing Context-- windowWidth : %d", ctx->windowWidth); */
}

void	CheckBlocks(GameContext *ctx)
{
	Block *current = ctx->head;

	while (current != NULL)
	{
		if (current->y <= ctx->windowHeight && current->y >= ctx->windowHeight - 100)
			current->isActive	= 1;
		else if (current->y > ctx->windowHeight) //continue;
			RemoveFirstBlock(ctx);
		current = current->next;
	}
}

void	CheckValid(GameContext *ctx)
{
	Block	*current	= ctx->head;
	while (current != NULL)
	{
		for (int i = 0; i < ctx->columnCount; i++)
		{
			if (current->column == i && IsKeyPressed(ctx->keybinds[i]) && current->y > ctx->validMark && (current->y + current->height)/2 < ctx->gameWindowHeight)
				ctx->score	+= 100;
		}
		current = current->next;
	}
}
