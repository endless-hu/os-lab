code Main

  -- OS Class: Project 3
  --
  -- Hu Zhijing
  --

  -- This package contains the following:
  --     Sleeping Barber
-----------------------------  Main  ---------------------------------

  function main ()
      InitializeScheduler ()
      sleepingbarber ()
      ThreadFinish ()
    endFunction

-----------------------------  Sleeping Barber --------------------------------
  -- In the Producer/Consumer and Dining Philosophers problems, we tried to print the activity
  -- of each thread in a different column so that you could see what each thread was doing.
  -- The actual events in this problem can be reduced to a single letter, such as:
  --
  --         E = Enter
  --         S = Sit in waiting chair
  --         B = Begin haircut
  --         F = Finish haircut
  --         L = Leave
  --         start = Barber begins haircut
  --         end = Barber ends haircut
  --
  -- The status of the chairs should be printed every time you print a line. If there are 5
  -- chairs, of which 2 are occupied, print 2 X's and 3 -'s.  For example:
  --
  --         XX---
  --

  enum Start, End

  var
    sb: SleepingBarber
    bStatus: int = End
    cStatus: array[nrCustomers] of char = 
             new array of char { nrCustomers of 'L' }
    -- Hint: Some variables are defined in "Task2.h".
    customers: array [nrCustomers] of Thread = 
               new array of Thread {nrCustomers of new Thread }
    barberThread: Thread = new Thread

  function sleepingbarber()
    var
      i: int

    -- print initial line
    for i = 0 to nrChairs
        printChar(' ')
    endFor
    print("Barber\t")
    for i = 0 to nrCustomers - 1
        printInt(i+1)
        printChar('\t')
    endFor
    nl()

    sb = new SleepingBarber
    sb.Init(bStatus, &cStatus, nrChairs)

    -- Remove the following line in your implementation
    -- This is only an example.
    -- sb.printExample()

    -- Add more code below.
    barberThread.Init("BarberThread begins")
    barberThread.Fork(Barber, 11)
    for i = 0 to nrCustomers -1
      customers[i].Init("")
      customers[i].Fork(newCustomer, i)
    endFor
  endFunction

  function Barber(unused: int)
    sb.Barber()
  endFunction

  function newCustomer(c: int)
    var
      i: int
  
    for i = 1 to 3
      sb.NewCustomer(c)
    endFor
  endFunction

  -- implementation of SleepingBarber class
  behavior SleepingBarber
    method Init(bStat: int, cStat: ptr to array of char, numOfChair: int)
      self.barberStatus = bStat
      self.customerStatus = cStat
      self.availChairs = numOfChair

      self.barberSema = new Semaphore
      self.barberSema.Init(0)
      self.barberDone = new Semaphore
      self.barberDone.Init(0)
      self.customerSema = new Semaphore
      self.customerSema.Init(0)
      self.mu = new Mutex
      self.mu.Init()
    endMethod

    method Barber()
      while true 
        self.customerSema.Down()
        self.mu.Lock()
        self.availChairs = self.availChairs + 1
        self.barberStatus = Start
        self.printBarberStatus()
        self.barberSema.Up()
        self.mu.Unlock()

        -- do hair cut
        self.barberDone.Down()

        ------ hair cut finished ------
      endWhile
    endMethod

    method NewCustomer(customer: int)
      self.mu.Lock()
      self.customerStatus[customer] = 'E'
      self.printCustomerStatus(customer)
      if self.availChairs == 0 
        -- shop full, leave now
        self.customerStatus[customer] = 'L'
        self.printCustomerStatus(customer)
        self.mu.Unlock()
        currentThread.Yield()
      else
        -- Sit down and waiting for service
        self.availChairs = self.availChairs - 1
        self.customerStatus[customer] = 'S'
        self.printCustomerStatus(customer)
        self.customerSema.Up()  -- wake up barber if necessary
        self.mu.Unlock()

        self.barberSema.Down() -- Go to sleep if no free barbers

        self.mu.Lock()
        self.customerStatus[customer] = 'B'
        self.printCustomerStatus(customer)
        self.mu.Unlock()

        currentThread.Yield()  -- Get hair cut

        self.mu.Lock()
        self.barberDone.Up()      -- Finished
        self.customerStatus[customer] = 'F'
        self.printCustomerStatus(customer)
        self.barberStatus = End
        self.printBarberStatus()
        self.mu.Unlock()

        self.mu.Lock()
        self.customerStatus[customer] = 'L'
        self.printCustomerStatus(customer)
        self.mu.Unlock()
        
      endIf

    endMethod

    method printExample()
      ----------------------------- Example -----------------------------
      -- This is an example of using print functions.
      -- You should not use this function in your implementation.

      -- Customer 1 enter.
      self.customerStatus[0] = 'E'
      self.printCustomerStatus(0)

      -- Customer 1 sit in waiting chair.
      self.customerStatus[0] = 'S'
      self.availChairs = self.availChairs - 1
      self.printCustomerStatus(0)

      -- Customer 2 enter.
      self.customerStatus[1] = 'E'
      self.printCustomerStatus(1)

      -- Customer 2 sit in waiting chair.
      self.customerStatus[1] = 'S'
      self.availChairs = self.availChairs - 1
      self.printCustomerStatus(1)

      -- Barber begins haircut. Customer begins haircut and chair becomes available.
      self.barberStatus = Start
      self.availChairs = self.availChairs + 1
      self.printBarberStatus()

      -- Customer 1 begins haircut.
      self.customerStatus[0] = 'B'
      self.printCustomerStatus(0)

      -- Barber ends haircut.
      self.barberStatus = End
      self.printBarberStatus()

      -- Customer 1 finish haircut.
      self.customerStatus[0] = 'F'
      self.printCustomerStatus(0)

      -- Customer 1 leave.
      self.customerStatus[0] = 'L'
      self.printCustomerStatus(0)

      -- Barber begins haircut. Customer begins haircut and chair becomes available.
      self.barberStatus = Start
      self.availChairs = self.availChairs + 1
      self.printBarberStatus()

      -- Customer 2 begins haircut.
      self.customerStatus[1] = 'B'
      self.printCustomerStatus(1)

      -- Barber ends haircut.
      self.barberStatus = End
      self.printBarberStatus()

      -- Customer 2 finish haircut.
      self.customerStatus[1] = 'F'
      self.printCustomerStatus(1)

      -- Customer 2 leave.
      self.customerStatus[1] = 'L'
      self.printCustomerStatus(1)
      -----------------------------  End  -----------------------------
    endMethod

    method printBarberStatus()
      self.printChairs()
      if self.barberStatus == Start
         print("Start\n")
      else
         print("End\n")
      endIf
    endMethod

    method printCustomerStatus(customer: int)
      var i: int
      self.printChairs()
      print("      \t")
      for i = 0 to nrCustomers - 1
        if (i != customer)
          printChar(' ')
        else
          printChar(self.customerStatus[i])
        endIf
        printChar('\t')
      endFor
      nl()
    endMethod

    method printChairs()
      var i: int
      for i = self.availChairs + 1 to nrChairs
        printChar('X')
      endFor
      for i = 1 to self.availChairs
        printChar('-')
      endFor
      printChar(' ')
    endMethod

  endBehavior

endCode
