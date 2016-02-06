#ifndef SKATE_BOARD_H
#define SKATE_BOARD_H

class SkateBoard{

	public:
	float x,y,wid,hei;
	float wheel_rad;
	float board_rad;

	int mwind;//=(int)ofRandom(2,5);

	SkateBoard(){}
	SkateBoard(float x_,float y_,float wid_,float hei_){
		x=x_; y=y_; wid=wid_; hei=hei_;
		wheel_rad=ofRandom(.2,.5)*hei;
		board_rad=ofRandom(.2,.6)*wid;
		mwind=ofRandom(2,5);
	}
	void draw(bool draw_fill){
		draw(0,0,draw_fill);
	}
	void draw(float x_,float y_,bool draw_fill){

		float draw_portion=abs(sin((float)ofGetFrameNum()/80));
		ofPushStyle();
		if(draw_fill){
			ofSetColor(255);
		}else{
			ofSetColor(0);
			ofNoFill();
		}


		//pg.stroke(0);

		ofPushMatrix();
		ofTranslate(x+x_,y+y_);

		ofBeginShape();
			ofVertex(0,0);
			ofBezierVertex(-board_rad,0,-board_rad*1.3,hei,
						 0,hei);
			ofBezierVertex(wid/3,hei+hei*.1*draw_portion,wid/3*2,hei+hei*.1*draw_portion,
						 wid,hei);
			ofBezierVertex(wid+board_rad*1.3,hei,wid+board_rad,hei*.1*draw_portion,
						 wid,0);
			
			ofBezierVertex(wid/3*2,hei*.1*draw_portion,wid/3*2,-hei*.1*draw_portion,
						 0,0);
		ofEndShape();

		drawWheel(0,hei+wheel_rad/2);
		drawWheel(wid,hei+wheel_rad/2);

		if(!draw_fill) drawWind(-wid/2-wid/2.5,hei*1.5,wid/3,hei/2);

		ofPopMatrix();

		ofPopStyle();
		
	}
	void drawWheel(float wx,float wy){
		ofPushMatrix();
		ofTranslate(wx,wy);
			for(int i=0;i<3;++i){
				float rad=wheel_rad/2;//-wheel_rad*i/10;
				ofRotate(((float)ofGetFrameNum()/2+i/2)*RAD_TO_DEG);
				ofBeginShape();
					ofVertex(0,-rad/2);
					ofBezierVertex(-rad,-rad/2,-rad,rad/2,
								 0,rad/2);
					ofBezierVertex(rad,rad/2,rad,-rad/2,
								 0,-rad/2);
				ofEndShape();
			}
		ofPopMatrix();
	}

	void drawWind(float wx,float wy,float wwid,float whei){
		ofNoFill();
		ofPushMatrix();
			ofTranslate(wx,wy);
			whei/=(float)3;
			for(int i=0;i<mwind;++i){
				wwid*=ofRandom(.5,2);
				ofTranslate(wwid*.3*sin((float)ofGetFrameNum()),-whei);
				ofBeginShape();
					ofVertex(wwid,0);
					ofBezierVertex(wwid*ofRandom(.1,.5),whei*.1*ofRandom(-1,1),
								 wwid*ofRandom(.6,.9),whei*.1*ofRandom(-1,1),
								 0,0);

					// PVector ctrl=new PVector(-whei/2,0);
					// ctrl.rotate(PI*sin((float)frameCount));
					// ofBezierVertex(-wwid/4*ofRandom(.5,1.5),0,
					// 			ctrl.x,ctrl.y-whei/4,
					// 			0,-whei/4);
					
				ofEndShape();
				drawArc(0,-wwid/8,wwid/4,wwid/4,PI,TWO_PI);//PI/2,PI/4+PI/2*3*sin((float)ofGetFrameNum()/2));
				
			}


		ofPopMatrix();
	}
	
	void drawArc(float ax,float ay,float aw,float ah,float start_ang,float end_ang){
			
			float mvertex=(end_ang-start_ang)/PI*30;

			ofBeginShape();
				for(int i=0;i<mvertex;++i){
					float ang=ofLerp(start_ang,end_ang,1.0/mvertex*i);
					ofVertex(ax+aw/2*sin(ang),ay+ah/2*cos(ang));
				}
			ofEndShape();
	}

};



#endif