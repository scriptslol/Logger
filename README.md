# Logger
C++ Logger DLL

_looking back on this now... i'm finding that this repo could stand for a little more explanation..._

- SomeApp: This is a `Client` application that interacts with the `Logger`
- Logger(DLL): This is a `Server` application that fulfills the `LoggerAPI` Contract
- LoggerAPI: This is a dual-facet; cooperative API. One lives with the `Client` as a proxy/stub class which represents a DLL. The other is a `Server` which "Fulfills" a Behavioral Contract.

I've grown really obsessed with this level of formalization. Not neccessariliy as way to code- but, as a way to cooperate with other programmers.  The painful thing with this library is that the DLL interface is consumable only by C++ applications that drag along `LoggerDLL.h` and `LoggerDLL.cpp`...

My thinking on the subject has taken me to many weird places and has adapted from the rigors of working in AAA games tooling/engine development.

I'm a fan of network libraries such as `zmq` and `nng`, they are like utensels. Very precise intstruments to "poke" other applications... Though, with the speed of computing the poke is very fast and can describe large volumes of information. It just costs energy to send the pulse over a network... The speed of transfer is still constrained by the speed of light. But's that's another topic entirely.

More recently, I've been very focused on collapsing what i've learned into something concrete and ready for extended testing.

This is the formulae I'm settling on:
  
    UX = API( Behavior (Data) )

Where:
- `Data` = A stream of input
- `Behavior` = A black-box process the Data flows into based on scalability protocols
- `API` = A well communicated and accessible means for accomplishing your task
- `UX` = A stream of output; both "ah that's a fun game" to "the software made my computer blue screen!"

Note:
- UX is a constantly moving target, refining the API helps engineers; refining the Behavior _may or may not_ help both engineers and users (players)
- API is a fxiture that brings _meaning_ to the intersection of Beahvior / Data. 1 transaction = "an instance of _something_"
- Behavior is a _promise_ when the promise is "broken" the implementation is faulty; plug in a new one (this branch leads out to culture... another topic...)
- Data is _concrete_ this is where the foundation of expansion begins. Data is virtual, or real, it's "something" that must be refined in order to achiev a good UX.

By the way, the above formulae should look familiar if you've ever used Lisp!

Lastly, it's important to point out the elephant in the room. This is a method that enables _collaboration_ so while reading through my explanation **think** about the opportunities that become available when boundaries are removed... This is a pattern of scale because the point is to allow correct solutions to assert themselves into a stable; ever growing virtual system. 

The pattern of 3 (API, Behavior, Data) can be applied orthogonally to itself. Behaviors may reference other Behaviors (which have their own data streams). When streams of Data move it should be thought of as the only real physical means of applying an impulse- It's what we do with that momentum that matters.

## Logger API Specification

    // dumps the log as a D3.js graph
    void  GenerateLogs
  
    // marks a named variable in the log, default time is "now"
    void  LogVariable     name:string, variable:double, time:double=0
  
    // marks a named function in the log, default time is "now"
    void  LogFuncCall     name:string, called:double=1, time:double=0
  
    // marks a named event in the log, default time is "now"
    void  LogEvent        name:string, time:double=0
  
    // appends to a stream of funcName
    void  RecordFuncCall  name:string
  
    // appends to a stream of named variables
    void  RecordVariable  name:string, value:double
  
## Example Output
![alt text](https://raw.githubusercontent.com/scriptslol/Logger/master/image.png "D3.js Graphs")

## Context
I specifically authored this library to quickly drop in a means to profile a game I was working on. The issue we were tracking had to do with the amount of calls coming in/out of the Lua VM. As you can see in the picture, FPS reguarly fellt to high 40 FPS once the game started playing.
  
This tool helped me build consensus through verifiable correlation. As a byproduct; it verified the the fixed tick rate simulation was "mostly" consistent. It's very easy to see where the major issues in hitches is during gameplay and therefore a little easier to track down.
