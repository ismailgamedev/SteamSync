extends Node

@onready var loadingScreen = preload("res://addons/steam-sync/SceneChanger/LoadingScreen.tscn")

func change_scene(scene : String):
	Command.send("start_scene",[scene])
	NetworkManager.GAME_STARTED = false
	var instance = loadingScreen.instantiate()
	get_parent().call_deferred("add_child",instance)
	await get_tree().create_timer(0.2).timeout
	instance.change(scene)
	#get_tree().change_scene_to_file(scene)
	
