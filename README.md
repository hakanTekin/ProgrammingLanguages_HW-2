# ProgrammingLanguages_HW-2
## C homework of PL course
A program for keeping notes, written in C.

There are 4 main commands, ADD, TAG, FIND, DISPLAY.

### ADD
Will add a new note with a unique ID

### TAG
Will tag notes with given ID's. If the given tag name is not specified, a new tag will be created. A note can have multiple tags.

### DISPLAY
Displays a note with a given ID.

### FIND
Finds the notes corresponding to the given query. Find command supports 4 additional commands for different combinations of notes.

#### AND
Finds all notes that has all of the attributes specified.
FIND( AND( important, timely ) ) will return all notes that are both timely and important.

#### OR
Finds notes that have atleast one of the tags specified
FIND( OR( work, plan )) will return all the notes that are either work or plan tagged.

#### NOT
Acts as AND but returns the notes that are not specified. 
FIND( NOT( important, timely ) ) will return the notes that does NOT have the tags important and timely together.
