# Object Documentation:

## Notes:
Object Interface Notes:

Any extension of Object needs to define several things:\
* Constructors: Three are required; base them off Stone's constructors
* Behavior function: view documentation below to utilize the interface

### Interface:
To trigger a state change for the object, set status to a new value:\
Possible Values (Object::Status) :
| Name  | Meaning    |
|-------|------------|
| Normal| No special behavior is required  |
| Destroy| Object should be removed from list after interaction is over  |

### Object Attributes:

* ID : Object's type
* Link ID : Used to define targets for inter-object communication
* Parent ID : The Link ID of the object's parent, if applicable
* Texture ID : Used to select a texture to draw
* Obj-Args : 8 General-use variables to specify object behavior
* Switches : defines which switches the object interfaces which

    Appear = 0
    Disappear = 1
    Act1 = 2
    Act2 = 3
    Trig1 = 4
    Trig2 = 5
    Gen1 = 6
    Gen2 = 7

## Built-in Objects

Overview:\
| Name           | ID |  Description                           |
|----------------|----|----------------------------------------|
| Solid Block    | 0  | blocks player's path                   |
| Pushable Object| 1  | can be pushed by player                |
| key            | 2  | plays cutscene and activates switch on pickup|
| board          | 3  | displays message stored in text        |
| cutscene player| 4  | plays a cutscene                       | 
| cactus         | 5  | harms player                           | 
| toggle-block   | 6  | toggles state upon player contact      |
| door           | 7  | solid, becomes intangible after switch is activated | 


## Arguments/Variables:

### Solid:
Arg 0 (uint): Texture
* default: stone

### Pushable:
Arg 0 (uint): Texture
* default: crate

### Key:
SW Act1: Triggered when collected


### Board:
text: message to display

### Cutscene Player:
Var 0 (uint): remaining cooldown

Arg 0 (bool): Single-Use?
Arg 1 (uint): maximum cooldown duration
text: name of cutscene to play

### Spike:
N/A

### Door:
Var 0 (bool): Is door locked? (init as true)
Var 1 (bool): Was switch activated last frame?
SW Trig1: Opens door, changes sprite, becomes intangible

### Toggle BlocK:
Var 0 (bool): Is Block Active?
Var 1 (uint): Toggle Cooldown remaining

Arg 0 (bool): Starting State
Arg 1 (uint): Maximum cooldown duration

