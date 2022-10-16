code Main

  -- OS Class: Project 3
  --
  -- Hu Zhijing
  --

  -- This package contains the following:
  --     Gaming Parlor
-----------------------------  Main  ---------------------------------

  function main ()
      InitializeScheduler ()
      gamingparlor ()
      ThreadFinish ()
    endFunction

------------------------------- Gaming Parlor ----------------------------------
  var
    GP: GamingParlor = new GamingParlor
    -- Hint: Some variables are defined in "Task3.h".
    playersThread: array [total_groups] of Thread = 
             new array of Thread {total_groups of new Thread }

  function gamingparlor()
    var
      i: int

    GP.Init()

    -- Remove the following line in your implementation
    -- This is only an example.
    -- GP.PrintExample()

    -- Add more code below

    -- Lauch `total_groups` threads to simulate each group
    for i = 0 to total_groups-1
      playersThread[i].Init("player")
      playersThread[i].Fork(play, i)
    endFor
  
  endFunction

  function play(player: int)
    var 
      i: int
    
    for i = 0 to 5   -- play 5 times
      GP.Request(group_names[player], dice_per_group[player])
      currentThread.Yield()  -- // play for a while
      GP.Return(group_names[player], dice_per_group[player])
    endFor
  endFunction



  behavior GamingParlor

    method Init()
      dice_available = total_dice
      mu = new Mutex
      mu.Init()
      cond = new Condition
      cond.Init()
    endMethod

    method Request(name: char, nr_of_dice: int)
      self.mu.Lock()
      self.Print(name, "requests", nr_of_dice)
      while dice_available < nr_of_dice
        self.cond.Wait(&self.mu)   -- No enough dice now, relinquish the lock and go to sleep
      endWhile
      dice_available = dice_available - nr_of_dice
      self.Print(name, "proceeds with", nr_of_dice)
      self.mu.Unlock()
    endMethod

    method Return(name: char, nr_of_dice: int)
      self.mu.Lock()
      dice_available = dice_available + nr_of_dice
      self.Print(name, "releases and adds back", nr_of_dice)
      self.cond.Broadcast(&self.mu)
      self.mu.Unlock()
    endMethod

    method PrintExample()
      ----------------------------- Example -----------------------------
      -- This is an example of using print functions.
      -- You should not use this function in your implementation.

      -- A requests
      self.Request(group_names[0], 4)
      -- B requests
      self.Request(group_names[1], 4)
      -- B returns
      self.Return(group_names[1], 4)
      -- A returns
      self.Return(group_names[0], 4)
      -- C requests
      self.Request(group_names[2], 5)
      -- C returns
      self.Return(group_names[2], 5)

    endMethod

    method Print(name: char, str: String, nr_of_dice: int)
    --
    -- This method prints the current thread's' name and the arguments.
    -- It also prints the current number of dice available.
    --
       printChar(name)
       print(" ")
       print(str)
       print(" ")
       printInt(nr_of_dice)
       nl()
       print("------------------------------Number of dice now avail = ")
       printInt(dice_available)
       nl()
    endMethod
  endBehavior

endCode
