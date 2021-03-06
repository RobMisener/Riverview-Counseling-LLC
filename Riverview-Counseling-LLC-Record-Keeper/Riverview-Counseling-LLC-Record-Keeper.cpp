//
#include "stdafx.h"
#include "FontTexture.h"

 //Using SDL, SDL_image, SDL_ttf, standard IO, math, and strings
#include <sstream> // for
#include <ostream> // for
#include <iostream> // for
#include <sstream>
#include <string>
#include <stdio.h>
#include <windows.h>
#include <stdio.h>
#include <sqlite3.h>
using namespace std;
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h> //for sound
#include <vector>

string inputText[6] = { "Enter Name","Enter Address", "Enter Phone", "Enter Emergency Contact", "Enter DOB", "Enter Occpupation" };

SDL_Window *mainWindow = NULL;
SDL_Renderer* mainRenderer = NULL;

const int SCREEN_WIDTH = 1400;
const int SCREEN_HEIGHT = 1052;
int currentPage = 0;
int imgFlags = IMG_INIT_PNG;
bool isRunning = true;

//SDL_Surface* windowSurface = nullptr;
SDL_Texture* image1 = nullptr;
SDL_Texture* image2 = nullptr;
SDL_Texture* image3 = nullptr;
SDL_Texture* ClientList = nullptr;

SDL_Texture* currentImage = nullptr;

//Globally used font
TTF_Font *font = NULL;

//Rendered texture
FontTexture textTexture;
FontTexture PromptTextTexture[6];
FontTexture InputTextTexture[6];

// Insert a data row into the table 


///////////////////////function declarations
bool initWindow();
void updateCurrentImage(SDL_Texture* updateImageToThis);
void RectToRender(SDL_Rect RenderThisRect);
SDL_Texture* createTexture(string filePath, SDL_Renderer* renderTarget);
void NewPatientView(SDL_Event currentEvent, bool rendText);
void destroyImage(SDL_Surface* imageToDestroy);
void renderImageToScreen(SDL_Texture* imageToRender);
void MainView(SDL_Event currentEvent);
void ClientListView(SDL_Event currentEvent, vector<FontTexture> allClientModelTextures);


///////////////////////Class definitions
class ClientModel {

public:
	string Name;
	string Address;
	string PhoneMumber;
	string EmergencyNumber;
	string DOB;
	string Occupation;
};


ClientModel clientModelSave; vector <ClientModel> clientModelVector;

class ClientDAL {
public:

	ClientModel client
	{

	};

	static int callback(void *NotUsed, int argc, char **argv, char **azColName) {

		ClientModel clientModelTemp;

		//void*,    /* Data provided in the 4th argument of sqlite3_exec() */
		//	int,      /* The number of columns in row */
		//	char**,   /* An array of strings representing fields in the row */
		//	char**    /* An array of strings representing column names */

		NotUsed = 0;

		for (int i = 0; i < argc; i++) {

			printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
		}

		clientModelTemp.Name = argv[0];
		clientModelTemp.Address = argv[1];
		clientModelTemp.PhoneMumber = argv[2];
		clientModelTemp.EmergencyNumber = argv[3];
		clientModelTemp.DOB = argv[4];
		clientModelTemp.Occupation = argv[5];


		cout << clientModelTemp.Name << endl << clientModelTemp.Address << endl << clientModelTemp.PhoneMumber << endl << clientModelTemp.EmergencyNumber;

		////reset vector to current size and assign the client models to its indexes
		//clientModelVector.resize(argc);
		//for (int i = 0; i < clientModelVector.size(); i++) {
		//	clientModelVector[i] = clientModelTemp;
		//}

		clientModelVector.push_back(clientModelTemp);

		return 0;
	}


	void function() {
		sqlite3 *database;

		clientModelVector.clear();

		//query string
		const int STATEMENTS = 8;
		const char *query[STATEMENTS];
		query[0] = "SELECT * FROM person";
		char *err_msg = 0;

		//open database
		int rc = sqlite3_open("person.db", &database);

		//error message if database does not open
		if (rc != SQLITE_OK) {
			fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(database));
			sqlite3_close(database);
		}
		else {
			cout << "database is open" << endl;
		}

		rc = sqlite3_exec(database, query[0], ClientDAL::callback, 0, &err_msg);

		//error message if database selection does not work
		if (rc != SQLITE_OK) {

			fprintf(stderr, "Failed to select data\n");
			fprintf(stderr, "SQL error: %s\n", err_msg);

			sqlite3_free(err_msg);
			sqlite3_close(database);
		}

		//close db
		sqlite3_close(database);
	}

	int addDataRow(ClientModel addClientModel)
	{
		sqlite3 *database;
		//open database
		int rc = sqlite3_open("person.db", &database);

		//error message
		if (rc != SQLITE_OK) {
			fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(database));
			sqlite3_close(database);
		}
		else {
			cout << "database is open" << endl;
		}

		// get the student data from user 
		//student stud = getStudent();

		std::stringstream strm;
		strm << "insert into person(name,address,phoneNum,emerNum,dob,occupation ) values(" << "'" << addClientModel.Name
			<< "','" << addClientModel.Address << "','" << addClientModel.PhoneMumber << "','" << addClientModel.EmergencyNumber << "','" << addClientModel.DOB << "','" << addClientModel.Occupation << "')";

		//strm << "insert into person(name,address,phoneNum,emerNum,dob,occupation ) values('" << addClientModel.Name
		//	<< "','" << addClientModel.Address << "','" << addClientModel.PhoneMumber << "','" << addClientModel.EmergencyNumber << "','" << addClientModel.DOB << "','" << addClientModel.Occupation << ");";

		string s = strm.str();

		char *str = &s[0];

		sqlite3_stmt *statement;
		int result = 0;
		//char *query="insert into student(roll,name,cgpa)values(4,'uuu',6.6)";
		char *query = str;
		{
			if (sqlite3_prepare(database, query, -1, &statement, 0) == SQLITE_OK)
			{
				int res = sqlite3_step(statement);
				result = res;
				sqlite3_finalize(statement);
			}
			return result;
		}

		//empty vector so that new data can be entered
		clientModelVector.clear();

		sqlite3_close(database);

		return 0;
	}


};
class NewPatientController {

public:
	void NewPatientViewAction(SDL_Event currentEvent, bool rendText) {
		NewPatientView(currentEvent, rendText);
	}


};
class MainController {

public:
	void MainViewAction(SDL_Event currentEvent) {
		MainView(currentEvent);
	}
};
class ClientListController {
public:
	vector<FontTexture> AllClientModelTextures;

	void ClientListViewAction(SDL_Event currentEvent) {
		ClientListView(currentEvent, AllClientModelTextures);
	}
	void DestroyVectorTextures() {
		for (int i = 0; i < AllClientModelTextures.size(); i++)
		{
			AllClientModelTextures[i].free();
		}
	}
};

//Class Object Declarartions

namespace init {
	bool initWindow() {
		bool success = true;
		//Create window
		mainWindow = SDL_CreateWindow("Riverview Counseling LLC | Version 0.00.00", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		//Create vsynced renderer for window
		//renderer acceleratd, lets the gpu do things, speeds up stuff
		mainRenderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		//windowSurface = SDL_GetWindowSurface(mainWindow);
		return success;
	}
	bool initImages() {

		bool success = true;

		image1 = createTexture("GUI/Screen/Main.bmp", mainRenderer);
		image2 = createTexture("GUI/Screen/NewClient.bmp", mainRenderer);
		image3 = createTexture("GUI/Screen/Search.bmp", mainRenderer);
		ClientList = createTexture("GUI/Screen/ClientList.bmp", mainRenderer);

		if (image1 == NULL) success = false;
		if (image2 == NULL) success = false;
		if (image3 == NULL) success = false;

		return success;
	}
	bool initText() {
		bool success = true;

		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//initialize SDL_ttf
		if (TTF_Init() == -1)
		{
			printf("SDL_ttf initialization Failure! SDL_ttf Error: %s\n", TTF_GetError());
			success = false;
		}

		//Open the font
		font = TTF_OpenFont("../Riverview-Counseling-LLC-Record-Keeper/GUI/Text/ariblk.ttf", 28);
		if (font == NULL)
		{
			printf("load font failure! SDL_ttf Error: %s\n", TTF_GetError());
			success = false;
		}
		else
		{
			//Render text
			SDL_Color textColor = { 50, 100, 200 };
			if (!textTexture.loadFromRenderedText("The quick brown fox jumps over the lazy dog", textColor))
			{
				printf("render text texture failure!\n");
				success = false;
			}
		}

		return success;
	}
}

namespace buttons {
	SDL_Rect UnitHoverRect(int x, int y, int w, int h) {

		SDL_Rect ReturnRect = { x, y, w, h };
		return ReturnRect;
	}
	void RectToRender(SDL_Rect RenderThisRect) {
		SDL_SetRenderDrawColor(mainRenderer, 30, 100, 10, 0xFF);
		SDL_RenderFillRect(mainRenderer, &RenderThisRect);
	}
}

void MainView(SDL_Event currentEvent) {


	SDL_Rect NewClientButton = buttons::UnitHoverRect(580, 387, 500, 120);
	SDL_Rect ListClientButton = buttons::UnitHoverRect(750, 720, 426, 150);

	renderImageToScreen(image1);

	//look for if mouse is hoverinng over button
	if ((currentEvent.button.x > NewClientButton.x && currentEvent.button.x < (NewClientButton.x + NewClientButton.w)) && (currentEvent.button.y > NewClientButton.y && currentEvent.button.y < (NewClientButton.y + NewClientButton.h)))
	{
		SDL_SetRenderDrawBlendMode(mainRenderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(mainRenderer, 30, 100, 10, 70);
		SDL_RenderFillRect(mainRenderer, &NewClientButton); //draw outline

		if (currentEvent.type == SDL_MOUSEBUTTONDOWN) {
			currentPage = 2;
		}
	}

	//look for if mouse is hoverinng over List Client button
	if ((currentEvent.button.x > ListClientButton.x && currentEvent.button.x < (ListClientButton.x + ListClientButton.w)) && (currentEvent.button.y > ListClientButton.y && currentEvent.button.y < (ListClientButton.y + ListClientButton.h)))
	{
		SDL_SetRenderDrawBlendMode(mainRenderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(mainRenderer, 30, 100, 10, 70);
		SDL_RenderFillRect(mainRenderer, &ListClientButton); //draw outline

		if (currentEvent.type == SDL_MOUSEBUTTONDOWN) {
			currentPage = 3;
		}
	}

}
int feildClickedNewPatient = -1;
void NewPatientView(SDL_Event currentEvent, bool rendText) {

	SDL_Color textColor = { 0, 0, 0, 0xFF };


	//issue: if moue button is down it is a single event. How will program know to edit that section of text after loop resets it?
	//possible answer: make a global bool array[6]. When feild is clicked, it switches that bool to true and the rest to false
	SDL_Rect FeildNameForm = buttons::UnitHoverRect(565, 345, 781, 57);
	SDL_Rect FeildAddressForm = buttons::UnitHoverRect(565, 426, 781, 57);
	SDL_Rect FeildPhoneNumberForm = buttons::UnitHoverRect(565, 504, 781, 57);
	SDL_Rect FeildEmergencyNumberForm = buttons::UnitHoverRect(565, 585, 781, 57);
	SDL_Rect FeildDOBForm = buttons::UnitHoverRect(565, 666, 781, 57);
	SDL_Rect FeildOccupationForm = buttons::UnitHoverRect(565, 744, 781, 57);
	SDL_Rect FeildFormXacHeightWidth = buttons::UnitHoverRect(565, 0, 781, 57);


	if (currentEvent.type == SDL_MOUSEBUTTONDOWN) {


		if ((currentEvent.button.x > FeildFormXacHeightWidth.x && currentEvent.button.x < (FeildFormXacHeightWidth.x + FeildFormXacHeightWidth.w)) &&
			(currentEvent.button.y > FeildNameForm.y && currentEvent.button.y < (FeildNameForm.y + FeildFormXacHeightWidth.h)))
		{
			feildClickedNewPatient = 0;
		}
		if ((currentEvent.button.x > FeildFormXacHeightWidth.x && currentEvent.button.x < (FeildFormXacHeightWidth.x + FeildFormXacHeightWidth.w)) &&
			(currentEvent.button.y > FeildAddressForm.y && currentEvent.button.y < (FeildAddressForm.y + FeildFormXacHeightWidth.h)))
		{

			feildClickedNewPatient = 1;
		}
		if ((currentEvent.button.x > FeildFormXacHeightWidth.x && currentEvent.button.x < (FeildFormXacHeightWidth.x + FeildFormXacHeightWidth.w)) &&
			(currentEvent.button.y > FeildPhoneNumberForm.y && currentEvent.button.y < (FeildPhoneNumberForm.y + FeildFormXacHeightWidth.h)))
		{
			feildClickedNewPatient = 2;
		}
		if ((currentEvent.button.x > FeildFormXacHeightWidth.x && currentEvent.button.x < (FeildFormXacHeightWidth.x + FeildFormXacHeightWidth.w)) &&
			(currentEvent.button.y > FeildEmergencyNumberForm.y && currentEvent.button.y < (FeildEmergencyNumberForm.y + FeildFormXacHeightWidth.h)))
		{
			feildClickedNewPatient = 3;
		}
		if ((currentEvent.button.x > FeildFormXacHeightWidth.x && currentEvent.button.x < (FeildFormXacHeightWidth.x + FeildFormXacHeightWidth.w)) &&
			(currentEvent.button.y > FeildDOBForm.y && currentEvent.button.y < (FeildDOBForm.y + FeildFormXacHeightWidth.h)))
		{
			feildClickedNewPatient = 4;
		}
		if ((currentEvent.button.x > FeildFormXacHeightWidth.x && currentEvent.button.x < (FeildFormXacHeightWidth.x + FeildFormXacHeightWidth.w)) &&
			(currentEvent.button.y > FeildOccupationForm.y && currentEvent.button.y < (FeildOccupationForm.y + FeildFormXacHeightWidth.h)))
		{
			feildClickedNewPatient = 5;
		}
	}

	//render NewPatient Background
	renderImageToScreen(image2);

	//feildClickedNewPatient is global variable keeping track of last thing clicked
	//Special key input
	if (currentEvent.type == SDL_KEYDOWN)
	{
		//Handle backspace
		if (currentEvent.key.keysym.sym == SDLK_BACKSPACE && inputText[feildClickedNewPatient].length() > 0)
		{
			//lop off character
			inputText[feildClickedNewPatient].pop_back();
			rendText = true;
		}
		//Handle copy
		else if (currentEvent.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL)
		{
			SDL_SetClipboardText(inputText[feildClickedNewPatient].c_str());
		}
		//Handle paste
		else if (currentEvent.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL)
		{
			inputText[feildClickedNewPatient] = SDL_GetClipboardText();
			rendText = true;
		}
	}
	//Special text input event
	else if (currentEvent.type == SDL_TEXTINPUT)
	{
		//Not copy or pasting
		if (!((currentEvent.text.text[0] == 'c' || currentEvent.text.text[0] == 'C') && (currentEvent.text.text[0] == 'v' || currentEvent.text.text[0] == 'V') && SDL_GetModState() & KMOD_CTRL))
		{
			//Append character
			inputText[feildClickedNewPatient] += currentEvent.text.text;
			rendText = true;
		}
	}
	//Rerender text if needed
	if (rendText)
	{
		//Text is not empty
		if (inputText[feildClickedNewPatient] != "")
		{
			//Render new text
			InputTextTexture[feildClickedNewPatient].loadFromRenderedText(inputText[feildClickedNewPatient].c_str(), textColor);
		}
		//Text is empty
		else
		{
			//Render space texture
			InputTextTexture[feildClickedNewPatient].loadFromRenderedText(" ", textColor);
		}
	}


	SDL_Rect FeildNull = buttons::UnitHoverRect(0, 0, 0, 0);
	SDL_Rect HighLightThisFeild;


	//highlight clicked feild text area
	switch (feildClickedNewPatient)
	{
	case 0:
		HighLightThisFeild = FeildNameForm;
		break;
	case 1:
		HighLightThisFeild = FeildAddressForm;
		break;
	case 2:
		HighLightThisFeild = FeildPhoneNumberForm;

		break;
	case 3:
		HighLightThisFeild = FeildEmergencyNumberForm;
		break;
	case 4:
		HighLightThisFeild = FeildDOBForm;

		break;
	case 5:
		HighLightThisFeild = FeildOccupationForm;
		break;
	default:
		HighLightThisFeild = FeildNull;
		break;
	}
	//highlighted text area render
	SDL_SetRenderDrawBlendMode(mainRenderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(mainRenderer, 30, 100, 10, 70);
	SDL_RenderFillRect(mainRenderer, &HighLightThisFeild); //draw outline

														   //delete initial starting text, only once
	static bool deleteInitalText[6] = { true,true,true,true,true,true };

	if (deleteInitalText[feildClickedNewPatient] == true) {
		inputText[feildClickedNewPatient].erase(inputText[feildClickedNewPatient].begin(), inputText[feildClickedNewPatient].end());
		deleteInitalText[feildClickedNewPatient] = false;
		InputTextTexture[feildClickedNewPatient].loadFromRenderedText(inputText[feildClickedNewPatient].c_str(), textColor);
	}


	if ((currentEvent.button.x > 36 && currentEvent.button.x < 505) && (currentEvent.button.y > 933 && currentEvent.button.y < 1005))
	{

		SDL_Rect NewRectangleGoBack = buttons::UnitHoverRect(36, 933, 500, 70);
		SDL_SetRenderDrawBlendMode(mainRenderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(mainRenderer, 30, 100, 10, 70);
		SDL_RenderFillRect(mainRenderer, &NewRectangleGoBack); //draw outline

		if (currentEvent.type == SDL_MOUSEBUTTONDOWN) {
			currentPage = 1;
		}
	}
	if ((currentEvent.button.x > 1042 && currentEvent.button.x < 1350) && (currentEvent.button.y > 931 && currentEvent.button.y < 1004))
	{
		SDL_Rect NewRectangleSave = buttons::UnitHoverRect(1042, 931, 308, 72);
		SDL_SetRenderDrawBlendMode(mainRenderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(mainRenderer, 30, 100, 10, 70);
		SDL_RenderFillRect(mainRenderer, &NewRectangleSave); //draw outline



		clientModelSave.Name = inputText[0];
		clientModelSave.Address = inputText[1];
		clientModelSave.PhoneMumber = inputText[2];
		clientModelSave.EmergencyNumber = inputText[3];
		clientModelSave.DOB = inputText[4];
		clientModelSave.Occupation = inputText[5];


		ClientDAL clientDALsave;
		if (currentEvent.type == SDL_MOUSEBUTTONDOWN) {

			clientDALsave.addDataRow(clientModelSave);

			//if saved go to man, go to main
			currentPage = 1;
		}

	}
}

void ClientListView(SDL_Event currentEvent, vector<FontTexture> allClientModelTextures) {


	//whats needed
	//create global/or controller vector of textures
	//free the memory of all textures stored in vector upon client listview start, or create seperate function
	//clear the vector indexes at start of clientlist view

	//get number of people from global clientModelVector
	int numberOfPeople = clientModelVector.size();

	//create number of textures based on number of people
	vector<FontTexture> AllClientModelTextures;
	AllClientModelTextures.clear();
	AllClientModelTextures.resize(numberOfPeople);


	bool success = false;

	//Open the font
	font = TTF_OpenFont("../Riverview-Counseling-LLC-Record-Keeper/GUI/Text/ariblk.ttf", 18);

	//Render the background imaghe to the screen
	renderImageToScreen(ClientList);

	//which location to render the text
	int xx = 112;
	int yy = 309;

	//if font texture does not load, print error message
	if (font == NULL) {
		printf("load font failure! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	//else print text
	else {
		//Text color
		SDL_Color textColor = { 50, 100, 200 };

		for (int i = 0; i < numberOfPeople; i++)
		{
			xx = 112;//x remains constant for horizontal lines

			AllClientModelTextures[i].loadFromRenderedText(clientModelVector[i].Name, textColor);
			AllClientModelTextures[i].render(xx, yy);
			AllClientModelTextures[i].loadFromRenderedText(clientModelVector[i].Address, textColor);
			AllClientModelTextures[i].render(xx += 200, yy);
			AllClientModelTextures[i].loadFromRenderedText(clientModelVector[i].PhoneMumber, textColor);
			AllClientModelTextures[i].render(xx += 200, yy);
			AllClientModelTextures[i].loadFromRenderedText(clientModelVector[i].EmergencyNumber, textColor);
			AllClientModelTextures[i].render(xx += 200, yy);
			AllClientModelTextures[i].loadFromRenderedText(clientModelVector[i].DOB, textColor);
			AllClientModelTextures[i].render(xx += 200, yy);
			AllClientModelTextures[i].loadFromRenderedText(clientModelVector[i].Occupation, textColor);
			AllClientModelTextures[i].render(xx += 200, yy);

			//go down vertically for next line/person
			yy += 40;
		}
	}
	//create and render go back button
	if ((currentEvent.button.x > 36 && currentEvent.button.x < 505) && (currentEvent.button.y > 933 && currentEvent.button.y < 1005))
	{
		SDL_Rect GoBackToMainButton = buttons::UnitHoverRect(36, 933, 500, 70);
		SDL_SetRenderDrawBlendMode(mainRenderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(mainRenderer, 30, 100, 10, 70);
		SDL_RenderFillRect(mainRenderer, &GoBackToMainButton); //draw outline

		//if clickjed, go to 
		if (currentEvent.type == SDL_MOUSEBUTTONDOWN) {
			currentPage = 1;
		}
	}

}

void updateCurrentImage(SDL_Texture* updateImageToThis) {
	currentImage = updateImageToThis;
}
void destroyImage(SDL_Surface* imageToDestroy) {
	SDL_FreeSurface(imageToDestroy);
}

//render an image
void renderImageToScreen(SDL_Texture* imageToRender) {
	//clear the current rendering target with the drawing color.
	SDL_RenderClear(mainRenderer);
	SDL_RenderCopy(mainRenderer, imageToRender, NULL, NULL);
}

SDL_Texture* createTexture(string filePath, SDL_Renderer* renderTarget) {

	SDL_Texture *returnTexture = nullptr;
	SDL_Surface *initialSurface = SDL_LoadBMP(filePath.c_str());

	if (initialSurface == NULL) {
		cout << "Failed to load " + filePath << endl;
	}
	else {
		returnTexture = SDL_CreateTextureFromSurface(renderTarget, initialSurface);
		if (returnTexture == NULL)  cout << "Failed : returnTexture " + filePath << endl;
	}

	//free surface from memory
	SDL_FreeSurface(initialSurface);

	return returnTexture;
}

int main(int argc, char* args[]) {


	currentPage = 1;

	//flags
	bool onMainScreen = false;

	//Initialize SDL
	SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	//Initialize window and renderer
	if (!(init::initWindow())) cout << "initWindow() function failure \n";
	//initialize images and text
	if (!(init::initImages())) cout << "Bmp Images failed in initImages() function \n";
	if (!(init::initText())) cout << "initText() function failure \n";

	MainController MainControl;
	NewPatientController NewPatientControl;
	ClientListController ClientListControl;

	//Set text color as black
	SDL_Color textColor = { 0, 0, 0, 0xFF };
	//The current input text.

	for (int i = 0; i < 6; i++)
	{
		InputTextTexture[i].loadFromRenderedText(inputText[i].c_str(), textColor);
	}

	//Enable text input
	SDL_StartTextInput();

	SDL_Event event; //Event handler

					 //main loop
	while (isRunning) {
		//The rerender text flag
		bool renderText = false;

		while (SDL_PollEvent(&event) != 0) {

			if (event.type == SDL_QUIT) {
				isRunning = false;
			}
			else if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_4)
				{
					isRunning = false;
				}
			}

			static bool page3RenderAgain = true;

			//whatever current page is selected go to that controller
			if (currentPage == 1) MainControl.MainViewAction(event);
			else if (currentPage == 2) NewPatientControl.NewPatientViewAction(event, renderText);
			else if (currentPage == 3) {

				//create data access layer
				ClientDAL clientDAL;

				//if first 
				if (page3RenderAgain == true)
				{
					clientDAL.function();
				}

				ClientListControl.ClientListViewAction(event);

				//if on page still, do not render again
				if (currentPage == 3) {
					page3RenderAgain = false;
				}
				//turn render on again if page 3 changed to new page
				if (currentPage != 3) {
					page3RenderAgain = true;
				}
			}

		}

		if (currentPage == 2) {

			//For Highting the text

			//reference for text feild coordinates for addNewClient view
			SDL_Rect FeildNameForm = buttons::UnitHoverRect(565, 345, 781, 57);
			SDL_Rect FeildAddressForm = buttons::UnitHoverRect(565, 426, 781, 57);
			SDL_Rect FeildPhoneNumberForm = buttons::UnitHoverRect(565, 504, 781, 57);
			SDL_Rect FeildEmergencyNumberForm = buttons::UnitHoverRect(565, 585, 781, 57);
			SDL_Rect FeildDOBForm = buttons::UnitHoverRect(565, 666, 781, 57);
			SDL_Rect FeildOccupationForm = buttons::UnitHoverRect(565, 744, 781, 57);

			PromptTextTexture[0].render((SCREEN_WIDTH - PromptTextTexture[0].getWidth()) / 2, 0);
			InputTextTexture[0].render(FeildNameForm.x + 5, FeildNameForm.y + 5);

			PromptTextTexture[1].render((SCREEN_WIDTH - PromptTextTexture[1].getWidth()) / 2, 0);
			InputTextTexture[1].render(FeildAddressForm.x + 5, FeildAddressForm.y + 5);

			PromptTextTexture[2].render((SCREEN_WIDTH - PromptTextTexture[2].getWidth()) / 2, 0);
			InputTextTexture[2].render(FeildPhoneNumberForm.x + 5, FeildPhoneNumberForm.y + 5);

			PromptTextTexture[3].render((SCREEN_WIDTH - PromptTextTexture[3].getWidth()) / 2, 0);
			InputTextTexture[3].render(FeildEmergencyNumberForm.x + 5, FeildEmergencyNumberForm.y);

			PromptTextTexture[4].render((SCREEN_WIDTH - PromptTextTexture[4].getWidth()) / 2, 0);
			InputTextTexture[4].render(FeildDOBForm.x + 5, FeildDOBForm.y + 5);

			PromptTextTexture[5].render((SCREEN_WIDTH - PromptTextTexture[5].getWidth()) / 2, 0);
			InputTextTexture[5].render(FeildOccupationForm.x + 5, FeildOccupationForm.y + 5);

		}
		//	SDL_RenderPresent(mainRenderer);
		//updates to windows so we can see whats on screen

		SDL_RenderPresent(mainRenderer);
		//SDL_UpdateWindowSurface(mainWindow);
	}

	//Disable text input
	SDL_StopTextInput();

	//destroy windows and images
	SDL_DestroyWindow(mainWindow);
	mainWindow = nullptr;

	//destroy all images
	SDL_DestroyTexture(image1);
	SDL_DestroyTexture(image2);
	SDL_DestroyTexture(image3);
	SDL_DestroyTexture(currentImage);

	//dangling pointer clearing
	currentImage = image1 = image2 = image3 = currentImage = nullptr;

	//Free loaded font texture
	textTexture.free();
	textTexture.free();

	//Free global font
	TTF_CloseFont(font);
	font = NULL;

	//destroy renderer
	SDL_DestroyRenderer(mainRenderer);
	mainRenderer = nullptr;


	//Quit SDL
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	return 0;
}





