#ifndef TRANSFORM_2D_SYNC_H
#define TRANSFORM_2D_SYNC_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include "godot_cpp/variant/dictionary.hpp"
#include "godot_cpp/classes/scene_tree.hpp"
#include "godot_cpp/variant/array.hpp"
#include "godot_cpp/variant/variant.hpp"
#include "godot_cpp/classes/object.hpp"
#include "godot_cpp/variant/vector2.hpp"
#include "godotsteam.h"
#include "NetworkManager.h"
#include "godotsteam.h"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/variant/string.hpp"
#include "godot_cpp/classes/node2d.hpp"

namespace godot
{

class AP2P;

class ATransformSync2D : public Node
{
    GDCLASS(ATransformSync2D, Node);
protected:
    static void _bind_methods();
private:
    double elapsed_time_pos;
    double elapsed_time_rot;
    double elapsed_time_scale;
public:
    ATransformSync2D();
    ~ATransformSync2D();

    ANetworkManager* NETWORK_MANAGER = nullptr;
    Steam* STEAM_PTR = nullptr;
    AP2P* P2P = nullptr;
    bool is_only_lobby_owner;
    NodePath object_player;

    bool POSITION;
    bool ROTATION;
    bool SCALE;

    uint64_t packet_index_pos;
    uint64_t packet_index_rot;
    uint64_t packet_index_scale;

    Vector2 last_pos;
    float last_rot;
    Vector2 last_scale;

    TypedArray<Dictionary> transform_buffer;
    PackedInt64Array packet_index_buffer;
    
    double call_per_sec_pos;
    double call_per_sec_rot;
    double call_per_sec_scale;

    double interval_pos;
    double interval_rot;
    double interval_scale;

    void set_call_per_sec_pos(double _call_per_sec_pos);
    void set_call_per_sec_rot(double _call_per_sec_rot);
    void set_call_per_sec_scale(double _call_per_sec_scale);

    double get_call_per_sec_pos();
    double get_call_per_sec_rot();
    double get_call_per_sec_scale();

    void set_is_only_lobby_owner(bool _is_only_lobby_owner);
    bool get_is_only_lobby_owner();

    void set_is_position(bool _is_position);
    bool get_is_position();

    void set_is_rotation(bool _is_rotation);
    bool get_is_rotation();

    void set_is_scale(bool _is_scale);
    bool get_is_scale();

    void set_object_player(NodePath _object_player);
    NodePath get_object_player();

    void sync_position();
    void sync_rotation();
    void sync_scale();

    void _ready() override;
    void _physics_process(double delta) override;
};
}
#endif