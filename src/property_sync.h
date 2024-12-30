#ifndef PROPERTY_SYNC_H
#define PROPERTY_SYNC_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include "godot_cpp/classes/packed_scene.hpp"
#include "godot_cpp/classes/resource.hpp"
#include "godot_cpp/variant/string.hpp"
#include "godot_cpp/variant/dictionary.hpp"
#include "godot_cpp/classes/scene_tree.hpp"
#include "godot_cpp/variant/array.hpp"
#include "godot_cpp/variant/variant.hpp"
#include "godot_cpp/classes/object.hpp"
#include "godot_cpp/classes/timer.hpp"
#include "NetworkManager.h"
namespace godot
{
class AP2P;

class APropertySync : public Node
{
    GDCLASS(APropertySync, Node);
protected:
    static void _bind_methods();
private:
    Timer timer = Timer();
    Array DATA;
    Array property_type;
    NodePath path;
public:
    APropertySync();
    ~APropertySync();

    ANetworkManager* NETWORK_MANAGER = nullptr;
    AP2P* P2P = nullptr;

    bool is_only_lobby_owner;
    bool is_interpolated;
    double interpolation_value;
    NodePath object_player;
    PackedStringArray properties;
    void init_timer();
    void _ready() override;
    void _process(double delta) override;
    void set_is_only_lobby_owner(bool _is_only_lobby_owner);
    bool get_is_only_lobby_owner();

    void set_object_player(NodePath _object_player);
    NodePath get_object_player();

    void set_is_interpolated(bool _is_interpolated);
    bool get_is_interpolated();

    void set_interpolation_value(double _interpolation_value);
    double get_interpolation_value();   

    void set_properties(PackedStringArray _properties);
    PackedStringArray get_properties();
};






}


#endif