#pragma once

#include "NPC.h"
#include "EnemyParty.h"

enum class BossPhase {
    INTRO_DIALOGUE,
    IN_COMBAT,
    OUTRO_DIALOGUE,
    DONE
};

class BossNPC : public NPC {
public:
    BossNPC() {}
    BossNPC(std::string id, std::string name, std::string texturePath, std::string combatTexturePath, std::string introDlgId, std::string outroDlgId)
        : NPC(id, name, texturePath, combatTexturePath, NPCInteractionType::DIALOGUE), introDlgId(introDlgId), outroDlgId(outroDlgId) {}

    virtual ~BossNPC();

    bool Start() override;
    bool Update(float dt) override;

    void OnDialogEnd() override;

    void OnCombatWon();

public:
    enum class EndingChoice { NONE, MERCY, KILL };
    EndingChoice endingChoice = EndingChoice::NONE;

    BossPhase phase = BossPhase::INTRO_DIALOGUE;

    EnemyParty* bossParty = nullptr;

    std::string introDlgId;
    std::string outroDlgId;
};