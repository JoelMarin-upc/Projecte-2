#include "EnemyParty.h"
#include "Enemy.h"

EnemyParty::EnemyParty(std::shared_ptr<Enemy> _leader)
{
	leader = _leader;
	members = std::vector<std::shared_ptr<Character>>();
	AddMember(_leader);
}

void EnemyParty::AddMember(std::shared_ptr<Enemy> member)
{
	if (!member) return;
	if (!CanAddMember()) return;
    for (const auto& existing : members) {
        if (existing->id == member->id) return;
    }
    if (member->party != this) {
        delete member->party;
    }

    members.push_back(member);
    member->party = this;
}

void EnemyParty::AddBossMember(std::shared_ptr<Character> member)
{
	if (!member) return;
	if (!CanAddMember()) return;
	for (const auto& existing : members) {
		if (existing->id == member->id) return;
	}
	members.push_back(member);
}

bool EnemyParty::CanAddMember() const
{
	return members.size() < maxMembers;
}