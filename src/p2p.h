#ifndef P2P_H
#define P2P_H

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
#include "NetworkManager.h"
#include "godot_cpp/classes/file_access.hpp"
namespace godot {

class AP2P : public Node
{
    GDCLASS(AP2P, Node);
private:

protected:
	static void _bind_methods();

public:
    AP2P();
    ~AP2P();

    ANetworkManager* NETWORK_MANAGER = nullptr;
    const int PACKET_READ_LIMIT = 32;

    void _read_All_P2P_Packets(int64_t read_count  = 0);
    void _read_P2P_Packet();
    bool _send_P2P_Packet(int16_t channel,int64_t target,Dictionary packet_data,Steam::P2PSend send_type);
    void handle_start_packet(Dictionary READABLE);
    void handle_event_packets(Dictionary READABLE);
    void handle_property_packets(Dictionary READABLE);
    void handle_voice_packets(Dictionary READABLE);
    void _process(double delta) override;
    void _ready() override;
};








}

#endif