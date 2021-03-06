// $Id: blockingqueue.java,v 1.3 2013-03-08 13:23:14-08 - - $

//
// Example of the use of a LinkedBlockingQueue.
//

import java.util.*;
import java.util.concurrent.*;
import static java.lang.System.*;

class blockingqueue {

   //
   // Class message includes the String message and a status.
   // NORMAL: usual case for receipt of message
   // TIMEOUT: polling timed out
   // INTERRUPT: some interrupt other than TIMEOUT
   // EOF: end of file
   //
   static enum status_t {NORMAL, TIMEOUT, INTERRUPT, EOF};
   static class message {
      String data;
      status_t status;
      message (String data, status_t status) {
         this.data = data;
         this.status = status;
      }
   };

   //
   // Put thread itself to sleep for a while.
   //
   static void sleep (int msec) {
      try {
         Thread.sleep (msec);
      }catch (InterruptedException exn) {
      }
   }

   //
   // Poll a queue, waiting for some number of milliseconds.
   // Return message or condition.  Used to avoid duplicate
   // code in polling different queues.
   //
   static message poll (BlockingQueue <message> source, long msec) {
      message msg = null;
      try {
         msg = source.poll (msec, TimeUnit.MILLISECONDS);
         if (msg == null) msg = new message (null, status_t.TIMEOUT);
      }catch (InterruptedException exn) {
         msg = new message (null, status_t.INTERRUPT);
      }
      return msg;
   }

   //
   // Put a new element into a blockingqueue, waiting if necessary
   // if the queue is full.  Used to avoid duplicate code.
   //
   static void put (BlockingQueue <message> source, message msg) {
      try {
         source.put (msg);
      }catch (InterruptedException exn) {
         out.printf ("%s%n", exn);
      }
   }

   //
   // Accepts a message from a source queue and distributes it to
   // each of some number of other queues.
   //
   static class distributor implements Runnable {
      BlockingQueue <message> source;
      List <BlockingQueue <message>> targets;
      distributor (BlockingQueue <message> source,
                      List <BlockingQueue <message>> targets) {
         this.source = source;
         this.targets = targets;
      }
      public void run () {
         for (;;) {
            message msg = poll (source, 5000);
            for (BlockingQueue <message> target: targets) {
               put (target, msg);
            }
         }
      }
   }

   //
   // Copies its own queue elements to stdout.  In this example
   // there is a race condition on stdout, so we put the message
   // into a string and then print as a unit.  Note that the order
   // of echoing each message by the several threads varies.
   //
   static class copier implements Runnable {
      BlockingQueue <message> source;
      copier (BlockingQueue <message> source) {
         this.source = source;
      }
      public void run () {
         String name = Thread.currentThread ().getName ();
         for (;;) {
            message msg = poll (source, 5000);
            String line = name + ": " + msg.status;
            if (msg.data != null) line += ": " + msg.data;
            out.println (line);
            if (msg.status == status_t.EOF) return;
         }
      }
   }

   //
   // Create a queue for the distributor, and one of each copier.
   // Start the copiers and distributors.  The copiers run until
   // EOF, but the distributor is a daemon.  Main copies stdin to
   // the distributor until EOF.
   //
   public static void main (String[] args) {
      String[] names
            = {"Pr�tos", "Deuteros", "Tritos", "Tetratos", "Pentos"};

      BlockingQueue <message> source
            = new LinkedBlockingQueue <message> (5);
      List <BlockingQueue <message>> targets
            = new ArrayList <BlockingQueue <message>> ();

      for (int itor = 0; itor < names.length; ++itor) {
         BlockingQueue <message> target
            = new LinkedBlockingQueue <message> (5);
         targets.add (target);
         Thread copy
               = new Thread (new copier (target), names[itor]);
         copy.start ();
      }
      
      Thread distrib
            = new Thread (new distributor (source, targets));
      distrib.setDaemon (true);
      distrib.start ();

      Scanner stdin = new Scanner (in);
      while (stdin.hasNext ()) {
         put (source, new message (stdin.next (), status_t.NORMAL));
      }
      put (source, new message (null, status_t.EOF));
   }

}
