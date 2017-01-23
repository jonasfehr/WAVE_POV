//
//  OscUtils.h
//  WAVE_POV
//
//  Created by Jonas Fehr on 23/01/2017.
//
//

#ifndef OscUtils_h
#define OscUtils_h

class OscReceiver{
    
    ofxOscReceiver receiver;
    
    void setup(int port){
        receiver.setup( port );

    }
    
    void update(){
        //PARSE OSC
        while( receiver.hasWaitingMessages() ){
            ofxOscMessage m;
            receiver.getNextMessage( &m );
            
            string msg_string;
            msg_string = m.getAddress(); //expect "/BeamBreak/[artnetaddr] [0-1]"
            msgTokens = ofSplitString(msg_string, "/", true); //ignore (leading) empty token = true
            
            if(msgTokens[0] == "Users"){
                
                //convert artnet string for easy array access
                int artnet = ofToInt(msgTokens[1]);
                //get value of BeamBreak, 0=false, 1=true
                int value = m.getArgAsInt32(0);
                
                //add trigger value and timestamp to sensor@artnetAddr
                //            gates[artnet].sensor.add(value,timeTriggered);
                if(value == 1){
                    gates[artnet].activate(); // activate gate
                }
                
                
                if(DEBUG){
                    string tempstr = "obj:";
                    tempstr += gates[artnet].sensor.toString();
                    cout << tempstr << "\n";
                    
                }
                msg_string += " value=";
                msg_string += ofToString(value);
                msg_string += " time=";
                msg_string += ofToString(ofGetElapsedTimeMillis());
            }
    }
        
        
    sender.setup( SENDHOST, SENDPORT );

    
};

#endif /* OscUtils_h */
