#pragma once

#include "ofMain.h"
#include "ofxGLWarper.h"
#include "ofxOpenCv.h"
#include "ofxUI.h"
#include "ofxXmlSettings.h"
#include "ofxTimer.h"

#include "LIGHT_JUMPER_DEFINES.h"
#include "Singleton.h"

#include "grid/grid.h"
#include "botons/botoDonut.h"
#include "peces/pecaEmpty.h"
#include <string>

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void setupTutorial1();
        void updatePecesTut1();

        // APP
        Singleton *singleton; // PUNTUACIÓ jugador
        string all_scores = "";
        pantallesJoc pantallaJoc;
        string pantallaToString();

        void setVariablesIniciPartida(); // endreçat
        void drawPuntuacio();
        void drawEnd();
       // void drawBackground();
        ofImage fons_inici;
        ofImage boto_start;
        ofImage fons_scores;
        // CORNER PIN
		ofxGLWarper warper;

        // BOTONS
        botoDonut botoStart;
        botoDonut botoHighScores;
        botoDonut botoInstructions;
        botoDonut botoBack;
        botoDonut botoReturn;

        // GRID
		grid myGrid;

        // TEMPS DE JOC
        ofxTimer jocMinutsTimer;
        ofxTimer Tuto1Timer;
        float jocMinutsTimerSegonsLeft;
        int jocMinutsTimerMinuts;
        int jocMinutsTimerSegons;
        void drawTemps();
		ofColor saltingBlue;
		ofTrueTypeFont	saltingTypo;

		// TEMPS DE PANTALLA FINAL
		ofxTimer duradaTheEndTimer;
        bool revisat;
        //--------------------------------------------------------------
        // PUNTER
        ofImage punter;
		float punterWidthMig, punterHeightMig;

        // PECES
        int comptadorPeces;
        pecaEmpty peca1;
        vector<pecaEmpty> peces;
        vector<pecaEmpty> pecesPantalla;
        vector<pecaEmpty> pecesTut;
       //vector<botoEmpty> botonsTut;
        vector<pecaEmpty> pecesMovTut;


        void setupPeca1();
        void setupPeces();
        void updatePeces();
        void drawPeces();
        void drawStart();
        void drawHighScores();
        void drawInstructions();
        void drawReturn();
        void actualitzaPuntsEmpty(int & e);
        void actualitzaPuntsEmpty_tut1(int & e);
        void drawScoreList();
        void drawBack();
        void drawTut1Msg();
        //--------------------------------------------------------------
        // DETECCIÓ
        float relAspectWidth; // detecció
        float relAspectHeight;
        float ampliaHeight; // compensem efecte enanito
        float baixaHoTotAvall; // abaixem la detecci'o una certa distancia
        float mouHoTotDretaEsq;

        // CAMERA
        ofVideoGrabber vidGrabber;
        int camWidth;
        int camHeight;
        int camID;
        float camWidthPorc, camHeightPorc;

        // OPEN CV
        ofxCvColorImage colorImg;
        ofxCvColorImage colorImgFliped;
        ofxCvGrayscaleImage grayImage;
        ofxCvGrayscaleImage grayImageWarped;
        ofxCvGrayscaleImage grayImageTh;
        ofxCvContourFinder contourFinder;

        bool bshowImagesAndContours;
        bool bshowCamera;

        int minArea;
        int maxArea;

        int threshold;
        bool bflipV, bflipH;

        ofPoint puntsSrc[4]; // warp src
        ofPoint puntsDst[4]; // warp desti

        // contorns
        float tmpX, tmpY;
        ofVec4f tmpVecBlobPos;
        ofVec2f warpMousePos;

        //blobs
        ofVec2f posicionsBlobs[MAX_NUM_BLOBS];
        int totalBlobsDetected;

        // GUI APP i DETECCIO i WARP camera
        ofxUICanvas *guia;
        ofxUICanvas *guiw;

        // GUI AJUST BOTONS
        ofxUICanvas *guib;

        // GUI HELP
        ofxUICanvas *guih;
        void guiEvent(ofxUIEventArgs &e); // per a tots els GUIs

        //--------------------------------------------------------------
        // HELP INFO
        void toogleDrawInfoGrid();

        // MUSIC PLAYER
        ofSoundPlayer soundPlayer;
        vector<int> songPeaks;
        void setupMusic(string filename);
        string gameLevel = "";
};
