#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    max_score_tutorial = 0;
    current_score_tutorial = 0;
    fons_inici.loadImage("start-background.png");
    fons_scores.loadImage("start-background.png");

    // APP
    //ofSetFullscreen(true);
    //ofHideCursor();
    ofSetFrameRate(60);
    revisat = false;
    // GENRAL
    ofSetVerticalSync(true);
    ofSetCircleResolution(300);
    ofTrueTypeFont::setGlobalDpi(72);
    singleton = Singleton::getInstance();

    // MUSIC
    soundPlayer.loadSound("DoIWannaKnow.mp3");
    setupMusic("levelDoIWannaKnow.txt");

    // TEMPS DE JOC
	jocMinutsTimer.setup(MAX_GAME_TIME*1000, false); // 3 minuts = 3*60*1000 ms
    jocMinutsTimer.stopTimer();

    tutoTimer.setup(24000, false); // 30 segons
    tutoTimer.stopTimer();
    duradaTheEndTimer.setup(5000, false); // 5 segons
    duradaTheEndTimer.stopTimer();
    shortTimer.setup(2800, false); // 2.8 segons
    shortTimer.stopTimer();

    // WARPER
    warper.setup(0,0,APP_WIDTH,APP_HEIGT);
    warper.load("LightJumper_CornerPin.xml");
    warper.enableKeys(true);

    // PUNTER
    punter.loadImage("punter.png");
    punterWidthMig = punter.getWidth()*0.5;
    punterHeightMig = punter.getHeight()*0.5;

    // COLOR JOC
    saltingBlue = ofColor(255,255,255);

    // TYPO
	saltingTypo.loadFont("Verdana.ttf", 25, true, true);
	saltingTypo.setLineHeight(14.0f);
	saltingTypo.setLetterSpacing(1.037);

    // PECES
    comptadorPeces = 0;
    ofAddListener(pecaEmpty::actualitzaPunts, this, &ofApp::actualitzaPuntsEmpty);

    //Setup
    botoStart.setup(botoStart.botoX, botoStart.botoY, 75, ofColor(204, 204, 0));
    botoHighScores.setup(botoHighScores.botoX, botoHighScores.botoY, 60, ofColor(204, 204, 0));
    botoInstructions.setup(botoInstructions.botoX, botoInstructions.botoY, 60, ofColor(204, 204, 0));
    botoBack.setup(botoBack.botoX, botoBack.botoY, 55, ofColor(204, 204, 0));
    botoReturn.setup(botoReturn.botoX, botoReturn.botoY, 30, ofColor(204, 204, 0));

    // DEBUG
    setupDebugGUI();
    setupCamera();

    // PARTIDA
    setVariablesIniciPartida();
}

//--------------------------------------------------------------
void ofApp::setVariablesIniciPartida(){
    // APP
    singleton->setPuntuacioJugador(0);
    tuto_phase = 1;

    // BOTONS
    botoStart.botoSeleccionat = false;
    botoHighScores.botoSeleccionat = false;
    botoInstructions.botoSeleccionat = false;
    botoBack.botoSeleccionat = false;
    botoReturn.botoSeleccionat = false;

    // TEMPS DE JOC
    jocMinutsTimer.reset();
    jocMinutsTimer.stopTimer();
    tutoTimer.reset();
    tutoTimer.stopTimer();
    shortTimer.reset();
    shortTimer.stopTimer();
    duradaTheEndTimer.reset();
    duradaTheEndTimer.stopTimer();
}

void ofApp::setupMusic(string filename) {
    ifstream levelFile;
    int positionMs;
    levelFile.open("levelDoIWannaKnow.txt");
    if (!levelFile) {
        cout << "Unable to open song level file :(";
        std::exit(1); // terminate with error
    }
    while (levelFile >> positionMs) {
        songPeaks.push_back(positionMs);
    }
    cout << "done reading level!";
    cout << songPeaks.size();
    levelFile.close();
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
        //update buttons
        botoStart.update(totalBlobsDetected, posicionsBlobs);
        botoStart.updatem(warpMousePos);
        botoInstructions.update(totalBlobsDetected, posicionsBlobs);
        botoInstructions.updatem(warpMousePos);
        botoHighScores.update(totalBlobsDetected, posicionsBlobs);
        botoHighScores.updatem(warpMousePos);

        if(botoStart.botoSeleccionat == true){ // CANVI A pantallaJoc = PLAY;
            pantallaJoc = PLAY;
            soundPlayer.play();
            jocMinutsTimer.startTimer();
            nextSongPeakIndex = 0;
            setupPeces();
        }

        if(botoInstructions.botoSeleccionat == true){ // CANVI A pantallaJoc = TUTORIAL_1;
            pantallaJoc = TUTORIAL_1;
            tutoTimer.startTimer();
            botoInstructions.botoSeleccionat = false;
            setupTutorial1();
        }

        if(botoHighScores.botoSeleccionat == true){ // CANVI A pantallaJoc = HIGH_SCORES;
            pantallaJoc = HIGH_SCORES;
            ifstream t("scores.txt");
            stringstream buffer;
            buffer << t.rdbuf();
            all_scores = buffer.str();
            botoHighScores.botoSeleccionat = false;
        }
    }

    else if(pantallaJoc == PLAY){
        updatePeces();
    }

    else if(pantallaJoc == TUTORIAL_1){
        //HSCORES BUTTON
        updatePecesTut1();
        botoReturn.update(totalBlobsDetected, posicionsBlobs);
        botoReturn.updatem(warpMousePos);

        if (singleton->getPuntuacioJugador()==8 || tutoTimer.isTimerFinished()) {
            shortTimer.startTimer();
            tutoTimer.reset();
            tutoTimer.stopTimer();
        }
        if(shortTimer.isTimerFinished()){
             pantallaJoc = SKIP_PHASE_TUTORIAL;
             pecesTut.clear();
             setupTutorial2();
             shortTimer.reset();
             shortTimer.stopTimer();
             singleton->setPuntuacioJugador(0);
             //FER SETUP SEGUENT FASE
        }
        if (botoReturn.botoSeleccionat == true)
        {
            pantallaJoc = START;
            botoReturn.botoSeleccionat = false;
            singleton->setPuntuacioJugador(0);
            duradaTheEndTimer.reset();
            duradaTheEndTimer.stopTimer();
            shortTimer.reset();
            shortTimer.stopTimer();
            tutoTimer.reset();
            tutoTimer.stopTimer();
            pecesTut.clear();
            pecesPantalla.clear();
            tuto_phase = 1;
        }
    }

    else if(pantallaJoc == TUTORIAL_2){
        //HSCORES BUTTON
        updatePecesTut2();
        botoReturn.update(totalBlobsDetected, posicionsBlobs);
        botoReturn.updatem(warpMousePos);

        if (singleton->getPuntuacioJugador()==8 || tutoTimer.isTimerFinished()) {
            shortTimer.startTimer();
            tutoTimer.reset();
            tutoTimer.stopTimer();
        }
        if(shortTimer.isTimerFinished()){
             setupTutorial3();
             pantallaJoc = SKIP_PHASE_TUTORIAL;
             shortTimer.reset();
             shortTimer.stopTimer();
             singleton->setPuntuacioJugador(0);
             //FER SETUP SEGUENT FASE
        }
        if (botoReturn.botoSeleccionat == true)
        {
            pantallaJoc = START;
            botoReturn.botoSeleccionat = false;
            singleton->setPuntuacioJugador(0);
            duradaTheEndTimer.reset();
            duradaTheEndTimer.stopTimer();
            shortTimer.reset();
            shortTimer.stopTimer();
            tutoTimer.reset();
            tutoTimer.stopTimer();
            pecesTut.clear();
            pecesPantalla.clear();
            tuto_phase = 1;
        }
    }

    else if(pantallaJoc == TUTORIAL_3){
        //HSCORES BUTTON
        updatePecesTut3();
        botoReturn.update(totalBlobsDetected, posicionsBlobs);
        botoReturn.updatem(warpMousePos);

        if (singleton->getPuntuacioJugador()==13 || tutoTimer.isTimerFinished()) {
            shortTimer.startTimer();
            tutoTimer.reset();
            tutoTimer.stopTimer();
        }
        if(shortTimer.isTimerFinished()){
            //setupTutorial4();
             pantallaJoc = SKIP_PHASE_TUTORIAL;
             shortTimer.reset();
             shortTimer.stopTimer();
             singleton->setPuntuacioJugador(0);
             //FER SETUP SEGUENT FASE
        }
        if (botoReturn.botoSeleccionat == true)
        {
            pantallaJoc = START;
            botoReturn.botoSeleccionat = false;
            singleton->setPuntuacioJugador(0);
            duradaTheEndTimer.reset();
            duradaTheEndTimer.stopTimer();
            shortTimer.reset();
            shortTimer.stopTimer();
            tutoTimer.reset();
            tutoTimer.stopTimer();
            pecesTut.clear();
            pecesPantalla.clear();
            tuto_phase = 1;
        }
    }

    else if(pantallaJoc == SKIP_PHASE_TUTORIAL){
        shortTimer.startTimer();
        if (shortTimer.isTimerFinished()){
            shortTimer.reset();
            shortTimer.stopTimer();
            switch(tuto_phase) {
                case(1):
                    pantallaJoc = TUTORIAL_2;
                    tutoTimer.startTimer();
                    tuto_phase = 2;
                    break;
                case(2):
                    pantallaJoc = TUTORIAL_3;
                    tutoTimer.startTimer();
                    tuto_phase = 3;
                    break;
                case(3):
                    pantallaJoc = START;
                    tuto_phase = 1;
                    /*
                case(4):
                    pantallaJoc = TUTORIAL_5;*/
            }
        }
    }

    else if(pantallaJoc == END){
        if(duradaTheEndTimer.isTimerFinished()){
            // HERE WE STORE THE DATA ON
            ifstream f("scores.txt");
            if(f.good()){
                ofstream log("scores.txt", std::ios_base::app | std::ios_base::out);
                log << "N - " + ofToString(singleton->getPuntuacioJugador()) + " Points" << endl;
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
        if (botoBack.botoSeleccionat == true){
            pantallaJoc = START;
            botoBack.botoSeleccionat =false;
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
    //cout<<"pantallaJOC: " << pantallaJoc<<endl;

    warper.begin();
    warper.draw();

    // APP
    ofSetWindowTitle("Light Jumper running at " + ofToString((int)ofGetFrameRate()) + " frames per second");

    // PANTALLES --------------------------------------------------------------------------
    if(pantallaJoc == START){
        fons_inici.draw(0,0, APP_WIDTH, APP_HEIGT);
//      drawTemps();
        drawStart();
        drawHighScores();
        drawInstructions();
        botoStart.draw(153, 115, 0, 0, 0);
        botoInstructions.draw(153, 115, 0, 0, 0);
        botoHighScores.draw(153, 115, 0, 0, 0);
    }

    else if(pantallaJoc == PLAY){
        // FONS
        ofSetColor(0,0,0);
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

    else if(pantallaJoc == TUTORIAL_1 || pantallaJoc == TUTORIAL_2 || pantallaJoc == TUTORIAL_3) {
        ofSetColor(0, 0, 77);
        ofFill();
        ofRect(0, 0, APP_WIDTH, APP_HEIGT);
        botoReturn.draw(255,255,255, 0, 0);
        drawReturn();
        drawTutMsg();
        drawPeces();
    }

    else if(pantallaJoc == SKIP_PHASE_TUTORIAL){
        ofSetColor(59, 0, 100);
        ofFill();
        ofRect(0,0, APP_WIDTH, APP_HEIGT);
        drawSkipMessage(tuto_phase);
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
    else if(key == ' ' && soundPlayer.getIsPlaying()) {
        stringstream ss;
        ss << soundPlayer.getPositionMS();
        ss << "\n";
        gameLevel.append(ss.str());
        cout << ss.str();
    }
    else if(key == 's') {
        ofstream myfile;
        myfile.open("newLevel.txt");
        myfile << gameLevel;
        myfile.close();
     }
    else if((key == 'b')||(key == 'B')){
        guia->setVisible(false);
        guiw->setVisible(false);
        guih->setVisible(false);
        warper.deactivate();
        bshowImagesAndContours = false;
        guib->toggleVisible();
    }
    else if(key == 'p'){
        soundPlayer.play();
    }
    else if(key == ' ' && soundPlayer.getIsPlaying()) {
        stringstream ss;
        ss << soundPlayer.getPositionMS();
        ss << "\n";
        gameLevel.append(ss.str());
        cout << ss.str();
    }
    else if(key == 's') {
        ofstream myfile;
        myfile.open("newLevel.txt");
        myfile << gameLevel;
        myfile.close();
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

int ofApp::setupPatron1(int offset){
    int g = 0;
    int midColArray[] = { 13, 31, 49, 67 };
    int pairLinesArray[] = { 10, 16, 28, 34, 46, 52, 64, 70 };
    ofVec2f speed;
    speed.x = 0;
    speed.y = 0;

    float dtAux = offset;
    //setup patron 1 - punts centrals
    for(int i=0; i<8; i++) {
        pecaEmpty peca;
        peca.setup(i,0,myGrid.returnPosicioOfPeca(0),50,dtAux,0,0,speed);
        dtAux += 4*BEAT_DIST_MS;
        g = midColArray[i%4];
        peca.init(comptadorPeces, g, myGrid.returnPosicioOfPeca(g));
        peca.estatPeca = CANVIA_ESTAT;
        peca.estatPecaNext = APAREIX;
        peca.bpecaActiva = false;
        peca.idleTimer = IDLE_TIMER_PECA;
        peca.tocadaTimer = TOCADA_TIMER_PECA;
        peca.onBeatTimer = BEAT_TIMER_PECA;
        peces.push_back(peca);
    }

    dtAux = offset+(2*BEAT_DIST_MS); //-10 ms adjust
    //setup patron 1 - punts perifery
    for(int i=0; i<16; i++) {
        pecaEmpty peca;
        peca.setup(i,0,myGrid.returnPosicioOfPeca(0),34,dtAux,0,0,speed);
        if(i%2 != 0) {
                dtAux += 4*BEAT_DIST_MS;
        }
        g = pairLinesArray[i%8];
        peca.init(comptadorPeces, g, myGrid.returnPosicioOfPeca(g));
        peca.estatPeca = CANVIA_ESTAT;
        peca.estatPecaNext = APAREIX;
        peca.bpecaActiva = false;
        peca.idleTimer = IDLE_TIMER_PECA;
        peca.tocadaTimer = TOCADA_TIMER_PECA;
        peca.onBeatTimer = BEAT_TIMER_PECA;
        peces.push_back(peca);
    }
    return dtAux;
}

int ofApp::setupPatron2(int offset) {
    int g = 0;
    int k = 0;
    int midColArray[] = { 31, 40, 49 };
    int shiftArrayCross[] = { -9, -1, 1, 9 };
    int shiftArrayEx[] = { -10, -8, 8, 10};
    ofVec2f speed;
    speed.x = 0;
    speed.y = 0;

    float dtAux = offset;
    //setup patron 2 - punts centrals
    for(int i=0; i<8; i++) {
        pecaEmpty peca;
        peca.setup(i,0,myGrid.returnPosicioOfPeca(0),50,dtAux,0,0,speed);
        g = midColArray[(i+1)%3];
        peca.init(comptadorPeces, g, myGrid.returnPosicioOfPeca(g));
        peca.estatPeca = CANVIA_ESTAT;
        peca.estatPecaNext = APAREIX;
        peca.bpecaActiva = false;
        peca.idleTimer = IDLE_TIMER_PECA;
        peca.tocadaTimer = TOCADA_TIMER_PECA;
        peca.onBeatTimer = BEAT_TIMER_PECA;
        peces.push_back(peca);

        for(int j=0; j<4; j++){
            if(i%2) {
                k = g + shiftArrayCross[j];
                switch(j){
                    case(0):
                        speed.x = 0;
                        speed.y = -1;
                    break;
                    case(1):
                        speed.x = -1;
                        speed.y = 0;
                    break;
                    case(2):
                        speed.x = 1;
                        speed.y = 0;
                    break;
                    case(3):
                        speed.x = 0;
                        speed.y = 1;
                    break;
                }
            }
            else {
                k = g + shiftArrayEx[j];
                switch(j){
                    case(0):
                        speed.x = -sqrt(2)/2;
                        speed.y = -sqrt(2)/2;
                    break;
                    case(1):
                        speed.x = sqrt(2)/2;
                        speed.y = -sqrt(2)/2;;
                    break;
                    case(2):
                        speed.x = -sqrt(2)/2;
                        speed.y = sqrt(2)/2;
                    break;
                    case(3):
                        speed.x = sqrt(2)/2;
                        speed.y = sqrt(2)/2;
                    break;
                }
            }
            peca.setup(i,0,myGrid.returnPosicioOfPeca(0),34,dtAux,0,0,speed);
            peca.init(comptadorPeces, k, myGrid.returnPosicioOfPeca(k));
            peca.estatPeca = CANVIA_ESTAT;
            peca.estatPecaNext = APAREIX;
            peca.bpecaActiva = false;
            peca.idleTimer = IDLE_TIMER_PECA;
            peca.tocadaTimer = TOCADA_TIMER_PECA;
            peca.onBeatTimer = BEAT_TIMER_PECA;
            peces.push_back(peca);
        }
        dtAux += 4*BEAT_DIST_MS;
        speed.x = 0;
        speed.y = 0;
    }
    return dtAux;
}

int ofApp::setupPatron3(int offset) {
    int g = 0;
    int k = 0;
    int topColArray[] = { 10, 11, 12, 13, 14, 15 };
    int leftColArray[] = { 64, 55, 46, 37, 28, 19 };
    int botColArray[] = { 70, 69, 68, 67, 66, 65 };
    int rightColArray[] = { 16, 25, 34, 43, 52, 61 };
    ofVec2f speed;

    float dtAux = offset;
    //setup patron 3
    for(int i=0; i<6; i++) {
        pecaEmpty peca;
        for(int j=0; j<4; j++){
            switch(j){
                case(0):
                    speed.x = 0;
                    speed.y = 1;
                    k = topColArray[i];
                break;
                case(1):
                    speed.x = -1;
                    speed.y = 0;
                    k = rightColArray[i];
                break;
                case(2):
                    speed.x = 1;
                    speed.y = 0;
                    k = leftColArray[i];
                break;
                case(3):
                    speed.x = 0;
                    speed.y = -1;
                    k = botColArray[i];
                break;
            }
            peca.setup(i,0,myGrid.returnPosicioOfPeca(0),34,dtAux,0,0,speed);
            peca.init(comptadorPeces, k, myGrid.returnPosicioOfPeca(k));
            peca.estatPeca = CANVIA_ESTAT;
            peca.estatPecaNext = APAREIX;
            peca.bpecaActiva = false;
            peca.idleTimer = IDLE_TIMER_PECA;
            peca.tocadaTimer = TOCADA_TIMER_PECA;
            peca.onBeatTimer = BEAT_TIMER_PECA;
            peces.push_back(peca);
        }
        dtAux += 2.7*BEAT_DIST_MS;
    }
    dtAux+=4*BEAT_DIST_MS;
    return dtAux;
}

int ofApp::setupPatron4(int offset) {
    int g = 0;
    int midColArray[] = { 22, 31, 40, 49, 58 };
    ofVec2f speed;

    float dtAux = offset;
    //setup patron 4
    for(int i=0; i<5; i++) {
        pecaEmpty peca;
        if(i%2){
            speed.x = 1;
            speed.y = 0;
            g = midColArray[i];
        }
        else{
            speed.x = -1;
            speed.y = 0;
            g = midColArray[i];
        }
        peca.setup(i,0,myGrid.returnPosicioOfPeca(0),34,dtAux,0,0,speed);
        peca.init(comptadorPeces, g, myGrid.returnPosicioOfPeca(g));
        peca.estatPeca = CANVIA_ESTAT;
        peca.estatPecaNext = APAREIX;
        peca.bpecaActiva = false;
        peca.idleTimer = IDLE_TIMER_PECA;
        peca.tocadaTimer = TOCADA_TIMER_PECA;
        peca.onBeatTimer = BEAT_TIMER_PECA;
        peces.push_back(peca);
    }
    return dtAux + (6*BEAT_DIST_MS);
}

int ofApp::setupPatron5(int offset) {
    int g = 0;
    int midColArray[] = { 38, 39, 40, 41, 42 };
    ofVec2f speed;

    float dtAux = offset;
    //setup patron 3 - punts centrals
    //setup patron 2 - punts centrals
    for(int i=0; i<5; i++) {
        pecaEmpty peca;
        if(i%2){
            speed.x = 0;
            speed.y = 1;
            g = midColArray[i];
        }
        else{
            speed.x = 0;
            speed.y = -1;
            g = midColArray[i];
        }
        peca.setup(i,0,myGrid.returnPosicioOfPeca(0),34,dtAux,0,0,speed);
        peca.init(comptadorPeces, g, myGrid.returnPosicioOfPeca(g));
        peca.estatPeca = CANVIA_ESTAT;
        peca.estatPecaNext = APAREIX;
        peca.bpecaActiva = false;
        peca.idleTimer = IDLE_TIMER_PECA;
        peca.tocadaTimer = TOCADA_TIMER_PECA;
        peca.onBeatTimer = BEAT_TIMER_PECA;
        peces.push_back(peca);
    }
    return dtAux + (6*BEAT_DIST_MS);
}

int ofApp::setupPatron6(int offset) {

    int g = 40;
    ofVec2f speed;
    float dtAux = offset;
    float rot = 0;
    float angle = 0;

    int swColArray[] = { 64 ,65, 66 };
    int seColArray[] = { 68, 69, 70 };
    int nwColArray[] = { 10, 11 ,12 };
    int neColArray[] = { 14, 15, 16 };

    for(int i=0; i<4; i++) {
        pecaEmpty peca;
        for(int j=0; j<3; j++){
            switch(i){
                case(0):
                    speed.x = -1;
                    speed.y = 0;
                    angle = PI;
                    rot = -0.005;
                    g = nwColArray[j];
                break;
                case(1):
                    speed.x = 1;
                    speed.y = 0;
                    angle = 0;
                    rot = 0.005;
                    g = neColArray[j];
                break;
                case(2):
                    speed.x = -1;
                    speed.y = 0;
                    angle = PI;
                    rot = 0.005;
                    g = swColArray[j];
                break;
                case(3):
                    speed.x = 0;
                    speed.y = 1;
                    angle = 0;
                    rot = -0.005;
                    g = seColArray[j];
                break;
            }
            peca.setup(i,0,myGrid.returnPosicioOfPeca(0),34,dtAux,angle,rot,speed);
            peca.init(comptadorPeces, g, myGrid.returnPosicioOfPeca(g));
            peca.estatPeca = CANVIA_ESTAT;
            peca.estatPecaNext = APAREIX;
            peca.bpecaActiva = false;
            peca.idleTimer = IDLE_TIMER_PECA+60;
            peca.tocadaTimer = TOCADA_TIMER_PECA;
            peca.onBeatTimer = BEAT_TIMER_PECA;
            peca.speedBonus = 1.5;
            peces.push_back(peca);
        }
    }
    return offset+(10*BEAT_DIST_MS);
}

int ofApp::setupPatron7(int offset) {
    int g = 40;
    ofVec2f speed;
    float dtAux = offset;
    float rot = 0;
    float angle = 0;

    //int swColArray[] = { 64 ,65, 66 };
    //int seColArray[] = { 68, 69, 70 };
    int nwColArray[] = { 10, 11 ,12 };
    int neColArray[] = { 14, 15, 16 };

    for(int i=0; i<2; i++) {
        pecaEmpty peca;
        for(int j=0; j<3; j++){
            switch(i){
                case(0):
                    speed.x = 0;
                    speed.y = 1;
                    angle = PI*0.25;
                    rot = -0.0006;
                    g = nwColArray[j];
                break;
                case(1):
                    speed.x = 0;
                    speed.y = 1;
                    angle = PI*0.75;
                    rot = 0.0006;
                    g = neColArray[j];
                break;
            }
            peca.setup(i,0,myGrid.returnPosicioOfPeca(0),34,dtAux,angle,rot,speed);
            peca.init(comptadorPeces, g, myGrid.returnPosicioOfPeca(g));
            peca.estatPeca = CANVIA_ESTAT;
            peca.estatPecaNext = APAREIX;
            peca.bpecaActiva = false;
            peca.idleTimer = IDLE_TIMER_PECA+60;
            peca.tocadaTimer = TOCADA_TIMER_PECA;
            peca.onBeatTimer = BEAT_TIMER_PECA;
            peca.speedBonus = 1.5;
            peces.push_back(peca);
        }
    }
    return offset+(10*BEAT_DIST_MS);
}

int ofApp::setupPatron8(int offset) {
    int g = 40;
    ofVec2f speed;
    float dtAux = offset;
    float rot = 0;
    float angle = 0;

    int starArray[] = { 30, 32, 48, 50 };
    int crossArray[] = { 13, 37, 43, 67 };
    int initStarSpeed[] = {-1,-1,1,-1,-1,1,1,1};
    int initCrossSpeed[] = {0,1,1,0,-1,0,0,-1};

    for(int i=0; i<2; i++) {
        pecaEmpty peca;
        for(int j=0; j<4; j++){
            switch(i){
                case(0):
                    speed.x = initStarSpeed[j*2];
                    speed.y = initStarSpeed[(j*2)+1];
                    g = starArray[j];
                break;
                case(1):
                    speed.x = initCrossSpeed[j*2];
                    speed.y = initCrossSpeed[(j*2)+1];
                    g = crossArray[j];
                break;
            }
            peca.setup(i,0,myGrid.returnPosicioOfPeca(0),34,dtAux,angle,rot,speed);
            peca.init(comptadorPeces, g, myGrid.returnPosicioOfPeca(g));
            peca.estatPeca = CANVIA_ESTAT;
            peca.estatPecaNext = APAREIX;
            peca.bpecaActiva = false;
            peca.idleTimer = IDLE_TIMER_PECA+60;
            peca.tocadaTimer = TOCADA_TIMER_PECA;
            peca.onBeatTimer = BEAT_TIMER_PECA;
            peca.speedBonus = 1.2;
            peces.push_back(peca);
        }
    }
    return offset+(10*BEAT_DIST_MS);
}

int ofApp::setupPatron9(int offset) {
    int g = 0;
    int k = 0;
    int topColArray[] = { 10, 12, 14, 16 };
    int rightColArray[] = { 16, 34, 52, 70 };
    int botColArray[] = { 70, 68, 66, 64 };
    int leftColArray[] = { 64, 46, 28, 10 };
    ofVec2f speed;
    float dtAux = offset;

    //setup patron 9
    for(int i=0; i<4; i++) {
        pecaEmpty peca;
        for(int j=0; j<4; j++){
            switch(i){
                case(0):
                    speed.x = 0;
                    speed.y = 1;
                    k = topColArray[j];
                break;
                case(1):
                    speed.x = -1;
                    speed.y = 0;
                    k = rightColArray[j];
                break;
                case(2):
                    speed.x = 0;
                    speed.y = -1;
                    k = botColArray[j];
                break;
                case(3):
                    speed.x = 1;
                    speed.y = 0;
                    k = leftColArray[j];
                break;
                dtAux+=0.25*BEAT_DIST_MS;
            }
            peca.setup(i,0,myGrid.returnPosicioOfPeca(0),34,dtAux,0,0,speed);
            peca.init(comptadorPeces, k, myGrid.returnPosicioOfPeca(k));
            peca.estatPeca = CANVIA_ESTAT;
            peca.estatPecaNext = APAREIX;
            peca.bpecaActiva = false;
            peca.idleTimer = IDLE_TIMER_PECA;
            peca.tocadaTimer = TOCADA_TIMER_PECA;
            peca.onBeatTimer = BEAT_TIMER_PECA;
            peca.speedBonus = 2.2;
            peces.push_back(peca);
            dtAux+=1*BEAT_DIST_MS;
        }
        dtAux += 2.7*BEAT_DIST_MS;
    }
    return dtAux;
}

void ofApp::setupPeces(){
    int offsetAcc = FIRST_BEAT;
    int trashOffs = 0;
    offsetAcc = setupPatron1(offsetAcc);
    offsetAcc = setupPatron2(offsetAcc);
    offsetAcc = setupPatron3(offsetAcc);
    offsetAcc = setupPatron4(offsetAcc);
    offsetAcc = setupPatron5(offsetAcc);
    offsetAcc = setupPatron6(offsetAcc);
    offsetAcc = setupPatron7(offsetAcc);
    offsetAcc = setupPatron4(offsetAcc);
    offsetAcc = setupPatron5(offsetAcc);
    offsetAcc = setupPatron2(offsetAcc);
    offsetAcc = setupPatron8(offsetAcc);
    offsetAcc = setupPatron9(offsetAcc);
    offsetAcc = setupPatron9(offsetAcc);
    offsetAcc = setupPatron6(offsetAcc);
    offsetAcc = setupPatron8(offsetAcc);
    offsetAcc = setupPatron8(offsetAcc);
    offsetAcc = setupPatron7(offsetAcc);
    cout<<offsetAcc<<endl;
}

void ofApp::setupTutorial1(){
    //Fase 1 <idle dots>
    int g = 0;
    float dtAux = 3;
    int offsetArray[] = {10, 25, 66, 52, 21, 40, 29, 47};
    ofVec2f speed;
    speed.x = 0;
    speed.y = 0;

    for(int i=0; i<8; i++) {
        pecaEmpty peca;
        peca.setup(i,0,myGrid.returnPosicioOfPeca(0),50,dtAux,0,0,speed);
        dtAux += 2;
        g = offsetArray[i];
        cout<<dtAux<<endl;
        peca.init(comptadorPeces, g, myGrid.returnPosicioOfPeca(g));
        peca.estatPeca = CANVIA_ESTAT;
        peca.estatPecaNext = APAREIX;
        peca.bpecaActiva = false;
        peca.idleTimer = IDLE_TIMER_PECA;
        peca.tocadaTimer = TOCADA_TIMER_PECA;
        pecesTut.push_back(peca);
    }
}

void ofApp::setupTutorial2(){
    //Fase 2 <moving dots>
    int g = 0;
    float dtAux = 3;
    int offsetArray[] = {22, 30, 41, 32, 38, 20, 48, 40};
    for(int i=0; i<8; i++) {
        pecaEmpty peca;
        float angle = ofRandom(-PI,PI);
        ofVec2f speed;
        speed.x = cos(angle);
        speed.y = sin(angle);
        peca.setup(i,0,myGrid.returnPosicioOfPeca(0),50,dtAux,0,0,speed);
        dtAux += 2;
        g = offsetArray[i];
        peca.init(comptadorPeces, g, myGrid.returnPosicioOfPeca(g));
        peca.estatPeca = CANVIA_ESTAT;
        peca.estatPecaNext = APAREIX;
        peca.bpecaActiva = false;
        peca.idleTimer = IDLE_TIMER_PECA;
        peca.tocadaTimer = TOCADA_TIMER_PECA;
        pecesMovTut.push_back(peca);
    }
}

void ofApp::setupTutorial3(){
    //Fase 3 <moving dots & still dots>
    int g = 0;
    float dtAux = 3;
    int offsetArray[] = {22, 30, 56, 32, 38, 20, 48};
    ofVec2f speed;
    speed.x = 0;
    speed.y = 0;

    for(int i=0; i<7; i++) {
        pecaEmpty peca;
        peca.setup(i,0,myGrid.returnPosicioOfPeca(0),50,dtAux,0,0,speed);
        dtAux += 3 + ofRandom(0.5,2);
        g = offsetArray[i];
        //cout<<dtAux<<endl;
        peca.init(comptadorPeces, g, myGrid.returnPosicioOfPeca(g));
        peca.estatPeca = CANVIA_ESTAT;
        peca.estatPecaNext = APAREIX;
        peca.bpecaActiva = false;
        peca.idleTimer = IDLE_TIMER_PECA;
        peca.tocadaTimer = TOCADA_TIMER_PECA;
        pecesTut.push_back(peca);
    }

    g = 0;
    dtAux = 6;
    int offsetArray2[] = {12, 23, 40, 52, 21, 41, 50, 31};

    for(int i=0; i<sizeof(offsetArray2)/sizeof(offsetArray2[0]); i++) {
        pecaEmpty peca;
        switch(i)
        {
            case(0):
                speed.x = 1;
                speed.y = 1;
            break;
            case(1):
                speed.x = -1;
                speed.y = 1;
            break;
            case(2):
                speed.x = 0;
                speed.y = -1;
            break;
            case(3):
                speed.x = -1;
                speed.y = -1;
            break;
            case(4):
                speed.x = 1;
                speed.y = 1;
            break;
            case(5):
                speed.x = -1;
                speed.y = 1;
            break;
            case(6):
                speed.x = 1;
                speed.y = 0;
            break;
            case(7):
                speed.x = 0;
                speed.y = -1;
            break;
        }

        peca.setup(i,0,myGrid.returnPosicioOfPeca(0),50,dtAux,0,0,speed);
        dtAux += 2 + ofRandom(0.5,2);
        g = offsetArray2[i];
        peca.init(comptadorPeces, g, myGrid.returnPosicioOfPeca(g));
        peca.estatPeca = CANVIA_ESTAT;
        peca.estatPecaNext = APAREIX;
        peca.bpecaActiva = false;
        peca.idleTimer = IDLE_TIMER_PECA;
        peca.tocadaTimer = TOCADA_TIMER_PECA;
        pecesMovTut.push_back(peca);
    }
}

void ofApp::updatePeces(){
    int timeElapsed = (MAX_GAME_TIME*1000) - jocMinutsTimer.getTimeLeftInMillis();
    pecaEmpty aux;
    //ofVec2f angleVec;
    vector<pecaEmpty>::iterator it;
    for(it = peces.begin(); it != peces.end(); ) {
        if(it->distanceInTime<=timeElapsed) {
            aux = (*it);
            aux.bpecaActiva = true;
            pecesPantalla.push_back(aux);
            it = peces.erase(it);
        }
        else it++;
    }
    bool onBeat = isOnBeat();
    for(it = pecesPantalla.begin(); it != pecesPantalla.end(); it++) {
        it->updatem(warpMousePos);
        it->update(totalBlobsDetected, posicionsBlobs, onBeat);
        it->pecaPos = (it->pecaPos + (it->speedBonus*(it->speed)));
        if(it->angle >= 2*PI) it->angle = 0;
        if(it->rot!=0) {
            it->angle += it->rot*PI;
            it->speed.x = it->speed.x + cos(it->angle);
            it->speed.y = it->speed.y + sin(it->angle);
        }
        it->speed.normalize();
        //cout<<"x"<<it->speed.x<<endl;
        //cout<<"y"<<it->speed.y<<endl;
        cout<<"a"<<it->angle<<endl;
    }
}

bool ofApp::isOnBeat() {
    int timeElapsed = (MAX_GAME_TIME*1000) - jocMinutsTimer.getTimeLeftInMillis();
    //cout << "songpeak: " << songPeaks.at(nextSongPeakIndex);
    //cout << " /  " << timeElapsed << endl;

    if((songPeaks.at(nextSongPeakIndex) + 30) >= timeElapsed && ((songPeaks.at(nextSongPeakIndex) - 30) <= timeElapsed)) {
        //cout << "oye we found it!!!!!!!!!!!!!!!!!!!" << endl;
        nextSongPeakIndex++;
        return true;
    }
    return false;
}

void ofApp::updatePecesTut1(){
    pecaEmpty aux;
    vector<pecaEmpty>::iterator it;
    for(it = pecesTut.begin(); it != pecesTut.end(); ) {
        if(it->distanceInTime<=tutoTimer.getTime()) {
            aux = (*it);
            aux.bpecaActiva = true;
            pecesPantalla.push_back(aux);
            it = pecesTut.erase(it);
        }
        else it++;
    }
    for(it = pecesPantalla.begin(); it != pecesPantalla.end(); it++) {
        it->updatem(warpMousePos);
        it->update(totalBlobsDetected, posicionsBlobs, false);
    }
}

void ofApp::updatePecesTut2(){
    pecaEmpty aux;
    vector<pecaEmpty>::iterator it;
    for(it = pecesMovTut.begin(); it != pecesMovTut.end(); ) {
        if(it->distanceInTime<=tutoTimer.getTime()) {
            aux = (*it);
            aux.bpecaActiva = true;
            pecesPantalla.push_back(aux);
            it = pecesMovTut.erase(it);
        }
        else it++;
    }
    for(it = pecesPantalla.begin(); it != pecesPantalla.end(); it++) {
        it->updatem(warpMousePos);
        it->update(totalBlobsDetected, posicionsBlobs, false);
        it->pecaPos = (it->pecaPos + (it->speed));
    }
}

void ofApp::updatePecesTut3(){
    pecaEmpty aux;
    vector<pecaEmpty>::iterator it;
    for(it = pecesTut.begin(); it != pecesTut.end(); ) {
        if(it->distanceInTime<=tutoTimer.getTime()) {
            aux = (*it);
            aux.bpecaActiva = true;
            pecesPantalla.push_back(aux);
            it = pecesTut.erase(it);
        }
        else it++;
    }
    bool onBeat = isOnBeat();
    for(it = pecesPantalla.begin(); it != pecesPantalla.end(); it++) {
        it->updatem(warpMousePos);
        it->update(totalBlobsDetected, posicionsBlobs, false);
    }

    for(it = pecesMovTut.begin(); it != pecesMovTut.end(); ) {
        if(it->distanceInTime<=tutoTimer.getTime()) {
            aux = (*it);
            aux.bpecaActiva = true;
            pecesPantalla.push_back(aux);
            it = pecesMovTut.erase(it);
        }
        else it++;
    }
    for(it = pecesPantalla.begin(); it != pecesPantalla.end(); it++) {
        it->updatem(warpMousePos);
        it->update(totalBlobsDetected, posicionsBlobs, false);
        //it->pecaPos = it->pecaPos.dot(it->speed);
        it->pecaPos = (it->pecaPos + 3*(it->speed));
    }
}

/*
void ofApp::updatePecesTut4()
{
    pecaEmpty aux;
    vector<pecaEmpty>::iterator it;
    for(it = pecesTut.begin(); it != pecesTut.end(); ) {
        if(it->distanceInTime<=tutoTimer.getTime()) {
            aux = (*it);
            aux.bpecaActiva = true;
            pecesPantalla.push_back(aux);
            it = pecesTut.erase(it);
        }
        else it++;
    }
    for(it = pecesPantalla.begin(); it != pecesPantalla.end(); it++) {
        it->updatem(warpMousePos);
        it->update(totalBlobsDetected, posicionsBlobs);
    }
}*/


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
  /*  else if(pantallaJoc == HIGH_SCORES)
        return "HIGH_SCORES";*/
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

void ofApp::drawTutMsg(){
    ofRectangle start;
    switch(pantallaJoc)
    {
        case(TUTORIAL_1):
            start = saltingTypo.getStringBoundingBox("Touch the dots! (" + ofToString(singleton->getPuntuacioJugador()) + "/5)",0,0);
            ofPushMatrix();
            ofTranslate(220,75);
            ofSetColor(saltingBlue);
            saltingTypo.drawString("Touch the dots! (" + ofToString(singleton->getPuntuacioJugador()) + "/8)",0,0);
            ofPopMatrix();
            max_score_tutorial = 8;
            current_score_tutorial = singleton->getPuntuacioJugador();
            break;
        case(TUTORIAL_2):
            start = saltingTypo.getStringBoundingBox("Touch the dots! (" + ofToString(singleton->getPuntuacioJugador()) + "/8)",0,0);
            ofPushMatrix();
            ofTranslate(220,75);
            ofSetColor(saltingBlue);
            saltingTypo.drawString("Touch the dots! (" + ofToString(singleton->getPuntuacioJugador()) + "/8)",0,0);
            max_score_tutorial = 8;
            ofPopMatrix();
            current_score_tutorial = singleton->getPuntuacioJugador();
            break;
        case(TUTORIAL_3):
            start = saltingTypo.getStringBoundingBox("Touch the dots! (" + ofToString(singleton->getPuntuacioJugador()) + "/15)",0,0);
            ofPushMatrix();
            ofTranslate(220,75);
            ofSetColor(saltingBlue);
            saltingTypo.drawString("Touch the dots! (" + ofToString(singleton->getPuntuacioJugador()) + "/15)",0,0);
            ofPopMatrix();
            max_score_tutorial = 15;
            current_score_tutorial = singleton->getPuntuacioJugador();
            break;
    }
}

void ofApp::drawSkipMessage(int tuto_phase){
    ofRectangle start;
    if(max_score_tutorial - current_score_tutorial == 0)
    {
        start = saltingTypo.getStringBoundingBox("You succesfully completed stage " + ofToString(tuto_phase) + "!\n\n             Errors: " + ofToString(max_score_tutorial - current_score_tutorial),0,0);
        ofPushMatrix();
        ofTranslate(85 ,250);
        ofSetColor(saltingBlue);
        saltingTypo.drawString("You succesfully completed stage " + ofToString(tuto_phase) + "!\n\n              Errors: " + ofToString(max_score_tutorial - current_score_tutorial) +"\n\n\n\n\t          Congratulations!",0,0);
        ofPopMatrix();
    }

    else
    {
        start = saltingTypo.getStringBoundingBox("You succesfully completed stage " + ofToString(tuto_phase) + "!\n\n             Errors: " + ofToString(max_score_tutorial - current_score_tutorial),0,0);
        ofPushMatrix();
        ofTranslate(85 ,250);
        ofSetColor(saltingBlue);
        saltingTypo.drawString("You succesfully completed stage " + ofToString(tuto_phase) + "!\n\n              Errors: " + ofToString(max_score_tutorial - current_score_tutorial) + "\n\n\n\n\t         It could be better!",0,0);
        ofPopMatrix();

    }


}

void ofApp::drawInstructions(){
    string instr = "Tutorial";
    ofRectangle instructions;
    instructions = saltingTypo.getStringBoundingBox(instr,0,0);
    ofPushMatrix();
    ofTranslate(APP_WIDTH_MEITAT-instructions.width*0.5 +215,80);
    ofSetColor(saltingBlue);
    saltingTypo.drawString(instr,0,0);
    ofPopMatrix();
}

void ofApp::drawHighScores(){
    string hs = "Scores";
    ofRectangle hscores;
    hscores = saltingTypo.getStringBoundingBox(hs,0,0);
    ofPushMatrix();
    ofTranslate(APP_WIDTH_MEITAT-hscores.width*0.5 + -170,80);
    ofSetColor(saltingBlue);
    saltingTypo.drawString(hs,0,0);
    ofPopMatrix();
}

void ofApp::drawReturn(){
    string hs = "Return";
    ofRectangle hscores;
    hscores = saltingTypo.getStringBoundingBox(hs,0,0);
    ofPushMatrix();
    ofTranslate(APP_WIDTH_MEITAT-hscores.width*0.5 + -290,610);
    ofSetColor(saltingBlue);
    saltingTypo.drawString(hs,0,0);
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::drawEnd(){
    int aux = NUM_PECES_TOTAL;
    string s = "GREAT JOB!! " + ofToString(singleton->getPuntuacioJugador()) + " POINTS";
    ofRectangle rs;
	rs = saltingTypo.getStringBoundingBox(s,0,0);
	ofPushMatrix();
    ofTranslate(APP_WIDTH_MEITAT-rs.width*0.5,APP_HEIGT_MEITAT-rs.height*0.5);
    ofSetColor(255);
    saltingTypo.drawString(s,0,0);
    ofPopMatrix();
}

void ofApp::setupCamera() {

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

}

void ofApp::setupDebugGUI() {

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

    //Boto Back
    guib->addIntSlider("botoBackX", 0, APP_WIDTH, &botoBack.botoX)->setIncrement(1);
    guib->addIntSlider("botoBackY", 0, APP_HEIGT, &botoBack.botoY)->setIncrement(1);

    //Boto Return
    guib->addIntSlider("botoReturnX", 0, APP_WIDTH, &botoReturn.botoX)->setIncrement(1);
    guib->addIntSlider("botoReturnY", 0, APP_HEIGT, &botoReturn.botoY)->setIncrement(1);

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
    camID = 0;
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
}
