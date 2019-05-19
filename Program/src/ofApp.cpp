//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
// respons 63 bit
// -- -- --//
// aa 00 67
// 4 x10-x1b (1-12) how many characters? 
// 5 -6 - 7 xAB xCD xEF keypad Number
// 5 -17 (4+ number of chars format x3N -3*16
// 

#include "ofApp.h"

#define VENDOR_ID 1008
#define PRODUCT_ID 34836

ofxIO::HIDDevice _device;
int res=0;
std::vector<uint8_t> buffer;
bool semafore = true;
string message, keypad_Type;
typedef struct _OUTDATA {
    unsigned char   keypad_Type[6];
    unsigned int    numOfDigits;
    unsigned int    keypadID;
//    unsigned char   keypad_Type[numOfDigits];
} OUTDATA, *POUTDATA;

int i;

void ofApp::setup()
{
///    ofFile indexHTML(ofToDataPath("index.html")); ////, ofFile::Write); // file doesn't exist yet
///    indexHTML.create(); // now file exists
///    ofSetLogLevel(OF_LOG_VERBOSE);
    
//  auto devices = ofxIO::HIDDeviceUtils::listDevices();
    auto devices = ofxIO::HIDDeviceUtils::listDevicesWithVendorAndProductIds(VENDOR_ID, PRODUCT_ID);

    for (const auto& device: devices)
    {
        std::cout << device.toJSON().dump(4) << "\n" << std::endl;
        
///        ofxIO::HIDDevice _device;

        if (_device.setup(device))
        {
            std::cout << "Success!" << std::endl;
            break;
        }
        else
        {
            std::cout << "Failed!" << std::endl;
        }

    }

}

void ofApp::update() {
/*
        if (_device.isOpen()) {std::cout << "IsOpen!!!!" << std::endl;}
        else std::cout << "Failed." << std::endl;
        usleep(500);
        std::streamsize read(std::vector<uint8_t>& buffer,
                         std::size_t readBufferSize = DEFAULT_READ_BUFFER_SIZE);
        */
 //       while (res == 0) {
//        _device.read
		res = _device.read( buffer );
		if (res == 0)
///			printf("waiting...\r");
           res=0;
		else if (res < 0) {
			printf("Unable to read()\r");
            setup();
        }
/*
		else {
		// Print out the returned buffer.
		printf ("RECIVED: %i\n", res);
		for (i = 0; i < 10; i++)
		printf("%02hhx ", buffer[i]);
		printf("\n");
		}
//		res=0;
		#ifdef WIN32
		Sleep(500);
		#else
		usleep(500*1000);
		#endif
        */
//	}
}

void ofApp::exit()
{
}


void ofApp::draw()
{

    if ( res < 0  ) {
        if (semafore) {
            ofSetColor(255,0,0);
            ofDrawCircle(ofGetWindowWidth()/2, ofGetWindowHeight()/2, ofGetWindowWidth()/3);
            }
            #ifdef WIN32
		    Sleep(500);
		    #else
		    usleep(500*1000);
		    #endif
            semafore=!semafore;
    }
    else {
        unsigned int digits = 0, sender_ID = 0, keypad_No = 0;
        short int master_keypad = 0;
        long long int sent_value = 0;
        if ( res >= 1 ) {
    		// Print out the returned buffer.
///    		printf ("RECIVED: %i\n", res);  ///must be 63 !!!!
            message = "\n";
            keypad_Type = "";
            char str[res];
        	for (i = 0; i < res; i++) {
                if ( i == 3 ) {
                    digits = (int)buffer[i];
                    if ( digits >= 16 )
                        digits = digits -15;
                    else if ( digits == 4 ) {
                        master_keypad = 1;
                        digits = 1;
                    }
///                    else break;
               }
                /// KeyPadNumber.
                else if ( (4 <= i) && ( i <= 6 )) {
                    keypad_No = 256*keypad_No + ((int)buffer[i]);
                }
                else if ( (6 < i) && ( i < digits + 7 )) {
                    sent_value = 10*sent_value + (int)buffer[i];
                    if ( sent_value > 48 )
                        sent_value = sent_value - 3*16;
                }
                else if  ( (digits + 6 < i )) {
                    if ( ((int)buffer[i] != 0) && ((int)buffer[i] != 6*16+6) && !master_keypad ) { ///Not 00 nor 66;
                        sender_ID = 10*sender_ID + (int)buffer[i]-3*16;
                    }
                }
                //sprintf(str,"%02hhx ",buffer[i]);
                sprintf(str,"%02hhx ",buffer[i]);
                if ( i < 3 ){
                    keypad_Type += str;
                }
                message += str;
            }
///            message+="\n";
            cout << "\nsender_ID: " << sender_ID << endl;
            cout << "keypad_Type: " << keypad_Type << endl;            
            cout << "sent_value: " << sent_value << endl;
            cout << "digits: " << digits << endl;
            cout << "keypad_No: " << keypad_No << endl;
            cout << "master_keypad: " << master_keypad;/* << endl;*/ 
            cout << message;
		}
        
            ofSetColor(0);
            ofDrawBitmapString(message,100,100);
            ofLogVerbose(__func__, message);
    }
//        message.clear();
}
