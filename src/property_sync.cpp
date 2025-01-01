#include "property_sync.h"
#include "p2p.h"


void APropertySync::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("set_property_list", "_property_list"), &APropertySync::set_property_list);
    ClassDB::bind_method(D_METHOD("get_property_list"), &APropertySync::get_property_list);

    ClassDB::bind_method(D_METHOD("set_is_reliable", "_is_reliable"), &APropertySync::set_is_reliable);
    ClassDB::bind_method(D_METHOD("get_is_reliable"), &APropertySync::get_is_reliable);

    ClassDB::bind_method(D_METHOD("set_is_interpolation_enabled", "_is_interpolation_enabled"), &APropertySync::set_is_interpolation_enabled);
    ClassDB::bind_method(D_METHOD("get_is_interpolation_enabled"), &APropertySync::get_is_interpolation_enabled);

    ClassDB::bind_method(D_METHOD("set_is_only_lobby_owner", "_is_only_lobby_owner"), &APropertySync::set_is_only_lobby_owner);
    ClassDB::bind_method(D_METHOD("get_is_only_lobby_owner"), &APropertySync::get_is_only_lobby_owner);

    ClassDB::bind_method(D_METHOD("set_object_player", "_object_player"), &APropertySync::set_object_player);
    ClassDB::bind_method(D_METHOD("get_object_player"), &APropertySync::get_object_player);

    ClassDB::bind_method(D_METHOD("set_call_per_second", "_call_per_second"), &APropertySync::set_call_per_second);
    ClassDB::bind_method(D_METHOD("get_call_per_second"), &APropertySync::get_call_per_second);

    ClassDB::bind_method(D_METHOD("set_interpolation_value", "_interpolation_value"), &APropertySync::set_interpolation_value);
    ClassDB::bind_method(D_METHOD("get_interpolation_value"), &APropertySync::get_interpolation_value);

    ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "property_list"), "set_property_list", "get_property_list");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_reliable"), "set_is_reliable", "get_is_reliable");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_interpolation_enabled"), "set_is_interpolation_enabled", "get_is_interpolation_enabled");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_only_lobby_owner"), "set_is_only_lobby_owner", "get_is_only_lobby_owner");
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "object_player"), "set_object_player", "get_object_player");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "call_per_second"), "set_call_per_second", "get_call_per_second");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "interpolation_value"), "set_interpolation_value", "get_interpolation_value");
}


APropertySync::APropertySync()
{
    interpolation_value = 0.1;
    interval = 1;
    elapsed_time = 0;
    call_per_second = 10;
    is_interpolation_enabled = false;
    is_only_lobby_owner = false;
    object_player = "";
    property_list = PackedStringArray();
    last_values = Array();
    IS_OWNER = false;
    DATA = Array();
    is_reliable = false;
}

APropertySync::~APropertySync()
{

}

void APropertySync::_ready()
{
    if (Engine::get_singleton()->is_editor_hint())
    {
        return;
    }
    NETWORK_MANAGER = get_node<ANetworkManager>(NodePath("/root/NetworkManager"));
    P2P = get_node<AP2P>(NodePath("/root/P2P"));
    STEAM_PTR = Object::cast_to<Steam>(Engine::get_singleton()->get_singleton("Steam"));

    parent_node = Object::cast_to<Node>(get_parent());
    if (is_only_lobby_owner == true && STEAM_PTR->getLobbyOwner(NETWORK_MANAGER->LOBBY_ID) == NETWORK_MANAGER->STEAM_ID)
    {
        IS_OWNER = true;
        if (is_reliable)
        {
            send_type = Steam::P2PSend::P2P_SEND_RELIABLE;
        }
        else
        {
            send_type = Steam::P2PSend::P2P_SEND_UNRELIABLE;
        }
        
    }
    else if (is_only_lobby_owner == false && (get_node<Node>(NodePath(object_player)))->get_name().to_int() == NETWORK_MANAGER->STEAM_ID)
    {
        IS_OWNER = true;
        if (is_reliable)
        {
            send_type = Steam::P2PSend::P2P_SEND_RELIABLE;
        }
        else
        {
            send_type = Steam::P2PSend::P2P_SEND_UNRELIABLE;
        }
    }
    if(is_interpolation_enabled == true){
        path = get_path();
    }   
    else{
        path = parent_node->get_path();
        if (IS_OWNER != true)
        {
            set_process(false);
        }
        
    }

    if (property_list.size() > 0)
    {
        for (String property : property_list)
        {
            Variant value = parent_node->get(property);

            last_values.append(value);

            
        }
        
    }
    


}


void APropertySync::_process(double delta)
{
    if (Engine::get_singleton()->is_editor_hint())
    {
        return;
    }
    if (NETWORK_MANAGER->GAME_STARTED)
    {
        if (IS_OWNER)
        {
            for (uint64_t property = 0; property < property_list.size(); property++)
            {
                if (last_values[property] != parent_node->get(property_list[property]))
                {
                    Dictionary DATA = Dictionary();
                    DATA["PI"] = NETWORK_MANAGER->STEAM_ID;
                    DATA["T"] = ANetworkManager::SEND_TYPE::PROPERTY;
                    DATA["V"] = parent_node->get(property_list[property]);
                    DATA["NP"] = path;
                    DATA["P"] = property_list[property];
                    DATA["ITP"] = is_interpolation_enabled;
                    P2P->_send_P2P_Packet(0,0,DATA,send_type);
                    last_values[property] = parent_node->get(property_list[property]);

                }
                
            }
            
        }
        else
        {
            if (!DATA.is_empty())
            {
                UtilityFunctions::print("DATA: ",DATA);
                parent_node->set(DATA[0],UtilityFunctions::lerp(parent_node->get(DATA[0]),DATA[1],interpolation_value));
                DATA.clear();
            }
            
        }
        
    }
    
}

void APropertySync::set_is_interpolation_enabled(bool _is_interpolation_enabled)
{
    is_interpolation_enabled = _is_interpolation_enabled;
}

bool APropertySync::get_is_interpolation_enabled()
{
    return is_interpolation_enabled;
}
bool APropertySync::get_is_reliable(){
    return is_reliable;
}
void APropertySync::set_is_reliable(bool _is_reliable){
    is_reliable = _is_reliable;
}

void APropertySync::set_object_player(NodePath _object_player)
{
    object_player = _object_player;
}

NodePath APropertySync::get_object_player()
{
    return object_player;
}

void APropertySync::set_is_only_lobby_owner(bool _is_only_lobby_owner)
{
    is_only_lobby_owner = _is_only_lobby_owner;
}

bool APropertySync::get_is_only_lobby_owner()
{
    return is_only_lobby_owner;
}

void APropertySync::set_interpolation_value(double _interpolation_value)
{
    interpolation_value = _interpolation_value;
}

double APropertySync::get_interpolation_value()
{
    return interpolation_value;
}

double APropertySync::get_call_per_second()
{
    return call_per_second;
}

void APropertySync::set_call_per_second(double _call_per_second)
{
    call_per_second = _call_per_second;
}

void APropertySync::set_property_list(PackedStringArray _property_list)
{
    property_list = _property_list;
}   

PackedStringArray APropertySync::get_property_list()
{
    return property_list;
}


void APropertySync::set_data(const Dictionary READABLE)
{
    DATA.append(READABLE["P"]);
    DATA.append(READABLE["V"]);
}
