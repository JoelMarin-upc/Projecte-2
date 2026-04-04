#pragma once

#include "Module.h"
#include "Map.h"
#include "Party.h"
#include "EnemyParty.h"
#include "UIManager.h"
#include "MenuManager.h"

enum Action {
	ATTACK,
	TAKE_STANCE,
	TAKE_CONSUMABLE,
	FLEE,
	NO_ACTION
};

struct TurnAction {
	TurnAction() {
		selected = nullptr;
		target = nullptr;
		action = NO_ACTION;
	}
	std::shared_ptr<Entity> selected;
	std::shared_ptr<Entity> target;
	Action action;
};

class Combat : public Module {
public:

	Combat(Party* _playerParty, EnemyParty* _enemyParty, std::string mapPath, std::string mapName);

	virtual ~Combat();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();

	bool OnUIMouseClickEvent(UIElement* uiElement);

	void ToggleActions(bool show);

	Party* playerParty;
	EnemyParty* enemyParty;
	Map* map;
	CombatData combatData;

	std::shared_ptr<UIImage> i_player;
	std::shared_ptr<UIImage> i_npc1;
	std::shared_ptr<UIImage> i_npc2;
	std::shared_ptr<UIImage> i_npc3;
	std::shared_ptr<UIImage> i_enemy1;
	std::shared_ptr<UIImage> i_enemy2;
	std::shared_ptr<UIImage> i_enemy3;
	std::shared_ptr<UIImage> i_enemy4;

	std::shared_ptr<Player> player;
	std::shared_ptr<NPC> npc1;
	std::shared_ptr<NPC> npc2;
	std::shared_ptr<NPC> npc3;
	std::shared_ptr<Enemy> enemy1;
	std::shared_ptr<Enemy> enemy2;
	std::shared_ptr<Enemy> enemy3;
	std::shared_ptr<Enemy> enemy4;

	std::shared_ptr<UIButton> action1;
	std::shared_ptr<UIButton> action2;
	std::shared_ptr<UIButton> action3;
	std::shared_ptr<UIButton> action4;

	bool isPlayerTurn = true;
	
	TurnAction* turnAction = nullptr;

	std::vector<TurnAction*> turnActions;
	
};