#include "NetworkManager.h"

using namespace godot;


static Steam *SteamPtr = nullptr; 
void ANetworkManager::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_app_id"), &ANetworkManager::get_app_id);
    ClassDB::bind_method(D_METHOD("get_steam_id"), &ANetworkManager::get_steam_id);
    ClassDB::bind_method(D_METHOD("get_steam_username"), &ANetworkManager::get_steam_username);
    ClassDB::bind_method(D_METHOD("get_lobby_id"), &ANetworkManager::get_lobby_id);
    ClassDB::bind_method(D_METHOD("get_lobby_members"), &ANetworkManager::get_lobby_members);
    ClassDB::bind_method(D_METHOD("get_members_data"), &ANetworkManager::get_members_data);
    ClassDB::bind_method(D_METHOD("get_game_started"), &ANetworkManager::get_game_started);
    ClassDB::bind_method(D_METHOD("get_player_node"), &ANetworkManager::get_player_node);

    ClassDB::bind_method(D_METHOD("set_player_node", "_player_node"), &ANetworkManager::set_player_node);
    ClassDB::bind_method(D_METHOD("set_members_data", "_members_data"), &ANetworkManager::set_members_data);
    ClassDB::bind_method(D_METHOD("set_game_started", "_game_started"), &ANetworkManager::set_game_started);
    ClassDB::bind_method(D_METHOD("set_app_id", "_app_id"), &ANetworkManager::set_app_id);
    ClassDB::bind_method(D_METHOD("set_lobby_id", "_lobby_id"), &ANetworkManager::set_lobby_id);
    ClassDB::bind_method(D_METHOD("set_lobby_members", "_lobby_members"), &ANetworkManager::set_lobby_members);
    ClassDB::bind_method(D_METHOD("set_steam_username", "_steam_username"), &ANetworkManager::set_steam_username);
    ClassDB::bind_method(D_METHOD("set_steam_id", "_steam_id"), &ANetworkManager::set_steam_id);

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "PLAYER_NODE",PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"), "set_player_node", "get_player_node");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "GAME_STARTED"), "set_game_started", "get_game_started");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "APP_ID"), "set_app_id", "get_app_id");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "LOBBY_ID"), "set_lobby_id", "get_lobby_id");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "STEAM_ID"), "set_steam_id", "get_steam_id");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "STEAM_USERNAME"), "set_steam_username", "get_steam_username");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "LOBBY_MEMBERS"), "set_lobby_members", "get_lobby_members");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "MEMBERS_DATA"), "set_members_data", "get_members_data");
    BIND_ENUM_CONSTANT(START);
    BIND_ENUM_CONSTANT(READY);
    BIND_ENUM_CONSTANT(START_SCENE);
    BIND_ENUM_CONSTANT(TRANFORM_SYNC);
    BIND_ENUM_CONSTANT(PROPERTY);
    BIND_ENUM_CONSTANT(EVENT);
    BIND_ENUM_CONSTANT(RIGIDBODY_SYNC);
    BIND_ENUM_CONSTANT(SCENE_LOADED);
    BIND_ENUM_CONSTANT(COMMAND);
    BIND_ENUM_CONSTANT(VOICE);
    BIND_ENUM_CONSTANT(RAGDOLL);
    BIND_ENUM_CONSTANT(HANDSHAKE);

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
    GAME_STARTED = false;
}



ANetworkManager::~ANetworkManager() {
}
void ANetworkManager:: _ready() {
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }

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
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }
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

TypedArray<Dictionary> ANetworkManager::get_lobby_members() {
    return LOBBY_MEMBERS;
}

void ANetworkManager::set_lobby_members(TypedArray<Dictionary> _lobby_members) {
    LOBBY_MEMBERS = _lobby_members;
}
void ANetworkManager::set_members_data(TypedArray<Dictionary> _members_data) {
    MEMBERS_DATA = _members_data;
}

TypedArray<Dictionary> ANetworkManager::get_members_data() {
    return MEMBERS_DATA;    
}

bool ANetworkManager::get_game_started() {
    return GAME_STARTED;
}

void ANetworkManager::set_game_started(bool _game_started) {
    GAME_STARTED = _game_started;    
}
void ANetworkManager::set_player_node(Ref<PackedScene> _player_node) {
    PLAYER_NODE = _player_node;
}

Ref<PackedScene> ANetworkManager::get_player_node() {
    return PLAYER_NODE;
}