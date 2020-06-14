 $$$$$$\                  $$\                 $$$$$$$\           $$\           
$$  __$$\                 $$ |                $$  __$$\          \__|          
$$ /  \__| $$$$$$\   $$$$$$$ | $$$$$$\        $$ |  $$ |$$$$$$\  $$\ $$$$$$$\  
$$ |      $$  __$$\ $$  __$$ |$$  __$$\       $$$$$$$  |\____$$\ $$ |$$  __$$\ 
$$ |      $$ /  $$ |$$ /  $$ |$$$$$$$$ |      $$  ____/ $$$$$$$ |$$ |$$ |  $$ |
$$ |  $$\ $$ |  $$ |$$ |  $$ |$$   ____|      $$ |     $$  __$$ |$$ |$$ |  $$ |
\$$$$$$  |\$$$$$$  |\$$$$$$$ |\$$$$$$$\       $$ |     \$$$$$$$ |$$ |$$ |  $$ |
 \______/  \______/  \_______| \_______|      \__|      \_______|\__|\__|  \__|

Engine made by Fieu Nicky
BitBucket repository : https://bitbucket.org/NickyFieu/code_pain/src/master/

External liberaries used in this engine:
	- glm
	- imgui
	- imgui_sdl
	- sdl2
	- sdl2_image
	- sld2_ttf
	- sdl2_mixer

Engine main loop:
	- The engine main loop has a few parts that run on diffrent update cycles
	- Update/Draw 	is ran at a target cycle rate of 240 updates / s 
	- FixedUpdate 	is ran at a target cycle rate of 40 updates / s ( more than 40 for physics is not needed as sweeping is implemented )
	- Input		is ran at a target cycle rate of 1200 update / s ( 1200 to make sure input feels smooth and responsive )

Scene manager:
	- Holds any amount of scenes
	- Holds the currently active scene in a pointer and only updates that one

Scenes:
	- At most 1 scene can be ran at a time, a scene holds game objects
	- Functions get called from the scene manager
	- A scene has a name and this name is hashed to a size_t

GameObject:
	- Has a enum class GameObjectType this is used to differentiate gameobjects from eachoter and easily get all gameobjects of a specific type
	- A game object can be set to not active meaning it will not update/draw or do any functionality
	- A game object can be set to be destroyed meaning in the next update cycle of the scene it will be removed from the scene and game
	- Holds a inactive tiemr that can be set to make a game object sleep for x amount of time. ( still draws but does not update )
	- Holds a current and new state variable the state will only update once the state has been initialized and from then on
		can only be changed from inside of the current state itself. If the bool returned from the current state execution is true
		then a new state has been set and it will sweap states.
	- Is final! ( no prefabs )
	- Holds an amount of observers

BaseComponent:
	- Has an enum class that has to be updated each time a new type of component gets created! this enum is used for safe type casting and easy access to
		all components of a certain type.
	- Holds a pointer to the owning gameobject.
	- Is abstract!

ResourceManager:
	- Is a singleton!
	- Can load in a SDL_Texture, SDL_Surface and a font.
	- Stores a std::string of the data file path where all data should be held

Logger:
	- Is a singleton!
	- Has an enum class loglevels describing the intensity of the issue at hand.
	- Logs to an imgui logging window!

Renderer:
	- Is a singleton!
	- Can render textures in 4 diffrent ways
	- Can render empty color rect.
	- Can render a color line.
	- Can render a filed color rect.

BinaryReaderWriter:
	- Used to read in or write binary date from or to a file.

Input:
	Command:
		- Is an abstract class
		- Calls an execute function passing a GameObject
	Handler:
		- Handles all the given input actions given to it  
		- Supports both controller / keyboard / mouse uttons and controller axis
		- Uses input manager to handle the commands it needs to execute
	Manager:
		- Manages and updates all input states of keys given to it and all controllers
		- Supports pressed, held and released input actions


Audio locator:
	- Is a service locator for managing audio
	- Defaults to null audio unless given the users own audio system
	

Observer:
	- Is an abstract class
	- Uses a pre defined enum with general definitions as events that "could" be used by the user
	- To notify an observer you pass the current event and a game object

GameManager:
	- Should be seen as a link between the engine and game
	- Not realy cleanly made up right now 
	- Holds a game state

Engine pre compiled header:
	- Has define MULTI_THREADING
	- Has define SAFE_DELETE
	- Has define FLT_EPSILON
	- Has templated function IS_VALID to check if not nullptr

Collision:
	- Handled in the fixed update
	- Dynamic collision box colides with static collision boxes => AABB Sweep
	- Dynamic collision box overlaps with dynamic collision boxes => AABB
	- Components needed are CollisionBox and RigidBody ( could be seen as the controller )
	- Has an enum collision side wich is used to chose wich sides you can collide with as the user
		(for example in bubble bobble you yourself can jump trough the bottom of blocks but some enemies cant (flying ones like pulpul and monsta))


Transform:
	- SDL 0 0 is top left of window so transform inverts the y to make sense that -y is down and +y is up

Sprites:
	- A sprite component can hold multiple sprites with a hashed string as its key value in a unordered map ( can add any amount of sprites to your liking )

Debug draw:
	- In debug mode some components have extra debug visuals

RigidBody:
	- Used as a manager for collision objects
	- Has a prefixedupdate for setting the velocity before the collision boxes do their fixed update using that velocity in
		sweptAABB collision detection wich works perfectly with box shapes ( most 2d games dont need more )
		then using the results of all collision boxes the rigid body moves the owning gameobject accordingly
	




