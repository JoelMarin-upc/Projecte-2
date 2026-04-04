#include "Combat.h"
#include "NPC.h"
#include "Enemy.h"
#include "Window.h"

Combat::Combat(Party* _playerParty, EnemyParty* _enemyParty, std::string mapPath, std::string mapName)
{
	playerParty = _playerParty;
	enemyParty = _enemyParty;
	map = new Map();
	map->Load(mapPath, mapName);
	combatData = map->combatData;
}

Combat::~Combat() {

}

// Called before render is available
bool Combat::Awake() {
	return true;
}

// Called before the first frame
bool Combat::Start() {
	Engine::GetInstance().render->camera.x = combatData.cameraPosition.getX() - Engine::GetInstance().render->camera.w / 2;
	Engine::GetInstance().render->camera.y = combatData.cameraPosition.getY() - Engine::GetInstance().render->camera.h / 2;
	
	int sw = Engine::GetInstance().window->width;
	int sh = Engine::GetInstance().window->height;
	SDL_Rect b_action1 = { sw / 2 - 120, sh - 200, 100, 40 };
	SDL_Rect b_action2 = { sw / 2 + 20, sh - 200, 100, 40 };
	SDL_Rect b_action3 = { sw / 2 - 120, sh - 100, 100, 40 };
	SDL_Rect b_action4 = { sw / 2 + 20, sh - 100, 100, 40 };
	action1 = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)UIID::ACTION1, b_action1, this, { { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 0, 0, 0, 255 } }, -1, -1, UIParameters::Button("Attack")));
	action2 = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)UIID::ACTION2, b_action2, this, { { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 0, 0, 0, 255 } }, -1, -1, UIParameters::Button("Stance")));
	action3 = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)UIID::ACTION3, b_action3, this, { { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 0, 0, 0, 255 } }, -1, -1, UIParameters::Button("Consume")));
	action4 = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)UIID::ACTION4, b_action4, this, { { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 0, 0, 0, 255 } }, -1, -1, UIParameters::Button("Flee")));

	action1->active = false;
	action2->active = false;
	action3->active = false;
	action4->active = false;

	// PLAYER TEAM

	if (playerParty->player)
	{
		SDL_Texture* tex = playerParty->player->texture;
		Vector2D pos;
		for (CombatPosition cPos : combatData.positions) if (!cPos.isEnemy && cPos.order == 0) pos = cPos.position;
		SDL_Rect boundaries = { pos.getX(), pos.getY(), tex->w, tex->h };
		i_player = std::dynamic_pointer_cast<UIImage>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::IMAGE, (int)UIID::C_PLAYER, boundaries, this, {  }, -1, -1, UIParameters::Image(tex, tex, tex, tex)));
		player = playerParty->player;
	}

	if (playerParty->members.size() > 0)
	{
		SDL_Texture* tex = playerParty->members[0]->texture;
		Vector2D pos;
		for (CombatPosition cPos : combatData.positions) if (!cPos.isEnemy && cPos.order == 1) pos = cPos.position;
		SDL_Rect boundaries = { pos.getX(), pos.getY(), tex->w, tex->h };
		i_npc1 = std::dynamic_pointer_cast<UIImage>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::IMAGE, (int)UIID::C_NPC1, boundaries, this, {  }, -1, -1, UIParameters::Image(tex, tex, tex, tex)));
		npc1 = playerParty->members[0];
	}

	if (playerParty->members.size() > 1)
	{
		SDL_Texture* tex = playerParty->members[1]->texture;
		Vector2D pos;
		for (CombatPosition cPos : combatData.positions) if (!cPos.isEnemy && cPos.order == 2) pos = cPos.position;
		SDL_Rect boundaries = { pos.getX(), pos.getY(), tex->w, tex->h };
		i_npc2 = std::dynamic_pointer_cast<UIImage>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::IMAGE, (int)UIID::C_NPC2, boundaries, this, {  }, -1, -1, UIParameters::Image(tex, tex, tex, tex)));
		npc2 = playerParty->members[1];
	}

	if (playerParty->members.size() > 2)
	{
		SDL_Texture* tex = playerParty->members[2]->texture;
		Vector2D pos;
		for (CombatPosition cPos : combatData.positions) if (!cPos.isEnemy && cPos.order == 3) pos = cPos.position;
		SDL_Rect boundaries = { pos.getX(), pos.getY(), tex->w, tex->h };
		i_npc3 = std::dynamic_pointer_cast<UIImage>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::IMAGE, (int)UIID::C_NPC3, boundaries, this, {  }, -1, -1, UIParameters::Image(tex, tex, tex, tex)));
		npc3 = playerParty->members[2];
	}

	// ENEMY TEAM

	if (enemyParty->members.size() > 0)
	{
		SDL_Texture* tex = enemyParty->members[0]->texture;
		Vector2D pos;
		for (CombatPosition cPos : combatData.positions) if (cPos.isEnemy && cPos.order == 0) pos = cPos.position;
		SDL_Rect boundaries = { pos.getX(), pos.getY(), tex->w, tex->h };
		i_enemy1 = std::dynamic_pointer_cast<UIImage>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::IMAGE, (int)UIID::C_ENEMY1, boundaries, this, {  }, -1, -1, UIParameters::Image(tex, tex, tex, tex)));
		enemy1 = enemyParty->members[0];
	}

	if (enemyParty->members.size() > 1)
	{
		SDL_Texture* tex = enemyParty->members[1]->texture;
		Vector2D pos;
		for (CombatPosition cPos : combatData.positions) if (cPos.isEnemy && cPos.order == 1) pos = cPos.position;
		SDL_Rect boundaries = { pos.getX(), pos.getY(), tex->w, tex->h };
		i_enemy2 = std::dynamic_pointer_cast<UIImage>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::IMAGE, (int)UIID::C_ENEMY2, boundaries, this, {  }, -1, -1, UIParameters::Image(tex, tex, tex, tex)));
		enemy2 = enemyParty->members[1];
	}

	if (enemyParty->members.size() > 2)
	{
		SDL_Texture* tex = enemyParty->members[2]->texture;
		Vector2D pos;
		for (CombatPosition cPos : combatData.positions) if (cPos.isEnemy && cPos.order == 2) pos = cPos.position;
		SDL_Rect boundaries = { pos.getX(), pos.getY(), tex->w, tex->h };
		i_enemy3 = std::dynamic_pointer_cast<UIImage>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::IMAGE, (int)UIID::C_ENEMY3, boundaries, this, {  }, -1, -1, UIParameters::Image(tex, tex, tex, tex)));
		enemy3 = enemyParty->members[2];
	}

	if (enemyParty->members.size() > 3)
	{
		SDL_Texture* tex = enemyParty->members[3]->texture;
		Vector2D pos;
		for (CombatPosition cPos : combatData.positions) if (cPos.isEnemy && cPos.order == 3) pos = cPos.position;
		SDL_Rect boundaries = { pos.getX(), pos.getY(), tex->w, tex->h };
		i_enemy4 = std::dynamic_pointer_cast<UIImage>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::IMAGE, (int)UIID::C_ENEMY4, boundaries, this, {  }, -1, -1, UIParameters::Image(tex, tex, tex, tex)));
		enemy4 = enemyParty->members[3];
	}

	return true;
}

// Called before all Updates
bool Combat::PreUpdate() {
	return true;
}

// Called each loop iteration
bool Combat::Update(float dt) {
	map->Update(dt);
	return true;
}

// Called before all Updates
bool Combat::PostUpdate(float dt) {
	return true;
}

// Called before quitting
bool Combat::CleanUp() {
	return true;
}

bool Combat::OnUIMouseClickEvent(UIElement* uiElement) {
	if (!isPlayerTurn) return true;
	if (uiElement->id == END_TURN) {
		isPlayerTurn = false;
	}
	if (turnActions.size() >= 4) return true;
	switch (uiElement->id)
	{
	case C_PLAYER:
		if (turnAction->action != NO_ACTION) {
			turnAction->target = player;
			turnActions.push_back(turnAction);
			turnAction = nullptr;
		}
		else {
			turnAction = new TurnAction();
			turnAction->selected = player;
			ToggleActions(true);
		}
		break;
	case C_NPC1:
		if (turnAction->action != NO_ACTION) {
			turnAction->target = npc1;
			turnActions.push_back(turnAction);
			turnAction = nullptr;
		}
		else {
			turnAction = new TurnAction();
			turnAction->selected = npc1;
			ToggleActions(true);
		}
		break;
	case C_NPC2:
		if (turnAction->action != NO_ACTION) {
			turnAction->target = npc2;
			turnActions.push_back(turnAction);
			turnAction = nullptr;
		}
		else {
			turnAction = new TurnAction();
			turnAction->selected = npc2;
			ToggleActions(true);
		}
		break;
	case C_NPC3:
		if (turnAction->action != NO_ACTION) {
			turnAction->target = npc3;
			turnActions.push_back(turnAction);
			turnAction = nullptr;
		}
		else {
			turnAction = new TurnAction();
			turnAction->selected = npc3;
			ToggleActions(true);
		}
		break;
	case C_ENEMY1:
		if (turnAction->action != NO_ACTION) {
			turnAction->target = enemy1;
			turnActions.push_back(turnAction);
			turnAction = nullptr;
		}
		break;
	case C_ENEMY2:
		if (turnAction->action != NO_ACTION) {
			turnAction->target = enemy2;
			turnActions.push_back(turnAction);
			turnAction = nullptr;
		}
		break;
	case C_ENEMY3:
		if (turnAction->action != NO_ACTION) {
			turnAction->target = enemy3;
			turnActions.push_back(turnAction);
			turnAction = nullptr;
		}
		break;
	case C_ENEMY4:
		if (turnAction->action != NO_ACTION) {
			turnAction->target = enemy4;
			turnActions.push_back(turnAction);
			turnAction = nullptr;
		}
		break;
	case ACTION1:
		if (!turnAction->selected) return true;
		turnAction->action = Action::ATTACK;
		ToggleActions(false);
		break;
	case ACTION2:
		if (!turnAction->selected) return true;
		turnAction->action = Action::TAKE_STANCE;
		ToggleActions(false);
		break;
	case ACTION3:
		if (!turnAction->selected) return true;
		turnAction->action = Action::TAKE_CONSUMABLE;
		ToggleActions(false);
		break;
	case ACTION4:
		if (!turnAction->selected) return true;
		turnAction->action = Action::FLEE;
		turnActions.push_back(turnAction);
		turnAction = nullptr;
		ToggleActions(false);
		break;
	default:
		break;
	}
	return true;
}

void Combat::ToggleActions(bool show)
{
	action1->active = show;
	action2->active = show;
	action3->active = show;
	action4->active = show;
}
