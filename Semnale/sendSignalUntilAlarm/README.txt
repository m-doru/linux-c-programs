Receiver and Sender gets the PID of the other one and :
  -Receiver arms the clock for CLOCK_SECONDS seconds when the first USR1 from Sender is received
  -During the CLOCK_SECONDS receiver counts the number of USR1 signals received from Sender
The 2 programs implement a protocol of exchanging signals:
  -Sender sends to Receiver USR1 signal and Receiver sends back to sender USR2 as a response
  
