
# That2DGameEngine

That2DGameEngine is a 2D component-based game engine written in C++. This game engine uses SDL for rendering/audio, glm for vector math and ImGui. 
Most of the syntax in this engine is based on the syntax of classes in Unity. (e.g. GameObject, Component, ...)

## Features

This is a list of the most prominent features included in this engine:
These topics are explained in detail below.

- GameObjects/Components
- Scene management
- Texture and sprite rendering
- Box physics
- Observers and event queue
- Resource management
- Command based input system with keyboard and controller support
- Audio

## Patterns and techniques used
This engine uses a few game design / programming patterns. They are listed below:

- Game loop 
- Update method
- Component
- Dirty flag
- Singleton
- Command
- Observer
- Event queue
- Service locator

Beside these patterns, I have also used these techniques and idioms:

- Multithreading
- pImpl

## Usage

### Scene Management
Scene management is handled by the SceneManager singleton.

Adding scenes to the game can be done using the `SceneManager::AddScene` method which takes a function that has the initialization of the scene.
At the start of the engine, the scene that has been added the first (index 0) will be automatically loaded.

You can load another scene using the `SceneManager::LoadScene` method which takes an index.

### Scenegraph

The base of any scenegraph is a **scene** object. This class can only store gameobjects.  You can add a gameobject to a scene using the `Scene::CreateGameObject` method.
If the parent of a gameobject is set to `null` the gameobject will be parented to the scene.

**Gameobjects** are classes that can have both children (other gameobjects) and components. Gameobjects by them selves have no logic and only act as containers for components and children objects. 
Each gameobject also has a tag which can be used to differentiate certain gameobjects from others. Gameobjects can be added using the `GameObject::CreateGameObject` method and components can be added using the `GameObject::AddComponent` method which is templated.

**Components** are classes that derive from `that::Component` and have multiple methods that can be overloaded such as `Init`, `Update`, `Render`, `OnGui`, `OnEnable`, `OnDisable`, etc.
These components are the base of the engine since these are the classes that will provide functionality to gameobjects.

Components that are in the engine:
- Transform
- Texture/SpriteRenderer
- Rigidbody
- AudioSource

If at any moment in development you'd like to see a **scenegraph** of your scenes, you can enable this in the SceneManager singleton by calling `that::SceneManager::GetInstance().DrawSceneHierarchy(true)`.

### Singletons
The engine has a few singletons to offer to the user.

- InputManager (can be used bind input)
- Window (can be used resize the window)
- Resource/TextureManager (can be used to load textures/fonts/...)
- SceneManager
- EventQueue
- Physics
- ServiceLocator (can be used to access an audiosystem)

These classes can be accessed from anywhere in the project using `that::ClassName::GetInstance()`.

### Resource management

To initialize a `TextComponent` or `SpriteRenderer` you need a *font* or a *texture*.
These can be retrieved from either the `ResourceManager` or the `TextureManager`.

The **ResourceManager** can be used to load fonts (only gets loaded once) and textures (gets loaded every time).
The **TextureManager** can be used to load textures as well but these get cached (so they only get loaded once).
Which method you use to load a texture is completely up to you and the circumstances.

### Input
Every game needs some sort of input. This engine offers a whole range of possibilities inside the InputManager singleton.

You can bind commands to digital (keyboard or controller) and axis buttons.
There are also methods to bind 2D axis commands (using both digital or axis buttons)

All the bind methods begin with the word `Bind` and can thus be easily found.
Examples:

    that::InputManager::GetInstance().BindDigitalCommand
    that::InputManager::GetInstance().BindAxisCommand
    that::InputManager::GetInstance().BindDigital2DAxisCommand
    that::InputManager::GetInstance().BindAnyDigitalKeyboardCommand

The engine has a base Command interface you can inherit from to create your own command classes. There are two command classes implemented that already derive from this base command (`DataCommand` and `LambdaCommand`). 

### Observers and event queue

This engine offers the observer pattern in the form of two classes: `Subject` (the class being observed) and `Observer`.  
Both classes are templated. The type passed into the template is the type of the variable passed to the Notify method of the Observer.
Observers can only observe a subject if they have the same template type. 

The subject class should be made a member variable so an observer class can call `Subject::AddListener` on the subject.
Observer is an interface that has a method Notify that needs to be overloaded. This method will be called if the subject that the observer is observing gets triggered.
  
  
The user can instead choose to send/listen to events on the EventQueue singleton.
You can send events using `EventQueue::SendEvent` and listen to events by inheriting from the EventListener interface and calling `EventQueue::AddListener`.
Just like observers, EventListeners are templated and will only be notified from events that have the same template type as the listeners.

### Audio

There are two ways to play audio in That2DGameEngine.

#### 1. AudioSystem
A user can send events to the AudioSystem using methods such as `AudioSystem::Load`, `AudioSystem::Play`, `AudioSystem::Pause`, etc.
The audio system of the engine is accessible in the ServiceLocator singleton and can be accessed like this: `that::ServiceLocator::GetAudio()`.

#### 2. AudioSource component
Most of the logic that can be done using the AudioSystem has been wrapped inside the AudioSource component. 
The AudioSource component has methods such as `AudioSource::Play`, `AudioSource::SetLooping`, `AudioSource::SetVolume`.

#### Under the hood
The AudioSystem implemented in the engine using SDL_Mixer but this has no impact to the user. This audio system runs on a different thread using an event queue.
The user can implement their own AudioSystem and register it to the ServiceLocator using the `ServiceLocator::RegisterAudioSystem` method.

### Physics
I have implemented 2D box physics in the engine.
These can be accessed using two components: `BoxCollider` and `Rigidbody`.

The **BoxCollider** component offers overlap events. Each collider can be assigned one or more layers. These layers can be used for ignore layers or to distinguish certain box colliders from others.

The **Rigidbody** component needs a BoxCollider component to work. It has gravity and also collision correction. 

Debug rendering of box colliders can be enabled using the Physics singleton using the `Physics::ActivateDebugRendering` method.

### ImGui

Each component can overload the `Component::OnGui` method in which you can call any ImGui method without issues.



# Dig Dug

That2DGameEngine comes with a default project which is a remake of the arcade game Dig Dug.


## Gameplay
The game can be played using both keyboard and controller.
There are three modes: 

- Singleplayer (default Dig Dug gameplay)
- Co-Op where there are two Dig Dug characters working together)
- Versus where one player controls Dig Dug and the other play controls a Fygar enemy

Each mode has four levels, after these four levels the game ends.

There is a high score table of 10 high scores. Each high score has a username displayed next to it of three characters.

## Patterns used
Next to the patterns that are used in the engine, this game also uses the State pattern for the player, enemies and the game state.
