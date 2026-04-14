#pragma once

#include "Module.h"
#include "Map.h"
#include "Party.h"
#include "EnemyParty.h"
#include "UIManager.h"
#include "MenuManager.h"
#include "Character.h"
#include "Timer.h"

enum CombatPhase {
	DECISION,
	ACTION,
	ENEMY_TURN
};

enum CombatResult {
	WIN,
	LOSE,
	FLED,
	NO_RESULT
};

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
		stance = NO_STANCE;
		consumableType = "";
	}
	std::shared_ptr<Character> selected;
	std::shared_ptr<Character> target;
	Action action;
	Stance stance;
	std::string consumableType;
	std::string ToString() const {
		std::string action;
		switch (this->action) {
		case ATTACK:
			action = "Attack";
			action += " (" + std::to_string((int)selected->stats->GetStat("attack").getValue()) + ")";
			break;
		case TAKE_STANCE:
			action = "Take Stance";
			break;
		case TAKE_CONSUMABLE:
			action = "Take Consumable";
			break;
		case FLEE:
			action = "Flee";
			break;
		}
		std::string s = selected->name + " -> " + action;
		if (stance != NO_STANCE)
		{
			std::string stance;
			switch (this->stance) {
			case ASSIST:
				stance = "Assist";
				break;
			case CONCENTRATE:
				stance = "Concentrate";
				break;
			case DEFEND:
				stance = "Defend";
				break;
			case REST:
				stance = "Rest";
				break;
			}
			s += "(" + stance + ")";
		}
		if (consumableType != "") s += "(" + consumableType + ")";
		if (target) s += " -> " + target->name;
		return s;
	}
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
	void ToggleStances(bool show);

	void KillCombatant(std::shared_ptr<Character> character);
	void CombatantFlees(std::shared_ptr<Character> character);
	
	void ShowCombatants();

	void AddTurnAction();

	void EndTurn();

	void CreateRandomAction(std::shared_ptr<Enemy> enemy);

	void DrawHealthBars() const;

	Party* playerParty;
	EnemyParty* enemyParty;
	Map* map;
	CombatData combatData;

	std::shared_ptr<UIImage> i_player = nullptr;
	std::shared_ptr<UIImage> i_npc1 = nullptr;
	std::shared_ptr<UIImage> i_npc2 = nullptr;
	std::shared_ptr<UIImage> i_npc3 = nullptr;
	std::shared_ptr<UIImage> i_enemy1 = nullptr;
	std::shared_ptr<UIImage> i_enemy2 = nullptr;
	std::shared_ptr<UIImage> i_enemy3 = nullptr;
	std::shared_ptr<UIImage> i_enemy4 = nullptr;

	std::shared_ptr<Player> player;
	std::shared_ptr<NPC> npc1;
	std::shared_ptr<NPC> npc2;
	std::shared_ptr<NPC> npc3;
	std::shared_ptr<Enemy> enemy1;
	std::shared_ptr<Enemy> enemy2;
	std::shared_ptr<Enemy> enemy3;
	std::shared_ptr<Enemy> enemy4;

	std::shared_ptr<UIButton> action1 = nullptr;
	std::shared_ptr<UIButton> action2 = nullptr;
	std::shared_ptr<UIButton> action3 = nullptr;
	std::shared_ptr<UIButton> action4 = nullptr;
	std::shared_ptr<UIButton> stance1 = nullptr;
	std::shared_ptr<UIButton> stance2 = nullptr;
	std::shared_ptr<UIButton> stance3 = nullptr;
	std::shared_ptr<UIButton> stance4 = nullptr;
	std::shared_ptr<UIButton> endTurn = nullptr;

	std::shared_ptr<UILabel> log1 = nullptr;
	std::shared_ptr<UILabel> log2 = nullptr;
	std::shared_ptr<UILabel> log3 = nullptr;
	std::shared_ptr<UILabel> log4 = nullptr;

	std::shared_ptr<UILabel> hint = nullptr;

	CombatPhase combatPhase = DECISION;
	CombatResult combatResult = NO_RESULT;

	bool actionTaken1 = false;
	bool actionTaken2 = false;
	bool actionTaken3 = false;
	bool actionTaken4 = false;

	bool isPlayerTurn;
	
	const int enemyTurnMS = 4000;
	bool enemyTurnTimerActive = false;
	Timer enemyTurnTimer;
	
	TurnAction* turnAction = nullptr;

	std::vector<TurnAction*> turnActions;
	
};