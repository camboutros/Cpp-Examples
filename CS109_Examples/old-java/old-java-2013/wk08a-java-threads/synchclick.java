// $Id: synchclick.java,v 1.1 2012-05-23 21:31:53-07 - - $

//
// Synchronized clicking of a counter.
// When one thread enters counter, the other must wait.
//

import static java.lang.System.*;

class synchclick {

   static class counter {
      private int count = 0;
      synchronized void click() {
         ++count;
      }
   }

   static class synchr implements Runnable {
      int ident;
      counter counted;
      synchr (int idinit, counter countinit) {
         ident = idinit;
         counted = countinit;
      }
      public void run() {
         out.printf ("synchr %d starting%n", ident);
         out.flush();
         for (int itor = 0; itor < 1000000; ++itor) counted.click();
         out.printf ("synchr %d finished, count = %d%n",
                     ident, counted.count);
         out.flush();
      }
   }

   public static void main (String[] args) {
      out.printf ("main starting%n");
      out.flush();
      counter counted = new counter();
      Thread synchr1 = new Thread (new synchr (1, counted));
      Thread synchr2 = new Thread (new synchr (2, counted));
      synchr1.start();
      synchr2.start();
      out.printf ("main finished, count = %d%n", counted.count);
      out.flush();
   }

}

//TEST// runprog -x synchclick.lis synchclick
//TEST// mkpspdf synchclick.ps synchclick.java* synchclick.lis

