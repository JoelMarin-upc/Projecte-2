#include "Party.h"
#include "NPC.h"
#include "Player.h"
#include "XMLHandler.h"

Party::Party(std::shared_ptr<Player> leader)
{
	player = leader;
	members = std::vector<std::shared_ptr<NPC>>();
	allMembers = std::vector<std::shared_ptr<Character>>();
	allMembers.push_back(leader);
}

void Party::AddMember(std::shared_ptr<NPC> member, bool write)
{
	if (!CanAddMember()) return;
	members.push_back(member);
	allMembers.push_back(member);
	member->party = this;
	member->partyIndex = allMembers.size() - 1;

	if (write)
	{
		pugi::xml_document charactersDoc = XMLHandler::LoadFile("Assets/Entities/characters_session.xml");
		pugi::xml_node party = charactersDoc.child("characters").child("player").child("party");
		pugi::xml_node mNode = party.append_child("member");
		mNode.append_attribute("id").set_value(member->id.c_str());
		charactersDoc.save_file("Assets/Entities/characters_session.xml");
	}
}

void Party::RemoveMember(std::string id, bool write)
{
	members.erase(
		std::remove_if(members.begin(), members.end(),
			[id](const std::shared_ptr<NPC> npc) {
				return npc->id == id;
			}),
		members.end()
	);
	allMembers.erase(
		std::remove_if(allMembers.begin(), allMembers.end(),
			[id](const std::shared_ptr<Character> character) {
				return character->id == id;
			}),
		allMembers.end()
	);
	for (int i = 0; i < members.size(); i++) {
		std::shared_ptr<NPC> npc = members[i];
		npc->partyIndex = i;
	}
	if (write)
	{
		pugi::xml_document charactersDoc = XMLHandler::LoadFile("Assets/Entities/characters.xml");
		pugi::xml_node party = charactersDoc.child("characters").child("player").child("party");
		for (pugi::xml_node mNode = party.child("member"); mNode != NULL; mNode = mNode.next_sibling("member")) {
			if (mNode.attribute("id").as_string() == id) party.remove_child(mNode);
			break;
		}
		charactersDoc.save_file("Assets/Entities/characters.xml");
	}
}

bool Party::CanAddMember() const
{
	return members.size() < maxMembers;
}

std::shared_ptr<Character> Party::GetMemberToFollow(int index) const
{
	if (index == 0) return player;
	return allMembers[index - 1];
}
