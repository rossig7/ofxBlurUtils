//
//  ofxBlurUtils.cpp
//  Grow_1b
//
//  Created by Guido Corallo on 5/10/17.
//
//

#include <ofxBlurUtils.h>

void ofxBlurUtils::setup(int ww_, int hh_){
    ww = ww_;
    hh = hh_;
    
    ping.allocate(ww, hh, GL_RGB);
    pong.allocate(ww, hh, GL_RGB);
    
    ping.begin();
    ofClear(0);
    ping.end();
    
    pong.begin();
    ofClear(0);
    pong.end();
    
    //load shaders.
    blurX.load("../../../../../addons/ofxBlurUtils/shaders/shaderBlurX");
    blurY.load("../../../../../addons/ofxBlurUtils/shaders/shaderBlurY");
    maskShader.load("../../../../../addons/ofxBlurUtils/shaders/maskSh");
    
    offSet = 1.0;
    mode = 0;
    nPasses = 1;
    
}

void ofxBlurUtils::begin(){
    
    
    
    if(!bypass){
        
        if(isTiltShift){
            mask.begin();
            maskShader.begin();
            maskShader.setUniform1i("mode", maskMode);
            ofSetColor(255);
            ofDrawRectangle(0, 0, ww, hh);
            
            maskShader.end();
            mask.end();
            
        }
        
        
        pong.begin();
    }
    
    
    
}


void ofxBlurUtils::end(){
    
    
    
    if(!bypass){
        pong.end();
        ofSetColor(255);
        
        pong.draw(0,0);
        
        
        
        
        for(int i=0; i< nPasses+1 ; i++){
            float offsetChoice;
            if(mode == 0){
                offsetChoice = offSet;
            }
            else if(mode == 1){
                offsetChoice = i*offSet;
            }
            else if(mode == 2){
                offsetChoice = (nPasses-i)*offSet;
            }
            else if(mode == 3){
                offsetChoice = (i*i)*offSet;
            }
            
            
            
            ping.begin();
            ofClear(0,255);
            blurX.begin();
            pong.draw(0,0);
            blurX.setUniform1f("blurAmnt", offsetChoice);
            ofSetColor(255);
            blurX.end();
            ping.end();
            
            
            pong.begin();
            ofClear(0,255);
            blurY.begin();
            blurY.setUniform1f("blurAmnt", offsetChoice);
            ofSetColor(255);
            ping.draw(0,0);
            blurY.end();
            pong.end();
            
        }
        
        if(isTiltShift){
            mix.begin();
            pong.draw(0,0);
            mix.end();
            
            
            mix.getTexture().setAlphaMask(mask.getTexture());
            mix.draw(0,0);
            
        }
        else{
            pong.draw(0, 0);
        }
        
    }
    
    
    
    
    
}



void ofxBlurUtils::setMode(int mode_){
    
    mode = mode_;
    
}

void ofxBlurUtils::setNPasses(int nPasses_){
    
    nPasses = nPasses_;
    
}

void ofxBlurUtils::setOffset(float offset_){
    
    offSet = offset_;
    
}

void ofxBlurUtils::setBypass(bool bypass_){
    
    bypass = bypass_;
    
}

void ofxBlurUtils::setupTiltShift(){
    
    mask.allocate(ww, hh, GL_RGB);
    mix.allocate(ww, hh, GL_RGBA);
    
    mask.begin();
    ofClear(0);
    mask.end();
    
    mix.begin();
    ofClear(0);
    mix.end();
    
    isTiltShiftInit = true;
    isTiltShift = true;
    
    maskMode = 1;
    
}

