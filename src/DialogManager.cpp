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

	//IF ANY OF THE BOUNDS ARE MODIFIED, COPY AND PASTE THE SAME VALUES AT ResizeDialogBox()
	dialogBox = Engine::GetInstance().textures->Load("Assets/Dialogues/TextBox.png");
	speakerName = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, (int)SPEAKER_NAME, { 100, sh - 150, 100, 40 }, this, { { 0, 0, 0, 255 }, { 0, 0, 0, 255 } }, -1, -1, UIParameters::Label("")));
	dialogText = std::dynamic_pointer_cast<UILabel>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::LABEL, (int)LABEL, { 100, sh - 120, 420, 40 }, this, { { 0, 0, 0, 255 }, { 0, 0, 0, 255 } }, -1, -1, UIParameters::Label("")));
	
	answerBox = Engine::GetInstance().textures->Load("Assets/Dialogues/answerBoxDialog.png");
	answer1 = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)ANSWER1, { sw / 2 + 60, sh - 210, 500, 40 }, this, { }, -1, -1, UIParameters::Button("")));
	answer2 = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)ANSWER2, { sw / 2 + 60, sh - 260, 500, 40 }, this, { }, -1, -1, UIParameters::Button("")));
	answer3 = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)ANSWER3, { sw / 2 + 60, sh - 280, 500, 40 }, this, { }, -1, -1, UIParameters::Button("")));
	answer4 = std::dynamic_pointer_cast<UIButton>(Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, (int)ANSWER4, { sw / 2 + 60, sh - 300, 500, 40 }, this, { }, -1, -1, UIParameters::Button("")));

	SetCurrentDialog();

	return true;
}

bool DialogManager::Update(float dt) {
	int sw = BASE_W;
	int sh = BASE_H;
	if (currentDialog) 
	{ 
		Engine::GetInstance().render->DrawTexture(dialogBox, 0, 0, 0.0f);
		if (answer1->active) Engine::GetInstance().render->DrawTexture(answerBox, 700, 510, 0.0f);
		if (answer2->active) Engine::GetInstance().render->DrawTexture(answerBox, 700, 460, 0.0f);
		if (answer3->active) Engine::GetInstance().render->DrawTexture(answerBox, 700, 440, 0.0f);
		if (answer4->active) Engine::GetInstance().render->DrawTexture(answerBox, 700, 420, 0.0f);
		LOG("%d %d", Engine::GetInstance().render->camera.x, Engine::GetInstance().render->camera.y);
	}
	return true;
}

bool DialogManager::CleanUp() {
	return true;
}

void DialogManager::LoadDialogs()
{
	dialogs = std::list<DialogTree*>();
	pugi::xml_document doc = XMLHandler::LoadFile("Assets/Dialogues/dialogues.xml"); // get all dialogues from Assets/Dialogues/...
	pugi::xml_node root = doc.child("dialogs");
	for (pugi::xml_node treeNode = root.child("tree"); treeNode != NULL; treeNode = treeNode.next_sibling("tree")) {
		DialogTree* tree = new DialogTree();
		tree->id = treeNode.attribute("id").as_string();
		tree->characterId = treeNode.attribute("characterId").as_string();
		tree->characterName = treeNode.attribute("characterName").as_string();
		tree->order = treeNode.attribute("order").as_int();
		tree->done = treeNode.attribute("done").as_bool();
		tree->isRepeatable = treeNode.attribute("isRepeatable").as_bool(false);
		tree->nodes = std::vector<DialogNode*>();

		for (pugi::xml_node nodeXml = treeNode.child("node"); nodeXml != NULL; nodeXml = nodeXml.next_sibling("node")) {
			DialogNode* node = new DialogNode();
			node->id = nodeXml.attribute("id").as_string();
			node->first = nodeXml.attribute("first").as_bool();
			node->text = nodeXml.child_value();
			node->text.erase(std::remove(node->text.begin(), node->text.end(), '\n'), node->text.end());
			node->text.erase(std::remove(node->text.begin(), node->text.end(), '\t'), node->text.end());

			// Convert | to newline
			std::replace(node->text.begin(), node->text.end(), '|', '\n');

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

		speakerName->text = "";
		dialogText->text = "";

		answer1->text = "";
		answer2->text = "";
		answer3->text = "";
		answer4->text = "";

		speakerName->active = false;
		dialogText->active = false;

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

	speakerName->text = currentDialog->characterName;
	speakerName->active = true;

	dialogText->text = node->text;
	dialogText->active = true;

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
		if (!currentDialog->isRepeatable) {
			currentDialog->done = true;
		}
		else {
			for (DialogNode* n : currentDialog->nodes) {
				if (n->first) {
					currentDialog->currentNode = n;
					break;
				}
			}
		}
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
	speakerName->bounds = { 100, sh - 150, 100, 40 };
	dialogText->bounds = { 100, sh - 120, 420, 40 };

	answer1->bounds = { sw / 2 + 60, sh - 200, 500, 40 };
	answer2->bounds = { sw / 2 + 60, sh - 250, 500, 40 };
	answer3->bounds = { sw / 2 + 60, sh - 270, 500, 40 };
	answer4->bounds = { sw / 2 + 60, sh - 290, 500, 40 };
}
