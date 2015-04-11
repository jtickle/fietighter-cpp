# fietighter-cpp

TL;DR: GNU GPL v3.  Non-Newtonian Space Shooter Engine.  Good luck getting it to build.

This was a project for Artificial Intelligence class back in college.  I spent
two months on a game engine, then remembered it needed AI, and threw together
something that actually worked out pretty well and made some interesting
emergent behavior.

I enjoyed some certain games as a kid based on movies made by a certain awkward
bearded man.  I hope I don't get sued for the name of this project.  Look,
Mickey, I'm just saying - imitation is the sincerest form of flattery.

After about half of the AI's are killed off in the first few
seconds, the remainder are locked into these loops where they can't turn fast
enough to hit their target, but they're too dumb to think about trying a
diffrent target or a random vector.  It's pretty, but ineffective.

Also, if I make them better at breaking these loops, they're going to be a force
to be reckoned with.  I'll have to tone down the precision of their shooting a
bit.

There's another improvement though: the game is currently instagib; that is to
say, if the target is in your sights and you mash the button, they're done for
instantaneously.  It acts too much like real lasers in how quick the effect
would be but too little like real lasers in how energetic the effect is.  A
nice middle ground is fake lasers from sci-fi movies that take time to travel
and deal whopping damage upon impact.  Maybe they're dense proton beams?  But
those wouldn't work effectively in an atmosphere, and they WOULD be traveling
at relativistic velocities.  So the lasers will be fake just like the physics.

Oh yeah, it's totally non-Newtonian.  Have you ever played a space shooter that
actually uses Newtonian physics?  The Real World disappoints once again!  In
150 years when the Mars One colony finally revolts for real (not like the fake
2099 Amazon Prime Special Event) we will be lobbing phone poles made of
tungsten into absurd orbits to make them smash into each other at high speeds
and it'll take years for them to hit their targets.  How droll.

So instead take a trip into a fantasy world.  Go get CodeBlocks, and the Ogre
libraries, and ... damn I can't remember, a bunch of Ogre plugins, you know -
get all that set up properly (actually refer to my CodeBlocks project file to
get an idea of where they go), upgrade my code to match the Ogre API because
they've broken it on me before (pull requests welcome)...

No the better plan is to hop on over to https://github.com/jtickle/fietighter
where I am reimplementing this using three.js.  Eventually.  I like to take my
time with things.
