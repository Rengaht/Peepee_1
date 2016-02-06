#pragma once

#include "ofMain.h"
#include "StringSplit.h"
#include "DistortGraph.h"
#include "PAnimal.h"





class ofApp : public ofBaseApp{

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
		
		float _control_val[5];

		ofSoundPlayer  _sound_bgm[3],_sound_transfer[3];
		FrameAnimation _bgm_fade_out,_bgm_fade_in,_bgm_trans_out;
		float _sound_vol;
		

		ofSerial _serial_ctrl;

		ofColor RoadColor[3];

	private:
		int M_CONTROL;
		int M_MODE;

		int mpas;
		vector<PAnimal> pas;

		ofVec2f mid_pos;
		float dest_pos;


		bool save_frame;

		//DistortGraphic fill_canvas,stroke_canvas;

		int play_mode,last_mode;

		float angle_for_mode;

		FrameAnimation _anim_mode_change;

		ofFbo _fbo_fill,_fbo_stroke;
		DistortGraph _distort_fill;
		ofImage _dimage;
		ofPixels _dpixels;
		ofShader _dshader;
		
		void initPAnimal();
		void updateMidPos();
		vector<ofVec2f> tmp_poses;

		void changeMode();
		void changeMode(int mode);
		
		
		void initControl();
		void initSound();

		void triggerEvent(int ev);

		void drawBackLine();

	
};
