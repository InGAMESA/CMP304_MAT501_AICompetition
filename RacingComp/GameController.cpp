#include "GameController.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include <chrono>
#include <format>

GameController::GameController()
{
    font.openFromFile("Assets/DefaultAriel.ttf");
}

GameController::~GameController()
{

    for (auto& d : drivers)
    {
        delete(d);
    }

}



/*

INITIALISATION

*/


void GameController::Init()
{

    grassColA = sf::Color(144,166, 111);
    grassColB = sf::Color(123, 142, 95);
    rumbleColA = sf::Color(255, 0, 0);
    rumbleColB = sf::Color(255, 255, 255);

    trackColA = sf::Color(20, 20, 20);
    trackColB = sf::Color(40, 40 , 40);

    LoadTextures();
    LoadLevelData();

    InitTrack();
    InitCars();
    InitCamera();

    InitAI();
    SetupDrivers();

    // start game straight away 
    gameRunning = true;
   
}

/*
* 
* LoadLevelData will populate the track data according to the currnetly selected track 'trackSelected'
*/
void GameController::LoadLevelData()
{
    std::string yDataPath;
    std::string curveDataPath;
    std::string spriteDataPath;

    if (trackSelected == TrackData::Track1)
    {
        yDataPath = "Assets/Level1TrackYData.csv";
        curveDataPath = "Assets/Level1TrackCurveData.csv";
        spriteDataPath = "Assets/Level1SpriteData.csv";

    }else if (trackSelected == TrackData::Track2)
    {
        yDataPath = "Assets/Level2TrackYData.csv";
        curveDataPath = "Assets/Level2TrackCurveData.csv";
        spriteDataPath = "Assets/Level2SpriteData.csv";

    }
    else if (trackSelected == TrackData::Track3)
    {
        yDataPath = "Assets/Level3TrackYData.csv";
        curveDataPath = "Assets/Level3TrackCurveData.csv";
        spriteDataPath = "Assets/Level3SpriteData.csv";

    }
    else
    {
        std::cout << "ERROR - no level data selected" << std::endl;
        return;

    }



    // Read from the text file
    std::ifstream levelYDataFile(yDataPath);
 

    std::string fileLine;

    levelData.y.assign(trackLength, 0.f);
    levelData.curve.assign(trackLength, 0.f);
   
    
    int i = 0;

    while (std::getline(levelYDataFile, fileLine)) {
       
        levelData.y[i] = std::stof(fileLine);
        i++;
       
    }

 
    levelYDataFile.close();


    std::ifstream levelCurveDataFile(curveDataPath);

    i = 0;

    while (std::getline(levelCurveDataFile, fileLine)) {

        levelData.curve[i] = std::stof(fileLine);
        i++;

    }


    levelCurveDataFile.close();


    std::ifstream levelSpriteDataFile(spriteDataPath);

    while (std::getline(levelSpriteDataFile, fileLine)) {

        LevelSpriteData lsd;

        std::vector<std::string> result;
        size_t start = 0, end;

        while ((end = fileLine.find(',', start)) != std::string::npos) {
            result.push_back(fileLine.substr(start, end - start));
            start = end + 1;
        }
        result.push_back(fileLine.substr(start));
       
        if (result.size() != 3)
        {
            std::cout << "ERROR parsing level sprite data" << std::endl;
        }else {

            lsd.seg = std::stof(result[0]);
            lsd.spriteID = std::stof(result[1]);
            lsd.x = std::stof(result[2]);

            levelData.sprites.push_back(lsd);
        }
       

    }

    levelSpriteDataFile.close();



}

void GameController::InitCamera()
{
    camera.speed = cars[activeCameraCar].inputAccelAxis;
    camera.cameraDepth = cameraDOF;
    camera.cameraHeight = cameraH;
    camera.position = cars[activeCameraCar].z - cameraFollowDist;
    camera.playerX = cars[activeCameraCar].x;
}

/*
InitTrack will create the Segments used in the track as well as the obstacles and scenery
*/
void GameController::InitTrack()
{
    for (int i = 0; i < trackLength; i++)
    {
        Segment s;

        s.curve = levelData.curve[i];
        s.y = levelData.y[i]*100; // convert to world units
 


        if (i % 2 == 0)
        {
            s.roadColour = trackColA;
            s.rumbleColour = rumbleColA;
            s.grassColour = grassColA;
        }
        else
        {
            s.roadColour = trackColB;
            s.rumbleColour = rumbleColB;
            s.grassColour = grassColB;
        }
      


        std::vector<LevelSpriteData> segSprites = getLevelSpriteDataForSegment(i);


        for (auto ss : segSprites)
        {
            SpriteInstance si;
            si.offsetX = ss.x;
            si.type = (ObstacleType)ss.spriteID;
            si.texture = GetTextureByID(ss.spriteID);

            s.sprites.push_back(si);
             
        }
     

        track.segments.push_back(s);

    }

    track.segmentLength = segmentLen;
 
}



void GameController::LoadTextures()
{

    textureCarRed.assign(5, sf::Texture());
    bool load = textureCarRed[0].loadFromFile("Assets/RedCarLL.png");
    load = textureCarRed[1].loadFromFile("Assets/RedCarL.png");
    load = textureCarRed[2].loadFromFile("Assets/RedCarC.png");
    load = textureCarRed[3].loadFromFile("Assets/RedCarR.png");
    load = textureCarRed[4].loadFromFile("Assets/RedCarRR.png");

    textureCarYellow.assign(5, sf::Texture());

    load = textureCarYellow[0].loadFromFile("Assets/YellowCarLL.png");
    load = textureCarYellow[1].loadFromFile("Assets/YellowCarL.png");
    load = textureCarYellow[2].loadFromFile("Assets/YellowCarC.png");
    load = textureCarYellow[3].loadFromFile("Assets/YellowCarR.png");
    load = textureCarYellow[4].loadFromFile("Assets/YellowCarRR.png");

    load = textureBush.loadFromFile("Assets/Bush.png");
    load = textureTree.loadFromFile("Assets/Tree.png");
    load = textureTrash.loadFromFile("Assets/Trash.png");
    load = textureOil.loadFromFile("Assets/Oil.png");
    load = textureTurnLeft.loadFromFile("Assets/TurnLeft.png");
    load = textureTurnRight.loadFromFile("Assets/TurnRight.png");
    load = textureBollards.loadFromFile("Assets/Bollards.png");
    load = textureBackground.loadFromFile("Assets/BackgroundMountainsFull2.png");

    load = textureDust.loadFromFile("Assets/DustCloud.png");
    load = textureExplosion.loadFromFile("Assets/Explosion.png");
    load = textureSmoke.loadFromFile("Assets/Smoke.png");

    backgroundSprite = new sf::Sprite(textureBackground);
    backgroundSprite->setOrigin(sf::Vector2f(1024, 0));
}


/*
*/
void GameController::InitCars()
{
    for (int i = 0; i < numCars; i++)
    {
        Car c;

        c.isPlayer = false;

        // if more than 2 cars, stagger the Z like a grid
        c.z = (4+(i/2)) * segmentLen;

        if (i % 2 == 0)
            c.x = -0.5;
        else
            c.x = 0.5;


        c.speed = 0;
        c.collisionCount = 0;
        c.lastCollisionSegment = 0;

        c.inputAccelAxis = 0;

        c.steerZone = 2; // centre

        c.isActive = true;

        cars.push_back(c);
    }
}

void GameController::InitAI()
{
    //DefineAI will initialise all of the Actions, Considerations and Options used by the AI
    rbsAIConstructor.Init();
    rbsAIConstructor.DefineAI();

    fsmAIConstructor.Init();
    fsmAIConstructor.DefineAI();

    dtAIConstructor.Init();
    dtAIConstructor.DefineAI();

    btAIConstructor.Init();
    btAIConstructor.DefineAI();

    mctsAIConstructor.Init();
    mctsAIConstructor.DefineAI();


}

void GameController::SetupDrivers()
{
 
    // to add more drivers, update driverNames and driverAIMethod. Both must be the same length.


    if (driverNames.size() != cars.size() || driverAIMethod.size() != cars.size())
    {
        std::cout << "ERROR in Driver Setup - driver to car number mismatch Cars = " << cars.size() << std::endl;
        return;
    }

    for (int d = 0; d < numCars; d++)
    {
           CreateAIDriver(driverNames[d], cars[d], driverAIMethod[d]);
    }


}

/*
CreateAIDriver will initialise a new AIActor_Driver with the provided AI method's constructor
The driver will be allocated to the provided car

*/
void GameController::CreateAIDriver(std::string _actorID, Car& _car, AIControllerType _aiController)
{
    AIActor_Driver* _d = new AIActor_Driver();

    // setup AI based on method and assign colors
    if (_aiController == AIControllerType::FSM)
    {
        _d->Init(_actorID, fsmAIConstructor);
        _car.texture = &textureCarYellow[2];
    }
    else if (_aiController == AIControllerType::RBS)
    {
        _d->Init(_actorID,rbsAIConstructor);
        _car.texture = &textureCarYellow[2];
        

    }
    else if (_aiController == AIControllerType::BT)
    {
        _d->Init(_actorID, btAIConstructor);
        _car.texture = &textureCarYellow[2];


    }
    else if (_aiController == AIControllerType::DT)
    {
        _d->Init(_actorID, dtAIConstructor);
        _car.texture = &textureCarYellow[2];


    }
    else if (_aiController == AIControllerType::MCTS)
    {
        _d->Init(_actorID, mctsAIConstructor);
        _car.texture = &textureCarYellow[2];

    }
    else
    {

        std::cout << "[ERROR] CreateAIDriver - no AI Constructor for controller type" << std::endl;

        return;
    }

    _car.controllerMethod = (int)_aiController;

    _d->SetDriverCar(_car);
    _d->AddBBValue("TrackCentrePos", 0);
    _d->SetGameContext(*this);

    
    drivers.push_back(_d);

}


/*

LOOKUPS and REFERENCE

*/


/*
GetTextureByID will return the texture with the associated texuture ID
*/
sf::Texture* GameController::GetTextureByID(int ID)
{
    if (ID == 0)
    {
        return &textureOil;
    }else if (ID == 1)
    {
        return &textureBollards;
    }else if (ID == 2)
    {
        return &textureTrash;
    }
    else if (ID == 3)
    {
        return &textureBush;
    }
    else if (ID == 4)
    {
        return &textureTree;
    }

    std::cout << "[ERROR] GetTextureByID cannot find txture for ID " << ID << std::endl;
    return nullptr;
}

std::vector<LevelSpriteData> GameController::getLevelSpriteDataForSegment(int segment)
{

    std::vector<LevelSpriteData> segSpriteData;

    for (int i = 0; i < levelData.sprites.size(); i++)
    {
        if (levelData.sprites[i].seg == segment)
            segSpriteData.push_back(levelData.sprites[i]);
    }

    return segSpriteData;
}


/*
Project will compute the ProjectedPoint for the provided world, screen and camera parameters
*/
ProjectedPoint GameController::Project(float worldX, float worldY, float worldZ, const Camera& cam, float screenW, float screenH, float roadWidth) {

    float camZ = cam.position;

    float dz = worldZ - camZ;

    if (dz < 1.f) {
        dz = 1.f;
    }

    float scale = cam.cameraDepth / dz;

    ProjectedPoint p;


    p.scale = scale;

    p.screenX = (1.f + scale * (worldX - cam.playerX)) * screenW / 2.f;
    p.screenY = (1.f - scale * (worldY - cam.cameraHeight)) * screenH / 2.f;
    p.width = scale * roadWidth * screenW / 2.f;

    return p;

}

/*
ComputeCurveOffsets will compute the renderContext's for the track
Should be updated each time world is rendered
*/
void GameController::ComputeCurveOffsets(Track& track, int baseSegment)
{
    renderContext.curveOffsetX.assign(drawDistanceInSegments, 0.f);
    float x = 0.f, dx = 0.f;
    float percent = std::fmod(camera.position, track.segmentLength) / track.segmentLength;

    dx = -(track.segments[baseSegment].curve * percent);

    for (int n = 0; n < drawDistanceInSegments; ++n)
    {
        const Segment& seg = track.segments[(baseSegment + n) % track.segments.size()];
        dx += seg.curve;
        x += dx;
        x += dx;
        renderContext.curveOffsetX[n] = x;
    }



}


int GameController::GetSteerZone(float steerAxis)
{


    if (steerAxis < -0.55)
        return 0;
    else  if (steerAxis < -0.25)
        return 1;
    else  if (steerAxis > 0.55)
        return 4;
    else  if (steerAxis > 0.25)
        return 3;
    else
        return 2;
}

/*

INPUT 

*/

/*
* HandleKeyPress will handle the SFML key presses
*/
void GameController::HandleKeyPress(const sf::Event::KeyPressed* _keyPress)
{

   
    if (!gameRunning)
        return;


       
    else if (_keyPress->code == sf::Keyboard::Key::Up)
    {
        activeCameraCar = ++activeCameraCar % numCars;

    }
    else if (_keyPress->code == sf::Keyboard::Key::Down)
    {
        activeCameraCar = activeCameraCar - 1;
        if (activeCameraCar < 0)
            activeCameraCar = numCars-1;

    }

    

}

/*
* HandleKeyPress will handle the SFML mouse presses
*/
void GameController::HandleMousePress(const sf::Event::MouseButtonPressed* _mousePress)
{

    if (_mousePress->button == sf::Mouse::Button::Right)
    {

    }
    else if (_mousePress->button == sf::Mouse::Button::Left)
    {

    }
}






/*

RENDERING AND UI

*/

void GameController::DisplayUIText(sf::RenderWindow& window, std::string _text, float _yPos)
{

    sf::Text text(font); // a font is required to make a text object


    // set the string to display
    text.setString(_text);

    // set the character size
    text.setCharacterSize(28); // in pixels, not points!

    // set the color
    text.setFillColor(sf::Color::White);
    text.setPosition({ 1400,_yPos });

    window.draw(text);


}


void GameController::RenderUI(sf::RenderWindow& window)
{


}

/*
* Render will render the world and HUD
*/
void GameController::Render(sf::RenderWindow& window)
{
    RenderWorld(window, track, camera, cars);
    DrawHUD(window, camera);
}

/*
* RenderWorld will render the Sky/Horizon, Track, Obstacles and Cars
*/
void GameController::RenderWorld(sf::RenderWindow& window, Track& track, const Camera& cam, const std::vector<Car>& cars)
{
    int _baseSegment = track.SegmentIndexAt(cam.position) % track.segments.size();
    int _baseSegmentCar = track.SegmentIndexAt(cars[activeCameraCar].z);


    ComputeCurveOffsets(track, _baseSegment);
    DrawSkyAndHorizon(window, cam);
    DrawRoad(window, track, cam, _baseSegment);
 
}

/*
DrawSkyAndHorizon will draw the far sky/horizon part of the world
*/
void GameController::DrawSkyAndHorizon(sf::RenderWindow& window, const Camera& cam)
{


    float xSky = screenW / 2 - (camCurveX * parallaxMultiplier);

    if (xSky < 0)
        xSky = 0;
    else if (xSky > 1024)
        xSky = 1024;

    backgroundSprite->setPosition(sf::Vector2f(xSky, 0.f));
    window.draw(*backgroundSprite);
}


/*
DrawRoad will render the road and every item along it, including cars
*/
void GameController::DrawRoad(sf::RenderWindow& window, Track& track, const Camera& cam, int baseSegment)
{
  

    ProjectedPoint prevP{};


    for (int n = drawDistanceInSegments - 1; n >= 0; --n)
    {
        const Segment& seg = track.segments[(baseSegment + n) % track.segments.size()];
        float curveX = renderContext.curveOffsetX[n];

        float worldZ = (baseSegment + n) * track.segmentLength;

        ProjectedPoint p = Project(curveX, seg.y, (baseSegment + n) * track.segmentLength,
            cam, screenW, screenH, roadHalfWidth);

        if (n < drawDistanceInSegments - 1 && n>0)
        {
            // draw the track

            if (baseSegment + n == trackLength - 1)
            {
            
                int segs = 10;

                float segWidth1 = abs(prevP.screenX + prevP.width) / segs;
                float segWidth2 = abs(p.screenX + p.width) / segs;


                for (int f = 0; f < segs; f++)
                {
                    sf::ConvexShape quad(4);
                    quad.setPoint(0, { prevP.screenX - prevP.width + (segWidth1 * f) , prevP.screenY});
                    quad.setPoint(1, { prevP.screenX - prevP.width + (segWidth1 * (f+1)), prevP.screenY });
                    quad.setPoint(2, { p.screenX - p.width+(segWidth2 * (f+1)), p.screenY });
                    quad.setPoint(3, { p.screenX - p.width+ (segWidth2 * f) , p.screenY });

                    if(f%2==0)
                        quad.setFillColor(sf::Color::White);
                    else
                        quad.setFillColor(sf::Color::Black);

                    window.draw(quad);
                }

               
            
            }
            else
            {
                sf::ConvexShape quad(4);
                quad.setPoint(0, { prevP.screenX - prevP.width, prevP.screenY });
                quad.setPoint(1, { prevP.screenX + prevP.width, prevP.screenY });
                quad.setPoint(2, { p.screenX + p.width, p.screenY });
                quad.setPoint(3, { p.screenX - p.width, p.screenY });
                quad.setFillColor(seg.roadColour);
                window.draw(quad);
            }


            float rWidth =   p.scale * rumbleWidth * screenW / 2.f;

            // draw the left rumble
            sf::ConvexShape rumbleQuadL(4);
            rumbleQuadL.setPoint(0, { prevP.screenX - prevP.width - rWidth, prevP.screenY });
            rumbleQuadL.setPoint(1, { prevP.screenX - prevP.width, prevP.screenY });
            rumbleQuadL.setPoint(2, { p.screenX - p.width, p.screenY });
            rumbleQuadL.setPoint(3, { p.screenX - p.width - rWidth, p.screenY });
            
            rumbleQuadL.setFillColor(seg.rumbleColour);
            window.draw(rumbleQuadL);

            // draw the right rumble
            sf::ConvexShape rumbleQuadR(4);
            rumbleQuadR.setPoint(0, { prevP.screenX + prevP.width + rWidth, prevP.screenY });
            rumbleQuadR.setPoint(1, { prevP.screenX + prevP.width, prevP.screenY });
            rumbleQuadR.setPoint(2, { p.screenX + p.width, p.screenY });
            rumbleQuadR.setPoint(3, { p.screenX + p.width + rWidth, p.screenY });

            rumbleQuadR.setFillColor(seg.rumbleColour);
            window.draw(rumbleQuadR);

            // draw the left grass
           sf::ConvexShape grassQuadL(4);
            grassQuadL.setPoint(0, { 0, prevP.screenY });
            grassQuadL.setPoint(1, { prevP.screenX - prevP.width - rWidth, prevP.screenY });
            grassQuadL.setPoint(2, { p.screenX - p.width - rWidth, p.screenY });
            grassQuadL.setPoint(3, { 0, p.screenY });

            grassQuadL.setFillColor(seg.grassColour);
            window.draw(grassQuadL);


            // draw the right grass
            sf::ConvexShape grassQuadR(4);
            grassQuadR.setPoint(0, { prevP.screenX + prevP.width + rWidth, prevP.screenY });
            grassQuadR.setPoint(1, { screenW, prevP.screenY });
            grassQuadR.setPoint(2, { screenW, p.screenY });
            grassQuadR.setPoint(3, { p.screenX + p.width + rWidth, p.screenY });

            grassQuadR.setFillColor(seg.grassColour);
            window.draw(grassQuadR);

        }

        for (auto& s : seg.sprites)
        {
            float worldX = s.offsetX * roadHalfWidth + curveX;

            DrawScaledSprite(window, s.texture, worldX, seg.y, worldZ, cam);

        }



        for (auto& car : cars)
        {

     
                if (track.SegmentIndexAt(car.z) == (baseSegment + n) % track.segments.size())
                {

                   float curTrackX = renderContext.curveOffsetX[n];
                   float nextTrackX = renderContext.curveOffsetX[std::min(n+1, drawDistanceInSegments -1)];

                   float percent = std::fmod(car.z, track.segmentLength) / track.segmentLength;

                    float carCurveX = std::lerp(curTrackX, nextTrackX, percent);



                    float worldX = car.x * roadHalfWidth  + carCurveX;

                    float curTrackHeight = track.segments[(baseSegment + n ) % track.segments.size()].y;
                    float nextTtrackHeight = track.segments[(baseSegment + n + 1 )% track.segments.size()].y;

                    float carY =  std::lerp(curTrackHeight, nextTtrackHeight, percent) ;


                    DrawScaledSprite(window, car.texture, worldX, carY, car.z, cam);


                    if (car.isActiveCollision)
                    {
                        DrawScaledSprite(window, &textureExplosion, worldX, carY, car.z, cam);
                    }

                    if (car.isActiveRumble)
                    {
                        DrawScaledSprite(window, &textureDust, worldX, carY, car.z, cam);
                    }
                }

        }

        prevP = p;

    }

}


void GameController::DrawScaledSprite(sf::RenderWindow& window, sf::Texture* tex, float worldX, float worldY, float worldZ, const Camera& cam)
{
    ProjectedPoint p = Project(worldX, worldY, worldZ, cam, screenW, screenH, roadHalfWidth);

    sf::Sprite sprite(*tex);
    float scale = p.scale * spriteScaleConstant;
    sprite.setScale(sf::Vector2f(scale, scale));
    sprite.setOrigin(sf::Vector2f(tex->getSize().x / 2.f, tex->getSize().y));
    sprite.setPosition(sf::Vector2f(p.screenX, p.screenY));

    window.draw(sprite);
}



void GameController::DrawHUD(sf::RenderWindow& window, const Camera& cam)
{
    sf::Text text(font); // a font is required to make a text object
    // set the string to display

    std::string xText = "Viewing Car :";
    xText.append(std::to_string(activeCameraCar));
    xText.append(" driver : ");
    xText.append(drivers[activeCameraCar]->GetActorID());
    xText.append(" time : ");
    xText.append(std::to_string((int)timer));

    text.setString(xText);
    text.setFillColor(sf::Color::Black);

    // set the character size
    text.setCharacterSize(22); // in pixels, not points

    window.draw(text);

    std::string xText2 = " - ";

    
    xText2.append(" speed : ");
    xText2.append(std::to_string((int)cars[activeCameraCar].speed));
    xText2.append(" collisions : ");
    xText2.append(std::to_string(cars[activeCameraCar].collisionCount));

    xText2.append(" steer axis : ");
    xText2.append(std::to_string(cars[activeCameraCar].inputSteerAxis));
    xText2.append(" accel axis : ");
    xText2.append(std::to_string(cars[activeCameraCar].inputAccelAxis));

    text.setString(xText2);
    text.setPosition(sf::Vector2f(0, 30));

    window.draw(text);

}

/*

UPDATE FUNCTIONS

*/

/*
UpdateCamera will position and move the camera relateive to the car being viewed
*/
void GameController::UpdateCamera(Camera& cam, Track& track, float dt)
{

    // match the activeCameraCar's position and speed
    cam.speed = cars[activeCameraCar].inputAccelAxis;
    cam.position = cars[activeCameraCar].z - cameraFollowDist;

    if(renderContext.curveOffsetX.size()>0)
        camCurveX += renderContext.curveOffsetX[0] * dt;

    // move camera forward
    cam.position += cam.speed * dt;

    
    //  to ensure smooth tracking, lerp between the current and next segemnt height
    int _baseSegment = track.SegmentIndexAt(cam.position);
    float trackHeight = track.segments[_baseSegment % track.segments.size()].y;
    float nextTtrackHeight = track.segments[(_baseSegment + 1 )% track.segments.size()].y;
    float percent = std::fmod(cam.position, track.segmentLength) / track.segmentLength;

    cam.cameraHeight = std::lerp(trackHeight, nextTtrackHeight, percent) + cameraH;

    // return to start of track if reach end
    if (cam.position >= track.TotalLength())
    {
        cam.position = cam.position -= track.TotalLength();
    }

    
}

// UNUSED AT PRESENT
void GameController::UpdatePlayerSteering(Camera& cam, float dt)
{

    int _camSeg = track.SegmentIndexAt(cam.position); 
    cam.playerX = cars[activeCameraCar].x;


}

/*
* isRaceOver will return true if all cars have completed the track, otherwise false
*/ 
bool GameController::isRaceOver()
{

    for (auto& c : cars)
    {
        if (c.isActive)
            return false;
    }

    return true;

}

void GameController::ApplyUpdateSteering(float dt)
{
    for (auto& c : cars)
    {
        // Deacceleration
        float _amount = c.updateDecelChange;

        if (_amount > maxDecelChange)
            _amount = maxDecelChange;
        else if (_amount < 0)
            _amount = 0;


        c.inputAccelAxis -= _amount;

        if (c.inputAccelAxis > maxAccelSpeed)
            c.inputAccelAxis = maxAccelSpeed;

        if (c.inputAccelAxis < -maxDecelSpeed)
            c.inputAccelAxis = -maxDecelSpeed;

        // Acceleration

        _amount = c.updateAccelChange ;


        if (_amount > maxAccelChange)
            _amount = maxAccelChange;
        else if (_amount < 0)
            _amount = 0;

        c.inputAccelAxis += _amount;

        if (c.inputAccelAxis > maxAccelSpeed)
            c.inputAccelAxis = maxAccelSpeed;

        if (c.inputAccelAxis < -maxDecelSpeed)
            c.inputAccelAxis = -maxDecelSpeed;

   
        // Turn

        _amount = c.updateSteerChange ;

        if (_amount > maxTurnSpeed)
            _amount = maxTurnSpeed;
        else if (_amount < -maxTurnSpeed)
            _amount = -maxTurnSpeed;

    
          c.inputSteerAxis += _amount;
  
 

        if (c.inputSteerAxis > carTurnBound)
            c.inputSteerAxis = carTurnBound;

        if (c.inputSteerAxis < -carTurnBound)
            c.inputSteerAxis = -carTurnBound;

     

        c.updateAccelChange = 0;
        c.updateDecelChange = 0;
        c.updateSteerChange = 0;
    }


    // update the driver's blackboard with the new values

    for (auto& d : drivers)
    {
        d->RefreshActionBB();
    }


}

/*
UpdateOpponents will update all AI driver's cars based on their steer and acceleration axis
*/
void GameController::UpdateOpponents( float dt)
{
    for (auto &c : cars)
    {

        if (!c.isActive)
            continue;


        // get update change values from car

           
        int _camSeg = track.SegmentIndexAt(c.z);
        int _steerZone = GetSteerZone(c.inputSteerAxis);

        // update the sprite to match the steering angle
        if (_steerZone != c.steerZone)
        {
            if(c.controllerMethod==(int)AIControllerType::FSM)
                c.texture = &textureCarRed[_steerZone];
            else  if (c.controllerMethod == (int)AIControllerType::RBS)
                c.texture = &textureCarYellow[_steerZone];

            c.steerZone = _steerZone;
        }


        // change speed based on the car's accel axis value
        c.speed += c.inputAccelAxis*dt;

        if (c.speed > aiMaxSpeed)
            c.speed = aiMaxSpeed;
        else if (c.speed < aiMinSpeed)
            c.speed = aiMinSpeed;




        // modify speed if car is on rumble strip
        float rumbleBound = roadHalfWidth * rumblePos;

        if (c.x * roadHalfWidth >= rumbleBound || c.x * roadHalfWidth <= -rumbleBound)
        {
            c.isActiveRumble = true;
        


            if (c.speed > rumbleMaxSpeed)
                c.speed -= rumbleDecelSpeed*dt;

        }
        else
        {
            c.isActiveRumble = false;
        }


        // move car forward based on speed
        c.z += c.speed * dt;

        // move car left/right based on steering axis
        c.x += c.inputSteerAxis*dt;


        // gradually return the steer axis to the centre zone
        if (c.inputSteerAxis > steerCentreBuffer)
        {
            c.inputSteerAxis += -steerBalance * dt;
        }
        else  if (c.inputAccelAxis < -steerCentreBuffer)
        {
            c.inputSteerAxis =+ steerBalance * dt;

        }
       

        // apply a drift on cornering to pull car

        float _speedRatio = c.speed / aiMaxSpeed;

        float _drift = track.segments[_camSeg % track.segments.size()].curve * centreForce * _speedRatio * dt;
        c.x -= _drift ;

        // if car is skilling, apply affect
       if (c.isSkidding)
        {

           // skid a random amount
           float _skidAmount = AIRandom_Global::getInstance()->GetRandomFloat(collideOilSkidMin, collideOilSkidMax);
            if (c.inputSteerAxis >= 0)
                c.x += _skidAmount * dt;
            else  if (c.inputSteerAxis < 0)
                c.x -= _skidAmount * dt;

        }

       // check bounds
        if (c.x > carXBound)
            c.x = carXBound;
        else if (c.x < -carXBound)
            c.x = -carXBound;
         
        
    }
}

/*
CheckFinishLine will return true
*/
bool GameController::CheckFinishLine(int _trackSegment)
{

    if (_trackSegment >= trackLength - 1)
        return true;
    else
        return false;

}

/*

*/


/*
UpdateAIDrivers will perfrm the AI Driver's Sense() to Act() if the actionDelay timer has expired
*/
void GameController::UpdateAIDrivers(float dt)
{
    if (clock.getElapsedTime().asSeconds() < actionDelay)
        return;


    UpdateAIDriverBB();

    PerformAIDriverActions(dt);

    clock.restart();

}


/*
UpdateAIDriverBB will update all active driver's blackboard
*/
void GameController::UpdateAIDriverBB()
{
    for (int d = 0; d < drivers.size(); d++)
    {

        if (!cars[d].isActive)
            continue;

        std::vector<float> _trackCurve;
        std::vector< AIMath_Global::Vector2f > _oil;
        std::vector< AIMath_Global::Vector2f > _bollard;
        std::vector< AIMath_Global::Vector2f > _trash;

        int _carSegment = track.SegmentIndexAt(cars[d].z);

        bool finished = CheckFinishLine(_carSegment);

        if (finished)
        {
            cars[d].finishTime = timer;
            std::cout << "Car " << d << " driven by " << drivers[d]->GetActorID() << " has finished the race with a time of " << timer << std::endl;
            cars[d].isActive = false;
        }

        for (int i = _carSegment, j = 0; i < _carSegment + aiLookAheadDistance; i++, j++)
        {
            _trackCurve.push_back(track.segments[i % trackLength].curve);

            for (auto s : track.segments[i % trackLength].sprites)
            {
                if (s.type == ObstacleType::Oil)
                    _oil.push_back(AIMath_Global::Vector2f(s.offsetX, j));
                else if (s.type == ObstacleType::Bollard)
                    _bollard.push_back(AIMath_Global::Vector2f(s.offsetX, j));
                else if (s.type == ObstacleType::Trash)
                    _trash.push_back(AIMath_Global::Vector2f(s.offsetX, j));
            }
        }

        drivers[d]->EditBBValue("AccelAxis", cars[d].inputAccelAxis);
        drivers[d]->EditBBValue("SteerAxis", cars[d].inputSteerAxis);
        drivers[d]->EditBBValue("Speed", cars[d].speed);
        drivers[d]->EditBBValue<int>("IsRumbleActive", cars[d].isActiveRumble);
        drivers[d]->EditBBValue<int>("IsCollisionActive", cars[d].isActiveCollision);
        drivers[d]->EditBBValue("DistTravelled", cars[d].z);
        drivers[d]->EditBBValue("TrackXPos", cars[d].x);
        drivers[d]->EditBBValue("Timer", timer);

        drivers[d]->EditBBValue<std::vector<float>>("TrackCurve", _trackCurve);
        drivers[d]->EditBBValue<std::vector<AIMath_Global::Vector2f >>("ObstacleOil", _oil);
        drivers[d]->EditBBValue<std::vector<AIMath_Global::Vector2f >>("ObstacleBollard", _bollard);
        drivers[d]->EditBBValue<std::vector<AIMath_Global::Vector2f >>("ObstacleTrash", _trash);

    }

}


/*
PerformAIDriverActions will use the AI Driver's reasoner to perform an Action

*/
void GameController::PerformAIDriverActions(float dt)
{
  
    //upcoming track details


    for (int d=0;d< drivers.size();d++)
    {

        if (!cars[d].isActive)
            continue;

        
        drivers[d]->Update(dt);
    }



}

/*
LogRaceResults should be used to log the race results to a text file
*/
void GameController::LogRaceResults()
{

    // get timestamp and format it to a string
    const std::chrono::time_point<std::chrono::system_clock> now =
        std::chrono::system_clock::now();

    const std::chrono::zoned_time localTime{ std::chrono::current_zone(), now };

    const std::string timestampString = std::format("{:%Y-%m-%d_%H-%M-%S}", localTime);

   // std::cout << "Timestamp is " << timestampString << std::endl;




    std::string _filename = "ResultsData";
    _filename.append(timestampString).append(".csv");
    
    std::ofstream logFile(_filename);
    if (logFile.is_open())
    {
        
        logFile << "DriverID,DriverVersion,RaceCompelteTime,TotalCollisions" << std::endl;
        for (int i = 0; i < cars.size(); i++)
        {
            logFile << drivers[i]->GetActorID() << ",0.1," << cars[i].finishTime << "," << cars[i].collisionCount << std::endl;
        }

        logFile.close();
    }
    else
        std::cout << "Cannot open file" << std::endl;



}
/*
ShowRaceResults will output the results of the race to the console
*/
void GameController::ShowRaceResults()
{
    std::cout << "Race Results " << std::endl;

    for (int i=0;i<cars.size();i++)
    {
        std::cout << "Diver " << drivers[i]->GetActorID() << " - Car Time " << cars[i].finishTime << " Collisions " << cars[i].collisionCount << std::endl;
    }
}


/*
* Update is called each frame and should only contain code designed to be called this often
*/
void GameController::Update(float _dt)
{

    if (gameRunning)
    {     

        dt = _dt;

        // increment the race timer
        timer += dt;

        // update all elements
        UpdateCamera(camera, track, dt);
        UpdateAIDrivers(dt);
        ApplyUpdateSteering(dt);
        UpdateOpponents(dt);
        CheckHazardCollisions();

        if (isRaceOver())
        {
            gameMode = GameMode::Results;
            gameRunning = false;
            ShowRaceResults();
            LogRaceResults();
        }
    }

}

/*

COLLISION AND STATUS

*/


/*CheckHazardCollisions will check all cars for collisions and apply the effects

*/
void GameController::CheckHazardCollisions()
{
       // if car in same segment as hazard, collide

    for (auto& c : cars)
    {
        if (!c.isActive)
            continue;

        int _curSegment = track.SegmentIndexAt(c.z) ;

        if (c.isActiveCollision && _curSegment > c.lastCollisionSegment) {
            //car has moved past point of recent collision
            c.isActiveCollision = false;
            c.isSkidding = false;
        }

        for (auto s : track.segments[_curSegment% track.segments.size()].sprites)
        {
                
            // only Oil, Bollard and Trash are obstacles, other items (bushes etc are decorative)
            if (s.type == ObstacleType::Oil || s.type == ObstacleType::Bollard || s.type == ObstacleType::Trash)
            {
       
                float obstacleX = s.offsetX * roadHalfWidth;
                float carX = c.x * roadHalfWidth;
                float xDif = abs(carX - obstacleX);

                // car within collision distance of obstacle
                if (xDif < collideDistance)
                {
                   
                    // is a new collision
                    if (_curSegment > c.lastCollisionSegment) {

                        c.lastCollisionSegment = _curSegment;
                        c.collisionCount++;
                        c.isActiveCollision = true;

                        // apply the new collision effects
                        if (s.type == ObstacleType::Oil)
                        {
                            c.speed -= collideMinorDeclSpeed;
                            c.isSkidding = true;

                        }
                        else if (s.type == ObstacleType::Bollard)
                        {
                            c.speed -= collideMediumDeclSpeed;
                        }
                        else if (s.type == ObstacleType::Trash)
                        {
                            c.speed -= collideMajorDeclSpeed;
                        }

                    }
                   
                }
                else
                {
                    // car outside collsion distance

                    if (c.isActiveCollision)
                    {
                        c.isActiveCollision = false;
                        c.isSkidding = false;
                    }
                }

            }

        }

    }

}


