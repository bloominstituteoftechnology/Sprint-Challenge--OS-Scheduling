# 1. Name at least three things that a general-purpose operating system is responsible for handling.
    Process management, memory management, booting.

# 2. Describe the job of the Scheduler in the OS in general.
    The scheduler is responsible for controlling when and how processes are managed and executed. Schedulers have strategies for handling multiple queued processes. They execute one process for a short amount of time, before switching to another, and looping through the processes indefinitely. The scheduler is in charge of removing processes that have been completed from the queue.

# 3. Describe the benefits of the MLFQ over a plain Round-Robin scheduler.
    MLFQ prioritizes based on time needed, executing smaller processes quicker, and Red Robin gives each process equal time and priority.
