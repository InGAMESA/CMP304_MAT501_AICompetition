#pragma once
#include <SFML/Graphics.hpp>
#include "AIConstructor_RBS.h"
#include "AIConstructor_FSM.h"
#include "AIConstructor_BT.h"
#include "AIConstructor_DT.h"
#include "AIConstructor_MCTS.h"
#include "AIBrain_Driver.h"
#include "AIActor_Driver.h"

/*

CMP304/MAT501 Racing Game Example (2026)

*/

// ObstacleType contains the possible obstacles found on the track
enum class ObstacleType {
	Oil = 0,
	Bollard,
	Trash,
	Bush,
	Tree
};

// SpriteInstance contains data for sprites that appear along the track
struct SpriteInstance {
	sf::Texture* texture;
	float offsetX = 0;
	ObstacleType type;
};

// Segment represents one part of the race track
struct Segment {
	float curve;
	float y;
	sf::Color roadColour, rumbleColour, grassColour;
	std::vector<SpriteInstance> sprites;

};

// RenderContext contains data used to ensure between segment transitions are smooth where possible
struct RenderContext {
	std::vector<float> curveOffsetX;
};


// Track contains data for the race track used in the game
struct Track {
	std::vector<Segment> segments;
	float segmentLength;

	float TotalLength() {
		return segmentLength* segments.size();
	}

	int SegmentIndexAt(float pos)
	{
		return pos / segmentLength;
	}

};

//Camera contains data releated to the view camera that will follow a race car
struct Camera {
	float position;
	float playerX;
	float speed;
	float cameraHeight;
	float cameraDepth;

};

// ProjectedPoint contains data related to the world projection system
struct ProjectedPoint {
	float screenX;
	float screenY;
	float scale;
	float width;

};

//LevelSpriteData contains data realted to the scenery and obstacle sprites along the track
struct LevelSpriteData {
	int seg;
	int spriteID;
	float x;
};

//LevelData contains track data as loaded from the track data files
struct LevelData {
	std::vector<float> y;
	std::vector<float> curve;
	std::vector< LevelSpriteData> sprites;
};



class GameController
{

public:
	GameController();
	~GameController();

	// the type of AI controlling the Drivers
	enum class AIControllerType {
		FSM=0, RBS, DT, BT, MCTS
	};

	enum GameMode {
		Initialisation, Race, Results
	};

	enum TrackData {
		Track1, Track2, Track3
	};

	// Initialisation
	void Init();

	//Events and Game Loop
	void Update(float dt);

	void HandleKeyPress(const sf::Event::KeyPressed* _keyPress);
	void HandleMousePress(const sf::Event::MouseButtonPressed* _mousePress);

	// world rendering systems
	ProjectedPoint Project(float worldX, float worldY, float worldZ, const Camera& cam, float screenW, float screenH, float roadWidth);

	// Rendering and UI
	void Render(sf::RenderWindow& window);
	void RenderUI(sf::RenderWindow& window);


	void LoadLevelData();

	// tracking
	float dt = 0;

private:

	// events and game loop
	void UpdateCamera(Camera& cam, Track& track, float dt);
	void UpdatePlayerSteering(Camera& cam, float dt);
	void UpdateOpponents(float dt);
	void PerformAIDriverActions(float dt);
	void UpdateAIDriverBB();
	void UpdateAIDrivers(float dt);
	void ApplyUpdateSteering(float dt);

	// rendering and UI
	void DisplayUIText(sf::RenderWindow& window, std::string _text, float _yPos);
	void DrawSkyAndHorizon(sf::RenderWindow& window, const Camera& cam);
	void DrawRoad(sf::RenderWindow& window, Track& track, const Camera& cam, int baseSegment);
	void DrawScaledSprite(sf::RenderWindow& window, sf::Texture* tex, float worldX, float worldY, float worldZ, const Camera& cam);
	//void DrawPlayerCarSprite(sf::RenderWindow& window, sf::Texture* tex, const Camera& cam); // unused - for player controller cars
	//void DrawPlayerCollideSprite(sf::RenderWindow& window, sf::Texture* tex, const Camera& cam); // unused - for player controller cars
	void DrawHUD(sf::RenderWindow& window, const Camera& cam);
	void RenderWorld(sf::RenderWindow& window, Track& track, const Camera& cam, const std::vector<Car>& cars);

	// initialisation

	void InitCamera();
	void InitTrack();
	void InitCars();
	void InitAI();
	void SetupDrivers();
	void CreateAIDriver(std::string _actorID, Car& _car, AIControllerType _aiController);
	void LoadTextures();
	//void SetCarTexture(); / unused - for player controller cars

	// data and lookops
	void ComputeCurveOffsets(Track& track, int baseSegment);
	void CheckHazardCollisions();
	bool CheckFinishLine(int _trackSegment);
	bool isRaceOver();
	void ShowRaceResults();
	void LogRaceResults();
	std::vector< LevelSpriteData> getLevelSpriteDataForSegment(int segment);
	sf::Texture* GetTextureByID(int ID);
	int GetSteerZone(float steerAxis);

	// The AI constructors
	AIConstructor_RBS rbsAIConstructor;
	AIConstructor_FSM fsmAIConstructor;
	AIConstructor_DT dtAIConstructor;
	AIConstructor_BT btAIConstructor;
	AIConstructor_MCTS mctsAIConstructor;



	//overall setup

	const int trackLength = 400; // segments in track
		
	//screen config
	const float screenW = 1024; // screen pixels
	const float screenH = 640; // screen pixels
	const float horizonY = 320; // screen pixels
	const float spriteScaleConstant = 512.f;

	//camera config
	const float cameraH = 400.f; // world units
	const float cameraDOF = 0.84f; // world units
	const float cameraFollowDist = 500.f; // world units
	const float parallaxMultiplier = 30.f;

	//game world config
	const float roadHalfWidth = 300.f; // world units
	const int drawDistanceInSegments = 200; // world units
	const int segmentLen = 200; // world units
	const float rumbleWidth = 32.f; // world units
	const float aiMaxSpeed = 1300.f; 
	const float aiMinSpeed = 1.f;

	const int aiLookAheadDistance = 20;// how many segments ahead the AI has knowledge of

	//gameplay - rumbles
	const float rumbleDecelSpeed = 400.f;
	const float rumbleMaxSpeed = 600.f;
	const float rumblePos = 0.95f;

	//gameplay - collision
	const float collideMajorDeclSpeed = 500.f;
	const float collideMediumDeclSpeed = 250.f;
	const float collideMinorDeclSpeed = 150.f;
	const float collideOilSkidMin = 0.5f;
	const float collideOilSkidMax = 2.0f;
	const float collideDistance = 128.f;

	//gameplay - driving
	const float centreForce = 0.1f; // per udpate
	const float steerBalance = 0.025f;
	const float steerCentreBuffer = 0.1;

	//bounds and limits
	const float carXBound = 1; // road widths	

	// bounds and limits for the Actions
	const float maxAccelSpeed = 120.f;
	const float maxAccelChange = 30.f;
	const float maxDecelSpeed = 250.f;
	const float maxDecelChange = 40.f;
	const float maxTurnSpeed = 0.5f;
	const float carTurnBound = 1;

	//tracking - camera
	int activeCameraCar = 0;
	float camCurveX = 0;


	//tracking - time and status
	float timer = 0;
	bool gameRunning = false;
	sf::Clock clock;
	float actionDelay = 0.1f; // how often AI make a decision
	GameMode gameMode = GameMode::Initialisation;

	//Objects
	Camera camera;
	Track track;
	std::vector<Car> cars; // all cars in the race, each should have a driver
	std::vector<AIActor_Driver*> drivers; // all AI drivers, each should have a car

	RenderContext renderContext;
	LevelData levelData;
	



	// Graphics
	sf::Font font;

	std::vector<sf::Texture> textureCarYellow;
	std::vector<sf::Texture> textureCarRed;
	sf::Texture textureBush;
	sf::Texture textureTree;
	sf::Texture textureTrash;
	sf::Texture textureOil;
	sf::Texture textureTurnLeft;
	sf::Texture textureTurnRight;
	sf::Texture textureBollards;

	sf::Texture textureSmoke;
	sf::Texture textureDust;
	sf::Texture textureExplosion;


	sf::Texture textureBackground;
	sf::Sprite *backgroundSprite;

	sf::Color grassColA;
	sf::Color grassColB;
	sf::Color rumbleColA;
	sf::Color trackColA;
	sf::Color rumbleColB;
	sf::Color trackColB;








	// *** change the driverAIMethod to determine which AI method will be used in the race ****
	const int numCars = 1;
	std::vector<std::string> driverNames = { "AI Driver" };
	std::vector<AIControllerType> driverAIMethod = { AIControllerType::FSM };



	// *** Set the track to be ued in the race ***
	TrackData trackSelected = TrackData::Track1; // Track1, Track2, Track3





};


