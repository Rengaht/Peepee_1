#ifndef PANIMAL_H
#define PANIMAL_H

#include "ofMain.h"
#include "FrameAnimation.h"
#include "SkateBoard.h"
#include "FireWheel.h"

class PAnimal{

private:
	float head_wid,head_hei;
	float body_wid,body_hei;
	
	float leg_wid,leg_hei;
	int mleg;
	vector<float> leg_span;

	float tail_wid,tail_hei;
	ofColor fcolor;
	
	float phi;
	float elastic_strength;
	int elastic_stage;

	vector<float> land_poses;
	int land_length;
	float land_vel;
	int land_index;

	SkateBoard sktb;

	FireWheel wheels[2];


	ofVec2f _move_vel,_calib_vel;

	int _cur_mode;
	int _dest_mode;

	float wheel_vel;
	float skate_vel;
	float run_vel;

	float v_jump_dest_y;
	float v_jump_vel;

	float s_body_hei_incre;

	ofVec2f first_foot_base;
	ofVec2f last_foot_base;

	FrameAnimation _anim_mode_change;
	float _src_ang,_dest_ang;

	void drawRunLeg(float lx,float ly,float lw,float lh,int leg_index);
	void drawLegs(float kang,float fang,float lx,float ly,float lw,float lh,int leg_index);
	
	void drawLegOnSkateBoard(float lx,float ly,float lw,float lh,int leg_index);
	void drawLand();
	void drawWheelLeg(float lx,float ly,float lw,float lh,int leg_index);
	void drawJumpLeg(float lx,float ly,float lw,float lh,int leg_index);

	

public:
	float x,y,wid,hei;
	
	PAnimal(float x_,float y_,float wid_,float hei_);

	void draw(bool draw_fill);

	void update(float ctrl_);

	void checkSpeed(float ctrl_,ofVec2f mid_);
	void checkCollide(float ctrl_,vector<ofVec2f> pos_);

	void changeMode(int mode_);

	bool operator < (const PAnimal& pa) const;

};



#endif