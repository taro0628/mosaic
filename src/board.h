//
//  board.h
//  mosaic
//
//  Created by 牧野幸太郎 on 2014/08/09.
//
//

#ifndef __mosaic__board__
#define __mosaic__board__

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;
#include "ofMain.h"

#define PIECE_NUM_MAX_X 24
#define PIECE_NUM_MAX_Y 12

const int FILE_NUM_MAX = 16;
const int CHANNEL_MAX = 16;

struct piece{
    int note[32];
    int note_off[32];
    int velocity[32];
    int channel;
    int setting_image[300][300];
};


class Board{
public:
    Board();
    int get_map(int x, int y);
    bool set_map(int x, int y, int value);
    bool map_select(int x, int y);
    bool map_unselect(int x, int y);
    piece get_sequence(int x, int y);
    bool set_sequence(int x, int y, piece value);
    bool set_sequence(int x, int y, int value);
    bool set_sequence(int x, int y, piece P, int value);

    
    int get_piece_num();
    piece get_piece(int num);
    int current();
    
private:
    int map[PIECE_NUM_MAX_X][PIECE_NUM_MAX_Y];
    piece sequence[PIECE_NUM_MAX_X][PIECE_NUM_MAX_Y];
    int current_index;
    piece Piece[16 * FILE_NUM_MAX];
    //piece Piece[CHANNEL_MAX][FILE_NUM_MAX];
    int PieceNum;
};

#endif /* defined(__mosaic__board__) */
