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
    
    interpolation_pos = 0.15;
    interpolation_rot = 0.15;
    interpolation_scale = 0.15;
    is_only_lobby_owner = false;

    packet_index_pos = 0;
    packet_index_rot = 0;
    packet_index_scale = 0;

    elapsed_time_pos = 0;
    elapsed_time_rot = 0;
    elapsed_time_scale = 0;

    call_per_sec_pos = 20;
    interval_pos = 1;

    call_per_sec_rot = 10;
    interval_rot = 1;

    call_per_sec_scale = 10;
    interval_scale = 1;

    last_pos = Vector2(0, 0);
    last_rot = 0;
    last_scale = Vector2(1, 1);
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
    ClassDB::bind_method(D_METHOD("set_interpolation_rot","_interpolation_ros"), &ATransformSync2D::set_interpolation_rot);
    ClassDB::bind_method(D_METHOD("get_interpolation_scale"), &ATransformSync2D::get_interpolation_scale);
    ClassDB::bind_method(D_METHOD("set_interpolation_scale","_interpolation_scale"), &ATransformSync2D::set_interpolation_scale);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "interpolation_scale"), "set_interpolation_scale", "get_interpolation_scale");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "interpolation_rot"), "set_interpolation_rot", "get_interpolation_rot");
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

        } 
        else if (is_only_lobby_owner)
        {
            if (STEAM_PTR->getLobbyOwner(NETWORK_MANAGER->LOBBY_ID) != NETWORK_MANAGER->STEAM_ID)
            {
                IS_OWNER = false;

            }
            
        }
    }
    
 
    
}


void ATransformSync2D::_process(double delta) {
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
                    last_pos = sync_transform(last_pos,&packet_index_pos,"global_position");
                }
            }
            if (ROTATION)
            {
                elapsed_time_rot += delta;
                while (elapsed_time_rot > interval_rot)
                {    
                    elapsed_time_rot -= interval_rot;
                    last_rot = sync_transform(last_rot,&packet_index_rot,"rotation");
                }
            }
            if (SCALE)
            {
                elapsed_time_scale += delta;
                while (elapsed_time_scale > interval_scale)
                {    
                    elapsed_time_scale -= interval_scale;
                    last_scale = sync_transform(last_scale,&packet_index_scale,"scale");
                }
            }
        }
        else if(!IS_OWNER)
        {
            if (POSITION)
            {
                // POSITION
                if (transform_buffer[0].operator!=(Variant::NIL))
                {
                    uint64_t last_index = last_index_buffer[0];
                    uint64_t current_index = transform_buffer[0]["I"];
                    if (current_index>=last_index)
                    {
                        
                        Vector2 value = transform_buffer[0].get("V");
                        Vector2 current_position = Object::cast_to<Node2D>(get_parent())->get_global_position();
                        Vector2 lerped_value = UtilityFunctions::lerp(current_position, value, interpolation_pos);
                        Object::cast_to<Node2D>(get_parent())->set_global_position(lerped_value);
                        last_index_buffer[0] = transform_buffer[0]["I"];
                    }

                }
            }
            
            if (ROTATION)
            {
                //ROTATION
                if (transform_buffer[1].operator!=(Variant::NIL))
                {
                    uint64_t last_index = last_index_buffer[1];
                    uint64_t current_index = transform_buffer[1]["I"];
                    if (current_index>=last_index)
                    {
                        float value = transform_buffer[1].get("V");
                        float current_rotation = Object::cast_to<Node2D>(get_parent())->get_rotation();
                        float lerped_value = UtilityFunctions::lerp(current_rotation, value, interpolation_rot);
                        Object::cast_to<Node2D>(get_parent())->set_rotation(lerped_value);
                        last_index_buffer[1] = transform_buffer[1]["I"];
                    }
                }
            }
            if (SCALE)
            {
                //SCALE
                if (transform_buffer[2].operator!=(Variant::NIL))
                {
                    uint64_t last_index = last_index_buffer[2];
                    uint64_t current_index = transform_buffer[2]["I"];
                    if (current_index>=last_index)
                    {
                        Vector2 value = transform_buffer[2].get("V");
                        Vector2 current_scale = Object::cast_to<Node2D>(get_parent())->get_scale();
                        Vector2 lerped_value = UtilityFunctions::lerp(current_scale, value, interpolation_scale);
                        Object::cast_to<Node2D>(get_parent())->set_scale(lerped_value);
                        last_index_buffer[2] = transform_buffer[2]["I"];
                    }
                }
            }
        }
    }

}


Variant ATransformSync2D::sync_transform(Variant last_property,uint64_t* packet_index_property,const char* property_name){
    Node2D* object = Object::cast_to<Node2D>(get_parent());
    if (object->get(property_name) != last_property)
    {
        Dictionary DATA = Dictionary();
        //I: Index
        DATA["I"] = *packet_index_property +1;
        //PI: PlayerID
        DATA["PI"] = NETWORK_MANAGER->STEAM_ID;
        //T: TYPE
        DATA["T"] = ANetworkManager::SEND_TYPE::TRANFORM_SYNC;
        //V: Value
        DATA["V"] = object->get(property_name);
        //NP: NodePath
        DATA["NP"] = get_path();
        //VP: Property
        DATA["P"] = property_name;
        P2P->_send_P2P_Packet(0,0,DATA,Steam::P2PSend::P2P_SEND_UNRELIABLE);
        *packet_index_property = *packet_index_property + 1;
        last_property = object->get(property_name);
        return last_property;
    }
    return last_property;
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

void ATransformSync2D::set_interpolation_rot(double _interpolation_rot) {
    interpolation_rot = _interpolation_rot;
}
void ATransformSync2D::set_interpolation_scale(double _interpolation_scale) {
    interpolation_scale = _interpolation_scale;
}

double ATransformSync2D::get_interpolation_rot() {
    return interpolation_rot;
}
double ATransformSync2D::get_interpolation_scale() {
    return interpolation_scale;
}