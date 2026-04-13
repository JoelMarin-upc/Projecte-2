#include "Combat.h"
#include "NPC.h"
#include "Enemy.h"
#include "Window.h"
#include <random>
#include <thread>
#include <chrono>

int random_int(int min, int max) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(min, max);
	return dist(gen);
}

void sleep_seconds(int seconds) {
	std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

Combat::Combat(Party* _playerParty, EnemyParty* _enemyParty, std::string mapPath, std::string mapName)
{
	playerParty = _playerParty;
	enemyParty = _enemyParty;
	map = new Map();
	map->Load(mapPath, mapName);
	combatData = map->combatData;
}

Combat::~Combat() {
	Engine::GetInstance().uiManager->DestroyUIElement(action1);
	Engine::GetInstance().uiManager->DestroyUIElement(action2);
	Engine::GetInstance().uiManager->DestroyUIElement(action3);
	Engine::GetInstance().uiManager->DestroyUIElement(action4);
	Engine::GetInstance().uiManager->DestroyUIElement(endTurn);
	Engine::GetInstance().uiManager->DestroyUIElement(i_player);
	Engine::GetInstance().uiManager->DestroyUIElement(i_npc1);
	Engine::GetInstance().uiManager->DestroyUIElement(i_npc2);
	Engine::GetInstance().uiManager->DestroyUIElement(i_npc3);
	Engine::GetInstance().uiManager->DestroyUIElement(i_enemy1);
	Engine::GetInstance().uiManager->DestroyUIElement(i_enemy2);
	Engine::GetInstance().uiManager->DestroyUIElement(i_enemy3);
	Engine::GetInstance().uiManager->DestroyUIElement(i_enemy4);
	Engine::GetInstance().uiManager->DestroyUIElement(log1);
	Engine::GetInstance().uiManager->DestroyUIElement(log2);
	Engine::GetInstance().uiManager->DestroyUIElement(log3);
	Engine::GetInstance().uiManager->DestroyUIElement(log4);
	Engine::GetInstance().uiManager->DestroyUIElement(hint);
}

// Called before render is available
bool Combat::Awake() {
	return true;
}

// Called before the first frame
bool Combat::Start() {
	Engine::GetInstance().render->camera.x = -combatData.cameraPosition.getX() + Engine::GetInstance().render->camera.w / 2;
	Engine::GetInstance().render->camera.y = -combatData.cameraPosition.getY() + Engine::GetInstance().render->camera.h / 2;

	int sw = Engine::GetInstance().window->width;
	int sh = Engine::GetInstance().window->height;
	int cx = Engine::GetInstance().render->camera.x;
	int cy = Engine::GetInstance().render->camera.y;

	SDL_Rect b_action1 = { sw / 2 - 145, sh - 200, 150, 40 };
	SDL_Rect b_action2 = { sw / 2 + 45, sh - 200, 150, 40 };
	SDL_Rect b_action3 = { sw / 2 - 145, sh - 100, 150, 40 };
	SDL_Rect b_action4 = { sw / 2 + 45, sh - 100, 150, 40 };
	SDL_Rect b_endTurn = { sw - 165, sh - 100, 150, 40 };
	SDL_Rect b_log1 = { sw / 2, 20, 300, 20 };
	SDL_Rect b_log2 = { sw / 2, 60, 300, 20 };
	SDL_Rect b_log3 = { sw / 2, 100, 300, 20 };
	SDL_Rect b_log4 = { sw / 2, 140, 300, 20 };
	SDL_Rect b_hint = { 20, 20, 300, 20 };
	
	action1 = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)UIID::ACTION1, b_action1, this, { { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 0, 0, 0, 255 } }, -1, -1, UIParameters::Button("Attack")));
	action2 = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)UIID::ACTION2, b_action2, this, { { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 0, 0, 0, 255 } }, -1, -1, UIParameters::Button("Stance")));
	action3 = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)UIID::ACTION3, b_action3, this, { { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 0, 0, 0, 255 } }, -1, -1, UIParameters::Button("Consume")));
	action4 = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)UIID::ACTION4, b_action4, this, { { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 0, 0, 0, 255 } }, -1, -1, UIParameters::Button("Flee")));
	
	stance1 = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)UIID::STANCE1, b_action1, this, { { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 0, 0, 0, 255 } }, -1, -1, UIParameters::Button("Assist")));
	stance2 = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)UIID::STANCE2, b_action2, this, { { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 0, 0, 0, 255 } }, -1, -1, UIParameters::Button("Concentrate")));
	stance3 = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)UIID::STANCE3, b_action3, this, { { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 0, 0, 0, 255 } }, -1, -1, UIParameters::Button("Defend")));
	stance4 = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)UIID::STANCE4, b_action4, this, { { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 0, 0, 0, 255 } }, -1, -1, UIParameters::Button("Rest")));

	endTurn = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)UIID::END_TURN, b_endTurn, this, { { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 0, 0, 0, 255 } }, -1, -1, UIParameters::Button("End turn")));

	log1 = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, (int)UIID::LOG1, b_log1, this, { { 255, 255, 255, 255 }, { 255, 255, 255, 255 } }, -1, -1, UIParameters::Label("")));
	log2 = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, (int)UIID::LOG2, b_log2, this, { { 255, 255, 255, 255 }, { 255, 255, 255, 255 } }, -1, -1, UIParameters::Label("")));
	log3 = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, (int)UIID::LOG3, b_log3, this, { { 255, 255, 255, 255 }, { 255, 255, 255, 255 } }, -1, -1, UIParameters::Label("")));
	log4 = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, (int)UIID::LOG4, b_log4, this, { { 255, 255, 255, 255 }, { 255, 255, 255, 255 } }, -1, -1, UIParameters::Label("")));

	hint = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, (int)UIID::HINT, b_hint, this, { { 255, 255, 255, 255 }, { 255, 255, 255, 255 } }, -1, -1, UIParameters::Label("")));

	action1->active = false;
	action2->active = false;
	action3->active = false;
	action4->active = false;

	log1->active = true;
	log2->active = true;
	log3->active = true;
	log4->active = true;
	ToggleStances(false);

	// PLAYER TEAM

	if (playerParty->player)
	{
		SDL_Texture* tex = playerParty->player->texture;
		Vector2D pos;
		for (CombatPosition cPos : combatData.positions) if (!cPos.isEnemy && cPos.order == 0) pos = cPos.position;
		SDL_Rect boundaries = { pos.getX() + cx, pos.getY() + cy, tex->w, tex->h };
		i_player = std::dynamic_pointer_cast<UIImage>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::IMAGE, (int)UIID::C_PLAYER, boundaries, this, {  }, -1, -1, UIParameters::Image(tex, tex, tex, tex)));
		player = playerParty->player;
		i_player->active = true;
		player->hasFled = false;
		player->isDead = false;
		player->position = pos;
	}

	if (playerParty->members.size() > 0)
	{
		SDL_Texture* tex = playerParty->members[0]->texture;
		Vector2D pos;
		for (CombatPosition cPos : combatData.positions) if (!cPos.isEnemy && cPos.order == 1) pos = cPos.position;
		SDL_Rect boundaries = { pos.getX() + cx, pos.getY() + cy, tex->w, tex->h };
		i_npc1 = std::dynamic_pointer_cast<UIImage>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::IMAGE, (int)UIID::C_NPC1, boundaries, this, {  }, -1, -1, UIParameters::Image(tex, tex, tex, tex)));
		npc1 = playerParty->members[0];
		i_npc1->active = true;
		npc1->hasFled = false;
		npc1->isDead = false;
		npc1->position = pos;
	}

	if (playerParty->members.size() > 1)
	{
		SDL_Texture* tex = playerParty->members[1]->texture;
		Vector2D pos;
		for (CombatPosition cPos : combatData.positions) if (!cPos.isEnemy && cPos.order == 2) pos = cPos.position;
		SDL_Rect boundaries = { pos.getX() + cx, pos.getY() + cy, tex->w, tex->h };
		i_npc2 = std::dynamic_pointer_cast<UIImage>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::IMAGE, (int)UIID::C_NPC2, boundaries, this, {  }, -1, -1, UIParameters::Image(tex, tex, tex, tex)));
		npc2 = playerParty->members[1];
		i_npc2->active = true;
		npc2->hasFled = false;
		npc2->isDead = false;
		npc2->position = pos;
	}

	if (playerParty->members.size() > 2)
	{
		SDL_Texture* tex = playerParty->members[2]->texture;
		Vector2D pos;
		for (CombatPosition cPos : combatData.positions) if (!cPos.isEnemy && cPos.order == 3) pos = cPos.position;
		SDL_Rect boundaries = { pos.getX() + cx, pos.getY() + cy, tex->w, tex->h };
		i_npc3 = std::dynamic_pointer_cast<UIImage>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::IMAGE, (int)UIID::C_NPC3, boundaries, this, {  }, -1, -1, UIParameters::Image(tex, tex, tex, tex)));
		npc3 = playerParty->members[2];
		i_npc3->active = true;
		npc3->hasFled = false;
		npc3->isDead = false;
		npc3->position = pos;
	}

	// ENEMY TEAM

	if (enemyParty->members.size() > 0)
	{
		SDL_Texture* tex = enemyParty->members[0]->texture;
		Vector2D pos;
		for (CombatPosition cPos : combatData.positions) if (cPos.isEnemy && cPos.order == 0) pos = cPos.position;
		SDL_Rect boundaries = { pos.getX() + cx, pos.getY() + cy, tex->w, tex->h };
		i_enemy1 = std::dynamic_pointer_cast<UIImage>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::IMAGE, (int)UIID::C_ENEMY1, boundaries, this, {  }, -1, -1, UIParameters::Image(tex, tex, tex, tex)));
		enemy1 = enemyParty->members[0];
		i_enemy1->active = true;
		enemy1->hasFled = false;
		enemy1->isDead = false;
		enemy1->position = pos;
	}

	if (enemyParty->members.size() > 1)
	{
		SDL_Texture* tex = enemyParty->members[1]->texture;
		Vector2D pos;
		for (CombatPosition cPos : combatData.positions) if (cPos.isEnemy && cPos.order == 1) pos = cPos.position;
		SDL_Rect boundaries = { pos.getX() + cx, pos.getY() + cy, tex->w, tex->h };
		i_enemy2 = std::dynamic_pointer_cast<UIImage>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::IMAGE, (int)UIID::C_ENEMY2, boundaries, this, {  }, -1, -1, UIParameters::Image(tex, tex, tex, tex)));
		enemy2 = enemyParty->members[1];
		i_enemy2->active = true;
		enemy2->hasFled = false;
		enemy2->isDead = false;
		enemy2->position = pos;
	}

	if (enemyParty->members.size() > 2)
	{
		SDL_Texture* tex = enemyParty->members[2]->texture;
		Vector2D pos;
		for (CombatPosition cPos : combatData.positions) if (cPos.isEnemy && cPos.order == 2) pos = cPos.position;
		SDL_Rect boundaries = { pos.getX() + cx, pos.getY() + cy, tex->w, tex->h };
		i_enemy3 = std::dynamic_pointer_cast<UIImage>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::IMAGE, (int)UIID::C_ENEMY3, boundaries, this, {  }, -1, -1, UIParameters::Image(tex, tex, tex, tex)));
		enemy3 = enemyParty->members[2];
		i_enemy3->active = true;
		enemy3->hasFled = false;
		enemy3->isDead = false;
		enemy3->position = pos;
	}

	if (enemyParty->members.size() > 3)
	{
		SDL_Texture* tex = enemyParty->members[3]->texture;
		Vector2D pos;
		for (CombatPosition cPos : combatData.positions) if (cPos.isEnemy && cPos.order == 3) pos = cPos.position;
		SDL_Rect boundaries = { pos.getX() + cx, pos.getY() + cy, tex->w, tex->h };
		i_enemy4 = std::dynamic_pointer_cast<UIImage>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::IMAGE, (int)UIID::C_ENEMY4, boundaries, this, {  }, -1, -1, UIParameters::Image(tex, tex, tex, tex)));
		enemy4 = enemyParty->members[3];
		i_enemy4->active = true;
		enemy4->hasFled = false;
		enemy4->isDead = false;
		enemy4->position = pos;
	}

	float highestSpeed = 0;
	highestSpeed = player->stats->GetStat("speed").getValue();
	if (npc1) {
		float newSpeed = npc1->stats->GetStat("speed").getValue();
		if (highestSpeed < newSpeed) highestSpeed = newSpeed;
	}
	if (npc2) {
		float newSpeed = npc2->stats->GetStat("speed").getValue();
		if (highestSpeed < newSpeed) highestSpeed = newSpeed;
	}
	if (npc3) {
		float newSpeed = npc3->stats->GetStat("speed").getValue();
		if (highestSpeed < newSpeed) highestSpeed = newSpeed;
	}

	float highestEnemySpeed = 0;
	if (enemy1) {
		float newSpeed = enemy1->stats->GetStat("speed").getValue();
		if (highestEnemySpeed < newSpeed) highestEnemySpeed = newSpeed;
	}
	if (enemy2) {
		float newSpeed = enemy2->stats->GetStat("speed").getValue();
		if (highestEnemySpeed < newSpeed) highestEnemySpeed = newSpeed;
	}
	if (enemy3) {
		float newSpeed = enemy3->stats->GetStat("speed").getValue();
		if (highestEnemySpeed < newSpeed) highestEnemySpeed = newSpeed;
	}
	if (enemy4) {
		float newSpeed = enemy4->stats->GetStat("speed").getValue();
		if (highestEnemySpeed < newSpeed) highestEnemySpeed = newSpeed;
	}

	highestSpeed += random_int(1, 10);
	highestEnemySpeed += random_int(1, 10);

	isPlayerTurn = highestSpeed >= highestEnemySpeed;
	combatPhase = isPlayerTurn ? DECISION : ENEMY_TURN;
	endTurn->active = isPlayerTurn;
	enemyTurnTimerActive = false;
	if (isPlayerTurn) hint->text = "Select a character";

	return true;
}

// Called before all Updates
bool Combat::PreUpdate() {
	return true;
}

// Called each loop iteration
bool Combat::Update(float dt) {
	map->Update(dt);
	switch (combatPhase)
	{
	case DECISION:
		break;
	case ACTION:
		for (TurnAction* action : turnActions) {
			switch (action->action)
			{
			case ATTACK:
				if (action->target->TakeDamage(action->selected->Attack())) KillCombatant(action->target);
				break;
			case TAKE_STANCE:
				action->selected->TakeStance(action->stance);
				break;
			case TAKE_CONSUMABLE:
				action->target->TakeConsumable(action->selected->UseConsumable(action->consumableType));
				break;
			case FLEE:
				CombatantFlees(action->selected);
				break;
			default:
				break;
			}
		}
		isPlayerTurn = !isPlayerTurn;
		combatPhase = isPlayerTurn ? DECISION : ENEMY_TURN;
		endTurn->active = isPlayerTurn;
		turnActions.clear();
		if (isPlayerTurn) {
			if (player) player->CheckModifiers();
			if (npc1) npc1->CheckModifiers();
			if (npc2) npc2->CheckModifiers();
			if (npc3) npc3->CheckModifiers();
		}
		else {
			if (enemy1) enemy1->CheckModifiers();
			if (enemy2) enemy2->CheckModifiers();
			if (enemy3) enemy3->CheckModifiers();
			if (enemy4) enemy4->CheckModifiers();
		}
		if (isPlayerTurn) hint->text = "Select a character";
		break;
	case ENEMY_TURN:
		if (enemyTurnTimerActive && enemyTurnTimer.ReadMSec() < enemyTurnMS) break;
		if (!enemyTurnTimerActive)
		{
			for (std::shared_ptr<Enemy> enemy : enemyParty->members) {
				CreateRandomAction(enemy);
			}
			enemyTurnTimerActive = true;
			enemyTurnTimer.Start();
		}
		if (enemyTurnTimerActive && enemyTurnTimer.ReadMSec() >= enemyTurnMS) EndTurn();
		break;
	default:
		break;
	}
	DrawHealthBars();
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
	if (Engine::GetInstance().uiManager->uiLockFrame == Engine::GetInstance().frameCount) return true;

	if (combatPhase != DECISION) return true;
	if (uiElement->id == END_TURN) {
		EndTurn();
	}
	if (turnActions.size() >= 4) return true;
	switch (uiElement->id)
	{
	case C_PLAYER:
		if (turnAction && turnAction->action != NO_ACTION) {
			turnAction->target = player;
			AddTurnAction();
		}
		else {
			if (actionTaken1) return true;
			turnAction = new TurnAction();
			turnAction->selected = player;
			ToggleActions(true);
		}
		break;
	case C_NPC1:
		if (turnAction && turnAction->action != NO_ACTION) {
			turnAction->target = npc1;
			AddTurnAction();
		}
		else {
			if (actionTaken2) return true;
			turnAction = new TurnAction();
			turnAction->selected = npc1;
			ToggleActions(true);
		}
		break;
	case C_NPC2:
		if (turnAction && turnAction->action != NO_ACTION) {
			turnAction->target = npc2;
			AddTurnAction();
		}
		else {
			if (actionTaken3) return true;
			turnAction = new TurnAction();
			turnAction->selected = npc2;
			ToggleActions(true);
		}
		break;
	case C_NPC3:
		if (turnAction && turnAction->action != NO_ACTION) {
			turnAction->target = npc3;
			AddTurnAction();
		}
		else {
			if (actionTaken4) return true;
			turnAction = new TurnAction();
			turnAction->selected = npc3;
			ToggleActions(true);
		}
		break;
	case C_ENEMY1:
		if (turnAction && turnAction->action != NO_ACTION) {
			turnAction->target = enemy1;
			AddTurnAction();
		}
		break;
	case C_ENEMY2:
		if (turnAction && turnAction->action != NO_ACTION) {
			turnAction->target = enemy2;
			AddTurnAction();
		}
		break;
	case C_ENEMY3:
		if (turnAction && turnAction->action != NO_ACTION) {
			turnAction->target = enemy3;
			AddTurnAction();
		}
		break;
	case C_ENEMY4:
		if (turnAction && turnAction->action != NO_ACTION) {
			turnAction->target = enemy4;
			AddTurnAction();
		}
		break;
	case ACTION1:
		if (!turnAction || !turnAction->selected) return true;
		turnAction->action = Action::ATTACK;
		ToggleActions(false);
		break;
	case ACTION2:
		if (!turnAction || !turnAction->selected) return true;
		turnAction->action = Action::TAKE_STANCE;
		ToggleActions(false);
		ToggleStances(true);
		break;
	case ACTION3:
		if (!turnAction || !turnAction->selected) return true;
		turnAction->action = Action::TAKE_CONSUMABLE;
		ToggleActions(false);
		break;
	case ACTION4:
		if (!turnAction || !turnAction->selected) return true;
		turnAction->action = Action::FLEE;
		AddTurnAction();
		ToggleActions(false);
		break;
	case STANCE1:
		if (!turnAction || !turnAction->selected) return true;
		turnAction->stance = Stance::ASSIST;
		AddTurnAction();
		ToggleStances(false);
		break;
	case STANCE2:
		if (!turnAction || !turnAction->selected) return true;
		turnAction->stance = Stance::CONCENTRATE;
		AddTurnAction();
		ToggleStances(false);
		break;
	case STANCE3:
		if (!turnAction || !turnAction->selected) return true;
		turnAction->stance = Stance::DEFEND;
		AddTurnAction();
		ToggleStances(false);
		break;
	case STANCE4:
		if (!turnAction || !turnAction->selected) return true;
		turnAction->stance = Stance::REST;
		AddTurnAction();
		ToggleStances(false);
		break;
	default:
		break;
	}
	return true;
}

void Combat::ToggleActions(bool show)
{
	Engine::GetInstance().uiManager->uiLockFrame = Engine::GetInstance().frameCount;
	action1->active = show;
	action2->active = show;
	action3->active = show;
	action4->active = show;
	if (show) hint->text = "Select an action";
	else hint->text = "Select a target";
}

void Combat::ToggleStances(bool show)
{
	Engine::GetInstance().uiManager->uiLockFrame = Engine::GetInstance().frameCount;
	stance1->active = show;
	stance2->active = show;
	stance3->active = show;
	stance4->active = show;
	if (show) hint->text = "Select a stance";
}

void Combat::KillCombatant(std::shared_ptr<Character> character)
{
	//character->isDead = true;
	if (player && character->id == player->id) player->isDead = true;
	if (npc1 && character->id == npc1->id) npc1->isDead = true;
	if (npc2 && character->id == npc2->id) npc2->isDead = true;
	if (npc3 && character->id == npc3->id) npc3->isDead = true;
	if (enemy1 && character->id == enemy1->id) enemy1->isDead = true;
	if (enemy2 && character->id == enemy2->id) enemy2->isDead = true;
	if (enemy3 && character->id == enemy3->id) enemy3->isDead = true;
	if (enemy4 && character->id == enemy4->id) enemy4->isDead = true;
	if (player->isDead) combatResult = LOSE;
	bool alive1 = enemy1 && !enemy1->isDead;
	bool alive2 = enemy2 && !enemy2->isDead;
	bool alive3 = enemy3 && !enemy3->isDead;
	bool alive4 = enemy4 && !enemy4->isDead;

	if (!alive1 && !alive2 && !alive3 && !alive4) combatResult = WIN;
	ShowCombatants();
}

void Combat::CombatantFlees(std::shared_ptr<Character> character)
{
	character->hasFled = true;
	if (player->hasFled) combatResult = FLED;
	ShowCombatants();
}

void Combat::ShowCombatants()
{
	if (player) i_player->active = !player->isDead && !player->hasFled;
	if (npc1) i_npc1->active = !npc1->isDead && !npc1->hasFled;
	if (npc2) i_npc2->active = !npc2->isDead && !npc2->hasFled;
	if (npc3) i_npc3->active = !npc3->isDead && !npc3->hasFled;
	if (enemy1) i_enemy1->active = !enemy1->isDead && !enemy1->hasFled;
	if (enemy2) i_enemy2->active = !enemy2->isDead && !enemy2->hasFled;
	if (enemy3) i_enemy3->active = !enemy3->isDead && !enemy3->hasFled;
	if (enemy4) i_enemy4->active = !enemy4->isDead && !enemy4->hasFled;
}

void Combat::AddTurnAction()
{
	turnActions.push_back(turnAction);
	if (turnActions.size() == 1) log1->text = turnAction->ToString();
	if (turnActions.size() == 2) log2->text = turnAction->ToString();
	if (turnActions.size() == 3) log3->text = turnAction->ToString();
	if (turnActions.size() == 4) log4->text = turnAction->ToString();
	if (player && turnAction->selected->id == player->id) actionTaken1 = true;
	if (npc1 && turnAction->selected->id == npc1->id) actionTaken2 = true;
	if (npc2 && turnAction->selected->id == npc2->id) actionTaken3 = true;
	if (npc3 && turnAction->selected->id == npc3->id) actionTaken4 = true;
	turnAction = nullptr;
	int activeCharacters = 1;
	for (std::shared_ptr<NPC> c : playerParty->members) if (!c->isDead && !c->hasFled) activeCharacters++;
	if (isPlayerTurn && turnActions.size() == activeCharacters) {
		hint->text = "End the turn";
	}
	else if (isPlayerTurn && turnActions.size() < activeCharacters) {
		hint->text = "Select a character";
	}
}

void Combat::EndTurn()
{
	enemyTurnTimerActive = false;
	combatPhase = ACTION;
	endTurn->active = false;
	actionTaken1 = false;
	actionTaken2 = false;
	actionTaken3 = false;
	actionTaken4 = false;
	log1->text = "";
	log2->text = "";
	log3->text = "";
	log4->text = "";
	hint->text = "";
}

void Combat::CreateRandomAction(std::shared_ptr<Enemy> enemy)
{
	turnAction = new TurnAction();
	turnAction->selected = enemy;
	//int action = random_int(1, 3); <-- when consumables are implemeted, 4 if flee is possible
	int action = random_int(1, 2);
	if (action == 1) {
		turnAction->action = Action::ATTACK;
		int activeCharacters = 1;
		for (std::shared_ptr<NPC> c : playerParty->members) if (!c->isDead && !c->hasFled) activeCharacters++;
		int target = random_int(1, activeCharacters);
		if (target == 1) turnAction->target = player;
		else if (target == 2) turnAction->target = npc1;
		else if (target == 3) turnAction->target = npc2;
		else if (target == 4) turnAction->target = npc3;
	}
	else if (action == 2) {
		turnAction->action = Action::TAKE_STANCE;
		int stance = random_int(1, 4);
		if (stance == 1) turnAction->stance = Stance::ASSIST;
		else if (stance == 2) turnAction->stance = Stance::CONCENTRATE;
		else if (stance == 3) turnAction->stance = Stance::DEFEND;
		else if (stance == 4) turnAction->stance = Stance::REST;
	}
	//else if (action == 3) {                       <-- when consumables are implemeted
	// turnAction->action = Action::TAKE_CONSUMABLE;
	//}

	AddTurnAction();
}

void Combat::DrawHealthBars()
{
	if (player) player->DrawHealthBar(player->texture);
	if (npc1) npc1->DrawHealthBar(npc1->texture);
	if (npc2) npc2->DrawHealthBar(npc2->texture);
	if (npc3) npc3->DrawHealthBar(npc3->texture);
	if (enemy1) enemy1->DrawHealthBar(enemy1->texture);
	if (enemy2) enemy2->DrawHealthBar(enemy2->texture);
	if (enemy3) enemy3->DrawHealthBar(enemy3->texture);
	if (enemy4) enemy4->DrawHealthBar(enemy4->texture);
}
