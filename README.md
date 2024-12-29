## Testing API connection

* Export your Spotify developer client ID and secret

```
export SPOTIFY_CLIENT_ID='your_id'
export SPOTIFY_CLIENT_SECRET='your_secret'
```

* Run the command below to run the test connectivity API call (need to run `make` first)

`./spotify_in_c test`

* Log into Spotify using your default host browser, then run 

`./spotify_in_c auth`

* All your user playlists will be list in raw JSON

## Project Structure

```
spotify_in-c/
├── include/
│   ├── api/           # API interfaces
│   │   ├── auth.h     # OAuth2 authentication
│   │   └── endpoints.h # Spotify API endpoints
│   └── core/          # Core functionality
│       ├── config.h   # Configuration & environment
│       ├── http.h     # HTTP request handling
│       └── server.h   # OAuth callback server
│
├── src/
│   ├── api/           # API implementations
│   │   ├── auth.c     # OAuth2 implementation
│   │   └── endpoints.c # API endpoint implementations
│   ├── core/          # Core implementations
│   │   ├── config.c   # Configuration management
│   │   ├── http.c     # HTTP client logic
│   │   └── server.c   # Local callback server
│   └── main.c         # Main program & CLI
│
├── Makefile          # Build configuration
└── README.md         # Project documentation
```
