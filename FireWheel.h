#ifndef FIRE_WHEEL_H
#define FIRE_WHEEL_H

#include "ofMain.h"

class FireWheel{

public:
	float x,y,rad;
	int mcurve;
	float phi;
	float phi2;

	FireWheel(){}
	FireWheel(float x_,float y_,float rad_){
		x=x_; y=y_; rad=rad_;
		mcurve=(int)ofRandom(8,15);

		phi=ofRandom(TWO_PI);
		phi2=ofRandom(TWO_PI);
	}
	void setPos(ofVec2f pos){
		
		x=pos.x; y=pos.y;
	}
	void draw(bool draw_fill){
		draw(draw_fill,0,0);
	}
	void draw(bool draw_fill,float dx_,float dy_){
		float draw_portion=(sin((float)ofGetFrameNum()/5+phi));
		ofPushStyle();
		if(draw_fill){
			ofSetColor(255,phi/TWO_PI*255,0,255);
			
		}else{
			ofNoFill();
			ofSetColor(0,200);
		}

		ofPushMatrix();
		ofTranslate(x+dx_,y+dy_);
		float ang=(float)ofGetFrameNum()/4+phi;

			ofBeginShape();

				ofVertex(rad/2,0);
				float etheta=TWO_PI/(float)mcurve;
				for(int i=0;i<=mcurve;++i){
					float theta=i*etheta;
					float ofRandom_strength1=ofRandom(1.5,5);
					float ofRandom_strength2=ofRandom(1.5,5);
					
					if(theta>=PI/2 && theta<=PI/2*3){
						ofVertex(rad/2*cos(theta+etheta/2)-rad*ofRandom(1.5,(draw_fill)?4.5:2.5)*(1-abs(theta-PI)/PI*2),rad/2*sin(theta+etheta/2));
						ofVertex(rad/2*cos(theta)*ofRandom(0.5,2.5),rad/2*sin(theta));
					}else
						ofVertex(rad/2*cos(theta)*ofRandom(0.5,1.5),rad/2*sin(theta));
				}
			ofEndShape();

		if(!draw_fill){ 
			ofRotate(ang*RAD_TO_DEG);
			ofBeginShape();
				ofVertex(rad/2,0);
				for(int i=0;i<=mcurve;++i){
					float theta=i*etheta;
					ofVertex(rad/2*cos(theta),rad/2*sin(theta));
				}
			ofEndShape();
			
			//float etheta=TWO_PI/(float)mcurve;
			for(int i=0;i<=mcurve;++i){
				float theta=i*etheta;
				ofBezier(0,0,
						 rad/4*cos(theta+etheta/2),rad/4*sin(theta+etheta/2),
						 rad/4*cos(theta+etheta/2),rad/4*sin(theta+etheta/2),
						 rad/2*cos(theta)*ofRandom(.8,1),rad/2*sin(theta)*ofRandom(.8,1));

			}
		}
			
		ofPopMatrix();
		ofPopStyle();
	}

};

#endif

