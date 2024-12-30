extends CharacterBody2D



const SPEED = 90.0
const JUMP_VELOCITY = -180.0
var IS_OWNER : bool = false

func make_owner():
	
	IS_OWNER = true
	$Camera2D.enabled = true
func _physics_process(delta: float) -> void:
	if IS_OWNER:
		# Add the gravity.
		if not is_on_floor():
			velocity += get_gravity() * delta

		# Handle jump.
		if Input.is_action_just_pressed("ui_accept") and is_on_floor():
			velocity.y = JUMP_VELOCITY

		# Get the input direction and handle the movement/deceleration.
		# As good practice, you should replace UI actions with custom gameplay actions.
		var direction := Input.get_axis("ui_left", "ui_right")
		
		if direction > 0:
			$PlayerSprite.flip_h = false
		elif direction < 0:
			$PlayerSprite.flip_h = true

		
		if direction:
			velocity.x = direction * SPEED
		else:
			velocity.x = move_toward(velocity.x, 0, SPEED)

		move_and_slide()

func printergo(isim : String):
	print(isim)
	if IS_OWNER:
		$AFuncSync.call_f("printergo",0,[isim])
	
func _input(event: InputEvent) -> void:
	if event is InputEventKey:
		if event.keycode == KEY_F1 and event.pressed:
			printergo("TESTO")
