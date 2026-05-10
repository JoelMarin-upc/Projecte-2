#include "MissionManager.h"
#include "XMLHandler.h"
#include "pugixml.hpp"

MissionManager::MissionManager()
{
}

MissionManager::~MissionManager()
{
}

bool MissionManager::Awake() {
	return true;
}

bool MissionManager::Start() {
	LoadMissions();
	return true;
}

bool MissionManager::Update(float dt) {
	return true;
}

void MissionManager::LoadMissions()
{
	pugi::xml_document missionDoc = XMLHandler::LoadFile("Assets/Missions/missions.xml");
	pugi::xml_node root = missionDoc.child("missions");

	for (pugi::xml_node missionNode = root.child("mission"); missionNode != NULL; missionNode = missionNode.next_sibling("mission")) {
		int missionType = missionNode.attribute("type").as_int();
		std::string missionId = missionNode.attribute("id").as_string();
		std::string targetId = missionNode.attribute("targetId").as_string();
		std::string targetName = missionNode.attribute("targetName").as_string();
		std::string itemName = missionNode.attribute("itemName").as_string();
		int rewardGold = missionNode.attribute("rewardGold").as_int();
		std::string rewardItemName = missionNode.attribute("rewardItemName").as_string();
		bool active = missionNode.attribute("active").as_bool();
		bool completed = missionNode.attribute("completed").as_bool();
		std::vector<std::string> unlocksMissions;
		for (pugi::xml_node unlockNode = missionNode.child("unlocks").child("mission"); unlockNode != NULL; unlockNode = unlockNode.next_sibling("unlock")) {
			std::string unlockId = unlockNode.attribute("id").as_string();
			if (unlockId != "") unlocksMissions.push_back(unlockId);
		}
		AddMission((MissionType)missionType, missionId, targetId, targetName, itemName, MissionReward(rewardGold, rewardItemName), unlocksMissions, active, completed);
	}
}

Mission* MissionManager::AddMission(MissionType type, std::string missionId, std::string targetId, std::string targetName, std::string itemName, MissionReward reward, std::vector<std::string> unlocksMissions, bool active, bool completed)
{
	Mission* mission = nullptr;
	switch (type)
	{
	case TALK:
		mission = new TalkMission(missionId, targetId, targetName, reward, unlocksMissions, active, completed);
		break;
	case REACH:
		mission = new ReachMission(missionId, targetId, targetName, reward, unlocksMissions, active, completed);
		break;
	case KILL:
		mission = new KillMission(missionId, targetId, targetName, reward, unlocksMissions, active, completed);
		break;
	case COLLECT:
		mission = new CollectMission(missionId, targetId, targetName, reward, unlocksMissions, active, completed);
		break;
	case BRING:
		mission = new BringMission(missionId, targetId, targetName, itemName, reward, unlocksMissions, active, completed);
		break;
	default:
		break;
	}
	if (mission) missions.push_back(mission);
	return mission;
}

Mission* MissionManager::ActivateMission(std::string missionId)
{
	for (Mission* mission : missions) {
		if (mission->id == missionId && !mission->completed) {
			mission->active = true;
			return mission;
		}
	}
}

Mission* MissionManager::CompleteMission(std::string missionId)
{
	for (Mission* mission : missions)
	{
		if (mission->id == missionId)
		{
			mission->active = false;
			mission->completed = true;

			for (std::string unlock : mission->unlocksMissions) {
				for (Mission* unlockMission : missions) {
					if (unlockMission->id == unlock) {
						unlockMission->active = true;
						mission->unlocksMissionsValues.push_back(unlockMission);
					}
				}
			}

			return mission;
		}
	}
	return nullptr;
}

std::vector<Mission*> MissionManager::GetActiveMissions()
{
	std::vector<Mission*> activeMissions = std::vector<Mission*>();
	for (Mission* mission : missions) if (mission->active) activeMissions.push_back(mission);
	return activeMissions;
}
