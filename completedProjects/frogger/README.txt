Assignment 1: Performance

This assignment involved taking a premade HTML5 frogger game and modifying it
to improve its load time and performance.  One necessary modification was
to successfully implement the html5 offline cache to allow the game to be 
played offline

1. To optimize the game, I did a few things.  Firstly, I moved the JS script
tag to the bottom of the body to allow other elements to load first. Then,
I minified both the JS and the CSS to shrink their size, thereby reduce the 
load time for both of them.  After that, I added a small favicon image to the
game and cached that to prevent the browser from trying to load one and getting
a 404 error, which also reduced wait times.  I also used tuftsdev.github.io as 
a CDN through the Yslow interface.  

2. To ensure that the HTML5 offline application cache was working, I wrote the 
manifest file and tried to load the game normally.  Then, I turned off my wifi
and made sure that I could not load anything else on the web.  I then tried to 
reload my Frogger game, which worked.  As an additional verification, I reloaded
the game with the chrome developer tools network tab open and saw that all my
data was pulled from the cache

3. I used Yslow to test the performance of my application.  Surprisingly, it 
gave me a very high rating without changing anything, but it still gave me 
hints as to where I could optimize my game

4. In the end, I was able to upgrade my game from an overall rating of 94 to a
97 according to Yslow.  When I was loading the original game from my comp20 
repository, I was getting between a 200-400 ms load time.  However, after 
optimizing my game, I was able to reduce the load time to 50-125 ms.  This is
less than half of what it was before, and considering how small this application
was, I can esily see how implementing these strategies on large web apps would
be EXTREMELY valuable.

5. One thing that I could not figure out how to fix was a message that Yslow was
giving me about setting expiration times for the browser caches.  However, this
would only be an issue if I was trying to update this game, which I doubt that
I will in the future.  Other than that, I see no performance issues with these 
techniques.