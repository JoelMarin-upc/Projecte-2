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
		std::string missionId = missionNode.attribute("id").as_string();
		std::string targetId = missionNode.attribute("targetId").as_string();
		std::string targetName = missionNode.attribute("targetName").as_string();
		int rewardGold = missionNode.attribute("rewardGold").as_int();
		std::string rewardItemName = missionNode.attribute("rewardItemName").as_string();
		bool active = missionNode.attribute("active").as_bool();
		bool completed = missionNode.attribute("completed").as_bool();
		std::vector<std::string> unlocksMissions;
		for (pugi::xml_node missionNode = root.child("mission"); missionNode != NULL; missionNode = missionNode.next_sibling("mission")) {
			std::string missionId = missionNode.attribute("id").as_string();
			if (missionId == "") unlocksMissions.push_back(missionId);
		}
		AddMission(missionId, targetId, targetName, MissionReward(rewardGold, rewardItemName), unlocksMissions, active, completed);
	}
}

Mission* MissionManager::AddMission(std::string missionId, std::string targetId, std::string targetName, MissionReward reward, std::vector<std::string> unlocksMissions, bool active, bool completed)
{
	Mission* mission = new Mission(missionId, targetId, targetName, reward, unlocksMissions, active, completed);
	missions.push_back(mission);
	return mission;
}

Mission* MissionManager::CompleteMission(std::string missionId)
{
	for (Mission* mission : missions)
	{
		if (mission->id == missionId)
		{
			mission->active = false;
			mission->completed = true;

			for (std::string unlock : mission->unlocksMissions) 
				for (Mission* mission : missions) 
					if (mission->id == unlock)
						mission->active = true;

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
