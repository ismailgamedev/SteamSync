#include "command.h"

void ACommand::_bind_methods() {
    ClassDB::bind_method(D_METHOD("send"), &ACommand::send);
    ClassDB::bind_method(D_METHOD("set_loading_screen", "loading_screen"), &ACommand::set_loading_screen);
    ClassDB::bind_method(D_METHOD("get_loading_screen"), &ACommand::get_loading_screen);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "loading_screen",PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"), "set_loading_screen", "get_loading_screen");
}

ACommand::ACommand() {}

ACommand::~ACommand() {}

void ACommand::_ready() {
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    NETWORK_MANAGER = get_node<ANetworkManager>(NodePath("/root/NetworkManager"));
    p2p = get_node<AP2P>(NodePath("/root/P2P"));
}
void ACommand::send(String method, Variant args) {

    Dictionary DATA = Dictionary();
    DATA["TYPE"] = NETWORK_MANAGER->COMMAND;
    DATA["player_id"] = NETWORK_MANAGER->STEAM_ID;
    DATA["method"] = method;    
    DATA["args"] = args;
    p2p->_send_P2P_Packet(0,0,DATA,Steam::P2PSend::P2P_SEND_RELIABLE);

   
}

void ACommand::set_loading_screen(Ref<PackedScene> loading_screen) {
    this->loading_screen = loading_screen;
}
Ref<PackedScene> ACommand::get_loading_screen() {
    return loading_screen;
}