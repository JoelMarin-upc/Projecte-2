#include "MissionManager.h"

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
	return true;
}

bool MissionManager::Update(float dt) {
	return true;
}

void MissionManager::LoadMissions()
{
}

void MissionManager::SaveMissions()
{
}

Mission* MissionManager::AddMission(std::string missionId, std::string targetId, std::string targetName, MissionReward reward, std::vector<std::string> unlocksMissions)
{
	return nullptr;
}

MissionReward MissionManager::CompleteMission(std::string missionId)
{
	return MissionReward();
}

std::vector<Mission*> MissionManager::GetActiveMissions()
{
	return std::vector<Mission*>();
}
