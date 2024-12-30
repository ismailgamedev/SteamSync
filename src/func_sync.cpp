#include "func_sync.h"
#include "p2p.h"


AFuncSync::AFuncSync(){

}

AFuncSync::~AFuncSync(){

}

void AFuncSync::_bind_methods(){
    ClassDB::bind_method(D_METHOD("call_f", "func_name","target","args"), &AFuncSync::call_f);
}
void AFuncSync::_ready(){
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    NETWORK_MANAGER = get_node<ANetworkManager>(NodePath("/root/NetworkManager"));
    P2P = get_node<AP2P>(NodePath("/root/P2P"));
}

void AFuncSync::call_f(String func_name,uint64_t target,Array args){

    Dictionary DATA = Dictionary();
    DATA["PI"] = NETWORK_MANAGER->STEAM_ID;
    DATA["T"] = ANetworkManager::SEND_TYPE::EVENT;
    DATA["NP"] = get_parent()->get_path();
    DATA["args"] = args;
    DATA["method"] = func_name;

    P2P->_send_P2P_Packet(0,target,DATA,Steam::P2PSend::P2P_SEND_RELIABLE);

}