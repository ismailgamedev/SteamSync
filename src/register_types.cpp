#include "register_types.h"
#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>
#include "NetworkManager.h"
#include "godot_cpp/classes/engine.hpp"
#include "godotsteam.h"
#include "command.h"
#include <godot_cpp/classes/engine.hpp>
#include "p2p.h"
#include "transform_2d_sync.h"
#include "func_sync.h"

using namespace godot;

static Steam *SteamPtr;
void initialize_gdextension_types(ModuleInitializationLevel p_level)
{
	if(p_level == MODULE_INITIALIZATION_LEVEL_SCENE){
		ClassDB::register_class<Steam>();
		SteamPtr = memnew(Steam);
		Engine::get_singleton()->register_singleton("Steam", Steam::get_singleton());
	}
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
		GDREGISTER_CLASS(AP2P);
		GDREGISTER_CLASS(ANetworkManager);
		GDREGISTER_CLASS(ACommand);
		GDREGISTER_CLASS(ATransformSync2D);
		GDREGISTER_CLASS(AFuncSync);


	}
	


}

void uninitialize_gdextension_types(ModuleInitializationLevel p_level) {
	if(p_level == MODULE_INITIALIZATION_LEVEL_SCENE){
		Engine::get_singleton()->unregister_singleton("Steam");
		memdelete(SteamPtr);
	}
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
		
	}


}

extern "C"
{
	// Initialization
	GDExtensionBool GDE_EXPORT steamsync_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization)
	{
		GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);
		init_obj.register_initializer(initialize_gdextension_types);
		init_obj.register_terminator(uninitialize_gdextension_types);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

		return init_obj.init();
	}
}