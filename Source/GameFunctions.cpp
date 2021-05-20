#include "stdafx.h"


float Cheat::GameFunctions::GetGameFramesPerSecond()
{
	static int		iFrames = 0;
	static clock_t	clockFrames = clock();
	static float	iFps;
	iFrames++;
	clock_t dif = clock() - clockFrames;
	if (dif > 500)
	{
		iFps = iFrames / (dif / 1000.f);
		iFrames = 0;
		clockFrames = clock();
	}
	return iFps;
}

void Cheat::GameFunctions::DrawGameFramesPerSecond()
{
	std::string str = std::to_string(GetGameFramesPerSecond());
	while (str.size() > str.find(".")) { str.pop_back(); }
	std::string MessageString = xorstr_("FPS: ") + str;
	Cheat::fps((char*)MessageString.c_str());
}


void Cheat::GameFunctions::GiveAllWeaponsToPlayer(Ped Player)
{
	for (int i = 0; i < (sizeof(Cheat::GameArrays::WeaponsHashList) / sizeof Cheat::GameArrays::WeaponsHashList[0]); i++)
	{
		WEAPON::GIVE_DELAYED_WEAPON_TO_PED(Player, Cheat::GameArrays::WeaponsHashList[i], 9999, false);
		WAIT(0.1);
	}
}

void Cheat::GameFunctions::RepairAndCleanVehicle()
{
	if (PED::IS_PED_IN_ANY_VEHICLE(PlayerPedID, false)) {
		VEHICLE::SET_VEHICLE_FIXED(PED::GET_VEHICLE_PED_IS_USING(PlayerPedID));
		ENTITY::SET_ENTITY_HEALTH(PED::GET_VEHICLE_PED_IS_USING(PlayerPedID), ENTITY::GET_ENTITY_MAX_HEALTH(PED::GET_VEHICLE_PED_IS_USING(PlayerPedID)));
		VEHICLE::SET_VEHICLE_DIRT_LEVEL(PED::GET_VEHICLE_PED_IS_USING(PlayerPedID), 0);
		VEHICLE::SET_VEHICLE_ENGINE_HEALTH(PED::GET_VEHICLE_PED_IS_USING(PlayerPedID), 1000);
		VEHICLE::SET_VEHICLE_ENGINE_ON(PED::GET_VEHICLE_PED_IS_USING(PlayerPedID), true, true, true);
		UI::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
		UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(xorstr_("Vehicle Fixed & Cleaned"));
		UI::_SET_NOTIFICATION_MESSAGE_CLAN_TAG(xorstr_("CHAR_LS_CUSTOMS"), xorstr_("CHAR_LS_CUSTOMS"), false, 7, xorstr_("Simple Customs"), "", 1.0, "");
		UI::_DRAW_NOTIFICATION(false, false);
	}
	else 
	{
		notifyleft(xorstr_("~r~Player isn't in a vehicle"));
	}
}



int torso = 0;
int torsotexture = 0;
int face = 0;
int facetexture = 0;
int head = 0;
int headtexture = 0;
int hair = 0;
int hairtexture = 0;
int legs = 0;
int legstexture = 0;
int hands = 0;
int handstexture = 0;
int feet = 0;
int feettexture = 0;
int eyes = 0;
int eyestexture = 0;
int accesories = 0;
int accesoriestexture = 0;
int accesoriessec = 0;
int accesoriessectexture = 0;
int textures = 0;
int texturestexture = 0;
int torsosec = 0;
int torsosectexture = 0;
void Cheat::GameFunctions::SetPedTexture(Ped Ped, int ComponentID, int DrawableID, int TextureID) 
{ 
	PED::SET_PED_COMPONENT_VARIATION(Ped, ComponentID, DrawableID, TextureID, 0);
}


void Cheat::GameFunctions::TeleportToObjective()
{
	Entity e;
	Vector3 wayp;
	Ped playerPed = PlayerPedID;
	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, FALSE))
		e = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	else e = playerPed;
	bool blipFound = false;
	if (ENTITY::IS_ENTITY_A_VEHICLE(e)) RequestControlOfEnt(e);
	for (int i = 0; i <= 1000; i++)
	{
		int blipIterator = UI::IS_WAYPOINT_ACTIVE() ? UI::_GET_BLIP_INFO_ID_ITERATOR() : SpriteStandard;    
		for (Blip i = UI::GET_FIRST_BLIP_INFO_ID(blipIterator);
			UI::DOES_BLIP_EXIST(i) != 0; i = UI::GET_NEXT_BLIP_INFO_ID(blipIterator)) {
			if (UI::GET_BLIP_INFO_ID_TYPE(i) == 4 && UI::GET_BLIP_COLOUR(i) == 5 != ColorBlue && UI::IS_BLIP_ON_MINIMAP(i) == 1) {
				wayp = UI::GET_BLIP_INFO_ID_COORD(i);
				blipFound = true;
				Cheat::GameFunctions::TeleportToCoords(e, wayp);
			}
			bool groundFound = false;
			static float groundCheckHeight[] =
			{ 100.0, 150.0, 50.0, 0.0, 200.0, 250.0, 300.0, 350.0, 400.0, 450.0, 500.0, 550.0, 600.0, 650.0, 700.0, 750.0, 800.0 };
			for (int i = 0; i < sizeof(groundCheckHeight) / sizeof(float); i++)
			{
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(e, wayp.x, wayp.y, groundCheckHeight[i], 0, 0, 1);
				WAIT(1);
				if (GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(wayp.x, wayp.y, groundCheckHeight[i], &wayp.z, 0))
				{
					groundFound = true;
					wayp.z += 3.0;
					break;
				}
			}
			if (!groundFound)
			{
				wayp.z = 1000.0;
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PlayerPedID, 0xFBAB5776, 1, 0);
			}
		}
		break;
	}
	if (!blipFound) {
		Blip i = UI::GET_FIRST_BLIP_INFO_ID(SpriteRaceFinish);
		if (UI::DOES_BLIP_EXIST(i) != 0) {
			wayp = UI::GET_BLIP_INFO_ID_COORD(i);
			blipFound = true;
		}
	}
	blipFound ? Cheat::GameFunctions::TeleportToCoords(e, wayp) : notifyleft(xorstr_("~r~Objective not found"));
}


void Cheat::GameFunctions::BurstSelectedPlayerTires(Ped selectedPed)
{
	if (PED::IS_PED_IN_ANY_VEHICLE(selectedPed, FALSE))
	{
		RequestControl(PED::GET_VEHICLE_PED_IS_USING(selectedPed));
		VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(PED::GET_VEHICLE_PED_IS_USING(selectedPed), TRUE);
		static int tireID = 0;
		for (tireID = 0; tireID < 8; tireID++)
		{
			VEHICLE::SET_VEHICLE_TYRE_BURST(PED::GET_VEHICLE_PED_IS_USING(selectedPed), tireID, true, 1000.0);
		}
	}
}

void Cheat::GameFunctions::SetOffAlarmPlayerVehicle(Ped selectedPed)
{
	if (PED::IS_PED_IN_ANY_VEHICLE(selectedPed, FALSE))
	{
		Entity selectedVehicle = PED::GET_VEHICLE_PED_IS_USING(selectedPed);
		RequestControl(PED::GET_VEHICLE_PED_IS_USING(selectedPed));
		VEHICLE::SET_VEHICLE_ALARM(selectedVehicle, true);
		VEHICLE::START_VEHICLE_ALARM(selectedVehicle);
		notifyleft(xorstr_("~g~Set off alarm of vehicle!"));
	}
}


bool Cheat::GameFunctions::IsPlayerFriend(Player player)
{
	BOOL BplayerFriend = false;
	bool bplayerFriend = false;
	int handle[76];
	NETWORK::NETWORK_HANDLE_FROM_PLAYER(player, &handle[0], 13);
	if (NETWORK::NETWORK_IS_HANDLE_VALID(&handle[0], 13))
	{
		BplayerFriend = NETWORK::NETWORK_IS_FRIEND(&handle[0]);
	}
	if (BplayerFriend == 1) 
	{ 
		bplayerFriend = true; 
	}
	else
	{
		bplayerFriend = false;
	}
	return bplayerFriend;
}

Vector3 Cheat::GameFunctions::MultiplyVector(Vector3 vector, float inc) {
	vector.x *= inc;
	vector.y *= inc;
	vector.z *= inc;
	vector._paddingx *= inc;
	vector._paddingy *= inc;
	vector._paddingz *= inc;
	return vector;
}

Vector3 Cheat::GameFunctions::AddVector(Vector3 vector, Vector3 vector2) {
	vector.x += vector2.x;
	vector.y += vector2.y;
	vector.z += vector2.z;
	vector._paddingx += vector2._paddingx;
	vector._paddingy += vector2._paddingy;
	vector._paddingz += vector2._paddingz;
	return vector;
}




double Cheat::GameFunctions::DegreeToRadian(double n) {
	return n * 0.017453292519943295;
}
Vector3 Cheat::GameFunctions::RotationToDirection(Vector3 rot) {
	double num = DegreeToRadian(rot.z);
	double num2 = DegreeToRadian(rot.x);
	double val = cos(num2);
	double num3 = abs(val);
	rot.x = (float)(-(float)sin(num) * num3);
	rot.y = (float)(cos(num) * num3);
	rot.z = (float)sin(num2);
	return rot;


}



void Cheat::GameFunctions::SetRankRockstarGift(int RPValue)
{
	if (RPValue < 0 || RPValue > 8000) { notifyleft(xorstr_("Invalid Rank Inputted")); return; }

	int iVar0;
	STATS::STAT_GET_INT(GAMEPLAY::GET_HASH_KEY(xorstr_("mpply_last_mp_char")), &iVar0, -1);
	if (iVar0 == 0) 
	{ 
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY(xorstr_("MP0_CHAR_SET_RP_GIFT_ADMIN")), Cheat::GameArrays::RankPointsArray[RPValue - 1], 0);
	}
	else if (iVar0 == 1) 
	{
		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY(xorstr_("MP1_CHAR_SET_RP_GIFT_ADMIN")), Cheat::GameArrays::RankPointsArray[RPValue - 1], 0);
	}
	notifyleft("Join a new GTAO session for the new ranked to be applied");
}


void Cheat::GameFunctions::DrawVehicleLasers()
{
	int VehHandle = PED::GET_VEHICLE_PED_IS_IN(PlayerPedID, 0);
	float Offset1[] = { 0.6f, 0.6707f, 0.3711f };
	float Offset2[] = { -0.6f, 0.6707f, 0.3711f };
	float Offset3[] = { 0.6f, 25.0707f, 0.3711f };
	float Offset4[] = { -0.6f, 25.0707f, 0.3711f };
	Vector3 getcoords1 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(VehHandle, 0.6f, 0.6707f, 0.3711f);
	Vector3 getcoords2 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(VehHandle, -0.6f, 0.6707f, 0.3711f);
	Vector3 getcoords3 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(VehHandle, 0.6f, 25.0707f, 0.3711f);
	Vector3 getcoords4 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(VehHandle, -0.6f, 25.0707f, 0.3711f);
	float StartCoords1[] = { getcoords1.x, getcoords1.y, getcoords1.z };
	float StartCoords2[] = { getcoords2.x, getcoords2.y, getcoords2.z };
	float EndCoords1[] = { getcoords3.x, getcoords3.y, getcoords3.z };
	float EndCoords2[] = { getcoords4.x, getcoords4.y, getcoords4.z };
	GRAPHICS::DRAW_LINE(getcoords1.x, getcoords1.y, getcoords1.z, getcoords3.x, getcoords3.y, getcoords3.z, 255, 0, 0, 255);
	GRAPHICS::DRAW_LINE(getcoords2.x, getcoords2.y, getcoords2.z, getcoords4.x, getcoords4.y, getcoords4.z, 255, 0, 0, 255);
}


void Cheat::GameFunctions::RequestControl(Entity input)
{
	NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(input);

	int tick = 0;
	while (tick <= 50)
	{
		if (!NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(input))
		{
			WAIT(0);
		}
		else
		{
			return;
		}		
		NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(input);
		tick++;
	}
}


Vector3 Cheat::GameFunctions::RotToDirection(Vector3* rot) {
	float radiansZ = rot->z * 0.0174532924f;
	float radiansX = rot->x * 0.0174532924f;
	float num = abs((float)cos((double)radiansX));
	Vector3 dir;
	dir.x = (float)((double)((float)(-(float)sin((double)radiansZ))) * (double)num);
	dir.y = (float)((double)((float)cos((double)radiansZ)) * (double)num);
	dir.z = (float)sin((double)radiansX);
	return dir;
}

Vector3 Cheat::GameFunctions::AddTwoVectors(Vector3* vectorA, Vector3* vectorB) {
	Vector3 result;
	result.x = vectorA->x;
	result.y = vectorA->y;
	result.z = vectorA->z;
	result.x += vectorB->x;
	result.y += vectorB->y;
	result.z += vectorB->z;
	return result;
}

Vector3 Cheat::GameFunctions::MultiplyVector(Vector3* vector, float x) {
	Vector3 result;
	result.x = vector->x;
	result.y = vector->y;
	result.z = vector->z;
	result.x *= x;
	result.y *= x;
	result.z *= x;
	return result;
}

float Cheat::GameFunctions::GetDistanceBetweenTwoVectors(Vector3* pointA, Vector3* pointB) {
	float a_x = pointA->x;
	float a_y = pointA->y;
	float a_z = pointA->z;
	float b_x = pointB->x;
	float b_y = pointB->y;
	float b_z = pointB->z;
	double x_ba = (double)(b_x - a_x);
	double y_ba = (double)(b_y - a_y);
	double z_ba = (double)(b_z - a_z);
	double y_2 = y_ba * y_ba;
	double x_2 = x_ba * x_ba;
	double sum_2 = y_2 + x_2;
	return(float)sqrt(sum_2 + z_ba);
}

float Cheat::GameFunctions::GetVectorLength(Vector3* vector) {
	double x = (double)vector->x;
	double y = (double)vector->y;
	double z = (double)vector->z;
	return(float)sqrt(x * x + y * y + z * z);
}

void Cheat::GameFunctions::notifyBottom(char* Message, int ShowDuration)
{
	UI::BEGIN_TEXT_COMMAND_PRINT("STRING");
	UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(Message);
	UI::END_TEXT_COMMAND_PRINT(ShowDuration, 1);
}

Vector3 Cheat::GameFunctions::GetBlipMarker()
{
	static Vector3 zero;
	Vector3 coords;

	bool blipFound = false;
	int blipIterator = UI::_GET_BLIP_INFO_ID_ITERATOR();
	for (Blip i = UI::GET_FIRST_BLIP_INFO_ID(blipIterator); UI::DOES_BLIP_EXIST(i) != 0; i = UI::GET_NEXT_BLIP_INFO_ID(blipIterator))
	{
		if (UI::GET_BLIP_INFO_ID_TYPE(i) == 4)
		{
			coords = UI::GET_BLIP_INFO_ID_COORD(i);
			blipFound = true;
			break;
		}
	}
	if (blipFound)
	{
		return coords;
	}

	return zero;
}


float Cheat::GameFunctions::DegreesToRadians(float degs)
{
	return degs * 3.141592653589793f / 180.f;
}

Vector3 Cheat::GameFunctions::GetEntityCoords(Entity entity) {
	return ENTITY::GET_ENTITY_COORDS(entity, 1);
}

float Cheat::GameFunctions::GetDistanceBetweenTwoPoints(Vector3 A, Vector3 B) {
	return GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(A.x, A.y, A.z, B.x, B.y, B.z, 1);
}

int Cheat::GameFunctions::ReturnRandomInteger(int start, int end) {
	return GAMEPLAY::GET_RANDOM_INT_IN_RANGE(start, end);
}

void Cheat::GameFunctions::TeleportToCoords(Entity e, Vector3 coords)
{
	ENTITY::SET_ENTITY_COORDS_NO_OFFSET(e, coords.x, coords.y, coords.z, 0, 0, 1);
	WAIT(0);
}



void Cheat::GameFunctions::GetCameraDirection(float* dirX, float* dirY, float* dirZ)
{
	float tX, tZ, num;
	Vector3 rot = CAM::GET_GAMEPLAY_CAM_ROT(0);

	tZ = rot.z * 0.0174532924f;
	tX = rot.x * 0.0174532924f;
	num = abs(cos(tX));

	*dirX = (-sin(tZ)) * num;
	*dirY = (cos(tZ)) * num;
	*dirZ = sin(tX);
}


void Cheat::GameFunctions::RequestControlOfEnt(Entity entity)
{
	int tick = 0;
	while (!NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(entity) && tick <= 25)
	{
		NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(entity);
		tick++;
	}
	if (NETWORK::NETWORK_IS_SESSION_STARTED())
	{
		int netID = NETWORK::NETWORK_GET_NETWORK_ID_FROM_ENTITY(entity);
		Cheat::GameFunctions::RequestControlOfId(netID);
		NETWORK::SET_NETWORK_ID_CAN_MIGRATE(netID, 1);
	}
}

void Cheat::GameFunctions::RequestControlOfId(Entity netid)
{
	int tick = 0;

	while (!NETWORK::NETWORK_HAS_CONTROL_OF_NETWORK_ID(netid) && tick <= 25)
	{
		NETWORK::NETWORK_REQUEST_CONTROL_OF_NETWORK_ID(netid);
		tick++;
	}
}

bool Cheat::GameFunctions::RequestNetworkControl(uint vehID)
{
	int Tries = 0;
	bool
		hasControl = false,
		giveUp = false;
	do
	{
		hasControl = NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(vehID);
		Tries++;
		if (Tries > 300)
			giveUp = true;
	} while (!hasControl && !giveUp);

	if (giveUp)
		return false;
	else return true;
}

Ped Cheat::GameFunctions::CreatePed(char* PedName, Vector3 SpawnCoordinates, int ped_type, bool network_handle)
{
	Ped NewPed;
	int PedHash = GAMEPLAY::GET_HASH_KEY(PedName);
	if (STREAMING::IS_MODEL_IN_CDIMAGE(PedHash))
	{
		if (STREAMING::IS_MODEL_VALID(PedHash))
		{
			STREAMING::REQUEST_MODEL(PedHash);
			while (!STREAMING::HAS_MODEL_LOADED(PedHash)) WAIT(0);

			NewPed = PED::CREATE_PED(ped_type, PedHash, SpawnCoordinates.x, SpawnCoordinates.y, SpawnCoordinates.z, 0, network_handle, 1);
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(PedHash);
			return NewPed;
		}
	}

	return -1;
}

Ped Cheat::GameFunctions::ClonePed(Ped ped)
{
	if (ENTITY::DOES_ENTITY_EXIST(ped) && !ENTITY::IS_ENTITY_DEAD(ped))
	{
		return PED::CLONE_PED(ped, ENTITY::GET_ENTITY_HEADING(ped), 1, 1);
	}

	return 0;
}

void notifyleft(char* msg)
{
	UI::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
	UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(msg);
	UI::_DRAW_NOTIFICATION(false, false);
}

Vector3 TPCoords;
void Cheat::GameFunctions::TPto(Vector3 Coords)
{
	if (PED::IS_PED_IN_ANY_VEHICLE(PlayerPedID, 0))
	{
		ENTITY::SET_ENTITY_COORDS(PED::GET_VEHICLE_PED_IS_IN(PlayerPedID, false), Coords.x, Coords.y, Coords.z, 0, 0, 0, 1);
	}
	else
	{
		ENTITY::SET_ENTITY_COORDS(PlayerPedID, Coords.x, Coords.y, Coords.z, 0, 0, 0, 1);
	}
}

char* Cheat::GameFunctions::CharKeyboard(char* windowName, int maxInput, char* defaultText) {
	GAMEPLAY::DISPLAY_ONSCREEN_KEYBOARD(0, "", windowName, defaultText, "", "", "", maxInput);
	while (GAMEPLAY::UPDATE_ONSCREEN_KEYBOARD() == 0) WAIT(0);
	if (!GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT()) return "";
	return GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT();
}
int NumberKeyboard() {
	GAMEPLAY::DISPLAY_ONSCREEN_KEYBOARD(1, "", "", "", "", "", "", 10);
	while (GAMEPLAY::UPDATE_ONSCREEN_KEYBOARD() == 0) WAIT(0);
	if (!GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT()) return 0;
	return atof(GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT());
}

void Cheat::GameFunctions::ClearAllAnimations()
{
	AI::CLEAR_PED_TASKS_IMMEDIATELY(PlayerPedID);
}

void Cheat::GameFunctions::ClearNearbyPedAnimations()
{
	const int ElementAmount = 10;
	const int ArrSize = ElementAmount * 2 + 2;

	Ped* peds = new Ped[ArrSize];
	peds[0] = ElementAmount;

	int PedFound = PED::GET_PED_NEARBY_PEDS(PlayerPedID, peds, -1);

	for (int i = 0; i < PedFound; i++)
	{
		int OffsetID = i * 2 + 2;
		Cheat::GameFunctions::RequestControlOfEnt(peds[OffsetID]);
		if (ENTITY::DOES_ENTITY_EXIST(peds[OffsetID]) && PlayerPedID != peds[OffsetID])
		{
			AI::CLEAR_PED_TASKS_IMMEDIATELY(peds[OffsetID]);
		}
	}
}

void Cheat::GameFunctions::DoLocalPedAnimation(char* AnimationName, char* AnimationID)
{
	Cheat::GameFunctions::RequestNetworkControl(PlayerPedID);
	STREAMING::REQUEST_ANIM_DICT(AnimationName);
	if (STREAMING::HAS_ANIM_DICT_LOADED((AnimationName))) { AI::TASK_PLAY_ANIM(PlayerPedID, AnimationName, AnimationID, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0); }
}



void Cheat::GameFunctions::DoNearbyPedsAnimation(char* AnimationName, char* AnimationID)
{
	const int ElementAmount = 10;
	const int ArrSize = ElementAmount * 2 + 2;

	Ped* peds = new Ped[ArrSize];
	peds[0] = ElementAmount;

	int PedFound = PED::GET_PED_NEARBY_PEDS(PlayerPedID, peds, -1);

	for (int i = 0; i < PedFound; i++)
	{
		int OffsetID = i * 2 + 2;
		Cheat::GameFunctions::RequestControlOfEnt(peds[OffsetID]);
		if (ENTITY::DOES_ENTITY_EXIST(peds[OffsetID]) && PlayerPedID != peds[OffsetID])
		{
			Cheat::GameFunctions::RequestNetworkControl(peds[OffsetID]);
			STREAMING::REQUEST_ANIM_DICT(AnimationName);
			if (STREAMING::HAS_ANIM_DICT_LOADED((AnimationName)))
			{
				AI::TASK_PLAY_ANIM(peds[OffsetID], AnimationName, AnimationID, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);
			}
		}
	}
}


void PlayScenarioNearbyPeds(char* Scenario)
{
	const int ElementAmount = 10;
	const int ArrSize = ElementAmount * 2 + 2;

	Ped* peds = new Ped[ArrSize];
	peds[0] = ElementAmount;

	int PedFound = PED::GET_PED_NEARBY_PEDS(PlayerPedID, peds, -1);

	for (int i = 0; i < PedFound; i++)
	{
		int OffsetID = i * 2 + 2;
		Cheat::GameFunctions::RequestControlOfEnt(peds[OffsetID]);
		if (ENTITY::DOES_ENTITY_EXIST(peds[OffsetID]) && PlayerPedID != peds[OffsetID])
		{
			AI::CLEAR_PED_TASKS_IMMEDIATELY(peds[OffsetID]);
			AI::TASK_START_SCENARIO_IN_PLACE(peds[OffsetID], Scenario, 0, true);
		}
	}
}

bool ShowPlayerInformationPlayerList = true;
void Cheat::GameFunctions::LoadPlayerInformation(char* playerName, Player p) 
{
	if (ShowPlayerInformationPlayerList)
	{
		Ped ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(p);
		RequestControlOfEnt(ped);
		std::ostringstream PlayerMoney; PlayerMoney << xorstr_("Cash: $") << globalHandle(1590446).At(p, 871).At(211).At(3).As<int>();
		std::ostringstream PlayerRank; PlayerRank << xorstr_("Rank: ") << globalHandle(1590446).At(p, 871).At(211).At(6).As<int>();
		float health = ENTITY::GET_ENTITY_HEALTH(ped);
		float maxHealth = ENTITY::GET_ENTITY_MAX_HEALTH(ped);
		float healthPercent = health * 100 / maxHealth;
		std::ostringstream Health; Health << xorstr_("Health:~s~ ") << healthPercent;
		float armor = PED::GET_PED_ARMOUR(ped);
		float maxArmor = PLAYER::GET_PLAYER_MAX_ARMOUR(p);
		float armorPercent = armor * 100 / maxArmor;
		std::ostringstream Armor; Armor << xorstr_("Armor:~s~ ") << armorPercent;
		bool alive = !PED::IS_PED_DEAD_OR_DYING(ped, 1);
		char* aliveStatus;
		if (alive) aliveStatus = xorstr_("Yes"); else aliveStatus = xorstr_("No");
		std::ostringstream Alive; Alive << xorstr_("Alive:~s~ ") << aliveStatus;
		bool inVehicle = PED::IS_PED_IN_ANY_VEHICLE(ped, 0);
		std::ostringstream VehicleModel; VehicleModel << xorstr_("Vehicle:~s~ ");
		std::ostringstream Speed; Speed << xorstr_("Speed:~s~ ");
		std::ostringstream IsInAVehicle; IsInAVehicle << xorstr_("In Vehicle:~s~ ");
		if (inVehicle) 
		{
			IsInAVehicle << xorstr_("Yes");
			Hash vehHash = ENTITY::GET_ENTITY_MODEL(PED::GET_VEHICLE_PED_IS_IN(ped, 0));
			VehicleModel << UI::_GET_LABEL_TEXT(VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(vehHash));
			float vehSpeed = ENTITY::GET_ENTITY_SPEED(PED::GET_VEHICLE_PED_IS_IN(ped, 0));
			float vehSpeedConverted;
			vehSpeedConverted = round(vehSpeed * 1.6);
			Speed << vehSpeedConverted << xorstr_(" KM/H");
		}
		else 
		{
			IsInAVehicle << xorstr_("No");
			float speed = round(ENTITY::GET_ENTITY_SPEED(ped) * 100) / 100;
			Speed << speed << xorstr_(" M/S");
			VehicleModel << xorstr_("None");
		}
		std::ostringstream WantedLevel; WantedLevel << xorstr_("Wanted Level:~s~ ") << PLAYER::GET_PLAYER_WANTED_LEVEL(p);
		std::ostringstream Weapon; Weapon << xorstr_("Weapon: ~s~");
		Hash weaponHash;
		if (WEAPON::GET_CURRENT_PED_WEAPON(ped, &weaponHash, 1)) 
		{
			char* weaponName;
			if (weaponHash == 2725352035) {
				weaponName = xorstr_("Unarmed");
			}
			else if (weaponHash == 2578778090) {
				weaponName = xorstr_("Knife");
			}
			else if (weaponHash == 0x678B81B1) {
				weaponName = xorstr_("Nightstick");
			}
			else if (weaponHash == 0x4E875F73) {
				weaponName = xorstr_("Hammer");
			}
			else if (weaponHash == 0x958A4A8F) {
				weaponName = xorstr_("Bat");
			}
			else if (weaponHash == 0x440E4788) {
				weaponName = xorstr_("GolfClub");
			}
			else if (weaponHash == 0x84BD7BFD) {
				weaponName = xorstr_("Crowbar");
			}
			else if (weaponHash == 0x1B06D571) {
				weaponName = xorstr_("Pistol");
			}
			else if (weaponHash == 0x5EF9FEC4) {
				weaponName = xorstr_("Combat Pistol");
			}
			else if (weaponHash == 0x22D8FE39) {
				weaponName = xorstr_("AP Pistol");
			}
			else if (weaponHash == 0x99AEEB3B) {
				weaponName = xorstr_("Pistol 50");
			}
			else if (weaponHash == 0x13532244) {
				weaponName = xorstr_("Micro SMG");
			}
			else if (weaponHash == 0x2BE6766B) {
				weaponName = xorstr_("SMG");
			}
			else if (weaponHash == 0xEFE7E2DF) {
				weaponName = xorstr_("Assault SMG");
			}
			else if (weaponHash == 0xBFEFFF6D) {
				weaponName = xorstr_("Assault Riffle");
			}
			else if (weaponHash == 0x83BF0278) {
				weaponName = xorstr_("Carbine Riffle");
			}
			else if (weaponHash == 0xAF113F99) {
				weaponName = xorstr_("Advanced Riffle");
			}
			else if (weaponHash == 0x9D07F764) {
				weaponName = xorstr_("MG");
			}
			else if (weaponHash == 0x7FD62962) {
				weaponName = xorstr_("Combat MG");
			}
			else if (weaponHash == 0x1D073A89) {
				weaponName = xorstr_("Pump Shotgun");
			}
			else if (weaponHash == 0x7846A318) {
				weaponName = xorstr_("Sawed-Off Shotgun");
			}
			else if (weaponHash == 0xE284C527) {
				weaponName = xorstr_("Assault Shotgun");
			}
			else if (weaponHash == 0x9D61E50F) {
				weaponName = xorstr_("Bullpup Shotgun");
			}
			else if (weaponHash == 0x3656C8C1) {
				weaponName = xorstr_("Stun Gun");
			}
			else if (weaponHash == 0x05FC3C11) {
				weaponName = xorstr_("Sniper Rifle");
			}
			else if (weaponHash == 0x0C472FE2) {
				weaponName = xorstr_("Heavy Sniper");
			}
			else if (weaponHash == 0xA284510B) {
				weaponName = xorstr_("Grenade Launcher");
			}
			else if (weaponHash == 0x4DD2DC56) {
				weaponName = xorstr_("Smoke Grenade Launcher");
			}
			else if (weaponHash == 0xB1CA77B1) {
				weaponName = xorstr_("RPG");
			}
			else if (weaponHash == 0x42BF8A85) {
				weaponName = xorstr_("Minigun");
			}
			else if (weaponHash == 0x93E220BD) {
				weaponName = xorstr_("Grenade");
			}
			else if (weaponHash == 0x2C3731D9) {
				weaponName = xorstr_("Sticky Bomb");
			}
			else if (weaponHash == 0xFDBC8A50) {
				weaponName = xorstr_("Smoke Grenade");
			}
			else if (weaponHash == 0xA0973D5E) {
				weaponName = xorstr_("BZGas");
			}
			else if (weaponHash == 0x24B17070) {
				weaponName = xorstr_("Molotov");
			}
			else if (weaponHash == 0x060EC506) {
				weaponName = xorstr_("Fire Extinguisher");
			}
			else if (weaponHash == 0x34A67B97) {
				weaponName = xorstr_("Petrol Can");
			}
			else if (weaponHash == 0xFDBADCED) {
				weaponName = xorstr_("Digital scanner");
			}
			else if (weaponHash == 0x88C78EB7) {
				weaponName = xorstr_("Briefcase");
			}
			else if (weaponHash == 0x23C9F95C) {
				weaponName = xorstr_("Ball");
			}
			else if (weaponHash == 0x497FACC3) {
				weaponName = xorstr_("Flare");
			}
			else if (weaponHash == 0xF9E6AA4B) {
				weaponName = xorstr_("Bottle");
			}
			else if (weaponHash == 0x61012683) {
				weaponName = xorstr_("Gusenberg");
			}
			else if (weaponHash == 0xC0A3098D) {
				weaponName = xorstr_("Special Carabine");
			}
			else if (weaponHash == 0xD205520E) {
				weaponName = xorstr_("Heavy Pistol");
			}
			else if (weaponHash == 0xBFD21232) {
				weaponName = xorstr_("SNS Pistol");
			}
			else if (weaponHash == 0x7F229F94) {
				weaponName = xorstr_("Bullpup Rifle");
			}
			else if (weaponHash == 0x92A27487) {
				weaponName = xorstr_("Dagger");
			}
			else if (weaponHash == 0x083839C4) {
				weaponName = xorstr_("Vintage Pistol");
			}
			else if (weaponHash == 0x7F7497E5) {
				weaponName = xorstr_("Firework");
			}
			else if (weaponHash == 0xA89CB99E) {
				weaponName = xorstr_("Musket");
			}
			else if (weaponHash == 0x3AABBBAA) {
				weaponName = xorstr_("Heavy Shotgun");
			}
			else if (weaponHash == 0xC734385A) {
				weaponName = xorstr_("Marksman Rifle");
			}
			else if (weaponHash == 0x63AB0442) {
				weaponName = xorstr_("Homing Launcher");
			}
			else if (weaponHash == 0xAB564B93) {
				weaponName = xorstr_("Proximity Mine");
			}
			else if (weaponHash == 0x787F0BB) {
				weaponName = xorstr_("Snowball");
			}
			else if (weaponHash == 0x47757124) {
				weaponName = xorstr_("Flare Gun");
			}
			else if (weaponHash == 0xE232C28C) {
				weaponName = xorstr_("Garbage Bag");
			}
			else if (weaponHash == 0xD04C944D) {
				weaponName = xorstr_("Handcuffs");
			}
			else if (weaponHash == 0x0A3D4D34) {
				weaponName = xorstr_("Combat PDW");
			}
			else if (weaponHash == 0xDC4DB296) {
				weaponName = xorstr_("Marksman Pistol");
			}
			else if (weaponHash == 0xD8DF3C3C) {
				weaponName = xorstr_("Brass Knuckles");
			}
			else if (weaponHash == 0x6D544C99) {
				weaponName = xorstr_("Railgun");
			}
			else if (weaponHash == 0xBFE256D4) {
				weaponName = xorstr_("Pistol Mk II");
			}
			else if (weaponHash == 0x2BE6766B) {
				weaponName = xorstr_("SMG Mk II");
			}
			else if (weaponHash == 0x394F415C) {
				weaponName = xorstr_("Assault Rifle Mk II");
			}
			else if (weaponHash == 0xFAD1F1C9) {
				weaponName = xorstr_("Carbine Rifle Mk II");
			}
			else if (weaponHash == 0x969C3D67) {
				weaponName = xorstr_("Special Carbine Mk II");
			}
			else if (weaponHash == 0x84D6FAFD) {
				weaponName = xorstr_("Bullpup Rifle Mk II");
			}
			else if (weaponHash == 0xDBBD7280) {
				weaponName = xorstr_("Combat MG Mk II");
			}
			else if (weaponHash == 0x6A6C02E0) {
				weaponName = xorstr_("Marksman Rifle Mk II");
			}
			else if (weaponHash == 0xA914799) {
				weaponName = xorstr_("Heavy Sniper Mk II");
			}
			else if (weaponHash == 0x476BF155) {
				weaponName = xorstr_("Unholy Hellbringer");
			}
			else if (weaponHash == 0xAF3696A1) {
				weaponName = xorstr_("Up-n-Atomizer");
			}
			else if (weaponHash == 0xB62D1F67) {
				weaponName = xorstr_("Widowmaker");
			}
			else if (weaponHash == 0xBD248B55) {
				weaponName = xorstr_("Mini SMG");
			}
			else if (weaponHash == 0xCB96392F) {
				weaponName = xorstr_("Heavy Revolver Mk II");
			}
			else if (weaponHash == 0xC1B3C3D1) {
				weaponName = xorstr_("Heavy Revolver");
			}
			else if (weaponHash == 0x97EA20B8) {
				weaponName = xorstr_("Double Action Revolver");
			}
			else if (weaponHash == 0x2BE6766B) {
				weaponName = xorstr_("SMG Mk II");
			}
			else {
				weaponName = xorstr_("Unarmed");
			}
			Weapon << weaponName;
		}
		else
		{
			Weapon << xorstr_("Unarmed");
		}

		Vector3 myCoords = ENTITY::GET_ENTITY_COORDS(PlayerPedID, 1);
		Vector3 coords = ENTITY::GET_ENTITY_COORDS(ped, 1);
		std::ostringstream Zone; Zone << xorstr_("Zone: ~s~") << UI::_GET_LABEL_TEXT(ZONE::GET_NAME_OF_ZONE(coords.x, coords.y, coords.z));
		Hash streetName, crossingRoad;
		PATHFIND::GET_STREET_NAME_AT_COORD(coords.x, coords.y, coords.z, &streetName, &crossingRoad);
		std::ostringstream Street; Street << xorstr_("Street: ~s~") << UI::GET_STREET_NAME_FROM_HASH_KEY(streetName);
		float distance = Get3DDistance(coords, myCoords);
		std::ostringstream Distance; Distance << xorstr_("Distance: ~s~");

		if (distance > 1000) 
		{
			distance = round((distance / 1000) * 100) / 100;
			Distance << distance << xorstr_(" Kilometers");
		}
		else 
		{
			distance = round(distance * 1000) / 100;
			Distance << distance << xorstr_(" Meters");
		}

		char* PlayerNameString = Cheat::CheatFunctions::CombineTwoStrings(xorstr_("Playername: "), PLAYER::GET_PLAYER_NAME(p));
		Cheat::PlayerInfoBoxTitle(xorstr_("Player Information"));
		Cheat::PlayerInfoBoxText(PlayerNameString, 1);

		if (NETWORK::NETWORK_IS_SESSION_STARTED()) 
		{
			Cheat::PlayerInfoBoxText((char*)PlayerRank.str().c_str(), 2);
			Cheat::PlayerInfoBoxText((char*)PlayerMoney.str().c_str(), 3);
		}
		else
		{
			Cheat::PlayerInfoBoxText(xorstr_("Rank: GTA Online Only"), 2);
			Cheat::PlayerInfoBoxText(xorstr_("Cash: GTA Online Only"), 3);
		}

		Cheat::PlayerInfoBoxText((char*)Health.str().c_str(), 4);
		Cheat::PlayerInfoBoxText((char*)Armor.str().c_str(), 5);
		Cheat::PlayerInfoBoxText((char*)Alive.str().c_str(), 6);
		Cheat::PlayerInfoBoxText((char*)IsInAVehicle.str().c_str(), 7);
		Cheat::PlayerInfoBoxText((char*)VehicleModel.str().c_str(), 8);
		Cheat::PlayerInfoBoxText((char*)Speed.str().c_str(), 9);
		Cheat::PlayerInfoBoxText((char*)WantedLevel.str().c_str(), 10);
		Cheat::PlayerInfoBoxText((char*)Weapon.str().c_str(), 11);
		Cheat::PlayerInfoBoxText((char*)Zone.str().c_str(), 12);
		Cheat::PlayerInfoBoxText((char*)Street.str().c_str(), 13);
		if (playerName != PLAYER::GET_PLAYER_NAME(PlayerID)) { Cheat::PlayerInfoBoxText((char*)Distance.str().c_str(), 14); }
	}
}


void Cheat::GameFunctions::MaxUpgradeVehicle(int VehicleHandle)
{
	Vehicle vehicle = PED::GET_VEHICLE_PED_IS_IN(PlayerPedID, false);
	VEHICLE::SET_VEHICLE_MOD_KIT(vehicle, 0);
	for (int i = 0; i < 50; i++)
	{
		VEHICLE::SET_VEHICLE_MOD(vehicle, i, VEHICLE::GET_NUM_VEHICLE_MODS(vehicle, i) - 1, false);
	}
	VEHICLE::TOGGLE_VEHICLE_MOD(vehicle, 18, true); // Set turbo on vehicle
	VEHICLE::TOGGLE_VEHICLE_MOD(vehicle, 22, true); // Enable xeon headlights
}

void Cheat::GameFunctions::MaxDowngradeVehicle(int VehicleHandle)
{
	Vehicle vehicle = PED::GET_VEHICLE_PED_IS_IN(PlayerPedID, false);
	VEHICLE::SET_VEHICLE_MOD_KIT(vehicle, 0);
	for (int i = 0; i < 50; i++)
	{
		VEHICLE::REMOVE_VEHICLE_MOD(vehicle, i);
	}
	VEHICLE::REMOVE_VEHICLE_MOD(vehicle, 18); // Remove turbo on vehicle
	VEHICLE::REMOVE_VEHICLE_MOD(vehicle, 22); // Remove xeon headlights
}

float Cheat::GameFunctions::Get3DDistance(Vector3 a, Vector3 b) {
	float x = pow((a.x - b.x), 2);
	float y = pow((a.y - b.y), 2);
	float z = pow((a.z - b.z), 2);
	return sqrt(x + y + z);
}


void Cheat::GameFunctions::ApplyForceToEntity(Entity e, float x, float y, float z)
{
	if (e != PlayerPedID && NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(e) == false) { RequestControlOfEnt(e); }
	ENTITY::APPLY_FORCE_TO_ENTITY(e, 1, x, y, z, 0, 0, 0, 0, 1, 1, 1, 0, 1);
}


void Cheat::GameFunctions::DetachObjectFromPed(Ped Ped, char* ObjectName)
{
	Vector3 PedCoords = ENTITY::GET_ENTITY_COORDS(Ped, true);
	Object Object = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(PedCoords.x, PedCoords.y, PedCoords.z, 4.0, GAMEPLAY::GET_HASH_KEY(ObjectName), false, false, true);
	if (ENTITY::DOES_ENTITY_EXIST(Object) && ENTITY::IS_ENTITY_ATTACHED_TO_ENTITY(Object, Ped))
	{
		Cheat::GameFunctions::RequestControlOfEnt(Object);
		int netID = NETWORK::NETWORK_GET_NETWORK_ID_FROM_ENTITY(Object);
		NETWORK::SET_NETWORK_ID_CAN_MIGRATE(netID, 1);
		Cheat::GameFunctions::RequestControlOfId(netID);
		ENTITY::DETACH_ENTITY(Object, 1, 1);
		ENTITY::SET_ENTITY_AS_MISSION_ENTITY(Object, 1, 1);
		ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&Object);
		ENTITY::DELETE_ENTITY(&Object);
	}
}

void Cheat::GameFunctions::AttachObjectToPed(Ped Ped, char* ObjectName)
{
	int attachobj[100];
	int nuattach = 1;
	Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(Ped), true);
	int hash = GAMEPLAY::GET_HASH_KEY(ObjectName);
	if (STREAMING::IS_MODEL_IN_CDIMAGE(hash))
	{
		if (STREAMING::IS_MODEL_VALID(hash))
		{
			STREAMING::REQUEST_MODEL(hash);
			while (!STREAMING::HAS_MODEL_LOADED(hash)) WAIT(0);

			if (STREAMING::HAS_MODEL_LOADED(hash))
			{
				attachobj[nuattach] = OBJECT::CREATE_OBJECT(hash, pos.x, pos.y, pos.z, 1, 1, 1);
				if (ENTITY::DOES_ENTITY_EXIST(attachobj[nuattach]))
				{
					ENTITY::ATTACH_ENTITY_TO_ENTITY(attachobj[nuattach], PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(Ped), 31086, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 2, 1);
					nuattach++;
					if (nuattach >= 101) { nuattach = 1; }
				}
			}
		}
	}
}


void Cheat::GameFunctions::NearbyPedsCommitSuicide()
{
	bool setUpAnim = true;
	const int numElements = 10;
	const int arrSize = numElements * 2 + 2;
	Ped ped[arrSize];
	ped[0] = numElements;
	int count = PED::GET_PED_NEARBY_PEDS(PlayerPedID, ped, 1);
	if (ped != NULL)
	{
		for (int i = 1; i <= count; i++)
		{
			int offsettedID = i;
			if (ped[offsettedID] != NULL && ENTITY::DOES_ENTITY_EXIST(ped[offsettedID]))
			{
				if (setUpAnim)
				{
					WEAPON::GIVE_DELAYED_WEAPON_TO_PED(ped[offsettedID], 0x1B06D571, 99999, true);
					WEAPON::SET_CURRENT_PED_WEAPON(ped[offsettedID], 0x1B06D571, true);
					char* dict = xorstr_("MP_SUICIDE");
					char* anim = xorstr_("pistol");
					STREAMING::REQUEST_ANIM_DICT(dict);
					STREAMING::REQUEST_ANIM_SET(anim);
					AI::TASK_PLAY_ANIM(ped[offsettedID], dict, anim, 8, -8, -1, 2105344, 0, false, 0, false);
					setUpAnim = false;
				}
				if (ENTITY::HAS_ANIM_EVENT_FIRED(ped[offsettedID], GAMEPLAY::GET_HASH_KEY(xorstr_("Fire"))))
				{
					PED::SET_PED_SHOOTS_AT_COORD(ped[offsettedID], 0, 0, 0, TRUE);
				}
			}
		}
	}
}


void Cheat::GameFunctions::DrawMarkerAbovePlayer(int Type, Player player, RGBA Color)
{
	Vector3 coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), false);
	GRAPHICS::DRAW_MARKER(Type, coords.x, coords.y, coords.z + 1.3f, 0, 0, 0, 0, 180, 0, 0.3, 0.3, 0.3, Color.r, Color.g, Color.b, Color.a, 1, 1, 1, 0, 0, 0, 0);
}

bool spawninvehicle = false;
bool spawner_deletecurrentvehicle = false;
bool spawnvehiclewithgodmode = false;
bool spawnmaxupgraded = false;
void Cheat::GameFunctions::SpawnVehicle(char* ModelHash)
{
	Hash model = GAMEPLAY::GET_HASH_KEY(ModelHash);
	if (!STREAMING::IS_MODEL_IN_CDIMAGE(model) || !STREAMING::IS_MODEL_A_VEHICLE(model)) { notifyleft(xorstr_("~r~That is not a valid vehicle model")); return; }
	if (spawner_deletecurrentvehicle) {
		if (PED::IS_PED_IN_ANY_VEHICLE(PlayerPedID, 0)) {
			auto veh = PED::GET_VEHICLE_PED_IS_USING(PlayerPedID);
			if (ENTITY::DOES_ENTITY_EXIST(veh))
			{
				ENTITY::SET_ENTITY_AS_MISSION_ENTITY(veh, 1, 1);
				VEHICLE::DELETE_VEHICLE(&veh);
			}
		}
	}
	STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY(ModelHash));
	while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY(ModelHash))) WAIT(0);
	Vector3 pos = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PlayerPedID, 0.0, 5.0, 0);
	auto veh = VEHICLE::CREATE_VEHICLE(GAMEPLAY::GET_HASH_KEY(ModelHash), pos.x, pos.y, pos.z, ENTITY::GET_ENTITY_HEADING(PlayerPedID), 1, 1);
	if (veh != 0)
	{
		NETWORK::NETWORK_FADE_OUT_ENTITY(veh, false, true);
		if (spawninvehicle) { PED::SET_PED_INTO_VEHICLE(PlayerPedID, veh, -1); }
		if (spawnvehiclewithgodmode)
		{
			ENTITY::SET_ENTITY_INVINCIBLE(veh, TRUE);
			ENTITY::SET_ENTITY_PROOFS(veh, 0, 0, 0, 0, 0, 0, 0, 0);
			VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(veh, 0);
			VEHICLE::SET_VEHICLE_WHEELS_CAN_BREAK(veh, 0);
			VEHICLE::SET_VEHICLE_CAN_BE_VISIBLY_DAMAGED(veh, 0);
		}
		if (spawnmaxupgraded) { MaxUpgradeVehicle(veh); }
		VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, "Vehicle");
		VEHICLE::SET_VEHICLE_ENGINE_ON(veh, true, true, true);
		VEHICLE::SET_VEHICLE_IS_STOLEN(veh, false);
		VEHICLE::SET_VEHICLE_IS_CONSIDERED_BY_PLAYER(veh, true);
		VEHICLE::SET_VEHICLE_IS_WANTED(veh, false); 
		ENTITY::SET_ENTITY_AS_MISSION_ENTITY(veh, 1, 1);
		NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(NETWORK::NET_TO_VEH(veh), 1);
		DECORATOR::DECOR_SET_INT(veh, xorstr_("MPBitset"), 0);
		ENTITY::_SET_ENTITY_SOMETHING(veh, true);
		notifyleft(xorstr_("Vehicle Spawned"));
	}
}

void Cheat::GameFunctions::TeleportToWaypoint()
{
	if (!UI::IS_WAYPOINT_ACTIVE()) { notifyleft(xorstr_("~r~Please set waypoint")); return; }
	Vector3 coords = Cheat::GameFunctions::GetBlipMarker();

	Entity e = PlayerPedID;
	if (PED::IS_PED_IN_ANY_VEHICLE(e, 0)) { e = PED::GET_VEHICLE_PED_IS_USING(e); }

	bool groundFound = false;
	static float groundCheckHeight[] = { 100.0, 150.0, 50.0, 0.0, 200.0, 250.0, 300.0, 350.0, 400.0, 450.0, 500.0, 550.0, 600.0, 650.0, 700.0, 750.0, 800.0 };
	for (int i = 0; i < sizeof(groundCheckHeight) / sizeof(float); i++)
	{
		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(e, coords.x, coords.y, groundCheckHeight[i], 0, 0, 1);
		WAIT(100);
		if (GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(coords.x, coords.y, groundCheckHeight[i], &coords.z, 0))
		{
			groundFound = true;
			coords.z += 3.0;
			break;
		}
	}
	if (!groundFound) { coords.z = 1000.0; WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PlayerPedID, 0xFBAB5776, 1, 0); }
	Cheat::GameFunctions::TeleportToCoords(e, coords);
}

Cam antiCrashCam;
void Cheat::GameFunctions::EnableDisableAntiCrashCamera()
{
	if (CAM::DOES_CAM_EXIST(antiCrashCam))
	{
		CAM::SET_CAM_ACTIVE(antiCrashCam, false);
		CAM::RENDER_SCRIPT_CAMS(0, 1, 10, 0, 0);
		CAM::DESTROY_CAM(antiCrashCam, false);
		PLAYER::SET_PLAYER_CONTROL(PlayerID, true, 0);
	}
	else
	{
		antiCrashCam = CAM::CREATE_CAM_WITH_PARAMS(xorstr_("DEFAULT_SCRIPTED_CAMERA"), 9999.0f, 9999.0f, 9999.0f, 9999.0f, 9999.0f, 9999.0f, 9999.0f, true, 1);
		CAM::RENDER_SCRIPT_CAMS(1, 1, 1, 0, 0);
		CAM::SET_CAM_ACTIVE(antiCrashCam, true);
		PLAYER::SET_PLAYER_CONTROL(PlayerID, false, 0);
	}
}


std::vector<std::string> CurrentPlayerNamesSession;
std::vector<std::string> CurrentPlayerNamesSession2;
bool SecondCall = false;
bool show_joining_players_notification = false;
void Cheat::GameFunctions::CheckNewSessionMembersLoop()
{
	if (NETWORK::NETWORK_IS_SESSION_STARTED() && show_joining_players_notification)
	{
		if (!SecondCall) { CurrentPlayerNamesSession.clear(); CurrentPlayerNamesSession2.clear(); }

		for (int i = 0; i < 32; ++i)
		{
			char* PlayernameString = PLAYER::GET_PLAYER_NAME(i);
			if (ENTITY::DOES_ENTITY_EXIST(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i)))
			{
				if (!SecondCall) 
				{ 
					CurrentPlayerNamesSession.push_back(PlayernameString); 
				}
				else
				{
					CurrentPlayerNamesSession2.push_back(PlayernameString);
				}
			}
		}

		if (SecondCall)
		{
			for (const auto& var : CurrentPlayerNamesSession2)
			{
				if (!std::count(CurrentPlayerNamesSession.begin(), CurrentPlayerNamesSession.end(), var))
				{
					std::string NewPlayerString = xorstr_("<C>") + var + xorstr_("</C> joined the session.");
					notifyleft((char*)NewPlayerString.c_str());
				}
			}
			SecondCall = false;
		}
		else
		{
			SecondCall = true;
		}
	}
}


bool Cheat::GameFunctions::IsEntityInInterior(Entity Entity)
{
	if (INTERIOR::GET_INTERIOR_FROM_ENTITY(Entity) == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

int VAR_INSTRUCTIONAL_CONTAINER;
int VAR_INSTRUCTIONAL_COUNT;
void Cheat::GameFunctions::InstructionsInit() 
{
	VAR_INSTRUCTIONAL_COUNT = 0;
	VAR_INSTRUCTIONAL_CONTAINER = GRAPHICS::REQUEST_SCALEFORM_MOVIE("instructional_buttons");
	GRAPHICS::DRAW_SCALEFORM_MOVIE_FULLSCREEN(VAR_INSTRUCTIONAL_CONTAINER, 255, 255, 255, 0, 0);
	GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(VAR_INSTRUCTIONAL_CONTAINER, "CLEAR_ALL");
	GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
	GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(VAR_INSTRUCTIONAL_CONTAINER, "SET_CLEAR_SPACE");
	GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_INT(200);
	GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
}
void Cheat::GameFunctions::InstructionsAdd(char* text, int button) 
{
	GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(VAR_INSTRUCTIONAL_CONTAINER, "SET_DATA_SLOT");
	GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_INT(VAR_INSTRUCTIONAL_COUNT);
	GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_INT(button);
	GRAPHICS::BEGIN_TEXT_COMMAND_SCALEFORM_STRING("STRING");
	UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
	GRAPHICS::END_TEXT_COMMAND_SCALEFORM_STRING();
	GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
	VAR_INSTRUCTIONAL_COUNT++;
}
void Cheat::GameFunctions::InstructionsEnd() 
{
	GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(VAR_INSTRUCTIONAL_CONTAINER, "DRAW_INSTRUCTIONAL_BUTTONS");
	GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
	GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(VAR_INSTRUCTIONAL_CONTAINER, "SET_BACKGROUND_COLOUR");
	GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_INT(0);
	GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_INT(0);
	GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_INT(0);
	GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_INT(80);
	GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
}

void Cheat::GameFunctions::SetSessionTime(int h, int m, int s) {
	Hooking::ClockTime->hour = h;
	Hooking::ClockTime->minute = h;
	Hooking::ClockTime->second = s;
	Hooking::set_session_time_info(4, 0);
}