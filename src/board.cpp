//
//  board.cpp
//  mosaic
//
//  Created by 牧野幸太郎 on 2014/08/09.
//
//

#include "board.h"


Board::Board(){
    current_index = 0;
    
    PieceNum = 0;
    for(int channel=1; channel<=16; channel++){
        for(int fileNum=1; fileNum<=FILE_NUM_MAX; fileNum++){
            //Path to the comma delimited file
            stringstream ss;
            ss<<""<<channel<<"/"<<fileNum<<".csv";
            std::string filePath = ss.str();
            cout<<"filepass"<<filePath<<endl;
            //Load file placed in bin/data
            ofFile file(filePath);
            
            if(!file.exists()){
                ofLogError("The file " + filePath + " is missing");
                PieceNum++;
                continue;
            }
            
            ofBuffer buffer(file);
            //ofBuffer buffer = ofBufferFromFile(filePath);
            int lineNum = 0;
            
            //Read file line by line
            while (!buffer.isLastLine()) {
                string line = buffer.getNextLine();
                
                //Split line into strings
                vector<string> words = ofSplitString(line, ",");
                
                if(words.size()!=1){
                    if (lineNum<=3) {
                        for (int i=0; i < 8; i++) {
                            Piece[PieceNum].note[lineNum*8+i] = std::atoi(words[i].c_str());
                            cout<<Piece[PieceNum].note[i];
                        }
                    }else if(lineNum<=8){
                        for (int i=0; i < 8; i++) {
                            Piece[PieceNum].note_off[(lineNum-5)*8+i] = std::atoi(words[i].c_str());
                            cout<<Piece[PieceNum].note_off[i];
                        }
                    }else if(lineNum<=13){
                        for (int i=0; i < 8; i++) {
                            Piece[PieceNum].velocity[(lineNum-10)*8+i] = std::atoi(words[i].c_str());
                            cout<<Piece[PieceNum].velocity[i];
                        }
                    }
                }
                
                lineNum++;
                cout<<"line"<<lineNum<<endl;
            }
            Piece[PieceNum].channel = channel;
            cout<<"channel"<<Piece[PieceNum].channel<<endl;
            for(int x = 0; x < 300; x++ ){
                for(int y = 0; y <300; y++){
                    Piece[PieceNum].setting_image[x][y] = 0;
                }
            }
            PieceNum++;
        }
    }
    
    
    for (int i =0; i < PIECE_NUM_MAX_X; i++) {
        for (int j = 0; j < PIECE_NUM_MAX_Y; j++) {
            map[i][j] = 1;
        }
    }
    
    for (int i =0; i < PIECE_NUM_MAX_X; i++) {
        for (int j = 0; j < PIECE_NUM_MAX_Y; j++) {
            sequence[i][j] = Piece[0];
        }
    }
    sequence[12][0] = Piece[1];
    sequence[21][1] = Piece[2];
    sequence[13][2] = Piece[3];
    sequence[14][0] = Piece[1];
    sequence[15][1] = Piece[2];
    sequence[16][2] = Piece[3];
    map[12][0] = 1;
    map[21][1] = 2;
    map[13][2] = 3;
    map[14][0] = 1;
    map[15][1] = 2;
    map[16][2] = 3;
    sequence[12][10] = Piece[1];
    sequence[21][11] = Piece[2];
    sequence[13][11] = Piece[3];
    sequence[14][0] = Piece[1];
    sequence[15][6] = Piece[2];
    sequence[16][8] = Piece[3];
    map[12][10] = 1;
    map[21][11] = 2;
    map[13][11] = 3;
    map[14][0] = 1;
    map[15][6] = 2;
    map[16][8] = 3;
    sequence[2][0] = Piece[1];
    sequence[2][1] = Piece[2];
    sequence[3][2] = Piece[3];
    sequence[4][0] = Piece[1];
    sequence[5][1] = Piece[2];
    sequence[6][2] = Piece[3];
    map[2][0] = 1;
    map[2][1] = 2;
    map[3][2] = 3;
    map[4][0] = 1;
    map[5][1] = 2;
    map[6][2] = 3;
    sequence[6][0] = Piece[1];
    sequence[7][1] = Piece[2];
    sequence[8][2] = Piece[3];
    sequence[9][0] = Piece[1];
    sequence[10][1] = Piece[2];
    sequence[11][2] = Piece[3];
    map[6][0] = 1;
    map[7][1] = 2;
    map[8][2] = 3;
    map[9][0] = 1;
    map[10][1] = 2;
    map[11][2] = 3;
    sequence[6][0] = Piece[1];
    sequence[7][1] = Piece[2];
    sequence[8][2] = Piece[3];
    sequence[9][0] = Piece[1];
    sequence[10][1] = Piece[2];
    sequence[11][2] = Piece[3];
    map[6][0] = 1;
    map[7][1] = 2;
    map[8][2] = 3;
    map[9][0] = 1;
    map[10][1] = 2;
    map[11][2] = 3;
}

bool Board::set_map(int x, int y, int value){
    if (x < PIECE_NUM_MAX_X && y < PIECE_NUM_MAX_Y){
        map[x][y] = value;
        return true;
    }
    return false;
}

int Board::get_map(int x, int y){
    if (x < PIECE_NUM_MAX_X && y < PIECE_NUM_MAX_Y){
        return map[x][y];
    }
}

bool Board::map_select(int x, int y){
    if (x < PIECE_NUM_MAX_X && y < PIECE_NUM_MAX_Y){
        map[x][y] += 100;
        return true;
    }
    return false;
}
bool Board::map_unselect(int x, int y){
    int tmp = map[x][y];
    if (x < PIECE_NUM_MAX_X && y < PIECE_NUM_MAX_Y){
        map[x][y] -= 100;
        if(map[x][y] < 0){map[x][y]=tmp;}
        return true;
    }
    return false;
}

bool Board::set_sequence(int x, int y, piece value){
    if (x < PIECE_NUM_MAX_X && y < PIECE_NUM_MAX_Y){
        sequence[x][y] = value;
        return true;
    }
    return false;
}

bool Board::set_sequence(int x, int y, int value){
    if (x < PIECE_NUM_MAX_X && y < PIECE_NUM_MAX_Y){
        sequence[x][y] = Piece[value];
        return true;
    }
    return false;
}

bool Board::set_sequence(int x, int y, piece P, int value){
    if (x < PIECE_NUM_MAX_X && y < PIECE_NUM_MAX_Y){
        sequence[x][y] = Piece[value];
        for (int i = 0; i<300; i++) {
            for(int j=0; j<300; j++){
                sequence[x][y].setting_image[i][j] = P.setting_image[i][j];
            }
        }
        return true;
    }
    return false;
}

piece Board::get_sequence(int x, int y){
    if (x < PIECE_NUM_MAX_X && y < PIECE_NUM_MAX_Y){
        return sequence[x][y];
    }
}

int Board::current(){
    int tmp;
    if (current_index > PIECE_NUM_MAX_X -1){current_index = 0;}
    tmp = current_index;
    current_index ++;
    return tmp;
}

int Board::get_piece_num(){
    return PieceNum;
}
