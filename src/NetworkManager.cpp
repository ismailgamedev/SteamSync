#include "NetworkManager.h"

using namespace godot;


static Steam *SteamPtr = nullptr; 
void ANetworkManager::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_app_id"), &ANetworkManager::get_app_id);
    ClassDB::bind_method(D_METHOD("get_steam_id"), &ANetworkManager::get_steam_id);
    ClassDB::bind_method(D_METHOD("get_steam_username"), &ANetworkManager::get_steam_username);
    ClassDB::bind_method(D_METHOD("get_lobby_id"), &ANetworkManager::get_lobby_id);
    ClassDB::bind_method(D_METHOD("get_lobby_members"), &ANetworkManager::get_lobby_members);


    ClassDB::bind_method(D_METHOD("set_app_id", "_app_id"), &ANetworkManager::set_app_id);
    ClassDB::bind_method(D_METHOD("set_lobby_id", "_lobby_id"), &ANetworkManager::set_lobby_id);
    ClassDB::bind_method(D_METHOD("set_lobby_members", "_lobby_members"), &ANetworkManager::set_lobby_members);
    ClassDB::bind_method(D_METHOD("set_steam_username", "_steam_username"), &ANetworkManager::set_steam_username);
    ClassDB::bind_method(D_METHOD("set_steam_id", "_steam_id"), &ANetworkManager::set_steam_id);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "APP_ID"), "set_app_id", "get_app_id");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "LOBBY_ID"), "set_lobby_id", "get_lobby_id");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "STEAM_ID"), "set_steam_id", "get_steam_id");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "STEAM_USERNAME"), "set_steam_username", "get_steam_username");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "LOBBY_MEMBERS",PROPERTY_HINT_INT_IS_POINTER ), "set_lobby_members", "get_lobby_members");
    

}

ANetworkManager::ANetworkManager() {
    APP_ID = 480;
    IS_ON_STEAM = false;
	IS_ON_STEAM_DECK = false;
	IS_ONLINE = false;
	IS_OWNED = false;
	STEAM_ID = 0;
	STEAM_USERNAME = "";
	LOBBY_ID = 0;
}



ANetworkManager::~ANetworkManager() {
}
void ANetworkManager:: _ready() {

    SteamPtr = Object::cast_to<Steam>(Engine::get_singleton()->get_singleton("Steam"));
    
    if (Engine::get_singleton()->has_singleton("Steam"))
    {
        Dictionary initialize = SteamPtr->steamInitEx(true,APP_ID);
 
        if (int(initialize["status"]) != 0)
        {
            UtilityFunctions::printerr("Steamworks failed to initialize. Error: ", initialize["verbal"]);
            get_tree()->quit();
        }
        IS_ON_STEAM = true;
        IS_ON_STEAM_DECK = SteamPtr->isSteamRunningOnSteamDeck();
        IS_ONLINE = SteamPtr->loggedOn();
        IS_OWNED = SteamPtr->isSubscribed();
        STEAM_ID = SteamPtr->getSteamID();
        STEAM_USERNAME = SteamPtr->getPersonaName();

        if (!IS_OWNED)
        {
            UtilityFunctions::printerr("[STEAM] User does not own this game");
            get_tree()->quit();
        }
    }

    
}

void ANetworkManager::_process(double delta) {
    if (IS_ON_STEAM)
    {
        SteamPtr->_run_callbacks();
    }
    
}


uint32_t ANetworkManager::get_app_id() {
    return APP_ID;
}

void ANetworkManager::set_app_id(uint32_t _app_id) {
    APP_ID = _app_id;
}

uint64_t ANetworkManager::get_steam_id() {
    return STEAM_ID;
}

void ANetworkManager::set_steam_id(uint64_t _steam_id) {
    STEAM_ID = _steam_id;
}

String ANetworkManager::get_steam_username() {
    return STEAM_USERNAME;
}

void ANetworkManager::set_steam_username(String _steam_username) {
    STEAM_USERNAME = _steam_username;    
}

uint64_t ANetworkManager::get_lobby_id() { 
    return LOBBY_ID;
}

void ANetworkManager::set_lobby_id(uint64_t _lobby_id) {
    LOBBY_ID = _lobby_id;
}

TypedArray<uint64_t> ANetworkManager::get_lobby_members() {
    return LOBBY_MEMBERS;
}

void ANetworkManager::set_lobby_members(TypedArray<uint64_t> _lobby_members) {
    LOBBY_MEMBERS = _lobby_members;
}