/* 
OpenGL Template for INM376 / IN3005
City University London, School of Mathematics, Computer Science and Engineering
Source code drawn from a number of sources and examples, including contributions from
 - Ben Humphrey (gametutorials.com), Michal Bubner (mbsoftworks.sk), Christophe Riccio (glm.g-truc.net)
 - Christy Quinn, Sam Kellett and others

 For educational use by Department of Computer Science, City University London UK.

 This template contains a skybox, simple terrain, camera, lighting, shaders, texturing

 Potential ways to modify the code:  Add new geometry types, shaders, change the terrain, load new meshes, change the lighting, 
 different camera controls, different shaders, etc.
 
 Template version 5.0a 29/01/2017
 Dr Greg Slabaugh (gregory.slabaugh.1@city.ac.uk) 

 version 6.0a 29/01/2019
 Dr Eddie Edwards (Philip.Edwards@city.ac.uk)

 version 6.1a 13/02/2022 - Sorted out Release mode and a few small compiler warnings
 Dr Eddie Edwards (Philip.Edwards@city.ac.uk)

*/


#include "game.h"


// Setup includes
#include "HighResolutionTimer.h"
#include "GameWindow.h"

// Game includes
#include "Camera.h"
#include "Player.h"
#include "Skybox.h"
#include "Plane.h"
#include "Shaders.h"
#include "FreeTypeFont.h"
#include "Sphere.h"
#include "MatrixStack.h"
#include "OpenAssetImportMesh.h"
#include "Audio.h"
#include "House.h"
#include "Pentagon.h"
#include "CatmullRom.h"
#include "HeightMapTerrain.h"

// Constructor
Game::Game()
{
	m_pSkybox = NULL;
	m_pCamera = NULL;
	m_pPlayer = NULL;
	m_pShaderPrograms = NULL;
	m_pPlanarTerrain = NULL;
	m_pFtFont = NULL;
	m_pBarrelMesh = NULL;
	m_pHorseMesh = NULL;
	m_pJeepMesh = NULL;
	m_pCarMesh = NULL;
	m_pMilitaryMesh = NULL;
	m_pTreeMesh = NULL;
	m_pSphere = NULL;
	m_pHighResolutionTimer = NULL;
	m_pAudio = NULL;
	m_pHouse = NULL;
	m_pPentagon = NULL;
	m_pCatmullRom = NULL;
	m_pHeightmapTerrain = NULL;

	m_dt = 0.0;
	m_framesPerSecond = 0;
	m_frameCount = 0;
	m_elapsedTime = 0.0f;
	m_currentDistance = 0.0f;
	m_damageTimer = 0.0f;
}

// Destructor
Game::~Game() 
{ 
	//game objects
	delete m_pCamera;
	delete m_pPlayer;
	delete m_pSkybox;
	delete m_pPlanarTerrain;
	delete m_pFtFont;
	delete m_pBarrelMesh;
	delete m_pHorseMesh;
	delete m_pJeepMesh;
	delete m_pCarMesh;
	delete m_pMilitaryMesh;
	delete m_pTreeMesh;
	delete m_pSphere;
	delete m_pAudio;
	delete m_pHouse;
	delete m_pPentagon;
	delete m_pCatmullRom;
	delete m_pHeightmapTerrain;

	if (m_pShaderPrograms != NULL) {
		for (unsigned int i = 0; i < m_pShaderPrograms->size(); i++)
			delete (*m_pShaderPrograms)[i];
	}
	delete m_pShaderPrograms;

	//setup objects
	delete m_pHighResolutionTimer;
}

// Initialisation:  This method only runs once at startup
void Game::Initialise() {

	// Set the clear colour and depth
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClearDepth(1.0f);

	/// Create objects
	m_pCamera = new CCamera;
	m_pPlayer = new CPlayer;
	m_pSkybox = new CSkybox;
	m_pShaderPrograms = new vector <CShaderProgram *>;
	m_pPlanarTerrain = new CPlane;
	m_pFtFont = new CFreeTypeFont;
	m_pBarrelMesh = new COpenAssetImportMesh;
	m_pHorseMesh = new COpenAssetImportMesh;
	m_pJeepMesh = new COpenAssetImportMesh;
	m_pCarMesh = new COpenAssetImportMesh;
	m_pMilitaryMesh = new COpenAssetImportMesh;
	m_pTreeMesh = new COpenAssetImportMesh;
	m_pSphere = new CSphere;
	m_pAudio = new CAudio;
	m_pHouse = new CHouse;
	m_pPentagon = new CPentagon;
	m_pCatmullRom = new CCatmullRom;
	m_pHeightmapTerrain = new CHeightMapTerrain;

	RECT dimensions = m_gameWindow.GetDimensions();

	int width = dimensions.right - dimensions.left;
	int height = dimensions.bottom - dimensions.top;

	// Set the orthographic and perspective projection matrices based on the image size
	m_pCamera->SetOrthographicProjectionMatrix(width, height); 
	m_pCamera->SetPerspectiveProjectionMatrix(45.0f, (float) width / (float) height, 0.5f, 5000.0f);

	// Load shaders
	vector<CShader> shShaders;
	vector<string> sShaderFileNames;
	sShaderFileNames.push_back("mainShader.vert");
	sShaderFileNames.push_back("mainShader.frag");
	sShaderFileNames.push_back("textShader.vert");
	sShaderFileNames.push_back("textShader.frag");
	sShaderFileNames.push_back("houseShader.vert");
	sShaderFileNames.push_back("houseShader.frag");

	for (int i = 0; i < (int) sShaderFileNames.size(); i++) {
		string sExt = sShaderFileNames[i].substr((int) sShaderFileNames[i].size()-4, 4);
		int iShaderType;
		if (sExt == "vert") iShaderType = GL_VERTEX_SHADER;
		else if (sExt == "frag") iShaderType = GL_FRAGMENT_SHADER;
		else if (sExt == "geom") iShaderType = GL_GEOMETRY_SHADER;
		else if (sExt == "tcnl") iShaderType = GL_TESS_CONTROL_SHADER;
		else iShaderType = GL_TESS_EVALUATION_SHADER;
		CShader shader;
		shader.LoadShader("resources\\shaders\\"+sShaderFileNames[i], iShaderType);
		shShaders.push_back(shader);
	}

	// Create the main shader program
	CShaderProgram *pMainProgram = new CShaderProgram;
	pMainProgram->CreateProgram();
	pMainProgram->AddShaderToProgram(&shShaders[0]);
	pMainProgram->AddShaderToProgram(&shShaders[1]);
	pMainProgram->LinkProgram();
	m_pShaderPrograms->push_back(pMainProgram);

	// Create a shader program for fonts
	CShaderProgram *pFontProgram = new CShaderProgram;
	pFontProgram->CreateProgram();
	pFontProgram->AddShaderToProgram(&shShaders[2]);
	pFontProgram->AddShaderToProgram(&shShaders[3]);
	pFontProgram->LinkProgram();
	m_pShaderPrograms->push_back(pFontProgram);

	CShaderProgram* pHouseProgram = new CShaderProgram;
	pHouseProgram->CreateProgram();
	pHouseProgram->AddShaderToProgram(&shShaders[4]);
	pHouseProgram->AddShaderToProgram(&shShaders[5]);
	pHouseProgram->LinkProgram();
	m_pShaderPrograms->push_back(pHouseProgram);

	// You can follow this pattern to load additional shaders

	// Create the skybox
	// Skybox downloaded from http://www.akimbo.in/forum/viewtopic.php?f=10&t=9
	m_pSkybox->Create(2500.0f);
	
	// Create the planar terrain
	m_pPlanarTerrain->Create("resources\\textures\\", "ground.jpg", 2000.0f, 2000.0f, 50.0f); // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013

	m_pFtFont->LoadSystemFont("arial.ttf", 32);
	m_pFtFont->SetShaderProgram(pFontProgram);

	// Load some meshes in OBJ format
	m_pBarrelMesh->Load("resources\\models\\Barrel\\Barrel02.obj");  // Downloaded from http://www.psionicgames.com/?page_id=24 on 24 Jan 2013
	m_pHorseMesh->Load("resources\\models\\Horse\\Horse2.obj");  // Downloaded from http://opengameart.org/content/horse-lowpoly on 24 Jan 2013
	m_pJeepMesh->Load("resources\\models\\Jeep\\Jeep1.obj");	// From Advanced Game Tech module
	m_pCarMesh->Load("resources\\models\\Car\\car.obj");
	m_pMilitaryMesh->Load("resources\\models\\Military\\soviet military off-road vehicle.obj");
	m_pTreeMesh->Load("resources\\models\\Tree\\tree.obj");

	// Create a sphere
	m_pSphere->Create("resources\\textures\\", "dirtpile01.jpg", 25, 25);  // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013
	glEnable(GL_CULL_FACE);

	m_pHouse->Create("resources\\textures\\", "brick.jpg", 15.0f, 22.5f, 1.0f);
	m_pPentagon->Create("resources\\textures\\", "barrel.jpg", 50.f, 20.0f, 1.0f);

	// Initialise audio and play background music
	m_pAudio->Initialise();
	m_pAudio->LoadEventSound("resources\\Audio\\Boing.wav");					// Royalty free sound from freesound.org
	m_pAudio->LoadMusicStream("resources\\Audio\\DST-Garote.mp3");	// Royalty free music from http://www.nosoapradio.us/
	m_pAudio->PlayMusicStream();

	m_pCatmullRom->CreateCentreline();
	m_pCatmullRom->CreateOffsetCurves();
	m_pCatmullRom->CreateTrack();

	m_pPlayer->Set(glm::vec3(-50.0f,0.0f,0.0f));

	m_pHeightmapTerrain->Create("resources\\textures\\terrainHeightMap200.bmp", "resources\\textures\\ground.jpg", glm::vec3(0, -0.55, 0), 1500.0f, 1500.0f, 40.0f);
}

// Render method runs repeatedly in a loop
void Game::Render() {
	
	// Clear the buffers and enable depth testing (z-buffering)
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// Set up a matrix stack
	glutil::MatrixStack modelViewMatrixStack;
	modelViewMatrixStack.SetIdentity();

	// Use the main shader program 
	CShaderProgram *pMainProgram = (*m_pShaderPrograms)[0];
	pMainProgram->UseProgram();
	pMainProgram->SetUniform("bUseTexture", true);
	pMainProgram->SetUniform("sampler0", 0);
	// Note: cubemap and non-cubemap textures should not be mixed in the same texture unit.  Setting unit 10 to be a cubemap texture.
	int cubeMapTextureUnit = 10; 
	pMainProgram->SetUniform("CubeMapTex", cubeMapTextureUnit);
	pMainProgram->SetUniform("sampler1", 2);
	

	// Set the projection matrix
	pMainProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());

	// Call LookAt to create the view matrix and put this on the modelViewMatrix stack. 
	// Store the view matrix and the normal matrix associated with the view matrix for later (they're useful for lighting -- since lighting is done in eye coordinates)
	modelViewMatrixStack.LookAt(m_pCamera->GetPosition(), m_pCamera->GetView(), m_pCamera->GetUpVector());
	glm::mat4 viewMatrix = modelViewMatrixStack.Top();
	glm::mat3 viewNormalMatrix = m_pCamera->ComputeNormalMatrix(viewMatrix);

	
	// Set light and materials in main shader program
	glm::vec4 lightPosition1 = glm::vec4(-100, 100, -100, 1); // Position of light source *in world coordinates*
	glm::vec4 lightPosition2 = glm::vec4(50, 100, 50, 1); // Position of light source *in world coordinates*
	pMainProgram->SetUniform("light1.position", viewMatrix*lightPosition1); // Position of light source *in eye coordinates*
	pMainProgram->SetUniform("light1.La", glm::vec3(1.0f));		// Ambient colour of light
	pMainProgram->SetUniform("light1.Ld", glm::vec3(1.0f));		// Diffuse colour of light
	pMainProgram->SetUniform("light1.Ls", glm::vec3(1.0f));		// Specular colour of light
	//pMainProgram->SetUniform("light2.position", viewMatrix * lightPosition2); // Position of light source *in eye coordinates*
	//pMainProgram->SetUniform("light2.La", glm::vec3(1.0f, 0.0f, 1.0f));
	//pMainProgram->SetUniform("light2.Ld", glm::vec3(1.0f, 0.0f, 1.0f));
	//pMainProgram->SetUniform("light2.Ls", glm::vec3(1.0f, 0.0f, 1.0f));
	//pMainProgram->SetUniform("light2.direction", glm::normalize(viewNormalMatrix * glm::vec3(0, -1, 0)));
	//pMainProgram->SetUniform("light2.exponent", 20.0f);
	//pMainProgram->SetUniform("light2.cutoff", 30.0f);
	pMainProgram->SetUniform("material1.Ma", glm::vec3(1.0f));	// Ambient material reflectance
	pMainProgram->SetUniform("material1.Md", glm::vec3(0.0f));	// Diffuse material reflectance
	pMainProgram->SetUniform("material1.Ms", glm::vec3(0.0f));	// Specular material reflectance
	pMainProgram->SetUniform("material1.shininess", 15.0f);		// Shininess material property
		

	// Render the skybox and terrain with full ambient reflectance 
	modelViewMatrixStack.Push();
		pMainProgram->SetUniform("renderSkybox", true);
		// Translate the modelview matrix to the camera eye point so skybox stays centred around camera
		glm::vec3 vEye = m_pCamera->GetPosition();
		modelViewMatrixStack.Translate(vEye);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pSkybox->Render(cubeMapTextureUnit);
		pMainProgram->SetUniform("renderSkybox", false);
	modelViewMatrixStack.Pop();

	// Render the planar terrain
	//modelViewMatrixStack.Push();
		//pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		//pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		//m_pPlanarTerrain->Render();
	//modelViewMatrixStack.Pop();

	// Render the new terrain
	modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(glm::vec3(0.0f, 0.0f, 0.0f));
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pHeightmapTerrain->Render();
	modelViewMatrixStack.Pop();


	// Turn on diffuse + specular materials
	pMainProgram->SetUniform("material1.Ma", glm::vec3(0.5f));	// Ambient material reflectance
	pMainProgram->SetUniform("material1.Md", glm::vec3(0.5f));	// Diffuse material reflectance
	pMainProgram->SetUniform("material1.Ms", glm::vec3(1.0f));	// Specular material reflectance	

	// Render the Jeep 
	modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(playerPosition);
		modelViewMatrixStack *= playerForward;
		modelViewMatrixStack.Scale(3.5f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		if (health > 0) {
			if (!damage) {
				m_pJeepMesh->Render();
			}
		}
	modelViewMatrixStack.Pop();

	// Render the Car 
	modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(carPosition1);
		modelViewMatrixStack *= carForward1;
		modelViewMatrixStack.Scale(4.5f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pCarMesh->Render();
	modelViewMatrixStack.Pop();

	// Render the Car 
	modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(carPosition2);
		modelViewMatrixStack *= carForward2;
		modelViewMatrixStack.Scale(4.5f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pCarMesh->Render();
	modelViewMatrixStack.Pop();

	// Render the Car 
	modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(carPosition3);
		modelViewMatrixStack *= carForward3;
		modelViewMatrixStack.Scale(4.5f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pCarMesh->Render();
	modelViewMatrixStack.Pop();

	// Render the Military Car 
	modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(militaryPosition1);
		modelViewMatrixStack *= militaryForward1;
		modelViewMatrixStack.Scale(4.5f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pMilitaryMesh->Render();
	modelViewMatrixStack.Pop();

	// Render the Military Car 
	modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(militaryPosition2);
		modelViewMatrixStack *= militaryForward2;
		modelViewMatrixStack.Scale(4.5f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pMilitaryMesh->Render();
	modelViewMatrixStack.Pop();

	// Render the Military Car 
	modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(militaryPosition3);
		modelViewMatrixStack *= militaryForward3;
		modelViewMatrixStack.Scale(4.5f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pMilitaryMesh->Render();
	modelViewMatrixStack.Pop();
	

	// Render Trees
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 12; j++) {
			glm::vec3 treePosition = glm::vec3(20.0f * i + 140, 0.0f, 40.0f - 20.0f * j);
			treePosition.y = m_pHeightmapTerrain->ReturnGroundHeight(treePosition);

			modelViewMatrixStack.Push();
			modelViewMatrixStack.Translate(treePosition);
			modelViewMatrixStack.Scale(15.0f);
			pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
			pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
			m_pTreeMesh->Render();
			modelViewMatrixStack.Pop();
		}
	}

	glm::vec3 containerPosition = glm::vec3(0.0f, 0.0f, 15.0f);
	containerPosition.y = m_pHeightmapTerrain->ReturnGroundHeight(containerPosition);

	modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(containerPosition);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pPentagon->Render();
	modelViewMatrixStack.Pop();


	modelViewMatrixStack.Push();
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		//m_pCatmullRom->RenderCentreline();
		//m_pCatmullRom->RenderOffsetCurves();
		m_pCatmullRom->RenderTrack();
	modelViewMatrixStack.Pop();


	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 2; j++) {
			glm::vec3 housePosition = glm::vec3(5.0f - 25 * i, 0.0f, -170.0f + 20.0f * j);
			housePosition.y = m_pHeightmapTerrain->ReturnGroundHeight(housePosition);

			modelViewMatrixStack.Push();
			modelViewMatrixStack.Translate(housePosition);
			pMainProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
			pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
			pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
			//pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
			//pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
			m_pHouse->Render();
			modelViewMatrixStack.Pop();
		}
	}


	CShaderProgram* pHouseProgram = (*m_pShaderPrograms)[2];
	pHouseProgram->UseProgram();

	pHouseProgram->SetUniform("sampler0", 0);
	pHouseProgram->SetUniform("material1.Ma", glm::vec3(0.0f, 1.0f, 0.0f));
	pHouseProgram->SetUniform("material1.Md", glm::vec3(0.0f, 1.0f, 0.0f));
	pHouseProgram->SetUniform("material1.Ms", glm::vec3(1.0f, 1.0f, 1.0f));
	pHouseProgram->SetUniform("material1.shininess", 50.0f);
	pHouseProgram->SetUniform("light1.La", glm::vec3(0.15f, 0.15f, 0.15f));
	pHouseProgram->SetUniform("light1.Ld", glm::vec3(1.0f, 1.0f, 1.0f));
	pHouseProgram->SetUniform("light1.Ls", glm::vec3(1.0f, 1.0f, 1.0f));
	pHouseProgram->SetUniform("light1.position", viewMatrix* lightPosition1);

	/*
	modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(glm::vec3(-5.0f, 0.0f, -5.0f));
		pHouseProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
		pHouseProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pHouseProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		//pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		//pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pHouse->Render();
	modelViewMatrixStack.Pop();
	*/
		
	// Draw the 2D graphics after the 3D graphics
	DisplayFrameRate();

	// Swap buffers to show the rendered image
	SwapBuffers(m_gameWindow.Hdc());		

}

// Update method runs repeatedly with the Render method
void Game::Update() {
	// Update the camera using the amount of time that has elapsed to avoid framerate dependent motion
	m_pCamera->Update(m_dt);

	//m_pPlayer->Update(m_dt);

	m_currentDistance += 0.1f * (float)m_dt;
	m_damageTimer -= 0.05f * (float)m_dt;
	m_pCamera->lowerChangeViewTimer(m_dt);

	m_car1Distance = m_currentDistance * 0.65f + 650;
	m_car2Distance = m_currentDistance * 0.55f + 1400;
	m_car3Distance = m_currentDistance * 0.55f + 1100;

	m_military1Distance = m_currentDistance * 0.65f + 350;
	m_military2Distance = m_currentDistance * 0.55f + 650;
	m_military3Distance = m_currentDistance * 0.55f + 150;

	glm::vec3 p;
	m_pCatmullRom->Sample(m_currentDistance, p);

	glm::vec3 pNext;
	m_pCatmullRom->Sample(m_currentDistance + 1.0f, pNext);

	glm::vec3 T = glm::normalize(pNext - p);
	glm::vec3 N = glm::normalize(glm::cross(T, glm::vec3(0.0f, 1.0f, 0.0f)));
	glm::vec3 B = glm::normalize(glm::cross(N, T));

	m_pCatmullRom->Sample(m_currentDistance, playerPosition);
	playerForward = glm::inverse(glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), T, B));


	//car1
	glm::vec3 car1p;
	m_pCatmullRom->Sample(m_car1Distance, car1p);

	glm::vec3 car1pNext;
	m_pCatmullRom->Sample(m_car1Distance + 1.0f, car1pNext);

	glm::vec3 car1T = glm::normalize(car1pNext - car1p);
	glm::vec3 car1N = glm::normalize(glm::cross(car1T, glm::vec3(0.0f, 1.0f, 0.0f)));
	glm::vec3 car1B = glm::normalize(glm::cross(car1N, car1T));

	m_pCatmullRom->Sample(m_car1Distance, carPosition1);
	carForward1 = glm::inverse(glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), -car1T, car1B));


	//car2
	glm::vec3 car2p;
	m_pCatmullRom->Sample(m_car2Distance, car2p);

	glm::vec3 car2pNext;
	m_pCatmullRom->Sample(m_car2Distance + 1.0f, car2pNext);

	glm::vec3 car2T = glm::normalize(car2pNext - car2p);
	glm::vec3 car2N = glm::normalize(glm::cross(car2T, glm::vec3(0.0f, 1.0f, 0.0f)));
	glm::vec3 car2B = glm::normalize(glm::cross(car2N, car2T));

	m_pCatmullRom->Sample(m_car2Distance, carPosition2);
	carForward2 = glm::inverse(glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), -car2T, car2B));


	//car3
	glm::vec3 car3p;
	m_pCatmullRom->Sample(m_car3Distance, car3p);

	glm::vec3 car3pNext;
	m_pCatmullRom->Sample(m_car3Distance + 1.0f, car3pNext);

	glm::vec3 car3T = glm::normalize(car3pNext - car3p);
	glm::vec3 car3N = glm::normalize(glm::cross(car3T, glm::vec3(0.0f, 1.0f, 0.0f)));
	glm::vec3 car3B = glm::normalize(glm::cross(car3N, car3T));

	m_pCatmullRom->Sample(m_car3Distance, carPosition3);
	carForward3 = glm::inverse(glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), -car3T, car3B));

	//Military car1
	glm::vec3 mCar1p;
	m_pCatmullRom->Sample(m_military1Distance, mCar1p);

	glm::vec3 mCar1pNext;
	m_pCatmullRom->Sample(m_military1Distance + 1.0f, mCar1pNext);

	glm::vec3 mCar1T = glm::normalize(mCar1pNext - mCar1p);
	glm::vec3 mCar1N = glm::normalize(glm::cross(mCar1T, glm::vec3(0.0f, 1.0f, 0.0f)));
	glm::vec3 mCar1B = glm::normalize(glm::cross(mCar1N, mCar1T));

	m_pCatmullRom->Sample(m_military1Distance, militaryPosition1);
	militaryForward1 = glm::inverse(glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), -mCar1T, mCar1B));


	//Military car2
	glm::vec3 mCar2p;
	m_pCatmullRom->Sample(m_military2Distance, mCar2p);

	glm::vec3 mCar2pNext;
	m_pCatmullRom->Sample(m_military2Distance + 1.0f, mCar2pNext);

	glm::vec3 mCar2T = glm::normalize(mCar2pNext - mCar2p);
	glm::vec3 mCar2N = glm::normalize(glm::cross(mCar2T, glm::vec3(0.0f, 1.0f, 0.0f)));
	glm::vec3 mCar2B = glm::normalize(glm::cross(mCar2N, mCar2T));

	m_pCatmullRom->Sample(m_military2Distance, militaryPosition2);
	militaryForward2 = glm::inverse(glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), -mCar2T, mCar2B));


	//Military car3
	glm::vec3 mCar3p;
	m_pCatmullRom->Sample(m_military3Distance, mCar3p);

	glm::vec3 mCar3pNext;
	m_pCatmullRom->Sample(m_military3Distance + 1.0f, mCar3pNext);

	glm::vec3 mCar3T = glm::normalize(mCar3pNext - mCar3p);
	glm::vec3 mCar3N = glm::normalize(glm::cross(mCar3T, glm::vec3(0.0f, 1.0f, 0.0f)));
	glm::vec3 mCar3B = glm::normalize(glm::cross(mCar3N, mCar3T));

	m_pCatmullRom->Sample(m_military3Distance, militaryPosition3);
	militaryForward3 = glm::inverse(glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), -mCar3T, mCar3B));


	if (car2Switch) {
		car2Movement += 0.015 * (float)m_dt;
		if (car2Movement > 6.5f) {
			car2Switch = false;
		}
	}
	else {
		car2Movement -= 0.015 * (float)m_dt;
		if (car2Movement < -6.5f) {
			car2Switch = true;
		}
	}

	if (car3Switch) {
		car3Movement += 0.015 * (float)m_dt;
		if (car3Movement > 6.5f) {
			car3Switch = false;
		}
	}
	else {
		car3Movement -= 0.015 * (float)m_dt;
		if (car3Movement < -6.5f) {
			car3Switch = true;
		}
	}

	if (military2Switch) {
		military2Movement += 0.0125 * (float)m_dt;
		if (military2Movement > 6.5f) {
			military2Switch = false;
		}
	}
	else {
		military2Movement -= 0.0125 * (float)m_dt;
		if (military2Movement < -6.5f) {
			military2Switch = true;
		}
	}

	if (military3Switch) {
		military3Movement += 0.0125 * (float)m_dt;
		if (military3Movement > 6.5f) {
			military3Switch = false;
		}
	}
	else {
		military3Movement -= 0.0125 * (float)m_dt;
		if (military3Movement < -6.5f) {
			military3Switch = true;
		}
	}


	carPosition2 -= car2Movement * car2N;
	carPosition3 += car3Movement * car3N;

	militaryPosition2 -= military2Movement * mCar2N;
	militaryPosition3 += military3Movement * mCar3N;



	if (m_pCamera->isLeftSide()) {
		playerMovement -= 0.05f * (float)m_dt;
	}
	else if (m_pCamera->isRightSide()) {
		playerMovement += 0.05f * (float)m_dt;
	}

	if (playerMovement > 6.5f) {
		playerMovement = 6.5f;
	}
	if (playerMovement < -6.5f) {
		playerMovement = -6.5f;
	}

	playerPosition += playerMovement * N;

	if (!m_pCamera->isDebug()) {
		if (health > 0) {
			if (!m_pCamera->isFirstPerson()) {
				p += 8.5f * B;
				m_pCamera->Set(p - 20.0f * T, p + 1.0f * T, glm::vec3(0, 1, 0));
			}
			else {
				p += (2.75f * B) + (playerMovement *N);
				m_pCamera->Set(p + 1.95f * T, p + 5.0f * T, glm::vec3(0, 1, 0));
			}
		}
	}
	
	//m_pCamera->Set(p + 10.0f * B, p + 50.0f * T, glm::vec3(0,1,0));
	
	m_pAudio->Update();

	if (m_damageTimer < 0.0f) {
		glm::vec3 d1 = carPosition1 - playerPosition;
		glm::vec3 d2 = carPosition2 - playerPosition;
		glm::vec3 d3 = carPosition3 - playerPosition;
		glm::vec3 d4 = militaryPosition1 - playerPosition;
		glm::vec3 d5 = militaryPosition2 - playerPosition;
		glm::vec3 d6 = militaryPosition3 - playerPosition;

		damage = false;

		if (glm::length(d1) < 5.0f) {
			damage = true;
		}
		if (glm::length(d2) < 5.0f) {
			damage = true;
		}
		if (glm::length(d3) < 5.0f) {
			damage = true;
		}
		if (glm::length(d4) < 5.0f) {
			damage = true;
		}
		if (glm::length(d5) < 5.0f) {
			damage = true;
		}
		if (glm::length(d6) < 5.0f) {
			damage = true;
		}

		if (damage) {
			m_damageTimer = 50.0f;
			health -= 25;
		}
	}
	else {
		
	}


}



void Game::DisplayFrameRate()
{

	CShaderProgram *fontProgram = (*m_pShaderPrograms)[1];

	RECT dimensions = m_gameWindow.GetDimensions();
	int height = dimensions.bottom - dimensions.top;
	int width = dimensions.right - dimensions.left;

	// Increase the elapsed time and frame counter
	m_elapsedTime += m_dt;
	m_frameCount++;
	timeString = std::to_string(minutes), ":", std::to_string(seconds);

	// Now we want to subtract the current time by the last time that was stored
	// to see if the time elapsed has been over a second, which means we found our FPS.
	if (m_elapsedTime >= 1000)
    {
		m_elapsedTime = 0;
		m_framesPerSecond = m_frameCount;

		if (health > 0) {
			seconds += 1;
		}

		if (seconds >= 60) {
			//seconds = 0;
			minutes += 1;
		}


		// Reset the frames per second
		m_frameCount = 0;
    }

	// Use the font shader program and render the text
	fontProgram->UseProgram();
	glDisable(GL_DEPTH_TEST);
	fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
	fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
	fontProgram->SetUniform("vColour", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	healthText = std::to_string(health) + "%";

	if (!m_pCamera->isDebug()) {
		if (health <= 0) {
			fontProgram->SetUniform("vColour", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
			m_pFtFont->Render(width / 2 - 40 * 3, height / 5, 40, "Final Time: %d", seconds);
			m_pFtFont->Render(width / 2 - 70 * 3, height / 2, 70, "GAME OVER");
		}
		else {
			fontProgram->SetUniform("vColour", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
			m_pFtFont->Render(20, height - 30, 30, "Health: %d", health);
			m_pFtFont->Render(20, height - 60, 30, "Time: %d", seconds);
		}
	}
}

// The game loop runs repeatedly until game over
void Game::GameLoop()
{
	/*
	// Fixed timer
	dDt = pHighResolutionTimer->Elapsed();
	if (dDt > 1000.0 / (double) Game::FPS) {
		pHighResolutionTimer->Start();
		Update();
		Render();
	}
	*/
	
	
	// Variable timer
	m_pHighResolutionTimer->Start();
	Update();
	Render();
	m_dt = m_pHighResolutionTimer->Elapsed();
	

}


WPARAM Game::Execute() 
{
	m_pHighResolutionTimer = new CHighResolutionTimer;
	m_gameWindow.Init(m_hInstance);

	if(!m_gameWindow.Hdc()) {
		return 1;
	}

	Initialise();

	m_pHighResolutionTimer->Start();

	
	MSG msg;

	while(1) {													
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { 
			if(msg.message == WM_QUIT) {
				break;
			}

			TranslateMessage(&msg);	
			DispatchMessage(&msg);
		} else if (m_appActive) {
			GameLoop();
		} 
		else Sleep(200); // Do not consume processor power if application isn't active
	}

	m_gameWindow.Deinit();

	return(msg.wParam);
}

LRESULT Game::ProcessEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param) 
{
	LRESULT result = 0;

	switch (message) {


	case WM_ACTIVATE:
	{
		switch(LOWORD(w_param))
		{
			case WA_ACTIVE:
			case WA_CLICKACTIVE:
				m_appActive = true;
				m_pHighResolutionTimer->Start();
				break;
			case WA_INACTIVE:
				m_appActive = false;
				break;
		}
		break;
		}

	case WM_SIZE:
			RECT dimensions;
			GetClientRect(window, &dimensions);
			m_gameWindow.SetDimensions(dimensions);
		break;

	case WM_PAINT:
		PAINTSTRUCT ps;
		BeginPaint(window, &ps);
		EndPaint(window, &ps);
		break;

	case WM_KEYDOWN:
		switch(w_param) {
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		case '1':
			m_pAudio->PlayEventSound();
			break;
		case VK_F1:
			m_pAudio->PlayEventSound();
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		result = DefWindowProc(window, message, w_param, l_param);
		break;
	}

	return result;
}

Game& Game::GetInstance() 
{
	static Game instance;

	return instance;
}

void Game::SetHinstance(HINSTANCE hinstance) 
{
	m_hInstance = hinstance;
}

LRESULT CALLBACK WinProc(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
	return Game::GetInstance().ProcessEvents(window, message, w_param, l_param);
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, PSTR, int) 
{
	Game &game = Game::GetInstance();
	game.SetHinstance(hinstance);

	return int(game.Execute());
}
