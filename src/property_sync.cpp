#include "property_sync.h"
#include "p2p.h"


APropertySync::APropertySync() {
    
}

APropertySync::~APropertySync() {    
}

void APropertySync::init_timer() {
   
    timer = memnew(Timer);
    timer->set_timer_process_callback(Timer::TIMER_PROCESS_PHYSICS);
    timer->set_wait_time(0.1);
    add_child(timer);
    timer->set_autostart(true);
    timer->start();
    



}

void APropertySync::_ready() {
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    NETWORK_MANAGER = get_node<ANetworkManager>(NodePath("/root/NetworkManager"));
    P2P = get_node<AP2P>(NodePath("/root/P2P"));


}

void APropertySync::_process(double delta) {
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }


}


void APropertySync::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_is_only_lobby_owner", "_is_only_lobby_owner"), &APropertySync::set_is_only_lobby_owner);	
    ClassDB::bind_method(D_METHOD("set_object_player", "_object_player"), &APropertySync::set_object_player);	
    ClassDB::bind_method(D_METHOD("get_is_only_lobby_owner"), &APropertySync::get_is_only_lobby_owner);	
    ClassDB::bind_method(D_METHOD("get_object_player"), &APropertySync::get_object_player);
    ClassDB::bind_method(D_METHOD("get_object_player"), &APropertySync::get_object_player);	
    ClassDB::bind_method(D_METHOD("set_properties", "_properties"), &APropertySync::set_properties);
    ClassDB::bind_method(D_METHOD("get_properties"), &APropertySync::get_properties);
    ClassDB::bind_method(D_METHOD("set_is_interpolated", "_is_interpolated"), &APropertySync::set_is_interpolated);
    ClassDB::bind_method(D_METHOD("get_is_interpolated"), &APropertySync::get_is_interpolated);
    ClassDB::bind_method(D_METHOD("set_interpolation_value", "_interpolation_value"), &APropertySync::set_interpolation_value);
    ClassDB::bind_method(D_METHOD("get_interpolation_value"), &APropertySync::get_interpolation_value);
    

    ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "PROPERTIES"), "set_properties", "get_properties");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "IS_INTERPOLATED"), "set_is_interpolated", "get_is_interpolated");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "INTERPOLATION_VALUE"), "set_interpolation_value", "get_interpolation_value");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "IS_ONLY_LOBBY_OWNER"), "set_is_only_lobby_owner", "get_is_only_lobby_owner");
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "OBJECT_PLAYER"), "set_object_player", "get_object_player");

}

void APropertySync::set_is_only_lobby_owner(bool _is_only_lobby_owner) {
    is_only_lobby_owner = _is_only_lobby_owner;
}

bool APropertySync::get_is_only_lobby_owner() {
    return is_only_lobby_owner;
}

void APropertySync::set_object_player(NodePath _object_player) {
    object_player = _object_player;
}

NodePath APropertySync::get_object_player() {
    return object_player;
}
void APropertySync::set_properties(PackedStringArray _properties) {
    properties = _properties;
}
PackedStringArray APropertySync::get_properties() {
    return properties;
}

void APropertySync::set_is_interpolated(bool _is_interpolated) {
    is_interpolated = _is_interpolated;
}

bool APropertySync::get_is_interpolated() {    
    return is_interpolated;
}

void APropertySync::set_interpolation_value(double _interpolation_value) {
    interpolation_value = _interpolation_value;
}

double APropertySync::get_interpolation_value() {
    return interpolation_value;
}