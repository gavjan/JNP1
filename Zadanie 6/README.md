# Playlists in C++
## The Goal
The goal of the task is to write a library implementing part of the media player with the functionality of creating and managing playlists.

The main library functionalities include:

- support for songs and movies,

- creating playlists with the ability to add and remove songs and movies to them
and other playlists (you can nest playlists),

- setting the play order for the list (sequentially, randomly, odd/even), and then play in the order specified.

All library errors should be reported through exceptions. The root of the exceptions library should be PlayerException class.

## Class Methods
The Player class should provide public methods:

- `openFile(file)` - creates and returns a song based on a file; the song can be played alone or added to playlist;

- `createPlaylist(name)` - creates and returns a playlist with the given name together with the default (sequential) playback order set; the list can be restored alone or added to another playlist; cycles in playlists are not acceptable.


The class representing the playlist should provide the following public methods:

- `add(element)` - adds an element to the end of the playlist;

- `add(element, position)` - adds an element to the specified position in the playlist(positions are numbered from 0);

- `remove()` - removes the last item from the playlist;

- `remove(position)` - removes item from the specified playlist position.

- `setMode(mode)` - sets the mode(order) of playing songs; mode of playing can be created using the `createSequenceMode()`, `createShuffleMode(seed)` function,
`createOddEvenMode()`.

- `play()` - plays the songs in the list in the order given:
    - (a) sequentially - in the order of the playlist,
    - (b) random - in the order specified by std::shuffle called from std::default_random_engine,
    - (c) odd/even - first all odd and then all even playlist items.

The song class should provide the following public methods:

- `play()` - playing a song consists of writing "Song" to the standard output, song performers, song title and content;
song content may only contain alphanumeric characters, white space and the following special characters: `,` `.` `!` `?` `'` `:` `;` `-`.

The video class should provide the following public methods:

- `play()` - playing a movie consists of writing "Movie" to the standard output, movie title, year of production and content; the content of the movies is encoded in ROT13 and can contain only alphanumeric characters, white space and the following special characters: `,` `.` `!` `?` `'` `:` `;` `-`.

    - The File class contains a file description, in particular a song or movie description.
    - The class provides a public constructor based on the description.
    - The description contains subsequent parts describing the song.
    - These parts are separated by the `|` character.
    - The first part of the description is type(for songs it will be 'audio' and for movies 'video').
    - The last part of the description is the content of the song(for movies encoded in ROT13).
    - Intermediate parts of the description describe the file's metadata in the format name: value(required for songs there is metadata called 'artist' and 'title', for movies metadata called 'title' is required and 'year', other metadata may also exist for both).
    - In addition, you must provide functions that create different ways of playing for playlists:

- `createSequenceMode()` - returns the sequential way of playing;

- `createShuffleMode(seed)` - returns a random way of playing with the created object std::default_random_engine with the given seed;

- `createOddEvenMode()` - returns the odd/even playing method.


## Other requirements
A very important part of the task is to design the appropriate class hierarchy and relationships between classes.
In particular, not all classes that you might require have been explicitly specified in the task description.
During designing you should be guided by SOLID principles and you should consider GoF patterns design. In particular, ways to reproduce lists should be designed in such a way so that you can add or restore another method without changing the existing code, according to the Open-Closed Principle principle of the SOLID principles.

## Example
An example of using the library is in the file `playlist_example.cc` and the result of it can be found in `playlist_example.out`