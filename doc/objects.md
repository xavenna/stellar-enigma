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


## Built-in Objects

Overview:\
| Name           | ID |  Description                           |
|----------------|----|----------------------------------------|
| Solid Block    | 0  | blocks player's path                   |
| Pushable Object| 1  | can be pushed by player                |
| key            | 2  | is picked up, plays specified cutscene |
| board          | 3  | displays message stored in text        |
| cutscene player| 4  | plays a cutscene                       | 
| cactus         | 5  | harms player                           | 
| toggle-block   | 6  | toggles state upon player contact      |
| status         | 7  | gives player a status effect           |

## Arguments/Variables:

### Solid:
Arg 0 (uint): Texture
* default: stone

### Pushable:
Arg 0 (uint): Texture
* default: crate

### Key:
N/A

### Board:
text: message to display

### Cutscene Player:
Var 0 (uint): remaining cooldown

Arg 0 (bool): Single-Use?
Arg 1 (uint): maximum cooldown duration
text: name of cutscene to play

### Spike:
N/A

### Status Area:
N/A

### Toggle BlocK:
Var 0 (bool): Is Block Active?
Var 1 (uint): Toggle Cooldown remaining

Arg 0 (bool): Starting State
Arg 2 (uint): Maximum cooldown duration

