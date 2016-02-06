#include "PAnimal.h"

PAnimal::PAnimal(float x_,float y_,float wid_,float hei_){
		x=x_; y=y_; wid=wid_; hei=hei_;
		

		float hhead_portion=ofRandom(.3,.5);
		head_wid=hhead_portion*wid;
		body_wid=wid-head_wid;
		
		body_hei=(hei)*ofRandom(.3,.5);
		float vhead_portion=ofRandom(.4,.8);
		head_hei=vhead_portion*body_hei;
		
		leg_hei=hei-body_hei;

		mleg=(int)ofRandom(3,6);
		leg_wid=body_wid/mleg*ofRandom(.2,.4);
		
		for(int i=0;i<mleg;++i){
			leg_span.push_back(ofRandom(0.05,0.5));
		}


		fcolor=ofColor(ofRandom(100,255),ofRandom(20,80)+100,ofRandom(20,85));
		
		phi=ofRandom(HALF_PI);
		
		
		_cur_mode=0;
		_dest_mode=0;
		elastic_stage=0;
		
		wheel_vel=27;
		skate_vel=60;
		run_vel=50;

		v_jump_vel=ofRandom(10,24);
		s_body_hei_incre=0;

		_src_ang=0;
		_dest_ang=0;

		
		land_length=(int)ofRandom(20,50);//(int)(ofRandom(.5,1.2)*width);
		for(int i=0;i<land_length;++i){
				land_poses.push_back(ofRandom(-.2,.2));
		} 
		land_vel=ofRandom(4,6);
		land_index=0;
		_move_vel=ofVec2f(land_vel/10.0,0);


		elastic_strength=(float)land_vel/10*ofRandom(1,3);


		sktb=SkateBoard(0,body_hei+leg_hei/4*3,wid*ofRandom(.5,.8),hei*ofRandom(.1,.4));

		
		wheels[0]=FireWheel(wid/4,hei,hei*ofRandom(.3,.5));
		wheels[1]=FireWheel(wid/4*3,hei,hei*ofRandom(.3,.5));


		_anim_mode_change=FrameAnimation(ofRandom(40,80),ofRandom(0,80));
		_calib_vel=ofVec2f(0,0);


	}	

void PAnimal::draw(bool draw_fill){
		
		
		ofPushStyle();
		
		if(draw_fill){
		 	
		 	ofSetColor(fcolor);
		 	
		}else{
			ofNoFill();
			ofSetColor(ofColor::black);			
		}
		
		float draw_portion=1.5*abs(sin((float)ofGetFrameNum()/(120/land_vel)));



		float elastic_portion=.6+elastic_strength*abs(sin((float)ofGetFrameNum()/(180/land_vel)+phi));
		
		int new_elastic_stage=(int)(fmod((float)ofGetFrameNum()/(180/land_vel)+phi,PI)/(PI/2));

		float cur_body_wid=(_cur_mode==0)?body_wid*elastic_portion:body_wid;
		if(_cur_mode==0)
			if(elastic_stage==1 && new_elastic_stage==0) x+=body_wid*(0.6+elastic_strength)-body_wid*.6;
		elastic_stage=new_elastic_stage;


		
		int jump_stage=0;//sship.stage;//(int)((jump_portion)/(PI/2));
		float jump_pos_y=0;
		float jump_pos_x=0;
		
		s_body_hei_incre=0;

	
		

		ofPushMatrix();

		ofTranslate(x,y);
		
		ofTranslate(body_wid/2,body_hei);
		ofRotate(ofLerp(_src_ang,_dest_ang,_anim_mode_change.GetPortion())*RAD_TO_DEG);
		ofTranslate(-body_wid/2,-body_hei);

		
		if(_dest_mode!=_cur_mode){
			float t=_anim_mode_change.GetPortion();
			switch(_dest_mode){
				case 1:
					sktb.draw(-ofGetWidth()*(1-t),0,draw_fill);
					break;
				case 2:
					wheels[0].setPos(first_foot_base);
					wheels[1].setPos(last_foot_base);
					if(draw_fill)
						for(auto& wheel:wheels) wheel.draw(draw_fill,-ofGetWidth()*(1-t),0);
					break;
			}
			ofTranslate(0,-body_hei*abs(sin(t*TWO_PI*2)));
		}else{
			switch(_cur_mode){
			case 0:
				if(elastic_stage!=0) ofTranslate(body_wid*(0.6+elastic_strength)-cur_body_wid,0);
				break;
			case 1:			
				sktb.draw(draw_fill);	
				break;
			case 2:
				wheels[0].setPos(first_foot_base);
				wheels[1].setPos(last_foot_base);
				float frame_portion=(float)ofGetFrameNum()/(wheel_vel/land_vel)+phi;
				int stage=(int)(fmod(frame_portion,TWO_PI)/(PI/2));
				if(stage>1 && draw_fill)				
					for(auto& wheel:wheels) wheel.draw(draw_fill);
			
				break;
			}
		}

		

		ofBeginShape();
		ofVertex(0,0+s_body_hei_incre);
		
		ofBezierVertex(cur_body_wid/3,-body_hei*.3*draw_portion+s_body_hei_incre,
						 cur_body_wid/3*2,-body_hei*.3*draw_portion+s_body_hei_incre,
						 cur_body_wid,0);


		ofVec2f ear_base1=ofVec2f(head_hei,0);
		ear_base1.rotate(((-PI/3)*draw_portion-PI/6-PI/4)*RAD_TO_DEG);
		ofVec2f ear_base2=ofVec2f(head_hei,0);
		ear_base2.rotate((-PI/3*draw_portion-PI/4)*RAD_TO_DEG);
		
		ofBezierVertex(cur_body_wid+ear_base1.x,ear_base1.y,
					  cur_body_wid+ear_base2.x,ear_base2.y,
					  cur_body_wid,0);

		ofBezierVertex(cur_body_wid+head_wid/2,-body_hei*.1*draw_portion,
					 cur_body_wid+head_wid,head_hei+body_hei*.4*draw_portion,
					 cur_body_wid,head_hei);
	
		ofVertex(cur_body_wid*(1-(float)(0+leg_span[0])/mleg),body_hei);

		for(int i=0;i<mleg;++i){
			if(i>0) ofVertex(cur_body_wid*(1-(float)(i+leg_span[i])/mleg),body_hei);

			if(_cur_mode!=_dest_mode){
				drawJumpLeg(cur_body_wid*(1-(float)(i+leg_span[i])/mleg),body_hei,leg_wid,leg_hei,i);
				continue;	
			} 

			if(_cur_mode==1) drawLegOnSkateBoard(cur_body_wid*(1-(float)(i+leg_span[i])/mleg),body_hei,leg_wid,leg_hei,i);
			else if(_cur_mode==0) drawRunLeg(cur_body_wid*(1-(float)(i+leg_span[i])/mleg),body_hei,leg_wid,leg_hei,i);
			else if(_cur_mode==2) drawWheelLeg(cur_body_wid*(1-(float)(i+leg_span[i])/mleg),body_hei,leg_wid,leg_hei,i);
		
		}
		
		ofBezierVertex(-cur_body_wid*.1*draw_portion,body_hei,
					 -cur_body_wid*.1*draw_portion,0,
					 0,0+s_body_hei_incre);


		ofEndShape();
		if(!draw_fill){
			ofSetColor(fcolor.r/1.1,fcolor.g/1.1,fcolor.b/1.3);
			ofNoFill();
			ofBeginShape();
			for(int i=0;i<50;++i){
					// pg.strokeWeight(ofRandom(2));
					float n=ofNoise((i+ofGetFrameNum()+land_vel));
					float n2=ofNoise(i*land_vel);
					ofVertex(n*cur_body_wid,
							   ofMap(n2,0,1,-body_hei*.1,body_hei/2));
			
			}
			ofEndShape();
		}

		if(_dest_mode!=_cur_mode){
			float t=_anim_mode_change.GetPortion();
			switch(_cur_mode){
				case 1:
					if(draw_fill) sktb.draw(ofGetWidth()*t,0,draw_fill);
					break;
				case 2:
					if(!draw_fill)
						for(auto& wheel:wheels) wheel.draw(draw_fill,-ofGetWidth()*t,0);
					break;
			}
		}else{
			switch(_cur_mode){
				case 0:
					if(draw_fill) drawLand();
					break;
				case 2:
					if(!draw_fill){
						for(auto& wheel:wheels) wheel.draw(draw_fill);
					}
					break;
			}
		}

		ofDrawBitmapString(ofToString(_cur_mode)+" "+ofToString(_dest_mode),0,0);

		ofPopMatrix();
		

		ofPopStyle();

		
		//update();
	}

void PAnimal::update(float ctrl_){

		if(_cur_mode==_dest_mode){
			float a=(float)ofGetFrameNum()/land_vel+phi;

			float sp1_=ofMap(ctrl_,0,1024,.5,3);
			//float sp2_=map(_control_val[2],0,1024,0.5,3);

			x+=(_move_vel.x)*abs(sin(a)*.3+.8)*sp1_+_calib_vel.x;
			y+=(_move_vel.y)*abs(sin(a)*.3+.8)*sp1_+_calib_vel.y;		
		}else{
			if(_anim_mode_change.GetPortion()==1) _cur_mode=_dest_mode;		
		}
		_anim_mode_change.Update();
		
}
void PAnimal::checkSpeed(float ctrl_,ofVec2f mid_){
		
		_calib_vel.scale(0);

		ofVec2f c(mid_.x-x,mid_.y-y);
		ofVec2f dir(land_vel,0);
		if(_cur_mode==1) dir.rotate(PI/4*RAD_TO_DEG);
		else if(_cur_mode==2) dir.rotate(-PI/8*RAD_TO_DEG);

		float sp_=ofMap(ctrl_,0,1024,5,.5);
		//dir.setMag(constrain(c.mag(),0,1.5));
		dir.scale(sp_);

		float dot=c.dot(dir);
		
		if(dot>ofGetWidth()/4) _calib_vel+=dir;
		if(dot<-ofGetWidth()/4) _calib_vel-=dir;
		 

		// if(_cur_mode!=0) c.setMag(constrain(c.mag(),0,1));
		// else c.setMag(0);
		
		//_calib_vel=c.get();
		
	}
void PAnimal::checkCollide(float ctrl_,vector<ofVec2f> pos_){
		
		for(ofVec2f p:pos_){
			//println(dist(p.x,p.y,x,y));
			if(ofDist(p.x,p.y,x,y)<60){
				ofVec2f dir(x-p.x,y-p.y);
				dir.scale(ofMap(ctrl_,0,1024,0.5,5));
				_calib_vel+=dir;
			}
		}

	}

void PAnimal::changeMode(int mode_){
		_move_vel=ofVec2f(land_vel,0);

		_src_ang=ofLerp(_src_ang,_dest_ang,_anim_mode_change.GetPortion());
		_anim_mode_change.Restart();

		switch(mode_){
			case 0:
				_dest_ang=0;
				break;
			case 1:
				_move_vel.rotate(PI/4*RAD_TO_DEG);
				_dest_ang=PI/4;
				break;
			case 2:
				_move_vel.rotate(-PI/8*RAD_TO_DEG);
				_dest_ang=-PI/8;
				break;
		}

		_dest_mode=mode_;

	}

	void PAnimal::drawRunLeg(float lx,float ly,float lw,float lh,int leg_index){


		float frame_portion=(float)ofGetFrameNum()/(run_vel/land_vel)+phi;
		int stage=(int)(fmod(frame_portion,TWO_PI)/(PI/2));
		
		float kang=-PI/2.5*(sin(frame_portion));
		if(leg_index%2==1) kang=-PI/2.5*(sin(frame_portion+PI/2));
		float fang=0;//kang+PI/1.8;
		
		switch(stage){
			case 0:
				fang=kang+PI/1.6;//*(sin(frame_portion));
				break;
			case 1:
				fang=kang+PI/1.6*((sin(frame_portion)));
				break;
				//println(stage+"  "+((sin(frame_portion))));
			case 2:
				//kang=PI/3;//*(sin(frame_portion));
				fang=kang;//PI/1.6*(sin(frame_portion));
				//ang=kang+PI/1.6;//*abs(sin(frame_portion%TWO_PI*2));
				break;
			
			case 3:
				float ang=fmod(frame_portion,TWO_PI)-PI/2*3;
				//kang=PI/3*(sin(ang*2+PI/2*3));
				fang=kang+PI/1.6*(1-abs(sin(frame_portion)));
				break;
				
		}
		drawLegs(kang,fang,lx,ly,lw,lh,leg_index);
		
	}	
void PAnimal::drawLegs(float kang,float fang,float lx,float ly,float lw,float lh,int leg_index){	
	ofVec2f knee_base(0,lh/2);
	knee_base.rotate(kang*RAD_TO_DEG);

	ofVec2f foot_base(0,lh/2);
	foot_base.rotate(fang*RAD_TO_DEG);
		

	ofVec2f foot_base2(-lw,lh/2);
	foot_base2.rotate(fang*RAD_TO_DEG);
		
		

	foot_base+=knee_base;
	foot_base2+=knee_base;
		
	ofBezierVertex(lx+knee_base.x,ly+knee_base.y,
					lx+foot_base.x,ly+foot_base.y,
					lx+foot_base.x,ly+foot_base.y);
	// vertex(lx+knee_base.x,ly+knee_base.y);
	// vertex(lx+foot_base.x,ly+foot_base.y);
		
	ofVertex(lx+foot_base2.x,ly+foot_base2.y);

	// vertex(lx-lw+knee_base.x,ly+knee_base.y);
	// vertex(lx-lw,ly);

	ofBezierVertex(lx-lw+knee_base.x,ly+knee_base.y,
					lx-lw+knee_base.x,ly+knee_base.y,
					lx-lw,ly);

	switch(_cur_mode){
		case 1:
			if(leg_index==0) first_foot_base=ofVec2f(lx-lw+foot_base.x,ly+foot_base.y);
			else if(leg_index==mleg-1) last_foot_base=ofVec2f(lx+lw+foot_base.x,ly+foot_base.y);
			break;
		case 2:
			if(leg_index==0) first_foot_base=ofVec2f(lx-lw+foot_base.x,ly+lh);
			else if(leg_index==mleg-1) last_foot_base=ofVec2f(lx+lw+foot_base.x,ly+lh);
			break;
		
	}
}
void PAnimal::drawLegOnSkateBoard(float lx,float ly,float lw,float lh,int leg_index){


	float frame_portion=(float)ofGetFrameNum()/(skate_vel/land_vel)+phi;
	int stage=(int)(fmod(frame_portion,TWO_PI)/(PI/2));
		
	
	float fang=PI/8;
	if(leg_index>(mleg-1)/2) fang-=-PI/10*abs(sin(frame_portion));//PI/6*(sin(frame_portion+PI/2));
	float kang=fang;

	drawLegs(kang,fang,lx,ly,lw,lh,leg_index);
			


}
void PAnimal::drawLand(){
	ofPushStyle();
	ofNoFill();
	ofSetColor(0,120);

	int len=land_poses.size();
	ofTranslate(-len*5,hei);
		

	ofBeginShape();
		
	for(int x=0;x<10;++x){
		for(int i=0;i<len;++i) ofVertex(i+x*len,land_poses[(i+land_index)%len]);
	}
	// for(int i=len;i>=0;--i){
	//  if(i<0 || i>=land_poses.size()) continue;
	//  ofVertex(i,land_poses.get((i+land_index)%len));
	// }
	ofEndShape();
		
	ofPopStyle();
		
	land_index+=land_vel;

	
}
	

void PAnimal::drawWheelLeg(float lx,float ly,float lw,float lh,int leg_index){

	float frame_portion=(float)ofGetFrameNum()/(wheel_vel/land_vel)+phi;
	int stage=(int)(fmod(frame_portion,TWO_PI)/(PI/2));
		
	float kang=-PI/2*(sin(frame_portion));
	if(leg_index%2==1) kang=-PI/2*(sin(frame_portion+PI/4));
	float fang=0;//kang+PI/1.8;
		
	switch(stage){
		case 0:
			fang=kang+PI/1.6;
			break;
		case 1:
			fang=kang+PI/1.6*((sin(frame_portion)));
			break;
		case 2:
			fang=kang;
			break;
		case 3:
			fang=kang+PI/1.6*(1-abs(sin(frame_portion)));
			break;
				
	}
		
	drawLegs(kang,fang,lx,ly,lw,lh,leg_index);

}

void PAnimal::drawJumpLeg(float lx,float ly,float lw,float lh,int leg_index){

	float frame_portion=(float)ofGetFrameNum()/(land_vel/2)+phi;
	int stage=(int)(fmod(frame_portion,TWO_PI)/(PI/2));
	stage%=2;
	bool left=(leg_index>(mleg-1)/2);
	float aoffset=0;//((left)?-PI/6:PI/6);
		
	float kang=0;//-PI/2*(sin(frame_portion));
	float fang=0;//kang+PI/1.8;
		
	frame_portion+=ofRandom(-.1,.1);
	if(left) kang=PI/3*(sin(frame_portion));
	else kang=-PI/3*(sin(frame_portion));
	fang=kang+ofRandom(-1,1)*PI/8;



	drawLegs(kang,fang,lx,ly,lw,lh,leg_index);

}


bool PAnimal::operator < (const PAnimal& pa) const{
	return y<pa.y;
}
