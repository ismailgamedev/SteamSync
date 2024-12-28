#include "transform_2d_sync.h"


ATransformSync2D::ATransformSync2D() {
    POSITION = true;
    ROTATION = false;
    SCALE = false;

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

    if (NETWORK_MANAGER->GAME_STARTED == true)
    {
        interval_pos = 1 / call_per_sec_pos;
        interval_rot = 1 / call_per_sec_rot;
        interval_scale = 1 / call_per_sec_scale;
        Node *player = get_node<Node>(NodePath(object_player));

        UtilityFunctions::print("Object Player: ",player->get_name() );

        if (player->get_name().to_int() != NETWORK_MANAGER->STEAM_ID)
        {
            POSITION = false;
            ROTATION = false;
            SCALE = false;
            UtilityFunctions::print("ATransformSync2D: Player node is not the owner of the this object");
        } 
        else if (is_only_lobby_owner)
        {
            if (STEAM_PTR->getLobbyOwner(NETWORK_MANAGER->LOBBY_ID) != NETWORK_MANAGER->STEAM_ID)
            {
                UtilityFunctions::print("ATransformSync2D: Is only lobby owner is true but Player node is not the owner of the lobby!");
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
    if (POSITION)
    {
        elapsed_time_pos += delta;
        if (elapsed_time_pos > interval_pos)
        {    
            elapsed_time_pos -= interval_pos;
            sync_position();
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

void ATransformSync2D::sync_position(){
    UtilityFunctions::print("sync_position");
}

void ATransformSync2D::sync_rotation(){
    UtilityFunctions::print("sync_rotation");
}


void ATransformSync2D::sync_scale(){  
    UtilityFunctions::print("sync_scale");
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
