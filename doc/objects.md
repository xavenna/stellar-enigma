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

| Name           | ID |  Description                           |
|----------------|----|----------------------------------------|
| Solid Block    | 0  | blocks player's path                   |
| Pushable Object| 1  | can be pushed by player                |
| key            | 2  | is picked up, plays specified cutscene |
| board          | 3  | displays message stored in text        |
| cutscene player| 4  | plays a cutscene                       | 
| cactus         | 5  | harms player                           | 
| status         | 6  | gives player a status effect           |

## Arguments:

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
text: name of cutscene to play
Arg 0 (bool): Single-Use?

### Spike:
N/A

### Status Area:
N/A

### Toggle BlocK:
N/A



