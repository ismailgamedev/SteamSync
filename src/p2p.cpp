#include "p2p.h"
#include "command.h"
#include "transform_2d_sync.h"
#include "property_sync.h"
#include "rigidbody2d_sync.h"
static Steam *SteamPtr = nullptr; 


AP2P::AP2P() {
}

AP2P::~AP2P() {
}

void AP2P::_bind_methods() {
    ClassDB::bind_method(D_METHOD("send_P2P_Packet","channel","target","packet_data","send_type"), &AP2P::_send_P2P_Packet);
    ClassDB::bind_method(D_METHOD("set_steam_id_debug","_steam_id_debug"), &AP2P::set_steam_id_debug);
    ClassDB::bind_method(D_METHOD("get_steam_id_debug"), &AP2P::get_steam_id_debug);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "steam_id_debug"), "set_steam_id_debug", "get_steam_id_debug");
}

void AP2P::_ready() {
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    SteamPtr = Object::cast_to<Steam>(Engine::get_singleton()->get_singleton("Steam"));
    NETWORK_MANAGER = get_node<ANetworkManager>(NodePath("/root/NetworkManager"));
    COMMAND = get_node<ACommand>(NodePath("/root/Command"));
    
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
    //UtilityFunctions::print("PACKET SIZE: ",packet_size);
    if (packet_size > 0)
    {
        Dictionary this_packet = SteamPtr->readP2PPacket(packet_size, 0);
        if (this_packet.is_empty() || this_packet == Variant::NIL) 
        {
            UtilityFunctions::print("WARNING: read an empty packet with non-zero size!");
        }
        PackedByteArray packed_code  = this_packet["data"];
       

        Dictionary READABLE = UtilityFunctions::bytes_to_var(packed_code.decompress_dynamic(-1,FileAccess::COMPRESSION_GZIP));
        

        
        if (READABLE.has("T"))
        {
            //UtilityFunctions::print("READABLE: ",READABLE);
            handle_property_packets(READABLE);
            handle_event_packets(READABLE);
            handle_start_packet(READABLE);
            handle_voice_packets(READABLE);
            handle_custom_packets(READABLE);
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
            for (int i = 0; i <NETWORK_MANAGER->LOBBY_MEMBERS.size(); i++)
            {
                uint64_t steam_id = NETWORK_MANAGER->LOBBY_MEMBERS[i].get("steam_id");
                if (steam_id != NETWORK_MANAGER->STEAM_ID)
                {
                    return SteamPtr->sendP2PPacket(steam_id, this_data, send_type, channel);
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

void AP2P::set_steam_id_debug(uint64_t steam_id_debug) {
    STEAM_ID_DEBUG = steam_id_debug;
}
uint64_t AP2P::get_steam_id_debug() {
    return STEAM_ID_DEBUG;
}
void AP2P::handle_start_packet(Dictionary READABLE) {
 
    if (check_type(READABLE) == ANetworkManager::READY)
    {
        for (int member_data = 0; member_data <NETWORK_MANAGER->MEMBERS_DATA.size(); member_data++)
        {
                Dictionary data = NETWORK_MANAGER->MEMBERS_DATA[member_data];
                if (data["steam_id"] == READABLE["PI"]){
                    data["ready"] = READABLE["ready"];
                }
        }
    } 

}
uint16_t AP2P::check_type(Dictionary READABLE) {
    uint16_t type = READABLE["T"];
    return type;
}

void AP2P::handle_event_packets(Dictionary READABLE) {

    
    if (check_type(READABLE) == ANetworkManager::COMMAND)
    {
            auto args = READABLE["args"];
            if (args.operator!=(Variant::NIL))
            {
                //UtilityFunctions::print("COMMAND ARGS: ",args);

                COMMAND->callv(READABLE["method"],args);
            }
            else
            {
                COMMAND->call(READABLE["method"]);
            }

    }
    else if (check_type(READABLE) == ANetworkManager::EVENT && NETWORK_MANAGER->GAME_STARTED)
    {
        auto args = READABLE["args"];
        if (args.operator!=(Variant::NIL))
        {
            get_node<Node>(READABLE["NP"])->callv(READABLE["method"],args);
        }
        else
        {
            get_node<Node>(READABLE["NP"])->call(READABLE["method"]);
        }
    }
    
        
}
void AP2P::handle_property_packets(Dictionary READABLE) {
    if (NETWORK_MANAGER->GAME_STARTED)
    {
        if (check_type(READABLE) == ANetworkManager::TRANFORM_SYNC )
        {
        
            if (READABLE["P"] == "global_position"){
                ATransformSync2D *transform_sync = get_node<ATransformSync2D>(READABLE["NP"]);
                transform_sync->transform_buffer[0] = READABLE;
            }
            else if (READABLE["P"] == "rotation"){
                ATransformSync2D *transform_sync = get_node<ATransformSync2D>(READABLE["NP"]);
                transform_sync->transform_buffer[1] = READABLE;
            }
            else if (READABLE["P"] == "scale"){
                ATransformSync2D *transform_sync = get_node<ATransformSync2D>(READABLE["NP"]);
                transform_sync->transform_buffer[2] = READABLE;
            }
        }
        else if (check_type(READABLE) == ANetworkManager::PROPERTY ){
            if (READABLE["ITP"].operator==(false))
            {
                get_node<Node>(READABLE["NP"])->set(READABLE["P"],READABLE["V"]);
            }
            else{
                
                APropertySync *node = get_node<APropertySync>(READABLE["NP"]);
                //get_node<APropertySync>(READABLE["NP"])->set_data(READABLE);
                node->set_data(READABLE);
            }
            
        }
        else if (check_type(READABLE) == ANetworkManager::RIGIDBODY_SYNC)
        {
            ARigidBody2DSync *rigidbody_sync = get_node<ARigidBody2DSync>(READABLE["NP"]);
            rigidbody_sync->state_data = READABLE;
        }


        
    }
    

    
    
}
void AP2P::handle_voice_packets(Dictionary READABLE) {
}

void AP2P::handle_custom_packets(Dictionary READABLE) {
}
