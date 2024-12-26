#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include "godot_cpp/classes/engine.hpp"
#include "godotsteam.h"
#include "godot_cpp/variant/string.hpp"
#include "godot_cpp/variant/dictionary.hpp"
#include "godot_cpp/classes/scene_tree.hpp"
#include "godot_cpp/variant/array.hpp"
#include "godot_cpp/variant/variant.hpp"
using namespace godot;

class ANetworkManager : public Node {
	GDCLASS(ANetworkManager, Node)
public:

	ANetworkManager();
	~ANetworkManager();
	enum SEND_TYPE {
		START = 0,
		READY = 1,
		START_SCENE = 2,
		TRANFORM_SYNC = 3,
		PROPERTY = 4,
		EVENT = 5,
		RIGIDBODY_SYNC = 6,
		SCENE_LOADED = 7,
		COMMAND = 8,
		VOICE = 9,
		RAGDOLL = 10
	};

	/// EXPORT VARIABLES
	uint32_t APP_ID;

	/// INTERNAL VARIABLES
	bool IS_ON_STEAM;
	bool IS_ON_STEAM_DECK;
	bool IS_ONLINE;
	bool IS_OWNED;
	uint64_t STEAM_ID;
	String STEAM_USERNAME;
	uint64_t LOBBY_ID;
	TypedArray<Dictionary> LOBBY_MEMBERS;

	
	uint64_t get_steam_id();
	void set_steam_id(uint64_t _steam_id);

	String get_steam_username();
	void set_steam_username(String _steam_username);

	uint64_t get_lobby_id();
	void set_lobby_id(uint64_t _lobby_id);

	TypedArray<Dictionary> get_lobby_members();
	void set_lobby_members(TypedArray<Dictionary>  _lobby_members);

	uint32_t get_app_id();
	void set_app_id(uint32_t _app_id);

	void  _ready() override;
	void _process(double delta) override;
protected:
	static void _bind_methods();
private:

};


VARIANT_ENUM_CAST(ANetworkManager::SEND_TYPE);


#endif