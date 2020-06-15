Used Callback command pattern for inputmanager because it does not require you to make a new class every time and is easier to pass extra variables to
Used Observer for soundmanager as it's useful to notify it of an event which plays a sound rather than saving the sound somewhere else
Gameloop Has an Update, FixedUpdate and Render (where fixedupdate runs at 60FPS)
Uses a Resourcemanager singleton that keeps track of all the textures & fonts (which also prevents multiple instances)
SceneManager can delete and reload scenes (with option to destory previous scene on loading)

https://github.com/rubsab/2DAE02_Sabbe_Ruben_Programming4_Engine