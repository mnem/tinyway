//
//  main.c
//  tinyway
//
//  Created by David Wagner on 22/06/2017.
//  Copyright © 2017 David Wagner. All rights reserved.
//

#include <stdio.h>
#include <stdint.h>

#define LOG(args...) printf(args)

#define log()

#define MAX_W (128)
#define MAX_H (16)

typedef uint8_t cell_group_t;
#define CELL_GROUP_WIDTH (8)
#define ROW_GROUP_COUNT (MAX_W / CELL_GROUP_WIDTH)
#define BOARD_GROUP_COUNT (ROW_GROUP_COUNT * MAX_H)

cell_group_t board[BOARD_GROUP_COUNT];
cell_group_t row_buffer[ROW_GROUP_COUNT];

void board_clear() {
    const cell_group_t zero = 0;
    for(size_t i = 0; i < BOARD_GROUP_COUNT; i++) {
        board[i] = zero;
    }
}


/**
 Adds a cell to the board.
 
 @param x
 @param y
 */
void board_seed(size_t x, size_t y) {
    if (y >= MAX_H) {
        LOG("board_seed - y too large: %zu", y);
        return;
    }
    
    const size_t w_group = (int)(x / CELL_GROUP_WIDTH);
    if (w_group >= ROW_GROUP_COUNT) {
        LOG("board_seed - x too large: %zu (%zu)", x, w_group);
        return;
    }
    
    const size_t group_offset = x - (w_group * CELL_GROUP_WIDTH);
    const size_t shift = CELL_GROUP_WIDTH - 1 - group_offset;
    board[y * ROW_GROUP_COUNT + w_group] |= 1 << shift;
}

void board_print() {
    printf("\nBoard:\n");
    for (size_t y = 0; y < MAX_H; y++) {
        printf("  ");
        for (size_t x = 0; x < ROW_GROUP_COUNT; x++) {
            cell_group_t group = board[y * ROW_GROUP_COUNT + x];
            for (int c = CELL_GROUP_WIDTH - 1; c >= 0; c-- ) {
                if (group & (1 << c)) {
                    printf("*");
                } else {
                    printf(".");
                }
            }
        }
        printf("\n");
    }
}

int main(int argc, const char * argv[]) {
    board_clear();
    board_print();
    
    board_seed(0, 0);
    board_print();
    
    board_seed(MAX_W - 1, 0);
    board_print();
    
    board_seed(MAX_W - 1, MAX_H - 1);
    board_print();
    
    board_seed(0, MAX_H - 1);
    board_print();
    
    board_clear();
    board_print();
}

