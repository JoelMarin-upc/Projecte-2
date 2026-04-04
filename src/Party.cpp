#include "Party.h"
#include "NPC.h"

Party::Party(std::shared_ptr<Player> leader)
{
	player = leader;
	members = std::vector<std::shared_ptr<NPC>>();
}

void Party::AddMember(std::shared_ptr<NPC> member)
{
	if (!CanAddMember()) return;
	members.push_back(member);
	member->party = this;
}

void Party::RemoveMember(std::string id)
{
	members.erase(
		std::remove_if(members.begin(), members.end(),
			[id](const std::shared_ptr<NPC> npc) {
				return npc->id == id;
			}),
		members.end()
	);
}

bool Party::CanAddMember() const
{
	return members.size() < maxMembers;
}
