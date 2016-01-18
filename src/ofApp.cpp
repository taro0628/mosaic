#include "ofApp.h"




//--------------------------------------------------------------
void ofApp::setup(){
	for(int x=0;x<PIECE_NUM_MAX_X;x++){
		for(int y=0;y<PIECE_NUM_MAX_Y;y++){
			int value = ofRandom(1, 80);
			int c = ofRandom(0, 16);
						if(c==6||c==8){c=9;}
			if(value<=16){
			board.set_map(x, y, value);
			board.set_sequence(x, y, c*15+value);
			}
		}
	}
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetWindowShape(WINDOW_SIZE_X, WINDOW_SIZE_Y);
    ofBackground(33, 33, 33);
    BPM_msec = 60 / BPM;
    timer = 0;
    beat = 0;
	is_setting = false;
	window_pos_x = 100;
	window_pos_y = 100;
	set_window_size_x = 600;
	set_window_size_y = 400;
	btn_close_position_x = window_pos_x+set_window_size_x-80;
	btn_close_position_y = window_pos_y+ 20;
	color_position_x = window_pos_x+ 30;
	color_position_y = set_window_size_y+window_pos_y - 50;
	setting_image_x =  window_pos_x+30;
	setting_image_y = window_pos_y+30;
	select_color =1;
	main_color[0].set(255, 255, 255);
	main_color[1].setHex(0xf39700);
	main_color[2].setHex(0xe60012);
	main_color[3].setHex(0x9caeb7);
	main_color[4].setHex(0x00a7db);
	main_color[5].setHex(0x009944);
	main_color[6].setHex(0xd7c447);
	main_color[7].setHex(0x9b7cb6);
	main_color[8].setHex(0x00ada9);
	main_color[9].setHex(0xbb641d);
	main_color[10].setHex(0xe85298);
	main_color[11].setHex(0x0079c2);
	main_color[12].setHex(0x6cbb5a);
	main_color[13].setHex(0xb6007a);
	main_color[14].setHex(0xe5171f);
	main_color[15].setHex(0x451c1d);
	
    //MIDIポートを開く
    midiOut.openPort(0); // by number
    channel = 1;
    note = 0;
    velocity = 0;
    current = 0;
	
	btn_close.loadImage("close.png");
}

//--------------------------------------------------------------
void ofApp::update(){
    if (timer > BPM_msec *60 / 8){timer=0;}
    if (beat >=32){beat=0;}
    if (timer == 0){
        //小節の頭で表示をひとつ進める
        if(beat == 0  && is_setting == false){
            current = board.current();
            int back_current = current -1;
            if (back_current ==  -1) {
                back_current = PIECE_NUM_MAX_X-1;
            }
			
            for (int y = 0; y< PIECE_NUM_MAX_Y; y++){
                board.map_unselect(back_current, y);
                board.map_select(current, y);
            }
        }
		
        for (int y=0; y<PIECE_NUM_MAX_Y; y++) {
            piece sequence = board.get_sequence(current, y);
            int note = filter[MAJOR][sequence.note[beat]];
            int note_off = filter[MAJOR][sequence.note_off[beat]];
            int velocity = sequence.velocity[beat];
            channel = sequence.channel;
            
            if(note_off != 0){
				for(int i=0;i<10;i++){
                midiOut.sendNoteOff(channel, note_off);
				}
            }
            if(note != 0){
                midiOut.sendNoteOn(channel, note,  velocity);
				if(channel==12){
				ofLogNotice("beat")<<beat;
                ofLogNotice("note")<<note;
				}
            }
            
        }
		if (is_play){
			beat ++;
		}
		
    }
	if (is_play) {
		timer ++;
	}
	
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    //Boardを描画
    for (int x =0; x < PIECE_NUM_MAX_X; x++) {
        for (int y = 0; y < PIECE_NUM_MAX_Y; y++) {
			ofSetColor(main_color[board.get_map(x, y) -1]);
			if (board.get_map(x, y)>=100) {
                ofSetColor(50, 55, 69);
            }
            ofRect(x*(piece_size_x+5)+5, y*(piece_size_y+5)+5, piece_size_x, piece_size_y);
        }
    }
	//設定画面を表示
	if(is_setting == true){
		ofSetColor(200, 200, 200);
		ofRect(window_pos_x, window_pos_y , set_window_size_x, set_window_size_y);
		
		btn_close.draw(btn_close_position_x, btn_close_position_y);
		
		for (int x = 0; x < 300; x++) {
			for(int y = 0; y < 300; y++){
				ofSetColor(main_color[image.setting_image[x][y]]);
				ofRect(setting_image_x + x, setting_image_y + y, 1, 1);
			}
		}
	
		for(int i = 0; i < 16; i++){
			ofSetColor(main_color[i]);
						ofRect(i * 30+color_position_x, color_position_y, 30, 30);
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == ' '){
		if (is_play){
			is_play = false;
			midi_reset();
		}else{
			is_play = true;
		}
	}
	if (key == OF_KEY_RETURN){
		midi_reset();
		if (is_setting){
			for (int x=0; x<300; x++) {
				for (int y=0; y<300; y++) {
					image.setting_image[x][y] = 0;
				}
			}

			board.set_map(boad_x, boad_y, 1);
			board.set_sequence(boad_x, boad_y, image, 1);
	}
	}
	
	
}

void ofApp::midi_reset(){
	for (int channel=1; channel<=16; channel++) {
		for(int j=0;j<1;j++){
		for(int i=60; i<95; i++){
			midiOut.sendNoteOff(channel, i);
			ofSleepMillis(1);
		}
		}
	}
}

//--------------------------------------------------------------
void ofApp::exit() {
    // clean up
	midi_reset();
	for(int i=0;i<10;i++){
    for (int channel=1; channel<=16; channel++) {
        for(int i=1; i<126; i++){
			midiOut.sendNoteOff(channel, i);
        }
		for(int i=1; i<126; i++){
			midiOut.sendNoteOff(channel, i);
			ofSleepMillis(1);
			midiOut.sendNoteOn(channel, i, 0);
			ofSleepMillis(1);
			midiOut.sendNoteOff(channel, i);
		}
		
    }
	}
    midiOut.closePort();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	if (is_setting) {
		//描画領域
		int image_x, image_y;
		if(setting_image_x < x && x < setting_image_x + 300 && setting_image_y < y && y < setting_image_y + 300){
					image_x = x - setting_image_x ;
					image_y = y - setting_image_y;
					if(image_x < 0){image_x=0;}
					if(image_y < 0){image_y=0;}
					if(300<image_x){image_x=300;}
					if(300<image_y){image_y=300;}
					if (before_mouse_x != -1 && before_mouse_y != -1) {
						draw_line(before_mouse_x, before_mouse_y, image_x, image_y);
					}
					before_mouse_x = image_x;
					before_mouse_y = image_y;
			
			int set = sel_col();
			int ch=sel_ch();

			ofLogNotice("set")<<set;
			ofLogNotice("ch")<<ch;
			if(ch==6||ch==8){ch=9;}
			board.set_map(boad_x, boad_y, set + 1);
			board.set_sequence(boad_x, boad_y, image, ch*16 + set);
		}
	}
	
}

int ofApp::sel_col(){
	int c=0;
	int i=0;
	for (int x=0; x<300; x++) {
		for (int y=0; y<300; y++) {
			if (image.setting_image[x][y]!=0) {
			c += image.setting_image[x][y];
				i++;
			}
		}
	}
	return (int)c/(i+1);

}
int ofApp::sel_ch(){
	int c=0;
	for (int x=0; x<300; x++) {
		for (int y=0; y<300; y++) {
			if (image.setting_image[x][y]!=0) {
				c++;
			}
		}
	}
	return (int)c%15+1;
	
}


void ofApp::plot(int x, int y){
	for (int dx = -1; dx <= 1; dx++) {
		for (int dy = -1; dy <= 1; dy++) {
			int sx = x + dx;
			int sy = y + dy;
			if (sx<0) {sx=0;}
			if (sy<0) {sy=0;}
			if (300<sx) {sx=300;}
			if (300<sy) {sy=300;}
			image.setting_image[sx][sy] = select_color;
		}
	}
}

void ofApp::draw_line(int ix1, int iy1, int ix2, int iy2) {
	int x = ix1;
	int y = iy1;
	int dx = abs(ix2 - ix1);
	int dy = abs(iy2 - iy1);
	int sx = (ix2>ix1)?1:-1;
	int sy = (iy2>iy1)?1:-1;
	
	if (dx >= dy) {
		int err = 2*dy - dx;
		int i = 0;
		for(i=0; i<=dx; ++i) {
			plot(x,y);
			x += sx;
			err += 2*dy;
			if (err >= 0) {
				y += sy;
				err -= 2*dx;
			}
		}
	}else{
		int err = 2*dx - dy;
		int i = 0;
		for(i=0; i<=dy; ++i) {
			plot(x,y);
			
			y += sy;
			err += 2*dx;
			if (err >= 0) {
				x += sx;
				err -= 2*dy;
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

	if	(is_setting){
		
		int image_x = x - setting_image_x ;
		int image_y = y - setting_image_y;
		before_mouse_x = image_x;
		before_mouse_y = image_y;
	}

	//通常画面の場合
	if(!is_setting){
			midi_reset();
		boad_x = (int) x / (piece_size_x +5);
		boad_y = (int) y / (piece_size_y +5);
		tmp_current = current;
		current = boad_x;
		
		for (int y = 0; y< PIECE_NUM_MAX_Y; y++){
			board.map_unselect(tmp_current, y);
			board.map_select(current, y);
		}
		
		image = board.get_sequence(boad_x, boad_y);
		board.set_map(boad_x, boad_y, select_color + 1);
		board.set_sequence(boad_x, boad_y, image, boad_y*16 + select_color);
		midi_reset();
		is_setting=true;
	}
	
	//設定画面の場合
	if(is_setting){
		//closeボタンが押された時
		if(btn_close_position_x < x && x < btn_close_position_x + 50 && btn_close_position_y < y && y < btn_close_position_y + 50){
			midi_reset();
			for (int y = 0; y< PIECE_NUM_MAX_Y; y++){
				board.map_unselect(current, y);
				board.map_select(tmp_current, y);
			}
			current = tmp_current;

			is_setting = false;
			
		}
		//色の選択
		for(int i = 0; i < 16; i++){
			if (color_position_x + (i*30) < x && x < color_position_x + (i*30) +30 && color_position_y < y && y < color_position_y + 30) {
				select_color = i;
				draw_color.set(main_color[i]);
				board.set_map(boad_x, boad_y, i+1);
				board.set_sequence(boad_x, boad_y, i+1);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}