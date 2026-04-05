#include "Scene.h"
#include "Log.h"
#include "Engine.h"
#include "NPC.h"
#include "Audio.h"
#include "Window.h"
#include "SceneManager.h"

Scene::Scene(std::string _id, std::string mapPath, std::string mapName)
{
	id = _id;
	name = "scene";
	
	entityManager = new EntityManager();
	missionManager = new MissionManager();
	dialogManager = new DialogManager();

	gameStarted = id != "main menu" && id != "intro";

	if (gameStarted)
	{
		LoadMap(mapPath, mapName);
	}

	//entityManager->CreateEntity("IT-001", EntityType::INTERACTABLE_ITEM);

	//entityManager->CreateEntity("player", EntityType::PLAYER);
	//entityManager->CreateEntity("CH-001", EntityType::NPC);

	//map->Load("base map path", mapName);
	/*entityManager->Load(entities);
	missionManager->Load(missions);
	dialogManager->Load(dialogs);*/

}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	entityManager->Awake();
	missionManager->Awake();
	dialogManager->Awake();

	return ret;
}

// Called before the first frame
bool Scene::Start(std::string spawnId)
{
	paused = false;
	pendingSpawnId = spawnId;

	sw = Engine::GetInstance().window->width;
	sh = Engine::GetInstance().window->height;
	logo = Engine::GetInstance().textures->Load("Assets/Textures/TeamDayo_Logo.png");
	b_logo = { sw/2 - logo->w/2, sh/2 - logo->h/2, 0, 0 };
	hoverFxId = Engine::GetInstance().audio->LoadFx(configParameters.child("audios").attribute("hover").as_string());
	clickFxId = Engine::GetInstance().audio->LoadFx(configParameters.child("audios").attribute("click").as_string());
	//logoFxId = Engine::GetInstance().audio->LoadFx(configParameters.child("audios").attribute("logo").as_string());
	logoFxId = Engine::GetInstance().audio->LoadFx("Assets/Audio/Fx/logo.wav");
	elevatorFxId = Engine::GetInstance().audio->LoadFx("Assets/Audio/Fx/elevator.wav");


	Engine::GetInstance().menuManager->SetObserver(this);

	if (id == "intro")
	{
		Engine::GetInstance().menuManager->HideMenu();
		studioLogo = std::dynamic_pointer_cast<UIImage>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::IMAGE, (int)LOGO, b_logo, this, {  }, hoverFxId, clickFxId, UIParameters::Image(logo, logo, logo, logo)));
		studioLogo->active = true;
		Engine::GetInstance().audio->PlayFx(logoFxId);
	}

	if (id == "main menu")
	{
		Engine::GetInstance().menuManager->ShowMainMenu();
		Engine::GetInstance().audio->PlayMusic("Assets/Audio/Music/RebelRefuge.wav");
	}

	if (gameStarted) {
		LoadScene();
		if (id == "SC-001")
		{
			Engine::GetInstance().audio->PlayMusic("Assets/Audio/Music/RebelRefuge.wav");
		}
		else if (id == "SC-002")
		{
			Engine::GetInstance().audio->PlayMusic("Assets/Audio/Music/shop.wav");
		}
		else if (id == "SC-003")
		{
			Engine::GetInstance().audio->PlayFx(elevatorFxId);
			Engine::GetInstance().audio->PlayMusic("Assets/Audio/Music/dungeon.wav");
		}
	}
	

	entityManager->Start();
	missionManager->Start();
	dialogManager->Start();

	if (id == "SC-001") {
		if (Engine::GetInstance().sceneManager->triggerFirstMonologue == true) {
			StartDialog("player");
			Engine::GetInstance().sceneManager->triggerFirstMonologue = false;
		}
	}


	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	if (id == "intro") {
		if (Engine::GetInstance().input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
			studioLogo->active = false;
			studioLogo->Destroy();
			Engine::GetInstance().audio->StopFx();
			Engine::GetInstance().sceneManager->SetCurrentScene("main menu");
		}
	}

	if (!gameStarted) return true;
	///////////// FOR TESTING (remove) /////////////
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_L) == KEY_DOWN) Engine::GetInstance().menuManager->ShowDeathScreen();
	////////////////////////////////////////////////
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_P) == KEY_DOWN || Engine::GetInstance().input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) TogglePause();
	//if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) StartDialog("gravekeeper");

	map->Update(dt);
	entityManager->Update(dt);
	missionManager->Update(dt);
	dialogManager->Update(dt);

	if (gameStarted && !paused && !isOnDialog) {
		CheckTransitions();
	}
		
	//////////////////// JUST TO SEE ACCESSES, DELETE LATER ////////////////////
	for (AccessData& t : mapData.accesses) {
		SDL_Rect rect = {
			(int)t.position.getX(),
			(int)t.position.getY(),
			(int)t.width,
			(int)t.height
		};
		Engine::GetInstance().render->DrawRectangle(rect, 0, 255, 0, 128, true, true);
	}
	/////////////////////

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate(float dt)
{
	return true;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");
	
	Engine::GetInstance().render->follow = nullptr;

	entityManager->CleanUp();
	dialogManager->CleanUp();

	if (map) {
		map->CleanUp();
		delete map;
		map = nullptr;
	}

	delete entityManager;
	entityManager = nullptr;
	delete missionManager;
	missionManager = nullptr;
	delete dialogManager;
	dialogManager = nullptr;

	return true;
}

void Scene::TogglePause()
{
	if (!gameStarted) return;
	paused = !paused;
	entityManager->paused = paused;
	//Engine::GetInstance().physics->paused = paused;

	if (paused) Engine::GetInstance().menuManager->ShowPauseMenu();
	else Engine::GetInstance().menuManager->HideMenu();
}

void Scene::SaveGame()
{
	
}

void Scene::LoadGame()
{
	
}

void Scene::LoadMap(std::string mapPath, std::string mapName)
{
	map = new Map();
	map->Load(mapPath, mapName);
	mapData = map->gameData;
}

void Scene::LoadScene(std::string spawnId)
{
	std::string baseTexturePath = "Assets/Textures/";

	pugi::xml_document charactersDoc = XMLHandler::LoadFile("Assets/Entities/characters.xml");
	pugi::xml_node characters = charactersDoc.child("characters");
	pugi::xml_document itemsDoc = XMLHandler::LoadFile("Assets/Entities/items.xml");
	pugi::xml_node items = itemsDoc.child("items");

	pugi::xml_node pNode = characters.child("player");
	std::string id = pNode.attribute("id").as_string();
	std::string name = pNode.attribute("name").as_string();
	std::string texture = pNode.attribute("texture").as_string();

	//Loads the spawnpoint
	Vector2D spawnPos(0, 0);
	bool found = false;

	for (const SpawnPoint& sp : mapData.spawnPoints) {
		if (sp.spawnId == pendingSpawnId) {
			spawnPos = sp.position;
			found = true;
			break;
		}
	}

	if (!found) {
		for (const SpawnPoint& sp : mapData.spawnPoints) {
			if (sp.spawnId == "default") {
				spawnPos = sp.position;
				break;
			}
		}
	}

	player = std::dynamic_pointer_cast<Player>(entityManager->CreateCharacter(id, name, baseTexturePath + texture, spawnPos, EntityType::PLAYER, NPCInteractionType::DEFAULT));
	Engine::GetInstance().render->follow = player;

	for (NPCData npc : mapData.npcs) {
		for (pugi::xml_node cNode = characters.child("character"); cNode != NULL; cNode = cNode.next_sibling("character")) {
			if (cNode.attribute("id").as_string() != npc.id) continue;
			std::string name = cNode.attribute("name").as_string();
			std::string texture = cNode.attribute("texture").as_string();
			int type = cNode.attribute("type").as_int();
			int npcInteractionType = cNode.attribute("npcInteractionType").as_int();
			entityManager->CreateCharacter(npc.id, name, baseTexturePath + texture, npc.position, (EntityType)type, (NPCInteractionType)npcInteractionType);
		}
	}

	for (ItemData item : mapData.items) {
		for (pugi::xml_node iNode = items.child("item"); iNode != NULL; iNode = iNode.next_sibling("item")) {
			if (iNode.attribute("id").as_string() != item.id) continue;
			std::string name = iNode.attribute("name").as_string();
			std::string texture = iNode.attribute("texture").as_string();
			int type = iNode.attribute("type").as_int();
			int interactionType = iNode.attribute("interactionType").as_int();
			bool canStack = iNode.attribute("canStack").as_bool();
			std::string itemClass = iNode.attribute("itemClass").as_string("item");
			entityManager->CreateItem(item.id, name, baseTexturePath + texture, item.position, (EntityType)type, (ItemInteractionType)interactionType, (bool)canStack);
		}
	}
}

void Scene::EndScene()
{
	
}

//Checks if the player is at a transition rectagle
void Scene::CheckTransitions()
{
	if (!player) return;
	Vector2D playerPos = player->position;

	for (AccessData& t : mapData.accesses) {
		if (playerPos.getX() >= t.position.getX() && playerPos.getX() <= t.position.getX() + t.width && playerPos.getY() >= t.position.getY() && playerPos.getY() <= t.position.getY() + t.height) {
			Engine::GetInstance().sceneManager->SetCurrentScene(t.targetSceneId, t.targetSpawnId);
			return;
		}
	}
}

void Scene::StartDialog(std::string characterId)
{
	if (!gameStarted) return;
	if (isOnDialog) return;
	if (!dialogManager->SetCurrentDialog(characterId)) return;
	isOnDialog = true;
	activeDialogId = characterId;
	entityManager->paused = true;
}

void Scene::EndDialog()
{
	if (!gameStarted) return;
	isOnDialog = false;
	entityManager->paused = false;

	if (!activeDialogId.empty()) {
		for (const auto& entity : entityManager->entities) {
			if (entity->id == activeDialogId) {
				if (auto npc = std::dynamic_pointer_cast<NPC>(entity)) {
					npc->OnDialogEnd();
					break;
				}
			}
		}
		activeDialogId = "";
	}
}

bool Scene::OnUIMouseClickEvent(UIElement* uiElement) {
	
	if (Engine::GetInstance().menuManager->uiLockFrame == Engine::GetInstance().frameCount) return true;

	float musicVol;
	float fxVol;
	switch ((UIID)uiElement->id)
	{
	case START_GAME:
		Engine::GetInstance().menuManager->HideMenu();
		Engine::GetInstance().sceneManager->SetCurrentScene("SC-001");
		break;
	case CONTINUE_GAME:
		Engine::GetInstance().menuManager->HideMenu();
		Engine::GetInstance().sceneManager->SetCurrentScene("SC-001"); // take the last scene from the save data
		break;
	case RESUME_GAME:
		TogglePause();
		break;
	case SETTINGS_BUTTON:
		Engine::GetInstance().menuManager->ShowSettingsMenu();
		break;
	case CREDITS_BUTTON:
		Engine::GetInstance().menuManager->ShowCreditsMenu();
		break;
	case MUSIC_VOLUME:
		musicVol = ((UISlider*)uiElement)->GetValue() / 10;
		Engine::GetInstance().audio->SetMusicVolume(musicVol);
		break;
	case FX_VOLUME:
		fxVol = ((UISlider*)uiElement)->GetValue() / 10;
		Engine::GetInstance().audio->SetFxVolume(fxVol);
		break;
	case FULLSCREEN:
		Engine::GetInstance().window->SetFullscreen(((UICheckbox*)uiElement)->checked);
		Engine::GetInstance().menuManager->Load(true);
		break;
	case BACK_MENU:
		Engine::GetInstance().menuManager->ShowPreviousMenu();
		break;
	case BACK_MAIN_MENU:
		Engine::GetInstance().sceneManager->SetCurrentScene("main menu");
		break;
	case EXIT:
		exit(0);
		break;
	default:
		break;
	}
	return true;
}