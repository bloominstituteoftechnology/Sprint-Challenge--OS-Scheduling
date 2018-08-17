# Sprint Challenge: Operating Systems and Scheduling

## Short Answer Questions

Add your answers inline, below, with your pull request.

> 1. Name at least three things that a general-purpose operating system is responsible for handling.

* Manage access and usage of computer resources.

That is, the OS manages access and usage of the CPU, Memory, and I/O. The OS kernel handles this responsibility. The kernel uses scheduling to manage CPU, allocation & paging to manage memory, and drivers to handle I/O.

* Provide a GUI.

When a user watches a YouTube video, or opens a word processor to work on a paper, that user does not necessarily write machine code to access where the program is stored and queue it for execution by the CPU. The operating system does all this nitty gritty for the user, and all these crazy nitty gritty is abstracted away through a *Graphical User Interface*. The OS translates inputs received in the GUI to appropriate commands and calls to be executed by the computer, and the outputs of the commands and calls are translated by the OS, through the GUI, to something the user can understand.

* Creates and maintains a file system.

The OS creates and maintains a file system so data that needs to be stored on long-term storage (*e.g. hard disk*) can accessed and manipulated efficiently.

<u>*Sources Used*</u><br/>
[Operating Systems: Role of the operating system](https://en.wikibooks.org/wiki/A-level_Computing/AQA/Problem_Solving,_Programming,_Operating_Systems,_Databases_and_Networking/Operating_Systems/Role_of_the_operating_system)
[Operating System Roles & Responsibilities](https://thetechnicaltutorial.wordpress.com/2016/04/21/operating-system-roles-responsibilities/)

> 2. Describe the job of the Scheduler in the OS in general.

The job of the scheduler is to manage process usage of the CPU. This seems pretty mundane, but it is in fact pretty amazing, for it allows the sensation of "multi-tasking". 

Remember, a CPU can generally run only one thing at a time. Yet our devices appear to be running multiple processes simultaneously, often many more processes than there are CPU's. This is possible because the scheduler rapidly switches between processes to be executed by the CPU. 

So, in a simple example, if there are three processes running "simultaneously", the scheduler will let the CPU execute one process for some time, then swap it with the second one, and have the CPU execute that for some time, then swap it with the third one and have the CPU execute it for some time. The scheduler will continue this process rotation until the processes are complete.

> 3. Describe the benefits of the MLFQ over a plain Round-Robin scheduler.

The example I detailed in question 2 is an example of a plain "Round-Robin scheduler". This gets the job done, but has a few shortcomings. The MLFQ addresses these with these benefits:

* Priority

There is no clear way to denote priority in a "Round-Robin" configuration; all processes are in the same queue and are rapidly switched between them in FIFO order. This is fair, but in an environment in which new processes are added to to the queue all the time, shorter processing time processes may have to wait longer and longer for their turn as the queue gets bigger and bigger. This will often mean an action the user expectes to take little time to execute may end up taking a long time to execute as it waits in the queue to be executed, leading to a feeling of "sluggishness" the user experiences.

MLFQ addresses that by putting processes in different priority queues. This allows quick processes to execute quickly and reduces average waiting time.

* Maintains Fairness

The "Round-Robin" is more fair than the MLFQ, but the MLFQ uses some neat tricks to strike a balance between fairness and wait-times. To ensure fairness (and prevent the inverse of the above happening; that is, longer processes just sitting around forever as short processes get continuously picked and executed), several techniques are employed, such as allowing more execution time for lower priority queues, and shaking up the whole priority queue every now and then and have all the processes have a chance to execute before reprioritizing them. 

Another technique is to gradually increase priority for processes that have been waiting to be executed so that these processes do not become "*CPU-starved*", a state which a process does not ever receive the CPU execution time it needs to complete.

<u>*Sources Used*</u><br/>
[MLFQ Scheduling](https://intro2operatingsystems.wordpress.com/tag/mlfq-scheduling/)

## Programming Exercise: The Lambda School Shell (`lssh`)

Important Safety Tip: Resist the urge to start coding until you:
1. Read this complete challenge

then

2. Inventory the code and figure out what needs to be written where.

### Task 1: Implement the Ability to Execute Arbitrary Commands
This program implements a new shell that you can use to run commands from in Unix, similar to bash!

At the end of the day, you should be able to run your shell, then run commands within it like in the following example.

**NOTE: you do not need to implement the `ls` or `head` commands! These already exist in Unix. Your goal is to write a program that runs them with `exec()` when
the user types in their names. And also the shell should be able to run _any_ command, not just `ls` and `head`.**

```
[bash]$ ./lssh 
lambda-shell$ ls -l
total 32
-rwxr-xr-x  1 beej  staff  9108 Mar 15 13:28 lssh
-rw-r--r--  1 beej  staff  2772 Mar 15 13:27 lssh.c
lambda-shell$ head lssh.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define PROMPT "lambda-shell$ "

#define MAX_TOKENS 100
#define COMMANDLINE_BUFSIZE 1024
#define DEBUG 0  // Set to 1 to turn on some debugging output
lambda-shell$ exit
[bash]$ 
```

General plan of attack is to:

* Loop until the user exits.
  * Print a prompt.
  * Read a line of input from the keyboard.
  * Parse that line down into individual space-separated parts of the command.
  * Fork a child process to run the new command.
  * Exec the command in the child process.
  * Parent process waits for child to complete.

Most of the shell's boilerplate is already written, but you'll have to implement the logic of it.

Examine the `lssh.c` file and:

* Determine which parts of the program do what.
* Read the header comments.
* Find the parts you need to implement.
* Come up with an individual plan of attack for each of those parts.
* Determine how to exit the shell.
* What happens if you build it?
* What happens if you run it?

Hint: you probably want one of the `exec` variants that has the `p` suffix on it so that the `PATH` will be searched for the file you're trying to run. Choose
your `exec` variant carefully. Some will be hard-to-impossible to use. Some will be easy.

When you get this working, _you will have your own shell_. It's not as full-featured as bash by any means, but it is a legitimate shell and the core is the same as any other shell.

If you finish early, look at extra credit to start implementing more features.

### Task 2: Implement the Ability to Change Directories with `cd`

In bash, you can change directories with the built-in `cd` command.

Each process keep track of which directory it is running in, and the shell is no exception. Each process can change its current directory, as well.

> Why is `cd` built into the shell? Why can't it run as an external command?

Because it is built in, you should check to see if the user entered `cd` in `args[0]` _before_ running the command. And if they did, you should short-circuit the rest of the main loop with a `continue` statement.

> Look at the implementation of the built-in `exit` command for inspiration.

You can use the program `pwd` to see what directory you are in.

Example run:

```
lambda-shell$ pwd
/Users/example
lambda-shell$ cd src
lambda-shell$ pwd
/Users/example/src
lambda-shell$ cd ..
lambda-shell$ pwd
/Users/example
lambda-shell$ cd foobar
chdir: No such file or directory
lambda-shell$ 
```

If the user entered `cd` as the first argument:

1. Check to make sure they've entered 2 total arguments
2. Run the system call `chdir()` on the second argument to change directories
3. Error check the result of `chdir()`. If it returns `-1`, meaning an error
   occurred, you can print out an error message with:
   ```
   perror("chdir"); // #include <errno.h> to use this
   ```
4. Execute a `continue` statement to short-circuit the rest of the main loop.

Note that `.` and `..` are actual directories. You don't need to write any special case code to handle them.

Be sure to think about where this functionality should live in the file. The order of execution of the code matters!

### Stretch Goal 1: Background Tasks

In bash, you can run a program in the background by adding an `&` after the command.

```
ls -la &
```

Add similar functionality to `lssh` by checking to see if the last argument is an `&`.

If it is:

1. Strip the `&` off the `args` (by setting that pointer to `NULL`).
2. Run the command in the child as usual.
3. Prevent the parent from `wait()`ing for the child to complete. Just give a
   new prompt immediately. The child will continue to run in the background.

In every instance of the main loop after the user hits `RETURN`, you should wait in a loop to reap any background zombies that have died in the meantime. You can use a loop do this with without blocking like so:

```c
// Wait for any other processes that have ended in the
// meantime. A more correct solution would be to listen for the
// SIGCHLD signal and wait for zombies at that point.
while (waitpid(-1, NULL, WNOHANG) > 0)
    ;
```

The above (empty) loop will just call `waitpid()` repeatedly until there are no more zombies to reap. Then the program continues on.

What happens if you don't do this?

Mini stretch challenge: wait for the `SIGCHLD` signal and put the above `while` loop in there instead.

Note that you might get weird output when doing this, like the prompt might appear before the program completes, or not at all if the program's output overwrites it. If it looks like it hangs at the end, just hit `RETURN` to get
another prompt back.


### Stretch Goal 2: File Redirection

In bash, you can redirect the output of a program into a file with `>`. This creates a new file and puts the output of the command in there instead of writing it to the screen.

```
ls -l > foo.txt
```

Check the `args` array for a `>`. If it's there:

1. Get the output file name from the next element in `args`.
2. Strip everything out of `args` from the `>` on. (Set the `args` element with
   the `>` in it to `NULL`).
3. In the child process:
    1. `open()` the file for output. Store the resultant file descriptor in a
       variable `fd`.
    2. Use the `dup2()` system call to make `stdout` (file descriptor `1`) refer
       to the newly-opened file instead:

	    ```c
		int fd = open(...
		dup2(fd, 1);  // Now stdout goes to the file instead
		```
	3. `exec` the program like normal.

Note that depending on your [`umask`](https://en.wikipedia.org/wiki/Umask), the
newly-created file might not be actually readable by you. If you can't read it, run `chmod 600 filename` on the new file and then you'll have permission.

### Stretch Goal 3: Pipes

In bash, you can pipe the output of one command into the input of another with the `|` symbol.

For example, this takes the output of `ls -l` and uses it as the input for `wc -l` (which counts the number of lines in the input):

```
ls -l | wc -l
```

This uses the `pipe()` system call.

Use a process similar to the above extra credit challenges, along with [this
description of how to implement pipes in
C](https://github.com/LambdaSchool/CS-Wiki/wiki/How-Unix-Pipes-are-Implemented) to get pipes implemented in `lssh`.
