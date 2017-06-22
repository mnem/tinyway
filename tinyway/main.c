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

typedef int8_t cell_group_t;
typedef int8_t cell_t;

#define kCellAlive (1)
#define kCellDead  (0)

#define CELL_GROUP_WIDTH (8)
#define ROW_GROUP_COUNT (MAX_W / CELL_GROUP_WIDTH)
#define BOARD_GROUP_COUNT (ROW_GROUP_COUNT * MAX_H)

cell_group_t board[BOARD_GROUP_COUNT];
cell_group_t row_buffer[ROW_GROUP_COUNT];
cell_group_t row_buffer_b[ROW_GROUP_COUNT];

void board_clear() {
    const cell_group_t zero = 0;
    for(int i = 0; i < BOARD_GROUP_COUNT; i++) {
        board[i] = zero;
    }
}

cell_t board_read(int x, int y) {
    if (x < 0 || y < 0 || x >= MAX_W || y >= MAX_H ) {
        return 0;
    }
    const int row_group = (int)(x / CELL_GROUP_WIDTH);
    const int board_group = y * ROW_GROUP_COUNT + row_group;
    const int group_offset = x - (row_group * CELL_GROUP_WIDTH);
    const int shift = CELL_GROUP_WIDTH - 1 - group_offset;
    
    if (board[board_group] & (1 << shift)) {
        return kCellAlive;
    } else {
        return kCellDead;
    }
}

void board_write(int x, int y) {
    if (x < 0 || y < 0 || x >= MAX_W || y >= MAX_H ) {
        return;
    }
    const int row_group = (int)(x / CELL_GROUP_WIDTH);
    const int board_group = y * ROW_GROUP_COUNT + row_group;
    const int group_offset = x - (row_group * CELL_GROUP_WIDTH);
    const int shift = CELL_GROUP_WIDTH - 1 - group_offset;
    
    board[board_group] |= 1 << shift;
}

void row_buffer_write(int x) {
    const int row_group = (int)(x / CELL_GROUP_WIDTH);
    const int group_offset = x - (row_group * CELL_GROUP_WIDTH);
    const int shift = CELL_GROUP_WIDTH - 1 - group_offset;
    
    row_buffer[row_group] |= 1 << shift;
}

void row_buffer_store_to_board(int y) {
    y -= 1;
    if (y >= 0) {
        const int row_start = y * ROW_GROUP_COUNT;
        for(int i = 0; i < ROW_GROUP_COUNT; i++) {
            board[row_start + i] = row_buffer_b[i];
        }
    }
    
    for(int i = 0; i < ROW_GROUP_COUNT; i++) {
        row_buffer_b[i] = row_buffer[i];
    }
}

void row_buffer_flush_to_board(int y) {
    if (y < 0 || y >= MAX_H ) {
        return;
    }
    const int row_start = y * ROW_GROUP_COUNT;
    for(int i = 0; i < ROW_GROUP_COUNT; i++) {
        board[row_start + i] = row_buffer[i];
    }
}

void row_buffer_clear() {
    const cell_group_t zero = 0;
    for(int i = 0; i < ROW_GROUP_COUNT; i++) {
        row_buffer[i] = zero;
    }
}

void row_buffer_clear_both() {
    const cell_group_t zero = 0;
    for(int i = 0; i < ROW_GROUP_COUNT; i++) {
        row_buffer[i] = zero;
        row_buffer_b[i] = zero;
    }
}

void board_evolve() {
    row_buffer_clear_both();
    
    for (int y = 0; y < MAX_H; y++) {
        row_buffer_clear();
        for (int x = 0; x < MAX_W; x++) {
            int n = 0;
            
            n += board_read(x - 1, y - 1);
            n += board_read(x    , y - 1);
            n += board_read(x + 1, y - 1);
            
            n += board_read(x - 1, y    );
            n += board_read(x + 1, y    );
            
            n += board_read(x - 1, y + 1);
            n += board_read(x    , y + 1);
            n += board_read(x + 1, y + 1);
            
            if (n == 3 || (n == 2 && (board_read(x, y) == kCellAlive))) {
                row_buffer_write(x);
            }
        }
        row_buffer_store_to_board(y);
    }
    
    row_buffer_flush_to_board(MAX_H - 1);
}

void board_print() {
    printf("\nBoard:\n");
    for (int y = 0; y < MAX_H; y++) {
        printf("  ");
        for (int x = 0; x < MAX_W; x++) {
            if (board_read(x, y) == kCellAlive) {
                printf("*");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
}

int main(int argc, const char * argv[]) {
    board_clear();
    
//    board_write(1, 10);
//    board_write(2, 10);
//    board_write(0, 11);
//    board_write(2, 11);
//    board_write(2, 12);
    
//    board_write(0, 0);
//    board_write(-1, 0);
//    board_write(-2, 0);
    
    board_write(1, 0);
    board_write(1, 1);
    board_write(1, 2);
    
    board_write(MAX_W-2, 0);
    board_write(MAX_W-2, 1);
    board_write(MAX_W-2, 2);
    
    board_write(MAX_W-2, MAX_H - 1);
    board_write(MAX_W-2, MAX_H - 2);
    board_write(MAX_W-2, MAX_H - 3);
    
    board_write(1, MAX_H - 1);
    board_write(1, MAX_H - 2);
    board_write(1, MAX_H - 3);
    
    
    board_write(11,  8);
    board_write(12,  8);
    board_write(10,  9);
    board_write(12,  9);
    board_write(12, 10);
    
    board_print();
    
    for (int i = 0; i < 200; i++) {
        board_evolve();
        board_print();
    }
}

