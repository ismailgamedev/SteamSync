#pragma once
#ifndef P2P_H
#define P2P_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/variant/string.hpp"
#include "godot_cpp/variant/dictionary.hpp"
#include "godot_cpp/classes/scene_tree.hpp"
#include "godot_cpp/variant/array.hpp"
#include "godot_cpp/variant/variant.hpp"
#include "godot_cpp/classes/file_access.hpp"
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>


#include "godotsteam.h"
#include "NetworkManager.h"
namespace godot {
 

class ATransformSync2D;
class ACommand;
class APropertySync;
class AP2P : public Node
{
    GDCLASS(AP2P, Node);
private:
protected:
	static void _bind_methods();

public:
    AP2P();
    ~AP2P();
    ACommand* COMMAND = nullptr;
    ANetworkManager* NETWORK_MANAGER = nullptr;
    const int PACKET_READ_LIMIT = 32;
    uint16_t check_type(Dictionary READABLE);
    uint64_t STEAM_ID_DEBUG;

    
    uint64_t get_steam_id_debug();
    void set_steam_id_debug(uint64_t _steam_id_debug);
    void _read_All_P2P_Packets(int64_t read_count  = 0);
    void _read_P2P_Packet();
    bool _send_P2P_Packet(int16_t channel,int64_t target,Dictionary packet_data,Steam::P2PSend send_type);
    void handle_start_packet(Dictionary READABLE);
    void handle_event_packets(Dictionary READABLE);
    void handle_property_packets(Dictionary READABLE);
    void handle_voice_packets(Dictionary READABLE);
    void handle_custom_packets(Dictionary READABLE);
    void _process(double delta) override;
    void _ready() override;
};








}

#endif