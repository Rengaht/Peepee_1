#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	ofHideCursor();

	_distort_fill=DistortGraph(20);
	_distort_fill.transform_mag=2.2;
//#ifdef TARGET_OPENGLES			
	ofDisableArbTex();
//#endif

	_fbo_fill.allocate(ofGetWidth()*_distort_fill.getWeight(),ofGetHeight()*_distort_fill.getWeight(),GL_RGBA);
	_fbo_stroke.allocate(ofGetWidth(),ofGetHeight(),GL_RGBA);

	_fbo_fill.begin();
	ofClear(255,255,255,0);
	_fbo_fill.end();

	_fbo_stroke.begin();
	ofClear(255,255,255,0);
	_fbo_stroke.end();

	_dimage.loadImage("cat.jpg");
//
//#ifdef TARGET_OPENGLES
//	_dshader.load("shaders_gles/dshader.vert","shaders_gles/dshader.frag");
//#else
//	_dshader.load("shaders/dshader.vert","shaders/dshader.frag");
//#endif

	M_CONTROL=5;
	M_MODE=3;
	
	mpas=5;

	last_mode=0;
	play_mode=-1;
	angle_for_mode=0;

	RoadColor[0]=ofColor(186,213,167);
	RoadColor[1]=ofColor(218,218,218);
	RoadColor[2]=ofColor(48,83,139);


	_anim_mode_change=FrameAnimation(50);
	
	initControl();
	initSound();
	initPAnimal();

	changeMode(0);
	
	ofEnableSmoothing();
	//ofEnableAntiAliasing();

	save_frame=false;
}

void ofApp::initPAnimal(){
	
	pas.clear();

	float tmp_pos=0;
	for(int i=0;i<mpas;++i){
		float tmp_h=ofRandom(0.6,1.2)*64;
		pas.push_back(PAnimal(0,tmp_pos+tmp_h,96*ofRandom(.6,1.4),tmp_h));
		tmp_pos+=tmp_h;
		if(tmp_h>ofGetHeight()) return;
	}
	mpas=pas.size();

}

		
		
void ofApp::initControl(){
	
	_control_val[0]=0;
	_control_val[1]=0;
	_control_val[2]=512;
	_control_val[3]=1024;
	_control_val[4]=900;
//	
//
//	_serial_ctrl.listDevices();
//
//#ifdef TARGET_WIN32
//	_serial_ctrl.setup(0,9600);
//#endif
//#if defined( TARGET_LINUX )
//		_serial_ctrl.setup("/dev/ttyACM0",9600);
//#endif

}

void ofApp::initSound(){

	_sound_bgm[0].loadSound("sound/PIPI_1_1.ogg");
	_sound_bgm[1].loadSound("sound/PIPI_1_2.ogg");
	_sound_bgm[2].loadSound("sound/PIPI_1_3.ogg");
	//for(int i=0;i<3;++i) _sound_bgm[i].setLoop(true);

	_sound_transfer[0].loadSound("sound/PIPI_1_transfer_1.ogg");
	_sound_transfer[1].loadSound("sound/PIPI_1_transfer_2.ogg");
	_sound_transfer[2].loadSound("sound/PIPI_1_transfer_3.ogg");

	_bgm_fade_in=FrameAnimation(40,120);
	_bgm_fade_out=FrameAnimation(20);
	
	_sound_vol=1.0;

}

//--------------------------------------------------------------
void ofApp::update(){
	
	 ofEnableAlphaBlending();
	
	for(auto& pa:pas) pa.update(_control_val[3]);
	_anim_mode_change.Update();
	if(_anim_mode_change.GetPortion()==1){
		last_mode=play_mode;
	}

	updateMidPos();
	_distort_fill.update();


	//update sound volume
	_bgm_fade_in.Update();
	_bgm_fade_out.Update();
	if(last_mode>-1 && last_mode!=play_mode){
		_sound_bgm[last_mode].setVolume(1-_bgm_fade_out.GetPortion());
		if(_bgm_fade_out.GetPortion()==1) _sound_bgm[last_mode].stop();
	}
	if(play_mode>-1) _sound_bgm[play_mode].setVolume(_sound_vol*_bgm_fade_in.GetPortion());
	//_sound_bgm[play_mode].setSpeed((_control_val[3]<512)?ofMap(_control_val[3],0,512,.5,1):ofMap(_control_val[3],512,1024,1,2));
	
	/*unsigned char bytesReturned[3];
	unsigned char bytesReadString[32];
	int nRead=0;
	int nTimesRead=0;
	int nBytesRead=0;

	memset(bytesReadString, 0, 32);
	memset(bytesReturned, 0, 3);
		
	while((nRead=_serial_ctrl.readBytes(bytesReturned,3))>0){
		unsigned char *endchar=(unsigned char*)memchr(bytesReturned,'!',3);
		if(endchar!=NULL){
			memcpy(bytesReadString+nBytesRead,bytesReturned,endchar-bytesReturned);
			break;
		}
		memcpy(bytesReadString+nBytesRead,bytesReturned,nRead);
		nBytesRead+=nRead;		
	};	
	cout<<ofToString(bytesReadString)<<endl;*/



	//string str=readSerialString(_serial_ctrl, '!');

	////cout<<"get: "<<str<<endl;
	//
	//
	//if(str.length()>0){
	//	string read_string=str;//ofToString(bytesReadString);
	//	

	//	vector<string> val=split(read_string,'|');
	//	

	//	if(val.size()==M_CONTROL){

	//		cout<<"get: "<<ofToString(val)<<endl;

	//		for(int i=0;i<val.size();++i){
	//			int n=ofToInt(val[i]);
	//			switch(i){
	//				case 0:
	//				case 1:
	//					if(n==0) triggerEvent(i);	
	//					_control_val[i]=n;
	//					break;
	//				case 2:
	//				case 3:
	//					_control_val[i]=n;
	//					break;
	//				case 4:
	//					if(abs(_control_val[4]-n)<=200){
	//						_sound_vol=ofMap(n,0,1024,0,2);
	//						if(play_mode>-1) _sound_bgm[play_mode].setVolume(_sound_vol);
	//						_control_val[i]=n;
	//					}
	//					break;
	//			}
	//		}
	//		//cout<<ofToString(_control_val)<<endl;
	//	}
	//}


	//if(ofGetFrameNum()%1201==1200) changeMode();
	
}
void ofApp::triggerEvent(int ev){
	switch(ev){
		case 0:
			changeMode();
			break;
		case 1:
			initPAnimal();
			changeMode(play_mode);
			break;
	}
}

void ofApp::updateMidPos(){
	//vector<ofVec2f> poses;
	tmp_poses.clear();
	for(int i=0;i<pas.size();++i){
		tmp_poses.push_back(ofVec2f(pas[i].x,pas[i].y));
	}


	ofVec2f tmp(0,0);
	for(PAnimal& pa:pas){
		tmp.x+=pa.x;
		tmp.y+=pa.y;
	}
	tmp*=(1.0/pas.size());

	if(mid_pos.x==0 && mid_pos.y==0){
		mid_pos=tmp;
		cout<<"hi";	
	}

	//float mid_constrain=5;
	// mid_pos.x+=constrain(tmp.x-mid_pos.x,0,mid_constrain);
	// mid_pos.y+=constrain(tmp.y-mid_pos.y,-mid_constrain,mid_constrain);

	mid_pos=tmp;

	
	
	for(auto& pa:pas){
		pa.checkSpeed(_control_val[2],mid_pos);
		pa.checkCollide(_control_val[2],tmp_poses);
	}
	
	//return poses;

}
void ofApp::changeMode(){
	changeMode((play_mode+1)%M_MODE);
}
void ofApp::changeMode(int mode){
	//fill_canvas.reset();

	//play_mode=(play_mode+1)%M_MODE;	
	
	if(play_mode!=-1 && last_mode!=play_mode) return;

	last_mode=play_mode;
	
	if(_sound_bgm[mode].isLoaded()){
		_bgm_fade_out.Restart();
		_bgm_fade_in.Restart();
		
		_sound_bgm[mode].play();
		_sound_bgm[mode].setPosition(ofRandom(1));
		_sound_bgm[mode].setVolume(_sound_vol);

		//cout<<_sound_bgm[mode].getPosition()<<endl;
		if(last_mode>-1) _sound_transfer[int(ofRandom(3))].play();
	}
	play_mode=mode;
	for(auto &pa:pas){
	 	pa.changeMode(play_mode);
	 	
	}			
	_anim_mode_change.Restart();
}

//--------------------------------------------------------------
void ofApp::draw(){
	

	_fbo_fill.begin();
		ofClear(255,255,255,0);
		ofPushMatrix();
		//ofTranslate(_distort_fill.getMargin()*ofGetWidth(),_distort_fill.getMargin()*ofGetHeight());
		ofTranslate(ofGetWidth()/2-mid_pos.x,ofGetHeight()/2-mid_pos.y);
		drawBackLine();
		for(auto& pa:pas) pa.draw(true);		
	
		ofPopMatrix();
	_fbo_fill.end();

	_fbo_stroke.begin();
		ofClear(255,255,255,0);
		ofPushMatrix();
		ofTranslate(ofGetWidth()/2-mid_pos.x,ofGetHeight()/2-mid_pos.y);

		for(auto& pa:pas) pa.draw(false);			
		ofPopMatrix();
	_fbo_stroke.end();

	/*ofBackground(ofColor::white);

	ofSetColor(255,255,255);*/
	//_fbo_fill.draw(0,0);
	//_fbo_fill.readToPixels(_dpixels);
	//_dimage.setFromPixels(_dpixels);

	ofPushMatrix();
		//ofTranslate(-_distort_fill.getMargin()*ofGetWidth(),-_distort_fill.getMargin()*ofGetHeight());

		_fbo_fill.getTextureReference().bind();
		_distort_fill._mesh.draw();
		_fbo_fill.getTextureReference().unbind();
		//_distort_fill._mesh.drawWireframe();
	ofPopMatrix();
	
	_fbo_stroke.draw(0,0);
	
	if(save_frame && ofGetFrameNum()%12==0) ofSaveFrame();


	/*ofPushStyle();
	ofSetColor(ofColor::red);		
		ofDrawBitmapString(ofToString(ofGetFrameRate()),20,20);
		for(int i=0;i<M_CONTROL;++i) ofDrawBitmapString(ofToString(_control_val[i]),20,30+10*i);
	ofPopStyle();*/




}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key){
		case 's':
			save_frame=!save_frame;
			//ofSaveFrame();
			break;
		case 'd':
			changeMode();
			break;
		
		case 'z':
			_distort_fill.reset();
			initPAnimal();
			changeMode(play_mode);
			
			break;
	}
}

#pragma region OF_CONTROL
//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

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

#pragma endregion

void ofApp::drawBackLine(){
	
	
	float ntheta=(play_mode==1)?PI/4:((play_mode==2)?-PI/8:0);
	float ltheta=(play_mode==1)?0:((play_mode==2)?PI/4:-PI/8);
	float theta=ofLerp(ltheta,ntheta,_anim_mode_change.GetPortion());

	int npos=pas.size();
	//Collections.sort(poses_,new CompareByY());
	sort(pas.begin(),pas.end());


	ofPushStyle();
	
	ofPushMatrix();
	ofTranslate(mid_pos.x,mid_pos.y);
	ofRotate(theta*RAD_TO_DEG);	
	ofTranslate(0,-mid_pos.y);

	//pg.noStroke();

	float cy=40/cos(theta);

	ofSetColor(238,129,62);
	ofRect(-ofGetWidth(),pas[0].y-cy/2,ofGetWidth()*2,cy/2);

	ofSetColor(129,162,92);
	ofRect(-ofGetWidth(),pas[0].y-cy*1.7,ofGetWidth()*2,cy*1.2);
	

	//pg.fill(lerpColor(RoadColor[(play_mode-1+3)%3],RoadColor[play_mode],_anim_mode_change.GetPortion()));
	float x=ofGetWidth()*2*(1-_anim_mode_change.GetPortion());
	for(int i=0;i<npos;++i){
		
		float h=(i+1<npos)?(pas[i+1].y-pas[i].y-1):ofGetHeight();

		ofSetColor(RoadColor[(play_mode-1+3)%3]);
		ofRect(-ofGetWidth(),pas[i].y,x,h);	
		ofSetColor(RoadColor[play_mode]);
		ofRect(-ofGetWidth()+x,pas[i].y,ofGetWidth()*2-x,h);	

		//println(poses_.get(i));
	}



	
	int num=floor(mid_pos.x/300);
	for(int i=0;i<5;++i){
		int k=num-(i-2);
		ofSetColor(0,0,0);
		ofDrawBitmapString(ofToString(k),k*300-mid_pos.x,pas[0].y-cy/2-10);


	}

	ofPopMatrix();

	ofPopStyle();

}

