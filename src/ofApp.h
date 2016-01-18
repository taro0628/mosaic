#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "board.h"

#define WINDOW_SIZE_X   800
#define WINDOW_SIZE_Y 600
const int piece_size_x = (WINDOW_SIZE_X-(PIECE_NUM_MAX_X+1)*5) / PIECE_NUM_MAX_X;
const int piece_size_y = (WINDOW_SIZE_Y-(PIECE_NUM_MAX_Y+1)*5) / PIECE_NUM_MAX_Y;
const int BPM = 60;

class ofApp : public ofBaseApp{
    
private:
    int sel_ch();
    void midi_reset();
    ofImage btn_close;
    int btn_close_position_x;
    int btn_close_position_y;
    int color_position_x;
    int color_position_y;
    int setting_image_x;
    int setting_image_y;
    int select_color;
    int before_mouse_x = -1;
    int before_mouse_y = -1;
    ofColor draw_color;
    ofColor main_color[16];
    int boad_x;
    int boad_y;
    piece image;
    void draw_line(int ix1, int iy1, int ix2, int iy2);
    void plot(int x, int y);
    const int MAJOR = 0;
    int filter[2][22] ={{0,
                                      60, 62, 64, 65, 67, 69, 71,
                                      72, 74, 76, 77, 79, 81, 83,
                                      84, 86, 88, 89, 91, 93, 95},
                                    {0,
                                      60, 62, 64, 65, 67, 69, 71,
                                      72, 74, 76, 77, 79, 81, 83,
                                      84, 86, 88, 89, 91, 93, 95}
    };
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void exit() ;
    
    int BPM_msec;
    int timer;
    int beat;
    int tmp_current;
    int current;
    bool is_setting;
    
    int window_pos_x;
    int window_pos_y;
    int set_window_size_x;
    int set_window_size_y;
    
    Board board;
    
    bool is_play = true;
    
    int sel_col();
    
    //MIDI用
    ofxMidiOut midiOut;
    int channel;
    int note, velocity;
};
