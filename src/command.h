#ifndef COMMAND_H
#define COMMAND_H

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
#include "godot_cpp/classes/object.hpp"
#include "NetworkManager.h"
#include "p2p.h"
#include "godot_cpp/classes/control.hpp"
#include "godot_cpp/classes/packed_scene.hpp"
#include "godot_cpp/classes/resource.hpp"
class ACommand : public Node
{
    GDCLASS(ACommand, Node);
protected:
    static void _bind_methods();
private:
    /* data */
public:
    ANetworkManager* NETWORK_MANAGER = nullptr;
    AP2P* p2p = nullptr;
    Ref<PackedScene> loading_screen;
    ACommand();
    ~ACommand();

    void send(String method, Variant args);  
    void _ready() override;

    void set_loading_screen(Ref<PackedScene> loading_screen);
    Ref<PackedScene> get_loading_screen();
};

#endif