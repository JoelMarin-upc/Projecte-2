#include "Party.h"
#include "NPC.h"

Party::Party(Player* leader)
{
	player = leader;
	members = std::list<std::shared_ptr<NPC>>();
}

void Party::AddMember(std::shared_ptr<NPC> member)
{
	if (!CanAddMember()) return;
	members.push_back(member);
	member->party = this;
}

void Party::RemoveMember(std::string id)
{
	members.remove_if([id](const std::shared_ptr<NPC> npc) {
		return npc->id == id;
	});
}

bool Party::CanAddMember() const
{
	return members.size() < maxMembers;
}
