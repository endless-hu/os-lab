code Main

  -- OS Class: Project 3
  --
  -- Hu Zhijing
  --

  -- This package contains the following:
  --     Dining Philospohers
-----------------------------  Main  ---------------------------------

  function main ()
      InitializeScheduler ()
      DiningPhilosophers ()
      ThreadFinish ()
    endFunction




-----------------------------  Dining Philosophers  ----------------------------

  -- This code is an implementation of the Dining Philosophers problem.  Each
  -- philosopher is simulated with a thread.  Each philosopher thinks for a while
  -- and then wants to eat.  Before eating, he must pick up both his forks.
  -- After eating, he puts down his forks.  Each fork is shared between
  -- two philosophers and there are 5 philosophers and 5 forks arranged in a
  -- circle.
  --
  -- Since the forks are shared, access to them is controlled by a monitor
  -- called "ForkMonitor".  The monitor is an object with two "entry" methods:
  --     PickupForks (phil)
  --     PutDownForks (phil)
  -- The philsophers are numbered 0 to 4 and each of these methods is passed an 
  -- integer indicating which philospher wants to pickup (or put down) the forks.
  -- The call to "PickUpForks" will wait until both of his forks are
  -- available.  The call to "PutDownForks" will never wait and may also
  -- wake up threads (i.e., philosophers) who are waiting.
  --
  -- Each philospher is in exactly one state: HUNGRY, EATING, or THINKING.  Each
  -- time a philosopher's state changes, a line of output is printed.  The 
  -- output is organized so that each philosopher has column of output with the
  -- following code letters:
  --           E    --  eating
  --           .    --  thinking
  --         blank  --  hungry (i.e., waiting for forks)
  -- By reading down a column, you can see the history of a philosopher.
  --
  -- The forks are not modeled explicitly.  A fork is only picked up
  -- by a philosopher if he can pick up both forks at the same time and begin
  -- eating.  To know whether a fork is available, it is sufficient to simply
  -- look at the status's of the two adjacent philosophers.  (Another way to 
  -- state the problem is to forget about the forks altogether and stipulate 
  -- that a philosopher may only eat when his two neighbors are not eating.)

  enum HUNGRY, EATING, THINKING
  var
    mon: ForkMonitor
    philospher: array [5] of Thread = new array of Thread {5 of new Thread }

  function DiningPhilosophers ()

      print ("Plato\n")
      print ("    Sartre\n")
      print ("        Kant\n")
      print ("            Nietzsche\n")
      print ("                Aristotle\n")

      mon = new ForkMonitor
      mon.Init ()
      mon.PrintAllStatus ()

      philospher[0].Init ("Plato")
      philospher[0].Fork (PhilosphizeAndEat, 0)

      philospher[1].Init ("Sartre")
      philospher[1].Fork (PhilosphizeAndEat, 1)

      philospher[2].Init ("Kant")
      philospher[2].Fork (PhilosphizeAndEat, 2)

      philospher[3].Init ("Nietzsche")
      philospher[3].Fork (PhilosphizeAndEat, 3)

      philospher[4].Init ("Aristotle")
      philospher[4].Fork (PhilosphizeAndEat, 4)

     endFunction

  function PhilosphizeAndEat (p: int)
    -- The parameter "p" identifies which philosopher this is.
    -- In a loop, he will think, acquire his forks, eat, and
    -- put down his forks.
      var
        i: int
      for i = 1 to 7
        -- Now he is thinking
        mon.PickupForks (p)
        -- Now he is eating
        mon.PutDownForks (p)
      endFor
    endFunction

  class ForkMonitor
    superclass Object
    fields
      status: array [5] of int -- For each philosopher: HUNGRY, EATING, or THINKING
      forks: array [5] of Mutex -- forks[0] is the left fork of philosopher[0]
      mu: Mutex -- Global Lock. Must be held when we access or change status
    methods
      Init ()
      PickupForks (p: int)
      PutDownForks (p: int)
      PrintAllStatus ()
  endClass

  behavior ForkMonitor

    method Init ()
      -- Initialize so that all philosophers are THINKING.
      var 
        i: int
      status = new array of int {5 of THINKING }
      forks = new array of Mutex {5 of new Mutex }
      mu = new Mutex
      mu.Init()
      for i = 0 to 4
        forks[i].Init()
      endFor
      endMethod

    method PickupForks (p: int)
      -- This method is called when philosopher 'p' wants to eat.
      mu.Lock()
      status[p] = HUNGRY
      self.PrintAllStatus()
      mu.Unlock()

      if p != 4
        -- First left, then right
        forks[p].Lock()
        forks[(p+1)%5].Lock()
      else
        -- First right, then left
        forks[0].Lock()
        forks[4].Lock()
      endIf

      mu.Lock()
      status[p] = EATING
      self.PrintAllStatus()
      mu.Unlock()

      endMethod

    method PutDownForks (p: int)
      -- This method is called when the philosopher 'p' is done eating.
      if !forks[p].IsHeldByCurrentThread() || !forks[(p+1)%5].IsHeldByCurrentThread()
        FatalError ("Attempt to unlock a mutex by a thread not holding it")
      endIf

      forks[p].Unlock()
      forks[(p+1)%5].Unlock()

      mu.Lock()
      status[p] = THINKING
      self.PrintAllStatus()
      mu.Unlock()
      
      endMethod

    method PrintAllStatus ()
      -- Print a single line showing the status of all philosophers.
      --      '.' means thinking
      --      ' ' means hungry
      --      'E' means eating
      -- Note that this method is internal to the monitor.  Thus, when
      -- it is called, the monitor lock will already have been acquired
      -- by the thread.  Therefore, this method can never be re-entered,
      -- since only one thread at a time may execute within the monitor.
      -- Consequently, printing is safe.  This method calls the "print"
      -- routine several times to print a single line, but these will all
      -- happen without interuption.
        var
          p: int
        for p = 0 to 4
          switch status [p]
            case HUNGRY:
              print ("    ")
              break
            case EATING:
              print ("E   ")
              break
            case THINKING:
              print (".   ")
              break
          endSwitch
        endFor
        nl ()
      endMethod

  endBehavior

endCode
