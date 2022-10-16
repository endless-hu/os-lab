header Main

  uses System, Thread, Synch

  const
    nrCustomers = 10
    nrChairs = 5

  functions
    main ()

  class SleepingBarber
    superclass Object
    fields
      barberStatus: int
      customerStatus: ptr to array of char
      availChairs: int
      -------- Additional member variables
      barberSema: Semaphore -- Number of free barbers
      barberDone: Semaphore
      customerSema: Semaphore  -- Number of customers waiting for service
      mu: Mutex             -- global lock to protect shared variables
    methods
      Init(bStat: int, cStat: ptr to array of char, numOfChair: int)
      printExample()
      Barber()
      NewCustomer(customer: int)
      printBarberStatus()
      printCustomerStatus(customer: int)
      printChairs()
  endClass
endHeader
