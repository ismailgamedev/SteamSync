#include "p2p.h"


static Steam *SteamPtr = nullptr; 


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
    SteamPtr = Object::cast_to<Steam>(Engine::get_singleton()->get_singleton("Steam"));
    NETWORK_MANAGER = get_node<ANetworkManager>(NodePath("/root/NetworkManager"));

    
}

void AP2P::_process(double delta) {
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    if (NETWORK_MANAGER->LOBBY_ID > 0)
    {
        _read_All_P2P_Packets();
    }
   
}

void AP2P::_read_All_P2P_Packets(int64_t read_count) {
    if (read_count >= PACKET_READ_LIMIT)
    {
        return;
    }
    if (SteamPtr->getAvailableP2PPacketSize(0) > 0)
    {
        _read_P2P_Packet();
        _read_All_P2P_Packets(read_count + 1);
    }
    
    
}

void AP2P::_read_P2P_Packet() {
    uint32_t packet_size = SteamPtr->getAvailableP2PPacketSize(0);
    if (packet_size > 0)
    {
        Dictionary this_packet = SteamPtr->readP2PPacket(packet_size, 0);
        if (this_packet.is_empty() || this_packet == Variant::NIL) 
        {
            UtilityFunctions::print("WARNING: read an empty packet with non-zero size!");
        }
        PackedByteArray packed_code  = this_packet["data"];
        
        Dictionary READABLE = UtilityFunctions::bytes_to_var(packed_code.decompress(-1,FileAccess::COMPRESSION_GZIP));
        
        int64_t sender = this_packet["sender"];

        if (READABLE.has("TYPE"))
        {
            handle_property_packets(READABLE);
            handle_event_packets(READABLE);
            handle_start_packet(READABLE);
            handle_voice_packets(READABLE);
        }
        
    }
}

bool AP2P::_send_P2P_Packet(int16_t channel,int64_t target,Dictionary packet_data,Steam::P2PSend send_type) {

    PackedByteArray this_data = PackedByteArray();
    PackedByteArray compressed_data = UtilityFunctions::var_to_bytes(packet_data).compress(FileAccess::COMPRESSION_GZIP);

    this_data.append_array(compressed_data);

    if (target == 0)
    {
        if (NETWORK_MANAGER->LOBBY_MEMBERS.size() > 1)
        {
            for (int member = 0; member == NETWORK_MANAGER->LOBBY_MEMBERS.size(); member++)
            {
                if (member["steam_id"] != NETWORK_MANAGER->STEAM_ID)
                {
                    return SteamPtr->sendP2PPacket(member["steam_id"], this_data, send_type, channel);
                }
            }
        }
        
    }
    else
    {
        return SteamPtr->sendP2PPacket(target, this_data, send_type, channel);
    }
    return false;
    
}


void AP2P::handle_start_packet(Dictionary READABLE) {
    
}

void AP2P::handle_event_packets(Dictionary READABLE) {
}
void AP2P::handle_property_packets(Dictionary READABLE) {
}
void AP2P::handle_voice_packets(Dictionary READABLE) {
}

