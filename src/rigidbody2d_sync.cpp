#include "rigidbody2d_sync.h"
#include "p2p.h"
ARigidBody2DSync::ARigidBody2DSync()
{
    IS_OWNER = false;
    interpolation_value = 0.3;
    call_per_second = 30;
    interval = 1;
    elapsed_time = 0; 
    last_packet_index = 0;
    last_pos = Vector2(0,0);
    IS_OWNER = false;
    state_data = Dictionary();
    packet_index = 0;
}

ARigidBody2DSync::~ARigidBody2DSync()
{
    
}
void ARigidBody2DSync::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("set_interpolation_value", "_interpolation_value"), &ARigidBody2DSync::set_interpolation_value);
    ClassDB::bind_method(D_METHOD("get_interpolation_value"), &ARigidBody2DSync::get_interpolation_value);

    ClassDB::bind_method(D_METHOD("set_call_per_second", "_call_per_second"), &ARigidBody2DSync::set_call_per_second);
    ClassDB::bind_method(D_METHOD("get_call_per_second"), &ARigidBody2DSync::get_call_per_second);

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "INTERPOLATION_VALUE"), "set_interpolation_value", "get_interpolation_value");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "CALL_PER_SECOND"), "set_call_per_second", "get_call_per_second");
}
void ARigidBody2DSync::_ready()
{
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    SteamPtr = Object::cast_to<Steam>(Engine::get_singleton()->get_singleton("Steam"));
    NETWORK_MANAGER = get_node<ANetworkManager>(NodePath("/root/NetworkManager"));
    P2P = get_node<AP2P>(NodePath("/root/P2P"));
    parent = Object::cast_to<RigidBody2D>(get_parent());

    if (SteamPtr->getLobbyOwner(NETWORK_MANAGER->LOBBY_ID) == NETWORK_MANAGER->STEAM_ID)
    {
        IS_OWNER = true;
    }
    interval = 1 / call_per_second;
}

void ARigidBody2DSync::set_physics_state()
{

    if (parent->get_global_position() != last_pos && NETWORK_MANAGER->GAME_STARTED) 
    {
        Dictionary DATA = Dictionary();
        DATA["I"] = packet_index + 1;
        DATA["T"] = ANetworkManager::SEND_TYPE::RIGIDBODY_SYNC;
        Array Value = Array();
        Value.append(parent->get_linear_velocity());
        Value.append(parent->get_angular_velocity());
        Value.append(parent->get_global_position());
        Value.append(parent->get_rotation());
        DATA["V"] = Value;
        DATA["NP"] = get_path();
        P2P->_send_P2P_Packet(0,0,DATA,Steam::P2PSend::P2P_SEND_UNRELIABLE);
        packet_index = packet_index + 1;
        last_pos = parent->get_global_position();
    }
    
}



void ARigidBody2DSync::_physics_process(double delta)
{
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    if (IS_OWNER)
    {
        elapsed_time += delta;
        while (elapsed_time > interval)
        {    
            elapsed_time -= interval;
            set_physics_state();
        }
    }
    else if (!IS_OWNER)
    {
        if (!state_data.is_empty() && NETWORK_MANAGER->GAME_STARTED)
        {
            if (state_data.has("V"))
            {
                uint64_t current_index = state_data["I"];
                if (current_index >= last_packet_index)
                {
                    update_physics_state();
                    last_packet_index = state_data["I"];
                }
            }
            
        }
        
        
    }
    
    
}

void ARigidBody2DSync::update_physics_state()
{
    //UtilityFunctions::print("UPDATE PHYSICS STATE", state_data["V"]);
    Array array_values = state_data["V"];
    Vector2 target_linear_vel = array_values[0];
    double target_angular_vel = array_values[1];
    Vector2 target_pos = array_values[2];
    double target_rot = array_values[3];

    parent->set_linear_velocity(UtilityFunctions::lerp(parent->get_linear_velocity(), target_linear_vel, interpolation_value));
    parent->set_angular_velocity(UtilityFunctions::lerp(parent->get_angular_velocity(), target_angular_vel, interpolation_value));
    parent->set_position(UtilityFunctions::lerp(parent->get_global_position(), target_pos, interpolation_value));
    parent->set_rotation(UtilityFunctions::lerp(parent->get_rotation(), target_rot, interpolation_value));
}

double ARigidBody2DSync::get_interpolation_value()
{
    return interpolation_value;
}

void ARigidBody2DSync::set_interpolation_value(double _interpolation_value)
{
    interpolation_value = _interpolation_value;
}

double ARigidBody2DSync::get_call_per_second()
{
    return call_per_second;
}

void ARigidBody2DSync::set_call_per_second(double _call_per_second)
{
    call_per_second = _call_per_second; 
}



