#ifndef FUNC_SYNC_H
#define FUNC_SYNC_H

#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/variant/utility_functions.hpp"
#include "godot_cpp/variant/dictionary.hpp"
#include "godot_cpp/classes/scene_tree.hpp"
#include "godot_cpp/variant/array.hpp"
#include "godot_cpp/variant/string.hpp"
#include "godot_cpp/variant/variant.hpp"
#include "NetworkManager.h"


namespace godot
{
class AP2P;
class AFuncSync : public Node
{
    GDCLASS(AFuncSync, Node);
protected:
    static void _bind_methods();

private:
    /* data */
public:
    AFuncSync();
    ~AFuncSync();
    ANetworkManager* NETWORK_MANAGER = nullptr;
    AP2P* P2P = nullptr;
    void call_f(String func_name,uint64_t target,Array args);
    void _ready() override;
};

}








#endif