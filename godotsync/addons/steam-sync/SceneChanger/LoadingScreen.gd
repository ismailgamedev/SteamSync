extends CanvasLayer


func change(scene_name: String):
	if scene_name != null:
		if Command.get_tree().change_scene_to_file(scene_name) != 0:
			printerr("Wrong Scene Path " + str(scene_name))
	else:
		print(scene_name)
func _process(delta):
	if NetworkManager.GAME_STARTED == true:
		queue_free()
