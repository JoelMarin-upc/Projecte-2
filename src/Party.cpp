#include "Party.h"
#include "NPC.h"
#include "XMLHandler.h"

Party::Party(std::shared_ptr<Player> leader)
{
	player = leader;
	members = std::vector<std::shared_ptr<NPC>>();
}

void Party::AddMember(std::shared_ptr<NPC> member, bool write)
{
	if (!CanAddMember()) return;
	members.push_back(member);
	member->party = this;

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
