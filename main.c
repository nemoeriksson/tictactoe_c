#include <stdio.h>

#define clear() printf("\033[H\033[J")

typedef enum {
    X,
    O,
    EMPTY
} symbols;

void render(int p1_board, int p2_board);
int check_win(int board, int p_board);
int check_valid_move(int board, int move);
void get_move(int *board, int *p_board);

int main(void){
    int p1_board = 0, p2_board = 0, winner = 0, move_count = 0;
    
    do {
        clear();
        render(p1_board, p2_board);

        int board = p1_board | p2_board;

        switch (move_count%2) {
            case 0:
                get_move(&board, &p1_board);
                break;
            case 1:
                get_move(&board, &p2_board);
                break;
        }

        if (check_win(board, p1_board)) winner = 1;
        else if (check_win(board, p2_board)) winner = 2;
        else if (board == 0b111111111) break;

        move_count++;
    } while (winner == 0);

    clear();
    render(p1_board, p2_board);
    
    if(winner) printf("\n\n\e[1mPLAYER %d WON!\e[0m", winner);
    else printf("\n\n\e[1mDRAW\e[0m");

    return 0;
}

void render(int p1_board, int p2_board){
    char *character_map[] = {"\e[1;36mX\e[0m", "\e[1;35mO\e[0m", "\e[1;2m.\e[0m"};
    int i = 0;

    printf("\e[1m\\ | 1 2 3\n--+------\e[0m");

    for (int r = 0; r < 3; r++){
        printf("\e[1m\n%d | \e[0m", r+1);
        for (int c = 0; c < 3; c++){
            printf("%s ", ((p1_board>>i) & 1) == 1 ? character_map[X]
                        : ((p2_board>>i) & 1) == 1 ? character_map[O] 
                        : character_map[EMPTY]);   
            i++;
        }
    }
}

int check_win(int board, int p_board){
    int wins[] = {
        0b111000000,
        0b000111000,
        0b000000111,
        0b100100100,
        0b010010010,
        0b001001001,
        0b100010001,
        0b001010100
    };

    for (int i = 0; i < 8; i++){
        if ((wins[i] & p_board) == wins[i]){
            return 1;
        }
    }

    return 0;
}

int check_valid_move(int board, int move){
    return !(board & move);
}

void get_move(int *board, int *p_board){
    int move = 0;
    int x, y;

    do {
        printf("\n\nEnter your move [x y]: ");
        scanf("%d %d", &x, &y);
        move = (1 << (x-1)) << (y-1) * 3;
    } while (!check_valid_move(*board, move));

    *p_board |= move;
    *board |= move;
}
