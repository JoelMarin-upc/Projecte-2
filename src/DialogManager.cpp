#include "DialogManager.h"
#include "XMLHandler.h"
#include <limits>
#include <algorithm>
#include "Engine.h"
#include "UIManager.h"
#include "Window.h"
#include "SceneManager.h"
#include "Textures.h"
#include "Log.h"

DialogManager::DialogManager()
{
}

DialogManager::~DialogManager()
{
}

bool DialogManager::Awake() {
	return true;
}

// Called after Awake
bool DialogManager::Start() {

	LoadDialogs();

	int sw = BASE_W;
	int sh = BASE_H;

	dialogBox = Engine::GetInstance().textures->Load("Assets/Dialogues/back.png");
	dialogText = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, (int)LABEL, { sw / 2 - 110, sh - 140, 220, 40 }, this, { { 0, 0, 0, 255 }, { 0, 0, 0, 255 } }, -1, -1, UIParameters::Label("")));
	speakerName = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, (int)SPEAKER_NAME, { sw / 2 - 130, sh - 160, 100, 40 }, this, { { 0, 0, 0, 255 }, { 0, 0, 0, 255 } }, -1, -1, UIParameters::Label("")));
	answer1 = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)ANSWER1, { sw / 2 - 110, sh - 70, 100, 20 }, this, { { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 0, 0, 0, 255 } }, -1, -1, UIParameters::Button("")));
	answer2 = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)ANSWER2, { sw / 2 + 10, sh - 70, 100, 20 }, this, { { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 0, 0, 0, 255 } }, -1, -1, UIParameters::Button("")));
	answer3 = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)ANSWER3, { sw / 2 - 110, sh - 40, 100, 20 }, this, { { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 0, 0, 0, 255 } }, -1, -1, UIParameters::Button("")));
	answer4 = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)ANSWER4, { sw / 2 + 10, sh - 40, 100, 20 }, this, { { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 0, 0, 0, 255 } }, -1, -1, UIParameters::Button("")));

	SetCurrentDialog();

	return true;
}

bool DialogManager::Update(float dt) {
	int sw = BASE_W;
	int sh = BASE_H;
	if (currentDialog) Engine::GetInstance().render->DrawTexture(dialogBox, sw / 2 - 130, sh - 160);
	return true;
}

bool DialogManager::CleanUp() {
	return true;
}

void DialogManager::LoadDialogs()
{
	dialogs = std::list<DialogTree*>();
	pugi::xml_document doc = XMLHandler::LoadFile("Assets/Dialogues/test.xml"); // get all dialogues from Assets/Dialogues/...
	pugi::xml_node root = doc.child("dialogs");
	for (pugi::xml_node treeNode = root.child("tree"); treeNode != NULL; treeNode = treeNode.next_sibling("tree")) {
		DialogTree* tree = new DialogTree();
		tree->id = treeNode.attribute("id").as_string();
		tree->characterId = treeNode.attribute("characterId").as_string();
		tree->characterName = treeNode.attribute("characterName").as_string();
		tree->order = treeNode.attribute("order").as_int();
		tree->done = treeNode.attribute("done").as_bool();
		tree->nodes = std::vector<DialogNode*>();

		for (pugi::xml_node nodeXml = treeNode.child("node"); nodeXml != NULL; nodeXml = nodeXml.next_sibling("node")) {
			DialogNode* node = new DialogNode();
			node->id = nodeXml.attribute("id").as_string();
			node->first = nodeXml.attribute("first").as_bool();
			node->text = nodeXml.child_value();
			node->text.erase(std::remove(node->text.begin(), node->text.end(), '\n'), node->text.end());
			node->text.erase(std::remove(node->text.begin(), node->text.end(), '\t'), node->text.end());
			node->answers = std::vector<DialogAnswer*>();

			if (node->first) tree->currentNode = node;

			for (pugi::xml_node answerNode = nodeXml.child("answer"); answerNode != NULL; answerNode = answerNode.next_sibling("answer")) {
				DialogAnswer* answer = new DialogAnswer();
				answer->leadsToNodeId = answerNode.attribute("leadsToNodeId").as_string();
				answer->text = answerNode.child_value();
				answer->text.erase(std::remove(answer->text.begin(), answer->text.end(), '\n'), answer->text.end());
				answer->text.erase(std::remove(answer->text.begin(), answer->text.end(), '\t'), answer->text.end());
				node->answers.push_back(answer);
			}

			tree->nodes.push_back(node);
		}
		dialogs.push_back(tree);
	}
}

bool DialogManager::SetCurrentDialog(std::string characterId)
{
	if (characterId == "")
	{
		currentDialog = nullptr;
		dialogText->text = "";
		speakerName->text = "";
		answer1->text = "";
		answer2->text = "";
		answer3->text = "";
		answer4->text = "";
		dialogText->active = false;
		speakerName->active = false;
		answer1->active = false;
		answer2->active = false;
		answer3->active = false;
		answer4->active = false;
		return false;
	}

	DialogTree* dialog = nullptr;
	int bestOrder = std::numeric_limits<int>::max();

	for (DialogTree* t : dialogs)
	{
		if (t->characterId != characterId) continue;
		if (t->done) continue;

		if (t->order < bestOrder)
		{
			bestOrder = t->order;
			dialog = t;
		}
	}

	currentDialog = dialog;
	ShowDialog();
	return currentDialog != nullptr;
}

void DialogManager::ShowDialog()
{
	if (!currentDialog) return;

	DialogNode* node = currentDialog->currentNode;

	if (!node) return;

	dialogText->text = node->text;
	dialogText->active = true;

	speakerName->text = currentDialog->characterName;
	speakerName->active = true;

	if (node->answers.size() > 0) {
		answer1->active = true;
		answer1->text = node->answers[0]->text;
	}
	else {
		answer1->active = false;
		answer1->text = "";
	}

	if (node->answers.size() > 1) {
		answer2->active = true;
		answer2->text = node->answers[1]->text;
	}
	else {
		answer2->active = false;
		answer2->text = "";
	}

	if (node->answers.size() > 2) {
		answer3->active = true;
		answer3->text = node->answers[2]->text;
	}
	else {
		answer3->active = false;
		answer3->text = "";
	}

	if (node->answers.size() > 3) {
		answer4->active = true;
		answer4->text = node->answers[3]->text;
	}
	else {
		answer4->active = false;
		answer4->text = "";
	}
}

bool DialogManager::OnUIMouseClickEvent(UIElement* uiElement)
{
	int answerNum;
	switch ((DIALOG_UIID)uiElement->id)
	{
	case ANSWER1: 
		answerNum = 0;
		break;
	case ANSWER2:
		answerNum = 1;
		break;
	case ANSWER3:
		answerNum = 2;
		break;
	case ANSWER4:
		answerNum = 3;
		break;
	default:
		answerNum = 0;
		break;
	}

	std::string nextId = currentDialog->currentNode->answers[answerNum]->leadsToNodeId;

	if (nextId == "") {
		currentDialog->done = true;
		SetCurrentDialog();
		Engine::GetInstance().sceneManager->currentScene->EndDialog();
		return true;
	}

	for (DialogNode* n : currentDialog->nodes)
	{
		if (n->id == nextId)
		{
			currentDialog->currentNode = n;
			break;
		}
	}

	ShowDialog();

	return true;
}

void DialogManager::ResizeDialogBox()
{
	int sw = BASE_W;
	int sh = BASE_H;

	//WRITE HERE THE SAME VALUES AS THE BOUNDS ESTABLISHED IN DialogManager::Start()!!!!!!!!!!!!!!!
	dialogText->bounds = { sw / 2 - 110, sh - 140, 220, 40 };
	speakerName->bounds = { sw / 2 - 130, sh - 160, 100, 40 };
	answer1->bounds = { sw / 2 - 110, sh - 70,  100, 20 };
	answer2->bounds = { sw / 2 + 10,  sh - 70,  100, 20 };
	answer3->bounds = { sw / 2 - 110, sh - 40,  100, 20 };
	answer4->bounds = { sw / 2 + 10,  sh - 40,  100, 20 };
}
