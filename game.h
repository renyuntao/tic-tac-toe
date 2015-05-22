#ifndef GAME_H
#define GAME_H
/**
 * Tic-Tac-Toe by the Monte Carlo Method
 *
 * The AI player has no knowledge of the rules or goals of the game.
 *
 * This source is hereby granted to the Public Domain.
 *
 * Compile with optimization for nice speed increases.
 *
 * Brian "Beej" Hall <beej@beej.us>
 *
 * Sun Aug 30 13:08:54 PDT 2009
 */

//#include <stdio.h>
//#include <stdlib.h>
//#include <time.h>
//#include <assert.h>
//#include <limits.h>
//#include <unistd.h>


#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/time.h>
#include<sys/stat.h>
#include<netinet/in.h>
#include<netdb.h>
#include<unistd.h>
#include<error.h>
#include<arpa/inet.h>
#include<sys/un.h>
#include<errno.h>
#include <pthread.h>
#include<time.h>
#include<assert.h>
#include<limits.h>
#define BUF_SIZE 5000
#define SERV_IP "127.0.0.1"
#define SERV_PORT "8989"
#define SERV_CHAT_PORT "6767"


// the following are used to reward a win and penalize a loss:
#define WIN_POINTS 1
#define LOSS_POINTS -10 // -1 doesn't work so well, -5 works well, -10 better
#define DRAW_POINTS 0   // dunno what would work best here

// how many simulations to run each computer move:
#define ROUNDS 30000    // lower == dumber

// which side the human shall play (MARK_NONE for computer vs computer)
#define HUMAN_PLAYER MARK_X  // MARK_X, MARK_O, or MARK_NONE


typedef enum tttpiece {
	MARK_NONE,
	MARK_X,
	MARK_O
} TTTPiece;

typedef struct tttboard {
	TTTPiece p[9];
} TTTBoard;


/**
 * Check for a win on a particular board
 */
TTTPiece ttt_check_win(TTTBoard *board);

/**
 * Empty a board
 */
void ttt_clear_board(TTTBoard *board);

/**
 * Copy a board
 */
void ttt_copy_board(TTTBoard *dest, TTTBoard *src);

/**
 * Return true if a board is full (no moves left)
 */
int ttt_board_full(TTTBoard *board);

/**
 * Print a board (for humans)
 */
void ttt_print_board(TTTBoard *board);

/**
 * Choose a random move on the board
 */
int ttt_choose_random_move(TTTBoard *board);

/**
 * Run random games from a starting position
 *
 * @param board the starting board configuration
 * @param currentrealplayer whose turn it is next in the real world
 * @param count the number of simulations to run
 *
 * @return the board position that has the best score, or -1 if there's no place to go
 */

int ttt_run_random_games(TTTBoard *board, TTTPiece currealplayer, int count);

void game(int clnt_sock);

void zero_buf(char ch[]);

void show_func(int clnt_sock,char *curname);

void show_score(int clnt_sock);

#endif
