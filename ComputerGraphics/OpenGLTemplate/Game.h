#pragma once

#include "Common.h"
#include "GameWindow.h"

// Classes used in game.  For a new class, declare it here and provide a pointer to an object of this class below.  Then, in Game.cpp, 
// include the header.  In the Game constructor, set the pointer to NULL and in Game::Initialise, create a new object.  Don't forget to 
// delete the object in the destructor.   
class CCamera;
class CPlayer;
class CSkybox;
class CShader;
class CShaderProgram;
class CPlane;
class CFreeTypeFont;
class CHighResolutionTimer;
class CSphere;
class COpenAssetImportMesh;
class CAudio;
class CHouse;
class CPentagon;
class CCatmullRom;
class CHeightMapTerrain;

class Game {
private:
	// Three main methods used in the game.  Initialise runs once, while Update and Render run repeatedly in the game loop.
	void Initialise();
	void Update();
	void Render();

	// Pointers to game objects.  They will get allocated in Game::Initialise()
	CSkybox *m_pSkybox;
	CCamera *m_pCamera;
	CPlayer *m_pPlayer;
	vector <CShaderProgram *> *m_pShaderPrograms;
	CPlane *m_pPlanarTerrain;
	CFreeTypeFont *m_pFtFont;
	COpenAssetImportMesh *m_pBarrelMesh;
	COpenAssetImportMesh *m_pHorseMesh;
	COpenAssetImportMesh *m_pJeepMesh;
	COpenAssetImportMesh *m_pCarMesh;
	COpenAssetImportMesh *m_pMilitaryMesh;
	COpenAssetImportMesh *m_pTreeMesh;
	CSphere *m_pSphere;
	CHighResolutionTimer *m_pHighResolutionTimer;
	CAudio *m_pAudio;
	CHouse *m_pHouse;
	CPentagon *m_pPentagon;
	CCatmullRom* m_pCatmullRom;
	CHeightMapTerrain *m_pHeightmapTerrain;

	// Some other member variables
	double m_dt;
	int m_framesPerSecond;
	int health = 100;
	std::string healthText;
	int minutes = 0;
	int seconds = 0;
	int milliseconds = 0;
	std::string timeString;
	bool m_appActive;


public:
	Game();
	~Game();
	static Game& GetInstance();
	LRESULT ProcessEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param);
	void SetHinstance(HINSTANCE hinstance);
	WPARAM Execute();

private:
	static const int FPS = 60;
	void DisplayFrameRate();
	void GameLoop();
	GameWindow m_gameWindow;
	HINSTANCE m_hInstance;
	int m_frameCount;
	double m_elapsedTime;
	double m_damageTimer;
	bool renderJeep;
	bool damage;
	float playerMovement;
	float m_currentDistance;
	float m_car1Distance;
	float m_car2Distance;
	float m_car3Distance;
	float m_military1Distance;
	float m_military2Distance;
	float m_military3Distance;
	glm::vec3 playerPosition;
	glm::mat4 playerForward;

	glm::vec3 carPosition1;
	glm::mat4 carForward1;
	glm::vec3 carPosition2;
	glm::mat4 carForward2;
	glm::vec3 carPosition3;
	glm::mat4 carForward3;

	glm::vec3 militaryPosition1;
	glm::mat4 militaryForward1;
	glm::vec3 militaryPosition2;
	glm::mat4 militaryForward2;
	glm::vec3 militaryPosition3;
	glm::mat4 militaryForward3;

	float car2Movement;
	float car3Movement;

	float military2Movement;
	float military3Movement;

	bool car2Switch;
	bool car3Switch;
	bool military2Switch;
	bool military3Switch;


};
