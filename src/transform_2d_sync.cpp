#include "transform_2d_sync.h"
#include "p2p.h"

ATransformSync2D::ATransformSync2D() {
    IS_OWNER=true ;
    POSITION = true;
    ROTATION = false;
    SCALE = false;
    transform_buffer = TypedArray<Dictionary>();
    transform_buffer.resize(3);
    last_index_buffer = PackedInt64Array();
    last_index_buffer.resize(3);
    
    interpolation_pos = 0.3;

    is_only_lobby_owner = false;

    packet_index_pos = 0;
    packet_index_rot = 0;
    packet_index_scale = 0;

    elapsed_time_pos = 0;
    elapsed_time_rot = 0;
    elapsed_time_scale = 0;

    call_per_sec_pos = 15;
    interval_pos = 1;

    call_per_sec_rot = 15;
    interval_rot = 1;

    call_per_sec_scale = 15;
    interval_scale = 1;

    last_pos = Vector2(0, 0);
    last_rot = 0;
    last_scale = Vector2(0, 0);
}

ATransformSync2D::~ATransformSync2D() {
    
}

void ATransformSync2D::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_is_only_lobby_owner", "_is_only_lobby_owner"), &ATransformSync2D::set_is_only_lobby_owner);	
    ClassDB::bind_method(D_METHOD("set_object_player", "_object_player"), &ATransformSync2D::set_object_player);	
    ClassDB::bind_method(D_METHOD("set_is_position", "_is_position"), &ATransformSync2D::set_is_position);	
    ClassDB::bind_method(D_METHOD("set_is_rotation", "_is_rotation"), &ATransformSync2D::set_is_rotation);	
    ClassDB::bind_method(D_METHOD("set_is_scale", "_is_scale"), &ATransformSync2D::set_is_scale);	
    ClassDB::bind_method(D_METHOD("set_call_per_sec_pos", "_call_per_sec_pos"), &ATransformSync2D::set_call_per_sec_pos);	
    ClassDB::bind_method(D_METHOD("set_call_per_sec_rot", "_call_per_sec_rot"), &ATransformSync2D::set_call_per_sec_rot);	
    ClassDB::bind_method(D_METHOD("set_call_per_sec_scale", "_call_per_sec_scale"), &ATransformSync2D::set_call_per_sec_scale);
    ClassDB::bind_method(D_METHOD("get_is_only_lobby_owner"), &ATransformSync2D::get_is_only_lobby_owner);	
    ClassDB::bind_method(D_METHOD("get_object_player"), &ATransformSync2D::get_object_player);	
    ClassDB::bind_method(D_METHOD("get_is_position"), &ATransformSync2D::get_is_position);	
    ClassDB::bind_method(D_METHOD("get_is_rotation"), &ATransformSync2D::get_is_rotation);	
    ClassDB::bind_method(D_METHOD("get_is_scale"), &ATransformSync2D::get_is_scale);	
    ClassDB::bind_method(D_METHOD("get_call_per_sec_pos"), &ATransformSync2D::get_call_per_sec_pos);	
    ClassDB::bind_method(D_METHOD("get_call_per_sec_rot"), &ATransformSync2D::get_call_per_sec_rot);	
    ClassDB::bind_method(D_METHOD("get_call_per_sec_scale"), &ATransformSync2D::get_call_per_sec_scale);
    ClassDB::bind_method(D_METHOD("set_interpolation_pos","_interpolation_pos"), &ATransformSync2D::set_interpolation_pos);
    ClassDB::bind_method(D_METHOD("get_interpolation_rot"), &ATransformSync2D::get_interpolation_pos);

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "interpolation_pos"), "set_interpolation_pos", "get_interpolation_rot");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "IS_ONLY_LOBBY_OWNER"), "set_is_only_lobby_owner", "get_is_only_lobby_owner");
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "OBJECT_PLAYER"), "set_object_player", "get_object_player");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "IS_POSITION"), "set_is_position", "get_is_position");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "IS_ROTATION"), "set_is_rotation", "get_is_rotation");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "IS_SCALE"), "set_is_scale", "get_is_scale");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "CALL_PER_SEC_POS"), "set_call_per_sec_pos", "get_call_per_sec_pos");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "CALL_PER_SEC_ROT"), "set_call_per_sec_rot", "get_call_per_sec_rot");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "CALL_PER_SEC_SCALE"), "set_call_per_sec_scale", "get_call_per_sec_scale");

}

void ATransformSync2D::_ready() {
    
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    STEAM_PTR = Object::cast_to<Steam>(Engine::get_singleton()->get_singleton("Steam"));
    NETWORK_MANAGER = get_node<ANetworkManager>(NodePath("/root/NetworkManager"));
    P2P = get_node<AP2P>(NodePath("/root/P2P"));
    if (NETWORK_MANAGER->GAME_STARTED == true)
    {
        interval_pos = 1 / call_per_sec_pos;
        interval_rot = 1 / call_per_sec_rot;
        interval_scale = 1 / call_per_sec_scale;
        //Node *player = );
        // UtilityFunctions::print("interval: ",interval_pos );
        // UtilityFunctions::print("Object Player: ",get_node<Node>(NodePath(object_player))->get_name() );
        //uint64_t steam_id = player->get_name().to_int();
        if (is_only_lobby_owner == false && (get_node<Node>(NodePath(object_player)))->get_name().to_int() != NETWORK_MANAGER->STEAM_ID)
        {
            IS_OWNER = false;
            POSITION = false;
            ROTATION = false;
            SCALE = false;
        } 
        else if (is_only_lobby_owner)
        {
            if (STEAM_PTR->getLobbyOwner(NETWORK_MANAGER->LOBBY_ID) != NETWORK_MANAGER->STEAM_ID)
            {
                IS_OWNER = false;
                POSITION = false;
                ROTATION = false;
                SCALE = false;
            }
            
        }
    }
    
 
    
}

void ATransformSync2D::_physics_process(double delta) {
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    if (NETWORK_MANAGER->GAME_STARTED)
    {
        if (IS_OWNER)
        {
            if (POSITION)
            {
                
                elapsed_time_pos += delta;
                while (elapsed_time_pos > interval_pos) {
                    elapsed_time_pos -= interval_pos;
                    sync_position(); // İşlemi burada çağır
                }
            }
            if (ROTATION)
            {
                elapsed_time_rot += delta;
                if (elapsed_time_rot > interval_rot)
                {    
                    elapsed_time_rot -= interval_rot;
                    sync_rotation();
                }
            }
            if (SCALE)
            {
                elapsed_time_scale += delta;
                if (elapsed_time_scale > interval_scale)
                {    
                    elapsed_time_scale -= interval_scale;
                    sync_scale();
                }
            }
        }


    }
    
}

void ATransformSync2D::_process(double delta) {
    if(!IS_OWNER && NETWORK_MANAGER->GAME_STARTED)
    {
        if (transform_buffer[0].operator!=(Variant::NIL))
        {
            uint64_t last_index = last_index_buffer[0];
            uint64_t current_index = transform_buffer[0]["Idx"];
            UtilityFunctions::print("last_index: ",last_index," current_index: ",current_index);
            UtilityFunctions::print("Transform_Buffer: ",transform_buffer[0]);
            if (current_index>=last_index)
            {
                    
                   
                //UtilityFunctions::print("lerp: ",lerped_value);
                Vector2 value = transform_buffer[0].get("value");
                Vector2 current_position = Object::cast_to<Node2D>(get_parent())->get_global_position();
                UtilityFunctions::print("current_position: ",current_position," value: ",value);

                Vector2 lerped_value = UtilityFunctions::lerp(current_position, value, interpolation_pos);
                Object::cast_to<Node2D>(get_parent())->set_global_position(lerped_value);
                last_index_buffer[0] = transform_buffer[0]["Idx"];
            }

        }
    }
}


void ATransformSync2D::sync_position(){
    
    if (Object::cast_to<Node2D>(get_parent())->get_global_position() != last_pos)
    {
        Dictionary DATA = Dictionary();
        DATA["Idx"] = packet_index_pos +1;
        DATA["player_id"] = NETWORK_MANAGER->STEAM_ID;
        DATA["TYPE"] = ANetworkManager::SEND_TYPE::TRANFORM_SYNC;
        DATA["value"] = Object::cast_to<Node2D>(get_parent())->get_global_position();
        DATA["node_path"] = get_path();
        DATA["property"] = "global_position";

        P2P->_send_P2P_Packet(0,0,DATA,Steam::P2PSend::P2P_SEND_UNRELIABLE);
        packet_index_pos = packet_index_pos +1;
        last_pos = Object::cast_to<Node2D>(get_parent())->get_global_position();
    }
    
}

void ATransformSync2D::sync_rotation(){
    UtilityFunctions::print("sync_rotation");
}

void ATransformSync2D::sync_scale(){  
    UtilityFunctions::print("sync_scale");
}

void ATransformSync2D::set_interpolation_pos(double _interpolation_pos) {
    interpolation_pos = _interpolation_pos;
}
double ATransformSync2D::get_interpolation_pos() {
    return interpolation_pos;
}

void ATransformSync2D::set_is_only_lobby_owner(bool _is_only_lobby_owner) {
    is_only_lobby_owner = _is_only_lobby_owner;
}
void ATransformSync2D::set_object_player(NodePath _object_player) {
    object_player = _object_player;
}
void ATransformSync2D::set_is_position(bool _is_position) {
    POSITION = _is_position;
}
void ATransformSync2D::set_is_rotation(bool _is_rotation) {
    ROTATION = _is_rotation;
}
void ATransformSync2D::set_is_scale(bool _is_scale) {
    SCALE = _is_scale;
}

void ATransformSync2D::set_call_per_sec_pos(double _call_per_sec_pos) {
    call_per_sec_pos = _call_per_sec_pos;
}
void ATransformSync2D::set_call_per_sec_rot(double _call_per_sec_rot) {
    call_per_sec_rot = _call_per_sec_rot;
}
void ATransformSync2D::set_call_per_sec_scale(double _call_per_sec_scale) {
    call_per_sec_scale = _call_per_sec_scale;
}

bool ATransformSync2D::get_is_only_lobby_owner() {
    return is_only_lobby_owner;
}
NodePath ATransformSync2D::get_object_player() {
    return object_player;
}
bool ATransformSync2D::get_is_position() {
    return POSITION;
}
bool ATransformSync2D::get_is_rotation() {
    return ROTATION;
}
bool ATransformSync2D::get_is_scale() {
    return SCALE;
}

double ATransformSync2D::get_call_per_sec_pos() {
    return call_per_sec_pos;
}
double ATransformSync2D::get_call_per_sec_rot() {
    return call_per_sec_rot;
}
double ATransformSync2D::get_call_per_sec_scale() {
    return call_per_sec_scale;
}
