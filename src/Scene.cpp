#include "Scene.h"
#include "Log.h"
#include "Engine.h"
#include "NPC.h"
#include "Audio.h"
#include "Window.h"
#include "SceneManager.h"
#include "Vector2D.h"
#include "Weapon.h"
#include "Gear.h"
#include "Consumable.h"
#include <memory>
#include <vector>
#include <unordered_set>
#include <fstream>
#include <algorithm>

Scene::Scene(std::string _id, std::string _mapsPath, std::string _mapName, std::string _combatMapName)
{
	id = _id;
	name = "scene";
	
	entityManager = new EntityManager();
	missionManager = new MissionManager();
	dialogManager = new DialogManager();

	mapsPath = _mapsPath;
	mapName = _mapName;
	combatMapName = _combatMapName;

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

	gameStarted = id != "main menu" && id != "intro";

	if (gameStarted)
	{
		LoadMap(mapsPath, mapName);
		//LoadScene();
	}

	paused = false;
	pendingSpawnId = spawnId;

	Engine::GetInstance().render->SetCursorTexture("Assets/Textures/cursor.png");
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
		Engine::GetInstance().audio->PlayMusic("Assets/Audio/Music/RebelRefuge.wav", 5000.0f);
	}

	if (gameStarted) {
		LoadScene();
		if (id == "SC-001")
		{
			Engine::GetInstance().audio->PlayMusic("Assets/Audio/Music/RebelRefuge.wav", 5000.0f);
		}
		else if (id == "SC-002")
		{
			Engine::GetInstance().audio->PlayMusic("Assets/Audio/Music/shop.wav", 5000.0f);
		}
		else if (id == "SC-003")
		{
			Engine::GetInstance().audio->PlayFx(elevatorFxId);
			Engine::GetInstance().audio->PlayMusic("Assets/Audio/Music/dungeon.wav", 5000.0f);
		}
	}
	

	entityManager->Start();
	missionManager->Start();
	dialogManager->Start();

	if (gameStarted) {
		LoadDialogState();
	}

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

	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_P) == KEY_DOWN || Engine::GetInstance().input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) TogglePause();
	
	if (combat) {
		combat->Update(dt);
		if (combat->combatResult != CombatResult::NO_RESULT) EndCombat(combat->enemyParty, combat->combatResult);
		return true;
	}

	///////////// FOR TESTING (remove) /////////////
	//if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) StartDialog("player");
	//if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_K) == KEY_DOWN) for (const auto& entity : entityManager->entities) if (entity->id == "CH-002" || entity->id == "CH-003") player->AddPartyMember(std::dynamic_pointer_cast<NPC>(entity));
	////////////////////////////////////////////////

	CheckTimers();
	if (!isOnDialog && !paused) ToggleInventory();
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
	
	if (paused) {
		Engine::GetInstance().audio->pauseMultiplier = 0.3f;
		Engine::GetInstance().audio->UpdateMusicVolume();
		Engine::GetInstance().menuManager->ShowPauseMenu();
		showingInventory = false;
		showingShop = false;
	} 
	else
	{
		Engine::GetInstance().audio->pauseMultiplier = 1.0f;
		Engine::GetInstance().audio->UpdateMusicVolume();
		Engine::GetInstance().menuManager->HideMenu();
	}
}

void Scene::SaveGame()
{
	std::ifstream src("Assets/Entities/characters_session.xml", std::ios::binary);
	std::ofstream dst("Assets/Entities/characters.xml", std::ios::binary | std::ios::trunc);
	dst << src.rdbuf();
	src.close();
	dst.close();

	pugi::xml_document charDoc = XMLHandler::LoadFile("Assets/Entities/characters.xml");
	pugi::xml_node characters = charDoc.child("characters");
	pugi::xml_node playerNode = characters.child("player");

	characters.attribute("savedGame").set_value(true);

	//Save player party
	playerNode.remove_child("party");
	pugi::xml_node partyNode = playerNode.append_child("party");
	if (player && player->party) {
		for (const auto& member : player->party->members) {
			pugi::xml_node mNode = partyNode.append_child("member");
			mNode.append_attribute("id").set_value(member->id.c_str());
		}
	}

	//Save player positions
	playerNode.attribute("savedX").set_value(player->position.getX());
	playerNode.attribute("savedY").set_value(player->position.getY());
	SaveCharacterStats(playerNode, player);

	//Save NPC positions and dead state
	for (const auto& entity : entityManager->entities) {
		auto npc = std::dynamic_pointer_cast<NPC>(entity);
		if (!npc) continue;
		for (pugi::xml_node cNode = characters.child("character"); cNode != NULL; cNode = cNode.next_sibling("character")) {
			if (std::string(cNode.attribute("id").as_string()) != npc->id) continue;
			cNode.attribute("savedX").set_value(npc->position.getX());
			cNode.attribute("savedY").set_value(npc->position.getY());
			SaveCharacterStats(cNode, npc);
			break;
		}
	}

	for (const std::string& deadId : Engine::GetInstance().sceneManager->deadNPCs) {
		for (pugi::xml_node cNode = characters.child("character"); cNode; cNode = cNode.next_sibling("character")) {
			if (std::string(cNode.attribute("id").as_string()) != deadId) continue;
			cNode.attribute("isDead").set_value(true);
			break;
		}
	}

	charDoc.save_file("Assets/Entities/characters.xml");
	SaveDialogState();
	LOG("GAME SAVED");
}

void Scene::LoadGame()
{
	
}

void Scene::SaveSessionState()
{
	std::ifstream src("Assets/Entities/characters.xml", std::ios::binary);
	std::ofstream dst("Assets/Entities/characters_session.xml", std::ios::binary | std::ios::trunc);
	dst << src.rdbuf();
	src.close();
	dst.close();

	pugi::xml_document doc = XMLHandler::LoadFile("Assets/Entities/characters_session.xml");
	pugi::xml_node characters = doc.child("characters");
	pugi::xml_node playerNode = characters.child("player");

	if (player) {
		playerNode.attribute("savedX").set_value(player->position.getX());
		playerNode.attribute("savedY").set_value(player->position.getY());
		SaveCharacterStats(playerNode, player);
	}

	for (const auto& entity : entityManager->entities) {
		auto npc = std::dynamic_pointer_cast<NPC>(entity);
		if (!npc) continue;
		for (pugi::xml_node cNode = characters.child("character"); cNode != NULL; cNode = cNode.next_sibling("character")) {
			if (std::string(cNode.attribute("id").as_string()) != npc->id) continue;
			cNode.attribute("savedX").set_value(npc->position.getX());
			cNode.attribute("savedY").set_value(npc->position.getY());
			SaveCharacterStats(cNode, npc);
			break;
		}
	}

	for (const std::string& deadId : Engine::GetInstance().sceneManager->deadNPCs) {
		for (pugi::xml_node cNode = characters.child("character"); cNode != NULL; cNode = cNode.next_sibling("character")) {
			if (std::string(cNode.attribute("id").as_string()) != deadId) continue;
			cNode.attribute("isDead").set_value(true);
			break;
		}
	}

	playerNode.remove_child("party");
	pugi::xml_node partyNode = playerNode.append_child("party");
	if (player && player->party) {
		for (const auto& member : player->party->members) {
			pugi::xml_node mNode = partyNode.append_child("member");
			mNode.append_attribute("id").set_value(member->id.c_str());
		}
	}

	doc.save_file("Assets/Entities/characters_session.xml");

	pugi::xml_document dialogDoc = XMLHandler::LoadFile("Assets/Dialogues/dialogues.xml");
	pugi::xml_node root = dialogDoc.child("dialogs");
	for (pugi::xml_node treeNode = root.child("tree"); treeNode; treeNode = treeNode.next_sibling("tree")) {
		std::string treeId = treeNode.attribute("id").as_string();
		for (DialogTree* tree : dialogManager->dialogs) {
			if (tree->id == treeId) {
				treeNode.attribute("done").set_value(tree->done);
				break;
			}
		}
	}
	dialogDoc.save_file("Assets/Dialogues/dialogues_session.xml");
}

void Scene::SaveDialogState()
{
	pugi::xml_document dialogDoc = XMLHandler::LoadFile("Assets/Dialogues/dialogues.xml");
	pugi::xml_node root = dialogDoc.child("dialogs");

	for (pugi::xml_node treeNode = root.child("tree"); treeNode != NULL; treeNode = treeNode.next_sibling("tree")) {
		std::string treeId = treeNode.attribute("id").as_string();
		for (DialogTree* tree : dialogManager->dialogs) {
			if (tree->id == treeId) {
				treeNode.attribute("done").set_value(tree->done);
				break;
			}
		}
	}
	dialogDoc.save_file("Assets/Dialogues/dialogues.xml");
}

void Scene::LoadDialogState()
{
	pugi::xml_document dialogDoc = XMLHandler::LoadFile("Assets/Dialogues/dialogues_session.xml");
	//pugi::xml_document dialogDoc = XMLHandler::LoadFile("Assets/Dialogues/dialogues.xml");
	pugi::xml_node root = dialogDoc.child("dialogs");

	for (pugi::xml_node treeNode = root.child("tree"); treeNode != NULL; treeNode = treeNode.next_sibling("tree")) {
		std::string treeId = treeNode.attribute("id").as_string();
		bool done = treeNode.attribute("done").as_bool();
		for (DialogTree* tree : dialogManager->dialogs) {
			if (tree->id == treeId) {
				tree->done = done;
				break;
			}
		}
	}
}

void Scene::SaveCharacterStats(pugi::xml_node charNode, std::shared_ptr<Character> character)
{
	pugi::xml_node statsNode = charNode.child("stats");

	for (const Stat& stat : character->stats->stats) {
		pugi::xml_node sNode = statsNode.child("stat");
		for (pugi::xml_node n = sNode; n != NULL; n = n.next_sibling("stat")) {
			if (std::string(n.attribute("name").as_string()) == stat.name) {
				n.attribute("value").set_value(stat.value);
			}
		}
	}
}

void Scene::LoadMap(std::string mapPath, std::string mapName)
{
	map = new Map();
	map->Load(mapPath, mapName);
	mapData = map->gameData;
}

void Scene::LoadScene(std::string spawnId)
{
	pugi::xml_document charactersDoc = XMLHandler::LoadFile("Assets/Entities/characters_session.xml");
	//pugi::xml_document charactersDoc = XMLHandler::LoadFile("Assets/Entities/characters.xml");
	pugi::xml_node characters = charactersDoc.child("characters");
	pugi::xml_document itemsDoc = XMLHandler::LoadFile("Assets/Entities/items.xml");
	pugi::xml_node items = itemsDoc.child("items");
	pugi::xml_document statsDoc = XMLHandler::LoadFile("Assets/Entities/base_stats.xml");
	pugi::xml_node stats = statsDoc.child("stats");

	/*Stats baseStats = Stats();
	for (pugi::xml_node sNode = stats.child("stat"); sNode != NULL; sNode = sNode.next_sibling("stat")) {
		std::string name = sNode.attribute("name").as_string();
		int value = sNode.attribute("value").as_float();
		int max = sNode.attribute("max").as_float();
		baseStats.AddStat(name, value, max);
	}*/

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

	std::vector<NPCData> partyMembers = std::vector<NPCData>();
	for (pugi::xml_node mNode = pNode.child("party").child("member"); mNode != NULL; mNode = mNode.next_sibling("member"))
	{
		NPCData member = NPCData();
		member.id = mNode.attribute("id").as_string();
		member.position = spawnPos;
		partyMembers.push_back(member);
	}

	player = std::dynamic_pointer_cast<Player>(entityManager->CreateCharacter(id, name, baseTexturePath + texture, spawnPos, EntityType::PLAYER, NPCInteractionType::DEFAULT));
	std::string animations = pNode.attribute("animations").as_string();
	player->animationsPath = animations.empty() ? "" : baseTexturePath + animations;
	player->LoadAnimations();

	Engine::GetInstance().render->follow = player;
	player->stats = LoadStats(pNode);
	player->inventory = LoadInventory(pNode);

	//Uncomment when I find a fix
	/*float savedX = pNode.attribute("savedX").as_float();
	float savedY = pNode.attribute("savedY").as_float();
	if (savedX >= 0 && savedY >= 0) {
		b2Body_SetTransform(player->pbody->body, { PIXEL_TO_METERS(savedX), PIXEL_TO_METERS(savedY) }, b2Rot_identity);
		player->position = Vector2D(savedX, savedY);
		LOG("Player position: %d, %d", player->position.getX(), player->position.getY());
	}*/

	std::unordered_set<std::string> ids;

	for (const auto& npc : partyMembers) {
		ids.insert(npc.id);
	}

	mapData.npcs.erase(
		std::remove_if(
			mapData.npcs.begin(),
			mapData.npcs.end(),
			[&](const NPCData& npc) {
				return ids.find(npc.id) != ids.end();
			}
		),
		mapData.npcs.end()
	);

	for (NPCData member : partyMembers) {
		for (pugi::xml_node cNode = characters.child("character"); cNode != NULL; cNode = cNode.next_sibling("character")) {
			if (cNode.attribute("id").as_string() != member.id) continue;
			if (cNode.attribute("isDead").as_bool(false)) break;
			std::string name = cNode.attribute("name").as_string();
			std::string texture = cNode.attribute("texture").as_string();
			int type = cNode.attribute("type").as_int();
			int npcInteractionType = cNode.attribute("npcInteractionType").as_int();
			std::shared_ptr<NPC> m = std::static_pointer_cast<NPC>(entityManager->CreateCharacter(member.id, name, baseTexturePath + texture, member.position, (EntityType)type, (NPCInteractionType)npcInteractionType));
			m->stats = LoadStats(cNode);
			m->inventory = LoadInventory(cNode);
			std::string animations = cNode.attribute("animations").as_string();
			m->animationsPath = animations.empty() ? "" : baseTexturePath + animations;
			m->LoadAnimations();
			m->CreateColliders();
			player->AddPartyMember(m);
		}
	}

	for (NPCData npc : mapData.npcs) {
		for (pugi::xml_node cNode = characters.child("character"); cNode != NULL; cNode = cNode.next_sibling("character")) {
			if (cNode.attribute("id").as_string() != npc.id) continue;
			if (cNode.attribute("isDead").as_bool(false)) break;
			std::string name = cNode.attribute("name").as_string();
			std::string texture = cNode.attribute("texture").as_string();
			int type = cNode.attribute("type").as_int();
			int npcInteractionType = cNode.attribute("npcInteractionType").as_int();

			float savedX = cNode.attribute("savedX").as_float();
			float savedY = cNode.attribute("savedY").as_float();
			Vector2D spawnPos = (savedX >= 0 && savedY >= 0) ? Vector2D(savedX, savedY) : npc.position;
			//entityManager->CreateCharacter(npc.id, name, baseTexturePath + texture, spawnPos, (EntityType)type, (NPCInteractionType)npcInteractionType);
			//entityManager->CreateCharacter(npc.id, name, baseTexturePath + texture, npc.position, (EntityType)type, (NPCInteractionType)npcInteractionType);
			LOG("NPC POSTITION: %f, %f", npc.position.getX(), npc.position.getY());

			std::shared_ptr<Character> m = std::static_pointer_cast<Character>(entityManager->CreateCharacter(npc.id, name, baseTexturePath + texture, spawnPos, (EntityType)type, (NPCInteractionType)npcInteractionType));

			m->stats = LoadStats(cNode);
			m->inventory = LoadInventory(cNode);
			if (auto npcPtr = std::dynamic_pointer_cast<NPC>(m)) {
				std::string animations = cNode.attribute("animations").as_string();
				npcPtr->animationsPath = animations.empty() ? "" : baseTexturePath + animations;
				npcPtr->LoadAnimations();
				npcPtr->CreateColliders();
			}
		}
	}

	for (ItemData item : mapData.items) {
		for (pugi::xml_node iNode = items.child("item"); iNode != NULL; iNode = iNode.next_sibling("item")) {
			if (iNode.attribute("id").as_string() != item.id) continue;
			std::string name = iNode.attribute("name").as_string();
			std::string description = iNode.attribute("description").as_string();
			std::string texture = iNode.attribute("texture").as_string();
			int type = iNode.attribute("type").as_int();
			int interactionType = iNode.attribute("interactionType").as_int();
			bool canStack = iNode.attribute("canStack").as_bool();
			std::string itemClass = iNode.attribute("itemClass").as_string("item");
			std::string toggledTexturePath = iNode.attribute("toggledTexturePath").as_string();
			std::shared_ptr<InteractableItem> newItem = std::dynamic_pointer_cast<InteractableItem>(entityManager->CreateItem(item.id, name, description, baseTexturePath + texture, item.position, itemClass, (EntityType)type, (ItemInteractionType)interactionType, (bool)canStack, baseTexturePath + toggledTexturePath));
			LoadItemDefinition(newItem);
		}
	}
}

Stats* Scene::LoadStats(pugi::xml_node node)
{
	Stats* stats = new Stats();
	for (pugi::xml_node sNode = node.child("stats").child("stat"); sNode != NULL; sNode = sNode.next_sibling("stat")) {
		std::string name = sNode.attribute("name").as_string();
		float value = sNode.attribute("value").as_float();
		float max = sNode.attribute("max").as_float();
		int turns = sNode.attribute("turns").as_int(-1);
		stats->AddStat(name, value, max, turns);
	}
	return stats;
}

Inventory* Scene::LoadInventory(pugi::xml_node characterNode)
{
	Inventory* inventory = new Inventory();
	pugi::xml_node inventoryNode = characterNode.child("inventory");
	if (!inventoryNode) return inventory;
	inventory->gold = inventoryNode.attribute("gold").as_int();
	for (pugi::xml_node iNode = inventoryNode.child("item"); iNode != NULL; iNode = iNode.next_sibling("item")) {
		std::string id = iNode.attribute("id").as_string();
		std::string name = iNode.attribute("name").as_string();
		std::string description = iNode.attribute("description").as_string();
		std::string texture = iNode.attribute("texture").as_string();
		int type = iNode.attribute("type").as_int();
		int interactionType = iNode.attribute("interactionType").as_int();
		bool canStack = iNode.attribute("canStack").as_bool();
		std::string itemClass = iNode.attribute("itemClass").as_string("item");
		std::string toggledTexturePath = iNode.attribute("toggledTexturePath").as_string();
		int slot = iNode.attribute("slot").as_int();
		std::shared_ptr<InteractableItem> item = std::dynamic_pointer_cast<InteractableItem>(entityManager->CreateItem(id, name, description, baseTexturePath + texture, { -999999, -999999 }, itemClass, (EntityType)type, (ItemInteractionType)interactionType, (bool)canStack, baseTexturePath + toggledTexturePath, (GearSlot)slot));
		LoadItemDefinition(item);
		inventory->AddItem(item);
	}
	std::string equippedWeapon = inventoryNode.attribute("equippedWeapon").as_string();
	std::string equippedHelmet = inventoryNode.attribute("equippedHelmet").as_string();
	std::string equippedBody = inventoryNode.attribute("equippedBody").as_string();
	std::string equippedBoots = inventoryNode.attribute("equippedBoots").as_string();
	if (equippedWeapon != "") inventory->EquipWeapon(equippedWeapon);
	if (equippedHelmet != "") inventory->EquipGear(equippedHelmet);
	if (equippedBody != "") inventory->EquipGear(equippedBody);
	if (equippedBoots != "") inventory->EquipGear(equippedBoots);

	return inventory;
}

void Scene::LoadItemDefinition(std::shared_ptr<InteractableItem> item)
{
	pugi::xml_document doc = XMLHandler::LoadFile("Assets/Entities/items_def.xml");
	for (pugi::xml_node iNode = doc.child("items").child("item"); iNode != NULL; iNode = iNode.next_sibling("item")) {
		if (iNode.attribute("itemName").as_string() == item->name)
		{
			item->price = iNode.attribute("gold").as_int();
			item->stats = LoadStats(iNode);
			return;
		}
	}
}

void Scene::EndScene()
{
	
}

void Scene::EndGame()
{
	gameStarted = false;
	entityManager->DestroyEntity(player);
	Engine::GetInstance().menuManager->ShowDeathScreen();
}

void Scene::CheckTimers() {
	if (combatTimer.ReadSec() > combatCooldownSeconds) hasCombatCooldown = false;
}

void Scene::ToggleInventory()
{
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN)
	{
		if (showingShop) {
			ToggleShop(nullptr);
			return;
		}
		showingInventory = !showingInventory;
		entityManager->paused = showingInventory;
		if (showingInventory) Engine::GetInstance().menuManager->ShowInventory(player->inventory);
		else Engine::GetInstance().menuManager->HideMenu();
		if (!showingInventory) UpdateInventory();
	}
}

void Scene::ToggleInventoryForCombat()
{
	showingInventoryForCombat = !showingInventoryForCombat;
	if (showingInventoryForCombat) Engine::GetInstance().menuManager->ShowCombatInventory(player->inventory);
	else Engine::GetInstance().menuManager->HideMenu();
}

void Scene::ToggleShop(NPC* shopOwner)
{
	showingInventory = shopOwner;
	entityManager->paused = showingInventory;
	if (showingInventory)
	{
		Engine::GetInstance().menuManager->ShowShop(player->inventory, shopOwner->inventory);
		showingShop = true;
	}
	else
	{
		Engine::GetInstance().menuManager->HideMenu();
		UpdateInventory();
		UpdateInventory(this->shopOwner);
		showingShop = false;
	}
	this->shopOwner = shopOwner;
}

static std::string ExtractFilename(const std::string& full_path) {
	size_t pos = full_path.find_last_of("/\\");
	if (pos == std::string::npos) return full_path;
	return full_path.substr(pos + 1);
}

void Scene::UpdateInventory(NPC* shopOwner) const
{
	pugi::xml_document doc = XMLHandler::LoadFile("Assets/Entities/characters.xml");
	pugi::xml_node cNode;
	std::vector< std::shared_ptr<InteractableItem>> items;
	Inventory* inventory;
	int gold = 0;
	if (shopOwner) {
		for (pugi::xml_node c = doc.child("characters").child("character"); c != NULL; c = c.next_sibling("character")) {
			if (c.attribute("id").as_string() == shopOwner->id)
			{
				cNode = c;
				break;
			}
		}
		
		items = shopOwner->inventory->items;
		inventory = shopOwner->inventory;
	}
	else {
		cNode = doc.child("characters").child("player");
		items = player->inventory->items;
		gold = player->inventory->gold;
		inventory = player->inventory;
	}
	cNode.remove_child("inventory");
	pugi::xml_node invNode = cNode.append_child("inventory");
	invNode.append_attribute("gold").set_value(gold);
	for (std::shared_ptr<InteractableItem> item : items) {
		if (!item) continue;
		int count = item->canStack ? item->count : 1;
		for (int i = 0; i < count; i++) {
			pugi::xml_node iNode = invNode.append_child("item");
			iNode.append_attribute("id").set_value(item->id.c_str());
			iNode.append_attribute("name").set_value(item->name.c_str());
			iNode.append_attribute("description").set_value(item->description.c_str());
			iNode.append_attribute("texture").set_value(ExtractFilename(item->texturePath).c_str());
			iNode.append_attribute("type").set_value((int)item->type);
			const char* itemClass = "item";
			int slot = -1;
			if (std::dynamic_pointer_cast<Weapon>(item)) itemClass = "weapon";
			else if (auto g = std::dynamic_pointer_cast<Gear>(item))
			{
				itemClass = "gear";
				slot = (int)g->gearSlot;
			}
			else if (std::dynamic_pointer_cast<Consumable>(item)) itemClass = "consumable";
			iNode.append_attribute("itemClass").set_value(itemClass);
			iNode.append_attribute("interactionType").set_value((int)item->itemInteractionType);
			iNode.append_attribute("canStack").set_value(item->canStack);
			if (slot != -1) iNode.append_attribute("slot").set_value(slot);
		}
	}
	if (inventory->equippedWeapon) invNode.append_attribute("equippedWeapon").set_value(inventory->equippedWeapon->name.c_str());
	if (inventory->equippedHelmet) invNode.append_attribute("equippedHelmet").set_value(inventory->equippedHelmet->name.c_str());
	if (inventory->equippedBody) invNode.append_attribute("equippedBody").set_value(inventory->equippedBody->name.c_str());
	if (inventory->equippedBoots) invNode.append_attribute("equippedBoots").set_value(inventory->equippedBoots->name.c_str());

	doc.save_file("Assets/Entities/characters.xml");
}

//Checks if the player is at a transition rectagle
void Scene::CheckTransitions()
{
	if (!player) return;
	Vector2D playerPos = player->position;

	for (AccessData& t : mapData.accesses) {
		if (playerPos.getX() >= t.position.getX() && playerPos.getX() <= t.position.getX() + t.width && playerPos.getY() >= t.position.getY() && playerPos.getY() <= t.position.getY() + t.height) {
			SaveSessionState();
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
		if (activeDialogId == "statue") {
			SaveGame();
		}
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

std::vector<std::shared_ptr<Enemy>> Scene::GetNearEnemies(Vector2D position, float rangePX, std::string enemyID)
{
	std::vector<std::shared_ptr<Enemy>> nearEnemies = std::vector<std::shared_ptr<Enemy>>();
	std::vector<std::shared_ptr<Enemy>> enemies = entityManager->GetEntities<Enemy>();
	
	std::sort(enemies.begin(), enemies.end(),
		[position](std::shared_ptr<Enemy> a, std::shared_ptr<Enemy> b)
		{
			return a->position.distanceEuclidean(position) < b->position.distanceEuclidean(position);
		});

	for (const auto& e : enemies) {
		if (e->id == enemyID) continue;
		if (e->position.distanceEuclidean(position) > rangePX) continue;
		nearEnemies.push_back(e);
		if (nearEnemies.size() == 3) break;
	}
	return nearEnemies;
}

void Scene::StartCombat(std::shared_ptr<Enemy> enemy)
{
	if (hasCombatCooldown || isOnDialog || showingInventory || paused) return;
	Engine::GetInstance().render->follow = nullptr;

	auto nearEnemies = GetNearEnemies(player->position, 300, enemy->id);
	for (const auto& e : nearEnemies) enemy->party->AddMember(e);
	
	combat = new Combat(player->party, enemy->party, mapsPath, combatMapName);
	combat->Awake();
	combat->Start();
}

void Scene::EndCombat(EnemyParty* enemyParty, CombatResult combatResult)
{
	std::shared_ptr<Enemy> leader;
	switch (combatResult)
	{
	case WIN:
		for (const auto& enemy : enemyParty->members) entityManager->DestroyEntity(enemy);
		for (const auto& npc : player->party->members) {
			if (npc->isDead) {
				Engine::GetInstance().sceneManager->deadNPCs.push_back(npc->id);
				player->party->RemoveMember(npc->id);
				entityManager->DestroyEntity(npc);
			}
		}
			
		break;
	case LOSE:
		EndGame();
		break;
	case FLED:
		for (const auto& npc : player->party->members) {
			if (npc->isDead) {
				Engine::GetInstance().sceneManager->deadNPCs.push_back(npc->id);
				player->party->RemoveMember(npc->id);
				entityManager->DestroyEntity(npc);
			}
		}
		combatTimer.Start();
		hasCombatCooldown = true;
		leader = enemyParty->leader;
		enemyParty->members.clear();
		enemyParty->AddMember(leader);
		break;
	default:
		break;
	}
	delete combat;
	combat = nullptr;
	UpdateInventory();
	Engine::GetInstance().render->follow = player;
}

void Scene::CopyCleanGameData()
{
	{
		std::ifstream src("Assets/Entities/characters_clean.xml", std::ios::binary);
		std::ofstream dst1("Assets/Entities/characters.xml", std::ios::binary | std::ios::trunc);
		dst1 << src.rdbuf();
		src.close();
		dst1.close();
	}
	{
		std::ifstream src("Assets/Entities/characters_clean.xml", std::ios::binary);
		std::ofstream dst2("Assets/Entities/characters_session.xml", std::ios::binary | std::ios::trunc);
		dst2 << src.rdbuf();
		src.close();
		dst2.close();
	}

	{
		std::ifstream src("Assets/Entities/items_clean.xml", std::ios::binary);
		std::ofstream dst("Assets/Entities/items.xml", std::ios::binary | std::ios::trunc);
		dst << src.rdbuf();
		src.close();
		dst.close();
	}

	{
		std::ifstream src("Assets/Dialogues/dialogues_clean.xml", std::ios::binary);
		std::ofstream dst1("Assets/Dialogues/dialogues.xml", std::ios::binary | std::ios::trunc);
		dst1 << src.rdbuf();
		src.close();
		dst1.close();
	}
	{
		std::ifstream src("Assets/Dialogues/dialogues_clean.xml", std::ios::binary);
		std::ofstream dst2("Assets/Dialogues/dialogues_session.xml", std::ios::binary | std::ios::trunc);
		dst2 << src.rdbuf();
		src.close();
		dst2.close();
	}

	Engine::GetInstance().sceneManager->deadNPCs.clear();
}

Vector2D Scene::GetPlayerPosition() {

	if (player)
		return player->GetPosition();

	return Vector2D(0, 0);
}

static std::shared_ptr<InteractableItem> CopyItem(std::shared_ptr<InteractableItem> item) {
	if (std::shared_ptr<Weapon> w = std::dynamic_pointer_cast<Weapon>(item)) return std::make_shared<Weapon>(*w);
	else if (std::shared_ptr<Gear> g = std::dynamic_pointer_cast<Gear>(item)) return std::make_shared<Gear>(*g);
	else if (std::shared_ptr<Consumable> c = std::dynamic_pointer_cast<Consumable>(item)) return std::make_shared<Consumable>(*c);
}

bool Scene::OnUIMouseClickEvent(UIElement* uiElement) {
	
	if (Engine::GetInstance().uiManager->uiLockFrame == Engine::GetInstance().frameCount) return true;

	float musicVol;
	float fxVol;
	int amount;
	std::shared_ptr<InteractableItem> copy;
	std::shared_ptr<Gear> g;
	std::shared_ptr<Weapon> w;
	std::shared_ptr<Consumable> c;
	if ((Engine::GetInstance().menuManager->currentMenu == SHOP || 
		 Engine::GetInstance().menuManager->currentMenu == INVENTORY ||
		 Engine::GetInstance().menuManager->currentMenu == COMBAT_INVENTORY) &&
		uiElement->id >= Engine::GetInstance().menuManager->baseSlotsId) 
	{
		UISlot* slot = (UISlot*)uiElement;
		selectedItem = slot->item;
		if (showingInventoryForCombat && combat) {
			if (c = std::dynamic_pointer_cast<Consumable>(selectedItem)) combat->SelectConsumable(c->name);
			return true;
		}
		selectedItemIsFromShop = false;
		if (uiElement->id >= Engine::GetInstance().menuManager->baseShopSlotsId) selectedItemIsFromShop = true;
		Engine::GetInstance().menuManager->selectedItem->SetItem(selectedItem, slot->amount, true, !selectedItemIsFromShop);
		Engine::GetInstance().menuManager->amount->SetMinMax(1, slot->amount, 1);
		const char* text = "Use";
		isUnequipping = false;
		if (w = std::dynamic_pointer_cast<Weapon>(selectedItem)) {
			text = "Equip";
			if (player->inventory->equippedWeapon && player->inventory->equippedWeapon->name == w->name)
			{
				text = "Unequip";
				isUnequipping = true;
			}
		}
		else if (g = std::dynamic_pointer_cast<Gear>(selectedItem)) {
			text = "Equip";
			std::shared_ptr<Gear> equipped = player->inventory->GetGearSlot(g->gearSlot);
			if (equipped && equipped->name == g->name)
			{
				text = "Unequip";
				isUnequipping = true;
			}
		}
		Engine::GetInstance().menuManager->use->text = text;
		Engine::GetInstance().menuManager->use->active = !selectedItemIsFromShop;
		Engine::GetInstance().menuManager->drop->active = !selectedItemIsFromShop;
	}
	switch ((UIID)uiElement->id)
	{
	case START_GAME:
		CopyCleanGameData();
		Engine::GetInstance().menuManager->HideMenu();
		Engine::GetInstance().sceneManager->SetCurrentScene("SC-001");
		break;
	case CONTINUE_GAME: {
		{
			std::ifstream src("Assets/Entities/characters.xml", std::ios::binary);
			std::ofstream dst("Assets/Entities/characters_session.xml", std::ios::binary | std::ios::trunc);
			dst << src.rdbuf();
			src.close();
			dst.close();
		}
		{
			std::ifstream src("Assets/Dialogues/dialogues.xml", std::ios::binary);
			std::ofstream dst("Assets/Dialogues/dialogues_session.xml", std::ios::binary | std::ios::trunc);
			dst << src.rdbuf();
			src.close();
			dst.close();
		}

		Engine::GetInstance().menuManager->HideMenu();
		Engine::GetInstance().sceneManager->SetCurrentScene("SC-001"); // take the last scene from the save data
		break;
	}
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
		dialogManager->SetCurrentDialog();
		//for (const auto& entity : entityManager->entities) entityManager->DestroyEntity(entity);
		if (combat) {
			delete combat;
			combat = nullptr;
		}
		Engine::GetInstance().sceneManager->SetCurrentScene("main menu");
		break;
	case EXIT:
		exit(0);
		break;
	case USE:
		if (!selectedItem || selectedItemIsFromShop) return true;
		if (w = std::dynamic_pointer_cast<Weapon>(selectedItem)) {
			player->inventory->UnequipWeapon();
			if (!isUnequipping) player->inventory->EquipWeapon(w->name);
		}
		else if (g = std::dynamic_pointer_cast<Gear>(selectedItem)) {
			player->inventory->UnequipGear(g->gearSlot);
			if (!isUnequipping) player->inventory->EquipGear(g->name);
		}
		else if (c = std::dynamic_pointer_cast<Consumable>(selectedItem)) {
			amount = Engine::GetInstance().menuManager->amount->GetValue();
			for (int i = 0; i < amount; i++)
			{
				player->TakeConsumable(player->UseConsumable(c->name));
			}
		}
		else {
			// base InteractableItem class / Equipable class
		}
		Engine::GetInstance().menuManager->use->text = "Use";
		Engine::GetInstance().menuManager->RedrawInventory();
		break;
	case DROP:
		if (!selectedItem || selectedItemIsFromShop) return true;
		amount = Engine::GetInstance().menuManager->amount->GetValue();
		for (int i = 0; i < amount; i++) player->inventory->RemoveItem(selectedItem->name);
		Engine::GetInstance().menuManager->RedrawInventory();
		break;
	case BUY:
		if (!selectedItem || !selectedItemIsFromShop) return true;
		amount = Engine::GetInstance().menuManager->amount->GetValue();
		if (player->inventory->gold < selectedItem->price * amount) return true;
		copy = CopyItem(selectedItem);
		copy->count = amount;
		player->inventory->AddItem(copy);
		for (int i = 0; i < amount; i++) shopOwner->inventory->RemoveItem(selectedItem->name);
		player->inventory->AddGold(-selectedItem->price * amount);
		Engine::GetInstance().menuManager->RedrawInventory();
		break;
	case SELL:
		if (!selectedItem || selectedItemIsFromShop) return true;
		amount = Engine::GetInstance().menuManager->amount->GetValue();
		copy = CopyItem(selectedItem);
		copy->count = amount;
		shopOwner->inventory->AddItem(copy);
		for (int i = 0; i < amount; i++) player->inventory->RemoveItem(selectedItem->name);
		player->inventory->AddGold((int)floor(selectedItem->price * SELLING_PRICE_RATIO) * amount);
		Engine::GetInstance().menuManager->RedrawInventory();
		break;
	case EXIT_SHOP:
		if (showingShop) ToggleShop(nullptr);
		break;
	default:
		break;
	}
	return true;
}
