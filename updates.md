# developer updates

2026-03-02:

Move updates to updates.md (make room for lab text).

2026-03-01:

Rename Client -> Unit (conflicts with Arduino built-in Client). (also improves local unit in screen demo)

I renamed Client to Unit because the former conflicted with one of Arduino's built-ins after I got rid of an argument.. A downside of trying to use short common words for names is that occasionally someone else has gotten there first. (Fair's fair.)

This also caused the local screen cli^H^H^H^H unit to start showing the screen message! Sort of. It flickers a lot when there's a client attached. There's no flicker in the main branch, so I know this isn't just a basic limit I've hit. somewhere I'm wasting *most* of my time doing something pointless over and over.

I swear my goal with this change wasn't to get the screen working again. But I must admit I'm seldom as fastidious about changes as I know I ought to be, so it's *proabably* one of those 'tweaks', and not just the rename that had this effect.

The bot will be so excited if I give it this update. It's been constantly asking about jitter, despite my telling it there aren't any feedback loops here to cause the kind of back-and-forth overcorrection loop the term usually implies in robotics. This is all feed-forward, so everything should move right along.

And then the local display on the scrolling screen client starts flickering, and so much for all that. But at least the demo has gone back to scrolling in response to both dials. The 'one dial moves all the displays' behavior doesn't need the local screen, so the basic demo is back to working.

Next will be looking into the in-stream channel changer. Right now whatever I'm doing appears to just crash the device, though not in a way that improves display rate on the client's local until I reboot the remote device.

2026-02-28:

(rename: enum PinFunction->Function; int hardware_size->interface)

Bit of light renaming before I try to get the last demo working. I was using channel 0 inside the screen class to run local hardware. But that was when the interface setup was a fragile mess that I meant to replace and was loathe to handle more than needed before excision. Now that's done, the right way to handle this is at the controller level. Give the controller hardware and it'll set up a client on 'channel 0' using that hardware.

This novel contraption has odd qualms. (And I can be a qualmy guy -- ask anyone!) In I2C, channel zero is for special use. The chatbot was **very** alarmed at the idea of my jacking it for the local. But on this particular hardware network I'm designing, I don't anticipate an *even more special use* than running the single client's own hardware. The device clients just ignore you if you try to send a message on an invalid channel, so the fallout here should be limited.

Anyway, when I put together the screen client (a scrolling-message device), because the dial and display clients seemed solid (and still do, so far) I was *bad* and skipped straight to the over-the-wire version. Then when I unthinkingly tried to the obvious and use the same trick on the client the heap of course blew up. (don't even try to think through the results here -- even in a program this small, "double-local-client" is way into vorpal-bunnies-ate-my-socks / "Her mind has been permanently Demented." territory.)

To atone, I must do what is needed (barring regression) to get the local, single board, dial-scrolled screen working. Then, and only then, will I turn back to the multi-device setup and the in-stream channel selector.

2026-02-26:

Get display_channel working. (uses channel selector.)

Again, mostly just a matter of "is that plugged in? yes? what about that? ah..."

2026-02-25:

Sliced out little bits of the Interface code in manageable chunks until I couldn't work out how to get more out without function template evasion shenanigans. Then I switched modes into "delete it all and sort it out later."

So right now the device-level scripts work fine (i.e., display_local, dial_client...) but the larger demos (anything using the channel selector, for instance) are still a mess. If you want to pull, do it from a different branch. I'll merge this one back in when everything is working again.

No more tonight -- it's late.

2026-02-24:

Add hardware.h, meant to replace Interface* pinout mess.

Last night I got stalled out trying to add some channel-swapping behavior by the usual "int* p vs int p[k]" / "what is this a pointer to again?" / "maybe I can cheat here. nope, now it spouts gibberish for five seconds, then boots normally and works for a while, then an guy shows up in an Uber with a Hawiaiian pizza demanding I sign a $1000 reciept." business.

Only in C++, man. It's like, if we can't get proper closures AND we can't pass a fixed-size array without carefully stepping over various unlabeled trap-doors to Wonderland then what are we even *doing* in this neighborhood? 

After extensive back-and-forth with the bot (more proposals of new levels of containers and abstract classes, and me saying "no std in arduino" for what feels like the nth time) I've decided the right way forward is to abandon the "yeah, we'll find a pinout somewhere, trust me" laissez faire of Interface* pinout for the strict constructor discipline of const Hardware& hardware; and dealing with it explicitly in every constructor. (Only in C++...)

This also gives me a good opportunity to rename the abstraction, which had been bothering me. Asking the bot to guess the new name was a laugh though. it: "Hmm, you need some way to refer to the hardware. So how about Topology, or FunctionGrid? Am I getting warmer?" me: "hmm, not exactly...". (exact quotes available at usual link.)

This will be several changes even if everything goes smoothly. There's (0) this one, add Hardware(just an array of fixed size that gets declared annoyingly as int[(int)PinFunction::END] -- gibberish to my students) and drop it in the root file. Next (1) thread a const Hardware& through all the constructors that will need it. Then (2) switch pin reads from the existing pinout_[PinFunction::MY_FN] method to the new bool get_pin(const Hardware& hardware, PinFunction fn). Last, (3) remove unused Interface/pinout code.

Then maybe I can get my channel selector working *and* keep all these toes I feel so attached to.

--

Thread one dial function ('clock') through the new hardware path.

(much later in the day)

This work is certainly underway at this point. I hope it's even fair to say I've reached a crack in the dam. The stuff flowing out looked wet anyway, and I tried drinking some and haven't died yet.

Getting going with this was such a pain. Once I found the right way to do it (with the robot's extensive help -- new link below, the old one was getting laggy) things went... quickly would be the wrong word, but as with most programming work, I didn't usually remain stuck on any individual point for long enough to think about asking for help.

The control loop would have to get a lot tighter for this version of the tool to speed me up in just writing some basic code. I understand that some of the tools people are using inside the big shops are getting interesting. But I'm working on other stuff right now.

Anyway, once all the references were in place things felt like they were rolling downhill. Turning the dial on the device didn't work -- until I'd actually threaded everything through all the relevant call paths. Then it did. Same on the client side: move it through, there it is, no problem. I guess that's the point of doing the up-front work here. But what a pain.

At least things are moving in what seems like a productive direction. Should be more code deletion coming. I like that part.

(new AI chatbot thread: https://chatgpt.com/share/699e57e8-ad84-8003-9c61-aa003648919b)

2026-02-23:

Client dial scrolls device displays.

This was the demo I was looking for, and it seems stable, so this is probably it for now. (maybe some cleanup later, who knows?) This part I got done myself, though I did give the bot regular updates. It was a lot like some interactions I've had with PMs -- tracking details, side conversations, general encouragement. (same thread)

With the other cleanups finished this part landed in motion, ready to go on first boot. I can't ask for more than that.

2026-02-22:

Scrolling-message demo for Board, a replacement for Display. (though more on those names below.)

This one took longer than seems reasonable given the line count. There was a long diversion with the robot into function pointers and proposals for abstract class handlers over my desire to interact with the Control class like I would in Lisp, Python, or even Java (create a little iterable object representing just the working clients and mostly interact with that). (same link, updated).

C++ offers dozens of plausible-looking ways of doing this (surely hundreds if we use a more kitchen-sink desktop development focused form of the language), of which some fraction will compile. (and it can seem like a vanishing fraction if you're new here.) of those, most will either blow up on you immediately, or *sometimes* work for a while before their behavior decays and your program's behavior slowly goes more non-linear until it crashes.

I tried to get the chatbot to give me a way to iterate like this that it would approve of in terms of easy-to-read and safe-to-use that beat what I would have done as a kid (allocate storage in place; loop through to copy in the pointers I wanted.)

When all we needed was zero-argument lambdas (I used to call them 'thunks') this worked okay-ish, modulo corp-speak names from the robot (it: 'ClientFn' would be a good name!; me: we'll go with 'Command'.), and epic struggle with C++'s abstract function syntax on my end (yes, it still needs a return type.). But once I wanted multi-argument functions that abstraction started to break down.

In the end it realized just after I did that the correct answer was to lean harder into the 'drawers of pointers, numbered by channel (and remember to clean them out first!)' metaphor. Rather than stepping forward into the late-century world of lambdas and abstract classes, the readable solution here was from the 1960s: they're pointers, just leave out the zeros and then use them, no need for an intermediate container.

Anyway, that plus a little "did you initialize everything? what if we reboot?" later it's scrolling on multiple channels again. No dial interaction with this one, so I won't remove the other demo yet, but I think the clock's ticking on that now.

Once I delete that code, I want to rename a bunch of stuff. The lab has little 4-digit seven segment displays in an almost unbounded quantity. (My central goal here is to get them into my students' projects without the students giving up because of C++ and other distractions.) I initially called these 'Display' because the most common-and-correct name for them is 'seven segment displays'.

But then my students kept calling them screens. So Screen it was. Then I called a collection of screens a Display (seemed reasonable enough). But I didn't really like that name, so I replaced it with Board.

Now that there's an abstraction almost ready to replace Display, I think I want to actually *flip* these names. The Screen is the software object I want the students to be interacting with. The Screen can run multiple Displays.

This kind of big rename is the sort of change you have to do on its own, or everything will go haywire. It's not time yet, but I'd like to get this handled today.

First though, this thing is acting a little funny still. Bit more debugging needed. But I wanted to capture the point when the device started scrolling again.

Next main step (after debugging, before renaming) is to add the remote dial response back in. If I can get that going on more than one screen (should be possible now if all the parts are moving like I want) then that'll be solid progress.

Beyond that, if I can rename the classes and then get the local client working on channel zero without having to change much of anything? For me, that would count as a win.

--

Fix miscellaneous bugs (with assist from chatbot). 

After pushing the change above I pasted the diff into the AI, and modulo some formatting-based nonsense (can I view the diff raw, or do they hide that?) it was like, "Fix these three bugs (off-by-one; zombie debugging code; just fine, as it figured out mid-explanation) and this spelling error." (except after C, indeed...) And just like it said on the tin, after dealing with the things it instantly listed out for me, the demo went from "does something possibly interesting" to "boringly does what I wanted".

So, another win for team robot. (link updated: https://chatgpt.com/share/699853fb-a1b8-8003-9d56-4e841b3d149c)

(and yes, after misspelling 'receipt' I did check my work on 'miscellaneous'. And 'misspelling'. I've gotten used to those little red lines! but I haven't gone looking for that option in Emacs. yet.)

--

Device dial now adjusts that device's screen in Display.

If I had better VC discipline it would be clearer how little was really needed to get this going. The key change was adding the dial read in just before the call to at(), plus the required zeroing.

That got the board responding to the dial, but haltingly, because it didn't have an update function. Or rather, the update function (present) was private, and called sparingly, I think because the chat bot seemed to feel sensitive about doing that too often? So the only thing that made it update was when Onir asked it to pan the message.

But the bot has often been confused about what is and isn't going to be making Wire calls. All the Board does is write to some storage in the Screen clients, which themeselves ignore all these goings-on and keep their own schedule for making phone calls. So right here I think it's fine to just bang it.

The rest of the change is more of this and that: add the option to keep all the screens at the start instead of fanning them out. Add some logging code. You know. Main thing is, the demo works! (but not on channel 8? odd, but something for later I think,

--

2026-02-21:

Add IODevice and Control/Client, with demo sketches (remote device dial & screen).

(Ditching the ordinal commit labels since git doesn't number things that way.)

This year's least impressive bench demo: a dial and screen needing not just one board to run them, but now two! One side is running a Control looking for devices. If it finds one on channel 9, it reads the device's dial state, and writes the dial's value to the screen.

C++ makes this just awkward enough to be annoying. I've committed my share of semicolons (if mostly long ago) and still much of what I spend time thinking about isn't what I want the program to do, but how to make the type system accept it without littering the code with the kind of copy this to that / that back to this 'helper' functions I've a saddening fraction of production code comprise. (sometimes running on big metal, at least by the standards of the time.)

The bot (link updated) was of limited help here. The fact that it's a pattern-matching system works against it here. It found way the code is fairly simple at most levels perplexing, despite its frequent hectoring about clean layer boundaries.

It particularly kept trying to push the Wire code further up- and down-stack and through more of the logic. As currently written the client code only needs to know that there's something called a Wire that it can call with a particular phone number and either give or take a message. The device code doesn't know anything at all about I2C -- the sketch just directly tinkers with its public state member. I find the way it seemed to think the *lack* of confusing I2C code in the files was something requiring a lot of alarmed-sounding explanatory comments written to... someone? (My students aren't gonna ask why the telephone set isn't out on the desk.)

It was, however, fairly useful for writing pretty-print functions and other such utilities. And I half-expect that it will spot the pointer error crashing the demo immediately -- it's fairly good with that kind of thing.

--

(later)

Fix missing-null-terminator and divide-by-zero errors.

As predicted, I did a lot of squinting and rubbing my forehead while the bot fretted about the wisdom of using pointers, and possible I2C race conditions (some seemingly involving future client code reaching back through a temporal-address error and crashing my present-day Arduino). Also as predicted, once I pasted the specific code problem (its pretty print method exposed my lack of null-terminator in an array I'd just been treating as a block of four small numbers) it spotted the problem right away and had me immediately nodding agreement.

Also fixed another silly error in a previously-disused codepath. (They don't even let you divide by zero in python!)


2026-02-20:

11 & 12: Get rid of [(int)PinFunction::END] clutter (It's meaningless to students.)

I added a 'using' declaration to turn a bunch of visual clutter into a word, except where C++ made that difficult for me. (https://chatgpt.com/share/6998ac1d-c3e8-8012-9a14-e79c23442880)

9: Remove CharDisplay/CharDevice and associated demos; organize demos. 

8: Add a class called Display that splits a string across a handful of screens and keeps them refreshed.

I got the bot to write this yesterday from a description and some iteration. It seemed like the type of problem it should be best at: empty page, not much to integrate with upstream and nothing downstream. As usual it started with the omni-loop style. But it only took some gentle, non-directive nudging to get it to break out the most egregious repeated chunk, the logic to find the right letter to show.

Initially the core logic was maybe 1/3 edge-checks and guards against bad inputs. Sometimes if you simplify your expectations about behavor, a lot of that logic falls away like a dry chrysalis, and you can just say what you need to say. In this case, stipulating that the offset would loop around the message circularly had this effect: lots of vanishing guard conditions, higher signal:noise for students trying to figure out what's going on.

But modulo the usual difficulties with the robot refusing to read the code and stubbornly making up methods (honestly I've gone through worse with junior hires) the code worked almost out of the box. First win on my books for team robot overlords.

(https://chatgpt.com/share/699853fb-a1b8-8003-9d56-4e841b3d149c)


2026-02-19:

seventh commit: Duplicate ChannelSelector into Selector, with a script that uses it to select a channel for Screen.

Not much to say here. Did this by hand, gonna see if the machine can do anything useful with the next part.

The next obvious step would be to update the multi-display demo to use the Screen class, but I don't want to do that. There's a bunch of logic dealing with more than one screen in there that belongs at the library level. Then I'll make a new multiscreen demo and delete most of the existing examples.

Have to think a minute on the design. I'll probably let the robot take a crack at it -- it loves that kind of high level hand-wavy stuff. We'll see how the ideas are.

(no new link, updated the thread.)

sixth commit: Miscellaneous readability / cleanup / calligraphy (yes).

This was also around the time I tried again to stuff the Wire handler that lives in the device script into the class as a static method, and once again figured out there's no good way to do that without covering the entire class in static declarations and initializations that are meaningless noise to my students.

Instead, I have followed the example of Aesop's fox, and decided that the presence of this alarming code ("readBytes((byte*)&device.state"? looks bold.) at the top level of the script is not a bug, but a feature.

After all, the prospect of being in the position of an I2C device would be alarming! Imagine being given a phone and told you have to pick up whenever it rings, then and believe anything you are told. (*That* is how the device do...)

The alert reader will note I said no *good* way of smuggling information into the handler. Like last year when I wrote the initial version of this code, I did, however, think of a *bad* way. Last year I had to talk myself off of this particular ledge.

But this year I could get robotic assistance with my problem, and say to a bot, "hey, did'ja ever notice that the parameter with the message size is the same as the Uno's address table? because I sure as heck noticed that." Then I got to hear a big pile of algebra play back a vague, half-coherent echoing chorus chanting dire tones laying out the reasons this would not be the right path to go down. (https://chatgpt.com/share/699755fb-e7fc-8003-9667-4ed68952f0f1)

I found this to be a reassuring experience overall, since if you read though most of my chats with the bot you'll see it validating my various choices to a degree I find kind of alarming. But it turns out this mule can balk when the path ahead is nothing but rocks to the horizon.

2026-02-18:

fifth commit: Add screen and screen_device, meant as a replacement for char_display. I find it amusing that my AI coding assistant, faced with the task of adapting this code, more or less told me, "Looks like you got this one, I'll just watch from here."

in all fairness, there was plenty of cruft I could remove and still have the minimal local screen reading "onir" and the I2C client/device pair blinking the colon. I probably would have made the bot port most of it over, but if I had to chip it out with my parentheses by hand I saw the upside of letting it go.

Next step is to swap the key demos over to use screen (and drop the non-key demos), and delete the char_thing code.

(updated the chat thread on the server side. no new link.)

third/fourth commit: (dropped the initial one that was just the creation of the repo without the files.) I used ChatGPT (the 'Plus' version, from my personal account) to split up the dial code into a client and device side, and got the demos working again.

I'd describe the result as mixed. I could've written the code in as much time as it took to produce with the model. Then I would've spent those hours writing C code instead of relitigating with a robot all of the semantic and procedural decisions I previously made with readability as my main (or only!) constraint. As the model put it, "The assistant frequently tried to generalize or conventionalize the structure. The final version is better because those changes were rejected."

But the end result wasn't that different from plenty of other code collaboration/review processes I've been part of. Maybe I'd rather be reviewing and mutating autogenerated code into saying what I want than writing it from scratch using letters and parentheses? I've written lots of C, and I'm sure I'll write more soon. But arguing with robots is pretty new. And I'm hoping that having hashed through these points we can pick up steam with the next part.

(https://chatgpt.com/share/6995fae0-6750-8003-951d-a63edae4d822)

second commit: Remove stray root directory.
(technical assistance: https://chatgpt.com/share/6995e7e2-22b4-8003-9260-1ff04d0b619f)

2026-02-17:
first commit of onir, a simple I2C library for dials, switches, and (eventually) servos. I hope it will help my students practice using some components that are greatly abundant in ko lab.

working sketches; char_channel, multi_char_client, char_device, local_char_device, simple_char_client, local_dial_device, test_char_client

I'm uploading onir to github because there's enough of it that it seems worth stashing here, and because I'm going to ask some coding AIs to handle the TODO material in dial_encoder.h to see how they manage the task.

I haven't successfully used AI for programming so far. I tried giving one of my old graph-traversal questions to ChatGPT shortly after its public release and was impressed at how readily it spat out what looked like working Python and C++ solutions. But it seemed to get lost when I tried it with anything bigger, and to have very different instincts than me when it came to how to break things up. And I didn't have any programs to write that I couldn't quickly bang out by hand anyway, so I deferred.

But I keep hearing that recent improvements have been impressive. Here I have a chunk of code that should be enough to generalize from. And I have other things to get done this week. So I thought I'd give it another shot.


