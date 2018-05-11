# Class Diagram #
<img src="Class%20Diagram.png" alt="Class diagram" width="800" />

# Class Explanation #
## Game ##
### Responsabilities ###
- Create all components
- Configure all components
- Start all components
- Game flow control (state machine)
- Stop components and free resources


## MentalMap ##
### Responsabilties ###
- Store info about game state
    - Enemies
    - Players
    - User ( also a player )
    - Weapons
- Let users modify this values concurrently (thread-safe)
- Single object (singleton)
- Offer events when the data changes, and listeners for those events can be attached (for example, to update the GUI )
- Update its own info according to data received from the server (listens to NetworkManager updates)
- Weapon and bullet control and monitoring (reload and shoot delays, collision calculation)


## SoundManager ##
### Responsabilities ###
- Only one instance is allowed (singleton)
- Load sounds in memory and play them when requested by game
- Abstract interface implemented by SDLSoundManager


## InputManager ##
### Resposabilities ###
- Only one instance is allowed (singleton)
- Poll the keyboard, joystick, gamepad, etc for key/button presses
- Offer input events, notifying the event to the listeners attached
- Run on its own thread


## ImageManager (?) ##
### Responsabilities ###
- Obtain an image from the webcam
- Notify the game/GUI when a new image arrived

## Robot ##
### Resposabilities ###
- Offer a interface to communicate with the robot and control the high-level movements (forward, left, right, etc)

## RobotClient ##
### Resposabilities ###
- Being executed on the robot, receive the commands from the Robot class, and act accordingly

## UserInterface ##
### Responsabilites ###
- Represent the interface the user is currently seeing
- Each subclass should take care of each concrete user interface ( LoginScreen, GameScreen, MenuScreen, etc)
- Should update the contents of the screen when MentalMap data changes 
- Should offer a basic navigation (up, down, left, right, enter, esc) interface, for menus.

## NetworkManager ##
### Resposabilities ###
- Send updates to the server
- Load updates from the server
- Manage login process
- Manage game setup
