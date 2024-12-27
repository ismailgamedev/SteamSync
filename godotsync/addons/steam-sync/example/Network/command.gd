extends ACommand

	
func start_scene(scene : String):
	NetworkManager.GAME_STARTED = false
	var instance = loading_screen.instantiate()
	get_parent().call_deferred("add_child",instance)
	instance.change(scene)
	
