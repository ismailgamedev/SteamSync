#ifndef RIGIDBODY2D_SYNC_H
#define RIGIDBODY2D_SYNC_H

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
#include "godot_cpp/classes/engine.hpp"
#include "NetworkManager.h"
#include "godotsteam.h"
#include "godot_cpp/classes/node2d.hpp"
#include "godot_cpp/classes/rigid_body2d.hpp"
namespace godot
{
class AP2P;

class ARigidBody2DSync : public Node
{
    GDCLASS(ARigidBody2DSync, Node);
protected:
    static void _bind_methods();

private:
    /* data */
public:
    ANetworkManager* NETWORK_MANAGER = nullptr;
    AP2P* P2P = nullptr;
    Steam* SteamPtr = nullptr;

    double interpolation_value;
    uint64_t packet_index;
    uint64_t last_packet_index;
    Vector2 last_pos;
    double elapsed_time;
    double interval;
    double call_per_second;
    bool IS_OWNER;
    RigidBody2D* parent = nullptr;
    Dictionary state_data = Dictionary();
    double get_interpolation_value();
    void set_interpolation_value(double _interpolation_value);
    double get_call_per_second();
    void set_call_per_second(double _call_per_second);


    void _ready() override;
    void _physics_process(double delta) override;
    void set_physics_state();
    void update_physics_state();

    ARigidBody2DSync();
    ~ARigidBody2DSync();


};



}
#endif