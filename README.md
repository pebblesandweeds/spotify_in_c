## Testing API connection

* Export your Spotify developer client ID and secret

```
export SPOTIFY_CLIENT_ID='your_id'
export SPOTIFY_CLIENT_SECRET='your_secret'
```

* Run the command below to run the test connectivity API call (need to run `make` first of course)

`./spotify_in_c test`

## Project Structure

```
spotify-playlist-manager/
├── include/
│   ├── api/            # Existing API stuff
│   │   ├── auth.h
│   │   └── endpoints.h
│   ├── core/           # Existing core stuff
│   │   ├── config.h
│   │   └── http.h
│   └── structures/     # NEW: Our data structures
│       ├── playlist.h  # Doubly linked list for playlist management
│       ├── queue.h     # Singly linked list for change tracking
│       └── collection.h # Circular linked list for playlist collections
│
├── src/
│   ├── api/            # Existing API stuff
│   │   ├── auth.c
│   │   └── endpoints.c
│   ├── core/           # Existing core stuff
│   │   ├── config.c
│   │   └── http.c
│   ├── structures/     # NEW: Implementations
│   │   ├── playlist.c  # Playlist operations
│   │   ├── queue.c     # Change queue operations
│   │   └── collection.c # Collection management
│   └── main.c         # Main program & test function
│
├── build/             # Compiled objects
│   ├── api/
│   │   ├── auth.o
│   │   └── endpoints.o
│   ├── core/
│   │   ├── config.o
│   │   └── http.o
│   └── structures/
│       ├── playlist.o
│       ├── queue.o
│       └── collection.o
│
├── Makefile
└── README.md
```
