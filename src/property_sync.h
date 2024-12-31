#ifndef PROPERTY_SYNC_H
#define PROPERTY_SYNC_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include "godot_cpp/variant/dictionary.hpp"
#include "godot_cpp/classes/scene_tree.hpp"
#include "godot_cpp/variant/packed_string_array.hpp"
#include "godot_cpp/variant/variant.hpp"
#include "godot_cpp/classes/object.hpp"
#include "godot_cpp/variant/vector2.hpp"
#include "godot_cpp/variant/array.hpp"
#include "godotsteam.h"
#include "NetworkManager.h"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/variant/string.hpp"
#include "godot_cpp/classes/node2d.hpp"

namespace godot
{

class AP2P;

class APropertySync : public Node
{
    GDCLASS(APropertySync, Node);
protected:
    static void _bind_methods();
private:
    
public:
    ANetworkManager* NETWORK_MANAGER = nullptr;
    Steam* STEAM_PTR = nullptr;
    AP2P* P2P = nullptr;
    bool is_only_lobby_owner;
    NodePath object_player;
    NodePath path;
    Node* parent_node = nullptr;
    Array last_values;
    Array DATA;
    bool IS_OWNER;
    Steam::P2PSend send_type;
    double interval;
    double elapsed_time;
    double call_per_second;
    
    bool is_reliable;

    bool is_interpolation_enabled;
    double interpolation_value;



    PackedStringArray property_list;



    APropertySync();
    ~APropertySync();


    void _ready() override;
    void _process(double delta) override;

    void set_object_player(NodePath _object_player);
    NodePath get_object_player();

    void set_is_only_lobby_owner(bool _is_only_lobby_owner);
    bool get_is_only_lobby_owner();

    void set_call_per_second(double _call_per_second);
    double get_call_per_second();
    
    void set_is_reliable(bool _is_reliable);
    bool get_is_reliable();
    void set_data(Dictionary READABLE);
    void set_property_list(PackedStringArray _property_list);
    PackedStringArray get_property_list();
    void set_is_interpolation_enabled(bool _is_interpolation_enabled);
    bool get_is_interpolation_enabled();
    void set_interpolation_value(double _interpolation_value);
    double get_interpolation_value();
};


}













#endif