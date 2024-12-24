#include "p2p.h"



AP2P::AP2P() {
}

AP2P::~AP2P() {
}

void AP2P::_bind_methods() {
}

void AP2P::_ready() {
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    
    NETWORK_MANAGER = get_node<ANetworkManager>(NodePath("/root/NetworkManager"));
    
}

void AP2P::_process(double delta) {
   
}

void AP2P::_read_All_P2P_Packets(int64_t read_count) {
}

void AP2P::_read_P2P_Packet() {
}

void AP2P::_send_P2P_Packet(int16_t channel,int64_t target,Dictionary packet_data,int16_t send_type){

}
void AP2P::handle_packets(Dictionary READABLE) {
}

void AP2P::handle_start_packet(Dictionary READABLE) {
}

void AP2P::handle_event_packets(Dictionary READABLE) {
}

