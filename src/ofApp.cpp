#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){


    fons_inici.loadImage("start-background.jpg");
    fons_scores.loadImage("score-background.jpg");

    // APP
    //ofSetFullscreen(true);
    //ofHideCursor();
    ofSetFrameRate(60);

    // GENRAL
    ofSetVerticalSync(true);
    ofSetCircleResolution(300);
    ofTrueTypeFont::setGlobalDpi(72);
    singleton = Singleton::getInstance();

    // TEMPS DE JOC
	jocMinutsTimer.setup(MAX_GAME_TIME*60*1000, false); // 3 minuts = 3*60*1000 ms
    jocMinutsTimer.stopTimer();

    duradaTheEndTimer.setup(5*1000, false); // 5 segons
    duradaTheEndTimer.stopTimer();

    // WARPER
    warper.setup(0,0,APP_WIDTH,APP_HEIGT);
    warper.load("LightJumper_CornerPin.xml");
    warper.enableKeys(true);

    // PUNTER
    punter.loadImage("punter.png");
    punterWidthMig = punter.getWidth()*0.5;
    punterHeightMig = punter.getHeight()*0.5;

    // COLOR JOC
    saltingBlue = ofColor(0,0,255);

    // TYPO
	ofTrueTypeFont::setGlobalDpi(72);
	saltingTypo.loadFont("Oswald-Bold.otf", 27, true, true); // temps nivell i normal, kids, pro
	saltingTypo.setLetterSpacing(1.2);

    // PECES
    comptadorPeces = 0;
    ofAddListener(pecaEmpty::actualitzaPunts, this, &ofApp::actualitzaPuntsEmpty);

    // BOTONS
    guib = new ofxUICanvas(20, 20, APP_WIDTH*0.3, APP_HEIGT*0.9);

    guib->addLabel(":: BOTONS ::", OFX_UI_FONT_MEDIUM);
    guib->addSpacer();

    //Boto Start
    guib->addIntSlider("botoStartX", 0, APP_WIDTH, &botoStart.botoX)->setIncrement(1);
    guib->addIntSlider("botoStartY", 0, APP_HEIGT, &botoStart.botoY)->setIncrement(1);

    //Boto High_scores
    guib->addIntSlider("botoHighScoresX", 0, APP_WIDTH, &botoHighScores.botoX)->setIncrement(1);
    guib->addIntSlider("botoHighScoresY", 0, APP_HEIGT, &botoHighScores.botoY)->setIncrement(1);

    //Boto Instructions
    guib->addIntSlider("botoInstructionsX", 0, APP_WIDTH, &botoInstructions.botoX)->setIncrement(1);
    guib->addIntSlider("botoInstructionsY", 0, APP_HEIGT, &botoInstructions.botoY)->setIncrement(1);

    //Boto Instructions
    guib->addIntSlider("botoBackX", 0, APP_WIDTH, &botoBack.botoX)->setIncrement(1);
    guib->addIntSlider("botoBackY", 0, APP_HEIGT, &botoBack.botoY)->setIncrement(1);

    //Setup
    botoStart.setup(botoStart.botoX, botoStart.botoY, 75, ofColor(204, 204, 0));
    botoHighScores.setup(botoHighScores.botoX, botoHighScores.botoY, 60, ofColor(204, 204, 0));
    botoInstructions.setup(botoInstructions.botoX, botoInstructions.botoY, 60, ofColor(204, 204, 0));
    botoBack.setup(botoBack.botoX, botoBack.botoY, 55, ofColor(204, 204, 0));


    guib->autoSizeToFitWidgets();
    ofAddListener(guib->newGUIEvent,this,&ofApp::guiEvent);
    guib->loadSettings("LightJumper_Botons.xml");

    guib->setVisible(false);

    // GRID
    myGrid.setup();
    myGrid.gridActivaExcepteMargesSupInfDretaEsq();

    // GUI APP
    guia = new ofxUICanvas(20, 20, APP_WIDTH*0.4, APP_HEIGT*0.9);

    guia->addLabel(":: LIGHT JUMPER SETTINGS ::", OFX_UI_FONT_MEDIUM);
    guia->addSpacer();

    guia->addSpacer();
    guia->addLabelButton("load factory defaults", false);

    guia->addSpacer();
    guia->addLabel("GAME IMAGE CORNERS", OFX_UI_FONT_MEDIUM);
    guia->addLabelButton("reset corners", false);

    guia->addSpacer();
    guia->addLabel("SENSOR SENSIBILITY", OFX_UI_FONT_MEDIUM);
    guia->addIntSlider("sensibility", 0, 255, &threshold);

    guia->addSpacer();
    guia->addLabel("4 GAME CONTROLLERS POSITION", OFX_UI_FONT_MEDIUM);
    guia->addSlider("enlarge gamer", 0.0, 10.0, &ampliaHeight);
    guia->addSlider("adjustment up-down", -1.5*APP_WIDTH, 1.5*APP_WIDTH, &baixaHoTotAvall);
    guia->addSlider("adjustment left-right", -1.5*APP_WIDTH, 1.5*APP_WIDTH, &mouHoTotDretaEsq);

    guia->autoSizeToFitWidgets();
    ofAddListener(guia->newGUIEvent,this,&ofApp::guiEvent);
    guia->loadSettings("LightJumper_Deteccio.xml");

    guia->setVisible(false);

    // HELP
    guih = new ofxUICanvas(20, 20, APP_WIDTH*0.5, APP_HEIGT*0.9);

    guih->addLabel(":: HELP ::", OFX_UI_FONT_MEDIUM);
    guih->addSpacer();
    guih->addFPS();
    guih->addSpacer();
    guih->addTextArea("helpText1", "step 1) WARP: press W | w and mouse click and drag to adjust game's image corners");
    guih->addTextArea("helpText2", "step 2) CAM: press C | c to show camera image");
    guih->addTextArea("helpText3", "step 3) GAME: press J | j | G | g to adjust game");
    guih->addTextArea("helpText4", "step 4) GRID: press I | i to show the game's grid");
    guih->addTextArea("helpText5", "step 5) BUTTONS: press B | b to show buttons adjustments");

    guih->autoSizeToFitWidgets();
    ofAddListener(guih->newGUIEvent,this,&ofApp::guiEvent);

    guih->setVisible(false);


    // CAMERA SENSOR before gui camera
    camID = 1;
    camWidth = 320;
    camHeight = 240;
    bflipV = 0;
    bflipH = 1;
    minArea = 3;
    maxArea = 500;

    // GUI CAMERA
    guiw = new ofxUICanvas(20+20+APP_WIDTH*0.4, 20, APP_WIDTH*0.5, APP_HEIGT*0.9);
    guiw->addLabel(":: CAMERA CORNERS CROP ::", OFX_UI_FONT_MEDIUM);

    camWidthPorc = camWidth*0.5;
    camHeightPorc = camHeight*0.5;
    guiw->add2DPad("top left", ofPoint(0,camWidth), ofPoint(0,camHeight), &puntsSrc[0], camWidthPorc, camHeightPorc);
    guiw->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiw->add2DPad("top right", ofPoint(0,camWidth), ofPoint(0,camHeight), &puntsSrc[3], camWidthPorc, camHeightPorc);
    guiw->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    guiw->add2DPad("botom left", ofPoint(0,camWidth), ofPoint(0,camHeight), &puntsSrc[1], camWidthPorc, camHeightPorc);
    guiw->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiw->add2DPad("botom right", ofPoint(0,camWidth), ofPoint(0,camHeight), &puntsSrc[2], camWidthPorc, camHeightPorc);
    guiw->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    guiw->addLabelButton("reset full camera view", false);

    guiw->autoSizeToFitWidgets();
    ofAddListener(guiw->newGUIEvent,this,&ofApp::guiEvent);
    guiw->loadSettings("LightJumper_Warp_Camera.xml");

    bshowImagesAndContours = false;
    bshowCamera = false;

    guiw->setVisible(false);

    // CAMERA SENSOR
    puntsDst[0].x = 0.0f;
    puntsDst[0].y = 0.0f;
    puntsDst[1].x = 0.0f;
    puntsDst[1].y = (float)camHeight;
    puntsDst[2].x = (float)camWidth;
    puntsDst[2].y = (float)camHeight;
    puntsDst[3].x = (float)camWidth;
    puntsDst[3].y = 0.0f;

    vector<ofVideoDevice> devices = vidGrabber.listDevices();

    ofLogNotice() << "------------------------------------------";
    ofLogNotice() << "CAMERAS sensor, trobades: " << devices.size();
    for(int i = 0; i < devices.size(); i++){
        ofLogNotice() << "  " << devices[i].id << ": " << devices[i].deviceName;
        if( devices[i].bAvailable ){
            ofLogNotice() << " - available ";
        }
        else{
            ofLogNotice() << " - UNavailable ";
        }
    }
    ofLogNotice() << "SETTING UP cam id: " << camID << " width: " << camWidth << " height: " << camHeight;
    ofLogNotice() << "------------------------------------------";

    vidGrabber.setDeviceID(camID);
    vidGrabber.setVerbose(true);
    vidGrabber.setDesiredFrameRate(30);
    vidGrabber.initGrabber(camWidth,camHeight);

    // OPENCV
    colorImg.allocate(camWidth, camHeight);
    colorImgFliped.allocate(camWidth, camHeight);
    grayImage.allocate(camWidth, camHeight);
    grayImageWarped.allocate(camWidth, camHeight);
    grayImageTh.allocate(camWidth, camHeight);

    // MAPING DE CAMERA VALORS ENTRE 0 i 1
    relAspectWidth = 1.0/camWidth;
    relAspectHeight = 1.0/camHeight;

    // PARTIDA
    setVariablesIniciPartida();
}

//--------------------------------------------------------------
void ofApp::setVariablesIniciPartida(){
    // APP
    singleton->setPuntuacioJugador(0);

    // BOTONS
    botoStart.botoSeleccionat = false;
    botoHighScores.botoSeleccionat = false;
    botoInstructions.botoSeleccionat = false;
    botoBack.botoSeleccionat = false;


    // TEMPS DE JOC
    jocMinutsTimer.reset();
    jocMinutsTimer.stopTimer();
    duradaTheEndTimer.reset();
    duradaTheEndTimer.stopTimer();

}

//--------------------------------------------------------------
void ofApp::update(){
    // DETECCIÓ I CAMERA --------------------------------------------------------------------
    vidGrabber.update();
    bool bframeNew = vidGrabber.isFrameNew();
    if(bframeNew){
        colorImg.setFromPixels(vidGrabber.getPixels(), camWidth, camHeight);
        grayImage = colorImg;
        grayImage.mirror(bflipV,bflipH);
        grayImageWarped.warpIntoMe(grayImage, puntsSrc, puntsDst);
        grayImageTh = grayImageWarped;
        grayImageTh.threshold(threshold);

        // CONTORNS
        totalBlobsDetected = 0;
        totalBlobsDetected = contourFinder.findContours(grayImageTh, minArea, maxArea, MAX_NUM_BLOBS, false);

        for(int i=0; i<totalBlobsDetected; i++){
            tmpX = contourFinder.blobs[i].boundingRect.getCenter().x * relAspectWidth;
            tmpY = contourFinder.blobs[i].boundingRect.getCenter().y * relAspectHeight;
            tmpY = tmpY*ampliaHeight; // compensem efecte enaninto

            tmpVecBlobPos = warper.fromScreenToWarpCoord(tmpX*APP_WIDTH, tmpY*APP_HEIGT, 0);
            tmpVecBlobPos.x = tmpVecBlobPos.x + mouHoTotDretaEsq;
            tmpVecBlobPos.y = tmpVecBlobPos.y + baixaHoTotAvall;
            posicionsBlobs[i] = tmpVecBlobPos;
        } // fi de for(int i=0; i<totalBlobsDetected; i++)
    } // fi de if(vidGrabber.isFrameNew())

    // PANTALLES ------------------------------------------------------------------------
    if(pantallaJoc == START){


        //drawBackground();
        botoStart.update(totalBlobsDetected, posicionsBlobs);
        botoStart.updatem(warpMousePos);

        if(botoStart.botoSeleccionat == true){ // CANVI A pantallaJoc = PLAY;
            pantallaJoc = PLAY;
            jocMinutsTimer.startTimer();

            //setupPeca1(); // la única peça d'aquest joc
            setupPeces();
        }

        //INSTRUCTIONS BUTTON
        botoInstructions.update(totalBlobsDetected, posicionsBlobs);
        botoInstructions.updatem(warpMousePos);

        if(botoInstructions.botoSeleccionat == true){ // CANVI A pantallaJoc = INSTRUCTIONS;
            pantallaJoc = INSTRUCTIONS;

               // FONS
            ofSetColor(55);
            ofFill();
            ofRect(0,0, APP_WIDTH, APP_HEIGT);
            // to do: SHOW INSTRUCTIONS!!!!!!!!!!!!
        }


        //HIGH SCORES BUTTON
        botoHighScores.update(totalBlobsDetected, posicionsBlobs);
        botoHighScores.updatem(warpMousePos);

        if(botoHighScores.botoSeleccionat == true){ // CANVI A pantallaJoc = HIGH_SCORES;
            pantallaJoc = HIGH_SCORES;

            ifstream t("scores.txt");
            stringstream buffer;
            buffer << t.rdbuf();
            all_scores = buffer.str();
        }
    }
    else if(pantallaJoc == PLAY){
        //peca1.updatem(warpMousePos);
        //peca1.update(totalBlobsDetected, posicionsBlobs);
        //comprobarEstatsPecesEmpty()
        updatePeces();
        //cout<<jocMinutsTimer.getTime()<<endl;

    }
    else if(pantallaJoc == END){
        if(duradaTheEndTimer.isTimerFinished()){
            // HERE WE STORE THE DATA ON

            ifstream f("scores.txt");
            if(f.good()){

            ofstream log("scores.txt", std::ios_base::app | std::ios_base::out);
            log << "N - " + ofToString(singleton->getPuntuacioJugador()) + " Points\n";
            log.close();

            }

            else{

            ofstream outfile ("scores.txt");
            outfile << "Score: " << singleton->getPuntuacioJugador() << endl;
            outfile.close();
            }


            setVariablesIniciPartida();
            pantallaJoc = START;
        }
    }

    else if(pantallaJoc == HIGH_SCORES){
        //HSCORES BUTTON
        botoBack.update(totalBlobsDetected, posicionsBlobs);
        botoBack.updatem(warpMousePos);
        
        if (botoBack.botoSeleccionat == true)
        {
            pantallaJoc = START;
        }

    }

    // TEMPS ------------------------------------------------------------------------
    jocMinutsTimerSegonsLeft = jocMinutsTimer.getTimeLeftInSeconds();
    jocMinutsTimerMinuts = (int)jocMinutsTimerSegonsLeft/60;
    jocMinutsTimerSegons = jocMinutsTimerSegonsLeft - jocMinutsTimerMinuts*60;

    //cout<<jocMinutsTimerSegons<<endl;

    if(jocMinutsTimer.isTimerFinished()){
        pantallaJoc = END;
        jocMinutsTimer.reset();
        jocMinutsTimer.stopTimer();
        duradaTheEndTimer.startTimer();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    // WARP begin
        cout<<"pantallaJOC: " << pantallaJoc<<endl;

    warper.begin();
    warper.draw();

    // APP
    ofBackground(0);
    ofSetColor(255);
    ofSetWindowTitle("Light Jumper running at " + ofToString((int)ofGetFrameRate()) + " frames per second");


    // PANTALLES --------------------------------------------------------------------------
    if(pantallaJoc == START){


        fons_inici.draw(0,0, APP_WIDTH, APP_HEIGT);
//        drawTemps();
        drawStart();
        drawHighScores();
        drawInstructions();
        botoStart.draw(153, 115, 0, 0, 0);
        botoInstructions.draw(153, 115, 0, 0, 0);
        botoHighScores.draw(153, 115, 0, 0, 0);


    }
    else if(pantallaJoc == PLAY){

        // FONS
        ofSetColor(102, 102, 255);
        ofFill();
        ofRect(0,0, APP_WIDTH, APP_HEIGT);
        drawTemps();
        //peca1.draw();
        drawPeces();
        drawPuntuacio();
    }
    else if(pantallaJoc == END){
        drawEnd();
    }

    else if(pantallaJoc == HIGH_SCORES){
        //FONS
        fons_scores.draw(0,0, APP_WIDTH, APP_HEIGT);

        //
        drawBack();
        drawScoreList();
        botoBack.draw(255,255,255, 0, 0);
        //LEFT
    }

      else if(pantallaJoc == INSTRUCTIONS){
        cout<<"HIGHSCORES"<<endl;
        //RIGHTTTTTTTTTTTTT
    }

    // PUNTERS ----------------------------------------------------
    ofSetColor(255);
    for( int i = 0; i < totalBlobsDetected; i++){
        punter.draw(posicionsBlobs[i].x-punterWidthMig, posicionsBlobs[i].y-punterHeightMig);
    }

    // HELP ----------------------------------------------------
    // GRID
    myGrid.draw();
    //ofSetColor(255,255,0);
    //ofDrawBitmapString("ESTAT: " + pantallaToString(), 20,20);

    // CAMERA
    if(bshowCamera){
        ofSetColor(255);
        colorImgFliped = colorImg;
        colorImgFliped.mirror(bflipV,bflipH);
        colorImgFliped.draw(0,0,APP_WIDTH,APP_HEIGT);
        ofNoFill();
        ofRect(0,0,APP_WIDTH,APP_HEIGT);
    }
    // WARP end
    warper.end();

    // INFO ------------------------------------------------------------------------
    // CAMERES
    if(bshowImagesAndContours){
        float prop = 0.65;
        ofSetHexColor(0xffffff);
        grayImage.draw(20+20+APP_WIDTH*0.4,400, camWidthPorc,camHeightPorc);
        grayImageWarped.draw(20+20+APP_WIDTH*0.4+camWidthPorc+10,400, camWidthPorc,camHeightPorc);

        grayImageTh.draw(20+20+APP_WIDTH*0.4,400+camHeightPorc+30, camWidthPorc,camHeightPorc);
        grayImageTh.draw(20+20+APP_WIDTH*0.4+camWidthPorc+10,400+camHeightPorc+30, camWidthPorc,camHeightPorc);
        contourFinder.draw(20+20+APP_WIDTH*0.4+camWidthPorc+10,400+camHeightPorc+30, camWidthPorc,camHeightPorc);

        ofSetHexColor(0xffffff);
        ofDrawBitmapString("camera image", 20+20+APP_WIDTH*0.4,400-10);
        ofDrawBitmapString("camera adjusted image", 20+20+APP_WIDTH*0.4+camWidthPorc+10,400-10);
        ofDrawBitmapString("sensibility b/w", 20+20+APP_WIDTH*0.4,400+camHeightPorc+30-10);
        ofDrawBitmapString("detection image", 20+20+APP_WIDTH*0.4+camWidthPorc+10,400+camHeightPorc+30-10);
    }

    // PECES ESTATS ------------------------------------------------------------------------------
}

//--------------------------------------------------------------
void ofApp::exit(){
    warper.save("LightJumper_CornerPin.xml");
    guia->saveSettings("LightJumper_Deteccio.xml");
    guiw->saveSettings("LightJumper_Warp_Camera.xml");
    delete guia;
    delete guiw;
    guib->saveSettings("LightJumper_Botons.xml");
    delete guib;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    // H + F1 + a : help, aide, ayuda, ajuda
    // w : corner pin
    // J ajusta joc
    // G : grid -> per development (cal descomentar a baix)
    // D ajusta botons -> per development (cal descomentar a baix)

    if((key == 'h')||(key == 'H')||(key == 'a')||(key == 'A')||(key == OF_KEY_F1)){
        guia->setVisible(false);
        guiw->setVisible(false);
        guib->setVisible(false);
        warper.deactivate();
        bshowImagesAndContours = false;
        guih->toggleVisible();
    }
    else if((key == 'c')||(key == 'C')){
        guia->setVisible(false);
        guiw->setVisible(false);
        guib->setVisible(false);
        guih->setVisible(false);
        bshowImagesAndContours = false;
        warper.deactivate();
        bshowCamera = !bshowCamera;
    }
    else if((key == 'w')||(key == 'W')){
        guia->setVisible(false);
        guiw->setVisible(false);
        guib->setVisible(false);
        guih->setVisible(false);
        bshowImagesAndContours = false;
        warper.toogleActive();
    }
    else if((key == 'j')||(key == 'J')||(key == 'g')||(key == 'G')){
        guib->setVisible(false);
        guih->setVisible(false);
        warper.deactivate();
        guia->toggleVisible();
        guiw->toggleVisible();
        bshowImagesAndContours = !bshowImagesAndContours;
    }
    else if((key == 'f')||(key == 'F')){
        ofToggleFullscreen();
    }
    else if((key == 'i')||(key == 'I')){
        toogleDrawInfoGrid();
    }
    else if((key == 'b')||(key == 'B')){
        guia->setVisible(false);
        guiw->setVisible(false);
        guih->setVisible(false);
        warper.deactivate();
        bshowImagesAndContours = false;
        guib->toggleVisible();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    ofVec4f v = warper.fromScreenToWarpCoord(x,y,0);
    warpMousePos.x = v.x;
    warpMousePos.y = v.y;
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

//--------------------------------------------------------------
void ofApp::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    int kind = e.widget->getKind();

    if(name == "reset full camera view"){
        puntsSrc[0].x = 0.0f;
        puntsSrc[0].y = 0.0f;
        puntsSrc[1].x = 0.0f;
        puntsSrc[1].y = (float)camHeight;
        puntsSrc[2].x = (float)camWidth;
        puntsSrc[2].y = (float)camHeight;
        puntsSrc[3].x = (float)camWidth;
        puntsSrc[3].y = 0.0f;
    }
    else if(name == "reset corners"){
        warper.setCorner(warper.TOP_LEFT, ofPoint(0,0));
        warper.setCorner(warper.TOP_RIGHT, ofPoint(APP_WIDTH,0));
        warper.setCorner(warper.BOTTOM_RIGHT, ofPoint(APP_WIDTH,APP_HEIGT));
        warper.setCorner(warper.BOTTOM_LEFT, ofPoint(0,APP_HEIGT));
    }
    else if(name == "load factory defaults"){
        // reset full camera view
        puntsSrc[0].x = 0.0f;
        puntsSrc[0].y = 0.0f;
        puntsSrc[1].x = 0.0f;
        puntsSrc[1].y = (float)camHeight;
        puntsSrc[2].x = (float)camWidth;
        puntsSrc[2].y = (float)camHeight;
        puntsSrc[3].x = (float)camWidth;
        puntsSrc[3].y = 0.0f;
        // reset corners
        warper.setCorner(warper.TOP_LEFT, ofPoint(0,0));
        warper.setCorner(warper.TOP_RIGHT, ofPoint(APP_WIDTH,0));
        warper.setCorner(warper.BOTTOM_RIGHT, ofPoint(APP_WIDTH,APP_HEIGT));
        warper.setCorner(warper.BOTTOM_LEFT, ofPoint(0,APP_HEIGT));
        // parametres
        threshold = 128; // sensibility
        ampliaHeight = 1.0; // enlarge gamer
        baixaHoTotAvall = 0; // adjustment up-down
        mouHoTotDretaEsq = 0; // adjustment left-right
    }
}


void ofApp::setupPeces(){
    int g = 0;
    int offs = 0;
    int offs2 = 0;
    int offsetArray[] = { 10, 11, 12, 19, 20, 21, 28, 29, 30, 14, 15, 16, 23, 24, 25, 32, 33, 34, 46, 47, 48, 55, 56, 57, 64, 65, 66, 50, 51, 52, 59, 60, 61, 68, 69, 70};

    // intent d'init de peces
    float dtAux = 0;
    for(int i=0; i<NUM_PECES_TOTAL; i++) {
        pecaEmpty peca;
        peca.setup(i,0,myGrid.returnPosicioOfPeca(0),35,dtAux);
        dtAux += 60/float(NUM_PECES_MIN);

        if (i % 4 == 0){
            offs = 0;
            offs2 = 8;
        }
        else if (i % 3 == 0){
            offs = 9;
            offs2 = 17;
        }
        else if (i % 2 == 0){
            offs = 18;
            offs2 = 26;
        }
        else{
            offs = 27;
            offs2 = 35;
        }

        do{
            g = (int)ofRandom(offs, offs2);
            g = offsetArray[g];
        } while(myGrid.brectGridActiu[g] == false);
        peca.init(comptadorPeces, g, myGrid.returnPosicioOfPeca(g));
        peca.estatPeca = CANVIA_ESTAT;
        peca.estatPecaNext = APAREIX;
        peca.bpecaActiva = false;
        peca.idleTimer = IDLE_TIMER_PECA;
        peca.tocadaTimer = TOCADA_TIMER_PECA;
        peces.push_back(peca);
        //cout<<i<<"-"<<dtAux<<endl;
    }
}

void ofApp::updatePeces(){
    pecaEmpty aux;
    vector<pecaEmpty>::iterator it;
    for(it = peces.begin(); it != peces.end(); ) {
        if(it->distanceInTime<=jocMinutsTimer.getTime()) {
            aux = (*it);
            aux.bpecaActiva = true;
            pecesPantalla.push_back(aux);
            it = peces.erase(it);
        }
        else it++;
        //it++;
        //comprobarEstatsPecesEmpty();
    }
    for(it = pecesPantalla.begin(); it != pecesPantalla.end(); it++) {
        it->updatem(warpMousePos);
        it->update(totalBlobsDetected, posicionsBlobs);
        //comprobarEstatsPecesEmpty();
    }

}

void ofApp::drawPeces() {
    vector<pecaEmpty>::iterator it;
    for(it = pecesPantalla.begin(); it != pecesPantalla.end(); it++) {
    //for(it = peces.begin(); it != peces.end(); it++) {
        it->draw();
    }
}

//--------------------------------------------------------------
//--- HELPERS --------------------------------------------------
//--------------------------------------------------------------
void ofApp::toogleDrawInfoGrid(){
    myGrid.toggleDrawInfoHelp();
    peca1.toggleDrawInfoHelp();
    botoStart.toggleDrawInfoHelp();
}

//--------------------------------------------------------------
string ofApp::pantallaToString(){
    if(pantallaJoc == START){
        return "START";
    }
    else if(pantallaJoc == PLAY){
        return "PLAY";
    }
    else if(pantallaJoc == END){
        return "END";
    }
    else if(pantallaJoc == HIGH_SCORES)
        return "HIGH_SCORES";
    else{
        return "NO SE";
    }
}

//--------------------------------------------------------------
//--- PUNTUACIO ------------------------------------------------
//--------------------------------------------------------------
void ofApp::actualitzaPuntsEmpty(int & e){
    singleton->setPuntuacioJugador(singleton->getPuntuacioJugador() + e);
}

//--------------------------------------------------------------
void ofApp::drawTemps(){
    string s = ofToString(jocMinutsTimerMinuts,2,'0') + ":" + ofToString(jocMinutsTimerSegons,2,'0');
    ofRectangle rs;
	rs = saltingTypo.getStringBoundingBox(s,0,0);
	ofPushMatrix();
    ofTranslate(APP_WIDTH_MEITAT-rs.width*0.5,35);
    if(jocMinutsTimerSegonsLeft < 60){
        ofSetColor(255,0,0,255); // temps en vermell
    }
    else{
        ofSetColor(255); // temps en blanc
    }
    saltingTypo.drawString(s,0,0);
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::drawPuntuacio(){
    string s = ofToString(singleton->getPuntuacioJugador()) + " POINTS!";
    ofRectangle rs;
	rs = saltingTypo.getStringBoundingBox(s,0,0);
	ofPushMatrix();
    ofTranslate(APP_WIDTH_MEITAT-rs.width*0.5,135);
    ofSetColor(saltingBlue);
    saltingTypo.drawString(s,0,0);
    ofPopMatrix();
}

void ofApp::drawStart(){
    string strt = "Start!";
    ofRectangle start;
    start = saltingTypo.getStringBoundingBox(strt,0,0);
    ofPushMatrix();
    ofTranslate(APP_WIDTH_MEITAT-start.width*0.5 + 10,370);
    ofSetColor(saltingBlue);
    saltingTypo.drawString(strt,0,0);
    ofPopMatrix();
}

void ofApp::drawScoreList(){
    ofRectangle start;
    start = saltingTypo.getStringBoundingBox(all_scores,0,0);
    ofPushMatrix();
    ofTranslate(APP_WIDTH_MEITAT-start.width*0.5 + 10,60);
    ofSetColor(ofColor(115, 38, 115));

    saltingTypo.drawString(all_scores,0,0);
    ofPopMatrix();
}

void ofApp::drawBack(){
    ofRectangle start;
    start = saltingTypo.getStringBoundingBox("Back",0,0);
    ofPushMatrix();
    ofTranslate(70,500);
    ofSetColor(saltingBlue);
    saltingTypo.drawString("Back",0,0);
    ofPopMatrix();
}


void ofApp::drawInstructions(){
    string instr = "Instructions";
    ofRectangle instructions;
    instructions = saltingTypo.getStringBoundingBox(instr,0,0);
    ofPushMatrix();
    ofTranslate(APP_WIDTH_MEITAT-instructions.width*0.5 +215,80);
    ofSetColor(saltingBlue);
    saltingTypo.drawString(instr,0,0);
    ofPopMatrix();
}
void ofApp::drawHighScores(){
    string hs = "High Scores";
    ofRectangle hscores;
    hscores = saltingTypo.getStringBoundingBox(hs,0,0);
    ofPushMatrix();
    ofTranslate(APP_WIDTH_MEITAT-hscores.width*0.5 + -170,80);
    ofSetColor(saltingBlue);
    saltingTypo.drawString(hs,0,0);
    ofPopMatrix();


}

//--------------------------------------------------------------
void ofApp::drawEnd(){
    int aux = NUM_PECES_TOTAL;
    string s = "GREAT JOB!! " + ofToString(singleton->getPuntuacioJugador()) + "/" + ofToString(aux) + " POINTS";
    ofRectangle rs;
	rs = saltingTypo.getStringBoundingBox(s,0,0);
	ofPushMatrix();
    ofTranslate(APP_WIDTH_MEITAT-rs.width*0.5,APP_HEIGT_MEITAT-rs.height*0.5);
    ofSetColor(255);
    saltingTypo.drawString(s,0,0);
    ofPopMatrix();
}
